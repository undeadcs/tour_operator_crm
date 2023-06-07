
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHPartnerReqListPick( ) {
		return new CHPartnerReqPick;
	} // udPListHandlerPick GetHPartnerReqListPick

	udPBase GetProjectPartnerReqList( ) {
		udPBase ret = GetProjectPartnerReq( );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		ret->get( "manager_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectPartnerReqList

	CHPartnerReq::CHPartnerReq( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltPartner( Glib::locale_to_utf8( "Партнер" ) ),
		m_lblFltDate1( Glib::locale_to_utf8( "Отправлен с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltUser( Glib::locale_to_utf8( "Отправитель" ) ),
		m_lblFltOrder( Glib::locale_to_utf8( "Заявка" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		m_szName = "partnerreq";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать запрос партнеру" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "partner" )->ini( ( udUInt ) 0 );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "order" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHPartnerReq::ColModelPartnerReq( );

		m_pData->SetName( "partnerreq" );
		m_pData->SetGetObject( GetProjectPartnerReqList );
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

		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( fltUser );
		m_pData->AddExtraData( pFde );
		m_inpFltUser.SetDataObject( pFde );
		m_inpFltUser.SetWindowTitle( Glib::locale_to_utf8( "Выбор пользователя" ) );
		m_inpFltUser.SetAttr( "fio" );
		m_inpFltUser.SetGetListHandler( GetHUserListPick );
		m_inpFltUser.SetGetFormHandler( GetHUserForm );
		m_inpFltUser.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пользователя" ) );
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

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Пользователь
		m_tblFilter.attach( m_lblFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHPartnerReq

	CHPartnerReq::CHPartnerReq( const CHPartnerReq& objCopy ) : udListHandler( ) {
		m_szName = "partnerreq";
	} // CHPartnerReq

	CHPartnerReq::~CHPartnerReq( ) {
	} // ~CPartnerReq

	CHPartnerReq& CHPartnerReq::operator =( const CHPartnerReq& objCopy ) {
		return *this;
	} // CHPartnerReq& operator =

	void CHPartnerReq::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Партнер" ), m_pColModel->m_col_partner );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Добавлен" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Отправитель" ), m_pColModel->m_col_manager );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHPartnerReq::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_partner ] = Glib::locale_to_utf8( obj->get( "partner_" )->toString( ) );
			row[ m_pColModel->m_col_created ] = Glib::locale_to_utf8( obj->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_manager ] = Glib::locale_to_utf8( obj->get( "manager_" )->toString( ) );
		}
	} // void Load

	void CHPartnerReq::FilterUiClear( ) {
		m_inpFltPartner.SetTitle( "" );
		m_inpFltUser.SetTitle( "" );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHPartnerReq::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
		filter->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "manager" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltUser ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerReqModeless::CHPartnerReqModeless( ) :
		udListHandlerModeless( ),
		CHPartnerReq( ),
		m_btnAnswr( Glib::locale_to_utf8( "Ответы" ) )
	{
		m_btnAnswr.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"partneranswr"
		) );
	} // CHPartnerReqModeless

	CHPartnerReqModeless::CHPartnerReqModeless( const CHPartnerReqModeless& objCopy ) : CHPartnerReq( ) {
	} // CHPartnerReqModeless

	CHPartnerReqModeless::~CHPartnerReqModeless( ) {
	} // ~CHPartnerReqModeless

	CHPartnerReqModeless& CHPartnerReqModeless::operator =( const CHPartnerReqModeless& objCopy ) {
		return *this;
	} // CHPartnerReqModeless& operator =

	void CHPartnerReqModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		m_hbbTop.pack_start( m_btnAnswr, false, false, 0 );
		CHPartnerReq::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHPartnerReqModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "partnerreq_form" );
	} // void OnButtonForm

	void CHPartnerReqModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "partnerreq_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "partnerreq_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerReqPick::CHPartnerReqPick( ) : CHPartnerReq( ) {
	} // CHPartnerReqPick

	CHPartnerReqPick::CHPartnerReqPick( const CHPartnerReqPick& objCopy ) : CHPartnerReq( ) {
	} // CHPartnerReqPick

	CHPartnerReqPick::~CHPartnerReqPick( ) {
	} // ~CHPartnerReqPick

	CHPartnerReqPick& CHPartnerReqPick::operator =( const CHPartnerReqPick& objCopy ) {
		return *this;
	} // CHPartnerReqPick& operator =

	void CHPartnerReqPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHPartnerReq::Init( );
	} // void CHPartnerReqPick

	void CHPartnerReqPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHPartnerReqFormModal handler = new udHPartnerReqFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание запроса партнеру" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание запроса партнеру" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHPartnerReqPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectPartnerReq( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите запрос партнеру" ),
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

	void CHPartnerReqPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHPartnerReqPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHPartnerReqFormModal handler = new udHPartnerReqFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование запроса партнеру" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование запроса партнеру" ) );
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

	udPFormHandler GetHPartnerReqForm( ) {
		return new CHPartnerReqFormModal;
	} // udPFormHandler GetHPartnerReqForm

	CHPartnerReqForm::CHPartnerReqForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Запрос партнеру" ) ),
		m_lblText( Glib::locale_to_utf8( "Текст" ) ),
		m_lblPartner( Glib::locale_to_utf8( "Партнер" ) )
	{
		m_szName = "partnerreq_form";

		m_pData->SetName( "partnerreq" );
		m_pData->SetGetObject( GetProjectPartnerReq );

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
	} // CHPartnerReqForm

	CHPartnerReqForm::CHPartnerReqForm( const CHPartnerReqForm& objCopy ) : udFormHandler( ) {
	} // CHPartnerReqForm

	CHPartnerReqForm::~CHPartnerReqForm( ) {
	} // ~CHPartnerReqForm

	CHPartnerReqForm& CHPartnerReqForm::operator =( const CHPartnerReqForm& objCopy ) {
		return *this;
	} // CHPartnerReqForm& operator =

	void CHPartnerReqForm::Init( ) {
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

	void CHPartnerReqForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpText.Reset( );
		m_inpPartner.SetTitle( "" );
	} // void Clear

	void CHPartnerReqForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpText.SetValue( Glib::locale_to_utf8( obj->get( "text" )->toString( ) ) );
		m_inpPartner.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "name" )->toString( )
		) );
	} // void Load

	udInt CHPartnerReqForm::Validate( ) {
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

	void CHPartnerReqForm::Send( ) {
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

	CHPartnerReqFormModal::CHPartnerReqFormModal( ) : CHPartnerReqForm( ) {
	} // CHPartnerReqFormModal

	CHPartnerReqFormModal::CHPartnerReqFormModal( const CHPartnerReqFormModal& objCopy ) : CHPartnerReqForm( ) {
	} // CHPartnerReqFormModal

	CHPartnerReqFormModal::~CHPartnerReqFormModal( ) {
	} // ~CHPartnerReqFormModal

	CHPartnerReqFormModal& CHPartnerReqFormModal::operator =( const CHPartnerReqFormModal& objCopy ) {
		return *this;
	} // CHPartnerReqFormModal& operator =

	void CHPartnerReqFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHPartnerReqFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHPartnerReqForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerReqFormModeless::CHPartnerReqFormModeless( ) : CHPartnerReqForm( ) {
	} // CHPartnerReqFormModeless

	CHPartnerReqFormModeless::CHPartnerReqFormModeless( const CHPartnerReqFormModeless& objCopy ) : CHPartnerReqForm( ) {
	} // CHPartnerReqFormModeless

	CHPartnerReqFormModeless::~CHPartnerReqFormModeless( ) {
	} // ~CHPartnerReqFormModeless

	CHPartnerReqFormModeless& CHPartnerReqFormModeless::operator =( const CHPartnerReqFormModeless& objCopy ) {
		return *this;
	} // CHPartnerReqFormModeless& operator =

	void CHPartnerReqFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "partnerreq" );
	} // void OnButtonOk

	void CHPartnerReqFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "partnerreq" );
	} // void CHPartnerReqForm

	void CHPartnerReqFormModeless::Init( ) {
		CHPartnerReqForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHPartnerReqFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHPartnerReqFormModeless

} // namespace UdSDK
