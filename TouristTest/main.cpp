
#include "includes.h"

namespace UdSDK {

	udPMorph	g_pConfig	= NULL;
	udPMySql	g_pMySQL	= NULL;
	udPHandler	g_pHandler	= NULL;

	void UdBaseEscapeString( char* out, const char* in, size_t len ) {
		g_pMySQL->EscapeString( out, in, len );
	} // void UdBaseEscapeString

} // namespace UdSDK

using namespace UdSDK;

bool	InitConfig( );
bool	InitMySQL( );

int main( int argc, char** argv ) {
	if ( InitConfig( ) && InitMySQL( ) ) {
	}
	udDELETE( g_pConfig );
	udDELETE( g_pHandler );
	udDELETE( g_pMySQL );
	printf( "Press any key to exit." );
	getchar( );
	return 0;
} // int main

bool InitConfig( ) {
	udPStdFile config = NULL;
	printf( "loading config...\r\n" );
	if ( fopen_s( &config, "server_config.txt", "rb" ) == 0 ) {
		g_pConfig = new udMorph;
		char line[ 1024 ];
		int i = 0, len = 0;
		string name = "", value = "";
		memset( line, 0, 1024 );
		while( !feof( config ) ) {
			if ( fgets( line, 1024, config ) != NULL ) {
				len = strlen( line );
				for( i = 0; i < len; ++i ) {
					if ( line[ i ] == '=' ) {
						break;
					}
					name += line[ i ];
				}
				++i;
				for( ; i < len; ++i ) {
					if ( line[ i ] == '\r' || line[ i ] == '\n' ) {
						break;
					}
					value += line[ i ];
				}
				if ( !name.empty( ) ) {
					g_pConfig->get( name )->ini( value );
				}
				name = "";
				value = "";
			}
		}

		fclose( config );
		printf( "config loaded...\r\n" );
		return true;
	}

	return false;
} // bool InitConfig

bool InitMySQL( ) {
	printf( "connecting to MySQL...\r\n" );
	wstring szPath = L"mysql\\bin\\mysqld.exe";
	if ( g_pConfig->isset( "mysql_path" ) ) {
		szPath = g_pConfig->get( "mysql_path" )->toWString( );
	}
	//
	g_pMySQL = new udMySql;
	string	host		= g_pConfig->get( "database_host"		)->toString( ),
			user		= g_pConfig->get( "database_user"		)->toString( ),
			password	= g_pConfig->get( "database_password"	)->toString( ),
			dbname		= g_pConfig->get( "database_dbname"		)->toString( );
	udUInt	port		= g_pConfig->get( "database_port"		)->toUInt( );
	// коннектимся к нему
	g_pMySQL->SetAccData( host, user, password, dbname );
	g_pMySQL->SetPortNumber( port );
	g_pMySQL->SetCharSet( "utf8" );
	if ( g_pMySQL->Connect( true ) == udMySql::UDSDK_MYSQL_ERROR_OK ) {
		printf( "connected to MySQL...\r\n" );
		g_pHandler = new udHandler( g_pMySQL );
		return true;
	}
		
	return false;
} // bool InitMySQL

void RepairPersAcc( ) {
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udBaseConfig config;
	udArrBase arrCompany, arr, arr1;
	udPBase company = NULL, obj = NULL, add = NULL;
	string szWhere = "", szIdxType = "", szIdxObj1 = "", szIdxObj2= "", szValType = "";
	udInt	count = 0, cnt_fiz = 0, cnt_ur = 0, cnt_partner = 0,
			cnta = 0, cnta_fiz = 0, cnta_ur = 0, cnta_partner = 0;
	obj = new udProjectPersAcc;
	obj->GetConfig( config );
	udDELETE( obj );
	szIdxType = config.GetAttrIndex( "type" );
	szIdxObj1 = config.GetAttrIndex( "obj1" );
	szIdxObj2 = config.GetAttrIndex( "obj2" );
	/**
		1. выгребаем компании.
		2. выгребаем клиентов (физ лица). для каждого проверяем наличие ЛС в каждой компании.
		3. выгребаем клиентов (юр. лица). для каждого проверяем наличие ЛС в каждой компании.
		4. выгребаем партнеров. для каждой компании проверяем наличие ЛС у каждого партнера.
	*/
	option.SetTable( "ud_company" );
	option.SetClass( GetProjectCompany );
	handler->GetUdObject( arrCompany, option );
	if ( !arrCompany.empty( ) ) {
		//////////////////////////////////////////////////////////////////////////////////////
		// клиент(физ.лицо)-компания
		option.Clear( );
		option.SetTable( "ud_client" );
		option.SetClass( GetProjectClient );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			option.Clear( );
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			szValType = toString( udProjectPersAcc::PERS_ACC_CLIENT_COMPANY );
			for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
				company = *i;
				for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
					obj = *j;
					szWhere = szIdxType + "=" + szValType + " AND " +
						szIdxObj1 + "=" + obj->get( "id" )->toString( ) + " AND " +
						szIdxObj2 + "=" + company->get( "id" )->toString( );
					option.SetWhere( szWhere );
					handler->GetUdObject( arr1, option );
					if ( arr1.empty( ) ) { // создать акк
						++count;
						++cnt_fiz;
						printf( "createing acc for fiz...\r\n" );
						add = new udProjectPersAcc;
						add->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENT_COMPANY );
						add->get( "obj1" )->ini( obj->get( "id" )->toUInt( ) );
						add->get( "obj2" )->ini( company->get( "id" )->toUInt( ) );
						arr1.push_back( add );
						option.SetWhere( "" );
						handler->AddUdObject( arr1, option );
					} else {
						++cnta;
						++cnta_fiz;
					}
					ClearArrBase( arr1 );
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		// клиент(юр.лицо)-компания
		option.Clear( );
		option.SetTable( "ud_clientur" );
		option.SetClass( GetProjectClientUr );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			option.Clear( );
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			szValType = toString( udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY );
			for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
				company = *i;
				for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
					obj = *j;
					szWhere = szIdxType + "=" + szValType + " AND " +
						szIdxObj1 + "=" + obj->get( "id" )->toString( ) + " AND " +
						szIdxObj2 + "=" + company->get( "id" )->toString( );
					option.SetWhere( szWhere );
					handler->GetUdObject( arr1, option );
					if ( arr1.empty( ) ) { // создать акк
						++count;
						++cnt_ur;
						printf( "createing acc for ur...\r\n" );
						add = new udProjectPersAcc;
						add->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY );
						add->get( "obj1" )->ini( obj->get( "id" )->toUInt( ) );
						add->get( "obj2" )->ini( company->get( "id" )->toUInt( ) );
						arr1.push_back( add );
						option.SetWhere( "" );
						handler->AddUdObject( arr1, option );
					} else {
						++cnta;
						++cnta_ur;
					}
					ClearArrBase( arr1 );
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		// компания-партнер
		option.Clear( );
		option.SetTable( "ud_partner" );
		option.SetClass( GetProjectPartner );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			option.Clear( );
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			szValType = toString( udProjectPersAcc::PERS_ACC_COMPANY_PARTNER );
			for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
				company = *i;
				for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
					obj = *j;
					szWhere = szIdxType + "=" + szValType + " AND " +
						szIdxObj1 + "=" + company->get( "id" )->toString( ) + " AND " +
						szIdxObj2 + "=" + obj->get( "id" )->toString( );
					option.SetWhere( szWhere );
					handler->GetUdObject( arr1, option );
					if ( arr1.empty( ) ) { // создать акк
						++count;
						++cnt_partner;
						printf( "createing acc for company...\r\n" );
						add = new udProjectPersAcc;
						add->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_COMPANY_PARTNER );
						add->get( "obj1" )->ini( company->get( "id" )->toUInt( ) );
						add->get( "obj2" )->ini( obj->get( "id" )->toUInt( ) );
						arr1.push_back( add );
						option.SetWhere( "" );
						handler->AddUdObject( arr1, option );
					} else {
						++cnta;
						++cnta_partner;
					}
					ClearArrBase( arr1 );
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		printf(
			"----- final information -----\r\nnumber of created accounts is %d\r\n"
			"for client ur %d\r\n"
			"for client fiz %d\r\n"
			"for company %d\r\n"
			"before create: total number %d\r\n"
			"client ur %d\r\n"
			"client fiz %d\r\n"
			"company %d\r\n",
			count, cnt_ur, cnt_fiz, cnt_partner,
			cnta, cnta_ur, cnta_fiz, cnta_partner
		);
		//////////////////////////////////////////////////////////////////////////////////////
	}
	ClearArrBase( arrCompany );
} // void RepairPersAcc
