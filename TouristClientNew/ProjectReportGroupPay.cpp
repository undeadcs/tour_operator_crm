
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDAReportGroupPay::CDAReportGroupPay( ) :
		udDAList( ),
		m_pGroup( NULL ),
		m_pGroupPeriod( NULL ),
		m_pGroupLeader( NULL )
	{
		m_szName = "report_group_pay";
	} // CDAReportGroupPay

	CDAReportGroupPay::CDAReportGroupPay( const CDAReportGroupPay& objCopy ) : udDAList( objCopy ) {
		m_szName = "report_group_pay";
	} // CDAReportGroupPay

	CDAReportGroupPay::~CDAReportGroupPay( ) {
		udDELETE( m_pGroup );
		udDELETE( m_pGroupPeriod );
		udDELETE( m_pGroupLeader );
	} // ~CDAReportGroupPay

	CDAReportGroupPay& CDAReportGroupPay::operator =( const CDAReportGroupPay& objCopy ) {
		udDAList::operator =( objCopy );
		return *this;
	} // CDAReportGroupPay& operator =

	void CDAReportGroupPay::Load( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;

		option.SetTable( "ud_group_pay" );
		option.SetClass( GetProjectGroup );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			m_pGroup = arr[ 0 ];
			arr.clear( );
		}

		option.SetTable( "ud_group_pay_period" );
		option.SetClass( GetProjectGroupPeriod );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			m_pGroupPeriod = arr[ 0 ];
			arr.clear( );
		}

		option.SetTable( "ud_group_pay_client" );
		option.SetClass( GetProjectClient );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			m_pGroupLeader = arr[ 0 ];
			arr.clear( );
		}

		udArrError err;

		option.SetTable( "ud_report_group_pay" );
		option.SetClass( GetProjectReportGroupPay );
		option.SetLimit( m_pPager->GetSQLLimit( ) );
		option.SetSqlCalcFoundRows( true );
		handler->GetUdObject( *m_arrObject, option, &err );

		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}

		udPDatabase db = g_pApp->GetDatabase( );
		udArrMorph arr1;
		if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arr1 ) ) {
			m_pPager->SetTotal( arr1[ 0 ]->get( "cnt" )->toUInt( ) );
			ClearArrMorph( arr1 );
		}
	} // void Load

	void CDAReportGroupPay::LoadRemote( ) {
		/*g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_group_pay`" );
		g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_group_pay_period`" );
		g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_group_pay`" );*/
		//*
		udArrBase arrResult;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			// ListRequest
			udBinaryData data;
			data.add( m_szName );

			string log = "\nфильтр, ";
			m_pFilter->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			m_pFilter->dump( data );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_group_pay`" );
			g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_group_pay_period`" );
			g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_group_pay_client`" );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				string log = "\n";

				udPBase tmp = new udProjectGroup;
				if ( tmp->read( sock ) ) {
					option.SetTable( "ud_group_pay" );
					handler->AddUdObject( tmp, option );
					option.Clear( );
					log = "\nCDAReportGroupPay::LoadRemote, group, ";
					tmp->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );
					udDELETE( tmp );

					tmp = new udProjectGroupPeriod;
					if ( tmp->read( sock ) ) {
						option.SetTable( "ud_group_pay_period" );
						handler->AddUdObject( tmp, option );
						option.Clear( );
						log = "\nCDAReportGroupPay::LoadRemote, period, ";
						tmp->dump( log );
						g_pApp->GetLog( )->Write( log + "\n" );
						udDELETE( tmp );

						char bHasLeader = 0;
						sock->ReadByte( &bHasLeader );
						
						if ( bHasLeader > 0 ) {
							tmp = new udProjectClient;
							if ( tmp->read( sock ) ) {
								
								option.SetTable( "ud_group_pay_client" );
								handler->AddUdObject( tmp, option );
								option.Clear( );
								log = "\nCDAReportGroupPay::LoadRemote, group leader, ";
								tmp->dump( log );
								g_pApp->GetLog( )->Write( log + "\n" );
								udDELETE( tmp );
							}
						}

						udDataAccess::Read( sock, arrResult, GetProjectReportGroupPay );
					}
				}
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
		//*/
		if ( !arrResult.empty( ) ) {
			string log = "\narrResult, ";
			for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
				( *i )->dump( log );
				log += "\n";
			}
			g_pApp->GetLog( )->Write( log );

			g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_group_pay`" );
			option.SetTable( "ud_report_group_pay" );
			handler->AddUdObject( arrResult, option );
			ClearArrBase( arrResult );
		}
	} // void LoadRemote

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportGroupPayList( ) {
		udPBase ret = GetProjectReportGroupPay( );
		return ret;
	} // udPBase GetProjectReportGroupPayList

	CHReportGroupPay::CHReportGroupPay( ) :
		udListHandler( ),
		m_lblFltGroup( Glib::locale_to_utf8( "Группа" ) ),
		m_lblFltPeriod( Glib::locale_to_utf8( "Период" ) ),
		m_btnLoad( Glib::locale_to_utf8( "Загрузить" ) ),
		m_btnExport( Glib::locale_to_utf8( "Экспорт" ) )
	{
		m_btnLoad.set_name( "TouristListButtonFilter" );
		m_btnLoad.set_size_request( 151, 34 );
		m_btnExport.set_name( "TouristListButtonReset" );
		m_btnExport.set_size_request( 151, 34 );

		m_szName = "report_group_pay";

		m_pRowModel = m_pColModel = new CHReportGroupPay::ColModelReportGroupPay( );

		udDELETE( m_pData );

		m_pData = new udDAReportGroupPay;
		//m_pData->SetName( "report_landing" );
		//m_pData->SetGetObject( GetProjectReportGroupPayList );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "group" )->ini( ( udUInt ) 0 );
		filter->get( "period" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectGroup );
		pFde->SetMark( fltGroup );
		m_pData->AddExtraData( pFde );
		m_inpFltGroup.SetDataObject( pFde );
		m_inpFltGroup.SetWindowTitle( Glib::locale_to_utf8( "Выбор группы" ) );
		m_inpFltGroup.SetAttr( "name" );
		m_inpFltGroup.SetGetListHandler( GetHGroupListPick );
		m_inpFltGroup.SetGetFormHandler( GetHGroupForm );
		m_inpFltGroup.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование группы" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectGroupPeriod );
		pFde->SetMark( fltPeriod );
		m_pData->AddExtraData( pFde );
		m_inpFltPeriod.SetGroupFrom( m_pData );
		m_inpFltPeriod.SetDataObject( pFde );
		//m_inpFltPeriod.SetWindowTitle( Glib::locale_to_utf8( "Выбор направления" ) );
		//m_inpFltPeriod.SetAttr( "name" );
		//m_inpFltPeriod.SetGetListHandler( GetHGroupListPick );
		//m_inpFltPeriod.SetGetFormHandler( GetHDirectForm );
		//m_inpFltPeriod.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование направления" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Группа
		m_tblFilter.attach( m_lblFltGroup, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltGroup, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Период
		m_tblFilter.attach( m_lblFltPeriod, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltPeriod, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnLoad, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnExport, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// дополнительные кнопки
		m_btnLoad.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportGroupPay::OnButtonLoad ) );
		m_btnExport.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportGroupPay::OnButtonExport ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHReportGroupPay

	CHReportGroupPay::CHReportGroupPay( const CHReportGroupPay& objCopy ) : udListHandler( ) {
		m_szName = "report_group_pay";
	} // CHReportGroupPay

	CHReportGroupPay::~CHReportGroupPay( ) {
	} // ~CReportGroupPay

	CHReportGroupPay& CHReportGroupPay::operator =( const CHReportGroupPay& objCopy ) {
		return *this;
	} // CHReportGroupPay& operator =

	void CHReportGroupPay::Init( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// создаем нужные таблицы
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		option.SetTable( "ud_group_pay" );
		option.SetClass( GetProjectGroup );
		option.SetForceTable( true );
		handler->CheckTable( option );

		option.SetTable( "ud_group_pay_period" );
		option.SetClass( GetProjectGroupPeriod );
		option.SetForceTable( true );
		handler->CheckTable( option );

		option.SetTable( "ud_group_pay_client" );
		option.SetClass( GetProjectClient );
		option.SetForceTable( true );
		handler->CheckTable( option );

		option.SetTable( "ud_report_group_pay" );
		option.SetClass( GetProjectReportGroupPay );
		option.SetForceTable( true );
		handler->CheckTable( option );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "ФИО" ), m_pColModel->m_col_fio );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Итоговая цена" ), m_pColModel->m_col_price );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Оплата за тур" ), m_pColModel->m_col_pay );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата оплаты" ), m_pColModel->m_col_date );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Место оплаты" ), m_pColModel->m_col_place );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Гостиница" ), m_pColModel->m_col_hotel );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Init

	void CHReportGroupPay::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;

		string log = "\n";
		udDatetime date;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			
			log = "\n";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id		] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_fio		] = Glib::locale_to_utf8( obj->get( "fio" )->toString( ) );
			row[ m_pColModel->m_col_price	] = Glib::locale_to_utf8( obj->get( "price" )->toString( "%.2f" ) );
			row[ m_pColModel->m_col_pay		] = Glib::locale_to_utf8( obj->get( "pay" )->toString( "%.2f" ) );
			date = obj->get( "date" )->toTime( );
			if ( date.get( ) ) {
				row[ m_pColModel->m_col_date ] = Glib::locale_to_utf8( date.toString( "%d.%m.%Y" ) );
			}
			row[ m_pColModel->m_col_place	] = Glib::locale_to_utf8( obj->get( "place" )->toString( ) );
			row[ m_pColModel->m_col_hotel	] = Glib::locale_to_utf8( obj->get( "hotel" )->toString( ) );
			row[ m_pColModel->m_col_object ] = obj;
		}
	} // void Load

	void CHReportGroupPay::FilterUiClear( ) {
		m_inpFltGroup.SetTitle( "" );
		m_inpFltPeriod.SetTitle( "" );
	} // void FilterUiClear

	void CHReportGroupPay::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "group" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltGroup ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "period" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltPeriod ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	void CHReportGroupPay::OnButtonLoad( ) {
		FilterUiToObject( );
		( ( udPDAReportGroupPay ) m_pData )->LoadRemote( );
		Clear( );
		Load( );
	} // void OnButtonLoad

	static void PrintReportHead( FILE* pFileOut, udPBase pGroup, udPBase pPeriod, udPBase pLeader ) {
		wstring tmp = L"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
			L"<html xmlns=\"http://www.w3.org/1999/xhtml\">"
			L"<head><title>Данные группы" + pGroup->get( "name" )->toWString( ) +
			L" за период c " + pPeriod->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L" по " +
			pPeriod->get( "date2" )->toTime( ).toWString( "%d.%m.%Y" ) +
			L"</title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>"
			L"<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\"/>"
			L"</head><body>"
			L"<div style=\"padding: 5px; font-size: 14px; line-height: 18px;\">"
			L"Группа: <b>" + pGroup->get( "name" )->toWString( ) + L"</b><br/>"
			L"Период: <b>" + pPeriod->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L"</b> &mdash; "
			L"<b>" + pPeriod->get( "date2" )->toTime( ).toWString( "%d.%m.%Y" ) + L"</b><br/>";

		if ( pLeader ) {
			tmp += L"Руководитель: <b>" + pLeader->get( "fio" )->toWString( ) + L"</b>";
		}

		tmp += L"</div>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportHead

	static void PrintReportTableHeader( FILE* pFileOut ) {
		wstring tmp = L"<table class=\"table\"><tr>"
			L"<th>№ п/п</th>"
			L"<th>ФИО</th>"
			L"<th>Итоговая цена</th>"
			L"<th>Оплата за тур</th>"
			L"<th>Дата оплаты</th>"
			L"<th>Место оплаты</th>"
			L"<th>Гостиница</th>"
			L"</tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportGroupTableHeader

	static void PrintReportTableRow( FILE* pFileOut, udPBase row ) {
		wstring tmp = L"<tr>"
			L"<td>" + row->get( "id" )->toWString( ) + L"</td>"
			L"<td>" + row->get( "fio" )->toWString( ) + L"</td>"
			L"<td>" + row->get( "price" )->toWString( L"%.2f" ) + L"</td>"
			L"<td>" + row->get( "pay" )->toWString( L"%.2f" ) + L"</td>"
			L"<td>";

		udDatetime date = row->get( "date" )->toTime( );

		if ( date.get( ) ) {
			tmp += date.toWString( "%d.%m.%Y" );
		}

		tmp += L"</td><td>" + row->get( "place" )->toWString( ) + L"</td>"
			L"<td>" + row->get( "hotel" )->toWString( ) + L"</td>"
			L"</tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportTableRow

	void CHReportGroupPay::OnButtonExport( ) {
		g_pApp->GetLog( )->Write( "CHReportGroupPay::OnButtonExport\n" );

		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udPBase obj = NULL;
		udBaseConfig config;
		bool bOpen = false;
		udPDAReportGroupPay pData = ( udPDAReportGroupPay ) m_pData;
		udPBase pGroup	= NULL,
				pPeriod = NULL,
				pLeader	= NULL;
		wstring szFilename = g_pApp->GetConfig( )->get( "workdir" )->toWString( ) + L"\\export\\report-group-pay.html";

		g_pApp->GetLog( )->Write( "CHReportGroupPay::OnButtonExport, filename='" + toString( szFilename ) + "'\n" );

		WriteUdBaseLog( "CHReportGroupPay::OnButtonExport\n\n" );
		option.SetTable( "ud_group_pay" );
		option.SetClass( GetProjectGroup );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		if ( !arr.empty( ) ) {
			pGroup = arr[ 0 ];
			arr.clear( );
		}
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}

		option.SetTable( "ud_group_pay_period" );
		option.SetClass( GetProjectGroupPeriod );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		if ( !arr.empty( ) ) {
			pPeriod = arr[ 0 ];
			arr.clear( );
		}
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}

		option.SetTable( "ud_group_pay_client" );
		option.SetClass( GetProjectClient );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		if ( !arr.empty( ) ) {
			pLeader = arr[ 0 ];
			arr.clear( );
		}
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}

		if ( !pGroup ) {
			g_pApp->GetLog( )->Write( "group not found\n" );
		}
		if ( !pPeriod ) {
			g_pApp->GetLog( )->Write( "period not found\n" );
		}

		if ( pGroup && pPeriod ) {
			option.SetTable( "ud_report_group_pay" );
			option.SetClass( GetProjectReportGroupPay );
			handler->GetUdObject( arr, option );
			option.Clear( );
			if ( !arr.empty( ) ) {
				FILE *pFileOut = NULL;
				if ( _wfopen_s( &pFileOut, szFilename.data( ), L"wb" ) == 0 ) {
					bOpen = true;

					wstring tmp = L"";
					string szUtf8 = "";

					PrintReportHead( pFileOut, pGroup, pPeriod, pLeader );
					PrintReportTableHeader( pFileOut );

					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						PrintReportTableRow( pFileOut, *i );
					}

					tmp = L"</table></body></html>";
					szUtf8 = ConvertString( tmp, CP_UTF8 );
					fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
					fclose( pFileOut );
				}
				ClearArrBase( arr );
			}
		}

		if ( bOpen ) {
			ExtOpenFile( szFilename );
		}
	} // void OnButtonExport

	void CHReportGroupPay::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase obj = row[ m_pColModel->m_col_object ];

			if ( obj && obj->get( "tour" )->toUInt( ) ) {
				Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
				udPHTourFormModal handler = new udHTourFormModal( );
				handler->Init( );
				handler->GetData( )->SetId( obj->get( "tour" )->toUInt( ) );
				handler->Load( );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование тура" ) );

				udDELETE( handler );
			}
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHReportGroupPayModeless::CHReportGroupPayModeless( ) :
		udListHandlerModeless( ),
		CHReportGroupPay( )
	{
	} // CHReportGroupPayModeless

	CHReportGroupPayModeless::CHReportGroupPayModeless( const CHReportGroupPayModeless& objCopy ) : CHReportGroupPay( ) {
	} // CHReportGroupPayModeless

	CHReportGroupPayModeless::~CHReportGroupPayModeless( ) {
	} // ~CHReportGroupPayModeless

	CHReportGroupPayModeless& CHReportGroupPayModeless::operator =( const CHReportGroupPayModeless& objCopy ) {
		return *this;
	} // CHReportGroupPayModeless& operator =

	void CHReportGroupPayModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHReportGroupPay::Init( );
		udListHandlerModeless::Init( );
	} // void Init

} // namespace UdSDK
