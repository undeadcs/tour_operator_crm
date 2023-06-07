
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHTransferPointListPick( ) {
		return new CHTransferPointPick;
	} // udPListHandlerPick GetHTransferPointListPick

	udPBase GetProjectTransferPointList( ) {
		udPBase ret = GetProjectTransferPoint( );
		return ret;
	} // udPBase GetProjectTransferPointList

	CHTransferPoint::CHTransferPoint( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "transferpoint";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать пункт перехода" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHTransferPoint::ColModelTransferPoint( );

		m_pData->SetName( "transferpoint" );
		m_pData->SetGetObject( GetProjectTransferPointList );
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
	} // CHTransferPoint

	CHTransferPoint::CHTransferPoint( const CHTransferPoint& objCopy ) : udListHandler( ) {
		m_szName = "transferpoint";
	} // CHTransferPoint

	CHTransferPoint::~CHTransferPoint( ) {
	} // ~CTransferPoint

	CHTransferPoint& CHTransferPoint::operator =( const CHTransferPoint& objCopy ) {
		return *this;
	} // CHTransferPoint& operator =

	void CHTransferPoint::Init( ) {
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

	void CHTransferPoint::Load( ) {
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

	void CHTransferPoint::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHTransferPoint::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTransferPointModeless::CHTransferPointModeless( ) :
		udListHandlerModeless( ),
		CHTransferPoint( )
	{
	} // CHTransferPointModeless

	CHTransferPointModeless::CHTransferPointModeless( const CHTransferPointModeless& objCopy ) : CHTransferPoint( ) {
	} // CHTransferPointModeless

	CHTransferPointModeless::~CHTransferPointModeless( ) {
	} // ~CHTransferPointModeless

	CHTransferPointModeless& CHTransferPointModeless::operator =( const CHTransferPointModeless& objCopy ) {
		return *this;
	} // CHTransferPointModeless& operator =

	void CHTransferPointModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHTransferPoint::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHTransferPointModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "transferpoint_form" );
	} // void OnButtonForm

	void CHTransferPointModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "transferpoint_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "transferpoint_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTransferPointPick::CHTransferPointPick( ) : CHTransferPoint( ) {
	} // CHTransferPointPick

	CHTransferPointPick::CHTransferPointPick( const CHTransferPointPick& objCopy ) : CHTransferPoint( ) {
	} // CHTransferPointPick

	CHTransferPointPick::~CHTransferPointPick( ) {
	} // ~CHTransferPointPick

	CHTransferPointPick& CHTransferPointPick::operator =( const CHTransferPointPick& objCopy ) {
		return *this;
	} // CHTransferPointPick& operator =

	void CHTransferPointPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTransferPoint::Init( );
	} // void CHTransferPointPick

	void CHTransferPointPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHTransferPointFormModal handler = new udHTransferPointFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание пункта перехода" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание пункта перехода" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHTransferPointPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectTransferPoint( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите пункт перехода" ),
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

	void CHTransferPointPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTransferPointPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHTransferPointFormModal handler = new udHTransferPointFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование пункта перехода" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование пункта перехода" ) );
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

	udPFormHandler GetHTransferPointForm( ) {
		return new CHTransferPointFormModal;
	} // udPFormHandler GetHTransferPointForm
	
	CHTransferPointForm::CHTransferPointForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Пункт перехода" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblNameLatin( Glib::locale_to_utf8( "Наименование латиницей" ) )
	{
		m_szName = "transferpoint_form";

		m_pData->SetName( "transferpoint" );
		m_pData->SetGetObject( GetProjectTransferPoint );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHTransferPointForm

	CHTransferPointForm::CHTransferPointForm( const CHTransferPointForm& objCopy ) : udFormHandler( ) {
	} // CHTransferPointForm

	CHTransferPointForm::~CHTransferPointForm( ) {
	} // ~CHTransferPointForm

	CHTransferPointForm& CHTransferPointForm::operator =( const CHTransferPointForm& objCopy ) {
		return *this;
	} // CHTransferPointForm& operator =

	void CHTransferPointForm::Init( ) {
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
		udFormHandler::Init( );
	} // void Init

	void CHTransferPointForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpNameLatin.Reset( );
	} // void Clear

	void CHTransferPointForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpNameLatin.SetValue( Glib::locale_to_utf8( obj->get( "namelatin" )->toString( ) ) );
	} // void Load

	udInt CHTransferPointForm::Validate( ) {
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
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHTransferPointForm::Send( ) {
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

	CHTransferPointFormModal::CHTransferPointFormModal( ) : CHTransferPointForm( ) {
	} // CHTransferPointFormModal

	CHTransferPointFormModal::CHTransferPointFormModal( const CHTransferPointFormModal& objCopy ) : CHTransferPointForm( ) {
	} // CHTransferPointFormModal

	CHTransferPointFormModal::~CHTransferPointFormModal( ) {
	} // ~CHTransferPointFormModal

	CHTransferPointFormModal& CHTransferPointFormModal::operator =( const CHTransferPointFormModal& objCopy ) {
		return *this;
	} // CHTransferPointFormModal& operator =

	void CHTransferPointFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTransferPointFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTransferPointForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTransferPointFormModeless::CHTransferPointFormModeless( ) : CHTransferPointForm( ) {
	} // CHTransferPointFormModeless

	CHTransferPointFormModeless::CHTransferPointFormModeless( const CHTransferPointFormModeless& objCopy ) : CHTransferPointForm( ) {
	} // CHTransferPointFormModeless

	CHTransferPointFormModeless::~CHTransferPointFormModeless( ) {
	} // ~CHTransferPointFormModeless

	CHTransferPointFormModeless& CHTransferPointFormModeless::operator =( const CHTransferPointFormModeless& objCopy ) {
		return *this;
	} // CHTransferPointFormModeless& operator =

	void CHTransferPointFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "transferpoint" );
	} // void OnButtonOk

	void CHTransferPointFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "transferpoint" );
	} // void CHTransferPointForm

	void CHTransferPointFormModeless::Init( ) {
		CHTransferPointForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHTransferPointFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHTransferPointFormModeless

} // namespace UdSDK
