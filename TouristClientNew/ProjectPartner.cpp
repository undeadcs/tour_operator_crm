
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHPartnerListPick( ) {
		return new CHPartnerPick;
	} // udPListHandlerPick GetHPartnerListPick

	udPBase GetProjectPartnerList( ) {
		udPBase ret = GetProjectPartner( );
		return ret;
	} // udPBase GetProjectPartnerList

	CHPartner::CHPartner( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltCfio( Glib::locale_to_utf8( "ФИО контактного лица" ) ),
		m_lblFltEmail( Glib::locale_to_utf8( "E-mail" ) ),
		m_lblFltPhone( Glib::locale_to_utf8( "Телефон" ) )
	{
		m_szName = "partner";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать партнера" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "cfio" )->ini( wstring( L"" ) );
		filter->get( "email" )->ini( wstring( L"" ) );
		filter->get( "phone" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHPartner::ColModelPartner( );

		m_pData->SetName( "partner" );
		m_pData->SetGetObject( GetProjectPartnerList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// ФИО контактного лица
		m_tblFilter.attach( m_lblFltCfio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCfio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// E-mail
		m_tblFilter.attach( m_lblFltEmail, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltEmail, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Телефон
		m_tblFilter.attach( m_lblFltPhone, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltPhone, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHPartner

	CHPartner::CHPartner( const CHPartner& objCopy ) : udListHandler( ) {
		m_szName = "partner";
	} // CHPartner

	CHPartner::~CHPartner( ) {
	} // ~CPartner

	CHPartner& CHPartner::operator =( const CHPartner& objCopy ) {
		return *this;
	} // CHPartner& operator =

	void CHPartner::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Контактное лицо" ), m_pColModel->m_col_cfio );
		m_objTreeView.append_column( Glib::locale_to_utf8( "E-mail" ), m_pColModel->m_col_email );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Телефон" ), m_pColModel->m_col_phone );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHPartner::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_cfio ] = Glib::locale_to_utf8( obj->get( "cfio" )->toString( ) );
			row[ m_pColModel->m_col_email ] = Glib::locale_to_utf8( obj->get( "email" )->toString( ) );
			row[ m_pColModel->m_col_phone ] = Glib::locale_to_utf8( obj->get( "phone" )->toString( ) );
		}
	} // void Load

	void CHPartner::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltCfio.Reset( );
		m_inpFltEmail.Reset( );
		m_inpFltPhone.Reset( );
	} // void FilterUiClear

	void CHPartner::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szCfio,
						szEmail,
						szPhone;

		szName = m_inpFltName.GetValue( );
		szCfio = m_inpFltCfio.GetValue( );
		szEmail = m_inpFltEmail.GetValue( );
		szPhone = m_inpFltPhone.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "cfio" )->ini( ConvertString( szCfio.data( ), CP_UTF8 ) );
		filter->get( "email" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
		filter->get( "phone" )->ini( ConvertString( szPhone.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerModeless::CHPartnerModeless( ) :
		udListHandlerModeless( ),
		CHPartner( ),
		m_btnRequest( Glib::locale_to_utf8( "Запросы" ) ),
		m_btnAnswer( Glib::locale_to_utf8( "Ответы" ) )
	{
		m_btnRequest.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"partnerreq"
		) );
		m_btnAnswer.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"partneranswr"
		) );
	} // CHPartnerModeless

	CHPartnerModeless::CHPartnerModeless( const CHPartnerModeless& objCopy ) : CHPartner( ) {
	} // CHPartnerModeless

	CHPartnerModeless::~CHPartnerModeless( ) {
	} // ~CHPartnerModeless

	CHPartnerModeless& CHPartnerModeless::operator =( const CHPartnerModeless& objCopy ) {
		return *this;
	} // CHPartnerModeless& operator =

	void CHPartnerModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		m_hbbTop.pack_start( m_btnRequest, false, false, 0 );
		m_hbbTop.pack_start( m_btnAnswer, false, false, 0 );
		CHPartner::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHPartnerModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "partner_form" );
	} // void OnButtonForm

	void CHPartnerModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "partner_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "partner_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerPick::CHPartnerPick( ) : CHPartner( ) {
	} // CHPartnerPick

	CHPartnerPick::CHPartnerPick( const CHPartnerPick& objCopy ) : CHPartner( ) {
	} // CHPartnerPick

	CHPartnerPick::~CHPartnerPick( ) {
	} // ~CHPartnerPick

	CHPartnerPick& CHPartnerPick::operator =( const CHPartnerPick& objCopy ) {
		return *this;
	} // CHPartnerPick& operator =

	void CHPartnerPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHPartner::Init( );
	} // void CHPartnerPick

	void CHPartnerPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHPartnerFormModal handler = new udHPartnerFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание партнера" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание партнера" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHPartnerPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectPartner( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите партнера" ),
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

	void CHPartnerPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHPartnerPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHPartnerFormModal handler = new udHPartnerFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование партнера" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование партнера" ) );
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

	udPFormHandler GetHPartnerForm( ) {
		return new CHPartnerFormModal;
	} // udPFormHandler GetHPartnerForm

	CHPartnerForm::CHPartnerForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Партнер" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblDfio( Glib::locale_to_utf8( "ФИО директора" ) ),
		m_lblCfio( Glib::locale_to_utf8( "ФИО контактного лица" ) ),
		m_lblEmail( Glib::locale_to_utf8( "E-mail" ) ),
		m_lblPhone( Glib::locale_to_utf8( "Телефон" ) ),
		m_lblFax( Glib::locale_to_utf8( "Факс" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) )
	{
		m_szName = "partner_form";

		m_pData->SetName( "partner" );
		m_pData->SetGetObject( GetProjectPartner );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHPartnerForm

	CHPartnerForm::CHPartnerForm( const CHPartnerForm& objCopy ) : udFormHandler( ) {
	} // CHPartnerForm

	CHPartnerForm::~CHPartnerForm( ) {
	} // ~CHPartnerForm

	CHPartnerForm& CHPartnerForm::operator =( const CHPartnerForm& objCopy ) {
		return *this;
	} // CHPartnerForm& operator =

	void CHPartnerForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// ФИО директора
		m_tblInput.attach( m_lblDfio, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDfio, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK );

		// ФИО контактного лица
		m_tblInput.attach( m_lblCfio, 0, 1, 2, 3, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCfio, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		// E-mail
		m_tblInput.attach( m_lblEmail, 0, 1, 3, 4, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpEmail, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK );

		// Телефон
		m_tblInput.attach( m_lblPhone, 0, 1, 4, 5, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPhone, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK );

		// Факс
		m_tblInput.attach( m_lblFax, 0, 1, 5, 6, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFax, 1, 2, 5, 6, Gtk::SHRINK, Gtk::SHRINK );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, 6, 7, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, 6, 7, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 7, 8, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHPartnerForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpDfio.Reset( );
		m_inpCfio.Reset( );
		m_inpEmail.Reset( );
		m_inpPhone.Reset( );
		m_inpFax.Reset( );
		m_inpComment.Reset( );
	} // void Clear

	void CHPartnerForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpDfio.SetValue( Glib::locale_to_utf8( obj->get( "dfio" )->toString( ) ) );
		m_inpCfio.SetValue( Glib::locale_to_utf8( obj->get( "cfio" )->toString( ) ) );
		m_inpEmail.SetValue( Glib::locale_to_utf8( obj->get( "email" )->toString( ) ) );
		m_inpPhone.SetValue( Glib::locale_to_utf8( obj->get( "phone" )->toString( ) ) );
		m_inpFax.SetValue( Glib::locale_to_utf8( obj->get( "fax" )->toString( ) ) );
		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
	} // void Load

	udInt CHPartnerForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,		// Наименование
						szDfio,		// ФИО директора
						szCfio,		// ФИО контактного лица
						szEmail,	// E-mail
						szPhone,	// Телефон
						szFax,		// Факс
						szComment;	// Комментарий
		
		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		szDfio = m_inpDfio.GetValue( );
		if ( szDfio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО директора" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDfio.GetEntry( ) ) );
			return 2;
		}

		szCfio = m_inpCfio.GetValue( );
		if ( szCfio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО контактного лица" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCfio.GetEntry( ) ) );
			return 3;
		}

		szEmail = m_inpEmail.GetValue( );
		if ( szEmail.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите E-mail" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEmail.GetEntry( ) ) );
			return 4;
		}

		szPhone = m_inpPhone.GetValue( );
		if ( szPhone.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите телефон" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpPhone.GetEntry( ) ) );
			return 5;
		}

		szFax = m_inpFax.GetValue( );
		if ( szFax.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите факс" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFax.GetEntry( ) ) );
			return 6;
		}

		/*
		szComment = m_inpComment.GetValue( );
		if ( szComment.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите комментарий" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpComment.GetEntry( ) ) );
			return 7;
		}*/
		
		return 0;
	} // udInt Validate

	void CHPartnerForm::Send( ) {
		Glib::ustring	szName,		// Наименование
						szDfio,		// ФИО директора
						szCfio,		// ФИО контактного лица
						szEmail,	// E-mail
						szPhone,	// Телефон
						szFax,		// Факс
						szComment;	// Комментарий
		
		szName = m_inpName.GetValue( );
		szDfio = m_inpDfio.GetValue( );
		szCfio = m_inpCfio.GetValue( );
		szEmail = m_inpEmail.GetValue( );
		szPhone = m_inpPhone.GetValue( );
		szFax = m_inpFax.GetValue( );
		szComment = m_inpComment.GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "dfio" )->ini( ConvertString( szDfio.data( ), CP_UTF8 ) );
		obj->get( "cfio" )->ini( ConvertString( szCfio.data( ), CP_UTF8 ) );
		obj->get( "email" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
		obj->get( "phone" )->ini( ConvertString( szPhone.data( ), CP_UTF8 ) );
		obj->get( "fax" )->ini( ConvertString( szFax.data( ), CP_UTF8 ) );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerFormModal::CHPartnerFormModal( ) : CHPartnerForm( ) {
	} // CHPartnerFormModal

	CHPartnerFormModal::CHPartnerFormModal( const CHPartnerFormModal& objCopy ) : CHPartnerForm( ) {
	} // CHPartnerFormModal

	CHPartnerFormModal::~CHPartnerFormModal( ) {
	} // ~CHPartnerFormModal

	CHPartnerFormModal& CHPartnerFormModal::operator =( const CHPartnerFormModal& objCopy ) {
		return *this;
	} // CHPartnerFormModal& operator =

	void CHPartnerFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHPartnerFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHPartnerForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPartnerFormModeless::CHPartnerFormModeless( ) : CHPartnerForm( ) {
	} // CHPartnerFormModeless

	CHPartnerFormModeless::CHPartnerFormModeless( const CHPartnerFormModeless& objCopy ) : CHPartnerForm( ) {
	} // CHPartnerFormModeless

	CHPartnerFormModeless::~CHPartnerFormModeless( ) {
	} // ~CHPartnerFormModeless

	CHPartnerFormModeless& CHPartnerFormModeless::operator =( const CHPartnerFormModeless& objCopy ) {
		return *this;
	} // CHPartnerFormModeless& operator =

	void CHPartnerFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "partner" );
	} // void OnButtonOk

	void CHPartnerFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "partner" );
	} // void CHPartnerForm

	void CHPartnerFormModeless::Init( ) {
		CHPartnerForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHPartnerFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHPartnerFormModeless

} // namespace UdSDK
