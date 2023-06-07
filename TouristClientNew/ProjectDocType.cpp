
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHDocTypeListPick( ) {
		return new CHDocTypePick;
	} // udPListHandlerPick GetHDocTypeListPick

	udPBase GetProjectDocTypeList( ) {
		udPBase ret = GetProjectDocType( );
		return ret;
	} // udPBase GetProjectDocTypeList

	CHDocType::CHDocType( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "doctype";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать тип документа" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHDocType::ColModelDocType( );

		m_pData->SetName( "doctype" );
		m_pData->SetGetObject( GetProjectDocTypeList );
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
	} // CHDocType

	CHDocType::CHDocType( const CHDocType& objCopy ) : udListHandler( ) {
		m_szName = "doctype";
	} // CHDocType

	CHDocType::~CHDocType( ) {
	} // ~CDocType

	CHDocType& CHDocType::operator =( const CHDocType& objCopy ) {
		return *this;
	} // CHDocType& operator =

	void CHDocType::Init( ) {
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

	void CHDocType::Load( ) {
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

	void CHDocType::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHDocType::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTypeModeless::CHDocTypeModeless( ) :
		udListHandlerModeless( ),
		CHDocType( )
	{
	} // CHDocTypeModeless

	CHDocTypeModeless::CHDocTypeModeless( const CHDocTypeModeless& objCopy ) : CHDocType( ) {
	} // CHDocTypeModeless

	CHDocTypeModeless::~CHDocTypeModeless( ) {
	} // ~CHDocTypeModeless

	CHDocTypeModeless& CHDocTypeModeless::operator =( const CHDocTypeModeless& objCopy ) {
		return *this;
	} // CHDocTypeModeless& operator =

	void CHDocTypeModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHDocType::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHDocTypeModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "doctype_form" );
	} // void OnButtonForm

	void CHDocTypeModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "doctype_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "doctype_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTypePick::CHDocTypePick( ) : CHDocType( ) {
	} // CHDocTypePick

	CHDocTypePick::CHDocTypePick( const CHDocTypePick& objCopy ) : CHDocType( ) {
	} // CHDocTypePick

	CHDocTypePick::~CHDocTypePick( ) {
	} // ~CHDocTypePick

	CHDocTypePick& CHDocTypePick::operator =( const CHDocTypePick& objCopy ) {
		return *this;
	} // CHDocTypePick& operator =

	void CHDocTypePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHDocType::Init( );
	} // void CHDocTypePick

	void CHDocTypePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHDocTypeFormModal handler = new udHDocTypeFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание типа документа" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание типа документа" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHDocTypePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectDocType( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите тип документа" ),
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

	void CHDocTypePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHDocTypePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHDocTypeFormModal handler = new udHDocTypeFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование типа документа" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование типа документа" ) );
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

	udPFormHandler GetHDocTypeForm( ) {
		return new CHDocTypeFormModal;
	} // udPFormHandler GetHDocTypeForm

	CHDocTypeForm::CHDocTypeForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Тип документа" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "doctype_form";

		m_pData->SetName( "doctype" );
		m_pData->SetGetObject( GetProjectDocType );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHDocTypeForm

	CHDocTypeForm::CHDocTypeForm( const CHDocTypeForm& objCopy ) : udFormHandler( ) {
	} // CHDocTypeForm

	CHDocTypeForm::~CHDocTypeForm( ) {
	} // ~CHDocTypeForm

	CHDocTypeForm& CHDocTypeForm::operator =( const CHDocTypeForm& objCopy ) {
		return *this;
	} // CHDocTypeForm& operator =

	void CHDocTypeForm::Init( ) {
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

	void CHDocTypeForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHDocTypeForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHDocTypeForm::Validate( ) {
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

	void CHDocTypeForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTypeFormModal::CHDocTypeFormModal( ) : CHDocTypeForm( ) {
	} // CHDocTypeFormModal

	CHDocTypeFormModal::CHDocTypeFormModal( const CHDocTypeFormModal& objCopy ) : CHDocTypeForm( ) {
	} // CHDocTypeFormModal

	CHDocTypeFormModal::~CHDocTypeFormModal( ) {
	} // ~CHDocTypeFormModal

	CHDocTypeFormModal& CHDocTypeFormModal::operator =( const CHDocTypeFormModal& objCopy ) {
		return *this;
	} // CHDocTypeFormModal& operator =

	void CHDocTypeFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHDocTypeFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHDocTypeForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTypeFormModeless::CHDocTypeFormModeless( ) : CHDocTypeForm( ) {
	} // CHDocTypeFormModeless

	CHDocTypeFormModeless::CHDocTypeFormModeless( const CHDocTypeFormModeless& objCopy ) : CHDocTypeForm( ) {
	} // CHDocTypeFormModeless

	CHDocTypeFormModeless::~CHDocTypeFormModeless( ) {
	} // ~CHDocTypeFormModeless

	CHDocTypeFormModeless& CHDocTypeFormModeless::operator =( const CHDocTypeFormModeless& objCopy ) {
		return *this;
	} // CHDocTypeFormModeless& operator =

	void CHDocTypeFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "doctype" );
	} // void OnButtonOk

	void CHDocTypeFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "doctype" );
	} // void CHDocTypeForm

	void CHDocTypeFormModeless::Init( ) {
		CHDocTypeForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHDocTypeFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHDocTypeFormModeless

} // namespace UdSDK
