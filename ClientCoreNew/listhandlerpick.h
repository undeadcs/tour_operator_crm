/*
	Tourist application
	Библиотека ядра клиентского приложения
	Обработчик выбора из списка
*/
#ifndef TOURIST_CLIENT_CORE_LISTHANDLER_PICK_H
#define TOURIST_CLIENT_CORE_LISTHANDLER_PICK_H

namespace UdSDK {

	class CListHandlerPick {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Button		*m_btnOk,
						*m_btnCancel;
		udPBase			m_pPickedObject;

	public:
		CListHandlerPick( );
		CListHandlerPick( const CListHandlerPick& objCopy );

		virtual ~CListHandlerPick( );

		CListHandlerPick& operator =( const CListHandlerPick& objCopy );

		virtual void	BeforeCreate( ) = 0;
		virtual void	OnButtonOk( ) = 0;
		virtual void	OnButtonCancel( ) = 0;

		//virtual udPWndHandler	GetWindowPick( ) const = 0;
		virtual udPWndHandler	GetWindow( ) const = 0;

		udPBase	GetObject( ) const { return m_pPickedObject; }

		Gtk::Button*	GetButtonOk( )		const { return m_btnOk;		}
		Gtk::Button*	GetButtonCancel( )	const { return m_btnCancel;	}

	}; // class CListHandlerPick

} // namespace UdSDK

#endif
