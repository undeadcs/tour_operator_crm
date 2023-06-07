
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CListHandler::CListHandler( ) :
		udStateHandler( ),
		m_hbbTop( Gtk::BUTTONBOX_START ),
		m_hbbPager( false, 5 ),
		m_btnFilter( Glib::locale_to_utf8( "Фильтровать" ) ),
		m_btnReset( Glib::locale_to_utf8( "Сброс" ) )
	{
		m_btnFilter.set_name( "TouristListButtonFilter" );
		m_btnFilter.set_size_request( 151, 34 );
		m_btnReset.set_name( "TouristListButtonReset" );
		m_btnReset.set_size_request( 151, 34 );

		m_wndListWrap	= new udWndHandler;
		m_pRowModel		= NULL;
		m_pData			= new udDAList;

		m_objTreeView.set_enable_search( false );
		m_objTreeView.set_headers_clickable( true );
		m_objTreeView.set_rules_hint( true );
		m_hbbPager.set_border_width( 10 );

		m_objTreeView.signal_row_activated( ).connect( sigc::mem_fun( this, &CListHandler::OnRowActivated ) );

		m_btnFilter.signal_clicked( ).connect( sigc::mem_fun( this, &CListHandler::OnFilter ) );
		m_btnReset.signal_clicked( ).connect( sigc::mem_fun( this, &CListHandler::OnReset ) );
	} // CListHandler

	CListHandler::CListHandler( const CListHandler& objCopy ) : udStateHandler( ) {
	} // CListHandler

	CListHandler::~CListHandler( ) {
		udDELETE( m_pData );
		udDELETE( m_pRowModel );
		udDELETE( m_wndListWrap );
	} // ~CListHandler

	CListHandler& CListHandler::operator =( const CListHandler& objCopy ) {
		return *this;
	} // CListHandler& operator =

	void CListHandler::Init( ) {
		Glib::ListHandle< Gtk::TreeViewColumn* > arrColumn = m_objTreeView.get_columns( );
		Gtk::CellRenderer* renderer = NULL;
		Gtk::TreeViewColumn *col = NULL;

		for( Glib::ListHandle< Gtk::TreeViewColumn* >::iterator i = arrColumn.begin( ); i != arrColumn.end( ); ++i ) {
			col = *i;
			renderer = col->get_first_cell_renderer( );
			renderer->set_padding( 5, 8 );
			col->set_resizable( true );
			col->set_min_width( 100 );
		}

		m_hbbTop.set_border_width( 5 );
		m_hbbTop.show_all( );
		m_wndListWrap->pack_start( m_hbbTop, false, false, 0 );
		m_wndListWrap->pack_start( m_tblFilter, Gtk::PACK_SHRINK, 5 );
		m_tblFilter.show( );
		m_objTreeView.show( );
		m_objTreeModel = Gtk::ListStore::create( *m_pRowModel );
		m_objTreeView.set_model( m_objTreeModel );
		m_wndListWrap->pack_start( m_objTreeView, false, true, 0 );
		m_wndListWrap->pack_start( m_hbbPager, false, true, 0 );
		m_wndListWrap->show_all( );
	} // void Init

	void CListHandler::Load( ) {
		m_pData->Load( );
		GeneratePager( );
	} // void Load

	void CListHandler::Show( ) {
	} // void Show

	void CListHandler::Hide( ) {
	} // void Hide

	void CListHandler::Clear( ) {
		g_pApp->GetLog( )->Write( "CListHandler::Clear, enter\n" );
		g_pApp->GetLog( )->Write( "CListHandler::Clear, m_objTreeModel->clear\n" );
		m_objTreeModel->clear( );
		g_pApp->GetLog( )->Write( "CListHandler::Clear, m_pData->Clear\n" );
		m_pData->Clear( );
		g_pApp->GetLog( )->Write( "CListHandler::Clear, m_hbbPager.children( ).clear\n" );
		m_hbbPager.children( ).clear( );
		g_pApp->GetLog( )->Write( "CListHandler::Clear, exit\n" );
	} // void Clear

	void CListHandler::OnFilter( ) {
		g_pApp->GetLog( )->Write( "CListHandler::OnFilter, enter\n" );
		g_pApp->GetLog( )->Write( "CListHandler::OnFilter, m_objTreeModel->clear\n" );
		m_objTreeModel->clear( );
		g_pApp->GetLog( )->Write( "CListHandler::OnFilter, m_pData->Clear\n" );
		m_pData->Clear( );
		g_pApp->GetLog( )->Write( "CListHandler::OnFilter, m_hbbPager.children( ).clear\n" );
		m_hbbPager.children( ).clear( );
		g_pApp->GetLog( )->Write( "CListHandler::OnFilter, FilterUiToObject\n" );
		FilterUiToObject( );
		g_pApp->GetLog( )->Write( "CListHandler::OnFilter, Load\n" );
		Load( );
		g_pApp->GetLog( )->Write( "CListHandler::OnFilter, exit\n" );
	} // void OnFilter

	void CListHandler::OnReset( ) {
		g_pApp->GetLog( )->Write( "CListHandler::OnReset, enter\n" );
		g_pApp->GetLog( )->Write( "CListHandler::OnReset, Clear\n" );
		Clear( );
		m_pData->GetPager( )->SetPage( 1 );
		g_pApp->GetLog( )->Write( "CListHandler::Clear, m_pData->ClearExtraData\n" );
		m_pData->ClearExtraData( );
		g_pApp->GetLog( )->Write( "CListHandler::Clear, m_pData->ResetFilter\n" );
		m_pData->ResetFilter( );
		g_pApp->GetLog( )->Write( "CListHandler::Clear, FilterUiClear\n" );
		FilterUiClear( );
		g_pApp->GetLog( )->Write( "CListHandler::OnReset, Load\n" );
		Load( );
		g_pApp->GetLog( )->Write( "CListHandler::OnReset exit\n" );
	} // void OnReset

	void CListHandler::OnButtonPager( udInt iPage ) {
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, enter\n" );
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, m_objTreeModel->clear\n" );
		m_objTreeModel->clear( );
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, m_pData->Clear\n" );
		m_pData->Clear( );
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, m_hbbPager.children( ).clear\n" );
		m_hbbPager.children( ).clear( );
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, m_pData->GetPager( )->SetPage\n" );
		m_pData->GetPager( )->SetPage( iPage );
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, FilterUiToObject\n" );
		FilterUiToObject( );
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, Load\n" );
		Load( );
		g_pApp->GetLog( )->Write( "CListHandler::OnButtonPager, exit\n" );
	} // void OnButtonPager

	void CListHandler::GeneratePager( ) {
		udPPager	pager		= m_pData->GetPager( );
		udUInt		page		= pager->GetPage( ),
					pagesize	= pager->GetPageSize( ),
					total		= pager->GetTotal( ),
					pagelen		= pager->GetPageLength( );

		if ( page && ( total > 0 ) && ( pagesize > 0 ) && ( pagelen > 0 ) ) {
			if ( page < 1 ) {
				page = 1;
			}
			if ( pagesize < 1 ) {
				pagesize = 1;
			}
			if ( pagelen < 10 ) {
				pagelen = 10;
			}
			udUInt	max_page	= ( udUInt ) ceil( ( float ) total / ( float ) pagesize ),
					cur_frame	= ( udUInt ) ceil( ( float ) page / ( float ) pagelen ),
					start		= ( cur_frame - 1 ) * pagelen,
					end			= cur_frame * pagelen + 1;
			Gtk::PackOptions	opt = Gtk::PACK_SHRINK;
			Gtk::Button	*btn = NULL;

			// предыдущий фрэйм
			if ( start ) {
				btn = Gtk::manage( new Gtk::Button );
				btn->set_name( "PagerPrevFrame" );
				btn->set_size_request( 20, 20 );
				btn->signal_clicked( ).connect( sigc::bind< udInt >(
					sigc::mem_fun( this, &CListHandler::OnButtonPager ),
					start
				) );
				m_hbbPager.pack_start( *btn, opt );
			}
			// предыдущая страница
			if ( page > 1 ) {
				btn = Gtk::manage( new Gtk::Button );
				btn->set_name( "PagerPrevPage" );
				btn->set_size_request( 20, 20 );
				btn->signal_clicked( ).connect( sigc::bind< udInt >(
					sigc::mem_fun( this, &CListHandler::OnButtonPager ),
					page - 1
				) );
				m_hbbPager.pack_start( *btn, opt );
			}
			
			// страницы
			++start;

			for( udUInt i = start; ( i < end ) && ( i <= max_page ); ++i ) {
				btn = Gtk::manage( new Gtk::Button( toString( i ) ) );
				btn->set_size_request( -1, 20 );
				btn->signal_clicked( ).connect( sigc::bind< udInt >(
					sigc::mem_fun( this, &CListHandler::OnButtonPager ),
					i
				) );

				if ( i == page ) {
					btn->set_sensitive( false );
					btn->set_name( "PagerCurrentPage" );
				} else {
					btn->set_name( "PagerPage" );
				}
				
				m_hbbPager.pack_start( *btn, opt );
			}
			
			// следующая страница
			if ( page < max_page ) {
				btn = Gtk::manage( new Gtk::Button );
				btn->set_name( "PagerNextPage" );
				btn->set_size_request( 20, 20 );
				btn->signal_clicked( ).connect( sigc::bind< udInt >(
					sigc::mem_fun( this, &CListHandler::OnButtonPager ),
					page + 1
				) );
				m_hbbPager.pack_start( *btn, opt );
			}
			// следующий фрэйм
			if ( ( end > pagelen ) && ( end < ( max_page + 1 ) ) ) {
				btn = Gtk::manage( new Gtk::Button );
				btn->set_name( "PagerNextFrame" );
				btn->set_size_request( 20, 20 );
				btn->signal_clicked( ).connect( sigc::bind< udInt >(
					sigc::mem_fun( this, &CListHandler::OnButtonPager ),
					end
				) );
				m_hbbPager.pack_start( *btn, opt );
			}
			m_hbbPager.show_all_children( );
		}
	} // void GeneratePager

	void CListHandler::ReplaceDataAccess( udPDAList pData ) {
		udDELETE( m_pData );
		m_pData = pData;
	} // void ReplaceDataAccess

} // namespace UdSDK
