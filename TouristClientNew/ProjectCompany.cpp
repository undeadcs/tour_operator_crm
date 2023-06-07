
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHCompanyListPick( ) {
		return new CHCompanyPick;
	} // udPListHandlerPick GetHCompanyListPick

	udPBase GetProjectCompanyList( ) {
		udPBase ret = GetProjectCompany( );
		return ret;
	} // udPBase GetProjectCompanyList

	CHCompany::CHCompany( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltEmail( Glib::locale_to_utf8( "E-mail" ) )
	{
		m_szName = "company";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать компанию" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "email" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHCompany::ColModelCompany( );

		m_pData->SetName( "company" );
		m_pData->SetGetObject( GetProjectCompanyList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// E-mail
		m_tblFilter.attach( m_lblFltEmail, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltEmail, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHCompany

	CHCompany::CHCompany( const CHCompany& objCopy ) : udListHandler( ) {
		m_szName = "company";
	} // CHCompany

	CHCompany::~CHCompany( ) {
	} // ~CCompany

	CHCompany& CHCompany::operator =( const CHCompany& objCopy ) {
		return *this;
	} // CHCompany& operator =

	void CHCompany::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "E-mail" ), m_pColModel->m_col_email );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHCompany::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_email ] = Glib::locale_to_utf8( obj->get( "email" )->toString( ) );
		}
	} // void Load

	void CHCompany::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltEmail.Reset( );
	} // void FilterUiClear

	void CHCompany::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szEmail;

		szName = m_inpFltName.GetValue( );
		szEmail = m_inpFltEmail.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "email" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCompanyModeless::CHCompanyModeless( ) :
		udListHandlerModeless( ),
		CHCompany( )
	{
	} // CHCompanyModeless

	CHCompanyModeless::CHCompanyModeless( const CHCompanyModeless& objCopy ) : CHCompany( ) {
	} // CHCompanyModeless

	CHCompanyModeless::~CHCompanyModeless( ) {
	} // ~CHCompanyModeless

	CHCompanyModeless& CHCompanyModeless::operator =( const CHCompanyModeless& objCopy ) {
		return *this;
	} // CHCompanyModeless& operator =

	void CHCompanyModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHCompany::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHCompanyModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "company_form" );
	} // void OnButtonForm

	void CHCompanyModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "company_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "company_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCompanyPick::CHCompanyPick( ) : CHCompany( ) {
	} // CHCompanyPick

	CHCompanyPick::CHCompanyPick( const CHCompanyPick& objCopy ) : CHCompany( ) {
	} // CHCompanyPick

	CHCompanyPick::~CHCompanyPick( ) {
	} // ~CHCompanyPick

	CHCompanyPick& CHCompanyPick::operator =( const CHCompanyPick& objCopy ) {
		return *this;
	} // CHCompanyPick& operator =

	void CHCompanyPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHCompany::Init( );
	} // void CHCompanyPick

	void CHCompanyPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHCompanyFormModal handler = new udHCompanyFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание компании" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание компании" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHCompanyPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectCompany( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			mb::err(
				*wndParent,
				Glib::locale_to_utf8( "Выберите компанию" ),
				Glib::locale_to_utf8( "Щелкните по строке списка и нажмите кнопку \"ОК\"" )
			);
		}
	} // void OnButtonOk

	void CHCompanyPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHCompanyPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHCompanyFormModal handler = new udHCompanyFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование компании" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование компании" ) );
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

	udPFormHandler GetHCompanyForm( ) {
		return new CHCompanyFormModal;
	} // udPFormHandler GetHCompanyForm

	CHCompanyForm::CHCompanyForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Компания" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblEmail( Glib::locale_to_utf8( "E-mail" ) ),
		m_lblInn( Glib::locale_to_utf8( "ИНН" ) ),
		m_lblUaddr( Glib::locale_to_utf8( "Юридический адрес" ) ),
		m_lblPaddr( Glib::locale_to_utf8( "Почтовый адрес" ) )
	{
		m_szName = "company_form";

		m_pData->SetName( "company" );
		m_pData->SetGetObject( GetProjectCompany );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHCompanyForm

	CHCompanyForm::CHCompanyForm( const CHCompanyForm& objCopy ) : udFormHandler( ) {
	} // CHCompanyForm

	CHCompanyForm::~CHCompanyForm( ) {
	} // ~CHCompanyForm

	CHCompanyForm& CHCompanyForm::operator =( const CHCompanyForm& objCopy ) {
		return *this;
	} // CHCompanyForm& operator =

	void CHCompanyForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// E-mail
		m_tblInput.attach( m_lblEmail, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpEmail, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK );

		// ИНН
		m_tblInput.attach( m_lblInn, 0, 1, 2, 3, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpInn, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		// Юридический адрес
		m_tblInput.attach( m_lblUaddr, 0, 1, 3, 4, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpUaddr, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK );

		// Почтовый адрес
		m_tblInput.attach( m_lblPaddr, 0, 1, 4, 5, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPaddr, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 5, 6, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHCompanyForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpEmail.Reset( );
		m_inpInn.Reset( );
		m_inpUaddr.Reset( );
		m_inpPaddr.Reset( );
	} // void Clear

	void CHCompanyForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpEmail.SetValue( Glib::locale_to_utf8( obj->get( "email" )->toString( ) ) );
		m_inpInn.SetValue( Glib::locale_to_utf8( obj->get( "inn" )->toString( ) ) );
		m_inpUaddr.SetValue( Glib::locale_to_utf8( obj->get( "uaddr" )->toString( ) ) );
		m_inpPaddr.SetValue( Glib::locale_to_utf8( obj->get( "paddr" )->toString( ) ) );
	} // void Load

	udInt CHCompanyForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szEmail,
						szInn,
						szUaddr,
						szPaddr;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName ).GetEntry( ) );
			return 1;
		}

		szEmail = m_inpEmail.GetValue( );
		if ( szEmail.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите E-mail" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEmail ).GetEntry( ) );
			return 2;
		}

		szInn = m_inpInn.GetValue( );
		if ( szInn.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ИНН" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpInn ).GetEntry( ) );
			return 3;
		}

		szUaddr = m_inpUaddr.GetValue( );
		if ( szUaddr.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите юридический адрес" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpUaddr ).GetEntry( ) );
			return 4;
		}

		szPaddr = m_inpPaddr.GetValue( );
		if ( szPaddr.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите почтовый адрес" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpPaddr ).GetEntry( ) );
			return 5;
		}

		return 0;
	} // udInt Validate

	void CHCompanyForm::Send( ) {
		Glib::ustring	szName,
						szEmail,
						szInn,
						szUaddr,
						szPaddr;

		szName = m_inpName.GetValue( );
		szEmail = m_inpEmail.GetValue( );
		szInn = m_inpInn.GetValue( );
		szUaddr = m_inpUaddr.GetValue( );
		szPaddr = m_inpPaddr.GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "email" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
		obj->get( "inn" )->ini( ConvertString( szInn.data( ), CP_UTF8 ) );
		obj->get( "uaddr" )->ini( ConvertString( szUaddr.data( ), CP_UTF8 ) );
		obj->get( "paddr" )->ini( ConvertString( szPaddr.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCompanyFormModal::CHCompanyFormModal( ) : CHCompanyForm( ) {
	} // CHCompanyFormModal

	CHCompanyFormModal::CHCompanyFormModal( const CHCompanyFormModal& objCopy ) : CHCompanyForm( ) {
	} // CHCompanyFormModal

	CHCompanyFormModal::~CHCompanyFormModal( ) {
	} // ~CHCompanyFormModal

	CHCompanyFormModal& CHCompanyFormModal::operator =( const CHCompanyFormModal& objCopy ) {
		return *this;
	} // CHCompanyFormModal& operator =

	void CHCompanyFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHCompanyFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHCompanyForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCompanyFormModeless::CHCompanyFormModeless( ) : CHCompanyForm( ) {
	} // CHCompanyFormModeless

	CHCompanyFormModeless::CHCompanyFormModeless( const CHCompanyFormModeless& objCopy ) : CHCompanyForm( ) {
	} // CHCompanyFormModeless

	CHCompanyFormModeless::~CHCompanyFormModeless( ) {
	} // ~CHCompanyFormModeless

	CHCompanyFormModeless& CHCompanyFormModeless::operator =( const CHCompanyFormModeless& objCopy ) {
		return *this;
	} // CHCompanyFormModeless& operator =

	void CHCompanyFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "company" );
	} // void OnButtonOk

	void CHCompanyFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "company" );
	} // void CHCompanyForm

	void CHCompanyFormModeless::Init( ) {
		CHCompanyForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHCompanyFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHCompanyFormModeless

} // namespace UdSDK
