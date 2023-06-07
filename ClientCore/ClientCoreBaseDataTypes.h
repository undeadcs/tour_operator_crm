/**
	Tourist application
	Клиентское приложение

	базовые типы данных
*/
#ifndef TOURIST_APPLICATION_CLIENT_CORE_DATA_TYPES_H
#define TOURIST_APPLICATION_CLIENT_CORE_DATA_TYPES_H

// базовые типы SDK
namespace UdSDK {

	typedef class CAppConfig		udAppConfig, *udPAppConfig, **udPPAppConfig;
	typedef class CAppInit			udAppInit, *udPAppInit, **udPPAppInit;
	typedef class CApplication		udApplication, *udPApplication, **udPPApplication;
	typedef class CStateHandler		udStateHandler, *udPStateHandler, **udPPStateHandler;
	typedef class CModelWork		udModelWork, *udPModelWork, **udPPModelWork;

	typedef class CStateHandlerSimple	udStateHandlerSimple, *udPStateHandlerSimple, **udPPStateHandlerSimple;
	typedef class CFormHandler		udFormHandler, *udPFormHandler, **udPPFormHandler;
	typedef class CListHandler		udListHandler, *udPListHandler, **udPPListHandler;

	typedef class CDatabaseSync		udDatabaseSync, *udPDatabaseSync, **udPPDatabaseSync;
	typedef class CDatabaseSyncInfo	udDatabaseSyncInfo, *udPDatabaseSyncInfo, **udPPDatabaseSyncInfo;
	typedef class CDatabaseTableQuery	udDatabaseTableQuery, *udPDatabaseTableQuery, *udPPDatabaseTableQuery;

	typedef class CWndGraphic		udWndGraphic, *udPWndGraphic, **udPPWndGraphic;
	typedef class CWndLogin			udWndLogin, *udPWndLogin, **udPPWndLogin;
	typedef class CWndSplash		udWndSplash, *udPWndSplash, **udPPWndSplash;
	typedef class CWndToolbarBtn	udWndToolbarBtn, *udPWndToolbarBtn, **udPPWndToolbarBtn;
	typedef class CWndToolbar		udWndToolbar, *udPWndToolbar, **udPPWndToolbar;
	typedef class CWndMenuBtn		udWndMenuBtn, *udPWndMenuBtn, **udPPWndMenuBtn;
	typedef class CWndMenu			udWndMenu, *udPWndMenu, **udPPWndMenu;
	typedef class CWndModel			udWndModel, *udPWndModel, **udPPWndModel;
	typedef class CWndWork			udWndWork, *udPWndWork, **udPPWndWork;
	typedef class CRenderer			udRenderer, *udPRenderer, **udPPRenderer;
	typedef class CExtList			udExtList, *udPExtList, **udPPExtList;
	typedef class CWndListPick		udWndListPick, *udPWndListPick, **udPPWndListPick;
	typedef class CWndLink			udWndLink, *udPWndLink, **udPPWndLink;
	typedef class CWndStatic		udWndStatic, *udPWndStatic, **udPPWndStatic;
	typedef class CWndFormList		udWndFormList, *udPWndFormList, **udPPWndFormList;
	typedef class CWndScroller		udWndScroller, *udPWndScroller, **udPPWndScroller;
	typedef class CWndRows			udWndRows, *udPWndRows, **udPPWndRows;
	typedef class CWndRow			udWndRow, *udPWndRow, **udPPWndRow;
	typedef class CWndInstanceList	udWndInstanceList, *udPWndInstanceList, **udPPWndInstanceList;
	typedef class CWndModeList		udWndModeList, *udPWndModeList, **udPPWndModeList;
	typedef class CWndButton		udWndButton, *udPWndButton, **udPPWndButton;
	typedef class CWndModeForm		udWndModeForm, *udPWndModeForm, **udPPWndModeForm;
	typedef class CWndPager			udWndPager, *udPWndPager, **udPPWndPager;

	typedef class CWndFilePick		udWndFilePick, *udPWndFilePick, **udPPWndFilePick;

	typedef map< string, udPStateHandler		>	udAscStateHandler;

	typedef udPStateHandler	( *pfnGetSHandler )( );
	typedef udPListHandler ( *pfnGetListHandler )( );
	typedef udPFormHandler ( *pfnGetFormHandler )( );
	typedef void	( *pfnExtLstAfterPick )( udPBase obj, udPListHandler handler );

	enum OrderType {
		ordDesc,
		ordAsc
	};

} // namespace UdSDK

#endif
