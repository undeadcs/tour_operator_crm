/*
	Tourist application
	Клиентское приложение
	Заказчик
*/
#ifndef TOURIST_CLIENT_EMPLOYER_H
#define TOURIST_CLIENT_EMPLOYER_H

namespace UdSDK {

	class CHEmployerPick : public udListHandlerPick, public udStateHandler {
	protected:
		udPWndHandler		m_wndWrap;
		udPHClientPick		m_wndClient;
		udPHClientUrPick	m_wndClientUr;
		udUInt				m_iType;

		Gtk::HButtonBox	m_boxSwitch;
		Gtk::Button		m_btnClient,
						m_btnClientUr;

	public:
		CHEmployerPick( );
		CHEmployerPick( const CHEmployerPick& objCopy );

		virtual ~CHEmployerPick( );

		CHEmployerPick& operator =( const CHEmployerPick& objCopy );

		virtual void	Init( );
		virtual void	Load( );
		virtual void	Show( );
		virtual void	Hide( );
		virtual void	Clear( );

		virtual void	BeforeCreate( ) {
			Init( );
			Load( );
		}
		virtual void	OnButtonOk( ) { }
		virtual void	OnButtonCancel( ) { }

		void	OnPickEmployer( udInt iType );
		void	OnSwitchEmployer( udInt iToType );

		//virtual udPWndHandler	GetWindowPick( ) const { return m_wndWrap; }
		virtual udPWndHandler	GetWindow( ) const { return m_wndWrap; }

		void	SetType( udUInt iType ) { m_iType = iType; }

		udInt	GetType( ) const { return m_iType; }

	}; // class CHEmployerPick

	class CWgtEmployerPick : public udWgtListPick {
		udUInt	m_iType;

	public:
		CWgtEmployerPick( );
		CWgtEmployerPick( const CWgtEmployerPick& objCopy );

		virtual ~CWgtEmployerPick( );

		CWgtEmployerPick& operator =( const CWgtEmployerPick& objCopy );

		void	SetType( udUInt iType ) { m_iType = iType; }

		udUInt	GetType( ) const { return m_iType; }

	protected:
		virtual void	OnButtonClick( );
		virtual bool	OnClick( GdkEventButton* evtClick );

	}; // class CWgtEmployerPick

	class CFDEEmployer : public udFDEObject {
		udPBase	m_pMainObject;	// объект, у которого есть атрибут emptype - отражающий тип заказчика

	public:
		CFDEEmployer( );
		CFDEEmployer( const CFDEEmployer& objCopy );

		virtual ~CFDEEmployer( );

		CFDEEmployer& operator =( const CFDEEmployer& objCopy );

		virtual void	Load( udPSocket sock );

		void	SetMainObject( udPBase obj ) { m_pMainObject = obj; }

	}; // class CFDEEmployer

} // namespace UdSDK

#endif
