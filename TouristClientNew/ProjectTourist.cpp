
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDAListTourist::CDAListTourist( ) :
		udDAList( ),
		m_pTourForm( NULL )
	{
	} // CDAListTourist

	CDAListTourist::CDAListTourist( const CDAListTourist& objCopy ) {
	} // CDAListTourist

	CDAListTourist::~CDAListTourist( ) {
	} // ~CDAListTourist

	CDAListTourist& CDAListTourist::operator =( const CDAListTourist& objCopy ) {
		return *this;
	} // CDAListTourist& operator =

	void CDAListTourist::Load( ) {
		if ( m_pTourForm ) {
			udPBase add = NULL;
			udPArrBase arrObject = ( ( udPFDEList ) m_pTourForm->GetData( )->GetExtraData( udHTourForm::ctrlidClient ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				map< udUInt, udUInt > ids;
				string log = "\npick_tourist_ids:\n";
				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					ids[ ( *i )->get( "client" )->toUInt( ) ] = 1;
					log += "id=" + ( *i )->get( "client" )->toString( ) + "\n";
				}
				g_pApp->GetLog( )->Write( log );
				udPSocket sock = g_pApp->BeginRequest( );
				if ( sock ) {
					// ListRequest
					udBinaryData data;
					data.add( m_szName );
					data.add( ( udInt ) udHClient::opidGet2 );
					data.add( m_pPager->GetPage( ) );
					data.add( m_pPager->GetPageSize( ) );
					data.add( m_szOrderBy );
					data.add( m_iOrderType );
					// добавляем набор ID клиентов
					data.add( ids.size( ) );
					for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
						data.add( i->first );
					}

					log = "\nфильтр, ";
					m_pFilter->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );
					m_pFilter->dump( data );

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );

					char code = 0;
					sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
					sock->ReadByte( &code );
					if ( code > 0 ) { // необходимо считать объекты
						udType total( ( udUInt ) 0 );
						total.read( sock );
						m_pPager->SetTotal( total.toUInt( ) );
						udDataAccess::Read( sock, *m_arrObject, m_fnGetObject );
					}

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				}
			}
		}
	} // void Load

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourist::CHTourist( ) :
		udListHandler( ),
		m_lblFltFio( Glib::locale_to_utf8( "ФИО" ) ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата рождения с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		udDELETE( m_pData );

		m_pData = new CDAListTourist;

		m_szName = "client";

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "fio" )->ini( wstring( L"" ) );
		filter->get( "date1" )->ini( wstring( L"" ) );
		filter->get( "date2" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHTourist::ColModelTourist( );

		m_pData->SetName( "client" );
		m_pData->SetGetObject( GetProjectClientList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// ФИО
		m_tblFilter.attach( m_lblFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата рождения с
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата рождения по
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHClient

	CHTourist::CHTourist( const CHTourist& objCopy ) : udListHandler( ) {
		m_szName = "client";
	} // CHTourist

	CHTourist::~CHTourist( ) {
	} // ~CHTourist

	CHTourist& CHTourist::operator =( const CHTourist& objCopy ) {
		return *this;
	} // CHClient& operator =

	void CHTourist::Init( ) {
		//m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "ФИО" ), m_pColModel->m_col_fio );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата рождения" ), m_pColModel->m_col_born );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHTourist::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		Gtk::TreeModel::iterator itRow;
		Gtk::TreePath objPath;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			itRow = m_objTreeModel->append( );
			objPath = m_objTreeModel->get_path( itRow );
			m_mapObject[ objPath.to_string( ) ] = obj;
			row = *itRow;
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_fio ] = Glib::locale_to_utf8( obj->get( "fio" )->toString( ) );
			row[ m_pColModel->m_col_born ] = Glib::locale_to_utf8( obj->get( "born" )->toString( ) );
		}
	} // void Load

	void CHTourist::FilterUiClear( ) {
		m_inpFltFio.Reset( );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHTourist::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szFio;
		wstring			szDate1,
						szDate2;

		szFio = m_inpFltFio.GetValue( );
		szDate1 = m_inpFltDate1.GetValue( ).toWString( "%Y-%m-%d" );
		szDate2 = m_inpFltDate1.GetValue( ).toWString( "%Y-%m-%d" );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		filter->get( "date1" )->ini( szDate1 );
		filter->get( "date2" )->ini( szDate2 );
	} // void FilterUiToObject

	void CHTourist::SetTourForm( udPHTourForm pTourForm ) {
		m_pTourForm = pTourForm;
		( ( CDAListTourist* ) m_pData )->SetTourForm( pTourForm );
	} // void SetTourForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTouristPick::CHTouristPick( ) : CHTourist( ) {
	} // CHTouristPick

	CHTouristPick::CHTouristPick( const CHTouristPick& objCopy ) : CHTourist( ) {
	} // CHTouristPick

	CHTouristPick::~CHTouristPick( ) {
	} // ~CHTouristPick

	CHTouristPick& CHTouristPick::operator =( const CHTouristPick& objCopy ) {
		return *this;
	} // CHTouristPick& operator =

	void CHTouristPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTourist::Init( );
	} // void CHTouristPick

	void CHTouristPick::OnButtonForm( ) {
		// надо подумать, а не прикрутить ли механизм синхронизации с формой тура
		// было бы замечательно
		/*Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHClientFormModal handler = new udHClientFormModal( );
		handler->Init( );
		Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание клиента" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );
		udDELETE( handler );

		Clear( );
		Load( );*/
	} // void OnButtonForm

	void CHTouristPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectClient( );
				m_pPickedObject->apply( itObj->second, true, true );
			}
			
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите клиента" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( Glib::locale_to_utf8(
				"Щелкните по строке списка и нажмите кнопку \"ОК\""
			) );
			dialog.run( );
		}
	} // void OnButtonOk

	void CHTouristPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTouristPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHClientFormModal handler = new udHClientFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование клиента" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование клиента" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/
			udDELETE( handler );

			Clear( );
			Load( );
		}
	} // void OnRowActivated

} // namespace UdSDK
