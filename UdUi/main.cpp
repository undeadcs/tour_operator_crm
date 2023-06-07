
#include "includes.h"

namespace UdSDK {

	udPMorph	g_pConfig	= NULL;
	udPMySql	g_pMySQL	= NULL;

	void UdBaseEscapeString( char* out, const char* in, size_t len ) {
		g_pMySQL->EscapeString( out, in, len );
	} // void UdBaseEscapeString

} // namespace UdSDK

using namespace UdSDK;

#define TOURIST_APPLICATION_CURRENT_VERSION	20100831

/**
	открываем файлик с версией.
	скачиваем число.
	сравниваем с текущей.
	если разные, делаем зашитое обновление
*/

void		WriteVersion( );
udLongLong	ReadVersion( );
bool		InitConfig( );
bool		InitMySQL( );

void UpdateTo20100831( );

int main( int arc, char** argv ) {
	udLongLong	lCurVer		= ( udLongLong ) TOURIST_APPLICATION_CURRENT_VERSION,
				lVersion	= ReadVersion( );
	if ( ( lCurVer > lVersion ) && InitConfig( ) && InitMySQL( ) ) {
		UpdateTo20100831( );
		WriteVersion( );
	}
	udDELETE( g_pConfig );
	udDELETE( g_pMySQL );
	printf( "Press any key to exit." );
	getchar( );
	return 0;
} // int main

/**
	лс - лицевой счет
	сносим таблицу лс.
	создаем таблицу лс с новой структурой.
	выгребаем компании, клиентов, партнеров.
	наполняем таблицу лс данными.
	создаем таблицы сумм, операций с лс.
*/
void UpdateTo20100831( ) {
	udPHandler handler = new udHandler( g_pMySQL );
	udHandlerOption option;
	udBaseConfig config;
	udArrBase arrCompany;
	// сносим таблицу лс
	g_pMySQL->Query( "DROP TABLE `ud_persacc`" );
	// создаем таблицу снова
	option.SetTable( "ud_persacc" );
	option.SetClass( GetProjectPersAcc );
	handler->CheckTable( option );
	option.Clear( );
	// выгребаем компании
	printf( "loading companies...\r\n" );
	option.SetTable( "ud_company" );
	option.SetClass( GetProjectCompany );
	handler->GetUdObject( arrCompany, option );
	if ( !arrCompany.empty( ) ) {
		udArrBase arr, arr1;
		udPBase obj = NULL;
		unsigned count = 0;
		// компания-партнер
		printf( "loading partners...\r\n" );
		option.SetTable( "ud_partner" );
		option.SetClass( GetProjectPartner );
		handler->GetUdObject( arr, option );
		option.Clear( );
		for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
			for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
				obj = new udProjectPersAcc;
				obj->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_COMPANY_PARTNER );
				// компания
				obj->get( "obj1" )->ini( ( *i )->get( "id" )->toUInt( ) );
				// партнер
				obj->get( "obj2" )->ini( ( *j )->get( "id" )->toUInt( ) );
				arr1.push_back( obj );
			}
		}
		option.SetTable( "ud_persacc" );
		option.SetClass( GetProjectPersAcc );
		handler->AddUdObject( arr1, option );
		count += arr1.size( );
		option.Clear( );
		ClearArrBase( arr );
		ClearArrBase( arr1 );
		// физ.лицо-компания
		printf( "loading clients...\r\n" );
		option.SetTable( "ud_client" );
		option.SetClass( GetProjectClient );
		handler->GetUdObject( arr, option );
		option.Clear( );
		for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
			for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
				obj = new udProjectPersAcc;
				obj->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENT_COMPANY );
				// клиент
				obj->get( "obj1" )->ini( ( *j )->get( "id" )->toUInt( ) );
				// компания
				obj->get( "obj2" )->ini( ( *i )->get( "id" )->toUInt( ) );
				arr1.push_back( obj );
			}
		}
		option.SetTable( "ud_persacc" );
		option.SetClass( GetProjectPersAcc );
		handler->AddUdObject( arr1, option );
		count += arr1.size( );
		option.Clear( );
		ClearArrBase( arr );
		ClearArrBase( arr1 );
		// юр.лицо-компания
		printf( "loading clients(ur)...\r\n" );
		option.SetTable( "ud_clientur" );
		option.SetClass( GetProjectClientUr );
		handler->GetUdObject( arr, option );
		option.Clear( );
		for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
			for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
				obj = new udProjectPersAcc;
				obj->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY );
				// клиент
				obj->get( "obj1" )->ini( ( *j )->get( "id" )->toUInt( ) );
				// компания
				obj->get( "obj2" )->ini( ( *i )->get( "id" )->toUInt( ) );
				arr1.push_back( obj );
			}
		}
		option.SetTable( "ud_persacc" );
		option.SetClass( GetProjectPersAcc );
		handler->AddUdObject( arr1, option );
		count += arr1.size( );
		option.Clear( );
		ClearArrBase( arr );
		ClearArrBase( arr1 );
		//
		ClearArrBase( arrCompany );
		printf( "total accounts added: %u\n", count );
	}
	udDELETE( handler );
} // void UpdateTo20100831







udLongLong ReadVersion( ) {
	FILE *fileVersion = NULL;

	if ( fopen_s( &fileVersion, "version.txt", "rb" ) == 0 ) {
		string version = "";
		char ch = fgetc( fileVersion );
		while( !feof( fileVersion ) ) {
			version += ch;
			ch = fgetc( fileVersion );
		}
		fclose( fileVersion );
		return toLLong( version.data( ) );
	}

	return 0;
} // udLongLong ReadVersion

void WriteVersion( ) {
	FILE *fileVersion = NULL;
	if ( fopen_s( &fileVersion, "version.txt", "wb" ) == 0 ) {
		string version = toString( TOURIST_APPLICATION_CURRENT_VERSION );
		fwrite( version.data( ), 1, version.size( ), fileVersion );
		fclose( fileVersion );
	}
} // void WriteVersion

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
		return true;
	}
		
	return false;
} // bool InitMySQL