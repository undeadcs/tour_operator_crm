
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHTRequestListPick( ) {
		return new CHTRequestPick;
	} // udPListHandlerPick GetHTRequestListPick

	udPBase GetProjectTRequestList( ) {
		udPBase ret = GetProjectTRequest( );
		return ret;
	} // udPBase GetProjectTRequestList

	CHTRequest::CHTRequest( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "trequest";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать тип обращения" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHTRequest::ColModelTRequest( );

		m_pData->SetName( "trequest" );
		m_pData->SetGetObject( GetProjectTRequestList );
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
	} // CHTRequest

	CHTRequest::CHTRequest( const CHTRequest& objCopy ) : udListHandler( ) {
		m_szName = "trequest";
	} // CHTRequest

	CHTRequest::~CHTRequest( ) {
	} // ~CTRequest

	CHTRequest& CHTRequest::operator =( const CHTRequest& objCopy ) {
		return *this;
	} // CHTRequest& operator =

	void CHTRequest::Init( ) {
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

	void CHTRequest::Load( ) {
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

	void CHTRequest::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHTRequest::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTRequestModeless::CHTRequestModeless( ) :
		udListHandlerModeless( ),
		CHTRequest( )
	{
	} // CHTRequestModeless

	CHTRequestModeless::CHTRequestModeless( const CHTRequestModeless& objCopy ) : CHTRequest( ) {
	} // CHTRequestModeless

	CHTRequestModeless::~CHTRequestModeless( ) {
	} // ~CHTRequestModeless

	CHTRequestModeless& CHTRequestModeless::operator =( const CHTRequestModeless& objCopy ) {
		return *this;
	} // CHTRequestModeless& operator =

	void CHTRequestModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHTRequest::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHTRequestModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "trequest_form" );
	} // void OnButtonForm

	void CHTRequestModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "trequest_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "trequest_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTRequestPick::CHTRequestPick( ) : CHTRequest( ) {
	} // CHTRequestPick

	CHTRequestPick::CHTRequestPick( const CHTRequestPick& objCopy ) : CHTRequest( ) {
	} // CHTRequestPick

	CHTRequestPick::~CHTRequestPick( ) {
	} // ~CHTRequestPick

	CHTRequestPick& CHTRequestPick::operator =( const CHTRequestPick& objCopy ) {
		return *this;
	} // CHTRequestPick& operator =

	void CHTRequestPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTRequest::Init( );
	} // void CHTRequestPick

	void CHTRequestPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHTRequestFormModal handler = new udHTRequestFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание типа обращения" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание типа обращения" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHTRequestPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectTRequest( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите тип обращения" ),
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

	void CHTRequestPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTRequestPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHTRequestFormModal handler = new udHTRequestFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование тип обращения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование тип обращения" ) );
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

	udPFormHandler GetHTRequestForm( ) {
		return new CHTRequestFormModal;
	} // udPFormHandler GetHTRequestForm

	CHTRequestForm::CHTRequestForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Тип обращения" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "trequest_form";

		m_pData->SetName( "trequest" );
		m_pData->SetGetObject( GetProjectTRequest );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHTRequestForm

	CHTRequestForm::CHTRequestForm( const CHTRequestForm& objCopy ) : udFormHandler( ) {
	} // CHTRequestForm

	CHTRequestForm::~CHTRequestForm( ) {
	} // ~CHTRequestForm

	CHTRequestForm& CHTRequestForm::operator =( const CHTRequestForm& objCopy ) {
		return *this;
	} // CHTRequestForm& operator =

	void CHTRequestForm::Init( ) {
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

	void CHTRequestForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHTRequestForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHTRequestForm::Validate( ) {
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

	void CHTRequestForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTRequestFormModal::CHTRequestFormModal( ) : CHTRequestForm( ) {
	} // CHTRequestFormModal

	CHTRequestFormModal::CHTRequestFormModal( const CHTRequestFormModal& objCopy ) : CHTRequestForm( ) {
	} // CHTRequestFormModal

	CHTRequestFormModal::~CHTRequestFormModal( ) {
	} // ~CHTRequestFormModal

	CHTRequestFormModal& CHTRequestFormModal::operator =( const CHTRequestFormModal& objCopy ) {
		return *this;
	} // CHTRequestFormModal& operator =

	void CHTRequestFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTRequestFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTRequestForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTRequestFormModeless::CHTRequestFormModeless( ) : CHTRequestForm( ) {
	} // CHTRequestFormModeless

	CHTRequestFormModeless::CHTRequestFormModeless( const CHTRequestFormModeless& objCopy ) : CHTRequestForm( ) {
	} // CHTRequestFormModeless

	CHTRequestFormModeless::~CHTRequestFormModeless( ) {
	} // ~CHTRequestFormModeless

	CHTRequestFormModeless& CHTRequestFormModeless::operator =( const CHTRequestFormModeless& objCopy ) {
		return *this;
	} // CHTRequestFormModeless& operator =

	void CHTRequestFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "trequest" );
	} // void OnButtonOk

	void CHTRequestFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "trequest" );
	} // void CHTRequestForm

	void CHTRequestFormModeless::Init( ) {
		CHTRequestForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHTRequestFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHTRequestFormModeless

} // namespace UdSDK
