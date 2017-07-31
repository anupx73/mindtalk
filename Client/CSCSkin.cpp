//----------------------------------------------------------------------------
// Copyright (c) 2000-2008 DMSoftware Technologies
// All Rights Reserved.
//----------------------------------------------------------------------------
#include "stdafx.h"
#include "CSCSkin.h"

//CSCSkin skin;

CSCSkin::CSCSkin() :
	m_hSkinCrafterDLL( NULL ),
	InitLicenKeys( NULL ),
	InitDecoration( NULL ),
	LoadSkinFromFile( NULL ),
	ApplySkin( NULL ),
	RemoveSkin( NULL ),
	DeInitDecoration( NULL ),
	DecorateAs( NULL ),
	SetCustomSkinWnd( NULL ),
	GetSkinCopyRight( NULL ),
	UpdateControl( NULL ),
	UpdateWnd( NULL ),
    AddSkinFromFile( NULL ),
    ApplyAddedSkin( NULL ),
    RemoveAddedSkin( NULL ),
    SetCustomScrollbars( NULL ),
    SetAddedCustomScrollbars( NULL ),
    SetAddedCustomSkinWnd( NULL ),
    LoadSkinFromResource( NULL ),
    GetUserDataSize( NULL ),
    GetUserData( NULL ),
	GetAddedUserDataSize( NULL ),
    GetAddedUserData( NULL ),
    SetDecorationMode(NULL), 
    IncludeThreadWindows(NULL), 
    ExcludeThreadWindows(NULL), 
    ClearSkin(NULL),
    ClearWnd(NULL),
	BeginHSL(NULL),
	BeginCustomHSL(NULL),
	ModifyHSL(NULL),
	EndHSL(NULL)

    
{
}

CSCSkin::~CSCSkin()
{
	if ( m_hSkinCrafterDLL ) {
		DeInitDecoration();
		FreeLibrary( m_hSkinCrafterDLL );
	}
}

BOOL CSCSkin::Init()
{
	m_hSkinCrafterDLL = LoadLibrary(L"UI.dll");
	
	if( m_hSkinCrafterDLL ){

		InitLicenKeys		= (PFNINITLICENKEYS)	GetProcAddress(m_hSkinCrafterDLL,"InitLicenKeys");
		InitDecoration		= (PFNINITDECORATION)	GetProcAddress(m_hSkinCrafterDLL,"InitDecoration");
		LoadSkinFromFile	= (PFNLOADSKIN)		GetProcAddress(m_hSkinCrafterDLL,"LoadSkinFromFile");
		ApplySkin		= (PFNAPPLYSKIN)	GetProcAddress(m_hSkinCrafterDLL,"ApplySkin");
		RemoveSkin		= (PFNREMOVESKIN)	GetProcAddress(m_hSkinCrafterDLL,"RemoveSkin");
		DeInitDecoration	= (PFNDEINITDECORATION)	GetProcAddress(m_hSkinCrafterDLL,"DeInitDecoration");
		DecorateAs		= (PFNDECORATEAS)	GetProcAddress(m_hSkinCrafterDLL,"DecorateAs");
		ExcludeWnd		= (PFNEXCLUDEWND)	GetProcAddress(m_hSkinCrafterDLL,"ExcludeWnd");
		IncludeWnd		= (PFNINCLUDEWND)	GetProcAddress(m_hSkinCrafterDLL,"IncludeWnd");
		SetCustomSkinWnd	= (PFNSETCUSTOMSKINWND)	GetProcAddress(m_hSkinCrafterDLL,"SetCustomSkinWnd");
		GetSkinCopyRight	= (PFNGETSKINCOPYRIGHT)	GetProcAddress(m_hSkinCrafterDLL,"GetSkinCopyRight");
		UpdateControl		= (PFNUPDATECONTROL)	GetProcAddress(m_hSkinCrafterDLL,"UpdateControl");
		UpdateWnd		    = (PFNUPDATEWND)	GetProcAddress(m_hSkinCrafterDLL,"UpdateWnd");
        AddAdditionalThread = (PFNADDTHREAD)GetProcAddress(m_hSkinCrafterDLL,"AddAdditionalThread");;
        DeleteAdditionalThread  = (PFNDELETETHREAD)GetProcAddress(m_hSkinCrafterDLL,"DeleteAdditionalThread");;

		AddSkinFromFile            =    (PFNADDSKIN)	GetProcAddress(m_hSkinCrafterDLL,"AddSkinFromFile");
		ApplyAddedSkin             =    (PFNAPPLYADDEDSKIN)	GetProcAddress(m_hSkinCrafterDLL,"ApplyAddedSkin"); 
		RemoveAddedSkin            =    (PFNREMOVEADDEDSKIN)	GetProcAddress(m_hSkinCrafterDLL,"RemoveAddedSkin"); 
		SetCustomScrollbars        =    (PFNAETCUSTOMSCROLL)	GetProcAddress(m_hSkinCrafterDLL,"SetCustomScrollbars"); 
		SetAddedCustomScrollbars   =    (PFNSETADDEDCUSTOMSCROLL)	GetProcAddress(m_hSkinCrafterDLL,"SetAddedCustomScrollbars"); 
		SetAddedCustomSkinWnd      =    (PFNSETADDEDCUSTOMSKIN)	GetProcAddress(m_hSkinCrafterDLL,"SetAddedCustomSkinWnd");
		LoadSkinFromResource       =    (PFNLOADSKINFROMRESOURCE)	GetProcAddress(m_hSkinCrafterDLL,"LoadSkinFromResource"); 
		GetUserDataSize            =    (PFNGETUSERDATASIZE)	GetProcAddress(m_hSkinCrafterDLL,"GetUserDataSize"); 
		GetUserData                =    (PFNGETUSERDATA)	GetProcAddress(m_hSkinCrafterDLL,"GetUserData"); 
		GetAddedUserDataSize       =    (PFNGETADDEDUSERDATASIZE)	GetProcAddress(m_hSkinCrafterDLL,"GetAddedUserDataSize"); 
		GetAddedUserData           =    (PFNGETADDEDUSERDATA)	GetProcAddress(m_hSkinCrafterDLL,"GetAddedUserData"); 

        SetDecorationMode          =    (PFNSETDECMODE) GetProcAddress(m_hSkinCrafterDLL,"SetDecorationMode"); 
        IncludeThreadWindows       =    (PFNINCLTHREADWINDS)    GetProcAddress(m_hSkinCrafterDLL,"IncludeThreadWindows"); 
        ExcludeThreadWindows       =    (PFNEXCLTHREADWINDS)    GetProcAddress(m_hSkinCrafterDLL,"ExcludeThreadWindows"); 
        ClearSkin                  =    (PFNCLEARSKIN)  GetProcAddress(m_hSkinCrafterDLL,"ClearSkin"); 
        ClearWnd                   =    (PFNCLEARWND)   GetProcAddress(m_hSkinCrafterDLL,"ClearWnd"); 

		BeginHSL			= (PFNBEGINHSL)GetProcAddress(m_hSkinCrafterDLL, "BeginHSL");
		BeginCustomHSL		= (PFNBEGINCUSTOMHSL)GetProcAddress(m_hSkinCrafterDLL, "BeginCustomHSL");
		ModifyHSL			= (PFNMODIFYHSL)GetProcAddress(m_hSkinCrafterDLL, "ModifyHSL");
		EndHSL				= (PFNENDHSL)GetProcAddress(m_hSkinCrafterDLL, "EndHSL");

                                               
		                            
		InitLicenKeys(L"SKINCRAFTER",
			L"SKINCRAFTER.COM",
			L"support@skincrafter.com",
			L"DEMOSKINCRAFTERLICENCE");

		InitDecoration(1);

		return TRUE;
	}

	return FALSE;
}

