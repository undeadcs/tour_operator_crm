/*
	Tourist application
	Клиентское приложение
	Инициализатор приложения
*/
#ifndef TOURIST_CLIENT_APP_INIT_H
#define TOURIST_CLIENT_APP_INIT_H

namespace UdSDK {

	class CAppInitProject : public udAppInit {
		Gtk::Image		m_imgTour,
						m_imgOrder,
						m_imgRequest,
						m_imgEdit,
						m_imgDel;

	public:
		CAppInitProject( );
		CAppInitProject( const CAppInitProject& objCopy );

		virtual ~CAppInitProject( );

		CAppInitProject& operator =( const CAppInitProject& objCopy );

		virtual void	InitToolbar( );
		virtual void	InitMenu( );
		virtual void	InitModel( );

	}; // class CAppInitProject

} // namespace UdSDK

#endif
