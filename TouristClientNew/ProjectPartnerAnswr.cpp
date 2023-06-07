
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHPartnerAnswrListPick( ) {
		return new CHPartnerAnswrPick;
	} // udPListHandlerPick GetHPartnerAnswrListPick

	udPBase GetProjectPartnerAnswrList( ) {
		udPBase ret = GetProjectPartnerAnswr( );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectPartnerAnswrList

	CHPartnerAnswr::CHPartnerAnswr( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltPartner( Glib::locale_to_utf8( "Партнер" ) ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		m_szName = "partneranswr";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать ответ партнера" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "partner" )->ini( ( udUInt ) 0 );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );

		m_pRowModel = m_pColModel = new CHPartnerAnswr::ColModelPartnerAnswr( );

		m_pData->SetName( "partneranswr" );
		m_pData->SetGetObject( GetProjectPartnerAnswrList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectPartner );
		pFde->SetMark( fltPartner );
		m_pData->AddExtraData( pFde );
		m_inpFltPartner.SetDataObject( pFde );
		m_inpFltPartner.SetWindowTitle( Glib::locale_to_utf8( "Выбор партнера" ) );
		m_inpFltPartner.SetAttr( "name" );
		m_inpFltPartner.SetGetListHandler( GetHPartnerListPick );
		m_inpFltPartner.SetGetFormHandler( GetHPartnerForm );
		m_inpFltPartner.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование партнера" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Партнер
		m_tblFilter.attach( m_lblFltPartner, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltPartner, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата с
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата по
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHPartnerAnswr

	CHPartnerAnswr::CHPartnerAnswr( const CHPartnerAnswr& objCopy ) : udListHandler( ) {
		m_szName = "partneranswr";
	} // CHPartnerAnswr

	CHPartnerAnswr::~CHPartnerAnswr( ) {
	} // ~CPartnerAnswr

	CHPartnerAnswr& CHPartnerAnswr::operator =( const CHPartnerAnswr& objCopy ) {
		return *this;
	} // CHPartnerAnswr& operator =

	void CHPartnerAnswr::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата и время" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Партнер" ), m_pColModel->m_col_partner );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHPartnerAnswr::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_created ] = Glib::locale_to_utf8( obj->get( "created" )->toString( ) );
			row[ m_pColModel->m_col_partner ] = Glib::locale_to_utf8( obj->get( "partner_" )->toString( ) );
		}
	} // void Load

	void CHPartnerAnswr::FilterUiClear( ) {
		m_inpFltPartner.SetTitle( "" );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHPartnerAnswr::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerAnswrModeless::CHPartnerAnswrModeless( ) :
		udListHandlerModeless( ),
		CHPartnerAnswr( ),
		m_btnRequest( Glib::locale_to_utf8( "Запросы" ) )
	{
		m_btnRequest.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"partnerreq"
		) );
	} // CHPartnerAnswrModeless

	CHPartnerAnswrModeless::CHPartnerAnswrModeless( const CHPartnerAnswrModeless& objCopy ) : CHPartnerAnswr( ) {
	} // CHPartnerAnswrModeless

	CHPartnerAnswrModeless::~CHPartnerAnswrModeless( ) {
	} // ~CHPartnerAnswrModeless

	CHPartnerAnswrModeless& CHPartnerAnswrModeless::operator =( const CHPartnerAnswrModeless& objCopy ) {
		return *this;
	} // CHPartnerAnswrModeless& operator =

	void CHPartnerAnswrModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		m_hbbTop.pack_start( m_btnRequest, false, false, 0 );
		CHPartnerAnswr::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHPartnerAnswrModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "partneranswr_form" );
	} // void OnButtonForm

	void CHPartnerAnswrModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "partneranswr_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "partneranswr_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerAnswrPick::CHPartnerAnswrPick( ) : CHPartnerAnswr( ) {
	} // CHPartnerAnswrPick

	CHPartnerAnswrPick::CHPartnerAnswrPick( const CHPartnerAnswrPick& objCopy ) : CHPartnerAnswr( ) {
	} // CHPartnerAnswrPick

	CHPartnerAnswrPick::~CHPartnerAnswrPick( ) {
	} // ~CHPartnerAnswrPick

	CHPartnerAnswrPick& CHPartnerAnswrPick::operator =( const CHPartnerAnswrPick& objCopy ) {
		return *this;
	} // CHPartnerAnswrPick& operator =

	void CHPartnerAnswrPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHPartnerAnswr::Init( );
	} // void CHPartnerAnswrPick

	void CHPartnerAnswrPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHPartnerAnswrFormModal handler = new udHPartnerAnswrFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание ответа партнера" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание ответа партнера" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHPartnerAnswrPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectPartnerAnswr( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите ответ партнера" ),
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

	void CHPartnerAnswrPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHPartnerAnswrPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHPartnerAnswrFormModal handler = new udHPartnerAnswrFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование ответа партнера" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование ответа партнера" ) );
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

	udPFormHandler GetHPartnerAnswrForm( ) {
		return new CHPartnerAnswrFormModal;
	} // udPFormHandler GetHPartnerAnswrForm

	CHPartnerAnswrForm::CHPartnerAnswrForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Запрос партнеру" ) ),
		m_lblText( Glib::locale_to_utf8( "Текст" ) ),
		m_lblPartner( Glib::locale_to_utf8( "Партнер" ) )
	{
		m_szName = "partneranswr_form";

		m_pData->SetName( "partneranswr" );
		m_pData->SetGetObject( GetProjectPartnerAnswr );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblText.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpText.set_size_request( 250 );

		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectPartner );
		pFde->SetMark( ctrlidPartner );
		m_pData->AddExtraData( pFde );
		m_inpPartner.SetDataObject( pFde );
		m_inpPartner.SetWindowTitle( Glib::locale_to_utf8( "Выбор партнера" ) );
		m_inpPartner.SetAttr( "name" );
		m_inpPartner.SetGetListHandler( GetHPartnerListPick );
		m_inpPartner.SetGetFormHandler( GetHPartnerForm );
		m_inpPartner.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование партнера" ) );
	} // CHPartnerAnswrForm

	CHPartnerAnswrForm::CHPartnerAnswrForm( const CHPartnerAnswrForm& objCopy ) : udFormHandler( ) {
	} // CHPartnerAnswrForm

	CHPartnerAnswrForm::~CHPartnerAnswrForm( ) {
	} // ~CHPartnerAnswrForm

	CHPartnerAnswrForm& CHPartnerAnswrForm::operator =( const CHPartnerAnswrForm& objCopy ) {
		return *this;
	} // CHPartnerAnswrForm& operator =

	void CHPartnerAnswrForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Партнер
		m_tblInput.attach( m_lblPartner, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPartner, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK );

		// Текст
		m_tblInput.attach( m_lblText, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpText, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHPartnerAnswrForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpText.Reset( );
		m_inpPartner.SetTitle( "" );
	} // void Clear

	void CHPartnerAnswrForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpText.SetValue( Glib::locale_to_utf8( obj->get( "text" )->toString( ) ) );
		m_inpPartner.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "name" )->toString( )
		) );
	} // void Load

	udInt CHPartnerAnswrForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szText;
		
		szText = m_inpText.GetValue( );
		if ( szText.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите текст" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpText.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите партнера" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpPartner.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHPartnerAnswrForm::Send( ) {
		Glib::ustring szText;
		
		szText = m_inpText.GetValue( );
		
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		
		obj->get( "created" )->ini( datetime( 0, true ) );
		obj->get( "manager" )->ini( g_pApp->GetUser( )->get( "id" )->toUInt( ) );
		obj->get( "text" )->ini( ConvertString( szText.data( ), CP_UTF8 ) );
		obj->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
		
		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerAnswrFormModal::CHPartnerAnswrFormModal( ) : CHPartnerAnswrForm( ) {
	} // CHPartnerAnswrFormModal

	CHPartnerAnswrFormModal::CHPartnerAnswrFormModal( const CHPartnerAnswrFormModal& objCopy ) : CHPartnerAnswrForm( ) {
	} // CHPartnerAnswrFormModal

	CHPartnerAnswrFormModal::~CHPartnerAnswrFormModal( ) {
	} // ~CHPartnerAnswrFormModal

	CHPartnerAnswrFormModal& CHPartnerAnswrFormModal::operator =( const CHPartnerAnswrFormModal& objCopy ) {
		return *this;
	} // CHPartnerAnswrFormModal& operator =

	void CHPartnerAnswrFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHPartnerAnswrFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHPartnerAnswrForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerAnswrFormModeless::CHPartnerAnswrFormModeless( ) : CHPartnerAnswrForm( ) {
	} // CHPartnerAnswrFormModeless

	CHPartnerAnswrFormModeless::CHPartnerAnswrFormModeless( const CHPartnerAnswrFormModeless& objCopy ) : CHPartnerAnswrForm( ) {
	} // CHPartnerAnswrFormModeless

	CHPartnerAnswrFormModeless::~CHPartnerAnswrFormModeless( ) {
	} // ~CHPartnerAnswrFormModeless

	CHPartnerAnswrFormModeless& CHPartnerAnswrFormModeless::operator =( const CHPartnerAnswrFormModeless& objCopy ) {
		return *this;
	} // CHPartnerAnswrFormModeless& operator =

	void CHPartnerAnswrFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "partneranswr" );
	} // void OnButtonOk

	void CHPartnerAnswrFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "partneranswr" );
	} // void CHPartnerAnswrForm

	void CHPartnerAnswrFormModeless::Init( ) {
		CHPartnerAnswrForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHPartnerAnswrFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHPartnerAnswrFormModeless

} // namespace UdSDK
