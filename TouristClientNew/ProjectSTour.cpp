
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHSTourListPick( ) {
		return new CHSTourPick;
	} // udPListHandlerPick GetHSTourListPick

	udPBase GetProjectSTourList( ) {
		udPBase ret = GetProjectSTour( );
		return ret;
	} // udPBase GetProjectSTourList

	CHSTour::CHSTour( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "stour";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать состояние тура" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHSTour::ColModelSTour( );

		m_pData->SetName( "stour" );
		m_pData->SetGetObject( GetProjectSTourList );
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
	} // CHSTour

	CHSTour::CHSTour( const CHSTour& objCopy ) : udListHandler( ) {
		m_szName = "stour";
	} // CHSTour

	CHSTour::~CHSTour( ) {
	} // ~CSTour

	CHSTour& CHSTour::operator =( const CHSTour& objCopy ) {
		return *this;
	} // CHSTour& operator =

	void CHSTour::Init( ) {
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

	void CHSTour::Load( ) {
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

	void CHSTour::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHSTour::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSTourModeless::CHSTourModeless( ) :
		udListHandlerModeless( ),
		CHSTour( )
	{
	} // CHSTourModeless

	CHSTourModeless::CHSTourModeless( const CHSTourModeless& objCopy ) : CHSTour( ) {
	} // CHSTourModeless

	CHSTourModeless::~CHSTourModeless( ) {
	} // ~CHSTourModeless

	CHSTourModeless& CHSTourModeless::operator =( const CHSTourModeless& objCopy ) {
		return *this;
	} // CHSTourModeless& operator =

	void CHSTourModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHSTour::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHSTourModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "stour_form" );
	} // void OnButtonForm

	void CHSTourModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "stour_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "stour_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSTourPick::CHSTourPick( ) : CHSTour( ) {
	} // CHSTourPick

	CHSTourPick::CHSTourPick( const CHSTourPick& objCopy ) : CHSTour( ) {
	} // CHSTourPick

	CHSTourPick::~CHSTourPick( ) {
	} // ~CHSTourPick

	CHSTourPick& CHSTourPick::operator =( const CHSTourPick& objCopy ) {
		return *this;
	} // CHSTourPick& operator =

	void CHSTourPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHSTour::Init( );
	} // void CHSTourPick

	void CHSTourPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHSTourFormModal handler = new udHSTourFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание состояние тура" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание состояние тура" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHSTourPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectSTour( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите состояние тура" ),
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

	void CHSTourPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHSTourPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHSTourFormModal handler = new udHSTourFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование состояния тура" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование состояния тура" ) );
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

	udPFormHandler GetHSTourForm( ) {
		return new CHSTourFormModal;
	} // udPFormHandler GetHSTourForm

	CHSTourForm::CHSTourForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Состояние тура" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "stour_form";

		m_pData->SetName( "stour" );
		m_pData->SetGetObject( GetProjectSTour );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHSTourForm

	CHSTourForm::CHSTourForm( const CHSTourForm& objCopy ) : udFormHandler( ) {
	} // CHSTourForm

	CHSTourForm::~CHSTourForm( ) {
	} // ~CHSTourForm

	CHSTourForm& CHSTourForm::operator =( const CHSTourForm& objCopy ) {
		return *this;
	} // CHSTourForm& operator =

	void CHSTourForm::Init( ) {
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

	void CHSTourForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHSTourForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHSTourForm::Validate( ) {
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

	void CHSTourForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSTourFormModal::CHSTourFormModal( ) : CHSTourForm( ) {
	} // CHSTourFormModal

	CHSTourFormModal::CHSTourFormModal( const CHSTourFormModal& objCopy ) : CHSTourForm( ) {
	} // CHSTourFormModal

	CHSTourFormModal::~CHSTourFormModal( ) {
	} // ~CHSTourFormModal

	CHSTourFormModal& CHSTourFormModal::operator =( const CHSTourFormModal& objCopy ) {
		return *this;
	} // CHSTourFormModal& operator =

	void CHSTourFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHSTourFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHSTourForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSTourFormModeless::CHSTourFormModeless( ) : CHSTourForm( ) {
	} // CHSTourFormModeless

	CHSTourFormModeless::CHSTourFormModeless( const CHSTourFormModeless& objCopy ) : CHSTourForm( ) {
	} // CHSTourFormModeless

	CHSTourFormModeless::~CHSTourFormModeless( ) {
	} // ~CHSTourFormModeless

	CHSTourFormModeless& CHSTourFormModeless::operator =( const CHSTourFormModeless& objCopy ) {
		return *this;
	} // CHSTourFormModeless& operator =

	void CHSTourFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "stour" );
	} // void OnButtonOk

	void CHSTourFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "stour" );
	} // void CHSTourForm

	void CHSTourFormModeless::Init( ) {
		CHSTourForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHSTourFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHSTourFormModeless

} // namespace UdSDK
