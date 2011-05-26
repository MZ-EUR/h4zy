// AdWareBHO.h : Declaration of the CAdWareBHO

#pragma once
#include "resource.h"       // main symbols

#include "Adware_i.h"
#include <exdispid.h> // DISPID_DOCUMENTCOMPLETE, etc.



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include <shlguid.h>     // IID_IWebBrowser2, DIID_DWebBrowserEvents2, etc.
#include <atlstr.h>

// CAdWareBHO

class ATL_NO_VTABLE CAdWareBHO :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAdWareBHO, &CLSID_AdWareBHO>,
	public IObjectWithSiteImpl<CAdWareBHO>,
	public IDispatchImpl<IAdWareBHO, &IID_IAdWareBHO, &LIBID_AdwareLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<1, CAdWareBHO, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>

{
public:
	STDMETHOD(SetSite)(IUnknown *pUnkSite);

	CAdWareBHO()
	{
		CString Test = _T("sharrangtyagi");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ADWAREBHO)

DECLARE_NOT_AGGREGATABLE(CAdWareBHO)

BEGIN_COM_MAP(CAdWareBHO)
	COM_INTERFACE_ENTRY(IAdWareBHO)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_SINK_MAP(CAdWareBHO)
    SINK_ENTRY_EX(1,  DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
	END_SINK_MAP()


	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	bool m_fAdvised;
	// DWebBrowserEvents2
    void STDMETHODCALLTYPE OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL); 
	void STDMETHODCALLTYPE OnQuit(IDispatch *pDisp, VARIANT *pvarURL); 
	bool IsBlackListed(CString);
    void BlackList(CString);
	bool CheckTime(CString);
	CString GetAffCookie();
	CString GetIdentCookie();

private:
	CComPtr<IWebBrowser2>  m_spWebBrowser;


};

OBJECT_ENTRY_AUTO(__uuidof(AdWareBHO), CAdWareBHO)
