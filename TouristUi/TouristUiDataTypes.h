/**
	Tourist application
	Библиотека интерфейса пользователя

	базовые типы данных
*/
#ifndef TOURIST_APPLICATION_USER_INTERFACE_DATA_TYPES_H
#define TOURIST_APPLICATION_USER_INTERFACE_DATA_TYPES_H

namespace UdSDK {

	/*typedef class CMySql			udMySql, *udPMySql, **udPPMySql;
	typedef class CServerAccount	udServerAccount, *udPServerAccount, **udPPServerAccount;
	typedef class CDatabaseAccount	udDatabaseAccount, *udPDatabaseAccount, **udPPDatabaseAccount;
	typedef class CAppConfig		udAppConfig, *udPAppConfig, **udPPAppConfig;
	typedef class CAppInit			udAppInit, *udPAppInit, **udPPAppInit;
	typedef class CApplication		udApplication, *udPApplication, **udPPApplication;
	typedef class CStateHandler		udStateHandler, *udPStateHandler, **udPPStateHandler;
	typedef class CModelWork		udModelWork, *udPModelWork, **udPPModelWork;
	typedef class CSystemUser		udSystemUser, *udPSystemUser, **udPPSystemUser;

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
	typedef class CWndModeForm		udWndModeForm, *udPWndModeForm, **udPPWndModeForm;*/

	typedef map< udInt, HWND					>	udIAscHWnd, *udPIAscHWnd;
	typedef map< udDWord, udPWndRow				>	udDwAscWndRow, *udPDwAscWndRow;
	typedef map< udLong, udPWndLink				>	udLAscWndLink, *udPLAscWndLink;

	typedef vector< udPWndLink					>	udArrLink, *udPArrLink, udArrWndLink, *udPArrWndLink;
	typedef vector< udPWndStatic				>	udArrWndStatic, *udPArrWndStatic;
	typedef vector< udPWndRow					>	udArrWndRow, *udPArrWndRow;

	enum wndmsg {
		WM_LINKCLICK	= WM_USER,	// клик по окну ссылки
		WM_ROW_EDIT,				// клик по ссылке редактирования строки
		WM_ROW_DELETE,				// клик по ссылке удаления строки
		WM_MENU_BUTTON_CLICK,		// клик по кнопке меню
		WM_TOOLBAR_BUTTON_CLICK,	// клик по кнопке тулбара
		WM_LIST_ROW_SELECT,			// выбор строки списка сущностей
		WM_LIST_HEADER_CLICK,		// клик по ячейке заголовка списка сущностей
		WM_BUTTONCLICK,				// клик по окну кнопки
		WM_FORM_CONFIRM,			// подтверждение формы
		WM_FORM_CANCEL,				// отмена формы
		WM_PAGER_SELECT,			// выбор страницы из пейджера
	};

	enum wndstyle {
		WS_STANDARD_EDIT = WS_CHILD | ES_AUTOHSCROLL,
		WS_STANDARD_EDIT_MULTILINE = WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL,
		WS_STANDARD_SELECT = WS_CHILD | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST
	};

} // namespace UdSDK

#endif
