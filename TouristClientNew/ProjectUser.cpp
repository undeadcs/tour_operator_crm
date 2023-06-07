
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHUserListPick( ) {
		return new CHUserPick;
	} // udPListHandlerPick GetHUserListPick

	udPBase GetProjectUserList( ) {
		udPBase ret = GetProjectUser( );
		return ret;
	} // udPBase GetProjectUserList

	CHUser::CHUser( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltLogin( Glib::locale_to_utf8( "Логин" ) ),
		m_lblFltEmail( Glib::locale_to_utf8( "E-mail" ) ),
		m_lblFltFio( Glib::locale_to_utf8( "ФИО" ) ),
		m_lblFltRank( Glib::locale_to_utf8( "Ранг" ) ),
		m_inpFltRank( udProjectUser::USERRANK_INVALID, udProjectUser::USERRANK_INVALID )
	{
		m_szName = "user";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать пользователя" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "login" )->ini( wstring( L"" ) );
		filter->get( "email" )->ini( wstring( L"" ) );
		filter->get( "fio" )->ini( wstring( L"" ) );
		filter->get( "rank" )->ini( ( udInt ) udProjectUser::USERRANK_INVALID );

		m_pRowModel = m_pColModel = new CHUser::ColModelUser;

		m_pData->SetName( "user" );
		m_pData->SetGetObject( GetProjectUserList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// контролы фильтра
		m_inpFltRank.AddRow( udProjectUser::USERRANK_INVALID, Glib::locale_to_utf8( "-- нет --" ) );
		m_inpFltRank.AddRow( udProjectUser::USERRANK_ADMINISTRATOR, Glib::locale_to_utf8( "Администратор" ) );
		m_inpFltRank.AddRow( udProjectUser::USERRANK_MANAGER, Glib::locale_to_utf8( "Менеджер" ) );
		m_inpFltRank.AddRow( udProjectUser::USERRANK_CACHIER, Glib::locale_to_utf8( "Кассир" ) );
		m_inpFltRank.AddRow( udProjectUser::USERRANK_ACCOUNTANT, Glib::locale_to_utf8( "Бухгалтер" ) );
		m_inpFltRank.AddRow( udProjectUser::USERRANK_CHIEF, Glib::locale_to_utf8( "Руководитель" ) );
		m_inpFltRank.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Логин
		m_tblFilter.attach( m_lblFltLogin, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltLogin, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// E-mail
		m_tblFilter.attach( m_lblFltEmail, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltEmail, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// ФИО
		m_tblFilter.attach( m_lblFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Ранг
		m_tblFilter.attach( m_lblFltRank, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltRank, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHUser

	CHUser::CHUser( const CHUser& objCopy ) : udListHandler( ) {
		m_szName = "user";
	} // CHUser

	CHUser::~CHUser( ) {
	} // ~CUser

	CHUser& CHUser::operator =( const CHUser& objCopy ) {
		return *this;
	} // CHUser& operator =

	void CHUser::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Логин" ), m_pColModel->m_col_login );
		m_objTreeView.append_column( Glib::locale_to_utf8( "E-mail" ), m_pColModel->m_col_email );
		m_objTreeView.append_column( Glib::locale_to_utf8( "ФИО" ), m_pColModel->m_col_fio );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Ранг" ), m_pColModel->m_col_rank );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHUser::Load( ) {
		udListHandler::Load( );

		map< udUInt, wstring > mapRank;
		mapRank[ udProjectUser::USERRANK_INVALID		] = L"";
		mapRank[ udProjectUser::USERRANK_ADMINISTRATOR	] = L"Администратор";
		mapRank[ udProjectUser::USERRANK_MANAGER		] = L"Менеджер";
		mapRank[ udProjectUser::USERRANK_CACHIER		] = L"Кассир";
		mapRank[ udProjectUser::USERRANK_ACCOUNTANT		] = L"Бухгалтер";
		mapRank[ udProjectUser::USERRANK_CHIEF			] = L"Руководитель";
		mapRank[ udProjectUser::USERRANK_SUPERADMIN		] = L"Суперадминистратор";

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		Gtk::TreeModel::iterator itRow;
		Gtk::TreePath objPath;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;

			obj->get( "rank_" )->ini( mapRank[ obj->get( "rank" )->toInt( ) ] );

			itRow = m_objTreeModel->append( );
			objPath = m_objTreeModel->get_path( itRow );
			m_mapObject[ objPath.to_string( ) ] = obj;
			row = *itRow;
			//row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_login ] = Glib::locale_to_utf8( obj->get( "login" )->toString( ) );
			row[ m_pColModel->m_col_email ] = Glib::locale_to_utf8( obj->get( "email" )->toString( ) );
			row[ m_pColModel->m_col_fio ] = Glib::locale_to_utf8( obj->get( "fio" )->toString( ) );
			row[ m_pColModel->m_col_rank ] = Glib::locale_to_utf8( obj->get( "rank_" )->toString( ) );
		}
	} // void Load

	void CHUser::FilterUiClear( ) {
		m_inpFltLogin.Reset( );
		m_inpFltEmail.Reset( );
		m_inpFltFio.Reset( );
		//m_inpFltRank.set_active( 0 );
		m_inpFltRank.Reset( );
	} // void FilterUiClear

	void CHUser::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szLogin,
						szEmail,
						szFio;

		szLogin = m_inpFltLogin.GetValue( );
		szEmail = m_inpFltEmail.GetValue( );
		szFio = m_inpFltFio.GetValue( );

		udInt rank = ( udInt ) m_inpFltRank.LoadValue( ).GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "login" )->ini( ConvertString( szLogin.data( ), CP_UTF8 ) );
		filter->get( "email" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
		filter->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		filter->get( "rank" )->ini( rank );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHUserModeless::CHUserModeless( ) :
		udListHandlerModeless( ),
		CHUser( )
	{
	} // CHUserModeless

	CHUserModeless::CHUserModeless( const CHUserModeless& objCopy ) : CHUser( ) {
	} // CHUserModeless

	CHUserModeless::~CHUserModeless( ) {
	} // ~CHUserModeless

	CHUserModeless& CHUserModeless::operator =( const CHUserModeless& objCopy ) {
		return *this;
	} // CHUserModeless& operator =

	void CHUserModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHUser::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHUserModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "user_form" );
	} // void OnButtonForm

	void CHUserModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "user_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "user_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHUserPick::CHUserPick( ) : CHUser( ) {
	} // CHUserPick

	CHUserPick::CHUserPick( const CHUserPick& objCopy ) : CHUser( ) {
	} // CHUserPick

	CHUserPick::~CHUserPick( ) {
	} // ~CHUserPick

	CHUserPick& CHUserPick::operator =( const CHUserPick& objCopy ) {
		return *this;
	} // CHUserPick& operator =

	void CHUserPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHUser::Init( );
	} // void CHUserPick

	void CHUserPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHUserFormModal handler = new udHUserFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание пользователя" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание пользователя" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHUserPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectUser( );
				m_pPickedObject->apply( itObj->second, true, true );
			}
			/*Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectUser( );
			Glib::ustring szFio = row[ m_pColModel->m_col_fio ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );*/
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите пользователя" ),
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

	void CHUserPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHUserPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHUserFormModal handler = new udHUserFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование пользователя" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование пользователя" ) );
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

	udPFormHandler GetHUserForm( ) {
		return new CHUserFormModal;
	} // udPFormHandler GetHUserForm

	CHUserForm::CHUserForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Пользователь" ) ),
		m_lblLogin( Glib::locale_to_utf8( "Логин" ) ),
		m_lblPassword( Glib::locale_to_utf8( "Пароль" ) ),
		m_lblEmail( Glib::locale_to_utf8( "E-mail" ) ),
		m_lblFio( Glib::locale_to_utf8( "ФИО" ) ),
		m_lblRank( Glib::locale_to_utf8( "Ранг" ) ),
		m_lblSeries( Glib::locale_to_utf8( "Серия" ) ),
		m_inpRank( udProjectUser::USERRANK_MANAGER, udProjectUser::USERRANK_MANAGER )
	{
		m_szName = "user_form";

		m_lblPassword.set_visible( false );

		m_pData->SetName( "user" );
		m_pData->SetGetObject( GetProjectUser );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblLogin.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblLogin.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpRank.AddRow( udProjectUser::USERRANK_ADMINISTRATOR, Glib::locale_to_utf8( "Администратор" ) );
		m_inpRank.AddRow( udProjectUser::USERRANK_MANAGER, Glib::locale_to_utf8( "Менеджер" ) );
		m_inpRank.AddRow( udProjectUser::USERRANK_CACHIER, Glib::locale_to_utf8( "Кассир" ) );
		m_inpRank.AddRow( udProjectUser::USERRANK_ACCOUNTANT, Glib::locale_to_utf8( "Бухгалтер" ) );
		m_inpRank.AddRow( udProjectUser::USERRANK_CHIEF, Glib::locale_to_utf8( "Руководитель" ) );
		m_inpRank.Reset( );
		/*m_pColModelRank = new CHUserForm::ColModelUserRank;
		m_objRankModel = Gtk::ListStore::create( *m_pColModelRank );

		Gtk::TreeModel::Row row;

		row = *( m_objRankModel->append( ) );
		row[ m_pColModelRank->m_col_code ] = udProjectUser::USERRANK_ADMINISTRATOR;
		row[ m_pColModelRank->m_col_name ] = Glib::locale_to_utf8( "Администратор" );

		row = *( m_objRankModel->append( ) );
		row[ m_pColModelRank->m_col_code ] = udProjectUser::USERRANK_MANAGER;
		row[ m_pColModelRank->m_col_name ] = Glib::locale_to_utf8( "Менеджер" );

		row = *( m_objRankModel->append( ) );
		row[ m_pColModelRank->m_col_code ] = udProjectUser::USERRANK_CACHIER;
		row[ m_pColModelRank->m_col_name ] = Glib::locale_to_utf8( "Кассир" );

		row = *( m_objRankModel->append( ) );
		row[ m_pColModelRank->m_col_code ] = udProjectUser::USERRANK_ACCOUNTANT;
		row[ m_pColModelRank->m_col_name ] = Glib::locale_to_utf8( "Бухгалтер" );

		row = *( m_objRankModel->append( ) );
		row[ m_pColModelRank->m_col_code ] = udProjectUser::USERRANK_CHIEF;
		row[ m_pColModelRank->m_col_name ] = Glib::locale_to_utf8( "Руководитель" );

		m_inpRank.set_model( m_objRankModel );

		m_inpRank.pack_start( m_pColModelRank->m_col_name );
		m_inpRank.set_active( 0 );*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHUserForm

	CHUserForm::CHUserForm( const CHUserForm& objCopy ) : udFormHandler( ) {
	} // CHUserForm

	CHUserForm::~CHUserForm( ) {
	} // ~CHUserForm

	CHUserForm& CHUserForm::operator =( const CHUserForm& objCopy ) {
		return *this;
	} // CHUserForm& operator =

	void CHUserForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Логин
		m_tblInput.attach( m_lblLogin, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpLogin, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// Пароль
		m_tblInput.attach( m_lblPassword, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPassword, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK );

		// E-mail
		m_tblInput.attach( m_lblEmail, 0, 1, 2, 3, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpEmail, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		// ФИО
		m_tblInput.attach( m_lblFio, 0, 1, 3, 4, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFio, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK );

		// Ранг
		m_tblInput.attach( m_lblRank, 0, 1, 4, 5, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpRank, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK );

		// Серия
		m_tblInput.attach( m_lblSeries, 0, 1, 5, 6, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSeries, 1, 2, 5, 6, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHUserForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpLogin.Reset( );
		m_inpPassword.Reset( );
		m_inpEmail.Reset( );
		m_inpFio.Reset( );
		m_inpSeries.Reset( );
		//m_inpRank.set_active( 0 );
		m_inpRank.Reset( );
	} // void Clear

	void CHUserForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpLogin.SetValue( Glib::locale_to_utf8( obj->get( "login" )->toString( ) ) );
		m_inpEmail.SetValue( Glib::locale_to_utf8( obj->get( "email" )->toString( ) ) );
		m_inpFio.SetValue( Glib::locale_to_utf8( obj->get( "fio" )->toString( ) ) );
		m_inpSeries.SetValue( Glib::locale_to_utf8( obj->get( "series" )->toString( ) ) );
		if ( obj->get( "rank" )->toInt( ) > 0 ) {
			//m_inpRank.set_active( obj->get( "rank" )->toInt( ) - 1 );
			m_inpRank.SetValue( obj->get( "rank" )->toUInt( ) - 1 );
		}
	} // void Load

	udInt CHUserForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szLogin,
						szPassword,
						szEmail,
						szFio,
						szRank,
						szSeries;

		szLogin = m_inpLogin.GetValue( );
		if ( szLogin.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите логин" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpLogin.GetEntry( ) ) );
			return 1;
		}

		if ( m_pData->GetData( )->get( "id" )->toUInt( ) == 0 ) {
			szPassword = m_inpPassword.GetValue( );
			if ( szPassword.empty( ) ) {
				mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите пароль" ) );
				g_pApp->GetWndWork( )->set_focus( *( m_inpPassword.GetEntry( ) ) );
				return 2;
			}
		}

		szEmail = m_inpEmail.GetValue( );
		if ( szEmail.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите E-mail" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEmail.GetEntry( ) ) );
			return 3;
		}

		szFio = m_inpFio.GetValue( );
		if ( szFio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFio.GetEntry( ) ) );
			return 4;
		}

		szSeries = m_inpSeries.GetValue( );
		if ( szSeries.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите серию" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpSeries.GetEntry( ) ) );
			return 5;
		}

		return 0;
	} // udInt Validate

	void CHUserForm::Send( ) {
		Glib::ustring	szLogin,
						szPassword,
						szEmail,
						szFio,
						szRank,
						szSeries;
		
		szLogin = m_inpLogin.GetValue( );
		szPassword = m_inpPassword.GetValue( );
		szEmail = m_inpEmail.GetValue( );
		szFio = m_inpFio.GetValue( );
		szSeries = m_inpSeries.GetValue( );

		udUInt rank = m_inpRank.LoadValue( ).GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "login" )->ini( ConvertString( szLogin.data( ), CP_UTF8 ) );
		obj->get( "password" )->ini( ConvertString( szPassword.data( ), CP_UTF8 ) );
		obj->get( "email" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
		obj->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		obj->get( "rank" )->ini( rank );
		obj->get( "series" )->ini( ConvertString( szSeries.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHUserFormModal::CHUserFormModal( ) : CHUserForm( ) {
	} // CHUserFormModal

	CHUserFormModal::CHUserFormModal( const CHUserFormModal& objCopy ) : CHUserForm( ) {
	} // CHUserFormModal

	CHUserFormModal::~CHUserFormModal( ) {
	} // ~CHUserFormModal

	CHUserFormModal& CHUserFormModal::operator =( const CHUserFormModal& objCopy ) {
		return *this;
	} // CHUserFormModal& operator =

	void CHUserFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHUserFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHUserForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHUserFormModeless::CHUserFormModeless( ) : CHUserForm( ) {
	} // CHUserFormModeless

	CHUserFormModeless::CHUserFormModeless( const CHUserFormModeless& objCopy ) : CHUserForm( ) {
	} // CHUserFormModeless

	CHUserFormModeless::~CHUserFormModeless( ) {
	} // ~CHUserFormModeless

	CHUserFormModeless& CHUserFormModeless::operator =( const CHUserFormModeless& objCopy ) {
		return *this;
	} // CHUserFormModeless& operator =

	void CHUserFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "user" );
	} // void OnButtonOk

	void CHUserFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "user" );
	} // void CHUserForm

	void CHUserFormModeless::Init( ) {
		CHUserForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHUserFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHUserFormModeless

} // namespace UdSDK
