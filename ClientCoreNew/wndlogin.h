/**
	Tourist application
	Библиотека ядра клиентского приложения
	Окно входа
*/
#ifndef TOURIST_CLIENT_CORE_WND_LOGIN_H
#define TOURIST_CLIENT_CORE_WND_LOGIN_H

namespace UdSDK {

	class CWndLogin : public Gtk::Window {
		Gtk::VBox		m_boxWindow;
		Gtk::Button		*m_btnOk,
						*m_btnCancel;
		udPWgtInput		m_inpLogin,
						m_inpPassword;
		Gtk::Label		m_lblLogin,
						m_lblPassword;
		Gtk::HButtonBox	m_boxButton;

	public:
		CWndLogin( );

		virtual ~CWndLogin( );

		udPWgtInput		GetInputLogin( )	const { return m_inpLogin;		}
		udPWgtInput		GetInputPassword( )	const { return m_inpPassword;	}
		Gtk::Button*	GetButtonOk( )		const { return m_btnOk;			}
		Gtk::Button*	GetButtonCancel( )	const { return m_btnCancel;		}

	protected:
		void	OnActivateEntry( );

	}; // class CWndLogin

} // namespace UdSDK

#endif
