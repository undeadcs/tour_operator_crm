/**
	Tourist application
	Библиотека ядра клиентского приложения
	Типы данных
*/
#ifndef TOURIST_CLIENT_CORE_DATA_TYPES_H
#define TOURIST_CLIENT_CORE_DATA_TYPES_H

namespace UdSDK {

	typedef class CAppInit		udAppInit, *udPAppInit, **udPPAppInit;
	typedef class CAppConfig	udAppConfig, *udPAppConfig, **udPPAppConfig;
	typedef class CWndLogin		udWndLogin, *udPWndLogin, **udPPWndLogin;
	typedef class CWndToolbar	udWndToolbar, *udPWndToolbar, **udPPWndToolbar;
	typedef class CWndMenu		udWndMenu, *udPWndMenu, **udPPWndMenu;
	typedef class CWndModel		udWndModel, *udPWndModel, **udPPWndModel;
	typedef class CWndWork		udWndWork, *udPWndWork, **udPPWndWork;
	typedef class CAppUser		udAppUser, *udPAppUser, **udPPAppUser;
	typedef class CApplication	udApplication, *udPApplication, **udPPApplication;
	typedef class CHandlerManager
								udHandlerManager, *udPHandlerManager, **udPPHandlerManager;
	typedef class CStateHandler	udStateHandler, *udPStateHandler, **udPPStateHandler;
	typedef class CFormHandler	udFormHandler, *udPFormHandler, **udPPFormHandler;
	typedef class CWndHandler	udWndHandler, *udPWndHandler, **udPPWndHandler;
	typedef class CStateHandlerEmpty
								udStateHandlerEmpty, *udPStateHandlerEmpty, **udPPStateHandlerEmpty;
	typedef class CDataAccess	udDataAccess, *udPDataAccess, **udPPDataAccess;
	typedef class CListHandler	udListHandler, *udPListHandler, **udPPListHandler;
	typedef class CDAList		udDAList, *udPDAList, **udPPDAList;
	typedef class CListHandlerBtnForm
								udListHandlerBtnForm, *udPListHandlerBtnForm, **udPPListHandlerBtnForm;
	typedef class CListHandlerModeless
								udListHandlerModeless, *udPListHandlerModeless, **udPPListHandlerModeless;
	typedef class CListHandlerPick
								udListHandlerPick, *udPListHandlerPick, **udPPListHandlerPick;
	typedef class CDAForm		udDAForm, *udPDAForm, **udPPDAForm;
	typedef class CFormDataExtra
								udFormDataExtra, *udPFormDataExtra, **udPPFormDataExtra;
	typedef class CFDECallback	udFDECallback, *udPFDECallback, **udPPFDECallback;
	typedef class CFDEObject	udFDEObject, *udPFDEObject, **udPPFDEObject;
	typedef class CFDEList		udFDEList, *udPFDEList, **udPPFDEList;
	typedef class CFDEFile		udFDEFile, *udPFDEFile, **udPPFDEFile;
	typedef class CWgtListPick	udWgtListPick, *udPWgtListPick, **udPPWgtListPick;
	typedef class CWgtFormList	udWgtFormList, *udPWgtFormList, **udPPWgtFormList;
	typedef class CWgtFilePick	udWgtFilePick, *udPWgtFilePick, **udPPWgtFilePick;
	typedef class CWgtDatePick	udWgtDatePick, *udPWgtDatePick, **udPPWgtDatePick;
	typedef class CWgtInput		udWgtInput, *udPWgtInput, **udPPWgtInput;
	typedef class CWgtSelect	udWgtSelect, *udPWgtSelect, **udPPWgtSelect;

	typedef udPListHandler		( *pfnGetListHandler )( );
	typedef udPListHandlerPick	( *pfnGetListHandlerPick )( );
	typedef udPFormHandler		( *pfnGetFormHandler )( );

} // namespace UdSDK

#endif
