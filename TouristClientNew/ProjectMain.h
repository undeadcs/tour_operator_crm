/*
	Tourist application
	Клиентское припложение
	Главаная страница
*/
#ifndef TOURIST_CLIENT_PROJECT_MAIN_H
#define TOURIST_CLIENT_PROJECT_MAIN_H

namespace UdSDK {

	class CHMain : public udStateHandlerEmpty {
		Gtk::Label	m_lblHeader,
					m_lblUserName;

	public:
		CHMain( );
		CHMain( const CHMain& objCopy );

		virtual ~CHMain( );

		CHMain& operator =( const CHMain& objCopy );

		virtual void	Load( );

	}; // class CProjectMain

} // namespace UdSDK

#endif
