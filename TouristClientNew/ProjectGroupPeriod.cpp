
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWgtGroupPeriodPick::CWgtGroupPeriodPick( ) : udWgtListPick( ), m_pGroupForm( NULL ) {
	} // CWgtEmployerPick

	CWgtGroupPeriodPick::CWgtGroupPeriodPick( const CWgtGroupPeriodPick& objCopy ) : udWgtListPick( ) {
	} // CWgtEmployerPick

	CWgtGroupPeriodPick::~CWgtGroupPeriodPick( ) {
	} // ~CWgtGroupPeriodPick

	CWgtGroupPeriodPick& CWgtGroupPeriodPick::operator =( const CWgtGroupPeriodPick& objCopy ) {
		return *this;
	} // CWgtEmployerPick& operator =

	void CWgtGroupPeriodPick::OnButtonClick( ) {
		udPHGroupPeriodPick handler = new CHGroupPeriodPick;
		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		handler->SetGroupForm( m_pGroupForm );
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
				obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" )
			) );
		}

		udDELETE( handler );
	} // void OnButtonClick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CDAListGroupPeriod::CDAListGroupPeriod( ) :
		udDAList( ),
		m_pGroupForm( NULL )
	{
	} // CDAListGroupPeriod

	CDAListGroupPeriod::CDAListGroupPeriod( const CDAListGroupPeriod& objCopy ) {
	} // CDAListGroupPeriod

	CDAListGroupPeriod::~CDAListGroupPeriod( ) {
	} // ~CDAListGroupPeriod

	CDAListGroupPeriod& CDAListGroupPeriod::operator =( const CDAListGroupPeriod& objCopy ) {
		return *this;
	} // CDAListGroupPeriod& operator =

	void CDAListGroupPeriod::Load( ) {
		g_pApp->GetLog( )->Write( "CDAListGroupPeriod::Load, enter\n" );

		if ( m_pGroupForm ) {
			udPBase add = NULL;
			udPArrBase arrObject = ( ( udPFDEList ) m_pGroupForm->GetData( )->GetExtraData( udHGroupForm::ctrlidPeriod ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				g_pApp->GetLog( )->Write( "CDAListGroupPeriod::Load, size=" + toString( arrObject->size( ) ) + "\n" );

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = new udProjectGroupPeriod;
					add->apply( *i, true, true );
					m_arrObject->push_back( add );
				}
			}
		}

		g_pApp->GetLog( )->Write( "CDAListGroupPeriod::Load, exit\n" );
	} // void Load

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPListHandlerPick GetHGroupPeriodListPick( ) {
		return new CHGroupPeriodPick;
	} // udPListHandlerPick GetHGroupPeriodListPick

	udPBase GetProjectGroupPeriodList( ) {
		udPBase ret = GetProjectGroupPeriod( );
		ret->get( "client_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGroupPeriodList

	CHGroupPeriod::CHGroupPeriod( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_pGroupForm( NULL )
	{
		udDELETE( m_pData );

		m_pData = new CDAListGroupPeriod;

		m_szName = "groupperiod";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать период" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHGroupPeriod::ColModelGroupPeriod( );

		m_pData->SetName( "groupperiod" );
		m_pData->SetGetObject( GetProjectGroupPeriodList );
	} // CHGroupPeriod

	CHGroupPeriod::CHGroupPeriod( const CHGroupPeriod& objCopy ) : udListHandler( ) {
		m_szName = "groupperiod";
	} // CHGroupPeriod

	CHGroupPeriod::~CHGroupPeriod( ) {
	} // ~CGroupPeriod

	CHGroupPeriod& CHGroupPeriod::operator =( const CHGroupPeriod& objCopy ) {
		return *this;
	} // CHGroupPeriod& operator =

	void CHGroupPeriod::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата начала" ), m_pColModel->m_col_date1 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата окончания" ), m_pColModel->m_col_date2 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Минимум человек" ), m_pColModel->m_col_min );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Максимум человек" ), m_pColModel->m_col_max );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Руководитель" ), m_pColModel->m_col_client );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHGroupPeriod::Load( ) {
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
			row[ m_pColModel->m_col_date1	] = Glib::locale_to_utf8( obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_date2	] = Glib::locale_to_utf8( obj->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_min		] = Glib::locale_to_utf8( obj->get( "min" )->toString( ) );
			row[ m_pColModel->m_col_max		] = Glib::locale_to_utf8( obj->get( "max" )->toString( ) );
			row[ m_pColModel->m_col_client	] = Glib::locale_to_utf8( obj->get( "client_" )->toString( ) );
		}
	} // void Load

	void CHGroupPeriod::SetGroupForm( udPHGroupForm pGroupForm ) {
		m_pGroupForm = pGroupForm;
		( ( CDAListGroupPeriod* ) m_pData )->SetGroupForm( pGroupForm );
	} // void SetGroupForm

	void CHGroupPeriod::FilterUiClear( ) {
	} // void FilterUiClear

	void CHGroupPeriod::FilterUiToObject( ) {
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupPeriodPick::CHGroupPeriodPick( ) : CHGroupPeriod( ), m_pGroupTourForm( NULL ) {
	} // CHGroupPeriodPick

	CHGroupPeriodPick::CHGroupPeriodPick( const CHGroupPeriodPick& objCopy ) : CHGroupPeriod( ), m_pGroupTourForm( NULL ) {
	} // CHGroupPeriodPick

	CHGroupPeriodPick::~CHGroupPeriodPick( ) {
	} // ~CHGroupPeriodPick

	CHGroupPeriodPick& CHGroupPeriodPick::operator =( const CHGroupPeriodPick& objCopy ) {
		return *this;
	} // CHGroupPeriodPick& operator =

	void CHGroupPeriodPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHGroupPeriod::Init( );
	} // void CHGroupPeriodPick

	void CHGroupPeriodPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHGroupPeriodFormModal handler = new udHGroupPeriodFormModal( m_pGroupForm );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание периода" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание периода" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHGroupPeriodPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				udPBase obj = itObj->second;

				if ( m_pGroupTourForm ) {
					udPBase pTour = ( ( udPFDEObject ) m_pGroupTourForm->GetData( )->GetExtraData( udHGroupTourForm::ctrlidTour ) )->GetObject( );

					if ( pTour && pTour->get( "id" )->toUInt( ) ) {
						datetime	dtTour1		= pTour->get( "date1" )->toTime( ),
									dtTour2		= pTour->get( "date2" )->toTime( ),
									dtPeriod1	= obj->get( "date1" )->toTime( ),
									dtPeriod2	= obj->get( "date2" )->toTime( );

						if ( !( ( ( dtPeriod1 <= dtTour1 ) && ( dtTour1 <= dtPeriod2 ) ) ||
							( ( dtPeriod2 <= dtTour2 ) && ( dtTour2 <= dtPeriod2 ) ) ) )
						{
							Gtk::MessageDialog dialog(
								*wndParent,
								Glib::locale_to_utf8( "Тур не входит в период" ),
								false,
								Gtk::MESSAGE_INFO,
								Gtk::BUTTONS_OK
							);
							dialog.set_secondary_text( Glib::locale_to_utf8(
								"Измените даты начала и окончания тура или периода"
							) );
							dialog.run( );
							return;
						}
					}
				}

				m_pPickedObject = GetProjectGroupPeriod( );
				m_pPickedObject->apply( obj, true, true );
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

	void CHGroupPeriodPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHGroupPeriodPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( i ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				Gtk::TreeModel::Row row = *i;
				Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
				udPHGroupPeriodFormModal handler = new udHGroupPeriodFormModal( m_pGroupForm );
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupPeriodForm::CHGroupPeriodForm( udPHGroupForm pGroupForm ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Период" ) ),
		m_lblDate1( Glib::locale_to_utf8( "Дата начала" ) ),
		m_lblDate2( Glib::locale_to_utf8( "Дата окончания" ) ),
		m_lblMin( Glib::locale_to_utf8( "Минимум человек" ) ),
		m_lblMax( Glib::locale_to_utf8( "Максимум человек" ) ),
		m_lblClient( Glib::locale_to_utf8( "Руководитель" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_pGroupForm( pGroupForm )
	{
		m_szName = "groupperiod_form";

		m_pData->SetName( "groupperiod" );
		m_pData->SetGetObject( GetProjectGroupPeriod );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDate1.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpMin.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClient );
		pFde->SetMark( ctrlidClient );
		m_pData->AddExtraData( pFde );
		m_inpClient.SetDataObject( pFde );
		m_inpClient.SetWindowTitle( Glib::locale_to_utf8( "Выбор клиента" ) );
		m_inpClient.SetAttr( "fio" );
		m_inpClient.SetGetListHandler( GetHClientListPick );
		m_inpClient.SetGetFormHandler( GetHClientForm );
		m_inpClient.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование клиента" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHGroupPeriodForm

	CHGroupPeriodForm::CHGroupPeriodForm( const CHGroupPeriodForm& objCopy ) : udFormHandler( ) {
	} // CHGroupPeriodForm

	CHGroupPeriodForm::~CHGroupPeriodForm( ) {
	} // ~CHGroupPeriodForm

	CHGroupPeriodForm& CHGroupPeriodForm::operator =( const CHGroupPeriodForm& objCopy ) {
		return *this;
	} // CHGroupPeriodForm& operator =

	void CHGroupPeriodForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Дата начала
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата окончания
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Минимум человек
		m_tblInput.attach( m_lblMin, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpMin, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Максимум человек
		m_tblInput.attach( m_lblMax, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpMax, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Руководитель
		m_tblInput.attach( m_lblClient, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpClient, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHGroupPeriodForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpMin.Reset( );
		m_inpMax.Reset( );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
		m_inpClient.SetTitle( "" );
	} // void Clear

	void CHGroupPeriodForm::Load( ) {
	} // void Load

	udInt CHGroupPeriodForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udUInt	iMin,
				iMax;

		iMin = toUInt( m_inpMin.GetValue( ).data( ) );
		if ( iMin <= 0 ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите минимальное значение" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpMin.GetEntry( ) ) );
			return 1;
		}

		iMax = toUInt( m_inpMax.GetValue( ).data( ) );
		if ( iMax <= 0 ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите максимальное значение" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpMax.GetEntry( ) ) );
			return 2;
		}

		if ( iMax < iMin ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Максимальное значение должно быть больше или равно минимальному" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpMax.GetEntry( ) ) );
			return 3;
		}

		return 0;
	} // udInt Validate

	void CHGroupPeriodForm::Send( ) {
		udUInt	iMin,
				iMax;

		iMin = toUInt( m_inpMin.GetValue( ).data( ) );
		iMax = toUInt( m_inpMax.GetValue( ).data( ) );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate2.GetValue( ) );
		obj->get( "min" )->ini( iMin );
		obj->get( "max" )->ini( iMax );
		obj->get( "client" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( )->get( "id" )->toUInt( )
		);

		if ( m_pGroupForm ) {
			m_pGroupForm->SavePeriod( this );
		}
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupPeriodFormModal::CHGroupPeriodFormModal( udPHGroupForm pGroupForm ) : CHGroupPeriodForm( pGroupForm ) {
	} // CHGroupPeriodFormModal

	CHGroupPeriodFormModal::CHGroupPeriodFormModal( const CHGroupPeriodFormModal& objCopy ) : CHGroupPeriodForm( ) {
	} // CHGroupPeriodFormModal

	CHGroupPeriodFormModal::~CHGroupPeriodFormModal( ) {
	} // ~CHGroupPeriodFormModal

	CHGroupPeriodFormModal& CHGroupPeriodFormModal::operator =( const CHGroupPeriodFormModal& objCopy ) {
		return *this;
	} // CHGroupPeriodFormModal& operator =

	void CHGroupPeriodFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHGroupPeriodFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHGroupPeriodForm

	void CHGroupPeriodFormModal::LoadFrom( udPBase from ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		// клиент
		udPBase	pClient	= ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( );
		udUInt	id			= pClient->get( "id" )->toUInt( ),
				client		= obj->get( "client" )->toUInt( );
		
		if ( ( id == 0 ) && ( client > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "client", client, GetProjectClient );
			if ( pLoaded ) {
				pClient->apply( pLoaded, true, true );
				udDELETE( pLoaded );
			}
		}

		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
		m_inpMin.SetValue( Glib::locale_to_utf8( obj->get( "min" )->toString( ) ) );
		m_inpMax.SetValue( Glib::locale_to_utf8( obj->get( "max" )->toString( ) ) );
		m_inpClient.SetTitle( Glib::locale_to_utf8( pClient->get( "fio" )->toString( ) ) );
	} // void LoadFrom

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPListHandlerPick GetHGroupPeriodLoadListPick( ) {
		return new CHGroupPeriodLoadPick;
	} // udPListHandlerPick GetHGroupPeriodLoadListPick

	udPBase GetProjectGroupPeriodLoadList( ) {
		udPBase ret = GetProjectGroupPeriod( );
		ret->get( "client_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGroupPeriodLoadList

	CHGroupPeriodLoad::CHGroupPeriodLoad( ) : udListHandler( ) {
		m_szName = "groupperiod";

		m_pRowModel = m_pColModel = new CHGroupPeriodLoad::ColModelGroupPeriodLoad( );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "group" )->ini( ( udUInt ) 0 );

		m_pData->SetName( "groupperiod" );
		m_pData->SetGetObject( GetProjectGroupPeriodLoadList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHGroupPeriodLoad

	CHGroupPeriodLoad::CHGroupPeriodLoad( const CHGroupPeriodLoad& objCopy ) : udListHandler( ) {
		m_szName = "groupperiod";
	} // CHGroupPeriodLoad

	CHGroupPeriodLoad::~CHGroupPeriodLoad( ) {
	} // ~CGroupPeriodLoad

	CHGroupPeriodLoad& CHGroupPeriodLoad::operator =( const CHGroupPeriodLoad& objCopy ) {
		return *this;
	} // CHGroupPeriodLoad& operator =

	void CHGroupPeriodLoad::Init( ) {
		// добавляем колонки
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата начала" ), m_pColModel->m_col_date1 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата окончания" ), m_pColModel->m_col_date2 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Минимум человек" ), m_pColModel->m_col_min );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Максимум человек" ), m_pColModel->m_col_max );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Руководитель" ), m_pColModel->m_col_client );

		// остальная инициализация
		udListHandler::Init( );
	} // void Init

	void CHGroupPeriodLoad::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_date1	] = Glib::locale_to_utf8( obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_date2	] = Glib::locale_to_utf8( obj->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_min		] = Glib::locale_to_utf8( obj->get( "min" )->toString( ) );
			row[ m_pColModel->m_col_max		] = Glib::locale_to_utf8( obj->get( "max" )->toString( ) );
			row[ m_pColModel->m_col_client	] = Glib::locale_to_utf8( obj->get( "client_" )->toString( ) );
			row[ m_pColModel->m_col_object	] = obj;
		}
	} // void Load

	void CHGroupPeriodLoad::SetGroupId( udUInt id ) {
		m_pData->GetFilter( )->get( "group" )->ini( id );
	} // void SetGroupId

	void CHGroupPeriodLoad::FilterUiClear( ) {
	} // void FilterUiClear

	void CHGroupPeriodLoad::FilterUiToObject( ) {
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupPeriodLoadPick::CHGroupPeriodLoadPick( ) : CHGroupPeriodLoad( ), m_pTourForm( NULL ) {
	} // CHGroupPeriodLoadPick

	CHGroupPeriodLoadPick::CHGroupPeriodLoadPick( const CHGroupPeriodLoadPick& objCopy ) : CHGroupPeriodLoad( ), m_pTourForm( NULL ) {
	} // CHGroupPeriodLoadPick

	CHGroupPeriodLoadPick::~CHGroupPeriodLoadPick( ) {
	} // ~CHGroupPeriodLoadPick

	CHGroupPeriodLoadPick& CHGroupPeriodLoadPick::operator =( const CHGroupPeriodLoadPick& objCopy ) {
		return *this;
	} // CHGroupPeriodLoadPick& operator =

	void CHGroupPeriodLoadPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHGroupPeriodLoad::Init( );
	} // void CHGroupPeriodLoadPick

	void CHGroupPeriodLoadPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			udPBase obj = row[ m_pColModel->m_col_object ];

			if ( m_pTourForm ) {
				udPBase pTour = m_pTourForm->GetData( )->GetData( );
				datetime	dtTour1		= pTour->get( "date1" )->toTime( ),
							dtTour2		= pTour->get( "date2" )->toTime( ),
							dtPeriod1	= obj->get( "date1" )->toTime( ),
							dtPeriod2	= obj->get( "date2" )->toTime( );

				if ( !( ( ( dtPeriod1 <= dtTour1 ) && ( dtTour1 <= dtPeriod2 ) ) ||
					( ( dtPeriod2 <= dtTour2 ) && ( dtTour2 <= dtPeriod2 ) ) ) )
				{
					Gtk::MessageDialog dialog(
						*wndParent,
						Glib::locale_to_utf8( "Тур не входит в период" ),
						false,
						Gtk::MESSAGE_INFO,
						Gtk::BUTTONS_OK
					);
					dialog.set_secondary_text( Glib::locale_to_utf8(
						"Измените даты начала и окончания тура или периода"
					) );
					dialog.run( );
					return;
				}
			}

			m_pPickedObject = new udProjectGroupPeriod;
			m_pPickedObject->apply( obj, true, true );

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

	void CHGroupPeriodLoadPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHGroupPeriodLoadPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CWgtGroupPeriodLoadPick::CWgtGroupPeriodLoadPick( ) : udWgtListPick( ), m_pGroupFrom( NULL ) {
	} // CWgtEmployerPick

	CWgtGroupPeriodLoadPick::CWgtGroupPeriodLoadPick( const CWgtGroupPeriodLoadPick& objCopy ) : udWgtListPick( ) {
	} // CWgtEmployerPick

	CWgtGroupPeriodLoadPick::~CWgtGroupPeriodLoadPick( ) {
	} // ~CWgtGroupPeriodLoadPick

	CWgtGroupPeriodLoadPick& CWgtGroupPeriodLoadPick::operator =( const CWgtGroupPeriodLoadPick& objCopy ) {
		return *this;
	} // CWgtEmployerPick& operator =

	void CWgtGroupPeriodLoadPick::OnButtonClick( ) {
		if ( !m_pGroupFrom ) {
			g_pApp->GetLog( )->Write( "CWgtGroupPeriodLoadPick::OnButtonClick, m_pGroupFrom is NULL\n" );
			return;
		}

		udPBase pGroup = ( ( udPFDEObject ) m_pGroupFrom->GetExtraData( udHReportGroupPay::fltGroup ) )->GetObject( );
		if ( !pGroup || !pGroup->get( "id" )->toUInt( ) ) {
			g_pApp->GetLog( )->Write( "CWgtGroupPeriodLoadPick::OnButtonClick, group is 0\n" );
			return;
		}

		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		udPHGroupPeriodLoadPick hPickPeriod = new udHGroupPeriodLoadPick;
		hPickPeriod->SetGroupId( pGroup->get( "id" )->toUInt( ) );
		hPickPeriod->BeforeCreate( );

		ModalWindow( wndParent, hPickPeriod->GetWindow( ), Glib::locale_to_utf8( "Выбор периода" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбор периода" ) );
		//wnd.add( *( hPickPeriod->GetWindowPick( ) ) );
		wnd.add( *( hPickPeriod->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = hPickPeriod->GetObject( );
		if ( obj ) {
			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );

			m_lblTitle.set_text( Glib::locale_to_utf8(
				obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) + " - " +
				obj->get( "date2" )->toTime( ).toString( "%d.%m.%Y" )
			) );
		}

		udDELETE( hPickPeriod );
	} // void OnButtonClick

} // namespace UdSDK
