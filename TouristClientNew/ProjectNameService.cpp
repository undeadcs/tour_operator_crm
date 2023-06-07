
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHNameServiceListPick( ) {
		return new CHNameServicePick;
	} // udPListHandlerPick GetHNameServiceListPick

	udPBase GetProjectNameServiceList( ) {
		udPBase ret = GetProjectNameService( );
		return ret;
	} // udPBase GetProjectNameServiceList

	CHNameService::CHNameService( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "nameservice";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать наименование услуги" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHNameService::ColModelNameService( );

		m_pData->SetName( "nameservice" );
		m_pData->SetGetObject( GetProjectNameServiceList );
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
	} // CHNameService

	CHNameService::CHNameService( const CHNameService& objCopy ) : udListHandler( ) {
		m_szName = "nameservice";
	} // CHNameService

	CHNameService::~CHNameService( ) {
	} // ~CNameService

	CHNameService& CHNameService::operator =( const CHNameService& objCopy ) {
		return *this;
	} // CHNameService& operator =

	void CHNameService::Init( ) {
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

	void CHNameService::Load( ) {
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

	void CHNameService::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHNameService::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHNameServiceModeless::CHNameServiceModeless( ) :
		udListHandlerModeless( ),
		CHNameService( )
	{
	} // CHNameServiceModeless

	CHNameServiceModeless::CHNameServiceModeless( const CHNameServiceModeless& objCopy ) : CHNameService( ) {
	} // CHNameServiceModeless

	CHNameServiceModeless::~CHNameServiceModeless( ) {
	} // ~CHNameServiceModeless

	CHNameServiceModeless& CHNameServiceModeless::operator =( const CHNameServiceModeless& objCopy ) {
		return *this;
	} // CHNameServiceModeless& operator =

	void CHNameServiceModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHNameService::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHNameServiceModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "nameservice_form" );
	} // void OnButtonForm

	void CHNameServiceModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "nameservice_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "nameservice_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHNameServicePick::CHNameServicePick( ) : CHNameService( ) {
	} // CHNameServicePick

	CHNameServicePick::CHNameServicePick( const CHNameServicePick& objCopy ) : CHNameService( ) {
	} // CHNameServicePick

	CHNameServicePick::~CHNameServicePick( ) {
	} // ~CHNameServicePick

	CHNameServicePick& CHNameServicePick::operator =( const CHNameServicePick& objCopy ) {
		return *this;
	} // CHNameServicePick& operator =

	void CHNameServicePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHNameService::Init( );
	} // void CHNameServicePick

	void CHNameServicePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHNameServiceFormModal handler = new udHNameServiceFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание наименования услуги" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание наименования услуги" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHNameServicePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectNameService( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите наименование услуги" ),
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

	void CHNameServicePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHNameServicePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHNameServiceFormModal handler = new udHNameServiceFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование наименования услуги" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование наименования услуги" ) );
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

	udPFormHandler GetHNameServiceForm( ) {
		return new CHNameServiceFormModal;
	} // udPFormHandler GetHNameServiceForm

	CHNameServiceForm::CHNameServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Наименование услуги" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "nameservice_form";

		m_pData->SetName( "nameservice" );
		m_pData->SetGetObject( GetProjectNameService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHNameServiceForm

	CHNameServiceForm::CHNameServiceForm( const CHNameServiceForm& objCopy ) : udFormHandler( ) {
	} // CHNameServiceForm

	CHNameServiceForm::~CHNameServiceForm( ) {
	} // ~CHNameServiceForm

	CHNameServiceForm& CHNameServiceForm::operator =( const CHNameServiceForm& objCopy ) {
		return *this;
	} // CHNameServiceForm& operator =

	void CHNameServiceForm::Init( ) {
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

	void CHNameServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHNameServiceForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHNameServiceForm::Validate( ) {
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

	void CHNameServiceForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHNameServiceFormModal::CHNameServiceFormModal( ) : CHNameServiceForm( ) {
	} // CHNameServiceFormModal

	CHNameServiceFormModal::CHNameServiceFormModal( const CHNameServiceFormModal& objCopy ) : CHNameServiceForm( ) {
	} // CHNameServiceFormModal

	CHNameServiceFormModal::~CHNameServiceFormModal( ) {
	} // ~CHNameServiceFormModal

	CHNameServiceFormModal& CHNameServiceFormModal::operator =( const CHNameServiceFormModal& objCopy ) {
		return *this;
	} // CHNameServiceFormModal& operator =

	void CHNameServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHNameServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHNameServiceForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHNameServiceFormModeless::CHNameServiceFormModeless( ) : CHNameServiceForm( ) {
	} // CHNameServiceFormModeless

	CHNameServiceFormModeless::CHNameServiceFormModeless( const CHNameServiceFormModeless& objCopy ) : CHNameServiceForm( ) {
	} // CHNameServiceFormModeless

	CHNameServiceFormModeless::~CHNameServiceFormModeless( ) {
	} // ~CHNameServiceFormModeless

	CHNameServiceFormModeless& CHNameServiceFormModeless::operator =( const CHNameServiceFormModeless& objCopy ) {
		return *this;
	} // CHNameServiceFormModeless& operator =

	void CHNameServiceFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "nameservice" );
	} // void OnButtonOk

	void CHNameServiceFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "nameservice" );
	} // void CHNameServiceForm

	void CHNameServiceFormModeless::Init( ) {
		CHNameServiceForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHNameServiceFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHNameServiceFormModeless

} // namespace UdSDK
