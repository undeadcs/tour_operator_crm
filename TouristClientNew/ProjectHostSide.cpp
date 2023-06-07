
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHHostSideListPick( ) {
		return new CHHostSidePick;
	} // udPListHandlerPick GetHHostSideListPick

	udPBase GetProjectHostSideList( ) {
		udPBase ret = GetProjectHostSide( );
		return ret;
	} // udPBase GetProjectHostSideList

	CHHostSide::CHHostSide( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "hostside";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать принимающую сторону" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHHostSide::ColModelHostSide( );

		m_pData->SetName( "hostside" );
		m_pData->SetGetObject( GetProjectHostSideList );
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
	} // CHHostSide

	CHHostSide::CHHostSide( const CHHostSide& objCopy ) : udListHandler( ) {
		m_szName = "hostside";
	} // CHHostSide

	CHHostSide::~CHHostSide( ) {
	} // ~CHostSide

	CHHostSide& CHHostSide::operator =( const CHHostSide& objCopy ) {
		return *this;
	} // CHHostSide& operator =

	void CHHostSide::Init( ) {
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

	void CHHostSide::Load( ) {
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

	void CHHostSide::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHHostSide::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHHostSideModeless::CHHostSideModeless( ) :
		udListHandlerModeless( ),
		CHHostSide( )
	{
	} // CHHostSideModeless

	CHHostSideModeless::CHHostSideModeless( const CHHostSideModeless& objCopy ) : CHHostSide( ) {
	} // CHHostSideModeless

	CHHostSideModeless::~CHHostSideModeless( ) {
	} // ~CHHostSideModeless

	CHHostSideModeless& CHHostSideModeless::operator =( const CHHostSideModeless& objCopy ) {
		return *this;
	} // CHHostSideModeless& operator =

	void CHHostSideModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHHostSide::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHHostSideModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "hostside_form" );
	} // void OnButtonForm

	void CHHostSideModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "hostside_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "hostside_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHHostSidePick::CHHostSidePick( ) : CHHostSide( ) {
	} // CHHostSidePick

	CHHostSidePick::CHHostSidePick( const CHHostSidePick& objCopy ) : CHHostSide( ) {
	} // CHHostSidePick

	CHHostSidePick::~CHHostSidePick( ) {
	} // ~CHHostSidePick

	CHHostSidePick& CHHostSidePick::operator =( const CHHostSidePick& objCopy ) {
		return *this;
	} // CHHostSidePick& operator =

	void CHHostSidePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHHostSide::Init( );
	} // void CHHostSidePick

	void CHHostSidePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHHostSideFormModal handler = new udHHostSideFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание принимающей стороны" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание принимающей стороны" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHHostSidePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectHostSide( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите принимающую сторону" ),
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

	void CHHostSidePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHHostSidePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHHostSideFormModal handler = new udHHostSideFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование принимающей стороны" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование принимающей стороны" ) );
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

	udPFormHandler GetHHostSideForm( ) {
		return new CHHostSideFormModal;
	} // udPFormHandler GetHHostSideForm

	CHHostSideForm::CHHostSideForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Принимающая сторона" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblNameLatin( Glib::locale_to_utf8( "Наименование латиницей" ) )
	{
		m_szName = "hostside_form";

		m_pData->SetName( "hostside" );
		m_pData->SetGetObject( GetProjectHostSide );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHHostSideForm

	CHHostSideForm::CHHostSideForm( const CHHostSideForm& objCopy ) : udFormHandler( ) {
	} // CHHostSideForm

	CHHostSideForm::~CHHostSideForm( ) {
	} // ~CHHostSideForm

	CHHostSideForm& CHHostSideForm::operator =( const CHHostSideForm& objCopy ) {
		return *this;
	} // CHHostSideForm& operator =

	void CHHostSideForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Наименование латиницей
		m_tblInput.attach( m_lblNameLatin, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNameLatin, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	void CHHostSideForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpNameLatin.Reset( );
	} // void Clear

	void CHHostSideForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpNameLatin.SetValue( Glib::locale_to_utf8( obj->get( "namelatin" )->toString( ) ) );
	} // void Load

	udInt CHHostSideForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szNameLatin;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		szNameLatin = m_inpNameLatin.GetValue( );
		if ( szNameLatin.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование латиницей" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpNameLatin.GetEntry( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHHostSideForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szNameLatin;

		szName = m_inpName.GetValue( );
		szNameLatin = m_inpNameLatin.GetValue( );

		udPBase obj = m_pData->GetData( );
		
		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "namelatin" )->ini( ConvertString( szNameLatin.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHHostSideFormModal::CHHostSideFormModal( ) : CHHostSideForm( ) {
	} // CHHostSideFormModal

	CHHostSideFormModal::CHHostSideFormModal( const CHHostSideFormModal& objCopy ) : CHHostSideForm( ) {
	} // CHHostSideFormModal

	CHHostSideFormModal::~CHHostSideFormModal( ) {
	} // ~CHHostSideFormModal

	CHHostSideFormModal& CHHostSideFormModal::operator =( const CHHostSideFormModal& objCopy ) {
		return *this;
	} // CHHostSideFormModal& operator =

	void CHHostSideFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHHostSideFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHHostSideForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHHostSideFormModeless::CHHostSideFormModeless( ) : CHHostSideForm( ) {
	} // CHHostSideFormModeless

	CHHostSideFormModeless::CHHostSideFormModeless( const CHHostSideFormModeless& objCopy ) : CHHostSideForm( ) {
	} // CHHostSideFormModeless

	CHHostSideFormModeless::~CHHostSideFormModeless( ) {
	} // ~CHHostSideFormModeless

	CHHostSideFormModeless& CHHostSideFormModeless::operator =( const CHHostSideFormModeless& objCopy ) {
		return *this;
	} // CHHostSideFormModeless& operator =

	void CHHostSideFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "hostside" );
	} // void OnButtonOk

	void CHHostSideFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "hostside" );
	} // void CHHostSideForm

	void CHHostSideFormModeless::Init( ) {
		CHHostSideForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHHostSideFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHHostSideFormModeless

} // namespace UdSDK
