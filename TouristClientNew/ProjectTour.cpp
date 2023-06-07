
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHTourListPick( ) {
		return new CHTourPick;
	} // udPListHandlerPick GetHTourListPick

	udPBase GetProjectTourList( ) {
		udPBase ret = GetProjectTour( );
		ret->get( "employer_" )->ini( wstring( L"" ) );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		ret->get( "status_" )->ini( wstring( L"" ) );
		ret->get( "manager_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourList

	CHTour::CHTour( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblFltDate1( Glib::locale_to_utf8( "С" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "По" ) ),
		m_lblFltStatus( Glib::locale_to_utf8( "Статус" ) ),
		m_lblFltEmployer( Glib::locale_to_utf8( "Заказчик" ) ),
		m_lblFltOrder( Glib::locale_to_utf8( "Заявка" ) ),
		m_lblFltTplTour( Glib::locale_to_utf8( "Шаблон тура" ) ),
		m_lblFltUser( Glib::locale_to_utf8( "Ответственный" ) ),
		m_lblFltClient( Glib::locale_to_utf8( "Турист" ) ),
		m_inpFltDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpFltDate2( datetime( 0, true ), datetime( 0, true ) ),
		m_inpFltStatus( udProjectTour::TOUR_STATUS_NONE, udProjectTour::TOUR_STATUS_NONE )
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_szName = "tour";

		m_pRowModel = m_pColModel = new CHTour::ColModelTour( );

		m_pData->SetName( "tour" );
		m_pData->SetGetObject( GetProjectTourList );

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать тур" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// объект с данными фильтра
		udDatetime date;
		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );

		date.load( );
		date.dec( 1, udDatetime::datetimeMonth );
		m_inpFltDate1.SetValue( date );
		m_inpFltDate1.SetDefaultValue( date );
		filter->get( "date1" )->ini( date );

		date.load( );
		date.inc( 1, udDatetime::datetimeMonth );
		m_inpFltDate2.SetValue( date );
		m_inpFltDate2.SetDefaultValue( date );
		filter->get( "date2" )->ini( date );
		
		filter->get( "status" )->ini( ( udInt ) 0 );
		filter->get( "employer" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		filter->get( "order" )->ini( ( udUInt ) 0 );
		filter->get( "tpltour" )->ini( ( udUInt ) 0 );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// контролы фильтра
		m_inpFltStatus.AddRow( udProjectTour::TOUR_STATUS_NONE, Glib::locale_to_utf8( "-- нет --" ) );
		m_inpFltStatus.AddRow( udProjectTour::TOUR_STATUS_OPENED, Glib::locale_to_utf8( "Открыт" ) );
		m_inpFltStatus.AddRow( udProjectTour::TOUR_STATUS_CLOSED, Glib::locale_to_utf8( "Закрыт" ) );
		m_inpFltStatus.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDirect );
		pFde->SetMark( fltDirect );
		m_pData->AddExtraData( pFde );
		m_inpFltDirect.SetDataObject( pFde );
		m_inpFltDirect.SetWindowTitle( Glib::locale_to_utf8( "Выбор направления" ) );
		m_inpFltDirect.SetAttr( "name" );
		m_inpFltDirect.SetGetListHandler( GetHDirectListPick );
		m_inpFltDirect.SetGetFormHandler( GetHDirectForm );
		m_inpFltDirect.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование направления" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( fltUser );
		m_pData->AddExtraData( pFde );
		m_inpFltUser.SetDataObject( pFde );
		m_inpFltUser.SetWindowTitle( Glib::locale_to_utf8( "Выбор пользователя" ) );
		m_inpFltUser.SetAttr( "fio" );
		m_inpFltUser.SetGetListHandler( GetHUserListPick );
		m_inpFltUser.SetGetFormHandler( GetHUserForm );
		m_inpFltUser.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пользователя" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectOrder );
		pFde->SetMark( fltOrder );
		m_pData->AddExtraData( pFde );
		m_inpFltOrder.SetDataObject( pFde );
		m_inpFltOrder.SetWindowTitle( Glib::locale_to_utf8( "Выбор заявки" ) );
		m_inpFltOrder.SetAttr( "employer_" );
		m_inpFltOrder.SetGetListHandler( GetHOrderListPick );
		m_inpFltOrder.SetGetFormHandler( GetHOrderForm );
		m_inpFltOrder.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование заявки" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTplTour );
		pFde->SetMark( fltTplTour );
		m_pData->AddExtraData( pFde );
		m_inpFltTplTour.SetDataObject( pFde );
		m_inpFltTplTour.SetWindowTitle( Glib::locale_to_utf8( "Выбор шаблона тура" ) );
		m_inpFltTplTour.SetAttr( "name" );
		m_inpFltTplTour.SetGetListHandler( GetHTplTourListPick );
		m_inpFltTplTour.SetGetFormHandler( GetHTplTourForm );
		m_inpFltTplTour.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование шаблона тура" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEEmployer pFdeEmployer = new udFDEEmployer;
		pFdeEmployer->SetMark( fltEmployer );
		m_pData->AddExtraData( pFdeEmployer );
		m_inpFltEmployer = new udWgtEmployerPick;
		m_inpFltEmployer->SetDataObject( pFdeEmployer );
		m_inpFltEmployer->SetWindowTitle( Glib::locale_to_utf8( "Выбор заказчика" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClient );
		pFde->SetMark( fltClient );
		m_pData->AddExtraData( pFde );
		m_inpFltClient.SetDataObject( pFde );
		m_inpFltClient.SetWindowTitle( Glib::locale_to_utf8( "Выбор клиента" ) );
		m_inpFltClient.SetAttr( "fio" );
		m_inpFltClient.SetGetListHandler( GetHClientListPick );
		m_inpFltClient.SetGetFormHandler( GetHClientForm );
		m_inpFltClient.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование клиента" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Статус
		m_tblFilter.attach( m_lblFltStatus, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltStatus, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Дата 1
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата 2
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Направление
		m_tblFilter.attach( m_lblFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Заказчик
		m_tblFilter.attach( m_lblFltEmployer, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( *m_inpFltEmployer, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Менеджер
		m_tblFilter.attach( m_lblFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Заявка
		m_tblFilter.attach( m_lblFltOrder, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltOrder, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Шаблон тура
		m_tblFilter.attach( m_lblFltTplTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltTplTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Турист
		m_tblFilter.attach( m_lblFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTour

	CHTour::CHTour( const CHTour& objCopy ) : udListHandler( ) {
		m_szName = "tour";
	} // CHTour

	CHTour::~CHTour( ) {
		udDELETE( m_inpFltEmployer );
	} // ~CTour

	CHTour& CHTour::operator =( const CHTour& objCopy ) {
		return *this;
	} // CHTour& operator =

	void CHTour::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Создан" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Направление" ), m_pColModel->m_col_direct );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Отправление" ), m_pColModel->m_col_date1 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Статус" ), m_pColModel->m_col_status );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Заказчик" ), m_pColModel->m_col_employer );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Ответственный" ), m_pColModel->m_col_manager );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );

		//col = m_objTreeView.get_column( 0 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickId ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 1 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickCreated ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 2 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickName ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 3 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickDirect ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 4 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickDate ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 5 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickStatus ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 6 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickEmployer ) );
		col->set_clickable( true );

		col = m_objTreeView.get_column( 7 );
		col->signal_clicked( ).connect( sigc::mem_fun( this, &CHTour::OnColClickManager ) );
		col->set_clickable( true );
	} // void Init

	void CHTour::Load( ) {
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
			//row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_created ] = Glib::locale_to_utf8( obj->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_direct ] = Glib::locale_to_utf8( obj->get( "direct_" )->toString( ) );
			row[ m_pColModel->m_col_date1 ] = Glib::locale_to_utf8( obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_status ] = Glib::locale_to_utf8( obj->get( "status_" )->toString( ) );
			row[ m_pColModel->m_col_employer ] = Glib::locale_to_utf8( obj->get( "employer_" )->toString( ) );
			row[ m_pColModel->m_col_manager ] = Glib::locale_to_utf8( obj->get( "manager_" )->toString( ) );
		}
	} // void Load

	void CHTour::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
		//m_inpFltStatus.set_active( 0 );
		m_inpFltStatus.Reset( );
		m_inpFltEmployer->SetTitle( "" );
		m_inpFltDirect.SetTitle( "" );
		m_inpFltUser.SetTitle( "" );
		m_inpFltOrder.SetTitle( "" );
		m_inpFltTplTour.SetTitle( "" );
		m_inpFltClient.SetTitle( "" );

		udPMorph filter = m_pData->GetFilter( );
		udDatetime date;

		date.load( );
		date.dec( 1, udDatetime::datetimeMonth );
		filter->get( "date1" )->ini( date );

		date.load( );
		date.inc( 1, udDatetime::datetimeMonth );
		filter->get( "date2" )->ini( date );
	} // void FilterUiClear

	void CHTour::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udInt status = m_inpFltStatus.LoadValue( ).GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
		filter->get( "status" )->ini( status );
		filter->get( "emptype" )->ini( m_inpFltEmployer->GetType( ) );
		filter->get( "employer" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltEmployer ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "order" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltOrder ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "tpltour" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltTplTour ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "manager" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltUser ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "client" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltClient ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	void CHTour::OnColClickId( ) {
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

	void CHTour::OnColClickCreated( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "created" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "created" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		
		Load( );
	} // void OnColClickCreated

	void CHTour::OnColClickName( ) {
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

	void CHTour::OnColClickDirect( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "direct" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "direct" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		
		Load( );
	} // void OnColClickDirect

	void CHTour::OnColClickDate( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "date1" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "date1" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		
		Load( );
	} // void OnColClickDate

	void CHTour::OnColClickStatus( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "status" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "status" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		
		Load( );
	} // void OnColClickStatus

	void CHTour::OnColClickEmployer( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "employer" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "employer" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		
		Load( );
	} // void OnColClickEmployer

	void CHTour::OnColClickManager( ) {
		Clear( );

		string szOrderBy = m_pData->GetOrderBy( );
		if ( szOrderBy == "manager2" ) {
			udInt iOrderType = m_pData->GetOrderType( );

			if ( iOrderType == udDAList::ordDesc ) {
				m_pData->SetOrderType( udDAList::ordAsc );
			} else {
				m_pData->SetOrderType( udDAList::ordDesc );
			}
		} else {
			m_pData->SetOrderBy( "manager2" );
			m_pData->SetOrderType( udDAList::ordDesc );
		}

		
		Load( );
	} // void OnColClickManager

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourModeless::CHTourModeless( ) :
		udListHandlerModeless( ),
		CHTour( ),
		m_btnPayOrder( Glib::locale_to_utf8( "Указания на оплату" ) )
	{
		m_btnPayOrder.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"tourpayorder"
		) );
	} // CHTourModeless

	CHTourModeless::CHTourModeless( const CHTourModeless& objCopy ) : CHTour( ) {
	} // CHTourModeless

	CHTourModeless::~CHTourModeless( ) {
	} // ~CHTourModeless

	CHTourModeless& CHTourModeless::operator =( const CHTourModeless& objCopy ) {
		return *this;
	} // CHTourModeless& operator =

	void CHTourModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		m_hbbTop.pack_start( m_btnPayOrder, false, false, 0 );
		CHTour::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHTourModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "tour_form" );
	} // void OnButtonForm

	void CHTourModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "tour_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "tour_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourPick::CHTourPick( ) : CHTour( ) {
	} // CHTourPick

	CHTourPick::CHTourPick( const CHTourPick& objCopy ) : CHTour( ) {
	} // CHTourPick

	CHTourPick::~CHTourPick( ) {
	} // ~CHTourPick

	CHTourPick& CHTourPick::operator =( const CHTourPick& objCopy ) {
		return *this;
	} // CHTourPick& operator =

	void CHTourPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTour::Init( );
	} // void CHTourPick

	void CHTourPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::ScrolledWindow wndScroll;
		udPHTourFormModal handler = new udHTourFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание тура" ) );

		/*Gtk::Window wnd;
		wnd.set_size_request( 800, 600 );
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание тура" ) );
		//wnd.add( *( handler->GetWindow( ) ) );
		wnd.add( wndScroll );
		wndScroll.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		handler->GetWindow( )->show( );
		wndScroll.show( );
		wnd.show( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHTourPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectTour( );
				m_pPickedObject->apply( itObj->second, true, true );
			}
			/*Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectTour( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );*/
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите тур" ),
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

	void CHTourPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTourPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			//Gtk::ScrolledWindow wndScroll;
			udPHTourFormModal handler = new udHTourFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование тура" ) );

			/*Gtk::Window wnd;
			wnd.set_size_request( 800, 600 );
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование тура" ) );
			//wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			//wnd.show_all( );
			wnd.add( wndScroll );
			wndScroll.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			handler->GetWindow( )->show( );
			wndScroll.show( );
			wnd.show( );
			Gtk::Main::run( wnd );*/
			udDELETE( handler );

			Clear( );
			Load( );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPFormHandler GetHTourForm( ) {
		return new CHTourFormModal;
	} // udPFormHandler GetHTourForm

	CHTourForm::CHTourForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Тур" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblDate1( Glib::locale_to_utf8( "Дата отъезда" ) ),
		m_lblDate2( Glib::locale_to_utf8( "Дата приезда" ) ),
		m_lblCompany( Glib::locale_to_utf8( "Компания-продавец" ) ),
		m_lblStatus( Glib::locale_to_utf8( "Статус" ) ),
		m_lblEmployer( Glib::locale_to_utf8( "Заказчик" ) ),
		m_lblUser( Glib::locale_to_utf8( "Ответственный" ) ),
		m_lblDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_lblState( Glib::locale_to_utf8( "Состояние" ) ),
		m_lblClient( Glib::locale_to_utf8( "Туристы" ) ),
		m_lblApp( Glib::locale_to_utf8( "Приложения" ) ),
		m_lblRoute( Glib::locale_to_utf8( "Маршрут" ) ),
		m_lblTour( Glib::locale_to_utf8( "Связанные услуги" ) ),
		m_lblService( Glib::locale_to_utf8( "Услуги" ) ),
		m_lblDocument( Glib::locale_to_utf8( "Документ заказчика" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) ),
		m_wndApp( NULL ),
		m_btnDocPrint( Glib::locale_to_utf8( "Распечатать документ" ) ),
		m_wndFinance( NULL ),
		m_iFinancePage( 0 ),
		m_inpStatus( udProjectTour::TOUR_STATUS_OPENED, udProjectTour::TOUR_STATUS_OPENED ),
		m_btnAddToGroup( Glib::locale_to_utf8( "Добавить в группу" ) ),
		m_btnMakeCopy( Glib::locale_to_utf8( "Создать копию" ) )
	{
		m_szName = "tour_form";

		m_pData->SetName( "tour" );
		m_pData->SetGetObject( GetProjectTour );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_lblName.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDate1.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDate2.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblCompany.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblStatus.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblEmployer.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblUser.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDirect.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblComment.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblState.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblClient.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblApp.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblRoute.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblTour.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblService.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );

		m_inpComment.set_size_request( 600, -1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCompany );
		pFde->SetMark( ctrlidCompany );
		m_pData->AddExtraData( pFde );
		m_inpCompany.SetDataObject( pFde );
		m_inpCompany.SetWindowTitle( Glib::locale_to_utf8( "Выбор компании" ) );
		m_inpCompany.SetAttr( "name" );
		m_inpCompany.SetGetListHandler( GetHCompanyListPick );
		m_inpCompany.SetGetFormHandler( GetHCompanyForm );
		m_inpCompany.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование компании" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEEmployer pFdeEmployer = new udFDEEmployer;
		pFdeEmployer->SetMark( ctrlidEmployer );
		m_pData->AddExtraData( pFdeEmployer );
		m_inpEmployer = new udWgtEmployerPick;
		m_inpEmployer->SetDataObject( pFdeEmployer );
		m_inpEmployer->SetWindowTitle( Glib::locale_to_utf8( "Выбор заказчика" ) );
		m_inpEmployer->SetHandler( this );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClDoc );
		pFde->SetMark( ctrlidDocument );
		m_pData->AddExtraData( pFde );
		m_inpDocument.SetDataObject( pFde );
		m_inpDocument.SetWindowTitle( Glib::locale_to_utf8( "Выбор документа заказчика" ) );
		//m_inpDocument.SetAttr( "client_" );
		m_inpDocument.SetGetListHandler( GetHEmployerDocListPick );
		m_inpDocument.SetHandler( this );
		m_inpDocument.SetGetFormHandler( GetHClDocForm );
		m_inpDocument.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование документа клиента" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( ctrlidUser );
		m_pData->AddExtraData( pFde );
		m_inpUser.SetDataObject( pFde );
		m_inpUser.SetWindowTitle( Glib::locale_to_utf8( "Выбор пользователя" ) );
		m_inpUser.SetAttr( "fio" );
		m_inpUser.SetGetListHandler( GetHUserListPick );
		m_inpUser.SetGetFormHandler( GetHUserForm );
		m_inpUser.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пользователя" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDirect );
		pFde->SetMark( ctrlidDirect );
		m_pData->AddExtraData( pFde );
		m_inpDirect.SetDataObject( pFde );
		m_inpDirect.SetWindowTitle( Glib::locale_to_utf8( "Выбор направления" ) );
		m_inpDirect.SetAttr( "name" );
		m_inpDirect.SetGetListHandler( GetHDirectListPick );
		m_inpDirect.SetGetFormHandler( GetHDirectForm );
		m_inpDirect.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование направления" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectSTour );
		pFde->SetMark( ctrlidState );
		m_pData->AddExtraData( pFde );
		m_inpState.SetDataObject( pFde );
		m_inpState.SetWindowTitle( Glib::locale_to_utf8( "Выбор состояния тура" ) );
		m_inpState.SetAttr( "name" );
		m_inpState.SetGetListHandler( GetHSTourListPick );
		m_inpState.SetGetFormHandler( GetHSTourForm );
		m_inpState.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование состояния тура" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpStatus.AddRow( udProjectTour::TOUR_STATUS_OPENED, Glib::locale_to_utf8( "Открыт" ) );
		m_inpStatus.AddRow( udProjectTour::TOUR_STATUS_CLOSED, Glib::locale_to_utf8( "Закрыт" ) );
		m_inpStatus.AddRow( udProjectTour::TOUR_STATUS_REMIND, Glib::locale_to_utf8( "Требует внимания" ) );
		m_inpStatus.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Gtk::TreeView *pTree = NULL;
		udPFDEList pFdeList = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// маршрут
		m_inpRoute.set_size_request( 400, 200 );
		m_pModelFormListRoute = new ColModelFormListRoute;
		m_objRouteModel = Gtk::ListStore::create( *m_pModelFormListRoute );
		
		pTree = m_inpRoute.GetTreeView( );
		pTree->set_model( m_objRouteModel );
		pTree->append_column( Glib::locale_to_utf8( "Город" ), m_pModelFormListRoute->m_col_name );
		m_inpRoute.SetColumnDefaults( );

		m_btnRouteReplace = m_inpRoute.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnRouteReplace->set_sensitive( false );
		m_btnRouteReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListRoute_Replace ) );

		m_btnRouteDel = m_inpRoute.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnRouteDel->set_sensitive( false );
		m_btnRouteDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListRoute_Del ) );

		m_btnRouteAdd = m_inpRoute.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnRouteAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListRoute_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListRoute_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourRouteList );
		pFdeList->SetCallbackSend( GetProjectTourRoute );
		pFdeList->SetMark( ctrlidRoute );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// услуги
		m_inpService.set_size_request( 400, 200 );
		m_pModelFormListService = new ColModelFormListService;
		m_objServiceModel = Gtk::ListStore::create( *m_pModelFormListService );
		
		pTree = m_inpService.GetTreeView( );
		pTree->set_model( m_objServiceModel );
		pTree->append_column( Glib::locale_to_utf8( "Услуга" ), m_pModelFormListService->m_col_service );
		pTree->append_column( Glib::locale_to_utf8( "Количество" ), m_pModelFormListService->m_col_num );
		pTree->append_column( Glib::locale_to_utf8( "С" ), m_pModelFormListService->m_col_date1 );
		pTree->append_column( Glib::locale_to_utf8( "По" ), m_pModelFormListService->m_col_date2 );
		pTree->append_column( Glib::locale_to_utf8( "Готовность" ), m_pModelFormListService->m_col_ready );
		m_inpService.SetColumnDefaults( );

		m_btnServiceEdit = m_inpService.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnServiceEdit->set_sensitive( false );
		m_btnServiceEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListService_Edit ) );

		m_btnServiceDel = m_inpService.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnServiceDel->set_sensitive( false );
		m_btnServiceDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListService_Del ) );

		m_btnServiceAdd = m_inpService.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnServiceAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListService_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListService_Select ) );
		pTree->signal_row_activated( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListService_Activate ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourServiceList );
		pFdeList->SetCallbackSend( GetProjectTourService );
		pFdeList->SetMark( ctrlidService );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// туристы
		m_inpClient.set_size_request( 400, 200 );
		m_pModelFormListClient = new ColModelFormListClient;
		m_objClientModel = Gtk::ListStore::create( *m_pModelFormListClient );
		
		pTree = m_inpClient.GetTreeView( );
		pTree->set_model( m_objClientModel );
		pTree->append_column( Glib::locale_to_utf8( "ФИО" ), m_pModelFormListClient->m_col_fio );
		m_inpClient.SetColumnDefaults( );

		m_btnClientReplace = m_inpClient.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnClientReplace->set_sensitive( false );
		m_btnClientReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListClient_Replace ) );

		m_btnClientDel = m_inpClient.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnClientDel->set_sensitive( false );
		m_btnClientDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListClient_Del ) );

		m_btnClientAdd = m_inpClient.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnClientAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListClient_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListClient_Select ) );
		pTree->signal_row_activated( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListClient_Activate ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourClientList );
		pFdeList->SetCallbackSend( GetProjectTourClient );
		pFdeList->SetMark( ctrlidClient );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// связанные туры
		m_inpTour.set_size_request( 400, 200 );
		m_pModelFormListTour = new ColModelFormListTour;
		m_objTourModel = Gtk::ListStore::create( *m_pModelFormListTour );
		
		pTree = m_inpTour.GetTreeView( );
		pTree->set_model( m_objTourModel );
		pTree->append_column( Glib::locale_to_utf8( "Наименование" ), m_pModelFormListTour->m_col_name );
		m_inpTour.SetColumnDefaults( );

		m_btnTourReplace = m_inpTour.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnTourReplace->set_sensitive( false );
		m_btnTourReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListTour_Replace ) );

		m_btnTourDel = m_inpTour.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnTourDel->set_sensitive( false );
		m_btnTourDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListTour_Del ) );

		m_btnTourAdd = m_inpTour.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnTourAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListTour_Add ) );

		m_btnTourFromCopy = m_inpTour.AddButton( Glib::locale_to_utf8( "Добавить копию" ) );
		m_btnTourFromCopy->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListTour_FromCopy ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListTour_Select ) );
		pTree->signal_row_activated( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListTour_Activate ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourTourList );
		pFdeList->SetCallbackSend( GetProjectTourTour );
		pFdeList->SetMark( ctrlidTour );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// приложения
		m_inpApp.set_size_request( 400, 200 );
		m_pModelFormListApp = new ColModelFormListApp;
		m_objAppModel = Gtk::ListStore::create( *m_pModelFormListApp );

		pTree = m_inpApp.GetTreeView( );
		pTree->set_model( m_objAppModel );
		pTree->append_column( Glib::locale_to_utf8( "Имя файла" ), m_pModelFormListApp->m_col_name );
		m_inpApp.SetColumnDefaults( );

		m_btnAppEdit = m_inpApp.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnAppEdit->set_sensitive( false );
		m_btnAppEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListApp_Edit ) );

		m_btnAppDel = m_inpApp.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnAppDel->set_sensitive( false );
		m_btnAppDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListApp_Del ) );

		m_btnAppReplace = m_inpApp.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnAppReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListApp_Replace ) );
		m_btnAppReplace->set_sensitive( false );

		m_btnAppAdd = m_inpApp.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnAppAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListApp_Add ) );

		m_btnFolder = m_inpApp.AddButton( Glib::locale_to_utf8( "Открыть папку" ) );
		m_btnFolder->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListApp_Folder ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourForm::OnFormListApp_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectAttach );
		pFdeList->SetMark( ctrlidApp );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CFDETourApp* pFdeApp1 = new CFDETourApp;
		m_wndApp = new udHTourAppForm( pFdeApp1 );
		pFdeApp1->SetCallback( GetProjectGroupContractApp1 );
		pFdeApp1->SetMark( ctrlidApp1 );
		m_pData->AddExtraData( pFdeApp1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_btnDocPrint.signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnButtonDocPrint ) );
		m_btnAddToGroup.signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnButtonAddToGroup ) );
		m_btnMakeCopy.signal_clicked( ).connect( sigc::mem_fun( this, &CHTourForm::OnButtonMakeCopy ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_wndFinance = new udHTourFinanceForm;
		m_wndFinance->SetTourForm( this );
		udPFDETourFinance pFdeFinance = new udFDETourFinance;
		pFdeFinance->SetTourFinanceForm( m_wndFinance );
		pFdeFinance->SetMark( ctrlidFinance );
		m_pData->AddExtraData( pFdeFinance );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTourForm

	CHTourForm::CHTourForm( const CHTourForm& objCopy ) : udFormHandler( ) {
	} // CHTourForm

	CHTourForm::~CHTourForm( ) {
		udDELETE( m_wndApp );
		udDELETE( m_wndFinance );
	} // ~CHTourForm

	CHTourForm& CHTourForm::operator =( const CHTourForm& objCopy ) {
		return *this;
	} // CHTourForm& operator =

	void CHTourForm::Init( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// приложение
		m_szFolderApp = g_pApp->GetUniqueTmpFolder( );
		m_pData->SetHasFiles( true );
		m_pData->SetFolder( m_szFolderApp );
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		udInt l = 0, r = 1;

		// заголовок
		//m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Дата отъезда
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Дата приезда
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Ответственный
		m_tblInput.attach( m_lblUser, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpUser, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Заказчик
		m_tblInput.attach( m_lblEmployer, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( *m_inpEmployer, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Документ заказчика
		m_tblInput.attach( m_lblDocument, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDocument, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Компания-продавец
		m_tblInput.attach( m_lblCompany, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCompany, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Направление
		m_tblInput.attach( m_lblDirect, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDirect, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Состояние
		m_tblInput.attach( m_lblState, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpState, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Статус
		m_tblInput.attach( m_lblStatus, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpStatus, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Маршрут
		m_tblInput.attach( m_lblRoute, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpRoute, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Услуги
		m_tblInput.attach( m_lblService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpService, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Туристы
		m_tblInput.attach( m_lblClient, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpClient, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Приложения
		m_tblInput.attach( m_lblApp, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpApp, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Связанные туры
		m_tblInput.attach( m_lblTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpTour, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_boxButton.pack_start( m_btnMakeCopy, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnAddToGroup, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnDocPrint, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );

		m_tblInput.show_all( );

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_wndApp->Init( );
		m_wndFinance->Init( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_wndTab.append_page( m_tblInput, Glib::locale_to_utf8( "Тур" ) );
		m_wndTab.append_page( *( m_wndApp->GetWindow( ) ), Glib::locale_to_utf8( "Приложение" ) );
		m_iFinancePage = m_wndTab.append_page( *( m_wndFinance->GetWindow( ) ), Glib::locale_to_utf8( "Финансы" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		m_wndFormWrap->pack_start( m_wndTab, Gtk::PACK_SHRINK );
		m_wndFormWrap->pack_start( m_boxButton, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );

		m_btnDocPrint.hide( );
		m_wndTab.get_nth_page( m_iFinancePage )->hide( );
	} // void Init

	void CHTourForm::Clear( ) {
		udFormHandler::Clear( );
		m_wndApp->Clear( );
		m_wndFinance->Clear( );

		m_inpName.Reset( );
		m_inpComment.Reset( );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
		m_inpEmployer->SetTitle( "" );
		m_inpDocument.SetTitle( "" );
		m_inpCompany.SetTitle( "" );
		m_inpDirect.SetTitle( "" );
		m_inpUser.SetTitle( "" );
		m_inpState.SetTitle( "" );
		//m_inpStatus.set_active( 0 );
		m_inpStatus.Reset( );
		m_objRouteModel->clear( );
		m_objClientModel->clear( );
		m_objAppModel->clear( );
		m_objTourModel->clear( );
		m_objServiceModel->clear( );
		m_btnDocPrint.hide( );
		m_wndTab.get_nth_page( m_iFinancePage )->hide( );
		m_wndTab.set_current_page( 0 );
	} // void Clear

	void CHTourForm::Load( ) {
		m_pData->MakeObject( );
		( ( udPFDEEmployer ) m_pData->GetExtraData( ctrlidEmployer ) )->SetMainObject( m_pData->GetData( ) );

		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );

		string szAttr = "fio";
		if ( obj->get( "emptype" )->toUInt( ) == emptypeUr ) {
			szAttr = "name";
		}
		m_inpEmployer->SetType( obj->get( "emptype" )->toUInt( ) );
		m_inpEmployer->SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( )->get( szAttr )->toString( )
		) );

		udPBase doc = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument ) )->GetObject( );
		if ( doc ) {
			m_inpDocument.SetTitle( Glib::locale_to_utf8(
				doc->get( "series" )->toString( ) + " " + doc->get( "number" )->toString( )
			) );
		}

		m_inpCompany.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCompany ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpDirect.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpUser.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidUser ) )->GetObject( )->get( "fio" )->toString( )
		) );
		m_inpState.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( )->get( "name" )->toString( )
		) );
		if ( obj->get( "status" )->toInt( ) > 0 ) {
			//m_inpStatus.set_active( obj->get( "status" )->toInt( ) - 1 );
			m_inpStatus.SetValue( obj->get( "status" )->toUInt( ) - 1 );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objRouteModel->append( ) );
				row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( add->get( "city_" )->toString( ) );
				row[ m_pModelFormListRoute->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objServiceModel->append( ) );
				row[ m_pModelFormListService->m_col_service	] = Glib::locale_to_utf8( add->get( "service_" )->toString( ) );
				row[ m_pModelFormListService->m_col_num		] = Glib::locale_to_utf8( add->get( "num" )->toString( ) );
				row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );

				if ( add->get( "ready" )->toUInt( ) ) {
					row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Готова" );
				} else {
					row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Не готова" );
				}

				row[ m_pModelFormListService->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objClientModel->append( ) );
				row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( add->get( "fio" )->toString( ) );
				row[ m_pModelFormListClient->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objTourModel->append( ) );
				row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( add->get( "tour2_" )->toString( ) );
				row[ m_pModelFormListTour->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objAppModel->append( ) );
				row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
				row[ m_pModelFormListApp->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_wndApp->Load( );
		m_wndFinance->Load( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if ( obj->get( "id" )->toUInt( ) ) {
			string log = "CHTourForm, ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			m_btnDocPrint.show( );
			m_wndTab.get_nth_page( m_iFinancePage )->show( );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHTourForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите заказчика" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEmployer->GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCompany ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите компанию" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCompany.GetButton( ) ) );
			return 3;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите направление" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDirect.GetButton( ) ) );
			return 4;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidUser ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ответственного" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpUser.GetButton( ) ) );
			return 5;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите состояние" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpState.GetButton( ) ) );
			return 6;
		}

		return 0;
	} // udInt Validate

	void CHTourForm::Send( ) {
		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Glib::ustring	szName,
						szComment;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		szName = m_inpName.GetValue( );
		szComment = m_inpComment.GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udUInt status = m_inpStatus.LoadValue( ).GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );
		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate2.GetValue( ) );
		obj->get( "emptype" )->ini( m_inpEmployer->GetType( ) );
		obj->get( "employer" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "document" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "company" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCompany ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "manager2" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidUser ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "state" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "status" )->ini( status );

		obj->get( "created" )->ini( datetime( 0, true ) );
		obj->get( "manager" )->ini( g_pApp->GetUser( )->get( "id" )->toUInt( ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_wndApp->Send( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pData->Send( ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( ) );
	} // void Send

	bool CHTourForm::BeforeCreatePickHandler( udPWgtEmployerDocPick wgt, udPHEmployerDocPick handler ) {
		assert( wgt );
		assert( handler );

		m_pData->MakeObject( );

		udPBase employer = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( );
		if ( employer && ( employer->get( "id" )->toUInt( ) > 0 ) ) {
			handler->SetEmployerId( employer->get( "id" )->toUInt( ) );
			return true;
		} else {
			mb::err( *( g_pApp->GetWndWork( ) ), Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Сначала выберите заказчика" ) );
		}
		return false;
	} // bool BeforeCreatePickHandler

	udPHClDocFormModal CHTourForm::GetEditHandler( udPWgtEmployerDocPick wgt ) {
		assert( wgt );

		m_pData->MakeObject( );

		udPBase employer = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( );
		if ( employer && ( employer->get( "id" )->toUInt( ) > 0 ) ) {
			return new CHClDocFormModal( employer->get( "id" )->toUInt( ), false, true );
		}
		return NULL;
	} // udPHClDocFormModal GetEditHandler

	void CHTourForm::AfterPick( udPWgtListPick wgt ) {
		if ( wgt != m_inpEmployer ) {
			return;
		}

		m_inpDocument.SetTitle( "" );

		udPFDEObject pFde = ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument );
		pFde->Clear( );
		pFde->Make( );
	} // void AfterPick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourForm::OnFormListRoute_Select( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnRouteReplace->set_sensitive( true );
			m_btnRouteDel->set_sensitive( true );
		} else {
			m_btnRouteReplace->set_sensitive( false );
			m_btnRouteDel->set_sensitive( false );
		}
	} // void OnFormListRoute_Select

	void CHTourForm::OnFormListRoute_Add( ) {
		m_pData->MakeObject( );

		udPListHandlerPick handler = GetHCityListPick( );
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать город" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбрать город" ) );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj ) {
			udPBase add = new udProjectTourRoute;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute );
			udPArrBase arrObject = pFde->GetArray( );

			add->get( "city" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "city_" )->ini( obj->get( "name" )->toWString( ) );
			add->get( "tour" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objRouteModel->append( ) );

			row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( add->get( "city_" )->toString( ) );
			row[ m_pModelFormListRoute->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListRoute_Add

	void CHTourForm::OnFormListRoute_Del( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListRoute->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objRouteModel->erase( i );
		}
	} // void OnFormListRoute_Del

	void CHTourForm::OnFormListRoute_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListRoute->m_col_object ];
			udPListHandlerPick handler = GetHCityListPick( );
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			handler->BeforeCreate( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать город" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Выбрать город" ) );
			//wnd.add( *( handler->GetWindowPick( ) ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPBase obj = handler->GetObject( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				replace->get( "city" )->ini( obj->get( "id" )->toUInt( ) );
				replace->get( "city_" )->ini( obj->get( "name" )->toWString( ) );

				row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( replace->get( "city_" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListRoute_Replace

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourForm::OnFormListClient_Select( ) {
		Gtk::TreeModel::iterator i = m_inpClient.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnClientReplace->set_sensitive( true );
			m_btnClientDel->set_sensitive( true );
		} else {
			m_btnClientReplace->set_sensitive( false );
			m_btnClientDel->set_sensitive( false );
		}
	} // void OnFormListClient_Select

	void CHTourForm::OnFormListClient_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objClientModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			Gtk::TreeModel::Row row = *i;
			udPBase obj = row[ m_pModelFormListClient->m_col_object ];

			if ( obj ) {
				string log = "\n";
				obj->dump( log );
				g_pApp->GetLog( )->Write( log );

				udPHClientFormModal handler = new udHClientFormModal;
				handler->Init( );
				handler->GetData( )->SetId( obj->get( "client" )->toUInt( ) );
				handler->Load( );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование клиента" ) );

				/*Gtk::Window wnd;
				wnd.set_modal( true );
				wnd.set_title( Glib::locale_to_utf8( "Редактирование клиента" ) );
				wnd.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				wnd.show_all( );
				Gtk::Main::run( wnd );*/

				udPBase client = handler->GetData( )->GetData( );
				if ( client ) {
					obj->get( "fio" )->ini( client->get( "fio" )->toWString( ) );

					row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( obj->get( "fio" )->toString( ) );
				}

				udDELETE( handler );
			}
		}
	} // void OnFormListClient_Activate

	void CHTourForm::OnFormListClient_Add( ) {
		m_pData->MakeObject( );

		udPListHandlerPick handler = GetHClientListPick( );
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать клиента" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбрать клиента" ) );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj ) {
			udPBase add = new udProjectTourClient;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidClient );
			udPArrBase arrObject = pFde->GetArray( );

			add->get( "client" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "fio" )->ini( obj->get( "fio" )->toWString( ) );
			add->get( "tour" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objClientModel->append( ) );

			row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( add->get( "fio" )->toString( ) );
			row[ m_pModelFormListClient->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListClient_Add

	void CHTourForm::OnFormListClient_Del( ) {
		Gtk::TreeModel::iterator i = m_inpClient.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListClient->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objClientModel->erase( i );
		}
	} // void OnFormListClient_Del

	void CHTourForm::OnFormListClient_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpClient.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListClient->m_col_object ];
			udPListHandlerPick handler = GetHClientListPick( );
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			handler->BeforeCreate( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать туриста" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Выбрать туриста" ) );
			//wnd.add( *( handler->GetWindowPick( ) ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPBase obj = handler->GetObject( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				replace->get( "client" )->ini( obj->get( "id" )->toUInt( ) );
				replace->get( "fio" )->ini( obj->get( "fio" )->toWString( ) );

				row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( replace->get( "fio" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListClient_Replace

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourForm::OnFormListApp_Select( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnAppEdit->set_sensitive( true );
			m_btnAppDel->set_sensitive( true );
			m_btnAppReplace->set_sensitive( true );
		} else {
			m_btnAppEdit->set_sensitive( false );
			m_btnAppDel->set_sensitive( false );
			m_btnAppReplace->set_sensitive( false );
		}
	} // void OnFormListApp_Select

	void CHTourForm::OnFormListApp_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Gtk::FileChooserDialog dlgPickFile( *wndParent, Glib::locale_to_utf8( "Выберите файл" ) );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

		int code = dlgPickFile.run( );

		if ( code == Gtk::RESPONSE_OK ) {
			Glib::RefPtr< Gio::File > file = dlgPickFile.get_file( );
			wstring szFileName = ConvertString( file->get_basename( ), CP_UTF8 );
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
			udPBase pCurApp = NULL;
			bool bFound = false;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				pCurApp = *i;
				if ( pCurApp->get( "name" )->toWString( ) == szFileName ) {
					bFound = true;
					break;
				}
			}

			if ( bFound ) {
				Gtk::MessageDialog dialog(
					*wndParent,
					Glib::locale_to_utf8( "Файл уже присутствует в списке" ),
					false,
					Gtk::MESSAGE_WARNING,
					Gtk::BUTTONS_OK
				);
				dialog.set_secondary_text(
					Glib::locale_to_utf8( "Файл '" ) +
					file->get_basename( ) +
					Glib::locale_to_utf8( "'\nСмените имя или выберите другой." )
				);
				dialog.run( );
			} else {
				wstring szFilePath;
				if ( file->has_parent( ) ) {
					szFilePath = ConvertString( file->get_parent( )->get_path( ), CP_UTF8 );
				}
				udPBase add = new udProjectAttach;
				add->get( "obj" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
				add->get( "type" )->ini( udProjectAttach::ATTACH_TOUR );
				add->get( "name" )->ini( szFileName );
				add->get( "orig" )->ini( szFilePath );

				wstring wFileTmpPath = m_szFolderApp + L"\\" + szFileName;
				Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

				if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
					add->get( "tmp_path" )->ini( wFileTmpPath );
					arrObject->push_back( add );

					Gtk::TreeModel::Row row = *( m_objAppModel->append( ) );

					row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
					row[ m_pModelFormListApp->m_col_object ] = add;
				} else {
					udDELETE( add );
					Gtk::MessageDialog dialog(
						*wndParent,
						Glib::locale_to_utf8( "Не удалось скопировать файл" ),
						false,
						Gtk::MESSAGE_ERROR,
						Gtk::BUTTONS_OK
					);
					dialog.set_secondary_text(
						Glib::locale_to_utf8( "Файл '" ) +
						file->get_basename( ) +
						Glib::locale_to_utf8( "'\nПовторите попытку или выберите другой." )
					);
					dialog.run( );
				}
			}
		}
	} // void OnFormListApp_Add

	void CHTourForm::OnFormListApp_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListApp->m_col_object ];
			ExtOpenFile( edit->get( "tmp_path" )->toWString( ), false );
		}
	} // void OnFormListApp_Edit

	void CHTourForm::OnFormListApp_Del( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListApp->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objAppModel->erase( i );
		}
	} // void OnFormListApp_Del

	void CHTourForm::OnFormListApp_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListApp->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			Gtk::FileChooserDialog dlgPickFile( *wndParent, Glib::locale_to_utf8( "Выберите файл" ) );
			dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
			dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

			int code = dlgPickFile.run( );

			if ( code == Gtk::RESPONSE_OK ) {
				Glib::RefPtr< Gio::File > file = dlgPickFile.get_file( );
				wstring szFileName = ConvertString( file->get_basename( ), CP_UTF8 );
				udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
				udPBase pCurApp = NULL;
				bool bFound = false;

				for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
					pCurApp = *k;
					if ( pCurApp->get( "name" )->toWString( ) == szFileName ) {
						bFound = true;
						break;
					}
				}

				if ( bFound ) {
					Gtk::MessageDialog dialog(
						*wndParent,
						Glib::locale_to_utf8( "Файл уже присутствует в списке" ),
						false,
						Gtk::MESSAGE_WARNING,
						Gtk::BUTTONS_OK
					);
					dialog.set_secondary_text(
						Glib::locale_to_utf8( "Файл '" ) +
						file->get_basename( ) +
						Glib::locale_to_utf8( "'\nСмените имя или выберите другой." )
					);
					dialog.run( );
				} else {
					wstring szFilePath;
					if ( file->has_parent( ) ) {
						szFilePath = ConvertString( file->get_parent( )->get_path( ), CP_UTF8 );
					}

					wstring wFileTmpPath = m_szFolderApp + L"\\" + szFileName;
					Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

					if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
						replace->get( "tmp_path" )->ini( wFileTmpPath );
						replace->get( "name" )->ini( szFileName );
						replace->get( "orig" )->ini( szFilePath );

						Gtk::TreeModel::Row row = *i;

						row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( replace->get( "name" )->toString( ) );
					} else {
						Gtk::MessageDialog dialog(
							*wndParent,
							Glib::locale_to_utf8( "Не удалось скопировать файл" ),
							false,
							Gtk::MESSAGE_ERROR,
							Gtk::BUTTONS_OK
						);
						dialog.set_secondary_text(
							Glib::locale_to_utf8( "Файл '" ) +
							file->get_basename( ) +
							Glib::locale_to_utf8( "'\nПовторите попытку или выберите другой." )
						);
						dialog.run( );
					}
				}
			}
		}
	} // void OnFormListApp_Del

	void CHTourForm::OnFormListApp_Folder( ) {
		ExtOpenFolder( m_szFolderApp );
	} // void OnFormListApp_Folder

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourForm::OnFormListTour_Select( ) {
		Gtk::TreeModel::iterator i = m_inpTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnTourReplace->set_sensitive( true );
			m_btnTourDel->set_sensitive( true );
		} else {
			m_btnTourReplace->set_sensitive( false );
			m_btnTourDel->set_sensitive( false );
		}
	} // void OnFormListTour_Select

	void CHTourForm::OnFormListTour_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTourModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			Gtk::TreeModel::Row row = *i;
			udPBase obj = row[ m_pModelFormListTour->m_col_object ];

			if ( obj ) {
				string log = "\n";
				obj->dump( log );
				g_pApp->GetLog( )->Write( log );

				Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
				Gtk::ScrolledWindow wndScroll;
				udPHTourFormModal handler = new udHTourFormModal( );
				handler->Init( );
				handler->GetData( )->SetId( obj->get( "tour2" )->toUInt( ) );
				handler->Load( );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование тура" ) );

				/*Gtk::Window wnd;
				wnd.set_size_request( 800, 600 );
				wnd.set_modal( true );
				wnd.set_title( Glib::locale_to_utf8( "Редактирование тура" ) );
				//wnd.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				//wnd.show_all( );
				wnd.add( wndScroll );
				wndScroll.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				handler->GetWindow( )->show( );
				wndScroll.show( );
				wnd.show( );
				Gtk::Main::run( wnd );*/

				udPBase tour = handler->GetData( )->GetData( );
				if ( tour ) {
					obj->get( "name" )->ini( tour->get( "name" )->toWString( ) );

					row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
				}

				udDELETE( handler );
			}
		}
	} // void OnFormListClient_Activate

	void CHTourForm::OnFormListTour_Add( ) {
		m_pData->MakeObject( );

		udPListHandlerPick handler = GetHTourListPick( );
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать тур" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбрать тур" ) );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj ) {
			udPBase add = new udProjectTourTour;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidTour );
			udPArrBase arrObject = pFde->GetArray( );

			add->get( "tour1" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			add->get( "tour2" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "tour2_" )->ini( obj->get( "name" )->toWString( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objTourModel->append( ) );

			row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( add->get( "tour2_" )->toString( ) );
			row[ m_pModelFormListTour->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListTour_Add

	void CHTourForm::OnFormListTour_Del( ) {
		Gtk::TreeModel::iterator i = m_inpTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListTour->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objTourModel->erase( i );
		}
	} // void OnFormListTour_Del

	void CHTourForm::OnFormListTour_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListTour->m_col_object ];
			udPListHandlerPick handler = GetHTourListPick( );
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			handler->BeforeCreate( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать тур" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Выбрать тур" ) );
			//wnd.add( *( handler->GetWindowPick( ) ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPBase obj = handler->GetObject( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				replace->get( "tour2" )->ini( obj->get( "id" )->toUInt( ) );
				replace->get( "tour2_" )->ini( obj->get( "name" )->toWString( ) );

				row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( replace->get( "tour2_" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListTour_Replace

	void CHTourForm::OnFormListTour_FromCopy( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Gtk::ScrolledWindow wndScroll;
		udPHTourFormModal handler = new udHTourFormModal( );
		handler->Init( );
		handler->LoadFromTour( this );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание тура" ) );

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectTourTour;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidTour );
			udPArrBase arrObject = pFde->GetArray( );

			add->get( "tour1" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			add->get( "tour2" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "tour2_" )->ini( obj->get( "name" )->toWString( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objTourModel->append( ) );

			row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( add->get( "tour2_" )->toString( ) );
			row[ m_pModelFormListTour->m_col_object ] = add;
		}
		
		udDELETE( handler );
	} // void OnFormListTour_FromCopy

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourForm::OnFormListService_Select( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnServiceEdit->set_sensitive( true );
			m_btnServiceDel->set_sensitive( true );
		} else {
			m_btnServiceEdit->set_sensitive( false );
			m_btnServiceDel->set_sensitive( false );
		}
	} // void OnFormListService_Select

	void CHTourForm::OnFormListService_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objServiceModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			Gtk::TreeModel::Row row = *i;
			udPBase obj = row[ m_pModelFormListService->m_col_object ];

			if ( obj ) {
				string log = "\n";
				obj->dump( log );
				g_pApp->GetLog( )->Write( log );

				udPHServiceFormModal handler = new udHServiceFormModal( );
				handler->Init( );
				handler->GetData( )->SetId( obj->get( "service" )->toUInt( ) );
				handler->Load( );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование услуги" ) );

				/*Gtk::Window wnd;
				wnd.set_modal( true );
				wnd.set_title( Glib::locale_to_utf8( "Редактирование услуги" ) );
				wnd.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				wnd.show_all( );
				Gtk::Main::run( wnd );*/

				udPBase service = handler->GetData( )->GetData( );
				if ( service ) {
					udPFDEObject pFdeName = ( udPFDEObject ) handler->GetData( )->GetExtraData( udHServiceForm::ctrlidName );
					if ( pFdeName ) {
						udPBase pName = pFdeName->GetObject( );
						if ( pName ) {
							obj->get( "service_" )->ini( pName->get( "name" )->toWString( ) );

							row[ m_pModelFormListService->m_col_service ] = Glib::locale_to_utf8( pName->get( "name" )->toString( ) );
						}
					}
				}

				udDELETE( handler );
			}
		}
	} // void OnFormListService_Activate

	void CHTourForm::OnFormListService_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourServiceFormModal handler = new udHTourServiceFormModal( );
		handler->Init( );

		udPBase pFakeService = new udProjectTourService;
		pFakeService->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		pFakeService->get( "date2" )->ini( m_inpDate2.GetValue( ) );

		handler->LoadFrom( pFakeService );

		udDELETE( pFakeService );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание услуги" ) );
		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание услуги" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectTourService;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidService );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "order" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objServiceModel->append( ) );

			udPFDEObject pFdeService = ( udPFDEObject ) pDa->GetExtraData( udHTourServiceForm::ctrlidService );
			if ( pFdeService ) {
				udPBase pService = pFdeService->GetObject( );
				if ( pService ) {
					row[ m_pModelFormListService->m_col_service ] = Glib::locale_to_utf8( pService->get( "name_" )->toString( ) );
				}
			}

			row[ m_pModelFormListService->m_col_num		] = Glib::locale_to_utf8( add->get( "num" )->toString( ) );
			row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );

			if ( add->get( "ready" )->toUInt( ) ) {
				row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Готова" );
			} else {
				row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Не готова" );
			}

			row[ m_pModelFormListService->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListService_Add

	void CHTourForm::OnFormListService_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListService->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourServiceFormModal handler = new udHTourServiceFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование услуги" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование услуги" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				udPFDEObject pFdeService = ( udPFDEObject ) pDa->GetExtraData( udHTourServiceForm::ctrlidService );
				if ( pFdeService ) {
					udPBase pService = pFdeService->GetObject( );
					if ( pService ) {
						row[ m_pModelFormListService->m_col_service ] = Glib::locale_to_utf8( pService->get( "name_" )->toString( ) );
					}
				}

				row[ m_pModelFormListService->m_col_num ] = Glib::locale_to_utf8( edit->get( "num" )->toString( ) );
				row[ m_pModelFormListService->m_col_date1 ] = Glib::locale_to_utf8( edit->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_date2 ] = Glib::locale_to_utf8( edit->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );

				if ( edit->get( "ready" )->toUInt( ) ) {
					row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Готова" );
				} else {
					row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Не готова" );
				}
			}

			udDELETE( handler );
		}
	} // void OnFormListService_Edit

	void CHTourForm::OnFormListService_Del( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListService->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objServiceModel->erase( i );
		}
	} // void OnFormListService_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectServiceForPrint( ) {
		udPBase ret = GetProjectService( );
		// атрибуты с наименованиями
		ret->get( "name_" )->ini( wstring( L"" ) );
		ret->get( "type_" )->ini( wstring( L"" ) );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		// атрибуты из цены
		ret->get( "num"					)->ini( ( udUInt )	0		);	// Количество людей
		ret->get( "amount"				)->ini( ( udFloat )	0.0f	);	// Сумма
		ret->get( "currency"			)->ini( ( udUInt )	0		);	// ID валюты
		ret->get( "amount_recommend"	)->ini( ( udFloat )	0.0f	);	// Сумма "рекомендованная"
		ret->get( "currency_recommend"	)->ini( ( udUInt )	0		);	// Сумма "рекомендованная"
		ret->get( "currency_" )->ini( wstring( L"" ) );
		ret->get( "currency_recommend_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectServiceForPrint

	void CHTourForm::GetServicesPrices( udArrBase& arrService ) {
		udPBase pTour = m_pData->GetData( );
		if ( !pTour->get( "id" )->toUInt( ) ) {
			return;
		}

		udPArrBase ret = NULL;
		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( string( "tour" ) );
			data.add( ( udInt ) opidGetServicePrice );
			data.add( pTour->get( "id" )->toUInt( ) );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udDataAccess::Read( sock, arrService, GetProjectServiceForPrint );
			}
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 1 );
			//////////////////////////////////////////////////////////////////////////////////
			sock->Shutdown( );
			sock->Close( );
		}
		udDELETE( sock );
	} // void GetServicesPrices

	udPArrBase LoadClients( const map< udUInt, udUInt >& ids ) {
		if ( ids.empty( ) ) {
			return new udArrBase;
		}
		udPArrBase ret = NULL;
		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( string( "client" ) );
			data.add( ( udInt ) udHClient::opidGet3 );
			data.add( ( udUInt ) ids.size( ) );
			for( map< udUInt, udUInt >::const_iterator i = ids.begin( ); i != ids.end( ); ++i ) {
				data.add( i->first );
			}
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udArrBase arr;
				udType total( ( udUInt ) 0 );
				total.read( sock );
				udDataAccess::Read( sock, arr, GetProjectClient );
				string log = "\n";
				udPBase add = NULL;
				ret = new udArrBase;
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					add = GetProjectClient( );
					add->apply( *i, true, true );
					add->dump( log );
					log += "\n";
					ret->push_back( add );
				}
				g_pApp->GetLog( )->Write( log );
				ClearArrBase( arr );
			}
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////
			sock->Shutdown( );
			sock->Close( );
		}
		udDELETE( sock );
		return ret;
	} // void LoadClients

	void CHTourForm::OnButtonDocPrint( ) {
		udPBase			pDocTpl = NULL,		// шаблон документа
						pClient	= NULL,		// клиент
						pEmpl	= NULL,		// заказчик
						pClDoc	= NULL,		// документ заказчика
						pInfo	= NULL,		// общие данные по групповому договору
						pApp1	= NULL,		// приложение 1 группового договора
						pApp2	= NULL,		// приложение 2 группового договора
						obj		= NULL;		// объект выборки
		udPArrBase		pClients	= NULL,	// туристы из тура
						pArrClient	= ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );	// туристы из тура, которые в форме
		udPListHandlerPick	hList	= NULL;
		udPMorph		tmp		= NULL;						// временный объект
		udPLog			__log	= g_pApp->GetLog( );
		string			log		= "";
		Gtk::Window*	wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		//////////////////////////////////////////////////////////////////////////////////
		// выбираем шаблон документа
		hList = GetHDocTemplateListPick( );
		hList->BeforeCreate( );

		ModalWindow( wndParent, hList->GetWindow( ), Glib::locale_to_utf8( "Выбор шаблона документа" ) );

		obj = hList->GetObject( );
		if ( obj ) {
			pDocTpl = new udProjectDocTemplate;
			pDocTpl->apply( obj, true, true );

			log = "\nвыбранный шаблон документа\n";
			pDocTpl->dump( log );
			__log->Write( log + "\n" );
			obj = NULL;
		}

		udDELETE( hList );
		//////////////////////////////////////////////////////////////////////////////////
		// выбираем туриста (если не групповой тур)
		if ( pDocTpl && ( pDocTpl->get( "type" )->toInt( ) != udProjectDocTemplate::tplContract2 ) ) {
			//////////////////////////////////////////////////////////////////////////////
			// выведем туристов из тура
			for( udArrBase::iterator i = pArrClient->begin( ); i != pArrClient->end( ); ++i ) {
				( *i )->dump( log );
				log += "\n";
			}
			__log->Write( log );
			//////////////////////////////////////////////////////////////////////////////
			// выбор туриста
			udPHTouristPick hTouristPick = new udHTouristPick;
			hTouristPick->SetTourForm( this );
			hTouristPick->BeforeCreate( );

			ModalWindow( wndParent, hTouristPick->GetWindow( ), Glib::locale_to_utf8( "Выбор туриста" ) );

			obj = hTouristPick->GetObject( );
			if ( obj ) {
				pClient = new udProjectClient;
				pClient->apply( obj, true, true );

				log = "\nвыбранный турист\n";
				pClient->dump( log );
				__log->Write( log + "\n" );
				obj = NULL;
			}
			udDELETE( hTouristPick );
			//////////////////////////////////////////////////////////////////////////////
		}
		// выбираем документ заказчика
		if ( pDocTpl ) {
			if ( m_inpEmployer->GetType( ) == emptypeFiz ) {
				udPBase doc = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument ) )->GetObject( );
				if ( !doc ) {
					m_inpDocument.CallClick( );
				}

				doc = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument ) )->GetObject( );
				pClDoc = new udProjectClDoc;
				pClDoc->apply( doc, true, true );

				log = "\nдокумент заказчика\n";
				pClDoc->dump( log );
				__log->Write( log + "\n" );
			}
		}
		
		if ( pDocTpl ) {
			map< udUInt, udUInt > ids;
			udUInt id = 0;
			for( udArrBase::iterator i = pArrClient->begin( ); i != pArrClient->end( ); ++i ) {
				id = ( *i )->get( "client" )->toUInt( );
				ids[ id ] = id;
				g_pApp->GetLog( )->Write( toString( id ) + "\n" );
			}
			pClients = LoadClients( ids );
		}
		// генерируем результат
		if ( pDocTpl ) {
			// генерируем результат
			udHDocTemplate::ProcContractTemplate( this, pClient, pDocTpl, pClDoc, /*amount,*/ pClients );
			ClearArrBase( pClients );
		}
		//////////////////////////////////////////////////////////////////////////////////
		udDELETE( pDocTpl );
		udDELETE( pClient );
		udDELETE( pClDoc );
		udDELETE( hList );
		udDELETE( pInfo );
		//udDELETE( pApp1 );
		udDELETE( pApp2 );
		udDELETE( pClients );
		udDELETE( tmp );
		//////////////////////////////////////////////////////////////////////////////////
	} // void OnButtonDocPrint

	void CHTourForm::OnButtonAddToGroup( ) {
		udPBase obj		= NULL,
				pGroup	= NULL,
				pPeriod	= NULL;
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHGroupPick hPick = new udHGroupPick;
		hPick->BeforeCreate( );

		ModalWindow( wndParent, hPick->GetWindow( ), Glib::locale_to_utf8( "Выбор группы" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбор группы" ) );
		//wnd.add( *( hPick->GetWindowPick( ) ) );
		wnd.add( *( hPick->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		obj = hPick->GetObject( );
		if ( obj ) {
			pGroup = new udProjectGroup;
			pGroup->apply( obj, true, true );
		}

		udDELETE( hPick );

		if ( pGroup ) {
			string log = "\n";
			pGroup->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			udPHGroupPeriodLoadPick hPickPeriod = new udHGroupPeriodLoadPick;
			hPickPeriod->SetTourForm( this );
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

			obj = hPickPeriod->GetObject( );
			if ( obj ) {
				pPeriod = new udProjectGroupPeriod;
				pPeriod->apply( obj, true, true );
			}

			udDELETE( hPickPeriod );
		}
		
		if ( pPeriod ) {
			string log = "\n";
			pPeriod->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			// тут вызов на добавление
			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				// ListRequest
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) opidAddToGroup );
				data.add( m_pData->GetData( )->get( "id" )->toUInt( ) );
				data.add( pGroup->get( "id" )->toUInt( ) );
				data.add( pPeriod->get( "index" )->toUInt( ) );

				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );

				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );

				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		udDELETE( pGroup );
		udDELETE( pPeriod );
	} // void OnButtonAddToGroup

	void CHTourForm::CalculateFinal( udArrBase& arrSumm ) {
		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			// ListRequest
			udBinaryData data;
			data.add( string( "tour" ) );
			data.add( ( udInt ) opidCalculateFinal );
			data.add( m_pData->GetData( )->get( "id" )->toUInt( ) );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udDataAccess::Read( sock, arrSumm, GetProjectPersAccSummList );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
	} // void CalculateFinal

	void CHTourForm::OnButtonMakeCopy( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Gtk::ScrolledWindow wndScroll;
		udPHTourFormModal handler = new udHTourFormModal( );
		handler->Init( );
		handler->LoadFromTour( this );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание тура" ) );
		
		udDELETE( handler );
	} // void OnButtonMakeCopy

	void CHTourForm::LoadFromTplTour( udPHTplTourForm pTplTour ) {
		if ( !pTplTour ) {
			return;
		}

		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPDAForm	pDATplTour		= pTplTour->GetData( );
		udPArrBase	arrDstObject	= NULL,
					arrSrcObject	= NULL;
		udPBase		pDstObject		= NULL,
					pSrcObject		= NULL;
		udPBase		pDstDirect		= NULL,
					pSrcDirect		= NULL;
		Gtk::TreeModel::Row row;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем наименование
		m_inpName.SetValue( pTplTour->m_inpName.GetValue( ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем направление
		pDstDirect = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( );
		pSrcDirect = ( ( udPFDEObject ) pDATplTour->GetExtraData( udHTplTourForm::ctrlidDirect ) )->GetObject( );
		pSrcDirect->apply( pDstDirect );
		m_inpDirect.SetTitle( Glib::locale_to_utf8( pDstDirect->get( "name" )->toString( ) ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем маршрут
		arrDstObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );
		arrSrcObject = ( ( udPFDEList ) pDATplTour->GetExtraData( udHTplTourForm::ctrlidRoute ) )->GetArray( );

		for( udArrBase::iterator i = arrSrcObject->begin( ); i != arrSrcObject->end( ); ++i ) {
			pSrcObject = *i;
			pDstObject = new udProjectTourRoute;
			pDstObject->get( "tour" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			pDstObject->get( "city" )->ini( pSrcObject->get( "city" )->toUInt( ) );
			pDstObject->get( "city_" )->ini( pSrcObject->get( "city_" )->toWString( ) );

			row = *( m_objRouteModel->append( ) );
			row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( pDstObject->get( "city_" )->toString( ) );
			row[ m_pModelFormListRoute->m_col_object ] = pDstObject;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем услуги
		arrDstObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		arrSrcObject = ( ( udPFDEList ) pDATplTour->GetExtraData( udHTplTourForm::ctrlidService ) )->GetArray( );

		for( udArrBase::iterator i = arrSrcObject->begin( ); i != arrSrcObject->end( ); ++i ) {
			pSrcObject = *i;
			pDstObject = new udProjectTourService;
			pDstObject->get( "tour" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			pDstObject->get( "service" )->ini( pSrcObject->get( "service" )->toUInt( ) );
			pDstObject->get( "service_" )->ini( pSrcObject->get( "service_" )->toWString( ) );
			pDstObject->get( "num" )->ini( pSrcObject->get( "num" )->toUInt( ) );
			pDstObject->get( "date1" )->ini( pSrcObject->get( "date1" )->toTime( ) );
			pDstObject->get( "date2" )->ini( pSrcObject->get( "date2" )->toTime( ) );

			row = *( m_objServiceModel->append( ) );
			row[ m_pModelFormListService->m_col_service	] = Glib::locale_to_utf8( pDstObject->get( "service_" )->toString( ) );
			row[ m_pModelFormListService->m_col_num		] = Glib::locale_to_utf8( pDstObject->get( "num" )->toString( ) );
			row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( pDstObject->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( pDstObject->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListService->m_col_ready	] = Glib::locale_to_utf8( "Не готова" );
			row[ m_pModelFormListService->m_col_object	] = pDstObject;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadFromTplTour

	void CHTourForm::LoadFromTour( udPHTourForm pTour ) {
		if ( !pTour ) {
			return;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем текстовые данные
		m_inpName.SetValue( pTour->m_inpName.GetValue( ) );
		m_inpComment.SetValue( pTour->m_inpComment.GetValue( ) );
		m_inpDate1.SetValue( pTour->m_inpDate1.GetValue( ) );
		m_inpDate2.SetValue( pTour->m_inpDate2.GetValue( ) );
		m_inpStatus.SetValue( pTour->m_inpStatus.LoadValue( ).GetValue( ) - 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем контролы выбора из списка
		udPBase pSrcObj = NULL, pDstObj = NULL;
		udPFDEObject pDstFde = NULL, pSrcFde = NULL;
		// компания
		pSrcFde = ( ( udPFDEObject ) pTour->m_pData->GetExtraData( ctrlidCompany ) );
		pSrcObj = pSrcFde->GetObject( );

		if ( pSrcObj && pSrcObj->get( "id" )->toUInt( ) ) {
			pDstFde = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCompany ) );
			pDstFde->Make( );
			pDstObj = pDstFde->GetObject( );
			pSrcObj->apply( pDstObj );
			m_inpCompany.SetTitle( Glib::locale_to_utf8( pSrcObj->get( "name" )->toString( ) ) );
		}
		// направление
		pSrcFde = ( ( udPFDEObject ) pTour->m_pData->GetExtraData( ctrlidDirect ) );
		pSrcObj = pSrcFde->GetObject( );

		if ( pSrcObj && pSrcObj->get( "id" )->toUInt( ) ) {
			pDstFde = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) );
			pDstFde->Make( );
			pDstObj = pDstFde->GetObject( );
			pSrcObj->apply( pDstObj );
			m_inpDirect.SetTitle( Glib::locale_to_utf8( pSrcObj->get( "name" )->toString( ) ) );
		}
		// ответственный
		pSrcFde = ( ( udPFDEObject ) pTour->m_pData->GetExtraData( ctrlidUser ) );
		pSrcObj = pSrcFde->GetObject( );

		if ( pSrcObj && pSrcObj->get( "id" )->toUInt( ) ) {
			pDstFde = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidUser ) );
			pDstFde->Make( );
			pDstObj = pDstFde->GetObject( );
			pSrcObj->apply( pDstObj );
			m_inpUser.SetTitle( Glib::locale_to_utf8( pSrcObj->get( "fio" )->toString( ) ) );
		}
		// состояние
		pSrcFde = ( ( udPFDEObject ) pTour->m_pData->GetExtraData( ctrlidState ) );
		pSrcObj = pSrcFde->GetObject( );

		if ( pSrcObj && pSrcObj->get( "id" )->toUInt( ) ) {
			pDstFde = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) );
			pDstFde->Make( );
			pDstObj = pDstFde->GetObject( );
			pSrcObj->apply( pDstObj );
			m_inpState.SetTitle( Glib::locale_to_utf8( pSrcObj->get( "name" )->toString( ) ) );
		}
		// заказчик
		pSrcFde = ( ( udPFDEObject ) pTour->m_pData->GetExtraData( ctrlidEmployer ) );
		pSrcObj = pSrcFde->GetObject( );

		if ( pSrcObj && pSrcObj->get( "id" )->toUInt( ) ) {
			string szAttr = "fio";
			if ( pTour->m_inpEmployer->GetType( ) == emptypeUr ) {
				szAttr = "name";
			}
			pDstFde = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) );
			pDstFde->Make( );
			pDstObj = pDstFde->GetObject( );
			pSrcObj->apply( pDstObj );
			m_inpEmployer->SetType( pTour->m_inpEmployer->GetType( ) );
			m_inpEmployer->SetTitle( Glib::locale_to_utf8( pSrcObj->get( szAttr )->toString( ) ) );
		}
		// документ заказчика
		pSrcFde = ( ( udPFDEObject ) pTour->m_pData->GetExtraData( ctrlidDocument ) );
		pSrcObj = pSrcFde->GetObject( );

		if ( pSrcObj && pSrcObj->get( "id" )->toUInt( ) ) {
			pDstFde = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument ) );
			pDstFde->Make( );
			pDstObj = pDstFde->GetObject( );
			pSrcObj->apply( pDstObj );
			m_inpDocument.SetTitle( Glib::locale_to_utf8( pSrcObj->get( "series" )->toString( ) + " " + pSrcObj->get( "number" )->toString( ) ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем списки
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrSrc = NULL, arrDst = NULL;
		// маршрут
		arrSrc = ( ( udPFDEList ) pTour->m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );
		arrDst = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );

		for( udArrBase::iterator i = arrSrc->begin( ); i != arrSrc->end( ); ++i ) {
			pDstObj = new udProjectTourRoute;
			( *i )->apply( pDstObj );
			pDstObj->get( "id" )->ini( ( udUInt ) 0 );
			arrDst->push_back( pDstObj );
		}
		// услуги
		arrSrc = ( ( udPFDEList ) pTour->m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		arrDst = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );

		for( udArrBase::iterator i = arrSrc->begin( ); i != arrSrc->end( ); ++i ) {
			pDstObj = new udProjectTourService;
			( *i )->apply( pDstObj );
			pDstObj->get( "id" )->ini( ( udUInt ) 0 );
			arrDst->push_back( pDstObj );
		}
		// туристы
		arrSrc = ( ( udPFDEList ) pTour->m_pData->GetExtraData( ctrlidClient ) )->GetArray( );
		arrDst = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );

		for( udArrBase::iterator i = arrSrc->begin( ); i != arrSrc->end( ); ++i ) {
			pDstObj = new udProjectTourClient;
			( *i )->apply( pDstObj );
			pDstObj->get( "id" )->ini( ( udUInt ) 0 );
			arrDst->push_back( pDstObj );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objRouteModel->append( ) );
				row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( add->get( "city_" )->toString( ) );
				row[ m_pModelFormListRoute->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objServiceModel->append( ) );
				row[ m_pModelFormListService->m_col_service	] = Glib::locale_to_utf8( add->get( "service_" )->toString( ) );
				row[ m_pModelFormListService->m_col_num		] = Glib::locale_to_utf8( add->get( "num" )->toString( ) );
				row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );

				if ( add->get( "ready" )->toUInt( ) ) {
					row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Готова" );
				} else {
					row[ m_pModelFormListService->m_col_ready ] = Glib::locale_to_utf8( "Не готова" );
				}

				row[ m_pModelFormListService->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objClientModel->append( ) );
				row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( add->get( "fio" )->toString( ) );
				row[ m_pModelFormListClient->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL, copy = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objTourModel->append( ) );
				row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( add->get( "tour2_" )->toString( ) );
				row[ m_pModelFormListTour->m_col_object ] = add;
			}
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*arrDstObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
		arrObject = ( ( udPFDEList ) pTour->m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objAppModel->append( ) );
				row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
				row[ m_pModelFormListApp->m_col_object ] = add;
			}
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// копируем приложение
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadFromTour

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourFormModal::CHTourFormModal( ) : CHTourForm( ) {
	} // CHTourFormModal

	CHTourFormModal::CHTourFormModal( const CHTourFormModal& objCopy ) : CHTourForm( ) {
	} // CHTourFormModal

	CHTourFormModal::~CHTourFormModal( ) {
		g_pApp->GetLog( )->Write( "CHTourFormModal::~CHTourFormModal\n" );
	} // ~CHTourFormModal

	CHTourFormModal& CHTourFormModal::operator =( const CHTourFormModal& objCopy ) {
		return *this;
	} // CHTourFormModal& operator =

	void CHTourFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourFormModeless::CHTourFormModeless( ) : CHTourForm( ) {
	} // CHTourFormModeless

	CHTourFormModeless::CHTourFormModeless( const CHTourFormModeless& objCopy ) : CHTourForm( ) {
	} // CHTourFormModeless

	CHTourFormModeless::~CHTourFormModeless( ) {
	} // ~CHTourFormModeless

	CHTourFormModeless& CHTourFormModeless::operator =( const CHTourFormModeless& objCopy ) {
		return *this;
	} // CHTourFormModeless& operator =

	void CHTourFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "tour" );
	} // void OnButtonOk

	void CHTourFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "tour" );
	} // void CHTourForm

	void CHTourFormModeless::Init( ) {
		CHTourForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHTourFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHTourFormModeless

} // namespace UdSDK
