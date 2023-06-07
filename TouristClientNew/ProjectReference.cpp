
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHReference::CHReference( ) :
		udStateHandlerEmpty( )
	{
		m_szName = "reference";
		m_tblButton.set_row_spacings( 5 );
		m_tblButton.set_col_spacings( 5 );
		m_wndEmpty->add( m_tblButton );
		m_wndEmpty->show_all_children( );
	} // CHReference

	CHReference::CHReference( const CHReference& objCopy ) :
		udStateHandlerEmpty( )
	{
		m_szName = "reference";
	} // CHReference

	CHReference::~CHReference( ) {
		for( t_vctButton::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
			delete *i;
		}
	} // ~CHReference

	CHReference& CHReference::operator =( const CHReference& objCopy ) {
		return *this;
	} // CHReference& operator =

	void CHReference::Init( ) {
		Gtk::Button	*btn = NULL;
		udInt	iColL = 0, iColR = 1,
				iLineL = 0, iLineR = 1;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		iColL = 0;
		iColR = 1;

		btn = new Gtk::Button( Glib::locale_to_utf8( "Страны" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"country"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Типы услуг" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"tservice"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);
		//*
		btn = new Gtk::Button( Glib::locale_to_utf8( "Состояния заявки" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"sorder"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Результаты обращения" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"rrequest"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		++iLineL;
		++iLineR;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		iColL = 0;
		iColR = 1;

		btn = new Gtk::Button( Glib::locale_to_utf8( "Состояние тура" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"stour"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Типы обращения" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"trequest"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Направления" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"direct"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Наименования услуги" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"nameservice"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		++iLineL;
		++iLineR;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		iColL = 0;
		iColR = 1;

		btn = new Gtk::Button( Glib::locale_to_utf8( "Типы документов" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"doctype"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Валюты" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"currency"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Города" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"city"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Валютные пары" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"currencypair"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		++iLineL;
		++iLineR;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		iColL = 0;
		iColR = 1;

		btn = new Gtk::Button( Glib::locale_to_utf8( "Состояния обращения" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"srequest"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Единицы измерения услуг" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"muservice"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Шаблоны документов" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"doctpl"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Распечатанные документы" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"doctplprinted"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		++iLineL;
		++iLineR;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		iColL = 0;
		iColR = 1;

		btn = new Gtk::Button( Glib::locale_to_utf8( "Пункты перехода" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"transferpoint"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Маршруты" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"route"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Принимающие стороны" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"hostside"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Списки туристов" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"touristlist"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		++iLineL;
		++iLineR;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		iColL = 0;
		iColR = 1;

		btn = new Gtk::Button( Glib::locale_to_utf8( "Услуги" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"service"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Партнеры" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"partner"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		btn = new Gtk::Button( Glib::locale_to_utf8( "Компании" ) );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"company"
		) );
		m_arrButton.push_back( btn );
		btn->show( );
		m_tblButton.attach(
			*btn, iColL++, iColR++, iLineL, iLineR, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK
		);

		++iLineL;
		++iLineR;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udStateHandlerEmpty::Init( );
	} // void Init

} // namespace UdSDK