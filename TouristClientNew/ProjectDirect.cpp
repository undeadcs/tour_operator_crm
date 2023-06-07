
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHDirectListPick( ) {
		return new CHDirectPick;
	} // udPListHandlerPick GetHDirectListPick

	udPBase GetProjectDirectList( ) {
		udPBase ret = GetProjectDirect( );
		return ret;
	} // udPBase GetProjectDirectList

	CHDirect::CHDirect( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "direct";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать направление" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHDirect::ColModelDirect( );

		m_pData->SetName( "direct" );
		m_pData->SetGetObject( GetProjectDirectList );
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
	} // CHDirect

	CHDirect::CHDirect( const CHDirect& objCopy ) : udListHandler( ) {
		m_szName = "direct";
	} // CHDirect

	CHDirect::~CHDirect( ) {
	} // ~CDirect

	CHDirect& CHDirect::operator =( const CHDirect& objCopy ) {
		return *this;
	} // CHDirect& operator =

	void CHDirect::Init( ) {
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

	void CHDirect::Load( ) {
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

	void CHDirect::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHDirect::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDirectModeless::CHDirectModeless( ) :
		udListHandlerModeless( ),
		CHDirect( )
	{
	} // CHDirectModeless

	CHDirectModeless::CHDirectModeless( const CHDirectModeless& objCopy ) : CHDirect( ) {
	} // CHDirectModeless

	CHDirectModeless::~CHDirectModeless( ) {
	} // ~CHDirectModeless

	CHDirectModeless& CHDirectModeless::operator =( const CHDirectModeless& objCopy ) {
		return *this;
	} // CHDirectModeless& operator =

	void CHDirectModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHDirect::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHDirectModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "direct_form" );
	} // void OnButtonForm

	void CHDirectModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "direct_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "direct_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDirectPick::CHDirectPick( ) : CHDirect( ) {
	} // CHDirectPick

	CHDirectPick::CHDirectPick( const CHDirectPick& objCopy ) : CHDirect( ) {
	} // CHDirectPick

	CHDirectPick::~CHDirectPick( ) {
	} // ~CHDirectPick

	CHDirectPick& CHDirectPick::operator =( const CHDirectPick& objCopy ) {
		return *this;
	} // CHDirectPick& operator =

	void CHDirectPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHDirect::Init( );
	} // void CHDirectPick

	void CHDirectPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHDirectFormModal handler = new udHDirectFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание направления" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание направления" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHDirectPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectDirect( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите направление" ),
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

	void CHDirectPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHDirectPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHDirectFormModal handler = new udHDirectFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование направления" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование направления" ) );
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

	udPFormHandler GetHDirectForm( ) {
		return new CHDirectFormModal;
	} // udPFormHandler GetHDirectForm

	CHDirectForm::CHDirectForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Направление" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "direct_form";

		m_pData->SetName( "direct" );
		m_pData->SetGetObject( GetProjectDirect );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHDirectForm

	CHDirectForm::CHDirectForm( const CHDirectForm& objCopy ) : udFormHandler( ) {
	} // CHDirectForm

	CHDirectForm::~CHDirectForm( ) {
	} // ~CHDirectForm

	CHDirectForm& CHDirectForm::operator =( const CHDirectForm& objCopy ) {
		return *this;
	} // CHDirectForm& operator =

	void CHDirectForm::Init( ) {
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

	void CHDirectForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHDirectForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHDirectForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName ).GetEntry( ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHDirectForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDirectFormModal::CHDirectFormModal( ) : CHDirectForm( ) {
	} // CHDirectFormModal

	CHDirectFormModal::CHDirectFormModal( const CHDirectFormModal& objCopy ) : CHDirectForm( ) {
	} // CHDirectFormModal

	CHDirectFormModal::~CHDirectFormModal( ) {
	} // ~CHDirectFormModal

	CHDirectFormModal& CHDirectFormModal::operator =( const CHDirectFormModal& objCopy ) {
		return *this;
	} // CHDirectFormModal& operator =

	void CHDirectFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHDirectFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHDirectForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDirectFormModeless::CHDirectFormModeless( ) : CHDirectForm( ) {
	} // CHDirectFormModeless

	CHDirectFormModeless::CHDirectFormModeless( const CHDirectFormModeless& objCopy ) : CHDirectForm( ) {
	} // CHDirectFormModeless

	CHDirectFormModeless::~CHDirectFormModeless( ) {
	} // ~CHDirectFormModeless

	CHDirectFormModeless& CHDirectFormModeless::operator =( const CHDirectFormModeless& objCopy ) {
		return *this;
	} // CHDirectFormModeless& operator =

	void CHDirectFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "direct" );
	} // void OnButtonOk

	void CHDirectFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "direct" );
	} // void CHDirectForm

	void CHDirectFormModeless::Init( ) {
		CHDirectForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHDirectFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHDirectFormModeless

} // namespace UdSDK
