
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDAReportDayPay::CDAReportDayPay( ) : udDAList( ) {
		m_szName = "report_day_pay";
	} // CDAReportGroupPay

	CDAReportDayPay::CDAReportDayPay( const CDAReportDayPay& objCopy ) : udDAList( objCopy ) {
		m_szName = "report_group_pay";
	} // CDAReportGroupPay

	CDAReportDayPay::~CDAReportDayPay( ) {
	} // ~CDAReportGroupPay

	CDAReportDayPay& CDAReportDayPay::operator =( const CDAReportDayPay& objCopy ) {
		udDAList::operator =( objCopy );
		return *this;
	} // CDAReportGroupPay& operator =

	void CDAReportDayPay::Load( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;

		option.SetTable( "ud_report_day_pay" );
		option.SetClass( GetProjectReportDayPay );
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

	int CDAReportDayPay::LoadRemote( ) {
		int ret = 0;
		udArrBase arrResult;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( m_szName );

			string log = "\nфильтр, ";
			m_pFilter->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			m_pFilter->dump( data );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			ret = ( int ) code;
			if ( code == 80 ) { // необходимо считать объекты
				udDataAccess::Read( sock, arrResult, GetProjectReportDayPay );
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

			g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_day_pay`" );
			option.SetTable( "ud_report_day_pay" );
			handler->AddUdObject( arrResult, option );
			ClearArrBase( arrResult );
		}

		return ret;
	} // void LoadRemote

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportDayPayList( ) {
		udPBase ret = GetProjectReportDayPay( );
		return ret;
	} // udPBase GetProjectReportDayPayList

	CHReportDayPay::CHReportDayPay( ) :
		udListHandler( ),
		m_lblFltDate1( Glib::locale_to_utf8( "С" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "По" ) ),
		m_btnLoad( Glib::locale_to_utf8( "Загрузить" ) ),
		m_btnExport( Glib::locale_to_utf8( "Экспорт" ) )
	{
		m_btnLoad.set_name( "TouristListButtonFilter" );
		m_btnLoad.set_size_request( 151, 34 );
		m_btnExport.set_name( "TouristListButtonReset" );
		m_btnExport.set_size_request( 151, 34 );

		m_szName = "report_day_pay";

		m_pRowModel = m_pColModel = new CHReportDayPay::ColModelReportDayPay( );

		udDELETE( m_pData );

		m_pData = new udDAReportDayPay;

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( udDatetime( ) );
		filter->get( "date2" )->ini( udDatetime( ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Дата "С"
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Дата "По"
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnLoad, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnExport, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// дополнительные кнопки
		m_btnLoad.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportDayPay::OnButtonLoad ) );
		m_btnExport.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportDayPay::OnButtonExport ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHReportGroupPay

	CHReportDayPay::CHReportDayPay( const CHReportDayPay& objCopy ) : udListHandler( ) {
		m_szName = "report_day_pay";
	} // CHReportGroupPay

	CHReportDayPay::~CHReportDayPay( ) {
	} // ~CReportGroupPay

	CHReportDayPay& CHReportDayPay::operator =( const CHReportDayPay& objCopy ) {
		return *this;
	} // CHReportGroupPay& operator =

	void CHReportDayPay::Init( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// создаем нужные таблицы
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		option.SetTable( "ud_report_day_pay" );
		option.SetClass( GetProjectReportDayPay );
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
		m_objTreeView.append_column( Glib::locale_to_utf8( "Группа" ), m_pColModel->m_col_group );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Период" ), m_pColModel->m_col_period );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Ответственный" ), m_pColModel->m_col_manager );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Init

	void CHReportDayPay::Load( ) {
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
			row[ m_pColModel->m_col_group	] = Glib::locale_to_utf8( obj->get( "group" )->toString( ) );
			row[ m_pColModel->m_col_period	] = Glib::locale_to_utf8( obj->get( "period" )->toString( ) );
			row[ m_pColModel->m_col_manager	] = Glib::locale_to_utf8( obj->get( "manager" )->toString( ) );
			row[ m_pColModel->m_col_object ] = obj;
		}
	} // void Load

	void CHReportDayPay::FilterUiClear( ) {
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHReportDayPay::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
	} // void FilterUiToObject

	void CHReportDayPay::OnButtonLoad( ) {
		FilterUiToObject( );
		int result = ( ( udPDAReportDayPay ) m_pData )->LoadRemote( );
		Clear( );
		Load( );

		if ( result != 80 ) {
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );

			switch( result ) {
				case 0:
					mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Отсутствуют данные за данный период" ) );
					break;

				default:
					break;
			}
		}
	} // void OnButtonLoad

	static void PrintReportHead( FILE* pFileOut, udArrBase& arr ) {
		udPBase obj = *( arr.begin( ) );
		wstring tmp = L"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
			L"<html xmlns=\"http://www.w3.org/1999/xhtml\">"
			L"<head><title>Оплата за день c " + obj->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L" по " +
			obj->get( "date2" )->toTime( ).toWString( "%d.%m.%Y" ) +
			L"</title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>"
			L"<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\"/>"
			L"</head><body>"
			L"<div style=\"padding: 5px; font-size: 14px; line-height: 18px;\">"
			L"<b>" + obj->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L"</b> &mdash; "
			L"<b>" + obj->get( "date2" )->toTime( ).toWString( "%d.%m.%Y" ) + L"</b><br/>";

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
			L"<th>Группа</th>"
			L"<th>Период</th>"
			L"<th>Ответственный</th>"
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
			L"<td>" + row->get( "group" )->toWString( ) + L"</td>"
			L"<td>" + row->get( "period" )->toWString( ) + L"</td>"
			L"<td>" + row->get( "manager" )->toWString( ) + L"</td>"
			L"</tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportTableRow

	void CHReportDayPay::OnButtonExport( ) {
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

		option.SetTable( "ud_report_day_pay" );
		option.SetClass( GetProjectReportDayPay );
		handler->GetUdObject( arr, option );
		option.Clear( );
		if ( !arr.empty( ) ) {
			FILE *pFileOut = NULL;
			if ( _wfopen_s( &pFileOut, szFilename.data( ), L"wb" ) == 0 ) {
				bOpen = true;

				wstring tmp = L"";
				string szUtf8 = "";

				PrintReportHead( pFileOut, arr );
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

		if ( bOpen ) {
			ExtOpenFile( szFilename );
		}
	} // void OnButtonExport

	void CHReportDayPay::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
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

	CHReportDayPayModeless::CHReportDayPayModeless( ) :
		udListHandlerModeless( ),
		CHReportDayPay( )
	{
	} // CHReportDayPayModeless

	CHReportDayPayModeless::CHReportDayPayModeless( const CHReportDayPayModeless& objCopy ) : CHReportDayPay( ) {
	} // CHReportDayPayModeless

	CHReportDayPayModeless::~CHReportDayPayModeless( ) {
	} // ~CHReportDayPayModeless

	CHReportDayPayModeless& CHReportDayPayModeless::operator =( const CHReportDayPayModeless& objCopy ) {
		return *this;
	} // CHReportDayPayModeless& operator =

	void CHReportDayPayModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHReportDayPay::Init( );
		udListHandlerModeless::Init( );
	} // void Init

} // namespace UdSDK
