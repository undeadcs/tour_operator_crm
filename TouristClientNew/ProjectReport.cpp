
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHReport::CHReport( ) :
		udStateHandlerEmpty( )
	{
		m_szName = "report";
		m_tblButton.set_row_spacings( 5 );
		m_tblButton.set_col_spacings( 5 );
		m_wndEmpty->add( m_tblButton );
		m_wndEmpty->show_all_children( );
	} // CHReport

	CHReport::CHReport( const CHReport& objCopy ) :
		udStateHandlerEmpty( )
	{
		m_szName = "report";
	} // CHReport

	CHReport::~CHReport( ) {
		for( t_vctButton::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
			delete *i;
		}
	} // ~CHReport

	CHReport& CHReport::operator =( const CHReport& objCopy ) {
		return *this;
	} // CHReport& operator =

	void CHReport::Init( ) {
		Gtk::Button	*btn = NULL;
		udInt	iColL = 0, iColR = 1,
				iLineL = 0, iLineR = 1;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		iColL = 0;
		iColR = 1;

		btn = new Gtk::Button( Glib::locale_to_utf8( "Посадочная ведомость" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"report_landing"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Ведомость текущего наполнения групп" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"report_filling"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);
		//*
		btn = new Gtk::Button( Glib::locale_to_utf8( "Оплата группы" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"report_group_pay"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Оплата гостиниц" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"report_hotel_pay"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Оплата за день" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"report_day_pay"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udStateHandlerEmpty::Init( );
	} // void Init

} // namespace UdSDK
