/*
	Tourist application
	Клиентское приложение
	Справочники
*/
#ifndef TOURIST_CLIENT_REFERENCE_H
#define TOURIST_CLIENT_REFERENCE_H

namespace UdSDK {

	class CHReference : public udStateHandlerEmpty {
		typedef std::vector< Gtk::Button* >	t_vctButton;

		t_vctButton	m_arrButton;	// кнопки
		Gtk::Table	m_tblButton;	// таблица кнопок

	public:
		CHReference( );
		CHReference( const CHReference& objCopy );

		virtual ~CHReference( );

		CHReference& operator =( const CHReference& objCopy );

		virtual void	Init( );

	}; // class CHReference

} // namespace UdSDK

#endif
