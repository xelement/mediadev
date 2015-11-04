#ifndef MAINFRAME_H_INCLUDED
#define MAINFRAME_H_INCLUDED

#include <list>
#include <vector>
#include <deque>
#include "HEVCAnalyzer.h"
#include "ThumbnailThread.h"
#include "PicViewCtrl.h"
#include "PixelViewCtrl.h"
#include "RulerCtrl.h"
#include "BitsBarCtrl.h"

#define ID_ZoomSlider      MainFrame::ID_MainFrameMax + 100
#define ID_ZoomSliderInfo  MainFrame::ID_MainFrameMax + 101

DECLARE_EVENT_TYPE(wxEVT_ADDANIMAGE_THREAD, wxID_ANY)
DECLARE_EVENT_TYPE(wxEVT_END_THUMB_THREAD, wxID_ANY)
DECLARE_EVENT_TYPE(wxEVT_END_DECODING_THREAD, wxID_ANY)
DECLARE_EVENT_TYPE(wxEVT_DROP_HEVCFILES, wxID_ANY)
DECLARE_EVENT_TYPE(wxEVT_LOGMSG, wxID_ANY)

// in Microsoft Visual Studio 2013, the line below must be commented
// I don't know why should do this, but the code above must not be commented
// Awesome, and the code will generate warnings of C4273 which is disabled
// in HEVCAnalyzer.h --> #pragma warning(disable:4273)

//DECLARE_EVENT_TYPE(wxEVT_DECODING_MAINFRAME_NOTIFY, wxID_ANY)

class ThumbnailThread;
class DecodingThread;
class CenterPageManager;

class MainFrame : public wxFrame
{
    typedef std::deque<BarData> BitsDataStore;

public:
    enum
    {
        ID_NoteBookLeftPane = wxID_HIGHEST+1,
        ID_NoteBookBottomPane,
        ID_NoteBookCenterPane,
        ID_ToolBarLowestID,
        ID_ReOpenWrongConfigYUVFile,
        ID_SwitchColorYUV,
        ID_SwitchGrid,
        ID_Switch_YUV,
        ID_Switch_Y,
        ID_Switch_U,
        ID_Switch_V,
        ID_SwitchfitMode,
        ID_SwitchHEXPixel,
        ID_GoToNextFrame,
        ID_GoToPreFrame,
        ID_Play_Pause,
        ID_FastForward,
        ID_FastBackward,
        ID_ToolBarHighestID,
        ID_TimerPlaying,
        ID_MainFrameMax,
    };

    enum DataBaseType
    {
        ID_SettingData = 0,
        ID_StreamInfoData,
    };

    MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
    virtual ~MainFrame();

    void           OnGoToNextFrame(wxCommandEvent& event);
    void           OnGoToPreFrame(wxCommandEvent& event);
    void           OnThumbnailLboxSelect(wxCommandEvent& event);

private:
    void           OnExit(wxCommandEvent& evt);
    void           OnThreadAddImage(wxCommandEvent& event);
    void           OnMainFrameSizeChange(wxSizeEvent& event);
    void           OnIdle(wxIdleEvent& event);
    // UI creator functions
    void           CreateMenuToolBar();
    void           CreateFileIOToolBar();
    void           CreateYUVToolBar();
    void           CreateNoteBookPane();
    wxNotebook*    CreateLeftNotebook();
    wxNotebook*    CreateCenterNotebook();
    wxNotebook*    CreateBottomNotebook();
    void           SetColorComponent();
    void           ClearThumbnalMemory();
    void           InitThumbnailListView();
    wxString       GetDataBaseFileName(const DataBaseType type);
    void           StoreYUVConfigData(const wxString& file, int width, int height, bool b10bit);
    bool           GetYUVConfigData(const wxString& file, int& width, int& height, bool& b10bit);
    void           SetTotalFrameNumber();
    void           OnFrameClose(wxCommandEvent& event);
    void           SetStatusBarConnection();
    // File IO operators
    void           OnOpenFile(wxCommandEvent& event);
    void           OnOpenYUVFile(const wxString& sFile, const wxString& sName, bool bWrongOpened = false);
    void           OnOpenStreamFile(const wxString& sFile, const wxString& sName);
    void           OnCloseFile(wxCommandEvent& event);
    void           OnThumbThreadEnd(wxCommandEvent& event);
    void           OnDecodingThreadEnd(wxCommandEvent& event);
    void           OnUpdateUI(wxUpdateUIEvent& event);
    void           OnDropDownToolbarYUV(wxAuiToolBarEvent& event);
    void           OnSwitchShowGrid(wxCommandEvent& event);
    void           OnSwitchYUV(wxCommandEvent& event);
    void           OnSwitchFitMode(wxCommandEvent& event);
    void           OnSwitchHEXPixel(wxCommandEvent& event);
    void           OnPlayorPause(wxCommandEvent& event);
    void           OnFastForward(wxCommandEvent& event);
    void           OnFastBackward(wxCommandEvent& event);
    void           OnTimer(wxTimerEvent& event);
    void           OnInputFrameNumber(wxCommandEvent& event);
    void           OnReOpenWrongConfigYUVFile(wxCommandEvent& event);
    void           OnDropFiles(wxCommandEvent& event);
    void           OnScrollChange(wxScrollEvent& event);
    void           OnLogMsg(wxCommandEvent& event);
    void           OnLogMsgWarning(wxCommandEvent& event);
    void           OnLogMsgError(wxCommandEvent& event);
    void           OnDecodingNotify(wxCommandEvent& event);
    void           SetPicViewTilesInfo(int decoding_order);
    void           SetCurrentTiles(int order, wxSQLite3Database* db, wxSQLite3ResultSet* pResult);
    void           SetPicViewSplitInfo(int poc);
    bool           IsBitsDataReady();

    // decoding notify msg handler
    void           OnDecodingSetSize(wxCommandEvent& event);
    void           OnDecodingSetYUVBuffer(wxCommandEvent& event);
    void           OnDecodingSetThumbnailBuffer(wxCommandEvent& event);
    void           OnDecodingSetTilesInfo(wxCommandEvent& event);
    void           OnDecodingSetSplitInfo(wxCommandEvent& event);
    void           OnDecodingSetBitsInfo(wxCommandEvent& event);

private:
    wxAuiManager         m_mgr;
    wxSimpleHtmlListBox* m_pThumbnalList;
    wxImageList*         m_pImageList;
    wxArrayString        m_StrMemFileName;
    wxString             m_sCurOpenedFilePath;
    wxString             m_sCurOpenedFileName;
    wxTextCtrl*          m_pTCLogWin;
    wxTextCtrl*          m_pFrameNumberText;
    wxStaticText*        m_pTotalFrameNumberText;
    PixelViewCtrl*       m_pPixelViewCtrl;
    RulerCtrl*           m_pPixelHRuler;
    RulerCtrl*           m_pPixelVRuler;

    long                 m_notebook_style;
    long                 m_notebook_theme;
    bool                 m_bYUVFile;
    bool                 m_bOPened;
    bool                 m_bPlaying;
    wxTimer*             m_pTimer;
    ShowMode             m_eYUVComponentChoose;

    int                  m_iSourceWidth;
    int                  m_iSourceHeight;
    int                  m_iYUVBit;
    wxFileOffset         m_FileLength;
    int                  m_iTotalFrame;

    TVideoIOYuv          m_cYUVIO;
    TComPicYuv*          m_pcPicYuvOrg;
    ThumbnailThread*     m_pThumbThread;
    CenterPageManager*   m_pCenterPageManager;

    HEVCStatusBar*       m_pStatusBar;
    wxAuiToolBar*        m_ioToolBar;
    wxAuiToolBar*        m_yuvToolBar;

    DecodingThread*      m_pDecodingThread;
    std::vector<int>     m_vDecodingPOCStore;
    BitsDataStore        m_dBitsData;
    int                  m_iLastMAXPOC;
    int                  m_iCurMAXBits;
    BitsBarCtrl*         m_pBitBarCtrl;
    wxString             m_sDecodedYUVPathName;

    DECLARE_EVENT_TABLE()
};

class HEVCStatusBar : public wxStatusBar
{
public:
    enum
    {
        Field_TextTips = 0,
        Field_Info,
        Field_ZoomInfo,
        Field_ZoomSlider,
        Field_Max,
    };
    HEVCStatusBar(wxWindow *parent);
    virtual ~HEVCStatusBar();
    void               OnSize(wxSizeEvent& event);
    wxSlider*          GetSlider() { return m_pZoomSlider; }
    void               SetCenterPageManager(CenterPageManager* pManager) { m_pManager = pManager; }

private:
    wxSlider*          m_pZoomSlider;
    CenterPageManager* m_pManager;

    DECLARE_EVENT_TABLE()
};

struct PanelElments
{
    wxPanel*          _pPicPanel;
    wxScrolledWindow* _pDecodeScrolledWin;
    PicViewCtrl*      _pPicViewCtrl;
    RulerCtrl*        _pPicHRuler;
    RulerCtrl*        _pPicVRuler;
    wxString          _name;
};

class CenterPageManager
{
public:
    CenterPageManager(wxNotebook* pNoteBook, wxSimpleHtmlListBox* pList, wxFrame* pMainFrame,
                     wxWindow* pPixelViewCtrl)
    : m_pCenterNoteBook(pNoteBook), m_pMainFrame(pMainFrame),
    m_pPixelViewCtrl(pPixelViewCtrl), m_pList(pList), m_bFirstCreate(true)
    {}
    ~CenterPageManager();

    void                     Destory();
    void                     Clear();
    void                     Close();
    void                     Show();
    void                     InsertNewPage(const int insertAt, const wxString& name);
    inline unsigned int      GetSize() const { return m_PageList.size(); }
    inline PicViewCtrl*      GetPicViewCtrl(const std::size_t index);
    inline wxScrolledWindow* GetDecodeScrolledWin(const std::size_t index);
    inline RulerCtrl*        GetHorRulerCtrl(const std::size_t index);
    inline RulerCtrl*        GetVerRulerCtrl(const std::size_t index);
    inline wxString          GetName(const std::size_t index);

private:
    std::list<PanelElments>  m_PageList;
    wxNotebook*              m_pCenterNoteBook;
    wxFrame*                 m_pMainFrame;
    wxWindow*                m_pPixelViewCtrl;
    wxSimpleHtmlListBox*     m_pList;
    wxStatusBar*             m_pStatusBar;
    bool                     m_bFirstCreate;
};

#endif // MAINFRAME_H_INCLUDED
