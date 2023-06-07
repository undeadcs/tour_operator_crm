
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHTServiceListPick( ) {
		return new CHTServicePick;
	} // udPListHandlerPick GetHTServiceListPick

	udPBase GetProjectTServiceList( ) {
		udPBase ret = GetProjectTService( );
		return ret;
	} // udPBase GetProjectTServiceList

	CHTService::CHTService( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "tservice";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать тип услуги" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHTService::ColModelTService( );

		m_pData->SetName( "tservice" );
		m_pData->SetGetObject( GetProjectTServiceList );
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
	} // CHTService

	CHTService::CHTService( const CHTService& objCopy ) : udListHandler( ) {
		m_szName = "tservice";
	} // CHTService

	CHTService::~CHTService( ) {
	} // ~CTService

	CHTService& CHTService::operator =( const CHTService& objCopy ) {
		return *this;
	} // CHTService& operator =

	void CHTService::Init( ) {
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

	void CHTService::Load( ) {
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

	void CHTService::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHTService::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTServiceModeless::CHTServiceModeless( ) :
		udListHandlerModeless( ),
		CHTService( )
	{
	} // CHTServiceModeless

	CHTServiceModeless::CHTServiceModeless( const CHTServiceModeless& objCopy ) : CHTService( ) {
	} // CHTServiceModeless

	CHTServiceModeless::~CHTServiceModeless( ) {
	} // ~CHTServiceModeless

	CHTServiceModeless& CHTServiceModeless::operator =( const CHTServiceModeless& objCopy ) {
		return *this;
	} // CHTServiceModeless& operator =

	void CHTServiceModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHTService::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHTServiceModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "tservice_form" );
	} // void OnButtonForm

	void CHTServiceModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "tservice_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "tservice_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTServicePick::CHTServicePick( ) : CHTService( ) {
	} // CHTServicePick

	CHTServicePick::CHTServicePick( const CHTServicePick& objCopy ) : CHTService( ) {
	} // CHTServicePick

	CHTServicePick::~CHTServicePick( ) {
	} // ~CHTServicePick

	CHTServicePick& CHTServicePick::operator =( const CHTServicePick& objCopy ) {
		return *this;
	} // CHTServicePick& operator =

	void CHTServicePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTService::Init( );
	} // void CHTServicePick

	void CHTServicePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHTServiceFormModal handler = new udHTServiceFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание типа услуги" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание типа услуги" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHTServicePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectTService( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите тип услуги" ),
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

	void CHTServicePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTServicePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHTServiceFormModal handler = new udHTServiceFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование тип услуги" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование тип услуги" ) );
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

	udPFormHandler GetHTServiceForm( ) {
		return new CHTServiceFormModal;
	} // udPFormHandler GetHTServiceForm

	CHTServiceForm::CHTServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Тип услуги" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "tservice_form";

		m_pData->SetName( "tservice" );
		m_pData->SetGetObject( GetProjectTService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHTServiceForm

	CHTServiceForm::CHTServiceForm( const CHTServiceForm& objCopy ) : udFormHandler( ) {
	} // CHTServiceForm

	CHTServiceForm::~CHTServiceForm( ) {
	} // ~CHTServiceForm

	CHTServiceForm& CHTServiceForm::operator =( const CHTServiceForm& objCopy ) {
		return *this;
	} // CHTServiceForm& operator =

	void CHTServiceForm::Init( ) {
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

	void CHTServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHTServiceForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHTServiceForm::Validate( ) {
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

	void CHTServiceForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTServiceFormModal::CHTServiceFormModal( ) : CHTServiceForm( ) {
	} // CHTServiceFormModal

	CHTServiceFormModal::CHTServiceFormModal( const CHTServiceFormModal& objCopy ) : CHTServiceForm( ) {
	} // CHTServiceFormModal

	CHTServiceFormModal::~CHTServiceFormModal( ) {
	} // ~CHTServiceFormModal

	CHTServiceFormModal& CHTServiceFormModal::operator =( const CHTServiceFormModal& objCopy ) {
		return *this;
	} // CHTServiceFormModal& operator =

	void CHTServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTServiceForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTServiceFormModeless::CHTServiceFormModeless( ) : CHTServiceForm( ) {
	} // CHTServiceFormModeless

	CHTServiceFormModeless::CHTServiceFormModeless( const CHTServiceFormModeless& objCopy ) : CHTServiceForm( ) {
	} // CHTServiceFormModeless

	CHTServiceFormModeless::~CHTServiceFormModeless( ) {
	} // ~CHTServiceFormModeless

	CHTServiceFormModeless& CHTServiceFormModeless::operator =( const CHTServiceFormModeless& objCopy ) {
		return *this;
	} // CHTServiceFormModeless& operator =

	void CHTServiceFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "tservice" );
	} // void OnButtonOk

	void CHTServiceFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "tservice" );
	} // void CHTServiceForm

	void CHTServiceFormModeless::Init( ) {
		CHTServiceForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHTServiceFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHTServiceFormModeless

} // namespace UdSDK
