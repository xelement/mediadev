#include "PicViewCtrl.h"
#include "MainFrame.h"
#include <algorithm>
#include <cmath>

extern const wxEventType wxEVT_YUVBUFFER_CHANGED;
extern const wxEventType wxEVT_POSITION_CHANGED;
extern const wxEventType wxEVT_DROP_HEVCFILES;

IMPLEMENT_DYNAMIC_CLASS(PicViewCtrl, wxControl);
//IMPLEMENT_DYNAMIC_CLASS(PicViewCtrl, wxFileDropTarget);

BEGIN_EVENT_TABLE(PicViewCtrl, wxControl)
    EVT_PAINT(PicViewCtrl::OnPaint)
    EVT_ERASE_BACKGROUND(PicViewCtrl::OnEraseBkg)
    EVT_MOTION(PicViewCtrl::OnMouseMove)
    EVT_LEFT_DOWN(PicViewCtrl::OnMouseLButtonDown)
    EVT_LEFT_UP(PicViewCtrl::OnMouseLButtonUp)
    EVT_MOUSEWHEEL(PicViewCtrl::OnMouseWheel)
    EVT_KEY_DOWN(PicViewCtrl::OnKeyDown)
END_EVENT_TABLE()

PicViewCtrl::PicViewCtrl(wxWindow* parent, wxWindowID id, wxSimpleHtmlListBox* pList, RulerCtrl* pHRuler, RulerCtrl* pVRuler,
            wxWindow* pPixelCtrl, wxFrame* pFrame)
    : wxControl(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxWANTS_CHARS),
    m_bClearFlag(true), m_bFitMode(true), m_dScaleRate(1.0), m_dMinScaleRate(0.1), m_dMaxScaleRate(2.0), m_dFitScaleRate(1.0),
    m_dScaleRateStep(0.02), m_delta(-1, -1), m_curLCUStart(-1, -1), m_curLCUEnd(-1, -1), m_iLCURasterID(-1), m_pList(pList),
    m_pFrame(pFrame), m_bShowGrid(true), m_bOpenedYUVfile(true), m_bShowTilesInfo(true), m_bMouseWheelPageUpDown(false), m_bShowPUType(true), m_pBuffer(NULL),
    m_iYUVBit(8), m_iShowWhich_O_Y_U_V(MODE_ORG), m_pHRuler(pHRuler), m_pVRuler(pVRuler), m_bFullRefresh(true),
    m_pPixelCtrl(pPixelCtrl), m_iSelectedLCUId(-1), m_curSelLCUStart(-1, -1), m_curSelLCUEnd(-1, -1),
    m_piRowData(NULL), m_piColData(NULL)
{
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    m_pDragDropFile = new DragDropFile();
    m_pDragDropFile->setFrameWindow(m_pFrame);
    SetDropTarget(m_pDragDropFile);
}

PicViewCtrl::~PicViewCtrl()
{
    if(m_piRowData)
        delete [] m_piRowData;
    if(m_piColData)
        delete [] m_piColData;
}

void PicViewCtrl::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    PrepareDC(dc);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    wxGraphicsContext *gct = wxGraphicsContext::Create(dc);
    if(m_bFullRefresh && !m_bClearFlag)
    {
        m_rectRefresh.x = 0;
        m_rectRefresh.y = 0;
        m_rectRefresh.width  = m_cViewBitmap.GetWidth();
        m_rectRefresh.height = m_cViewBitmap.GetHeight();
    }
    Render(gc, gct);
    delete gc;
    delete gct;
}

void PicViewCtrl::Render(wxGraphicsContext* gc, wxGraphicsContext* gct)
{
    if(!m_bClearFlag)
    {
        gc->Scale(m_dScaleRate, m_dScaleRate);
        DrawBackGround(gc, gct);
        m_pHRuler->SetTagValue(m_curLCUStart.x);
        m_pVRuler->SetTagValue(m_curLCUStart.y);
        gct->Scale(1, 1);
        gct->SetBrush(wxBrush(wxColor(255, 255, 255, 100)));
        gct->SetPen(wxPen(wxColor(255, 0, 0, 255)));
        gct->StrokeLine(0, m_curLCUStart.y*m_dScaleRate, m_curLCUStart.x*m_dScaleRate, m_curLCUStart.y*m_dScaleRate);
        gct->StrokeLine(m_curLCUStart.x*m_dScaleRate, 0, m_curLCUStart.x*m_dScaleRate, m_curLCUStart.y*m_dScaleRate);
        gct->SetPen(wxPen(wxColor(255, 255, 255, 100)));
        gct->DrawRectangle(m_curLCUStart.x*m_dScaleRate, m_curLCUStart.y*m_dScaleRate, (m_curLCUEnd.x - m_curLCUStart.x)*m_dScaleRate,
                           (m_curLCUEnd.y - m_curLCUStart.y)*m_dScaleRate);
        if(m_iSelectedLCUId < 0)
        {
            m_iSelectedLCUId = 0;
            CalStartEndPointByLCUId(m_iSelectedLCUId, m_curSelLCUStart, m_curSelLCUEnd);
            m_PosData._iBlockX      = m_curSelLCUStart.x;
            m_PosData._iBlockY      = m_curSelLCUStart.y;
            m_PosData._iBlockWidth  = m_curSelLCUEnd.x - m_curSelLCUStart.x;
            m_PosData._iBlockHeight = m_curSelLCUEnd.y - m_curSelLCUStart.y;
            m_PosData._iOffsetX     = 0;
            m_PosData._iOffsetY     = 0;
            wxCommandEvent event(wxEVT_POSITION_CHANGED, wxID_ANY);
            event.SetClientData(&m_PosData);
            wxPostEvent(m_pPixelCtrl, event);
        }
        m_pHRuler->SetTagValue2(m_curSelLCUStart.x);
        m_pVRuler->SetTagValue2(m_curSelLCUStart.y);
        gct->SetPen(wxPen(wxColor(250, 5, 243, 128)));
        gct->StrokeLine(0, m_curSelLCUStart.y*m_dScaleRate, m_curSelLCUStart.x*m_dScaleRate, m_curSelLCUStart.y*m_dScaleRate);
        gct->StrokeLine(m_curSelLCUStart.x*m_dScaleRate, 0, m_curSelLCUStart.x*m_dScaleRate, m_curSelLCUStart.y*m_dScaleRate);
        gct->DrawRectangle(m_curSelLCUStart.x*m_dScaleRate, m_curSelLCUStart.y*m_dScaleRate, (m_curSelLCUEnd.x - m_curSelLCUStart.x)*m_dScaleRate,
                       (m_curSelLCUEnd.y - m_curSelLCUStart.y)*m_dScaleRate);
    }
    else
        DrawNoPictureTips(gc);
    m_bFullRefresh = true;
}

void PicViewCtrl::SetBitmap(wxBitmap bitmap, wxBitmap bitmapY, wxBitmap bitmapU, wxBitmap bitmapV)
{
    m_bClearFlag   = false;
    m_cViewBitmap  = bitmap;
    m_cViewBitmapY = bitmapY;
    m_cViewBitmapU = bitmapU;
    m_cViewBitmapV = bitmapV;
    CalFitScaleRate();
    m_CtrlSize.SetWidth(m_dScaleRate*m_cViewBitmap.GetWidth());
    m_CtrlSize.SetHeight(m_dScaleRate*m_cViewBitmap.GetHeight());
    this->SetSizeHints(m_CtrlSize);
    GetParent()->FitInside();
    SetRulerCtrlFited();
    CalCurScrolledRectOnPicView(m_rectRefresh);
    Refresh();
}

void PicViewCtrl::SetScale(const double dScale)
{
    m_dScaleRate = dScale;
}

void PicViewCtrl::SetLCUSize(const wxSize& size)
{
    m_LCUSize = size;
}

void PicViewCtrl::OnMouseMove(wxMouseEvent& event)
{
    if(!m_bClearFlag)
    {
        SetFocus();
        wxPoint pt = event.GetPosition();
        if(event.Dragging() && event.LeftIsDown())
        {
            m_bFullRefresh = true;
            int xper, yper;
            wxPoint pos = ClientToScreen(pt);
            wxScrolledWindow* pPar = (wxScrolledWindow*)GetParent();
            pPar->GetScrollPixelsPerUnit(&xper, &yper);
            pPar->Scroll((m_delta.x - pos.x)/xper, (m_delta.y - pos.y)/yper);
            SetRulerCtrlFited();
        }
        m_bFullRefresh = false;
        int posx = event.m_x/m_dScaleRate;
        int posy = event.m_y/m_dScaleRate;
        posx     = max(0, posx);
        posy     = max(0, posy);
        int id = GetCurLCURasterID(posx, posy, m_curLCUStart, m_curLCUEnd);
        if(id == m_iSelectedLCUId)
        {
            m_PosData._iBlockX      = m_curLCUStart.x;
            m_PosData._iBlockY      = m_curLCUStart.y;
            m_PosData._iBlockWidth  = m_curLCUEnd.x - m_curLCUStart.x;
            m_PosData._iBlockHeight = m_curLCUEnd.y - m_curLCUStart.y;
            m_PosData._iOffsetX     = posx % m_LCUSize.x;
            m_PosData._iOffsetY     = posy % m_LCUSize.y;
            wxCommandEvent event(wxEVT_POSITION_CHANGED, wxID_ANY);
            event.SetClientData(&m_PosData);
            wxPostEvent(m_pPixelCtrl, event);
        }
        if(id != m_iLCURasterID)
        {
            CalCurScrolledRectOnPicView(m_rectRefresh);
            m_iLCURasterID = id;
            if(id != -1)
            {
                Refresh();
            }
        }
    }
}

void PicViewCtrl::OnEraseBkg(wxEraseEvent& event)
{
}

int PicViewCtrl::GetCurLCURasterID(const double x, const double y, wxPoint& start, wxPoint& end)
{
    int cuw = m_LCUSize.GetWidth();
    int cuh = m_LCUSize.GetHeight();
    int col_num = m_cViewBitmap.GetWidth()/cuw + (m_cViewBitmap.GetWidth()%cuw != 0);
    int row_num = m_cViewBitmap.GetHeight()/cuh + (m_cViewBitmap.GetHeight()%cuh != 0);
    int col = x/cuw;
    int row = y/cuh;
    if(col >= 0 && col < col_num && row >= 0 && row < row_num)
    {
        int x = (col+1)*cuw;
        int y = (row+1)*cuh;
        start.x = col*cuw;
        start.y = row*cuh;
        end.x   = (x > m_cViewBitmap.GetWidth() ? m_cViewBitmap.GetWidth() : x);
        end.y   = (y > m_cViewBitmap.GetHeight() ? m_cViewBitmap.GetHeight(): y);
        return (row*col_num + col);
    }
    return -1;
}

void PicViewCtrl::OnMouseLButtonDown(wxMouseEvent& event)
{
    CaptureMouse();
    wxPoint pos = ClientToScreen(event.GetPosition());
    wxPoint origin = GetPosition();
    int dx = pos.x - origin.x;
    int dy = pos.y - origin.y;
    m_delta = wxPoint(dx, dy);
}

void PicViewCtrl::OnMouseLButtonUp(wxMouseEvent& event)
{
    if(HasCapture())
    {
        if(m_bClearFlag)
        {
            ReleaseMouse();
            return;
        }
        // deal with the m_iSelectedLCUId
        int posx = event.m_x/m_dScaleRate;
        int posy = event.m_y/m_dScaleRate;
        int id = GetCurLCURasterID(posx, posy, m_curSelLCUStart, m_curSelLCUEnd);
        if(id != m_iSelectedLCUId && id != -1)
        {
            m_iSelectedLCUId = id;
            Refresh();
        }
        ReleaseMouse();
    }
}

void PicViewCtrl::OnMouseWheel(wxMouseEvent& event)
{
    if(m_bClearFlag)
    {
        event.Skip();
        return;
    }
    int direction = event.GetWheelRotation();
    int delta     = event.GetWheelDelta();
    if(event.CmdDown()) // scale
    {
        double bigger = (direction/delta)*m_dScaleRateStep;
        double rate   = ((m_dScaleRate + bigger) > m_dMaxScaleRate ? m_dMaxScaleRate : (m_dScaleRate + bigger));
        rate          = rate < m_dMinScaleRate ? m_dMinScaleRate : rate;
        if(fabs(rate - m_dScaleRate) > MINDIFF)
            m_bFitMode = false;
        ChangeScaleRate(rate);
        SetRulerCtrlFited();
    }
    else
    {
        if(m_bMouseWheelPageUpDown)
        {
            int pages = -(direction/delta);
            ShowPageByDiffNumber(pages);
        }
        else
            event.Skip();
    }
}

void PicViewCtrl::CalMinMaxScaleRate()
{
    if(!m_bClearFlag && m_cViewBitmap.IsOk())
    {
        int width    = m_cViewBitmap.GetWidth();
        int height   = m_cViewBitmap.GetHeight();
        // get the screen size to cal min max scale rate
        int scrwidth  = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
        int scrheight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
        double sratew = scrwidth/static_cast<double>(width);
        double srateh = scrheight/static_cast<double>(height);
        double srate  = sratew < srateh ? sratew : srateh;
        m_dMaxScaleRate  = 1.5*srate < 4.0 ? 4.0 : 1.5*srate;
        m_dMinScaleRate  = 50/static_cast<double>(width);
        if(m_dMinScaleRate > m_dMaxScaleRate)
        {
            // this situation may be terrible, here just exchange each other
            double tmp      = m_dMinScaleRate;
            m_dMinScaleRate = m_dMaxScaleRate;
            m_dMaxScaleRate = tmp;
        }
        m_dScaleRateStep = (m_dMaxScaleRate - m_dMinScaleRate)/100.0;
    }
}

void PicViewCtrl::CalFitScaleRate()
{
    if(!m_bClearFlag && m_cViewBitmap.IsOk())
    {
        // get the FitScaleRate, which is the smaller one frome ratew and rateh
        int width    = m_cViewBitmap.GetWidth();
        int height   = m_cViewBitmap.GetHeight();
        int cwidth   = GetParent()->GetSize().GetWidth();
        int cheight  = GetParent()->GetSize().GetHeight();
        double ratew = cwidth/static_cast<double>(width);
        double rateh = cheight/static_cast<double>(height);
        m_dFitScaleRate = ratew < rateh ? ratew : rateh;
        if(m_bFitMode)
            ChangeScaleRate(m_dFitScaleRate);
    }
}

void PicViewCtrl::ChangeScaleRate(const double rate)
{
    if(fabs(rate - m_dScaleRate) > MINDIFF)
    {
        m_dScaleRate = rate;
        m_CtrlSize.SetWidth(m_dScaleRate*m_cViewBitmap.GetWidth());
        m_CtrlSize.SetHeight(m_dScaleRate*m_cViewBitmap.GetHeight());
        this->SetSizeHints(m_CtrlSize);
        GetParent()->FitInside();
        m_bFullRefresh = true;
        SetCurSliderInStatusBarPos();
        Refresh();
    }
}

// if diff > 0 show next, else show previous
bool PicViewCtrl::ShowPageByDiffNumber(const int diff, bool bWantRet)
{
    int cnt     = m_pList->GetItemCount();
    int cursel  = m_pList->GetSelection();
    int nextsel = 0;
    if(cursel != wxNOT_FOUND)
    {
        nextsel = ((cursel+diff) > cnt-1 ? cnt-1 : (cursel+diff));
        nextsel = nextsel < 0 ? 0 : nextsel;
    }
    if(nextsel != cursel)
    {
        m_pList->SetSelection(nextsel);
        wxCommandEvent evt(wxEVT_COMMAND_LISTBOX_SELECTED, wxID_ANY);
        evt.SetInt(nextsel);
        if(!bWantRet)
            wxPostEvent(m_pFrame, evt);
        else
            ((MainFrame*)m_pFrame)->OnThumbnailLboxSelect(evt);
        return true;
    }
    return false;
}

void PicViewCtrl::MoveLCURect(const Direction& d)
{
    if(m_iSelectedLCUId == -1)
        return;
    int cuw = m_LCUSize.GetWidth();
    int cuh = m_LCUSize.GetHeight();
    int col_num = m_cViewBitmap.GetWidth()/cuw + (m_cViewBitmap.GetWidth()%cuw != 0);
    int row_num = m_cViewBitmap.GetHeight()/cuh + (m_cViewBitmap.GetHeight()%cuh != 0);
    int maxid   = row_num*col_num - 1;
    int id = -1;
    switch(d)
    {
    case MOVE_UP:
        id = m_iSelectedLCUId - col_num;
        break;
    case MOVE_DOWN:
        id = m_iSelectedLCUId + col_num;
        break;
    case MOVE_LEFT:
        id = m_iSelectedLCUId - 1;
        break;
    case MOVE_RIGHT:
        id = m_iSelectedLCUId + 1;
        break;
    default:
        assert(0);
        break;
    }
    if(id == -1 && d == MOVE_LEFT) id = 0;
    else if(id == maxid + 1 && d == MOVE_RIGHT) id = maxid;
    else
    {
        id = id > maxid ? id - col_num : id;
        id = id < 0 ? id + col_num : id;
    }
    if(id != m_iSelectedLCUId)
    {
        CalStartEndPointByLCUId(id, m_curSelLCUStart, m_curSelLCUEnd);
        m_iSelectedLCUId = id;
        m_PosData._iBlockX      = m_curSelLCUStart.x;
        m_PosData._iBlockY      = m_curSelLCUStart.y;
        m_PosData._iBlockWidth  = m_curSelLCUEnd.x - m_curSelLCUStart.x;
        m_PosData._iBlockHeight = m_curSelLCUEnd.y - m_curSelLCUStart.y;
        m_PosData._iOffsetX     = 0;
        m_PosData._iOffsetY     = 0;
        wxCommandEvent event(wxEVT_POSITION_CHANGED, wxID_ANY);
        event.SetClientData(&m_PosData);
        wxPostEvent(m_pPixelCtrl, event);
        Refresh();
    }
}

void PicViewCtrl::OnKeyDown(wxKeyEvent& event)
{
    if(m_bClearFlag)
    {
        event.Skip();
        return;
    }
    int key = event.GetKeyCode();
    wxCommandEvent e;
    switch(key)
    {
    case WXK_PAGEUP:
        ((MainFrame*)m_pFrame)->OnGoToPreFrame(e);
        break;
    case WXK_PAGEDOWN:
        ((MainFrame*)m_pFrame)->OnGoToNextFrame(e);
        break;
    case WXK_UP:
        MoveLCURect(MOVE_UP);
        break;
    case WXK_DOWN:
        MoveLCURect(MOVE_DOWN);
        break;
    case WXK_LEFT:
        MoveLCURect(MOVE_LEFT);
        break;
    case WXK_RIGHT:
        MoveLCURect(MOVE_RIGHT);
        break;
    default:
        event.Skip();
        break;
    }
}

void PicViewCtrl::CalStartEndPointByLCUId(const int id, wxPoint& start, wxPoint& end)
{
    int cuw = m_LCUSize.GetWidth();
    int cuh = m_LCUSize.GetHeight();
    int col_num = m_cViewBitmap.GetWidth()/cuw + (m_cViewBitmap.GetWidth()%cuw != 0);
    int row_num = m_cViewBitmap.GetHeight()/cuh + (m_cViewBitmap.GetHeight()%cuh != 0);
    int col = id%col_num;
    int row = id/col_num;
    if(col >= 0 && col < col_num && row >= 0 && row < row_num)
    {
        int x = (col+1)*cuw;
        int y = (row+1)*cuh;
        start.x = col*cuw;
        start.y = row*cuh;
        end.x   = (x > m_cViewBitmap.GetWidth() ? m_cViewBitmap.GetWidth() : x);
        end.y   = (y > m_cViewBitmap.GetHeight() ? m_cViewBitmap.GetHeight() : y);
    }
}

void PicViewCtrl::DrawGrid(wxGraphicsContext* gc)
{
    int cuw = m_LCUSize.GetWidth();
    int cuh = m_LCUSize.GetHeight();
    //int col_num = m_cViewBitmap.GetWidth()/cuw + (m_cViewBitmap.GetWidth()%cuw != 0);
    //int row_num = m_cViewBitmap.GetHeight()/cuh + (m_cViewBitmap.GetHeight()%cuh != 0);
    int startx = m_rectRefresh.x/cuw;
    int starty = m_rectRefresh.y/cuh;
    if(startx < 1) startx = 1;
    if(starty < 1) starty = 1;
    int width  = m_rectRefresh.x + m_rectRefresh.width;
    int height = m_rectRefresh.y + m_rectRefresh.height;
    int endx   = width/cuw + (width%cuw != 0);
    int endy   = height/cuh + (height%cuh != 0);
    gc->SetPen(*wxBLACK_PEN);
    for(int i = starty; i < endy; i++)
        gc->StrokeLine(0, i*cuh*m_dScaleRate, m_cViewBitmap.GetWidth()*m_dScaleRate, i*cuh*m_dScaleRate);
    for(int i = startx; i < endx; i++)
        gc->StrokeLine(i*cuw*m_dScaleRate, 0, i*cuw*m_dScaleRate, m_cViewBitmap.GetHeight()*m_dScaleRate);
}

void PicViewCtrl::DrawBackGround(wxGraphicsContext* gc, wxGraphicsContext* gct)
{
    switch(m_iShowWhich_O_Y_U_V)
    {
    case MODE_ORG:
        gc->DrawBitmap(m_cViewBitmap, 0, 0, m_cViewBitmap.GetWidth(), m_cViewBitmap.GetHeight());
        break;
    case MODE_Y:
        gc->DrawBitmap(m_cViewBitmapY, 0, 0, m_cViewBitmap.GetWidth(), m_cViewBitmap.GetHeight());
        break;
    case MODE_U:
        gc->DrawBitmap(m_cViewBitmapU, 0, 0, m_cViewBitmap.GetWidth(), m_cViewBitmap.GetHeight());
        break;
    case MODE_V:
        gc->DrawBitmap(m_cViewBitmapV, 0, 0, m_cViewBitmap.GetWidth(), m_cViewBitmap.GetHeight());
        break;
    default:
        assert(0);
        break;
    }

    if(m_bShowPUType)
    {
//        gc->SetPen(*wxTRANSPARENT_PEN);
//        gc->SetBrush(wxBrush(wxColor(255, 0, 0, 50)));
//        gc->DrawRectangle(0, 0, m_cViewBitmap.GetWidth(), m_cViewBitmap.GetHeight());
    }
    if(m_bShowGrid)
        DrawGrid(gct);
    if(!m_bOpenedYUVfile && m_bShowTilesInfo)
    {
        DrawSplitInfo(gct);
        DrawTilesGrid(gct);
    }
}

void PicViewCtrl::DrawNoPictureTips(wxGraphicsContext* gc)
{
    double w, h, descent, externalLeading;
    wxSize size = GetClientSize();
    wxString s;
    s.Printf(_T("No picture to show!"), size.x, size.y);
    gc->SetFont(*wxNORMAL_FONT, *wxBLACK);
    gc->SetPen(*wxBLACK_PEN);
    gc->GetTextExtent(s, &w, &h, &descent, &externalLeading);
    gc->SetBrush(wxBrush(wxColor(255, 255, 255, 255)));
    gc->DrawRectangle(0, 0, size.GetWidth()-1, size.GetHeight()-1);
    h += 3;
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    gc->SetPen(*wxLIGHT_GREY_PEN);
    gc->StrokeLine(0, 0, size.x, size.y);
    gc->StrokeLine(0, size.y, size.x, 0);
    gc->DrawText(s, (size.x-w)/2, ((size.y-(h))/2));
    SetRulerCtrlFited();
    wxBitmap::CleanUpHandlers();
}

void PicViewCtrl::SetPicYuvBuffer(TComPicYuv* pBuffer, const int w, const int h, const int bit)
{
    m_pBuffer = pBuffer;
    m_iYUVBit = bit;
    wxBitmap bmp(w, h, 24);
    wxBitmap bmpY(w, h, 24);
    wxBitmap bmpU(w, h, 24);
    wxBitmap bmpV(w, h, 24);
    g_tranformYUV2RGB(w, h, m_pBuffer, m_iYUVBit, bmp, bmpY, bmpU, bmpV, true);
    SetLCUSize(wxSize(g_uiMaxCUWidth, g_uiMaxCUHeight));
    SetBitmap(bmp, bmpY, bmpU, bmpV);
    CalMinMaxScaleRate();
    wxCommandEvent event(wxEVT_YUVBUFFER_CHANGED, wxID_ANY);
    event.SetClientData(pBuffer);
    wxPostEvent(m_pPixelCtrl, event);
}

// pt2 is useless and it is a wrong position now
void PicViewCtrl::GetCurPicViewCtrlPosOnParent(wxPoint& pt1, wxPoint& pt2)
{
    wxSizer* pSizer = GetParent()->GetSizer();
    wxSizerItemList itemList = pSizer->GetChildren();

    assert(itemList.size() > 0);
    pt1 = (*itemList.begin())->GetPosition();
    // wrong cal
//    wxSize size = GetSize();
//    pt2.x = pt1.x + size.x;
//    pt2.y = pt1.y + size.y;
}

void PicViewCtrl::SetRulerCtrlFited()
{
    wxPoint pt1, pt2;
    GetCurPicViewCtrlPosOnParent(pt1, pt2);
    double startx, starty;
    startx = pt1.x;
    starty = pt1.y;
    if(!m_bFitMode || m_bClearFlag)
    {
        int xper, yper, x, y;
        wxScrolledWindow* pPar = (wxScrolledWindow*)GetParent();
        pPar->GetScrollPixelsPerUnit(&xper, &yper);
        pPar->GetViewStart(&x, &y);
        x *= xper;
        y *= yper;
        if(pt1.x >= 0 || pt1.y >= 0)
        {
            pt1.x  = ((pt1.x > 0) ? pt1.x : 0);
            pt1.y  = ((pt1.y > 0) ? pt1.y : 0);
            startx = pt1.x - x;
            starty = pt1.y - y;
        }
        else
        {
            startx = -x;
            starty = -y;
        }
    }
    m_pHRuler->SetStartPos(m_pHRuler->GetRulerWidth() + startx);
    m_pHRuler->SetStartValue(0);
    m_pVRuler->SetStartPos(starty);
    m_pVRuler->SetStartValue(0);
    if(m_bClearFlag)
    {
        m_pHRuler->SetEndValue(300);
        m_pVRuler->SetEndValue(300);
        m_pHRuler->SetScaleRate(1.0);
        m_pVRuler->SetScaleRate(1.0);
    }
    else
    {
        m_pHRuler->SetEndValue(m_cViewBitmap.GetWidth());
        m_pVRuler->SetEndValue(m_cViewBitmap.GetHeight());
        m_pHRuler->SetScaleRate(m_dScaleRate);
        m_pVRuler->SetScaleRate(m_dScaleRate);
    }
}

// get the real ScrolledRect to improve the efficiency
void PicViewCtrl::CalCurScrolledRectOnPicView(wxRect& rect)
{
    m_bFullRefresh = false;
    if(m_bFitMode || m_bClearFlag)
    {
        rect.x = 0;
        rect.y = 0;
        rect.width  = m_cViewBitmap.GetWidth();
        rect.height = m_cViewBitmap.GetHeight();
        return;
    }
    int startx, starty, endx, endy;
    wxPoint pt1, pt2;
    GetCurPicViewCtrlPosOnParent(pt1, pt2);
    int xper, yper, x, y;
    wxScrolledWindow* pPar = (wxScrolledWindow*)GetParent();
    pPar->GetScrollPixelsPerUnit(&xper, &yper);
    pPar->GetViewStart(&x, &y);
    wxSize parSize = pPar->GetSize();
    x *= xper;
    y *= yper;
    if(pt1.x >= 0 || pt1.y >= 0)
    {
        pt1.x  = ((pt1.x > 0) ? pt1.x : 0);
        pt1.y  = ((pt1.y > 0) ? pt1.y : 0);
        startx = pt1.x - x;
        starty = pt1.y - y;
    }
    else
    {
        startx = -x;
        starty = -y;
    }
    wxSize size = m_CtrlSize;
    endx = startx + size.x;
    endy = starty + size.y;
    endx = min(parSize.x, endx);
    endy = min(parSize.y, endy);
    if(startx > 0) startx = 0;
    else startx = -startx;
    if(starty > 0) starty = 0;
    else starty = -starty;
    rect.x = max(startx/m_dScaleRate - 4, 0.0);
    rect.y = max(starty/m_dScaleRate - 4, 0.0);
    rect.width = min(endx/m_dScaleRate + 4, (double)m_cViewBitmap.GetWidth());
    rect.height = min(endy/m_dScaleRate + 4, (double)m_cViewBitmap.GetHeight());
}

void PicViewCtrl::CalTwoRectsOutsideBox(wxRect& rect, const wxPoint& start, const wxPoint& end)
{
    int x2 = max(m_curLCUEnd.x, end.x);
    int y2 = max(m_curLCUEnd.y, end.y);
    rect.x = 0;
    rect.y = 0;
    rect.width = (x2+4)*m_dScaleRate;
    rect.height = (y2+4)*m_dScaleRate;
}

void PicViewCtrl::Clear()
{
    m_iSelectedLCUId = -1;
    m_pHRuler->SetTagValue(-1);
    m_pVRuler->SetTagValue(-1);
    m_pHRuler->SetTagValue2(-1);
    m_pVRuler->SetTagValue2(-1);
    SetSizeHints(300, 300);
    SetFitMode(true);
    GetParent()->FitInside();
    SetClear();
    CalFitScaleRate();
    SetRulerCtrlFited();
    Refresh();
}

void PicViewCtrl::OnDropFiles(wxDropFilesEvent& event)
{
    //g_LogMessage(_T("Enter PicViewCtrl::OnDropFiles"));
    if(event.GetNumberOfFiles() != 1)
    {
        wxMessageBox(_T("Only can drop one files once!"));
        return;
    }
    wxString* filenames = event.GetFiles();
    wxCommandEvent evt(wxEVT_DROP_FILES, wxID_ANY);
    evt.SetString(filenames[0]);
    wxPostEvent(m_pFrame, evt);
    return;
}

void PicViewCtrl::SetCurSliderInStatusBarPos()
{
    wxSlider* pSlider = m_pStatusBar->GetSlider();
    // zoom the double value which put on the slider
    int minVal = (int)(m_dMinScaleRate * 10000);
    int maxVal = (int)(m_dMaxScaleRate * 10000);
    int curVal = (int)(m_dScaleRate * 10000);
    pSlider->SetRange(minVal, maxVal);
    pSlider->SetValue(curVal);
    double curValScale = static_cast<double>(pSlider->GetValue()) / 10000;
    wxString txt = wxString::Format(_T("%.2f%%"), curValScale * 100);
    m_pStatusBar->SetStatusText(txt, HEVCStatusBar::Field_ZoomInfo);
}

void PicViewCtrl::SetRowData(const int num_row, const int* pRowData)
{
    assert(pRowData);
    m_iNumRow = num_row;
    if(m_piRowData)
        delete [] m_piRowData;
    m_piRowData = new int [num_row];
    memcpy(m_piRowData, pRowData, num_row * sizeof(int));
}

void PicViewCtrl::SetColData(const int num_col, const int* pColData)
{
    assert(pColData);
    m_iNumCol = num_col;
    if(m_piColData)
        delete [] m_piColData;
    m_piColData = new int [num_col];
    memcpy(m_piColData, pColData, num_col * sizeof(int));
}

void PicViewCtrl::SetSplitData(const int size, const PtInfo* pData)
{
    assert(size);
    // clear the vector data NOTICE that vector clear() did not clear the memory
    typedef vector<vector<PtInfo> >::iterator ptIterator;
    for(ptIterator it = m_vCurCUSplitInfo.begin(); it < m_vCurCUSplitInfo.end(); ++it)
    {
        it->clear();
    }
    for(ptIterator it = m_vCurPUSplitInfo.begin(); it < m_vCurPUSplitInfo.end(); ++it)
    {
        it->clear();
    }
    for(ptIterator it = m_vCurTUSplitInfo.begin(); it < m_vCurTUSplitInfo.end(); ++it)
    {
        it->clear();
    }
    m_vCurCUSplitInfo.clear();
    m_vCurPUSplitInfo.clear();
    m_vCurTUSplitInfo.clear();

    int iLCUWidth = ceil(m_iPicWidth / (double)m_LCUSize.GetWidth());
    int iLCUHeight = ceil(m_iPicHeight / (double)m_LCUSize.GetHeight());
    int iLCUNumber = iLCUWidth * iLCUHeight;

    m_vCurCUSplitInfo.resize(iLCUNumber);
    m_vCurPUSplitInfo.resize(iLCUNumber);
    m_vCurTUSplitInfo.resize(iLCUNumber);
    // add the data to each vectors
    for(int index = 0; index < size; index++)
    {
        int indexOfInfo = pData[index]._ptCUBlockX + pData[index]._ptCUBlockY * iLCUWidth;
        if(pData[index]._sType == Type_CU)
        {
            m_vCurCUSplitInfo[indexOfInfo].push_back(pData[index]);
        }
        else if(pData[index]._sType == Type_PU)
        {
            m_vCurPUSplitInfo[indexOfInfo].push_back(pData[index]);
        }
        else
        {
            m_vCurTUSplitInfo[indexOfInfo].push_back(pData[index]);
        }
    }
}

void PicViewCtrl::DrawSplitInfo(wxGraphicsContext* gc)
{
    typedef vector<vector<PtInfo> >::iterator ptIterator;
    typedef vector<PtInfo>::iterator         sPtIterator;
    wxPen penCU(wxColor(0, 0, 0, 255), 1);
    wxPen penPU(wxColor(0, 0, 100, 255), 1);
    wxPen penTU_LUMA(wxColor(100, 0, 0, 255), 1);
    wxPen penTU_CHROMA(wxColor(0, 100, 0, 255), 1);
    wxPen penPU_MV(wxColor(100, 0, 100, 200), 1);
    gc->SetPen(penCU);

    for(ptIterator it = m_vCurCUSplitInfo.begin(); it < m_vCurCUSplitInfo.end(); ++it)
    {
        for(sPtIterator sIt = (*it).begin(); sIt < (*it).end(); ++sIt)
        {
            switch(sIt->_preMode)
            {
            case Type_INTRA:
                gc->SetBrush(wxColor(255, 0, 0, 80));
                break;
            case Type_SKIP:
                gc->SetBrush(wxColor(255, 255, 255, 0));
                break;
            default:
                gc->SetBrush(wxColor(255, 255, 255, 0));
                break;
            }
            int sx = sIt->_ptStartX;
            int sy = sIt->_ptStartY;
            int ex = sIt->_ptEndX;
            int ey = sIt->_ptEndY;
            gc->DrawRectangle(sx * m_dScaleRate, sy * m_dScaleRate, (ex - sx) * m_dScaleRate, (ey - sy) * m_dScaleRate);
        }
    }
    gc->SetPen(penPU);
    gc->SetBrush(wxColor(255, 255, 255, 0));
    // PU data draw the Motion Vector data and PU prediction data
    for(ptIterator it = m_vCurPUSplitInfo.begin(); it < m_vCurPUSplitInfo.end(); ++it)
    {
        for(sPtIterator sIt = (*it).begin(); sIt < (*it).end(); ++sIt)
        {
            switch(sIt->_preMode)
            {
            case Type_INTER_P:
                gc->SetBrush(wxColor(0, 0, 255, 80));
                break;
            case Type_INTER_B:
                gc->SetBrush(wxColor(0, 255, 0, 80));
                break;
            default:
                gc->SetBrush(wxColor(255, 255, 255, 0));
                break;
            }
            int sx = sIt->_ptStartX;
            int sy = sIt->_ptStartY;
            int ex = sIt->_ptEndX;
            int ey = sIt->_ptEndY;
            gc->DrawRectangle(sx * m_dScaleRate, sy * m_dScaleRate, (ex - sx) * m_dScaleRate, (ey - sy) * m_dScaleRate);
        }
    }
    gc->SetPen(penTU_LUMA);
    gc->SetBrush(wxColor(255, 255, 255, 0));
    for(ptIterator it = m_vCurTUSplitInfo.begin(); it < m_vCurTUSplitInfo.end(); ++it)
    {
        for(sPtIterator sIt = (*it).begin(); sIt < (*it).end(); ++sIt)
        {
            int sx = sIt->_ptStartX;
            int sy = sIt->_ptStartY;
            int ex = sIt->_ptEndX;
            int ey = sIt->_ptEndY;
            gc->DrawRectangle(sx * m_dScaleRate, sy * m_dScaleRate, (ex - sx) * m_dScaleRate, (ey - sy) * m_dScaleRate);
        }
    }
    gc->SetPen(penPU_MV);
    for (ptIterator it = m_vCurPUSplitInfo.begin(); it < m_vCurPUSplitInfo.end(); ++it)
    {
        for (sPtIterator sIt = (*it).begin(); sIt < (*it).end(); ++sIt)
        {
            if (sIt->_preMode == Type_INTER_P || sIt->_preMode == Type_INTER_B)
            {
                int motionXPre = sIt->_iMotionXPre;
                int motionYPre = sIt->_iMotionYPre;
                int motionXSuf = sIt->_iMotionXSuf;
                int motionYSuf = sIt->_iMotionYSuf;
                if (motionXPre != 0 || motionYPre != 0)
                {
                    int sx = sIt->_ptStartX;
                    int sy = sIt->_ptStartY;
                    int ex = sIt->_ptEndX;
                    int ey = sIt->_ptEndY;
                    double startX = ((sx + ex) / 2.0) * m_dScaleRate;
                    double startY = ((sy + ey) / 2.0) * m_dScaleRate;
                    // the motion data is 1/4 MV
                    double endX = ((sx + ex) / 2.0 + motionXPre / 4.0) * m_dScaleRate;
                    double endY = ((sy + ey) / 2.0 + motionYPre / 4.0) * m_dScaleRate;
                    // draw the motion vector data
                    gc->StrokeLine(startX, startY, endX, endY);
                    gc->DrawEllipse(endX - 2, endY - 2, 4, 4);
                }
                if (sIt->_preMode == Type_INTER_B)
                {
                    if (motionXSuf != 0 || motionYSuf != 0)
                    {
                        int sx = sIt->_ptStartX;
                        int sy = sIt->_ptStartY;
                        int ex = sIt->_ptEndX;
                        int ey = sIt->_ptEndY;
                        double startX = ((sx + ex) / 2.0) * m_dScaleRate;
                        double startY = ((sy + ey) / 2.0) * m_dScaleRate;
                        double endSX = ((sx + ex) / 2.0 + motionXSuf / 4.0) * m_dScaleRate;
                        double endSY = ((sy + ey) / 2.0 + motionYSuf / 4.0) * m_dScaleRate;
                        gc->StrokeLine(startX, startY, endSX, endSY);
                        gc->DrawEllipse(endSX - 2, endSY - 2, 4, 4);
                    }
                }
            }
        }
    }
}

void PicViewCtrl::DrawTilesGrid(wxGraphicsContext* gc)
{
    // TODO the tiles info did not fully support
    gc->SetPen(wxPen(wxColor(255, 255, 0, 255), 2));
    int w = 0, h = 0;
    for(int i = 0 ; i < m_iNumRow; i++)
    {
        h += m_piRowData[i]*m_LCUSize.GetY();
        gc->StrokeLine(0, h * m_dScaleRate, m_cViewBitmap.GetWidth() * m_dScaleRate, h * m_dScaleRate);
    }
    for(int i = 0; i < m_iNumCol; i++)
    {
        w += m_piColData[i]*m_LCUSize.GetX();
        gc->StrokeLine(w * m_dScaleRate, 0, w * m_dScaleRate, m_cViewBitmap.GetHeight() * m_dScaleRate);
    }
}

bool DragDropFile::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
    if(filenames.size() != 1)
    {
        wxMessageBox(_T("Only can drop one files once!"));
        return false;
    }
    wxCommandEvent evt(wxEVT_DROP_FILES, wxID_ANY);
    evt.SetString(filenames[0]);
    wxPostEvent(m_pFrame, evt);
    return true;
}
