#ifndef MAINUIINSTANCE_H_INCLUDED
#define MAINUIINSTANCE_H_INCLUDED

#include "HEVCAnalyzer.h"

extern const wxEventType wxEVT_DECODING_MAINFRAME_NOTIFY;

namespace Utils
{
// ===========================================================
//      tuple utils implementation (variadic templates)
//  the code below use the feature of C++11, so it may not be
//  compiled successfully by Visual Studio lower version
// ===========================================================

    template<typename... TList> struct tuple;
    template<> struct tuple<> {};
    typedef tuple<>                nulltuple;

    template<typename T, typename... TList>
    struct tuple<T, TList...> : public tuple<TList...>
    {
        typedef T                  value_type;
        typedef tuple<TList...>    base_type;

        tuple() {}
        tuple(value_type v, TList... tails)
            : base_type(tails...), _value(v) {}
        const value_type& head() const { return _value; }
        value_type& head() { return _value; }
        tuple<T, TList...>& operator = (const tuple<T, TList...>& _tuple)
        {
            if(&_tuple == this) return *this;
            _value = _tuple._value;
            tuple<TList...>::operator = (_tuple);
            return *this;
        }

    protected:
        value_type                 _value;
    };

    template<typename T>
    struct tuple<T> : public nulltuple
    {
        typedef T                  value_type;
        typedef nulltuple          base_type;

        tuple() {}
        tuple(value_type v) : _value(v) {}
        tuple<T>& operator = (const tuple<T>& _tuple)
        {
            if(&_tuple == this) return *this;
            _value = _tuple._value;
            return *this;
        }

        const value_type& head() const { return _value; }
        value_type& head() { return _value; }

    protected:
        value_type                 _value;
    };

    template<std::size_t N, typename... TList> struct tuple_at;
    template<std::size_t N, typename T, typename... TList>
    struct tuple_at<N, tuple<T, TList...> >
    {
        typedef typename tuple_at<N - 1, tuple<TList...> >::value_type value_type;
        typedef typename tuple_at<N - 1, tuple<TList...> >::tuple_type tuple_type;
    };

    template<typename T, typename... TList>
    struct tuple_at<0, tuple<T, TList...> >
    {
        typedef T                  value_type;
        typedef tuple<T, TList...> tuple_type;
    };

    template<>
    struct tuple_at<0, nulltuple>
    {
        typedef nulltuple          value_type;
        typedef nulltuple          tuple_type;
    };

    template<std::size_t N, typename... TList>
    const typename tuple_at<N, tuple<TList...> >::value_type&
    tuple_get(const tuple<TList...>& _tuple)
    {
        typedef tuple<TList...>    tuple_type;
        typedef typename tuple_at<N, tuple_type>::tuple_type base_tuple_type;
        typedef typename tuple_at<N, tuple_type>::value_type return_type;

        return static_cast<const base_tuple_type&>(_tuple).head();
    }
// =============================================
//       tuple utils implementation end
// =============================================
}

class NoneCopyable
{
public:
    NoneCopyable() = default;

    // delete the copy interface
    NoneCopyable(const NoneCopyable& ins) = delete;
    NoneCopyable& operator = (const NoneCopyable& ins) = delete;
};

enum MainMSG_TYPE
{
    MainMSG_SETSIZE_OF_DECODE_FRMAE = 0,    // the size of frame
    MainMSG_SETYUV_BUFFER,                  // the YUV buffer of each frame
    MainMSG_SETTHUMBNAIL_BUFFER,            // the thumbnail buffer of each frame
    MainMSG_SETTILESINFO,                   // the tiles info of each frame (have bugs)
    MainMSG_SETSPLITINFO,                   // CU/TU/PU split info (more data)
    MainMSG_SETBITSINFO,                    // bits allocation of each frame
    MainMSG_MAX,
};

enum SplitType
{
    Type_CU = 0,
    Type_PU,
    Type_TU_LUMA,
    Type_TU_CHROMA,
    Type_SplitType_MAX,
};

enum PreType
{
    Type_INTRA = 0,
    Type_SKIP,
    Type_INTER_P,
    Type_INTER_B,
    Type_PreType_MAX,
};

struct PtInfo
{
    UInt _ptCUBlockX;
    UInt _ptCUBlockY;
    UInt _ptStartX;
    UInt _ptStartY;
    UInt _ptEndX;
    UInt _ptEndY;
    SplitType _sType;   // the PU split
    PreType   _preMode; // the mb predict type
    // motion vector used by inter P/B
    Int  _iMotionXPre;  // motion vector x/y Pre is the previous Motion Vector of P/B
    Int  _iMotionYPre;
    Int  _iMotionXSuf;  // Suf is the suffix Motion Vector of B
    Int  _iMotionYSuf;
};

class MainUIInstance : public NoneCopyable
{
public:
    static MainUIInstance*   GetInstance();
    static void              Destroy();
    void                     SetActiveTargetMainFrame(wxFrame* pFrame);
    void                     SetCurrentInfoDb(wxSQLite3Database* pDataBase);
    wxSQLite3Database*       GetDataBase() { return m_pDataBase; }

    // the code of template function must be put the in the header file
    // Decoding msg router function, the msg is sent to MainFrame
    template<typename... T>
    void                     MessageRouterToMainFrame(MainMSG_TYPE type, const Utils::tuple<T...>& data)
    {
        if(!m_pMainFrame) assert(0);
        wxCommandEvent event(wxEVT_DECODING_MAINFRAME_NOTIFY, wxID_ANY);
        Utils::tuple<T...>* pData_val = new Utils::tuple<T...>();
        *pData_val = data;
        event.SetId((int)type);
        event.SetClientData((void*)pData_val);
        wxPostEvent(m_pMainFrame, event);
    }

private:
    // private data member
    MainUIInstance() : NoneCopyable(), m_pMainFrame(NULL) {}
    static MainUIInstance*   m_pInstance;
    static wxMutex           m_Mutex;
    wxFrame*                 m_pMainFrame;

    wxSQLite3Database*       m_pDataBase;
};

class LogMsgUIInstance : public NoneCopyable
{
public:
    enum MSG_TYPE
    {
        MSG_TYPE_NORMAL = 0,
        MSG_TYPE_WARNING,
        MSG_TYPE_ERROR,
        MSG_TYPE_CLEAR,
        MSG_TYPE_MAX,
    };
    static LogMsgUIInstance* GetInstance();
    static void              Destory();
    void                     SetActiveTarget(wxFrame *pFrame);
    void                     LogMessage(const wxString& msg, MSG_TYPE type = MSG_TYPE_NORMAL);
    void                     ClearLog();

private:
    LogMsgUIInstance() : NoneCopyable(), m_pFrame(NULL) {}
    static LogMsgUIInstance* m_pInstance;
    static wxMutex           m_Mutex;
    wxFrame*                 m_pFrame;
};

#endif // MAINUIINSTANCE_H_INCLUDED
