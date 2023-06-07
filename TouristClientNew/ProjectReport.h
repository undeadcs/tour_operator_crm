/*
	Tourist application
	Клиентское приложение
	Отчеты
*/
#ifndef TOURIST_CLIENT_REPORT_H
#define TOURIST_CLIENT_REPORT_H

namespace UdSDK {

	class CHReport : public udStateHandlerEmpty {
		typedef std::vector< Gtk::Button* >	t_vctButton;

		t_vctButton	m_arrButton;	// кнопки
		Gtk::Table	m_tblButton;	// таблица кнопок

	public:
		CHReport( );
		CHReport( const CHReport& objCopy );

		virtual ~CHReport( );

		CHReport& operator =( const CHReport& objCopy );

		virtual void	Init( );

	}; // class CHReport

} // namespace UdSDK

#endif
