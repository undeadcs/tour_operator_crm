
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectGroupContractF( ) {
		udPBase ret = new udBase;
		ret->get( "days1" )->ini( wstring( L"" ) );
		ret->get( "days2" )->ini( wstring( L"" ) );
		ret->get( "days3" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGroupContractF

	udPBase GetProjectGroupContractApp2F( ) {
		udPBase ret = new udBase;
		ret->get( "name" )->ini( wstring( L"" ) );
		ret->get( "addr" )->ini( wstring( L"" ) );
		ret->get( "number" )->ini( wstring( L"" ) );
		ret->get( "fintype" )->ini( wstring( L"" ) );
		ret->get( "garant" )->ini( wstring( L"" ) );
		ret->get( "orgname" )->ini( wstring( L"" ) );
		ret->get( "garantaddr" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGCTouristInfo






	CHDocTemplate::CHDocTemplate( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_doctpl";
		m_szUpdIndex	= "id";
		m_szName		= "doctpl";
		m_fnGetObject	= GetProjectDocTemplate;

		//this->AddManagedTable( "ud_doctplprinted", "id", GetProjectDocTemplatePrinted );

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
	}

	CHDocTemplate::~CHDocTemplate( ) {
	}

	void CHDocTemplate::Init( ) {
		udStateHandlerSimple::Init( );
		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		//folder = appConfig->get( "temporary_dir" )->toWString( ) + L"\\tour";
		//CreateDirectory( folder.data( ), NULL );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\doctpl";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	void CHDocTemplate::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		if ( ( opId == opidAdd1 ) || ( opId == opidUpd1 ) ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		char code = 0;
		sock->ReadByte( &code );
		g_pApp->GetLog( )->Write( "code=" + toString( code ) + "\n" );
		if ( code == 80 ) {
			udType num( ( udUInt ) 0 );
			if ( num.read( sock ) != 1 ) {
				return;
			}
			g_pApp->GetLog( )->Write( "num=" + num.toString( ) + "\n" );
			if ( num.toUInt( ) > 0 ) {
				//////////////////////////////////////////////////////////////////////////////////
				udPLog __log = g_pApp->GetLog( );
				udUInt cnt = 0;
				string log;
				wstring tmp_dir = g_pApp->GetConfig( )->get( "temporary_dir" )->toWString( );
				udInt err = 0;
				udFileTransfer objFileTransfer;
				// очищаем временную папку от левых файлов
				DirectoryClear( tmp_dir );
				objFileTransfer.SetTmpDir( tmp_dir );
				err = objFileTransfer.Recv( sock );
				log = udFileTransfer::ErrorExplain( err ) + "\n";
				__log->Write( log );
				//////////////////////////////////////////////////////////////////////////////////
				// копируем файлы
				wstring dst_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) + L"\\doctpl\\" + obj->get( "id" )->toWString( );
				CreateDirectory( dst_dir.data( ), NULL );
				DirectoryCopyFiles( tmp_dir, dst_dir );
				DirectoryClear( tmp_dir );
				//////////////////////////////////////////////////////////////////////////////////
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void ExtraDataRecieve

	void CHDocTemplate::ExtraDataSending( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->SendByte( ( char ) 80 );
		udPLog __log = g_pApp->GetLog( );
		string log = "\n";
		udInt err = 0;
		udFileTransfer objFileTransfer;
		wstring filename, path,
		client_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
			L"\\doctpl\\" + obj->get( "id" )->toWString( );
		filename = obj->get( "text" )->toWString( );
		path = client_dir + L"\\" + filename;
		objFileTransfer.SetFileName( filename );
		objFileTransfer.SetFilePath( path );
		err = objFileTransfer.Send( sock );
		log += udFileTransfer::ErrorExplain( err ) + "\n";
		__log->Write( log );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	string CHDocTemplate::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectDocTemplate;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		string attrs[ 1 ] = { "name" };
		//////////////////////////////////////////////////////////////////////////////////////////
		for( int i = 0; i < 1; ++i ) {
			str = m_pObjFilter->get( attrs[ i ] )->toWString( );
			if ( !str.empty( ) ) {
				string szStr = ConvertString( str, CP_UTF8 );
				if ( szStr.size( ) ) {
					udDWord	dwLen	= szStr.size( ) * 2;
					udPChar szTmp	= new udChar[ dwLen + 1 ];

					memset( szTmp, 0, dwLen + 1 );
					UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

					szStr = szTmp;

					delete [ ] szTmp;
				}

				string add = config.GetAttrIndex( attrs[ i ] ) + " LIKE '";

				int pos = 0;
				for( string::iterator i = szStr.begin( ); i != szStr.end( ); ++i, ++pos ) {
					if ( *i == '*' ) {
						add += '%';
					} else if ( *i == '%' ) {
					} else {
						add += *i;
					}
				}

				add += "'";
				arrString.push_back( add );
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

	void CHDocTemplate::SchemeCenter( udInt opId, udPSocket sock ) {
		if ( opId > opidGet ) {
			if ( opId == opidGetFile ) { // подгрузка файла шаблона
				udType id( ( udUInt  ) 0 );
				if ( id.read( sock ) == 1 ) {
					udPHandler handler = g_pApp->GetHandler( );
					udHandlerOption option;
					udBaseConfig config;
					udArrBase arr;
					udArrError err;
					udPBase obj = NULL;
					string szWhere = "", log = "";
					udPLog __log = g_pApp->GetLog( );
					//
					obj = new udProjectDocTemplate;
					obj->GetConfig( config );
					udDELETE( obj );
					szWhere = config.GetAttrIndex( "id" ) + "=" + id.toString( );
					config.Clear( );
					option.SetTable( "ud_doctpl" );
					option.SetClass( GetProjectDocTemplate );
					option.SetWhere( szWhere );
					handler->GetUdObject( arr, option, &err );
					if ( !err.empty( ) ) {
						sock->SendByte( ( char ) 0 );
						WriteErrors( __log, err );
					} else if ( arr.empty( ) ) {
						sock->SendByte( ( char ) 0 );
					} else {
						sock->SendByte( ( char ) 1 );
						obj = arr[ 0 ];
						udInt errCode = 0;
						udFileTransfer objFileTransfer;
						wstring filename, path,
						client_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
							L"\\doctpl\\" + obj->get( "id" )->toWString( );
						filename = obj->get( "text" )->toWString( );
						path = client_dir + L"\\" + filename;
						objFileTransfer.SetFileName( filename );
						objFileTransfer.SetFilePath( path );
						errCode = objFileTransfer.Send( sock );
						log += udFileTransfer::ErrorExplain( errCode ) + "\n";
						__log->Write( log );
						char byte = 0;
						sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
						sock->ReadByte( &byte );
					}
					ClearArrBase( arr );
				}
			} else if ( opId == opidGetResult ) {
				//////////////////////////////////////////////////////////////////////////////////
				// считываем
				udPBase	pTour	= GetProjectTour( ),
						pTpl	= GetProjectDocTemplate( ),
						add		= GetProjectDocTemplatePrinted( );
				udType str( wstring( L"" ) );
				wstring szHead = L"", szText = L"", szRet = L"";
				datetime date;
				udArrError err;
				udBinaryData data;
				pTour->read( sock );
				pTpl->read( sock );
				str.read( sock );
				szHead = str.toWString( );
				str.ini( wstring( L"" ) );
				str.read( sock );
				szText = str.toWString( );
				date.load( );
				//////////////////////////////////////////////////////////////////////////////////
				// сохраняем распечатанный документ
				udPAppConfig appConfig = g_pApp->GetConfig( );
				udPHandler handler = g_pApp->GetHandler( );
				udHandlerOption option;
				CAppInitProject *init = ( CAppInitProject* ) g_pApp->GetInitObj( );
				SessionData sess = init->GetCurrentSessionData( );
				udPBase pUser = sess.user;
				//
				add->get( "created" )->ini( date );
				add->get( "createdby" )->ini( pUser->get( "id" )->toUInt( ) );
				add->get( "series" )->ini( pUser->get( "series" )->toWString( ) );
				add->get( "name" )->ini( pTpl->get( "name" )->toWString( ) );
				add->get( "type" )->ini( pTpl->get( "type" )->toInt( ) );
				add->get( "text" )->ini( pTpl->get( "text" )->toWString( ) );
				add->get( "tour" )->ini( pTour->get( "id" )->toUInt( ) );
				//
				option.SetTable( "ud_doctplprinted" );
				handler->AddUdObject( add, option, &err );
				if ( err.empty( ) ) {
					wchar_t tmp[ 1024 ];
					udUInt id = ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( );
					add->get( "id" )->ini( id );
					swprintf_s( tmp, 1024, L"%010d", id );
					add->get( "number" )->ini( wstring( tmp ) );
					option.SetIndexAttr( "id" );
					handler->UpdUdObject( add, option );
					szRet = L"<printed>"
							L"<series><![CDATA[" + add->get( "series" )->toWString( ) + L"]]></series>"
							L"<number><![CDATA[" + add->get( "number" )->toWString( ) + L"]]></number>"
							L"</printed>";
					string out = "";
					wstring folder = L"";
					folder = appConfig->get( "files_dir" )->toWString( ) + L"\\doctplprinted\\" + add->get( "id" )->toWString( );
					CreateDirectory( folder.data( ), NULL );
					szText = szHead + szRet + szText + L"</tpldata>";
					out = ConvertString( szText, CP_UTF8 );
					WriteToFile( folder + L"\\out.xml", out );
					out.clear( );
					wstring szCopyFile = appConfig->get( "files_dir" )->toWString( ) + L"\\doctpl\\" + pTpl->get( "id" )->toWString( ) + L"\\" + pTpl->get( "text" )->toWString( );
					CopyFile( szCopyFile.data( ), ( folder + L"\\" + pTpl->get( "text" )->toWString( ) ).data( ), FALSE );
				}
				//////////////////////////////////////////////////////////////////////////////////
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				data.add( ( char ) 80 );
				data.add( szRet );
				sock->Send( data );
				data.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				udDELETE( pTour );
				udDELETE( pTpl );
				udDELETE( add );
				//////////////////////////////////////////////////////////////////////////////////
				// старый вариант
				/*udPBase		pTour		= GetProjectTour( ),
							pClient		= NULL,
							pClDoc		= NULL,
							pDocTpl		= GetProjectDocTemplate( ),
							pInfo		= NULL,
							pApp1		= NULL,
							pApp2		= NULL;
				udPArrBase	pClients	= NULL;
				wstring		szAmount	= L"";
				udPLog		__log		= g_pApp->GetLog( );
				char		bExists		= 0;
				//////////////////////////////////////////////////////////////////////////////////
				// тур
				__log->Write( "CHDocTemplate::SchemeCenter, opidGetResult\nreading tour\n" );
				pTour->read( sock );
				//////////////////////////////////////////////////////////////////////////////////
				// клиент
				__log->Write( "reading client\n" );
				bExists = 0;
				sock->ReadByte( &bExists );
				if ( bExists ) {
					pClient = GetProjectClient( );
					pClient->read( sock );
				}
				
				//////////////////////////////////////////////////////////////////////////////////
				// документ клиента
				__log->Write( "reading client doc\n" );
				bExists = 0;
				sock->ReadByte( &bExists );
				if ( bExists ) {
					pClDoc = GetProjectClDoc( );
					pClDoc->read( sock );
				}
				//////////////////////////////////////////////////////////////////////////////////
				// шаблон документа
				__log->Write( "reading document template\n" );
				pDocTpl->read( sock );
				//////////////////////////////////////////////////////////////////////////////////
				// сумма
				udUInt iServiceNumber = 0, id = 0;
				udType amount( wstring( L"" ) );
				__log->Write( "reading amount\n" );
				amount.read( sock );
				//////////////////////////////////////////////////////////////////////////////////
				// услуги
				udType num( ( udUInt ) 0 );
				__log->Write( "reading number of services\n" );
				num.read( sock );
				iServiceNumber = num.toUInt( );
				map< udUInt, udUInt > ids;
				__log->Write( "reading services id\n" );
				for( udUInt i = 0; i < iServiceNumber; ++i ) {
					num.read( sock );
					id = num.toUInt( );
					ids[ id ] = id;
				}
				ids.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				// клиенты
				__log->Write( "reading clients\n" );
				bExists = 0;
				sock->ReadByte( &bExists );
				if ( bExists ) {
					pClients = new udArrBase;
					ReadInstances( sock, *pClients, GetProjectClient );
				}
				//////////////////////////////////////////////////////////////////////////////////
				// города
				num.ini( ( udUInt ) 0 );
				__log->Write( "reading number of cities\n" );
				num.read( sock );
				iServiceNumber = num.toUInt( );
				__log->Write( "reading cities id\n" );
				for( udUInt i = 0; i < iServiceNumber; ++i ) {
					num.read( sock );
					id = num.toUInt( );
					ids[ id ] = id;
				}
				ids.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				// информация
				__log->Write( "reading info\n" );
				bExists = 0;
				sock->ReadByte( &bExists );
				if ( bExists ) {
					pInfo = GetProjectGroupContractF( );
					pInfo->read( sock );
				}
				//////////////////////////////////////////////////////////////////////////////////
				// приложение 1
				__log->Write( "reading app1\n" );
				bExists = 0;
				sock->ReadByte( &bExists );
				if ( bExists ) {
					pApp1 = GetProjectGroupContractApp1F( );
					pApp1->read( sock );
					//////////////////////////////////////////////////////////////////////////////
					udPArrBase pArr = NULL;
					//////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCTouristInfo );
					pApp1->get( "info" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCOccupancy );
					pApp1->get( "occupancy" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCProgram );
					pApp1->get( "program" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCTransfer );
					pApp1->get( "transfer" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCCover );
					pApp1->get( "cover" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCVisa );
					pApp1->get( "visa" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCService );
					pApp1->get( "service" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = new udArrBase;
					ReadInstances( sock, *pArr, GetProjectGCTransfer2 );
					pApp1->get( "transfer2" )->ini( ( udPVoid ) pArr );
					//////////////////////////////////////////////////////////////////////////////
				}
				//////////////////////////////////////////////////////////////////////////////////
				// приложение 2
				__log->Write( "reading app2\n" );
				bExists = 0;
				sock->ReadByte( &bExists );
				if ( bExists ) {
					pApp2 = GetProjectGroupContractApp2F( );
					pApp2->read( sock );
				}
				//////////////////////////////////////////////////////////////////////////////////
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//////////////////////////////////////////////////////////////////////////////////
				ProcContractTemplate( pTour, pClient, pDocTpl, pClDoc, szAmount, pClients, pInfo, pApp1, pApp2 );
				//////////////////////////////////////////////////////////////////////////////////
				if ( pApp1 ) {
					udPArrBase pArr = NULL;
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "info" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "occupancy" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "program" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "transfer" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "cover" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "visa" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "service" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) pApp1->get( "transfer2" )->get( );
					for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
						udDELETE( *i );
					}
					udDELETE( pArr );
					//////////////////////////////////////////////////////////////////////////
				}
				if ( pClients ) {
					ClearArrBase( pClients );
				}
				//////////////////////////////////////////////////////////////////////////////////
				udDELETE( pClients );
				udDELETE( pInfo );
				udDELETE( pApp1 );
				udDELETE( pApp2 );
				udDELETE( pTour );
				udDELETE( pClient );
				udDELETE( pClDoc );
				udDELETE( pDocTpl );*/
				//////////////////////////////////////////////////////////////////////////////////
			}
		} else {
			udStateHandlerSimple::SchemeCenter( opId, sock );
		}
	} // void SchemeCenter

	void CHDocTemplate::ProcContractTemplate( udPBase pTour, udPBase pClient, udPBase pTpl, udPBase pClDoc, const wstring& szAmount, udPArrBase pClients, udPBase pInfo, udPBase pApp1, udPBase pApp2 ) {
		assert( pTpl );
		//////////////////////////////////////////////////////////////////////////////////////////
		/*wstring folder = L"", text = L"";
		string out= "", log = "";
		udPExtList extlst = NULL;
		udPBase obj = NULL;
		udPDwAscBase items = NULL;
		udPLog __log = g_pApp->GetLog( );
		udPSocket sock = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		// генерируем xml
		datetime dcur, dt1 = pTour->get( "date1" )->toTime( ), dt2 = pTour->get( "date2" )->toTime( );
		time_t days = 0, nights = 0, t1 = dt1.get( ), t2 = dt2.get( );
		dcur.load( );
		//nights = ( time_t ) ceil( ( double ) ( ( double ) t2 - ( double ) t1 ) / ( double ) 86400 );
		nights = ( t2 - t1 ) / 86400;
		days = nights + 1;
		__log->Write(
			"date1=" + dt1.toString( "%Y-%m-%d %H-%M-%S" ) + ", "
			"date2=" + dt2.toString( "%Y-%m-%d %H-%M-%S" ) + ", "
			"t1=" + toString( t1 ) + ", "
			"t2=" + toString( t2 ) + ", "
			"diff=" + toString( t2 - t1 ) + ", "
			"nights=" + toString( nights ) + ", "
			"days=" + toString( days ) + "\n"
		);
		text = L"<?xml version=\"1.0\"?><?xml-stylesheet type=\"text/xsl\" href=\"" + pTpl->get( "text" )->toWString( ) + L"\"?><tpldata>";
		text += L"<current_date date=\"" + dcur.toWString( "%d/%m/%Y" ) + L"\"/>";
		if ( pInfo ) {
			text += L"<info>"
				L"<days1><![CDATA[" + pInfo->get( "days1" )->toWString( ) + L"]]></days1>"
				L"<days2><![CDATA[" + pInfo->get( "days2" )->toWString( ) + L"]]></days2>"
				L"<days3><![CDATA[" + pInfo->get( "days3" )->toWString( ) + L"]]></days3>"
				L"</info>";
		}
		text += L"<tour "
			L"days=\"" + toWString( days ) + L"\" "
			L"nights=\"" + toWString( nights ) + L"\" "
			L"date_begin=\"" + dt1.toWString( "%d/%m/%Y" ) + L"\" "
			L"date_end=\"" + dt2.toWString( "%d/%m/%Y" ) + L"\" "
			L"><name><![CDATA[" + pTour->get( "name" )->toWString( ) + L"]]></name>"
			L"<amount><![CDATA[" + szAmount + L"]]></amount>"
			L"</tour>";
		text += L"<company><name><![CDATA[" + formTour->GetInfo( udHTour::ctrlidSeller )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) + L"]]></name></company>";
		text += L"<user><fio><![CDATA[" + formTour->GetInfo( udHTour::ctrlidManager )->extlst->extlst->GetObj( )->get( "fio" )->toWString( ) + L"]]></fio></user>";
		if ( pClient ) { // если распечатка группового договора, то клиент не турист не выбирается
			text += L"<tourist>"
				L"<fio><![CDATA[" + pClient->get( "fio" )->toWString( ) + L"]]></fio>"
				L"<email><![CDATA[" + pClient->get( "lemail" )->toWString( ) + L"]]></email>"
				L"<phone><![CDATA[" + pClient->get( "lphone" )->toWString( ) + L"]]></phone>"
				L"<addr><![CDATA[" + pClient->get( "laddr" )->toWString( ) + L"]]></addr>"
				L"</tourist>";
		}
		extlst = formTour->GetInfo( udHTour::ctrlidEmployer )->extlst->extlst;
		obj = extlst->GetObj( );
		text += L"<employer>"
			L"<fio><![CDATA[" + ( obj->isset( "fio" ) ? obj->get( "fio" )->toWString( ) : L"" ) + L"]]></fio>"
			L"<name><![CDATA[" + ( obj->isset( "name" ) ? obj->get( "name" )->toWString( ) : L"" ) + L"]]></name>"
			L"<email><![CDATA[" + obj->get( "lemail" )->toWString( ) + L"]]></email>"
			L"<phone><![CDATA[" + obj->get( "lphone" )->toWString( ) + L"]]></phone>"
			L"<addr><![CDATA[" + obj->get( "laddr" )->toWString( ) + L"]]></addr>"
			L"</employer>";
		if ( pClDoc ) {
			text += L"<cldoc>"
				L"<type><![CDATA[" + pClDoc->get( "type_" )->toWString( ) + L"]]></type>"
				L"<series><![CDATA[" + pClDoc->get( "series" )->toWString( ) + L"]]></series>"
				L"<number><![CDATA[" + pClDoc->get( "number" )->toWString( ) + L"]]></number>"
				L"<issuedate><![CDATA[" + pClDoc->get( "issuedate" )->toWString( ) + L"]]></issuedate>"
				L"<issuedby><![CDATA[" + pClDoc->get( "issuedby" )->toWString( ) + L"]]></issuedby>"
				L"<birthplace><![CDATA[" + pClDoc->get( "birthplace" )->toWString( ) + L"]]></birthplace>"
				L"<enddate><![CDATA[" + pClDoc->get( "enddate" )->toWString( ) + L"]]></enddate>"
				L"<fio><![CDATA[" + pClDoc->get( "fio" )->toWString( ) + L"]]></fio>"
				L"</cldoc>";
		}
		text += L"<services>";
		udPBase service = NULL;
		wstring name = L"", type_name = L"";
		for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
			service = *i;
			name = service->get( "name_" )->toWString( );
			type_name = service->get( "type_" )->toWString( );
			text += wstring( L"<service live=\"" ) + ( type_name == L"Гостиница" ? L"1" : L"0" ) + L"\" " +
				L"trans=\"" + ( type_name == L"Внутренний трансфер" ? L"1" : L"0" ) + L"\">"
				L"<name><![CDATA[" + name + L"]]></name></service>";
		}
		ClearArrBase( arrService );
		text += L"</services>";
		//////////////////////////////////////////////////////////////////////////////////////////
		map< udUInt, udUInt > idsCity;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		text += L"<route>";
		items = formTour->GetInfo( udHTour::ctrlidRoute )->frmlst->GetItems( );
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			id = i->second->get( "city" )->toUInt( );
			idsCity[ id ] = id;
			text += L"<city><name><![CDATA[" + i->second->get( "name" )->toWString( ) + L"]]></name></city>";
		}
		text += L"</route>";
		udPArrBase pArrCities = LoadCitiesById( idsCity );
		if ( pArrCities ) {
			__log->Write( "pArrCities->size()=" + toString( pArrCities->size( ) ) + "\n" );
			__log->Write( "items->size()=" + toString( items->size( ) ) + "\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			udPBase city = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			text += L"<route2>";
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				id = i->second->get( "city" )->toUInt( );
				__log->Write( "id=" + toString( id ) + "\n" );
				for( udArrBase::iterator j = pArrCities->begin( ); j != pArrCities->end( ); ++j ) {
					city = *j;
					__log->Write( "citi->id=" + city->get( "id" )->toString( ) + "\n" );
					if ( id == city->get( "id" )->toUInt( ) ) {
						text += L"<city>"
							L"<name><![CDATA[" + city->get( "name" )->toWString( ) + L"]]></name>"
							L"<country><![CDATA[" + city->get( "country_" )->toWString( ) + L"]]></country>"
							L"</city>";
						break;
					}
				}
			}
			text += L"</route2>";
			//////////////////////////////////////////////////////////////////////////////////////
			udUInt iLastCountry = 0, iCurCountry = 0;;
			text += L"<route3>";
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				id = i->second->get( "city" )->toUInt( );
				for( udArrBase::iterator j = pArrCities->begin( ); j != pArrCities->end( ); ++j ) {
					city = *j;
					if ( id == city->get( "id" )->toUInt( ) ) {
						iCurCountry = city->get( "country" )->toUInt( );
						if ( ( iLastCountry == 0 ) || ( iCurCountry != iLastCountry ) ) {
							text += L"<country><![CDATA[" + city->get( "country_" )->toWString( ) + L"]]></country>";
						}
						iLastCountry = iCurCountry;
						break;
					}
				}
			}
			text += L"</route3>";
			//////////////////////////////////////////////////////////////////////////////////////
			for( udArrBase::iterator i = pArrCities->begin( ); i != pArrCities->end( ); ++i ) {
				udDELETE( *i );
			}
			//////////////////////////////////////////////////////////////////////////////////////
		}
		udDELETE( pArrCities );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( pTpl->get( "type" )->toInt( ) == udProjectDocTemplate::tplContract2 ) {
			if ( pApp1 ) {
				text += L"<app1>"
					L"<country><![CDATA[" + pApp1->get( "country" )->toWString( ) + L"]]></country>"
					L"<date1><![CDATA[" + pApp1->get( "date1" )->toWString( ) + L"]]></date1>"
					L"<date2><![CDATA[" + pApp1->get( "date2" )->toWString( ) + L"]]></date2>"
					L"<price1><![CDATA[" + pApp1->get( "price1" )->toWString( ) + L"]]></price1>"
					L"<price2><![CDATA[" + pApp1->get( "price2" )->toWString( ) + L"]]></price2>"
					L"<advance><![CDATA[" + pApp1->get( "advance" )->toWString( ) + L"]]></advance>"
					L"<advdate><![CDATA[" + pApp1->get( "advdate" )->toWString( ) + L"]]></advdate>"
					L"<surcharge><![CDATA[" + pApp1->get( "surcharge" )->toWString( ) + L"]]></surcharge>"
					L"<surdate><![CDATA[" + pApp1->get( "surdate" )->toWString( ) + L"]]></surdate>"
					L"<product><![CDATA[" + pApp1->get( "product" )->toWString( ) + L"]]></product>";
				//////////////////////////////////////////////////////////////////////////////////////////
				udPArrBase pArr = NULL;
				udPBase pObj = NULL;
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "info" )->get( );
				text += L"<info_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<info>"
						L"<fio><![CDATA[" + pObj->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<sex><![CDATA[" + pObj->get( "sex" )->toWString( ) + L"]]></sex>"
						L"<born><![CDATA[" + pObj->get( "born" )->toWString( ) + L"]]></born>"
						L"<passport><![CDATA[" + pObj->get( "passport" )->toWString( ) + L"]]></passport>"
						L"</info>";
				}
				text += L"</info_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "occupancy" )->get( );
				text += L"<occupancy_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<occupancy>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"<category><![CDATA[" + pObj->get( "category" )->toWString( ) + L"]]></category>"
						L"<amount><![CDATA[" + pObj->get( "amount" )->toWString( ) + L"]]></amount>"
						L"<feed><![CDATA[" + pObj->get( "feed" )->toWString( ) + L"]]></feed>"
						L"</occupancy>";
				}
				text += L"</occupancy_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "program" )->get( );
				text += L"<program_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<program>"
						L"<text><![CDATA[" + pObj->get( "text" )->toWString( ) + L"]]></text>"
						L"</program>";
				}
				text += L"</program_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "transfer" )->get( );
				text += L"<transfer_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<transfer>"
						L"<fio><![CDATA[" + pObj->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<trans><![CDATA[" + pObj->get( "trans" )->toWString( ) + L"]]></trans>"
						L"<level><![CDATA[" + pObj->get( "level" )->toWString( ) + L"]]></level>"
						L"<date1><![CDATA[" + pObj->get( "date1" )->toWString( ) + L"]]></date1>"
						L"<date2><![CDATA[" + pObj->get( "date2" )->toWString( ) + L"]]></date2>"
						L"</transfer>";
				}
				text += L"</transfer_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "cover" )->get( );
				text += L"<cover_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<cover>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"</cover>";
				}
				text += L"</cover_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "visa" )->get( );
				text += L"<visa_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<visa>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"</visa>";
				}
				text += L"</visa_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "service" )->get( );
				text += L"<service_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<service>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"<char><![CDATA[" + pObj->get( "char" )->toWString( ) + L"]]></char>"
						L"</service>";
				}
				text += L"</service_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "transfer2" )->get( );
				text += L"<transfer2_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<transfer2>"
						L"<route><![CDATA[" + pObj->get( "route" )->toWString( ) + L"]]></route>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"</transfer2>";
				}
				text += L"</transfer2_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				text += L"</app1>";
			}
			if ( pApp2 ) {
				text += L"<app2>"
					L"<name><![CDATA[" + pApp2->get( "name" )->toWString( ) + L"]]></name>"
					L"<addr><![CDATA[" + pApp2->get( "addr" )->toWString( ) + L"]]></addr>"
					L"<number><![CDATA[" + pApp2->get( "number" )->toWString( ) + L"]]></number>"
					L"<fintype><![CDATA[" + pApp2->get( "fintype" )->toWString( ) + L"]]></fintype>"
					L"<garant><![CDATA[" + pApp2->get( "garant" )->toWString( ) + L"]]></garant>"
					L"<orgname><![CDATA[" + pApp2->get( "orgname" )->toWString( ) + L"]]></orgname>"
					L"<garantaddr><![CDATA[" + pApp2->get( "garantaddr" )->toWString( ) + L"]]></garantaddr>"
					L"</app2>";
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( pClients ) {
			g_pApp->GetLog( )->Write( "pClients:\n" );
			text += L"<clients>";
			if ( pTour->get( "emptype" )->toInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
				log = "\nclients:\n";
				udPBase client = NULL;
				udUInt iEmployerId = pTour->get( "employer" )->toUInt( );
				for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
					client = *i;
					client->dump( log );
					log += "\n";
					text += L"<client";
					if ( iEmployerId == client->get( "id" )->toUInt( ) ) {
						text += L" employer=\"1\"";
					}
					text += L">";
					text +=	L"<fio><![CDATA[" + client->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<fiolatin><![CDATA[" + client->get( "fiolatin" )->toWString( ) + L"]]></fiolatin>"
						L"<born><![CDATA[" + client->get( "born" )->toWString( ) + L"]]></born>"
						L"<sex><![CDATA[" + client->get( "sex" )->toWString( ) + L"]]></sex>"
						L"<lemail><![CDATA[" + client->get( "lemail" )->toWString( ) + L"]]></lemail>"
						L"<lphone><![CDATA[" + client->get( "lphone" )->toWString( ) + L"]]></lphone>"
						L"<laddr><![CDATA[" + client->get( "laddr" )->toWString( ) + L"]]></laddr>"
						L"</client>";
				}
				__log->Write( log );
			} else {
				udPBase client = NULL;
				for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
					client = *i;
					text += L"<client>"
						L"<fio><![CDATA[" + client->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<fiolatin><![CDATA[" + client->get( "fiolatin" )->toWString( ) + L"]]></fiolatin>"
						L"<born><![CDATA[" + client->get( "born" )->toWString( ) + L"]]></born>"
						L"<sex><![CDATA[" + client->get( "sex" )->toWString( ) + L"]]></sex>"
						L"<lemail><![CDATA[" + client->get( "lemail" )->toWString( ) + L"]]></lemail>"
						L"<lphone><![CDATA[" + client->get( "lphone" )->toWString( ) + L"]]></lphone>"
						L"<laddr><![CDATA[" + client->get( "laddr" )->toWString( ) + L"]]></laddr>"
						L"</client>";
				}
			}
			text += L"</clients>";
		} else {
			g_pApp->GetLog( )->Write( "pClients:NULL\n" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		text += L"</tpldata>";
		out = ConvertString( text, CP_UTF8 );
		WriteToFile( folder + L"\\out.xml", out );
		out.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// загружаем xsl
		sock = BeginRequest( );
		if ( sock ) {
			char code = 0;
			string log = "";
			udBinaryData data;
			data.add( string( "doctpl" ) );
			data.add( ( udInt ) opidGetFile );
			data.add( pTpl->get( "id" )->toUInt( ) );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->ReadByte( &code );
			if ( code > 0 ) {
				udInt errCode = 0;
				udFileTransfer objFileTransfer;
				// очищаем временную папку от левых файлов
				objFileTransfer.SetTmpDir( folder );
				errCode = objFileTransfer.Recv( sock );
				log = udFileTransfer::ErrorExplain( errCode ) + "\n";
				__log->Write( log );
			}
			//////////////////////////////////////////////////////////////////////////////////////
			// закрываем соединение
			sock->Shutdown( );
			sock->Close( );
			udDELETE( sock );
		}
		//*/
		//////////////////////////////////////////////////////////////////////////////////////////
		//ExtOpenFile( folder + L"\\out.xml" );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ProcContractTemplate

} // namespace UdSDK
