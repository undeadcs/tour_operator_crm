
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHRRequestListPick( ) {
		return new CHRRequestPick;
	} // udPListHandlerPick GetHRRequestListPick

	udPBase GetProjectRRequestList( ) {
		udPBase ret = GetProjectRRequest( );
		return ret;
	} // udPBase GetProjectRRequestList

	CHRRequest::CHRRequest( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "rrequest";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать результат обращения" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHRRequest::ColModelRRequest( );

		m_pData->SetName( "rrequest" );
		m_pData->SetGetObject( GetProjectRRequestList );
		///////////////////////////////////////////////////////////////////////////////////////////////////////
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
	} // CHRRequest

	CHRRequest::CHRRequest( const CHRRequest& objCopy ) : udListHandler( ) {
		m_szName = "rrequest";
	} // CHRRequest

	CHRRequest::~CHRRequest( ) {
	} // ~CRRequest

	CHRRequest& CHRRequest::operator =( const CHRRequest& objCopy ) {
		return *this;
	} // CHRRequest& operator =

	void CHRRequest::Init( ) {
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

	void CHRRequest::Load( ) {
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

	void CHRRequest::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHRRequest::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRRequestModeless::CHRRequestModeless( ) :
		udListHandlerModeless( ),
		CHRRequest( )
	{
	} // CHRRequestModeless

	CHRRequestModeless::CHRRequestModeless( const CHRRequestModeless& objCopy ) : CHRRequest( ) {
	} // CHRRequestModeless

	CHRRequestModeless::~CHRRequestModeless( ) {
	} // ~CHRRequestModeless

	CHRRequestModeless& CHRRequestModeless::operator =( const CHRRequestModeless& objCopy ) {
		return *this;
	} // CHRRequestModeless& operator =

	void CHRRequestModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHRRequest::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHRRequestModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "rrequest_form" );
	} // void OnButtonForm

	void CHRRequestModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "rrequest_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "rrequest_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRRequestPick::CHRRequestPick( ) : CHRRequest( ) {
	} // CHRRequestPick

	CHRRequestPick::CHRRequestPick( const CHRRequestPick& objCopy ) : CHRRequest( ) {
	} // CHRRequestPick

	CHRRequestPick::~CHRRequestPick( ) {
	} // ~CHRRequestPick

	CHRRequestPick& CHRRequestPick::operator =( const CHRRequestPick& objCopy ) {
		return *this;
	} // CHRRequestPick& operator =

	void CHRRequestPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHRRequest::Init( );
	} // void CHRRequestPick

	void CHRRequestPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHRRequestFormModal handler = new udHRRequestFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание результата обращения" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание результата обращения" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHRRequestPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectRRequest( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите результат обращения" ),
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

	void CHRRequestPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHRRequestPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHRRequestFormModal handler = new udHRRequestFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование результата обращения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование результата обращения" ) );
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

	udPFormHandler GetHRRequestForm( ) {
		return new CHRRequestFormModal;
	} // udPFormHandler GetHRRequestForm

	CHRRequestForm::CHRRequestForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Результат обращения" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "rrequest_form";

		m_pData->SetName( "rrequest" );
		m_pData->SetGetObject( GetProjectRRequest );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHRRequestForm

	CHRRequestForm::CHRRequestForm( const CHRRequestForm& objCopy ) : udFormHandler( ) {
	} // CHRRequestForm

	CHRRequestForm::~CHRRequestForm( ) {
	} // ~CHRRequestForm

	CHRRequestForm& CHRRequestForm::operator =( const CHRRequestForm& objCopy ) {
		return *this;
	} // CHRRequestForm& operator =

	void CHRRequestForm::Init( ) {
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

	void CHRRequestForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHRRequestForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHRRequestForm::Validate( ) {
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

	void CHRRequestForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRRequestFormModal::CHRRequestFormModal( ) : CHRRequestForm( ) {
	} // CHRRequestFormModal

	CHRRequestFormModal::CHRRequestFormModal( const CHRRequestFormModal& objCopy ) : CHRRequestForm( ) {
	} // CHRRequestFormModal

	CHRRequestFormModal::~CHRRequestFormModal( ) {
	} // ~CHRRequestFormModal

	CHRRequestFormModal& CHRRequestFormModal::operator =( const CHRRequestFormModal& objCopy ) {
		return *this;
	} // CHRRequestFormModal& operator =

	void CHRRequestFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHRRequestFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHRRequestForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRRequestFormModeless::CHRRequestFormModeless( ) : CHRRequestForm( ) {
	} // CHRRequestFormModeless

	CHRRequestFormModeless::CHRRequestFormModeless( const CHRRequestFormModeless& objCopy ) : CHRRequestForm( ) {
	} // CHRRequestFormModeless

	CHRRequestFormModeless::~CHRRequestFormModeless( ) {
	} // ~CHRRequestFormModeless

	CHRRequestFormModeless& CHRRequestFormModeless::operator =( const CHRRequestFormModeless& objCopy ) {
		return *this;
	} // CHRRequestFormModeless& operator =

	void CHRRequestFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "rrequest" );
	} // void OnButtonOk

	void CHRRequestFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "rrequest" );
	} // void CHRRequestForm

	void CHRRequestFormModeless::Init( ) {
		CHRRequestForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHRRequestFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHRRequestFormModeless

} // namespace UdSDK
