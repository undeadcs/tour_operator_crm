/**
	Tourist application
	Клиентское приложение

	простой обработчик, сущность которого состоит только лишь из одного имени
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_STATE_HANDLER_SIMPLE_ONE_NAME_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_STATE_HANDLER_SIMPLE_ONE_NAME_H

namespace UdSDK {

	class CStateHandlerInfo : public udStateHandlerSimple {

	public:
		CStateHandlerInfo( );
		CStateHandlerInfo( const CStateHandlerInfo& objCopy );

		virtual ~CStateHandlerInfo( );

		CStateHandlerInfo& operator =( const CStateHandlerInfo& objCopy );

		virtual void	OnLinkClick( udPWndLink lnk );

	}; // class CStateHandlerInfo

	class CStateHandlerSimpleOneName : public udStateHandlerInfo {
	protected:
		enum {
			ctrlidName
		};

		wstring		m_szAddModeless,
					m_szAddModal,
					m_szFormTitle;
		udPWndLink	m_lnkPrint;

	public:
		CStateHandlerSimpleOneName( );
		CStateHandlerSimpleOneName( const CStateHandlerSimpleOneName& objCopy );

		virtual ~CStateHandlerSimpleOneName( );

		CStateHandlerSimpleOneName& operator =( const CStateHandlerSimpleOneName& objCopy );

		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );

	}; // class CHCountry

} // namespace UdSDK

#endif
