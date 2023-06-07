
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPArrBase CHCity::LoadCitiesById( const map< udUInt, udUInt >& ids ) {
		g_pApp->GetLog( )->Write( "LoadCitiesById, enter\n" );
		udPArrBase ret = NULL;
		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( string( "city" ) );
			data.add( ( udInt ) udHCity::opidGet2 );
			data.add( ( udUInt ) ids.size( ) );
			for( map< udUInt, udUInt >::const_iterator i = ids.begin( ); i != ids.end( ); ++i ) {
				data.add( i->first );
				g_pApp->GetLog( )->Write( "id=" + toString( i->first ) + "\n" );
			}
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udArrBase arr;
				udType total( ( udUInt ) 0 );
				total.read( sock );
				udDataAccess::Read( sock, arr, GetProjectCityList );
				string log = "\nсохраняем города\n";
				udPBase add = NULL;
				ret = new udArrBase;
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					add = GetProjectCityList( );
					add->apply( *i, true, true );
					add->dump( log );
					log += "\n";
					ret->push_back( add );
				}
				g_pApp->GetLog( )->Write( log );
				ClearArrBase( arr );
			}
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			sock->Shutdown( );
			sock->Close( );
		}
		udDELETE( sock );
		g_pApp->GetLog( )->Write( "LoadCitiesById, exit\n" );
		return ret;
	} // udPArrBase LoadCitiesById

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPListHandlerPick GetHCityListPick( ) {
		return new CHCityPick;
	} // udPListHandlerPick GetHCityListPick

	udPBase GetProjectCityList( ) {
		udPBase ret = GetProjectCity( );
		ret->get( "country_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectCityList

	CHCity::CHCity( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltCountry( Glib::locale_to_utf8( "Страна" ) )
	{
		m_szName = "city";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать город" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "country" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHCity::ColModelCity( );

		m_pData->SetName( "city" );
		m_pData->SetGetObject( GetProjectCityList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCountry );
		pFde->SetMark( fltCountry );
		m_pData->AddExtraData( pFde );
		m_inpFltCountry.SetDataObject( pFde );
		m_inpFltCountry.SetWindowTitle( Glib::locale_to_utf8( "Выбор страны" ) );
		m_inpFltCountry.SetAttr( "name" );
		m_inpFltCountry.SetGetListHandler( GetHCountryListPick );
		m_inpFltCountry.SetGetFormHandler( GetHCountryForm );
		m_inpFltCountry.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование страны" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Страна
		m_tblFilter.attach( m_lblFltCountry, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCountry, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1, col2, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHCity

	CHCity::CHCity( const CHCity& objCopy ) : udListHandler( ) {
		m_szName = "city";
	} // CHCity

	CHCity::~CHCity( ) {
	} // ~CCity

	CHCity& CHCity::operator =( const CHCity& objCopy ) {
		return *this;
	} // CHCity& operator =

	void CHCity::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Страна" ), m_pColModel->m_col_country );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHCity::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_country ] = Glib::locale_to_utf8( obj->get( "country_" )->toString( ) );
		}
	} // void Load

	void CHCity::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltCountry.SetTitle( "" );
	} // void FilterUiClear

	void CHCity::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "country" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltCountry ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCityModeless::CHCityModeless( ) :
		udListHandlerModeless( ),
		CHCity( )
	{
	} // CHCityModeless

	CHCityModeless::CHCityModeless( const CHCityModeless& objCopy ) : CHCity( ) {
	} // CHCityModeless

	CHCityModeless::~CHCityModeless( ) {
	} // ~CHCityModeless

	CHCityModeless& CHCityModeless::operator =( const CHCityModeless& objCopy ) {
		return *this;
	} // CHCityModeless& operator =

	void CHCityModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHCity::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHCityModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "city_form" );
	} // void OnButtonForm

	void CHCityModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "city_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "city_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCityPick::CHCityPick( ) : CHCity( ) {
	} // CHCityPick

	CHCityPick::CHCityPick( const CHCityPick& objCopy ) : CHCity( ) {
	} // CHCityPick

	CHCityPick::~CHCityPick( ) {
	} // ~CHCityPick

	CHCityPick& CHCityPick::operator =( const CHCityPick& objCopy ) {
		return *this;
	} // CHCityPick& operator =

	void CHCityPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHCity::Init( );
	} // void CHCityPick

	void CHCityPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHCityFormModal handler = new udHCityFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание города" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание города" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHCityPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectCity( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите город" ),
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

	void CHCityPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHCityPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHCityFormModal handler = new udHCityFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование города" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование города" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/
			udDELETE( handler );

			Clear( );
			Load( );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPFormHandler GetHCityForm( ) {
		return new CHCityFormModal;
	} // udPFormHandler GetHCityForm

	CHCityForm::CHCityForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Город" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblCountry( Glib::locale_to_utf8( "Страна" ) )
	{
		m_szName = "city_form";

		m_pData->SetName( "city" );
		m_pData->SetGetObject( GetProjectCity );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblCountry.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );

		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCountry );
		pFde->SetMark( ctrlidCountry );
		m_pData->AddExtraData( pFde );
		m_inpCountry.SetDataObject( pFde );
		m_inpCountry.SetWindowTitle( Glib::locale_to_utf8( "Выбор страны" ) );
		m_inpCountry.SetAttr( "name" );
		m_inpCountry.SetGetListHandler( GetHCountryListPick );
		m_inpCountry.SetGetFormHandler( GetHCountryForm );
		m_inpCountry.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование страны" ) );
	} // CHCityForm

	CHCityForm::CHCityForm( const CHCityForm& objCopy ) : udFormHandler( ) {
	} // CHCityForm

	CHCityForm::~CHCityForm( ) {
	} // ~CHCityForm

	CHCityForm& CHCityForm::operator =( const CHCityForm& objCopy ) {
		return *this;
	} // CHCityForm& operator =

	void CHCityForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK );

		// Страна
		m_tblInput.attach( m_lblCountry, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCountry, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 0, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHCityForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpCountry.SetTitle( "" );
	} // void Clear

	void CHCityForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpCountry.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCountry ) )->GetObject( )->get( "name" )->toString( )
		) );
	} // void Load

	udInt CHCityForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;
		
		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCountry ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите страну" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCountry.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHCityForm::Send( ) {
		Glib::ustring szName;
		
		szName = m_inpName.GetValue( );
		
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		
		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "country" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCountry ) )->GetObject( )->get( "id" )->toUInt( )
		);
		
		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCityFormModal::CHCityFormModal( ) : CHCityForm( ) {
	} // CHCityFormModal

	CHCityFormModal::CHCityFormModal( const CHCityFormModal& objCopy ) : CHCityForm( ) {
	} // CHCityFormModal

	CHCityFormModal::~CHCityFormModal( ) {
	} // ~CHCityFormModal

	CHCityFormModal& CHCityFormModal::operator =( const CHCityFormModal& objCopy ) {
		return *this;
	} // CHCityFormModal& operator =

	void CHCityFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHCityFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHCityForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCityFormModeless::CHCityFormModeless( ) : CHCityForm( ) {
	} // CHCityFormModeless

	CHCityFormModeless::CHCityFormModeless( const CHCityFormModeless& objCopy ) : CHCityForm( ) {
	} // CHCityFormModeless

	CHCityFormModeless::~CHCityFormModeless( ) {
	} // ~CHCityFormModeless

	CHCityFormModeless& CHCityFormModeless::operator =( const CHCityFormModeless& objCopy ) {
		return *this;
	} // CHCityFormModeless& operator =

	void CHCityFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "city" );
	} // void OnButtonOk

	void CHCityFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "city" );
	} // void CHCityForm

	void CHCityFormModeless::Init( ) {
		CHCityForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHCityFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHCityFormModeless

} // namespace UdSDK
