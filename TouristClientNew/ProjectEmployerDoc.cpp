
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWgtEmployerDocPick::CWgtEmployerDocPick( ) :
		udWgtListPick( ),
		m_pHandler( NULL )
	{
		m_szAttr = "fio";
	} // CWgtEmployerDocPick

	CWgtEmployerDocPick::CWgtEmployerDocPick( const CWgtEmployerDocPick& objCopy ) : udWgtListPick( ) {
		m_szAttr = "fio";
	} // CWgtEmployerDocPick

	CWgtEmployerDocPick::~CWgtEmployerDocPick( ) {
	} // ~CWgtEmployerDocPick

	CWgtEmployerDocPick& CWgtEmployerDocPick::operator =( const CWgtEmployerDocPick& objCopy ) {
		return *this;
	} // CWgtEmployerDocPick& operator =

	void CWgtEmployerDocPick::OnButtonClick( ) {
		udPHEmployerDocPick handler = new CHEmployerDocPick;
		if ( m_pHandler ) {
			if ( !m_pHandler->BeforeCreatePickHandler( this, handler ) ) {
				udDELETE( handler );
				return;
			}
		}

		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szWindowTitle );
		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( m_szWindowTitle );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		//wnd.show_all( );
		wnd.show( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj && m_pFde ) {
			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );

			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8(
					obj->get( "series" )->toString( ) + " " +
					obj->get( "number" )->toString( )
				) );
			}
		}

		udDELETE( handler );
	} // void OnButtonClick

	bool CWgtEmployerDocPick::OnClick( GdkEventButton* evtClick ) {
		if ( !m_pFde || !m_pFde->GetObject( ) || !m_pFde->GetObject( )->get( "id" )->toUInt( ) || !m_fnMakeFormHandler || !m_pHandler ) {
			return false;
		}

		udPHClDocFormModal handler = m_pHandler->GetEditHandler( this );
		if ( !handler ) {
			return false;
		}

		Gtk::Window *wndParent = ( Gtk::Window* ) get_toplevel( );
		handler->Init( );
		handler->GetData( )->SetId( m_pFde->GetObject( )->get( "id" )->toUInt( ) );
		handler->Load( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szFormWindowTitle );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( m_szFormWindowTitle );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetData( )->GetData( );
		if ( obj ) {
			string log = "modified object, ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );
			
			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8( obj->get( m_szAttr )->toString( ) ) );
			}
		}

		udDELETE( handler );

		return true;
	} // bool OnClick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPListHandlerPick GetHEmployerDocListPick( ) {
		return new CHEmployerDocPick;
	} // udPListHandlerPick GetHEmployerDocListPick

	CHEmployerDoc::CHEmployerDoc( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblFltSeries( Glib::locale_to_utf8( "Серия" ) ),
		m_lblFltNumber( Glib::locale_to_utf8( "Номер" ) )
	{
		m_szName = "cldoc";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать документ клиента" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "series" )->ini( wstring( L"" ) );
		filter->get( "number" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHEmployerDoc::ColModelEmployerDoc( );

		m_pData->SetName( "cldoc" );
		m_pData->SetGetObject( GetProjectClDocList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDocType );
		pFde->SetMark( fltType );
		m_pData->AddExtraData( pFde );
		m_inpFltType.SetDataObject( pFde );
		m_inpFltType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа" ) );
		m_inpFltType.SetAttr( "name" );
		m_inpFltType.SetGetListHandler( GetHDocTypeListPick );
		m_inpFltType.SetGetFormHandler( GetHDocTypeForm );
		m_inpFltType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование типа документа" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Серия
		m_tblFilter.attach( m_lblFltSeries, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltSeries, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Номер
		m_tblFilter.attach( m_lblFltNumber, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltNumber, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Тип
		m_tblFilter.attach( m_lblFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHEmployerDoc

	CHEmployerDoc::CHEmployerDoc( const CHEmployerDoc& objCopy ) : udListHandler( ) {
		m_szName = "cldoc";
	} // CHEmployerDoc

	CHEmployerDoc::~CHEmployerDoc( ) {
	} // ~CEmployerDoc

	CHEmployerDoc& CHEmployerDoc::operator =( const CHEmployerDoc& objCopy ) {
		return *this;
	} // CHEmployerDoc& operator =

	void CHEmployerDoc::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тип" ), m_pColModel->m_col_type );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Серия" ), m_pColModel->m_col_series );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Номер" ), m_pColModel->m_col_number );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHEmployerDoc::Clear( ) {
		udListHandler::Clear( );
		m_pData->GetFilter( )->get( "client" )->ini( m_iEmployerId );
	} // void Clear

	void CHEmployerDoc::Load( ) {
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
			row[ m_pColModel->m_col_type ] = Glib::locale_to_utf8( obj->get( "type_" )->toString( ) );
			row[ m_pColModel->m_col_series ] = Glib::locale_to_utf8( obj->get( "series" )->toString( ) );
			row[ m_pColModel->m_col_number ] = Glib::locale_to_utf8( obj->get( "number" )->toString( ) );
		}
	} // void Load

	void CHEmployerDoc::FilterUiClear( ) {
		m_inpFltType.SetTitle( "" );
		m_inpFltNumber.Reset( );
		m_inpFltSeries.Reset( );

		m_pData->GetFilter( )->get( "client" )->ini( m_iEmployerId );
	} // void FilterUiClear

	void CHEmployerDoc::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szNumber,
						szSeries;

		szSeries = m_inpFltSeries.GetValue( );
		szNumber = m_inpFltNumber.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "client" )->ini( m_iEmployerId );
		filter->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "series" )->ini( ConvertString( szSeries.data( ), CP_UTF8 ) );
		filter->get( "number" )->ini( ConvertString( szNumber.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	void CHEmployerDoc::SetEmployerId( udUInt iEmployerId ) {
		m_iEmployerId = iEmployerId;
		m_pData->GetFilter( )->get( "client" )->ini( m_iEmployerId );
	} // void SetEmployerId

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHEmployerDocPick::CHEmployerDocPick( ) : CHEmployerDoc( ) {
	} // CHEmployerDocPick

	CHEmployerDocPick::CHEmployerDocPick( const CHEmployerDocPick& objCopy ) : CHEmployerDoc( ) {
	} // CHEmployerDocPick

	CHEmployerDocPick::~CHEmployerDocPick( ) {
	} // ~CHEmployerDocPick

	CHEmployerDocPick& CHEmployerDocPick::operator =( const CHEmployerDocPick& objCopy ) {
		return *this;
	} // CHEmployerDocPick& operator =

	void CHEmployerDocPick::Init( ) {
		g_pApp->GetLog( )->Write( "CHEmployerDocPick::Init, enter\n" );
		g_pApp->GetLog( )->Write( "CHEmployerDocPick::Init, m_wndListWrap->pack_end\n" );
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		g_pApp->GetLog( )->Write( "CHEmployerDocPick::Init, CHEmployerDoc::Init\n" );
		CHEmployerDoc::Init( );
		g_pApp->GetLog( )->Write( "CHEmployerDocPick::Init, exit\n" );
	} // void CHEmployerDocPick

	void CHEmployerDocPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHClDocFormModal handler = new udHClDocFormModal( m_iEmployerId, false );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание документа клиента" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание документа клиента" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHEmployerDocPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectClDoc( );
				m_pPickedObject->apply( itObj->second, true, true );
			}

			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите документ клиента" ),
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

	void CHEmployerDocPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHEmployerDocPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHClDocFormModal handler = new udHClDocFormModal( m_iEmployerId, false );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование документа клиента" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование документа клиента" ) );
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
