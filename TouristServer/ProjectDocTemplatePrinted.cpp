
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectDocTemplatePrinted2( ) {
		udPBase ret = GetProjectDocTemplatePrinted( );
		ret->get( "createdby_" )->ini( wstring( L"" ) );
		ret->get( "tour_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectDocTemplatePrinted2

	CHDocTemplatePrinted::CHDocTemplatePrinted( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_doctplprinted";
		m_szUpdIndex	= "id";
		m_szName		= "doctplprinted";
		m_fnGetObject	= GetProjectDocTemplatePrinted;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_user", "createdby", "id", GetProjectUser );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_tour", "tour", "id", GetProjectTour );

		m_pObjFilter->get( "series" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "number" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		m_pObjFilter->get( "date2" )->ini( date );
		m_pObjFilter->get( "createdby" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "type" )->ini( ( udInt ) 0 );
		m_pObjFilter->get( "tour" )->ini( ( udUInt ) 0 );
	}

	CHDocTemplatePrinted::CHDocTemplatePrinted( const CHDocTemplatePrinted& objCopy ) {
	}

	CHDocTemplatePrinted::~CHDocTemplatePrinted( ) {
	}

	CHDocTemplatePrinted& CHDocTemplatePrinted::operator =( const CHDocTemplatePrinted& objCopy ) {
		return *this;
	}

	void CHDocTemplatePrinted::Init( ) {
		udStateHandlerSimple::Init( );
		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\doctplprinted";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	void CHDocTemplatePrinted::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		if ( ( opId == opidAdd1 ) || ( opId == opidUpd1 ) ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		/*char code = 0;
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
				wstring dst_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) + L"\\doctplprinted\\" + obj->get( "id" )->toWString( );
				CreateDirectory( dst_dir.data( ), NULL );
				DirectoryCopyFiles( tmp_dir, dst_dir );
				DirectoryClear( tmp_dir );
				//////////////////////////////////////////////////////////////////////////////////
			}
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void ExtraDataRecieve

	void CHDocTemplatePrinted::ExtraDataSending( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		//sock->SendByte( ( char ) 1 );
		udPLog __log = g_pApp->GetLog( );
		string log = "\n";
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj1 = NULL;
		udArrBase arr;
		udArrError aerr;
		udBinaryData data;
		//////////////////////////////////////////////////////////////////////////////////////////
		// тур
		/*obj1 = GetProjectTour( );
		obj1->GetConfig( config );
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + obj->get( "tour" )->toString( ) );
		handler->GetUdObject( arr, option, &aerr );
		if ( aerr.empty( ) && !arr.empty( ) ) {
			obj1 = arr[ 0 ];
			arr.clear( );
		}
		obj1->dump( data );
		udDELETE( obj1 );
		//////////////////////////////////////////////////////////////////////////////////////////
		// пользователь
		obj1 = GetProjectUser( );
		obj1->GetConfig( config );
		option.SetTable( "ud_user" );
		option.SetClass( GetProjectUser );
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + obj->get( "createdby" )->toString( ) );
		handler->GetUdObject( arr, option, &aerr );
		if ( aerr.empty( ) && !arr.empty( ) ) {
			obj1 = arr[ 0 ];
			arr.clear( );
		}
		obj1->dump( data );
		udDELETE( obj1 );
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		data.clear( );*/
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->SendByte( ( char ) 80 ); // есть файлы
		//////////////////////////////////////////////////////////////////////////////////////////
		udInt err = 0;
		udFileTransfer objFileTransfer;
		wstring filename, path,
		client_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
			L"\\doctplprinted\\" + obj->get( "id" )->toWString( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// файл шаблона
		WriteUdLibLog( "\n\nотсылаем файл шаблона\n" );
		filename = obj->get( "text" )->toWString( );
		path = client_dir + L"\\" + filename;
		objFileTransfer.SetFileName( filename );
		objFileTransfer.SetFilePath( path );
		err = objFileTransfer.Send( sock );
		log += udFileTransfer::ErrorExplain( err ) + "\n";
		__log->Write( log );
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->SendByte( ( char ) 80 ); // есть файлы
		log = "\n";
		//////////////////////////////////////////////////////////////////////////////////////////
		// файл результата для шаблона
		WriteUdLibLog( "\n\nотсылаем файл результата для шаблона\n" );
		filename = L"out.xml";
		path = client_dir + L"\\" + filename;
		objFileTransfer.SetFileName( filename );
		objFileTransfer.SetFilePath( path );
		err = objFileTransfer.Send( sock );
		log += udFileTransfer::ErrorExplain( err ) + "\n";
		__log->Write( log );
		//////////////////////////////////////////////////////////////////////////////////////////
		WriteUdLibLog( "> закончена отсылка шаблонов\n" );
	} // void ExtraDataSending

	bool CHDocTemplatePrinted::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_doctplprinted" );
		option.SetClass( GetProjectDocTemplatePrinted2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "createdby" ) {
			szOrder = "`doctplprinted_createdby_` ";
		} else {
			szOrder = "`doctplprinted_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"fio", "doctplprinted_createdby_", "ud_user",
			"`ud_user`.`user_id`=`ud_doctplprinted`.`doctplprinted_createdby`", GetProjectUser
		);
		option.AddLeftJoin(
			"name", "doctplprinted_tour_", "ud_tour",
			"`ud_tour`.`tour_id`=`ud_doctplprinted`.`doctplprinted_tour`", GetProjectTour
		);
		option.SetSqlCalcFoundRows( true );
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetIgnoreAttr( "createdby_" );
		option.SetIgnoreAttr( "type_" );
		option.SetIgnoreAttr( "tour_" );
		handler->GetUdObject( arr, option, &err );
		if ( err.empty( ) ) {
		} else {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPDatabase db = g_pApp->GetDatabase( );
		udArrMorph arr1;
		if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arr1 ) ) {
			cnt = arr1[ 0 ]->get( "cnt" )->toLLong( );
			ClearArrMorph( arr1 );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool CustomLoadObjects

	string CHDocTemplatePrinted::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectDocTemplatePrinted;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udUInt id = 0;
		udInt status = 0;
		datetime date;
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "series" )->toWString( );
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

			string add = config.GetAttrIndex( "series" ) + " LIKE '";

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
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "number" )->toWString( );
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

			string add = config.GetAttrIndex( "number" ) + " LIKE '";

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
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "created" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "created" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		status = m_pObjFilter->get( "type" )->toInt( );
		if ( status > 0 ) {
			arrString.push_back( config.GetAttrIndex( "type" ) + "=" + toString( status ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "createdby" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "createdby" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "tour" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "tour" ) + "=" + toString( id ) );
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

} // namespace UdSDK
