
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHMUServiceListPick( ) {
		return new CHMUServicePick;
	} // udPListHandlerPick GetHMUServiceListPick

	udPBase GetProjectMUServiceList( ) {
		udPBase ret = GetProjectMUService( );
		return ret;
	} // udPBase GetProjectMUServiceList

	CHMUService::CHMUService( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "muservice";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать единицу измерения услуги" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHMUService::ColModelMUService( );

		m_pData->SetName( "muservice" );
		m_pData->SetGetObject( GetProjectMUServiceList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHMUService

	CHMUService::CHMUService( const CHMUService& objCopy ) : udListHandler( ) {
		m_szName = "muservice";
	} // CHMUService

	CHMUService::~CHMUService( ) {
	} // ~CMUService

	CHMUService& CHMUService::operator =( const CHMUService& objCopy ) {
		return *this;
	} // CHMUService& operator =

	void CHMUService::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHMUService::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
		}
	} // void Load

	void CHMUService::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHMUService::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHMUServiceModeless::CHMUServiceModeless( ) :
		udListHandlerModeless( ),
		CHMUService( )
	{
	} // CHMUServiceModeless

	CHMUServiceModeless::CHMUServiceModeless( const CHMUServiceModeless& objCopy ) : CHMUService( ) {
	} // CHMUServiceModeless

	CHMUServiceModeless::~CHMUServiceModeless( ) {
	} // ~CHMUServiceModeless

	CHMUServiceModeless& CHMUServiceModeless::operator =( const CHMUServiceModeless& objCopy ) {
		return *this;
	} // CHMUServiceModeless& operator =

	void CHMUServiceModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHMUService::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHMUServiceModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "muservice_form" );
	} // void OnButtonForm

	void CHMUServiceModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "muservice_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "muservice_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHMUServicePick::CHMUServicePick( ) : CHMUService( ) {
	} // CHMUServicePick

	CHMUServicePick::CHMUServicePick( const CHMUServicePick& objCopy ) : CHMUService( ) {
	} // CHMUServicePick

	CHMUServicePick::~CHMUServicePick( ) {
	} // ~CHMUServicePick

	CHMUServicePick& CHMUServicePick::operator =( const CHMUServicePick& objCopy ) {
		return *this;
	} // CHMUServicePick& operator =

	void CHMUServicePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHMUService::Init( );
	} // void CHMUServicePick

	void CHMUServicePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHMUServiceFormModal handler = new udHMUServiceFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание единицы измерения услуги" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание единицы измерения услуги" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHMUServicePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectMUService( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите единицу измерения услуги" ),
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

	void CHMUServicePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHMUServicePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHMUServiceFormModal handler = new udHMUServiceFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование единицы измерения услуги" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование единицы измерения услуги" ) );
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

	udPFormHandler GetHMUServiceForm( ) {
		return new CHMUServiceFormModal;
	} // udPFormHandler GetHMUServiceForm

	CHMUServiceForm::CHMUServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Единица измерения услуги" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "muservice_form";

		m_pData->SetName( "muservice" );
		m_pData->SetGetObject( GetProjectMUService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHMUServiceForm

	CHMUServiceForm::CHMUServiceForm( const CHMUServiceForm& objCopy ) : udFormHandler( ) {
	} // CHMUServiceForm

	CHMUServiceForm::~CHMUServiceForm( ) {
	} // ~CHMUServiceForm

	CHMUServiceForm& CHMUServiceForm::operator =( const CHMUServiceForm& objCopy ) {
		return *this;
	} // CHMUServiceForm& operator =

	void CHMUServiceForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHMUServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	udInt CHMUServiceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHMUServiceForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	void CHMUServiceForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHMUServiceFormModal::CHMUServiceFormModal( ) : CHMUServiceForm( ) {
	} // CHMUServiceFormModal

	CHMUServiceFormModal::CHMUServiceFormModal( const CHMUServiceFormModal& objCopy ) : CHMUServiceForm( ) {
	} // CHMUServiceFormModal

	CHMUServiceFormModal::~CHMUServiceFormModal( ) {
	} // ~CHMUServiceFormModal

	CHMUServiceFormModal& CHMUServiceFormModal::operator =( const CHMUServiceFormModal& objCopy ) {
		return *this;
	} // CHMUServiceFormModal& operator =

	void CHMUServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHMUServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHMUServiceForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHMUServiceFormModeless::CHMUServiceFormModeless( ) : CHMUServiceForm( ) {
	} // CHMUServiceFormModeless

	CHMUServiceFormModeless::CHMUServiceFormModeless( const CHMUServiceFormModeless& objCopy ) : CHMUServiceForm( ) {
	} // CHMUServiceFormModeless

	CHMUServiceFormModeless::~CHMUServiceFormModeless( ) {
	} // ~CHMUServiceFormModeless

	CHMUServiceFormModeless& CHMUServiceFormModeless::operator =( const CHMUServiceFormModeless& objCopy ) {
		return *this;
	} // CHMUServiceFormModeless& operator =

	void CHMUServiceFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "muservice" );
	} // void OnButtonOk

	void CHMUServiceFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "muservice" );
	} // void CHMUServiceForm

	void CHMUServiceFormModeless::Init( ) {
		CHMUServiceForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHMUServiceFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHMUServiceFormModeless

} // namespace UdSDK
