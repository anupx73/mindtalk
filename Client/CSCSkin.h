//----------------------------------------------------------------------------
// Copyright (c) 2000-2008 DMSoftware Technologies
// All Rights Reserved.
//----------------------------------------------------------------------------
#pragma once

typedef HRESULT (__cdecl *PFNINITLICENKEYS)(BSTR,BSTR,BSTR,BSTR);
typedef HRESULT (__cdecl *PFNINITDECORATION)(BOOL mode);
typedef HRESULT (__cdecl *PFNLOADSKIN)(BSTR);
typedef HRESULT (__cdecl *PFNAPPLYSKIN)();
typedef HRESULT (__cdecl *PFNREMOVESKIN)();
typedef HRESULT (__cdecl *PFNDEINITDECORATION)();
typedef HRESULT (__cdecl *PFNDECORATEAS)(long,long);
typedef HRESULT (__cdecl *PFNINCLUDEWND)(long,long);
typedef HRESULT (__cdecl *PFNEXCLUDEWND)(long,long);
typedef HRESULT (__cdecl *PFNSETCUSTOMSKINWND)(long,BSTR,BOOL);
typedef HRESULT (__cdecl *PFNGETSKINCOPYRIGHT)(BSTR,BSTR*,BSTR*,BSTR*,BSTR*);
typedef HRESULT (__cdecl *PFNUPDATECONTROL)(long);
typedef HRESULT (__cdecl *PFNUPDATEWND)(long);
typedef HRESULT (__cdecl *PFNADDTHREAD)();
typedef HRESULT (__cdecl *PFNDELETETHREAD)();

typedef HRESULT (__cdecl *PFNADDSKIN)(BSTR, short);
typedef	HRESULT (__cdecl *PFNAPPLYADDEDSKIN)(long, short);
typedef HRESULT (__cdecl *PFNREMOVEADDEDSKIN)(short);
typedef HRESULT (__cdecl *PFNAETCUSTOMSCROLL)(long, BSTR);
typedef HRESULT (__cdecl *PFNSETADDEDCUSTOMSCROLL)(long, short, BSTR);
typedef HRESULT (__cdecl *PFNSETADDEDCUSTOMSKIN)(long, short, BSTR,BOOL);
typedef HRESULT (__cdecl *PFNLOADSKINFROMRESOURCE)(long, long);
typedef HRESULT (__cdecl *PFNGETUSERDATASIZE)(BSTR, long);
typedef HRESULT (__cdecl *PFNGETUSERDATA)(BSTR, long, long);
typedef HRESULT (__cdecl *PFNGETADDEDUSERDATASIZE)(short, BSTR, long);
typedef HRESULT (__cdecl *PFNGETADDEDUSERDATA)(short, BSTR, long, long);

typedef HRESULT (__cdecl *PFNSETDECMODE)(long); 
typedef HRESULT (__cdecl *PFNINCLTHREADWINDS)(long, BOOL); 
typedef HRESULT (__cdecl *PFNEXCLTHREADWINDS)(long, BOOL); 
typedef HRESULT (__cdecl *PFNCLEARSKIN)();
typedef HRESULT (__cdecl *PFNCLEARWND)(long, BOOL);

typedef HRESULT (__cdecl *PFNBEGINHSL)(long type, long hslID);
typedef HRESULT (__cdecl *PFNBEGINCUSTOMHSL)(BSTR skinName, long hslID);
typedef HRESULT (__cdecl *PFNMODIFYHSL)(long hslID, double hue, double saturation, double lightness, double opacity);
typedef HRESULT (__cdecl *PFNENDHSL)(long hslID);


class CSCSkin
{
public:
	CSCSkin();
	~CSCSkin();

	BOOL Init();

	PFNINITLICENKEYS	InitLicenKeys;
	PFNINITDECORATION	InitDecoration;
	PFNLOADSKIN			LoadSkinFromFile;
	PFNAPPLYSKIN		ApplySkin;
	PFNREMOVESKIN		RemoveSkin;
	PFNDEINITDECORATION	DeInitDecoration;
	PFNDECORATEAS		DecorateAs;
	PFNEXCLUDEWND		ExcludeWnd;
	PFNINCLUDEWND		IncludeWnd;
	PFNSETCUSTOMSKINWND	SetCustomSkinWnd;
	PFNGETSKINCOPYRIGHT GetSkinCopyRight;
	PFNUPDATECONTROL	UpdateControl;
	PFNUPDATEWND		UpdateWnd;
    PFNADDTHREAD        AddAdditionalThread;
    PFNDELETETHREAD     DeleteAdditionalThread;

    PFNADDSKIN 				AddSkinFromFile;
    PFNAPPLYADDEDSKIN 		ApplyAddedSkin;
    PFNREMOVEADDEDSKIN		RemoveAddedSkin;
    PFNAETCUSTOMSCROLL 		SetCustomScrollbars;
    PFNSETADDEDCUSTOMSCROLL SetAddedCustomScrollbars;
    PFNSETADDEDCUSTOMSKIN 	SetAddedCustomSkinWnd;
    PFNLOADSKINFROMRESOURCE LoadSkinFromResource;
    PFNGETUSERDATASIZE 		GetUserDataSize;
    PFNGETUSERDATA 			GetUserData;
	PFNGETADDEDUSERDATASIZE GetAddedUserDataSize;
    PFNGETADDEDUSERDATA 	GetAddedUserData;

    PFNSETDECMODE           SetDecorationMode;
    PFNINCLTHREADWINDS      IncludeThreadWindows;
    PFNEXCLTHREADWINDS      ExcludeThreadWindows;
    PFNCLEARSKIN            ClearSkin;
    PFNCLEARWND             ClearWnd;

	PFNBEGINHSL				BeginHSL;
	PFNBEGINCUSTOMHSL		BeginCustomHSL;
	PFNMODIFYHSL			ModifyHSL;
	PFNENDHSL				EndHSL;

protected:
	HMODULE m_hSkinCrafterDLL;
};

//extern CSCSkin skin;