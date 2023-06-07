
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWgtServicePeriodPick::CWgtServicePeriodPick( ) : udWgtListPick( ) {
	} // CWgtEmployerPick

	CWgtServicePeriodPick::CWgtServicePeriodPick( const CWgtServicePeriodPick& objCopy ) : udWgtListPick( ) {
	} // CWgtEmployerPick

	CWgtServicePeriodPick::~CWgtServicePeriodPick( ) {
	} // ~CWgtServicePeriodPick

	CWgtServicePeriodPick& CWgtServicePeriodPick::operator =( const CWgtServicePeriodPick& objCopy ) {
		return *this;
	} // CWgtEmployerPick& operator =

	void CWgtServicePeriodPick::OnButtonClick( ) {
		udPHServicePeriodPick handler = new CHServicePeriodPick;
		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		handler->SetServiceForm( m_pServiceForm );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szWindowTitle );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( m_szWindowTitle );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		//wnd.show_all( );
		wnd.show( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj && m_pFde ) {
			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );

			m_lblTitle.set_text( Glib::locale_to_utf8(
				obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) + " - " +
				obj->get( "date2" )->toTime( ).toString( "%d.%m.%Y" )
			) );
		}

		udDELETE( handler );
	} // void OnButtonClick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CDAListServicePeriod::CDAListServicePeriod( ) :
		udDAList( ),
		m_pServiceForm( NULL )
	{
	} // CDAListServicePeriod

	CDAListServicePeriod::CDAListServicePeriod( const CDAListServicePeriod& objCopy ) {
	} // CDAListServicePeriod

	CDAListServicePeriod::~CDAListServicePeriod( ) {
	} // ~CDAListServicePeriod

	CDAListServicePeriod& CDAListServicePeriod::operator =( const CDAListServicePeriod& objCopy ) {
		return *this;
	} // CDAListServicePeriod& operator =

	void CDAListServicePeriod::Load( ) {
		if ( m_pServiceForm ) {
			udPBase add = NULL;
			udPArrBase arrObject = ( ( udPFDEList ) m_pServiceForm->GetData( )->GetExtraData( udHServiceForm::ctrlidPeriod ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = new udProjectServicePeriod;
					add->apply( *i, true, true );
					m_arrObject->push_back( add );
				}
			}
		}
	} // void Load





	udPListHandlerPick GetHServicePeriodListPick( ) {
		return new CHServicePeriodPick;
	} // udPListHandlerPick GetHServicePeriodListPick

	udPBase GetProjectServicePeriodList( ) {
		udPBase ret = GetProjectServicePeriod( );
		return ret;
	} // udPBase GetProjectServicePeriodList

	CHServicePeriod::CHServicePeriod( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_pServiceForm( NULL )
	{
		udDELETE( m_pData );

		m_pData = new CDAListServicePeriod;

		m_szName = "serviceperiod";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать период" ) );

		m_pRowModel = m_pColModel = new CHServicePeriod::ColModelServicePeriod( );

		m_pData->SetName( "serviceperiod" );
		m_pData->SetGetObject( GetProjectServicePeriodList );
	} // CHServicePeriod

	CHServicePeriod::CHServicePeriod( const CHServicePeriod& objCopy ) : udListHandler( ) {
		m_szName = "country";
	} // CHServicePeriod

	CHServicePeriod::~CHServicePeriod( ) {
	} // ~CServicePeriod

	CHServicePeriod& CHServicePeriod::operator =( const CHServicePeriod& objCopy ) {
		return *this;
	} // CHServicePeriod& operator =

	void CHServicePeriod::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		//m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "С" ), m_pColModel->m_col_date1 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "По" ), m_pColModel->m_col_date2 );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHServicePeriod::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		Gtk::TreeModel::iterator itRow;
		Gtk::TreePath objPath;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			itRow = m_objTreeModel->append( );
			objPath = m_objTreeModel->get_path( itRow );
			m_mapObject[ objPath.to_string( ) ] = obj;
			row = *itRow;
			//row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_date1 ] = Glib::locale_to_utf8( obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_date2 ] = Glib::locale_to_utf8( obj->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
		}
	} // void Load

	void CHServicePeriod::SetServiceForm( udPHServiceForm pServiceForm ) {
		m_pServiceForm = pServiceForm;
		( ( CDAListServicePeriod* ) m_pData )->SetServiceForm( pServiceForm );
	} // void SetServiceForm

	void CHServicePeriod::FilterUiClear( ) {
	} // void FilterUiClear

	void CHServicePeriod::FilterUiToObject( ) {
	} // void FilterUiToObject




	CHServicePeriodPick::CHServicePeriodPick( ) : CHServicePeriod( ) {
	} // CHServicePeriodPick

	CHServicePeriodPick::CHServicePeriodPick( const CHServicePeriodPick& objCopy ) : CHServicePeriod( ) {
	} // CHServicePeriodPick

	CHServicePeriodPick::~CHServicePeriodPick( ) {
	} // ~CHServicePeriodPick

	CHServicePeriodPick& CHServicePeriodPick::operator =( const CHServicePeriodPick& objCopy ) {
		return *this;
	} // CHServicePeriodPick& operator =

	void CHServicePeriodPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHServicePeriod::Init( );
	} // void CHServicePeriodPick

	void CHServicePeriodPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHServicePeriodFormModal handler = new udHServicePeriodFormModal( m_pServiceForm );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание периода" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание периода" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );
		udDELETE( handler );*/

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHServicePeriodPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectServicePeriod( );
				m_pPickedObject->apply( itObj->second, true, true );
			}

			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите период" ),
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

	void CHServicePeriodPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHServicePeriodPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( i ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				Gtk::TreeModel::Row row = *i;
				Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
				udPHServicePeriodFormModal handler = new udHServicePeriodFormModal( m_pServiceForm );
				handler->Init( );
				handler->LoadFrom( itObj->second );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование периода" ) );

				/*Gtk::Window wnd;
				wnd.set_modal( true );
				wnd.set_title( Glib::locale_to_utf8( "Редактирование периода" ) );
				wnd.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				wnd.show_all( );
				Gtk::Main::run( wnd );*/
				udDELETE( handler );

				Clear( );
				Load( );
			}
		}
	} // void OnRowActivated





	CHServicePeriodForm::CHServicePeriodForm( udPHServiceForm pServiceForm ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Период" ) ),
		m_lblDate1( Glib::locale_to_utf8( "С" ) ),
		m_lblDate2( Glib::locale_to_utf8( "По" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) ),
		m_pServiceForm( pServiceForm )
	{
		m_szName = "serviceperiod_form";

		m_pData->SetName( "serviceperiod" );
		m_pData->SetGetObject( GetProjectServicePeriod );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDate1.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDate1.set_size_request( 250 );
	} // CHServicePeriodForm

	CHServicePeriodForm::CHServicePeriodForm( const CHServicePeriodForm& objCopy ) : udFormHandler( ) {
	} // CHServicePeriodForm

	CHServicePeriodForm::~CHServicePeriodForm( ) {
	} // ~CHServicePeriodForm

	CHServicePeriodForm& CHServicePeriodForm::operator =( const CHServicePeriodForm& objCopy ) {
		return *this;
	} // CHServicePeriodForm& operator =

	void CHServicePeriodForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Дата "с"
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата "по"
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHServicePeriodForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
	} // void Clear

	void CHServicePeriodForm::Load( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
	} // void Load

	udInt CHServicePeriodForm::Validate( ) {
		return 0;
	} // udInt Validate

	void CHServicePeriodForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate2.GetValue( ) );

		if ( m_pServiceForm ) {
			m_pServiceForm->SavePeriod( obj );
		}
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHServicePeriodFormModal::CHServicePeriodFormModal( udPHServiceForm pServiceForm ) : CHServicePeriodForm( pServiceForm ) {
	} // CHServicePeriodFormModal

	CHServicePeriodFormModal::CHServicePeriodFormModal( const CHServicePeriodFormModal& objCopy ) : CHServicePeriodForm( ) {
	} // CHServicePeriodFormModal

	CHServicePeriodFormModal::~CHServicePeriodFormModal( ) {
	} // ~CHServicePeriodFormModal

	CHServicePeriodFormModal& CHServicePeriodFormModal::operator =( const CHServicePeriodFormModal& objCopy ) {
		return *this;
	} // CHServicePeriodFormModal& operator =

	void CHServicePeriodFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHServicePeriodFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHServicePeriodForm

	void CHServicePeriodFormModal::LoadFrom( udPBase from ) {
		//udFormHandler::Load( );
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
	} // void LoadFrom

} // namespace UdSDK
