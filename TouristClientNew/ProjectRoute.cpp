
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHRouteListPick( ) {
		return new CHRoutePick;
	} // udPListHandlerPick GetHRouteListPick

	udPBase GetProjectRouteList( ) {
		udPBase ret = GetProjectRoute( );
		return ret;
	} // udPBase GetProjectRouteList

	CHRoute::CHRoute( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "route";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать маршрут" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHRoute::ColModelRoute( );

		m_pData->SetName( "route" );
		m_pData->SetGetObject( GetProjectRouteList );
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
	} // CHRoute

	CHRoute::CHRoute( const CHRoute& objCopy ) : udListHandler( ) {
		m_szName = "route";
	} // CHRoute

	CHRoute::~CHRoute( ) {
	} // ~CRoute

	CHRoute& CHRoute::operator =( const CHRoute& objCopy ) {
		return *this;
	} // CHRoute& operator =

	void CHRoute::Init( ) {
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

	void CHRoute::Load( ) {
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

	void CHRoute::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHRoute::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRouteModeless::CHRouteModeless( ) :
		udListHandlerModeless( ),
		CHRoute( )
	{
	} // CHRouteModeless

	CHRouteModeless::CHRouteModeless( const CHRouteModeless& objCopy ) : CHRoute( ) {
	} // CHRouteModeless

	CHRouteModeless::~CHRouteModeless( ) {
	} // ~CHRouteModeless

	CHRouteModeless& CHRouteModeless::operator =( const CHRouteModeless& objCopy ) {
		return *this;
	} // CHRouteModeless& operator =

	void CHRouteModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHRoute::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHRouteModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "route_form" );
	} // void OnButtonForm

	void CHRouteModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "route_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "route_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRoutePick::CHRoutePick( ) : CHRoute( ) {
	} // CHRoutePick

	CHRoutePick::CHRoutePick( const CHRoutePick& objCopy ) : CHRoute( ) {
	} // CHRoutePick

	CHRoutePick::~CHRoutePick( ) {
	} // ~CHRoutePick

	CHRoutePick& CHRoutePick::operator =( const CHRoutePick& objCopy ) {
		return *this;
	} // CHRoutePick& operator =

	void CHRoutePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHRoute::Init( );
	} // void CHRoutePick

	void CHRoutePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHRouteFormModal handler = new udHRouteFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание маршрута" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание маршрута" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHRoutePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectRoute( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите маршрут" ),
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

	void CHRoutePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHRoutePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHRouteFormModal handler = new udHRouteFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование маршрута" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование маршрута" ) );
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

	udPFormHandler GetHRouteForm( ) {
		return new CHRouteFormModal;
	} // udPFormHandler GetHRouteForm

	CHRouteForm::CHRouteForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Маршрут" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblNameLatin( Glib::locale_to_utf8( "Наименование латиницей" ) )
	{
		m_szName = "route_form";

		m_pData->SetName( "route" );
		m_pData->SetGetObject( GetProjectRoute );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHRouteForm

	CHRouteForm::CHRouteForm( const CHRouteForm& objCopy ) : udFormHandler( ) {
	} // CHRouteForm

	CHRouteForm::~CHRouteForm( ) {
	} // ~CHRouteForm

	CHRouteForm& CHRouteForm::operator =( const CHRouteForm& objCopy ) {
		return *this;
	} // CHRouteForm& operator =

	void CHRouteForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Наименование латиницей
		m_tblInput.attach( m_lblNameLatin, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNameLatin, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHRouteForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpNameLatin.Reset( );
	} // void Clear

	udInt CHRouteForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szNameLatin;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		szNameLatin = m_inpNameLatin.GetValue( );
		if ( szNameLatin.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование латиницей" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpNameLatin.GetEntry( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHRouteForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpNameLatin.SetValue( Glib::locale_to_utf8( obj->get( "namelatin" )->toString( ) ) );
	} // void Load

	void CHRouteForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szNameLatin;

		szName = m_inpName.GetValue( );
		szNameLatin = m_inpNameLatin.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "namelatin" )->ini( ConvertString( szNameLatin.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRouteFormModal::CHRouteFormModal( ) : CHRouteForm( ) {
	} // CHRouteFormModal

	CHRouteFormModal::CHRouteFormModal( const CHRouteFormModal& objCopy ) : CHRouteForm( ) {
	} // CHRouteFormModal

	CHRouteFormModal::~CHRouteFormModal( ) {
	} // ~CHRouteFormModal

	CHRouteFormModal& CHRouteFormModal::operator =( const CHRouteFormModal& objCopy ) {
		return *this;
	} // CHRouteFormModal& operator =

	void CHRouteFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHRouteFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHRouteForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRouteFormModeless::CHRouteFormModeless( ) : CHRouteForm( ) {
	} // CHRouteFormModeless

	CHRouteFormModeless::CHRouteFormModeless( const CHRouteFormModeless& objCopy ) : CHRouteForm( ) {
	} // CHRouteFormModeless

	CHRouteFormModeless::~CHRouteFormModeless( ) {
	} // ~CHRouteFormModeless

	CHRouteFormModeless& CHRouteFormModeless::operator =( const CHRouteFormModeless& objCopy ) {
		return *this;
	} // CHRouteFormModeless& operator =

	void CHRouteFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "route" );
	} // void OnButtonOk

	void CHRouteFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "route" );
	} // void CHRouteForm

	void CHRouteFormModeless::Init( ) {
		CHRouteForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHRouteFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHRouteFormModeless

} // namespace UdSDK
