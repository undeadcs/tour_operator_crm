
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHCurrencyListPick( ) {
		return new CHCurrencyPick;
	} // udPListHandlerPick GetHCurrencyListPick

	udPBase GetProjectCurrencyList( ) {
		udPBase ret = GetProjectCurrency( );
		return ret;
	} // udPBase GetProjectCurrencyList

	CHCurrency::CHCurrency( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltCode( Glib::locale_to_utf8( "Код" ) )
	{
		m_szName = "currency";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать валюту" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "code" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHCurrency::ColModelCurrency( );

		m_pData->SetName( "currency" );
		m_pData->SetGetObject( GetProjectCurrencyList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Код
		m_tblFilter.attach( m_lblFltCode, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCode, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHCurrency

	CHCurrency::CHCurrency( const CHCurrency& objCopy ) : udListHandler( ) {
		m_szName = "currency";
	} // CHCurrency

	CHCurrency::~CHCurrency( ) {
	} // ~CCurrency

	CHCurrency& CHCurrency::operator =( const CHCurrency& objCopy ) {
		return *this;
	} // CHCurrency& operator =

	void CHCurrency::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Код" ), m_pColModel->m_col_code );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHCurrency::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_code ] = Glib::locale_to_utf8( obj->get( "code" )->toString( ) );
		}
	} // void Load

	void CHCurrency::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltCode.Reset( );
	} // void FilterUiClear

	void CHCurrency::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szCode;

		szName = m_inpFltName.GetValue( );
		szCode = m_inpFltCode.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "code" )->ini( ConvertString( szCode.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyModeless::CHCurrencyModeless( ) :
		udListHandlerModeless( ),
		CHCurrency( )
	{
	} // CHCurrencyModeless

	CHCurrencyModeless::CHCurrencyModeless( const CHCurrencyModeless& objCopy ) : CHCurrency( ) {
	} // CHCurrencyModeless

	CHCurrencyModeless::~CHCurrencyModeless( ) {
	} // ~CHCurrencyModeless

	CHCurrencyModeless& CHCurrencyModeless::operator =( const CHCurrencyModeless& objCopy ) {
		return *this;
	} // CHCurrencyModeless& operator =

	void CHCurrencyModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHCurrency::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHCurrencyModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "currency_form" );
	} // void OnButtonForm

	void CHCurrencyModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "currency_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "currency_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyPick::CHCurrencyPick( ) : CHCurrency( ) {
	} // CHCurrencyPick

	CHCurrencyPick::CHCurrencyPick( const CHCurrencyPick& objCopy ) : CHCurrency( ) {
	} // CHCurrencyPick

	CHCurrencyPick::~CHCurrencyPick( ) {
	} // ~CHCurrencyPick

	CHCurrencyPick& CHCurrencyPick::operator =( const CHCurrencyPick& objCopy ) {
		return *this;
	} // CHCurrencyPick& operator =

	void CHCurrencyPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHCurrency::Init( );
	} // void CHCurrencyPick

	void CHCurrencyPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHCurrencyFormModal handler = new udHCurrencyFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание валюты" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание валюты" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHCurrencyPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectCurrency( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			mb::err(
				*wndParent,
				Glib::locale_to_utf8( "Выберите валюту" ),
				Glib::locale_to_utf8( "Щелкните по строке списка и нажмите кнопку \"ОК\"" )
			);
		}
	} // void OnButtonOk

	void CHCurrencyPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHCurrencyPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHCurrencyFormModal handler = new udHCurrencyFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование валюты" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование валюты" ) );
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

	udPFormHandler GetHCurrencyForm( ) {
		return new CHCurrencyFormModal;
	} // udPFormHandler GetHCurrencyForm

	CHCurrencyForm::CHCurrencyForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Валюта" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblCode( Glib::locale_to_utf8( "Код" ) )
	{
		m_szName = "currency_form";

		m_pData->SetName( "currency" );
		m_pData->SetGetObject( GetProjectCurrency );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHCurrencyForm

	CHCurrencyForm::CHCurrencyForm( const CHCurrencyForm& objCopy ) : udFormHandler( ) {
	} // CHCurrencyForm

	CHCurrencyForm::~CHCurrencyForm( ) {
	} // ~CHCurrencyForm

	CHCurrencyForm& CHCurrencyForm::operator =( const CHCurrencyForm& objCopy ) {
		return *this;
	} // CHCurrencyForm& operator =

	void CHCurrencyForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// код
		m_tblInput.attach( m_lblCode, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCode, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHCurrencyForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpCode.Reset( );
	} // void Clear

	void CHCurrencyForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpCode.SetValue( Glib::locale_to_utf8( obj->get( "code" )->toString( ) ) );
	} // void Load

	udInt CHCurrencyForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szCode;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName ).GetEntry( ) );
			return 1;
		}

		szCode = m_inpCode.GetValue( );
		if ( szCode.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите код" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCode ).GetEntry( ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHCurrencyForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName, szCode;

		szName = m_inpName.GetValue( );
		szCode = m_inpCode.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "code" )->ini( ConvertString( szCode.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyFormModal::CHCurrencyFormModal( ) : CHCurrencyForm( ) {
	} // CHCurrencyFormModal

	CHCurrencyFormModal::CHCurrencyFormModal( const CHCurrencyFormModal& objCopy ) : CHCurrencyForm( ) {
	} // CHCurrencyFormModal

	CHCurrencyFormModal::~CHCurrencyFormModal( ) {
	} // ~CHCurrencyFormModal

	CHCurrencyFormModal& CHCurrencyFormModal::operator =( const CHCurrencyFormModal& objCopy ) {
		return *this;
	} // CHCurrencyFormModal& operator =

	void CHCurrencyFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHCurrencyFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHCurrencyForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyFormModeless::CHCurrencyFormModeless( ) : CHCurrencyForm( ) {
	} // CHCurrencyFormModeless

	CHCurrencyFormModeless::CHCurrencyFormModeless( const CHCurrencyFormModeless& objCopy ) : CHCurrencyForm( ) {
	} // CHCurrencyFormModeless

	CHCurrencyFormModeless::~CHCurrencyFormModeless( ) {
	} // ~CHCurrencyFormModeless

	CHCurrencyFormModeless& CHCurrencyFormModeless::operator =( const CHCurrencyFormModeless& objCopy ) {
		return *this;
	} // CHCurrencyFormModeless& operator =

	void CHCurrencyFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "currency" );
	} // void OnButtonOk

	void CHCurrencyFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "currency" );
	} // void CHCurrencyForm

	void CHCurrencyFormModeless::Init( ) {
		CHCurrencyForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHCurrencyFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHCurrencyFormModeless

} // namespace UdSDK
