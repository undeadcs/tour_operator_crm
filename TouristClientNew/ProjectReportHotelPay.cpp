
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDAReportHotelPay::CDAReportHotelPay( ) : udDAList( ) {
		m_szName = "report_hotel_pay";
	} // CDAReportHotelPay

	CDAReportHotelPay::CDAReportHotelPay( const CDAReportHotelPay& objCopy ) : udDAList( objCopy ) {
		m_szName = "report_hotel_pay";
	} // CDAReportHotelPay

	CDAReportHotelPay::~CDAReportHotelPay( ) {
	} // ~CDAReportHotelPay

	CDAReportHotelPay& CDAReportHotelPay::operator =( const CDAReportHotelPay& objCopy ) {
		udDAList::operator =( objCopy );
		return *this;
	} // CDAReportHotelPay& operator =

	void CDAReportHotelPay::Load( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;

		option.SetTable( "ud_report_hotel_pay" );
		option.SetClass( GetProjectReportHotelPay );
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

	int CDAReportHotelPay::LoadRemote( ) {
		int ret = 0;
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

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			ret = ( int ) code;
			if ( code == 80 ) { // необходимо считать объекты
				udDataAccess::Read( sock, arrResult, GetProjectReportHotelPay );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
		
		if ( !arrResult.empty( ) ) {
			string log = "\narrResult, ";
			for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
				( *i )->dump( log );
				log += "\n";
			}
			g_pApp->GetLog( )->Write( log );

			g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_hotel_pay`" );
			option.SetTable( "ud_report_hotel_pay" );
			handler->AddUdObject( arrResult, option );
			ClearArrBase( arrResult );
		}

		return ret;
	} // int LoadRemote

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportHotelPayList( ) {
		udPBase ret = GetProjectReportHotelPay( );
		return ret;
	} // udPBase GetProjectReportHotelPayList

	CHReportHotelPay::CHReportHotelPay( ) :
		udListHandler( ),
		m_lblFltDate1( Glib::locale_to_utf8( "С" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "По" ) ),
		m_lblFltDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_inpFltDate2( udDatetime( 0, true ), udDatetime( 0, true ) ),
		m_btnLoad( Glib::locale_to_utf8( "Загрузить" ) ),
		m_btnExport( Glib::locale_to_utf8( "Экспорт" ) )
	{
		m_btnLoad.set_name( "TouristListButtonFilter" );
		m_btnLoad.set_size_request( 151, 34 );
		m_btnExport.set_name( "TouristListButtonReset" );
		m_btnExport.set_size_request( 151, 34 );

		m_szName = "report_hotel_pay";

		m_pRowModel = m_pColModel = new CHReportHotelPay::ColModelReportHotelPay( );

		udDELETE( m_pData );

		m_pData = new udDAReportHotelPay;

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( udDatetime( ) );
		filter->get( "date2" )->ini( udDatetime( 0, true ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
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
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// С
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// По
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Направление
		m_tblFilter.attach( m_lblFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		++++col1;
		++++col2;

		m_tblFilter.attach( m_btnLoad, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnExport, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// дополнительные кнопки
		m_btnLoad.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportHotelPay::OnButtonLoad ) );
		m_btnExport.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportHotelPay::OnButtonExport ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHReportHotelPay

	CHReportHotelPay::CHReportHotelPay( const CHReportHotelPay& objCopy ) : udListHandler( ) {
		m_szName = "report_hotel_pay";
	} // CHReportHotelPay

	CHReportHotelPay::~CHReportHotelPay( ) {
	} // ~CReportHotelPay

	CHReportHotelPay& CHReportHotelPay::operator =( const CHReportHotelPay& objCopy ) {
		return *this;
	} // CHReportHotelPay& operator =

	void CHReportHotelPay::Init( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// создаем нужные таблицы
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		option.SetTable( "ud_report_hotel_pay" );
		option.SetClass( GetProjectReportHotelPay );
		option.SetForceTable( true );
		handler->CheckTable( option );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "С" ), m_pColModel->m_col_date1 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "По" ), m_pColModel->m_col_date2 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Руководитель" ), m_pColModel->m_col_leader );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Количество" ), m_pColModel->m_col_num );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Гостиница" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Цена" ), m_pColModel->m_col_price );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Сумма" ), m_pColModel->m_col_price_and_num );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Цена (Юань)" ), m_pColModel->m_col_price_cny );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Сумма (Юань)" ), m_pColModel->m_col_price_cny_and_num );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Init

	void CHReportHotelPay::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;

		string log = "\n";
		udDatetime date;
		udFloat fPrice;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			
			log = "\n";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );

			date = obj->get( "date1" )->toTime( );
			if ( date.get( ) ) {
				row[ m_pColModel->m_col_date1 ] = Glib::locale_to_utf8( date.toString( "%d.%m.%Y" ) );
			}

			date = obj->get( "date2" )->toTime( );
			if ( date.get( ) ) {
				row[ m_pColModel->m_col_date2 ] = Glib::locale_to_utf8( date.toString( "%d.%m.%Y" ) );
			}

			if ( obj->get( "num" )->toUInt( ) > 0 ) {
				row[ m_pColModel->m_col_num ] = obj->get( "num" )->toString( );
			}

			fPrice = obj->get( "price" )->toFloat( );
			if ( fPrice > 0.0f ) {
				row[ m_pColModel->m_col_price ] = toString( fPrice, "%.2f" );

				if ( obj->get( "num" )->toUInt( ) > 0 ) {
					row[ m_pColModel->m_col_price_and_num ] = toString( fPrice * obj->get( "num" )->toFloat( ), "%.2f" );
				}
			}

			fPrice = obj->get( "price_cny" )->toFloat( );
			if ( fPrice > 0.0f ) {
				row[ m_pColModel->m_col_price_cny ] = toString( fPrice, "%.2f" );

				if ( obj->get( "num" )->toUInt( ) > 0 ) {
					row[ m_pColModel->m_col_price_cny_and_num ] = toString( fPrice * obj->get( "num" )->toFloat( ), "%.2f" );
				}
			}

			row[ m_pColModel->m_col_leader	] = Glib::locale_to_utf8( obj->get( "leader" )->toString( ) );
			row[ m_pColModel->m_col_name	] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_object	] = obj;
		}
	} // void Load

	void CHReportHotelPay::FilterUiClear( ) {
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
		m_inpFltDirect.SetTitle( "" );
	} // void FilterUiClear

	void CHReportHotelPay::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
		filter->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	void CHReportHotelPay::OnButtonLoad( ) {
		FilterUiToObject( );
		int result = ( ( udPDAReportHotelPay ) m_pData )->LoadRemote( );
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

	static void PrintReportHead( FILE* pFileOut, udPBase pPeriod ) {
		wstring tmp = L"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
			L"<html xmlns=\"http://www.w3.org/1999/xhtml\">"
			L"<head><title>Оплата гостиниц за период c " + pPeriod->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L" по " +
			pPeriod->get( "date2" )->toTime( ).toWString( "%d.%m.%Y" ) +
			L"</title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>"
			L"<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\"/>"
			L"</head><body>"
			L"<div style=\"padding: 5px; font-size: 14px; line-height: 18px;\">"
			L"Период: <b>" + pPeriod->get( "period_date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L"</b> &mdash; "
			L"<b>" + pPeriod->get( "period_date2" )->toTime( ).toWString( "%d.%m.%Y" ) + L"</b><br/>";

		wstring szDirect = pPeriod->get( "direct" )->toWString( );
		if ( !szDirect.empty( ) ) {
			tmp += L"Направление: <b>" + szDirect + L"</b><br/>";
		}

		tmp += L"</div>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportHead

	static void PrintReportTableHeader( FILE* pFileOut ) {
		wstring tmp = L"<table class=\"table\"><tr>"
			L"<th>С</th>"
			L"<th>По</th>"
			L"<th>Руководитель</th>"
			L"<th>Кол-во</th>"
			L"<th>Гостиница</th>"
			L"<th>Цена</th>"
			L"<th>Сумма</th>"
			L"<th>Цена (Юани)</th>"
			L"<th>Сумма (Юани)</th>"
			L"</tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportGroupTableHeader

	static void PrintReportTableRow( FILE* pFileOut, udPBase row ) {
		udDatetime date;
		wstring tmp = L"<tr><td>";

		date = row->get( "date1" )->toTime( );
		if ( date.get( ) ) {
			tmp += date.toWString( "%d.%m.%Y" );
		} else {
			tmp += L"&nbsp;";
		}

		tmp += L"</td><td>";

		date = row->get( "date2" )->toTime( );
		if ( date.get( ) ) {
			tmp += date.toWString( "%d.%m.%Y" );
		} else {
			tmp += L"&nbsp;";
		}

		tmp += L"</td><td>" + row->get( "leader" )->toWString( ) + L"</td>";

		if ( row->get( "num" )->toUInt( ) ) {
			tmp += L"<td>" + row->get( "num" )->toWString( ) + L"</td>";
		} else {
			tmp += L"<td>&nbsp;</td>";
		}
			
		tmp += L"<td>" + row->get( "name" )->toWString( ) + L"</td>";

		if ( row->get( "price" )->toFloat( ) ) {
			tmp += L"<td>" + row->get( "price" )->toWString( L"%.2f" ) + L"</td>";

			if ( row->get( "num" )->toUInt( ) ) {
				tmp += L"<td>" + toWString( row->get( "num" )->toFloat( ) * row->get( "price" )->toFloat( ), L"%.2f" ) + L"</td>";
			} else {
				tmp += L"<td>&nbsp;</td>";
			}
		} else {
			tmp += L"<td>&nbsp;</td><td>&nbsp;</td>";
		}

		if ( row->get( "price_cny" )->toFloat( ) ) {
			tmp += L"<td>" + row->get( "price_cny" )->toWString( L"%.2f" ) + L"</td>";

			if ( row->get( "num" )->toUInt( ) ) {
				tmp += L"<td>" + toWString( row->get( "num" )->toFloat( ) * row->get( "price_cny" )->toFloat( ), L"%.2f" ) + L"</td>";
			} else {
				tmp += L"<td>&nbsp;</td>";
			}
		} else {
			tmp += L"<td>&nbsp;</td><td>&nbsp;</td>";
		}

		tmp += L"</tr>";

		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportTableRow

	void CHReportHotelPay::OnButtonExport( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udPBase obj = NULL;
		udBaseConfig config;
		bool bOpen = false;
		udPDAReportHotelPay pData = ( udPDAReportHotelPay ) m_pData;
		udPBase pPeriod = NULL;
		wstring szFilename = g_pApp->GetConfig( )->get( "workdir" )->toWString( ) + L"\\export\\report-hotel-pay.html";

		option.SetTable( "ud_report_hotel_pay" );
		option.SetClass( GetProjectReportHotelPay );
		handler->GetUdObject( arr, option );
		option.Clear( );
		if ( !arr.empty( ) ) {
			FILE *pFileOut = NULL;
			if ( _wfopen_s( &pFileOut, szFilename.data( ), L"wb" ) == 0 ) {

				bOpen = true;

				wstring tmp = L"";
				string szUtf8 = "";

				PrintReportHead( pFileOut, *( arr.begin( ) ) );
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

	void CHReportHotelPay::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
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

	CHReportHotelPayModeless::CHReportHotelPayModeless( ) :
		udListHandlerModeless( ),
		CHReportHotelPay( )
	{
	} // CHReportHotelPayModeless

	CHReportHotelPayModeless::CHReportHotelPayModeless( const CHReportHotelPayModeless& objCopy ) : CHReportHotelPay( ) {
	} // CHReportHotelPayModeless

	CHReportHotelPayModeless::~CHReportHotelPayModeless( ) {
	} // ~CHReportHotelPayModeless

	CHReportHotelPayModeless& CHReportHotelPayModeless::operator =( const CHReportHotelPayModeless& objCopy ) {
		return *this;
	} // CHReportHotelPayModeless& operator =

	void CHReportHotelPayModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHReportHotelPay::Init( );
		udListHandlerModeless::Init( );
	} // void Init

} // namespace UdSDK
