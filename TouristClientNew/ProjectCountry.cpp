
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHCountryListPick( ) {
		return new CHCountryPick;
	} // udPListHandlerPick GetHCountryListPick

	udPBase GetProjectCountryList( ) {
		udPBase ret = GetProjectCountry( );
		return ret;
	} // udPBase GetProjectCountryList

	CHCountry::CHCountry( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "country";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать страну" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHCountry::ColModelCountry( );

		m_pData->SetName( "country" );
		m_pData->SetGetObject( GetProjectCountryList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHCountry

	CHCountry::CHCountry( const CHCountry& objCopy ) : udListHandler( ) {
		m_szName = "country";
	} // CHCountry

	CHCountry::~CHCountry( ) {
	} // ~CCountry

	CHCountry& CHCountry::operator =( const CHCountry& objCopy ) {
		return *this;
	} // CHCountry& operator =

	void CHCountry::Init( ) {
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
		
		//col = m_objTreeView.get_column( 0 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHCountry::OnColClickId ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 1 );
		col->set_clickable( true );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHCountry::OnColClickName ) );
	} // void Init

	void CHCountry::Load( ) {
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

	void CHCountry::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHCountry::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	void CHCountry::OnColClickId( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "id" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "id" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		
		Load( );
	} // void OnColClickId

	void CHCountry::OnColClickName( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "name" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "name" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		Load( );
	} // void OnColClickName

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCountryModeless::CHCountryModeless( ) :
		udListHandlerModeless( ),
		CHCountry( )
	{
	} // CHCountryModeless

	CHCountryModeless::CHCountryModeless( const CHCountryModeless& objCopy ) : CHCountry( ) {
	} // CHCountryModeless

	CHCountryModeless::~CHCountryModeless( ) {
	} // ~CHCountryModeless

	CHCountryModeless& CHCountryModeless::operator =( const CHCountryModeless& objCopy ) {
		return *this;
	} // CHCountryModeless& operator =

	void CHCountryModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHCountry::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHCountryModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "country_form" );
	} // void OnButtonForm

	void CHCountryModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "country_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "country_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCountryPick::CHCountryPick( ) : CHCountry( ) {
	} // CHCountryPick

	CHCountryPick::CHCountryPick( const CHCountryPick& objCopy ) : CHCountry( ) {
	} // CHCountryPick

	CHCountryPick::~CHCountryPick( ) {
	} // ~CHCountryPick

	CHCountryPick& CHCountryPick::operator =( const CHCountryPick& objCopy ) {
		return *this;
	} // CHCountryPick& operator =

	void CHCountryPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHCountry::Init( );
	} // void CHCountryPick

	void CHCountryPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHCountryFormModal handler = new udHCountryFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание страны" ) );

		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHCountryPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectCountry( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите страну" ),
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

	void CHCountryPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHCountryPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHCountryFormModal handler = new udHCountryFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование страны" ) );

			udDELETE( handler );

			Clear( );
			Load( );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPFormHandler GetHCountryForm( ) {
		return new CHCountryFormModal;
	} // udPFormHandler GetHCountryForm

	CHCountryForm::CHCountryForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Страна" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "country_form";

		m_pData->SetName( "country" );
		m_pData->SetGetObject( GetProjectCountry );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
	} // CHCountryForm

	CHCountryForm::CHCountryForm( const CHCountryForm& objCopy ) : udFormHandler( ) {
	} // CHCountryForm

	CHCountryForm::~CHCountryForm( ) {
	} // ~CHCountryForm

	CHCountryForm& CHCountryForm::operator =( const CHCountryForm& objCopy ) {
		return *this;
	} // CHCountryForm& operator =

	void CHCountryForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 0, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHCountryForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHCountryForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHCountryForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHCountryForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCountryFormModal::CHCountryFormModal( ) : CHCountryForm( ) {
	} // CHCountryFormModal

	CHCountryFormModal::CHCountryFormModal( const CHCountryFormModal& objCopy ) : CHCountryForm( ) {
	} // CHCountryFormModal

	CHCountryFormModal::~CHCountryFormModal( ) {
	} // ~CHCountryFormModal

	CHCountryFormModal& CHCountryFormModal::operator =( const CHCountryFormModal& objCopy ) {
		return *this;
	} // CHCountryFormModal& operator =

	void CHCountryFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHCountryFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHCountryForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCountryFormModeless::CHCountryFormModeless( ) : CHCountryForm( ) {
	} // CHCountryFormModeless

	CHCountryFormModeless::CHCountryFormModeless( const CHCountryFormModeless& objCopy ) : CHCountryForm( ) {
	} // CHCountryFormModeless

	CHCountryFormModeless::~CHCountryFormModeless( ) {
	} // ~CHCountryFormModeless

	CHCountryFormModeless& CHCountryFormModeless::operator =( const CHCountryFormModeless& objCopy ) {
		return *this;
	} // CHCountryFormModeless& operator =

	void CHCountryFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "country" );
	} // void OnButtonOk

	void CHCountryFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "country" );
	} // void CHCountryForm

	void CHCountryFormModeless::Init( ) {
		CHCountryForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHCountryFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHCountryFormModeless

} // namespace UdSDK
