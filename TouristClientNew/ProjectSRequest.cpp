
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHSRequestListPick( ) {
		return new CHSRequestPick;
	} // udPListHandlerPick GetHSRequestListPick

	udPBase GetProjectSRequestList( ) {
		udPBase ret = GetProjectSRequest( );
		return ret;
	} // udPBase GetProjectSRequestList

	CHSRequest::CHSRequest( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "srequest";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать состояние обращения" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHSRequest::ColModelSRequest( );

		m_pData->SetName( "srequest" );
		m_pData->SetGetObject( GetProjectSRequestList );
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
	} // CHSRequest

	CHSRequest::CHSRequest( const CHSRequest& objCopy ) : udListHandler( ) {
		m_szName = "srequest";
	} // CHSRequest

	CHSRequest::~CHSRequest( ) {
	} // ~CSRequest

	CHSRequest& CHSRequest::operator =( const CHSRequest& objCopy ) {
		return *this;
	} // CHSRequest& operator =

	void CHSRequest::Init( ) {
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

	void CHSRequest::Load( ) {
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

	void CHSRequest::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHSRequest::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSRequestModeless::CHSRequestModeless( ) :
		udListHandlerModeless( ),
		CHSRequest( )
	{
	} // CHSRequestModeless

	CHSRequestModeless::CHSRequestModeless( const CHSRequestModeless& objCopy ) : CHSRequest( ) {
	} // CHSRequestModeless

	CHSRequestModeless::~CHSRequestModeless( ) {
	} // ~CHSRequestModeless

	CHSRequestModeless& CHSRequestModeless::operator =( const CHSRequestModeless& objCopy ) {
		return *this;
	} // CHSRequestModeless& operator =

	void CHSRequestModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHSRequest::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHSRequestModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "srequest_form" );
	} // void OnButtonForm

	void CHSRequestModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "srequest_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "srequest_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSRequestPick::CHSRequestPick( ) : CHSRequest( ) {
	} // CHSRequestPick

	CHSRequestPick::CHSRequestPick( const CHSRequestPick& objCopy ) : CHSRequest( ) {
	} // CHSRequestPick

	CHSRequestPick::~CHSRequestPick( ) {
	} // ~CHSRequestPick

	CHSRequestPick& CHSRequestPick::operator =( const CHSRequestPick& objCopy ) {
		return *this;
	} // CHSRequestPick& operator =

	void CHSRequestPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHSRequest::Init( );
	} // void CHSRequestPick

	void CHSRequestPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHSRequestFormModal handler = new udHSRequestFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание состояния обращения" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание состояния обращения" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHSRequestPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectSRequest( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите состояние обращения" ),
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

	void CHSRequestPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHSRequestPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHSRequestFormModal handler = new udHSRequestFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование состояния обращения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование состояния обращения" ) );
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

	udPFormHandler GetHSRequestForm( ) {
		return new CHSRequestFormModal;
	} // udPFormHandler GetHSRequestForm
	
	CHSRequestForm::CHSRequestForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Состояние обращения" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "srequest_form";

		m_pData->SetName( "srequest" );
		m_pData->SetGetObject( GetProjectSRequest );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHSRequestForm

	CHSRequestForm::CHSRequestForm( const CHSRequestForm& objCopy ) : udFormHandler( ) {
	} // CHSRequestForm

	CHSRequestForm::~CHSRequestForm( ) {
	} // ~CHSRequestForm

	CHSRequestForm& CHSRequestForm::operator =( const CHSRequestForm& objCopy ) {
		return *this;
	} // CHSRequestForm& operator =

	void CHSRequestForm::Init( ) {
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

	void CHSRequestForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHSRequestForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHSRequestForm::Validate( ) {
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

	void CHSRequestForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSRequestFormModal::CHSRequestFormModal( ) : CHSRequestForm( ) {
	} // CHSRequestFormModal

	CHSRequestFormModal::CHSRequestFormModal( const CHSRequestFormModal& objCopy ) : CHSRequestForm( ) {
	} // CHSRequestFormModal

	CHSRequestFormModal::~CHSRequestFormModal( ) {
	} // ~CHSRequestFormModal

	CHSRequestFormModal& CHSRequestFormModal::operator =( const CHSRequestFormModal& objCopy ) {
		return *this;
	} // CHSRequestFormModal& operator =

	void CHSRequestFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHSRequestFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHSRequestForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSRequestFormModeless::CHSRequestFormModeless( ) : CHSRequestForm( ) {
	} // CHSRequestFormModeless

	CHSRequestFormModeless::CHSRequestFormModeless( const CHSRequestFormModeless& objCopy ) : CHSRequestForm( ) {
	} // CHSRequestFormModeless

	CHSRequestFormModeless::~CHSRequestFormModeless( ) {
	} // ~CHSRequestFormModeless

	CHSRequestFormModeless& CHSRequestFormModeless::operator =( const CHSRequestFormModeless& objCopy ) {
		return *this;
	} // CHSRequestFormModeless& operator =

	void CHSRequestFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "srequest" );
	} // void OnButtonOk

	void CHSRequestFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "srequest" );
	} // void CHSRequestForm

	void CHSRequestFormModeless::Init( ) {
		CHSRequestForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHSRequestFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHSRequestFormModeless

} // namespace UdSDK
