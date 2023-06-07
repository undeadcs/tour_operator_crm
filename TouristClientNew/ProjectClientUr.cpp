
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHClientUrListPick( ) {
		return new CHClientUrPick;
	} // udPListHandlerPick GetHClientUrListPick

	udPBase GetProjectClientUrList( ) {
		udPBase ret = GetProjectClientUr( );
		return ret;
	} // udPBase GetProjectClientUrList

	CHClientUr::CHClientUr( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltCfio( Glib::locale_to_utf8( "ФИО контактного лица" ) )
	{
		m_szName = "clientur";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать клиента юр. лицо" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "cfio" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHClientUr::ColModelClientUr( );

		m_pData->SetName( "clientur" );
		m_pData->SetGetObject( GetProjectClientUrList );
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
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHClientUr

	CHClientUr::CHClientUr( const CHClientUr& objCopy ) : udListHandler( ) {
		m_szName = "clientur";
	} // CHClientUr

	CHClientUr::~CHClientUr( ) {
	} // ~CClientUr

	CHClientUr& CHClientUr::operator =( const CHClientUr& objCopy ) {
		return *this;
	} // CHClientUr& operator =

	void CHClientUr::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Контактное лицо" ), m_pColModel->m_col_cfio );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHClientUr::Load( ) {
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
		}
	} // void Load

	void CHClientUr::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltCfio.Reset( );
	} // void FilterUiClear

	void CHClientUr::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szCfio;

		szName = m_inpFltName.GetValue( );
		szCfio = m_inpFltCfio.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "cfio" )->ini( ConvertString( szCfio.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientUrModeless::CHClientUrModeless( ) :
		udListHandlerModeless( ),
		CHClientUr( ),
		m_btnFiz( Glib::locale_to_utf8( "Клиенты" ) )
	{
		m_btnFiz.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"client"
		) );
	} // CHClientUrModeless

	CHClientUrModeless::CHClientUrModeless( const CHClientUrModeless& objCopy ) : CHClientUr( ) {
	} // CHClientUrModeless

	CHClientUrModeless::~CHClientUrModeless( ) {
	} // ~CHClientUrModeless

	CHClientUrModeless& CHClientUrModeless::operator =( const CHClientUrModeless& objCopy ) {
		return *this;
	} // CHClientUrModeless& operator =

	void CHClientUrModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		m_hbbTop.pack_start( m_btnFiz, false, false, 0 );
		CHClientUr::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHClientUrModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "clientur_form" );
	} // void OnButtonForm

	void CHClientUrModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "clientur_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "clientur_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientUrPick::CHClientUrPick( ) : CHClientUr( ) {
	} // CHClientUrPick

	CHClientUrPick::CHClientUrPick( const CHClientUrPick& objCopy ) : CHClientUr( ) {
	} // CHClientUrPick

	CHClientUrPick::~CHClientUrPick( ) {
	} // ~CHClientUrPick

	CHClientUrPick& CHClientUrPick::operator =( const CHClientUrPick& objCopy ) {
		return *this;
	} // CHClientUrPick& operator =

	void CHClientUrPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHClientUr::Init( );
	} // void CHClientUrPick

	void CHClientUrPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHClientUrFormModal handler = new udHClientUrFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание клиента (юр. лицо)" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание клиента (юр. лицо)" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHClientUrPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectClientUr( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			mb::err(
				*wndParent,
				Glib::locale_to_utf8( "Выберите клиента (юр. лицо)" ),
				Glib::locale_to_utf8( "Щелкните по строке списка и нажмите кнопку \"ОК\"" )
			);
		}
	} // void OnButtonOk

	void CHClientUrPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHClientUrPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHClientUrFormModal handler = new udHClientUrFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование клиента (юр. лицо)" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование клиента (юр. лицо)" ) );
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

	udPFormHandler GetHClientUrForm( ) {
		return new CHClientUrFormModal;
	} // udPFormHandler GetHClientUrForm

	CHClientUrForm::CHClientUrForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Клиент юридическое лицо" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblDfio( Glib::locale_to_utf8( "ФИО руководителя" ) ),
		m_lblCfio( Glib::locale_to_utf8( "ФИО контактного лица" ) ),
		m_lblEmail( Glib::locale_to_utf8( "E-mail" ) ),
		m_lblPhone( Glib::locale_to_utf8( "Телефон" ) ),
		m_lblFax( Glib::locale_to_utf8( "Факс" ) ),
		m_lblAddr( Glib::locale_to_utf8( "Адрес" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) )
	{
		m_szName = "clientur_form";

		m_pData->SetName( "clientur" );
		m_pData->SetGetObject( GetProjectClientUr );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHClientUrForm

	CHClientUrForm::CHClientUrForm( const CHClientUrForm& objCopy ) : udFormHandler( ) {
	} // CHClientUrForm

	CHClientUrForm::~CHClientUrForm( ) {
	} // ~CHClientUrForm

	CHClientUrForm& CHClientUrForm::operator =( const CHClientUrForm& objCopy ) {
		return *this;
	} // CHClientUrForm& operator =

	void CHClientUrForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// ФИО руководителя
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

		// Адрес
		m_tblInput.attach( m_lblAddr, 0, 1, 6, 7, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpAddr, 1, 2, 6, 7, Gtk::SHRINK, Gtk::SHRINK );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, 7, 8, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, 7, 8, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 8, 9, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHClientUrForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpDfio.Reset( );
		m_inpCfio.Reset( );
		m_inpEmail.Reset( );
		m_inpPhone.Reset( );
		m_inpFax.Reset( );
		m_inpAddr.Reset( );
		m_inpComment.Reset( );
	} // void Clear

	void CHClientUrForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpDfio.SetValue( Glib::locale_to_utf8( obj->get( "dfio" )->toString( ) ) );
		m_inpCfio.SetValue( Glib::locale_to_utf8( obj->get( "cfio" )->toString( ) ) );
		m_inpEmail.SetValue( Glib::locale_to_utf8( obj->get( "lemail" )->toString( ) ) );
		m_inpPhone.SetValue( Glib::locale_to_utf8( obj->get( "lphone" )->toString( ) ) );
		m_inpFax.SetValue( Glib::locale_to_utf8( obj->get( "lfax" )->toString( ) ) );
		m_inpAddr.SetValue( Glib::locale_to_utf8( obj->get( "laddr" )->toString( ) ) );
		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
	} // void Load

	udInt CHClientUrForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szDfio,
						szCfio,
						szEmail,
						szPhone,
						szFax,
						szAddr,
						szComment;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName ).GetEntry( ) );
			return 1;
		}

		szDfio = m_inpDfio.GetValue( );
		if ( szDfio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО директора" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDfio ).GetEntry( ) );
			return 2;
		}

		szCfio = m_inpCfio.GetValue( );
		if ( szCfio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО контактного лица" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCfio ).GetEntry( ) );
			return 3;
		}

		szEmail = m_inpEmail.GetValue( );
		if ( szEmail.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите E-mail" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEmail ).GetEntry( ) );
			return 4;
		}

		szPhone = m_inpPhone.GetValue( );
		if ( szPhone.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите телефон" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpPhone ).GetEntry( ) );
			return 5;
		}

		szFax = m_inpFax.GetValue( );
		if ( szFax.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите факс" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFax ).GetEntry( ) );
			return 6;
		}

		szAddr = m_inpAddr.GetValue( );
		if ( szAddr.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите адрес" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpAddr ).GetEntry( ) );
			return 7;
		}

		/*szComment = m_inpComment.LoadValue( ).GetValue( );
		if ( szComment.empty( ) ) {
			return 8;
		}*/

		return 0;
	} // udInt Validate

	void CHClientUrForm::Send( ) {
		Glib::ustring	szName,
						szDfio,
						szCfio,
						szEmail,
						szPhone,
						szFax,
						szAddr,
						szComment;

		szName = m_inpName.GetValue( );
		szDfio = m_inpDfio.GetValue( );
		szCfio = m_inpCfio.GetValue( );
		szEmail = m_inpEmail.GetValue( );
		szPhone = m_inpPhone.GetValue( );
		szFax = m_inpFax.GetValue( );
		szAddr = m_inpAddr.GetValue( );
		szComment = m_inpComment.GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "dfio" )->ini( ConvertString( szDfio.data( ), CP_UTF8 ) );
		obj->get( "cfio" )->ini( ConvertString( szCfio.data( ), CP_UTF8 ) );
		obj->get( "lemail" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
		obj->get( "lphone" )->ini( ConvertString( szPhone.data( ), CP_UTF8 ) );
		obj->get( "lfax" )->ini( ConvertString( szFax.data( ), CP_UTF8 ) );
		obj->get( "laddr" )->ini( ConvertString( szAddr.data( ), CP_UTF8 ) );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientUrFormModal::CHClientUrFormModal( ) : CHClientUrForm( ) {
	} // CHClientUrFormModal

	CHClientUrFormModal::CHClientUrFormModal( const CHClientUrFormModal& objCopy ) : CHClientUrForm( ) {
	} // CHClientUrFormModal

	CHClientUrFormModal::~CHClientUrFormModal( ) {
	} // ~CHClientUrFormModal

	CHClientUrFormModal& CHClientUrFormModal::operator =( const CHClientUrFormModal& objCopy ) {
		return *this;
	} // CHClientUrFormModal& operator =

	void CHClientUrFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHClientUrFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHClientUrForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientUrFormModeless::CHClientUrFormModeless( ) : CHClientUrForm( ) {
	} // CHClientUrFormModeless

	CHClientUrFormModeless::CHClientUrFormModeless( const CHClientUrFormModeless& objCopy ) : CHClientUrForm( ) {
	} // CHClientUrFormModeless

	CHClientUrFormModeless::~CHClientUrFormModeless( ) {
	} // ~CHClientUrFormModeless

	CHClientUrFormModeless& CHClientUrFormModeless::operator =( const CHClientUrFormModeless& objCopy ) {
		return *this;
	} // CHClientUrFormModeless& operator =

	void CHClientUrFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "clientur" );
	} // void OnButtonOk

	void CHClientUrFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "clientur" );
	} // void CHClientUrForm

	void CHClientUrFormModeless::Init( ) {
		CHClientUrForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHClientUrFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHClientUrFormModeless

} // namespace UdSDK
