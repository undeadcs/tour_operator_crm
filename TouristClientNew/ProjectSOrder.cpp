
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHSOrderListPick( ) {
		return new CHSOrderPick;
	} // udPListHandlerPick GetHSOrderListPick

	udPBase GetProjectSOrderList( ) {
		udPBase ret = GetProjectSOrder( );
		return ret;
	} // udPBase GetProjectSOrderList

	CHSOrder::CHSOrder( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "sorder";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать тип заявки" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHSOrder::ColModelSOrder( );

		m_pData->SetName( "sorder" );
		m_pData->SetGetObject( GetProjectSOrderList );
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
	} // CHSOrder

	CHSOrder::CHSOrder( const CHSOrder& objCopy ) : udListHandler( ) {
		m_szName = "sorder";
	} // CHSOrder

	CHSOrder::~CHSOrder( ) {
	} // ~CSOrder

	CHSOrder& CHSOrder::operator =( const CHSOrder& objCopy ) {
		return *this;
	} // CHSOrder& operator =

	void CHSOrder::Init( ) {
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

	void CHSOrder::Load( ) {
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

	void CHSOrder::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHSOrder::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSOrderModeless::CHSOrderModeless( ) :
		udListHandlerModeless( ),
		CHSOrder( )
	{
	} // CHSOrderModeless

	CHSOrderModeless::CHSOrderModeless( const CHSOrderModeless& objCopy ) : CHSOrder( ) {
	} // CHSOrderModeless

	CHSOrderModeless::~CHSOrderModeless( ) {
	} // ~CHSOrderModeless

	CHSOrderModeless& CHSOrderModeless::operator =( const CHSOrderModeless& objCopy ) {
		return *this;
	} // CHSOrderModeless& operator =

	void CHSOrderModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHSOrder::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHSOrderModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "sorder_form" );
	} // void OnButtonForm

	void CHSOrderModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "sorder_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "sorder_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSOrderPick::CHSOrderPick( ) : CHSOrder( ) {
	} // CHSOrderPick

	CHSOrderPick::CHSOrderPick( const CHSOrderPick& objCopy ) : CHSOrder( ) {
	} // CHSOrderPick

	CHSOrderPick::~CHSOrderPick( ) {
	} // ~CHSOrderPick

	CHSOrderPick& CHSOrderPick::operator =( const CHSOrderPick& objCopy ) {
		return *this;
	} // CHSOrderPick& operator =

	void CHSOrderPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHSOrder::Init( );
	} // void CHSOrderPick

	void CHSOrderPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHSOrderFormModal handler = new udHSOrderFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание состояния заявки" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание состояния заявки" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHSOrderPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectSOrder( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите состояние заявки" ),
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

	void CHSOrderPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHSOrderPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHSOrderFormModal handler = new udHSOrderFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование состояние заявки" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование состояние заявки" ) );
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

	udPFormHandler GetHSOrderForm( ) {
		return new CHSOrderFormModal;
	} // udPFormHandler GetHSOrderForm

	CHSOrderForm::CHSOrderForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Состояние заявки" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "sorder_form";

		m_pData->SetName( "sorder" );
		m_pData->SetGetObject( GetProjectSOrder );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHSOrderForm

	CHSOrderForm::CHSOrderForm( const CHSOrderForm& objCopy ) : udFormHandler( ) {
	} // CHSOrderForm

	CHSOrderForm::~CHSOrderForm( ) {
	} // ~CHSOrderForm

	CHSOrderForm& CHSOrderForm::operator =( const CHSOrderForm& objCopy ) {
		return *this;
	} // CHSOrderForm& operator =

	void CHSOrderForm::Init( ) {
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

	void CHSOrderForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHSOrderForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHSOrderForm::Validate( ) {
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

	void CHSOrderForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSOrderFormModal::CHSOrderFormModal( ) : CHSOrderForm( ) {
	} // CHSOrderFormModal

	CHSOrderFormModal::CHSOrderFormModal( const CHSOrderFormModal& objCopy ) : CHSOrderForm( ) {
	} // CHSOrderFormModal

	CHSOrderFormModal::~CHSOrderFormModal( ) {
	} // ~CHSOrderFormModal

	CHSOrderFormModal& CHSOrderFormModal::operator =( const CHSOrderFormModal& objCopy ) {
		return *this;
	} // CHSOrderFormModal& operator =

	void CHSOrderFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHSOrderFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHSOrderForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHSOrderFormModeless::CHSOrderFormModeless( ) : CHSOrderForm( ) {
	} // CHSOrderFormModeless

	CHSOrderFormModeless::CHSOrderFormModeless( const CHSOrderFormModeless& objCopy ) : CHSOrderForm( ) {
	} // CHSOrderFormModeless

	CHSOrderFormModeless::~CHSOrderFormModeless( ) {
	} // ~CHSOrderFormModeless

	CHSOrderFormModeless& CHSOrderFormModeless::operator =( const CHSOrderFormModeless& objCopy ) {
		return *this;
	} // CHSOrderFormModeless& operator =

	void CHSOrderFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "sorder" );
	} // void OnButtonOk

	void CHSOrderFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "sorder" );
	} // void CHSOrderForm

	void CHSOrderFormModeless::Init( ) {
		CHSOrderForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHSOrderFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHSOrderFormModeless

} // namespace UdSDK
