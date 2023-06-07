
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

//#define TOURIST_APPLICATION_CURRENT_VERSION	20100831
//#define TOURIST_APPLICATION_CURRENT_VERSION	20100903
//#define TOURIST_APPLICATION_CURRENT_VERSION	20100914
//#define TOURIST_APPLICATION_CURRENT_VERSION	20100924
//#define TOURIST_APPLICATION_CURRENT_VERSION	20101014
//#define TOURIST_APPLICATION_CURRENT_VERSION	20101020
//#define TOURIST_APPLICATION_CURRENT_VERSION	20101026
//#define TOURIST_APPLICATION_CURRENT_VERSION	20110120
//#define TOURIST_APPLICATION_CURRENT_VERSION	20110724
//#define TOURIST_APPLICATION_CURRENT_VERSION	20110725
//#define TOURIST_APPLICATION_CURRENT_VERSION	20110726
//#define TOURIST_APPLICATION_CURRENT_VERSION	20110811
//#define TOURIST_APPLICATION_CURRENT_VERSION	20110817
//#define TOURIST_APPLICATION_CURRENT_VERSION	20110914
//#define TOURIST_APPLICATION_CURRENT_VERSION	20111122
#define TOURIST_APPLICATION_CURRENT_VERSION 20111204

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
void UpdateTo20100903( );
void UpdateTo20100914( );
void UpdateTo20100924( );
void UpdateTo20101014( );
void UpdateTo20101020( );
void UpdateTo20101026( );
void UpdateTo20110120( );
void UpdateTo20110724( );
void UpdateTo20110725( );
void UpdateTo20110726( );
void UpdateTo20110811( );
void UpdateTo20110817( );
void UpdateTo20110914( );
void UpdateTo20111122( );
void UpdateTo20111204( );

int main( int arc, char** argv ) {
	udLongLong	lCurVer		= ( udLongLong ) TOURIST_APPLICATION_CURRENT_VERSION,
				lVersion	= ReadVersion( );
	if ( ( lCurVer > lVersion ) && InitConfig( ) && InitMySQL( ) ) {
		//UpdateTo20100831( );
		//UpdateTo20100903( );
		//UpdateTo20100914( );
		//UpdateTo20100924( );
		//UpdateTo20101014( );
		//UpdateTo20101020( );
		//UpdateTo20101026( );
		//UpdateTo20110120( );
		//UpdateTo20110724( );
		//UpdateTo20110725( );
		//UpdateTo20110726( );
		//UpdateTo20110811( );
		//UpdateTo20110817( );
		//UpdateTo20110914( );
		//UpdateTo20111122( );
		UpdateTo20111204( );
		WriteVersion( );
	}
	udDELETE( g_pConfig );
	udDELETE( g_pHandler );
	udDELETE( g_pMySQL );
	printf( "Press any key to exit." );
	getchar( );
	return 0;
} // int main

void UpdateTo20111204( ) {
	printf( "updating to 20111204\r\n" );

	if ( g_pMySQL->Query( "ALTER TABLE `ud_group_pay_client` ADD COLUMN `client_doc` INT UNSIGNED" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20111204

void UpdateTo20111122( ) {
	printf( "updating to 20111122\r\n" );

	if ( g_pMySQL->Query( "ALTER TABLE `ud_client` ADD COLUMN `client_doc` INT UNSIGNED" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}

	if ( g_pMySQL->Query( "ALTER TABLE `ud_cldoc` ADD COLUMN `cldoc_is_default` INT UNSIGNED" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20111120

void UpdateTo20110914( ) {
	printf( "updating to 20110914\r\n" );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf( "alter group period table... " );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_groupperiod` ADD COLUMN `groupperiod_date2` DATETIME" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
	if ( g_pMySQL->Query( "UPDATE TABLE `ud_groupperiod` SET `groupperiod_date2`=NULL" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
	if ( g_pMySQL->Query( "ALTER TABLE `ud_groupperiod` ADD COLUMN `groupperiod_client` INT UNSIGNED" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
	if ( g_pMySQL->Query( "UPDATE TABLE `ud_groupperiod` SET `groupperiod_client`=0" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf( "repearing service price table... " );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udBaseConfig config;
	udArrBase arr;
	udPBase obj = NULL;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	obj = new udProjectPriceTableItem;
	obj->GetConfig( config );
	udDELETE( obj );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	option.SetTable( "ud_pricetableitem" );
	option.SetClass( GetProjectPriceTableItem );
	handler->GetUdObject( arr, option );
	if ( !arr.empty( ) ) {
		udFloat value = 0.0f;
		udArrBase arrUpd;

		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			value = obj->get( "amount_recommend" )->toFloat( );
			if ( value < 0.0f ) {
				obj->get( "amount_recommend" )->ini( ( udFloat ) 0.0f );
				arrUpd.push_back( obj );
			}
		}

		if ( !arrUpd.empty( ) ) {
			option.SetIndexAttr( "id" );
			handler->UpdUdObject( arrUpd, option );
			arrUpd.clear( );
		}

		ClearArrBase( arr );
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
} // void UpdateTo20110914

void UpdateTo20110817( ) {
	printf( "updating to 20110817\r\n" );

	printf( "alter service price table... " );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_pricetableitem` ADD COLUMN `pricetableitem_amount_recommend` FLOAT" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
	if ( g_pMySQL->Query( "ALTER TABLE `ud_pricetableitem` ADD COLUMN `pricetableitem_currency_recommend` INT UNSIGNED" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20110817

void UpdateTo20110811( ) {
	printf( "updating to 20110811\r\n" );

	printf( "alter tour table... " );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_tour` ADD COLUMN `tour_document` INT UNSIGNED AFTER `tour_emptype`" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}

	printf( "alter order table... " );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_order` ADD COLUMN `order_document` INT UNSIGNED AFTER `order_emptype`" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}

	printf( "alter tour application table... " );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_gcapp1` ADD COLUMN `gcapp1_price` VARCHAR(255) AFTER `gcapp1_tour`" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20110811

void UpdateTo20110726( ) {
	printf( "updating to UpdateTo20110726\r\n" );

	if ( g_pMySQL->Query( "ALTER TABLE `ud_client` ADD COLUMN `client_bornplacelatin` VARCHAR(255) AFTER `client_bornplace`" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20110726

void UpdateTo20110725( ) {
	printf( "updating to UpdateTo20110725\r\n" );

	printf( "altering client table\r\n" );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_client` ADD COLUMN `client_bornplace` VARCHAR(255) AFTER `client_born`" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
	// FAIL
	if ( g_pMySQL->Query( "ALTER TABLE `ud_client` ADD COLUMN `ud_client` ADD COLUMN `client_bornplacelatin` VARCHAR(255) AFTER `client_bornplace`" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20110725

void UpdateTo20110724( ) {
	printf( "updating to UpdateTo20110724\r\n" );

	// тут вылазит ошибка
	printf( "altering client table\r\n" );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_client` ADD COLUMN `client_bornplace` VARCHAR(255), `ud_client` ADD COLUMN `client_bornplacelatin` VARCHAR(255)" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}

	printf( "altering host side table\r\n" );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_hostside` ADD COLUMN `hostside_namelatin` VARCHAR(255)" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}

	printf( "altering route table\r\n" );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_route` ADD COLUMN `route_namelatin` VARCHAR(255)" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}

	printf( "altering transfer point table\r\n" );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_transferpoint` ADD COLUMN `transferpoint_namelatin` VARCHAR(255)" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20110724

/**
 *	добавляем колонку в таблицу пользователя
 */
void UpdateTo20110120( ) {
	printf( "updating to 20110120\r\n" );
	if ( g_pMySQL->Query( "ALTER TABLE `ud_user` ADD COLUMN `user_series` VARCHAR(255)" ) ) {
		printf( "done\r\n" );
	} else {
		printf( "failed, error: %s\r\n", g_pMySQL->GetErrorMessage( ).data( ) );
	}
} // void UpdateTo20110120

/**
	добавляем колонку в таблицу документа клиента
*/
void UpdateTo20101026( ) {
	g_pMySQL->Query( "ALTER TABLE `ud_cldoc` ADD COLUMN `cldoc_fiolatin` VARCHAR(255)" );
} // void UpdateTo20101026

/**
	1. сносим все суммы себестоимости. (сносим связи сумм и партнеров по себестоимости)
	2. считаем себестоимость по новой.
*/
void UpdateTo20101020( ) {
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udBaseConfig config;
	udArrBase arr;
	udPBase obj = NULL;
	//////////////////////////////////////////////////////////////////////////////////////////////
	// сносим суммы по себестоимости
	obj = new udProjectFinanceSumm;
	obj->GetConfig( config );
	udDELETE( obj );
	option.SetTable( "ud_fsumm" );
	option.SetClass( GetProjectFinanceSumm );
	option.SetWhere( config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourCost ) );
	handler->GetUdObject( arr, option );
	if ( !arr.empty( ) ) {
		udArrString arrString;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			arrString.push_back( ( *i )->get( "id" )->toString( ) );
		}
		config.Clear( );
		obj = new udProjectFinanceSummToPartner;
		obj->GetConfig( config );
		udDELETE( obj );
		// сносим
		g_pMySQL->Query( "DELETE FROM `ud_fsummtopartner` WHERE " + config.GetAttrIndex( "summ_id" ) + " IN(" + Join( ",", arrString ) + ")" );
		g_pMySQL->Query( "DELETE FROM `ud_fsumm` WHERE " + config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourCost ) );
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	// считаем себестоимость туров
	UpdateTo20100924( );
	//////////////////////////////////////////////////////////////////////////////////////////////
} // void UpdateTo20101020

/**
	1. выгребаем всех клиентов старой структуры.
	2. дропаем старую таблицу клиентов.
	3. создаем новую таблицу клиентов.
	4. сменяем тип атрибута даты рождения у каждого клиента.
	5. добавляем клиентов.
*/
udPBase GetOldClient( ) {
	udPBase ret = GetProjectClient( );
	ret->get( "born" )->ini( datetime( ) );
	return ret;
} // udPBase GetOldClient

void UpdateTo20101014( ) {
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udArrBase arr;
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 1
	option.SetTable( "ud_client" );
	option.SetClass( GetOldClient );
	handler->GetUdObject( arr, option );
	if ( !arr.empty( ) ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// 2
		g_pMySQL->Query( "DROP TABLE IF EXISTS `ud_client`" );
		//////////////////////////////////////////////////////////////////////////////////////////
		// 3
		option.SetTable( "ud_client" );
		option.SetClass( GetProjectClient );
		handler->CheckTable( option );
		option.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// 4
		udPBase obj = NULL;
		wstring born = L"", tmp = L"";
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			born = obj->get( "born" )->toWString( );
			tmp = born.substr( 0, 10 );
			obj->get( "born" )->ini( tmp );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		// 5
		option.SetTable( "ud_client" );
		handler->AddUdObject( arr, option );
		option.Clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
} // void UpdateTo20101014

void UpdateTo20100914( ) {
	// дропаем таблицы для финансов ud_fsumm, ud_fop
	g_pMySQL->Query( "DROP TABLE IF EXISTS `ud_fsumm`" );
	g_pMySQL->Query( "DROP TABLE IF EXISTS `ud_fop`" );
	// создаем их с новой структурой
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	// суммы
	option.SetTable( "ud_fsumm" );
	option.SetClass( GetProjectFinanceSumm );
	handler->CheckTable( option );
	// операции
	option.SetTable( "ud_fop" );
	option.SetClass( GetProjectFinanceOperation );
	handler->CheckTable( option );
} // void UpdateTo20100914

/**
	добавляем в базу пользователя 1/1
*/
void UpdateTo20100903( ) {
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udArrBase arr;
	udPBase obj = new udProjectUser;
	obj->get( "login" )->ini( wstring( L"1" ) );
	obj->get( "password" )->ini( wstring( L"1" ) );
	obj->get( "email" )->ini( wstring( L"1@1.1" ) );
	obj->get( "fio" )->ini( wstring( L"1" ) );
	obj->get( "rank" )->ini( ( udInt ) udProjectUser::USERRANK_ADMINISTRATOR );
	arr.push_back( obj );
	option.SetTable( "ud_user" );
	handler->AddUdObject( arr, option );
	ClearArrBase( arr );
} // void UpdateTo20100903

/**
	лс - лицевой счет
	сносим таблицу лс.
	создаем таблицу лс с новой структурой.
	выгребаем компании, клиентов, партнеров.
	наполняем таблицу лс данными.
	создаем таблицы сумм, операций с лс.
*/
void UpdateTo20100831( ) {
	udPHandler handler = g_pHandler;
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
	if ( fopen_s( &config, "client_config.txt", "rb" ) == 0 ) {
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
	} else {
		STARTUPINFO			objSI;
		PROCESS_INFORMATION	objPI;
		g_pMySQL->Close( );
		CreateProcess(
			szPath.data( ), NULL, NULL, NULL, FALSE,
			DETACHED_PROCESS,
			NULL, NULL, &objSI, &objPI
		);
		CloseHandle( objPI.hProcess );
		CloseHandle( objPI.hThread );
		objPI.hProcess = NULL;
		objPI.hThread = NULL;
		Sleep( 3000 );
		if ( g_pMySQL->Connect( true ) == udMySql::UDSDK_MYSQL_ERROR_OK ) {
			return true;
		}
		printf( "connection failed\r\n" );
	}
		
	return false;
} // bool InitMySQL




static udInt FindPeriod( udArrBase& arrTourService, udArrBase& arrServicePeriod, udArrBase& arrResult ) {
	udInt		found	= 0,		// количество найденных
				k		= -1;		// индекс найденного периода
	udPBase		s		= NULL,		// указатель на услугу тура
				p		= NULL,		// указатель на период услуги
				add		= NULL;		// новый объект
	datetime	sd1, sd2,			// даты оказания услуги
				pd1, pd2,			// даты периода
				tmp;				// максимальная (минимальная) дата слева (справа)
	udUInt		id		= 0;		// ID услуги
	
	for( size_t i = 0; i < arrTourService.size( ); ++i ) {
		s	= arrTourService[ i ];
		id	= s->get( "service" )->toUInt( );
		k	= -1;
		sd1	= s->get( "date1" )->toTime( );
		sd2	= s->get( "date2" )->toTime( );
		tmp.set( 0 ); // зануляем дату
		// пробегаемся по периодам
		for( size_t j = 0; j < arrServicePeriod.size( ); ++j ) {
			p = arrServicePeriod[ j ];
			if ( p->get( "service" )->toUInt( ) == id ) { // период текущей услуги
				pd1 = p->get( "date1" )->toTime( );
				pd2 = p->get( "date2" )->toTime( );
				if ( ( pd1 <= sd1 ) && ( pd1 > tmp ) ) {
					tmp = pd1;
					k = j;
				}
			}
		}
		// подводим итог пробежки по периодам
		if ( tmp.get( ) == 0 ) { // дат слева не найдено, ищем справа
			for( size_t j = 0; j < arrServicePeriod.size( ); ++j ) {
				p = arrServicePeriod[ j ];
				if ( p->get( "service" )->toUInt( ) == id ) { // период текущей услуги
					pd1 = p->get( "date1" )->toTime( );
					pd2 = p->get( "date2" )->toTime( );
					if ( ( pd1 >= sd1 ) && ( pd1 < tmp ) ) {
						tmp = pd1;
						k = j;
					}
				}
			}
		}
		if ( tmp.get( ) != 0 ) { // в итоге был найден период
			p = arrServicePeriod[ k ];
			add = new udProjectServicePeriod;
			p->apply( add );
			arrResult.push_back( add );
			++found;
		}
	}
	return found;
} // udInt FindPeriod

static udInt FindPrice( udUInt num, udArrBase& arrServicePeriod, udArrBase& arrServicePrice, udArrBase& arrResult ) {
	udInt	found		= 0,	// количество найденных
			k			= -1;	// индекс найденной цены
	udPBase	p			= NULL,	// указатель на период
			r			= NULL,	// указатель на цену
			add			= NULL;	// указатель на объект для добавления
	udUInt	rn			= 0,	// количество людей в цене
			tmp			= 0,	// временное искомое количество
			iPeriod		= 0,	// индекс периода
			iService	= 0;	// ID услуги
	// пробегаемся по периодам
	for( size_t i = 0; i < arrServicePeriod.size( ); ++i ) {
		p = arrServicePeriod[ i ];
		k = -1;
		tmp = 0;
		iPeriod = p->get( "index" )->toUInt( );
		iService = p->get( "service" )->toUInt( );
		// пробегаемся по ценам
		for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
			r = arrServicePrice[ j ];
			if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
				rn = r->get( "num" )->toUInt( );
				if ( ( rn <= num ) && ( rn > tmp ) ) {
					tmp = rn;
					k = j;
				}
			}
		}
		// подводим итоги
		if ( tmp == 0 ) {
			for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
				r = arrServicePrice[ j ];
				if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
					rn = r->get( "num" )->toUInt( );
					if ( ( rn >= num ) && ( rn < tmp ) ) {
						tmp = rn;
						k = j;
					}
				}
			}
		}
		if ( tmp != 0 ) {
			r = arrServicePrice[ k ];
			add = new udProjectPriceTableItem;
			r->apply( add );
			arrResult.push_back( add );
			++found;
		}
	}
	return found;
} // udInt FindPrice

static udInt FindPrice( map< udUInt, udUInt > mapServiceAmount, udArrBase& arrServicePeriod, udArrBase& arrServicePrice, udArrBase& arrResult ) {
	udInt	found		= 0,	// количество найденных
			k			= -1;	// индекс найденной цены
	udPBase	p			= NULL,	// указатель на период
			r			= NULL,	// указатель на цену
			add			= NULL;	// указатель на объект для добавления
	udUInt	rn			= 0,	// количество людей в цене
			tmp			= 0,	// временное искомое количество
			iPeriod		= 0,	// индекс периода
			iService	= 0,	// ID услуги
			num			= 0;
	map< udUInt, udUInt >::iterator itAmount;
	// пробегаемся по периодам
	for( size_t i = 0; i < arrServicePeriod.size( ); ++i ) {
		p = arrServicePeriod[ i ];
		k = -1;
		tmp = 0;
		iPeriod = p->get( "index" )->toUInt( );
		iService = p->get( "service" )->toUInt( );
		itAmount = mapServiceAmount.find( iService );
		if ( itAmount != mapServiceAmount.end( ) ) {
			num = itAmount->second;
			// пробегаемся по ценам
			for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
				r = arrServicePrice[ j ];
				if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
					rn = r->get( "num" )->toUInt( );
					if ( ( rn <= num ) && ( rn > tmp ) ) {
						tmp = rn;
						k = j;
					}
				}
			}
			// подводим итоги
			if ( tmp == 0 ) {
				for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
					r = arrServicePrice[ j ];
					if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
						rn = r->get( "num" )->toUInt( );
						if ( ( rn >= num ) && ( rn < tmp ) ) {
							tmp = rn;
							k = j;
						}
					}
				}
			}
			if ( tmp != 0 ) {
				r = arrServicePrice[ k ];
				add = new udProjectPriceTableItem;
				r->apply( add );
				arrResult.push_back( add );
				++found;
			}
		}
	}
	return found;
} // udInt FindPrice

static void LoadPriceByPeriod( udArrBase& arrPeriod, udArrBase& arrResult, udPHandler handler ) {
	udHandlerOption option;
	udBaseConfig config;
	udPBase obj = NULL;
	udArrError err;
	udArrString arrString;
	string szWhere = "";
	//////////////////////////////////////////////////////////////////////////////////////////////
	obj = new udProjectPriceTableItem;
	obj->GetConfig( config );
	udDELETE( obj );
	for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
		obj = *i;
		arrString.push_back(
			"(" +
			config.GetAttrIndex( "service" ) + "=" + obj->get( "service" )->toString( ) +
			" AND " +
			config.GetAttrIndex( "period" ) + "=" + obj->get( "index" )->toString( ) +
			")"
		);
	}
	szWhere = Join( " OR ", arrString );
	option.SetTable( "ud_pricetableitem" );
	option.SetClass( GetProjectPriceTableItem );
	option.SetWhere( szWhere );
	handler->GetUdObject( arrResult, option, &err );
	//////////////////////////////////////////////////////////////////////////////////////////////
} // void LoadPriceByPeriod

// глупо, но сделаем пока так
static udUInt CountTourTourist( udUInt id ) {
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udBaseConfig config;
	udArrBase arr;
	udPBase obj = NULL;
	udUInt ret = 0;
	obj = new udProjectTourClient;
	obj->GetConfig( config );
	udDELETE( obj );
	option.SetTable( "ud_tourclient" );
	option.SetClass( GetProjectTourClient );
	option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + toString( id ) );
	handler->GetUdObject( arr, option );
	ret = arr.size( );
	ClearArrBase( arr );
	return ret;
} // udUInt CountTourTourist

// собирает цены услуг в единый набор сумм
static udUInt CollectTourCost( udPBase tour, udArrBase& arrPrice, udArrBase& arrResult ) {
	udUInt currency_num = 0, iCurrency = 0;
	udPBase obj = NULL, add = NULL;
	udFloat s1 = 0.0f, s2 = 0.0f;
	map< udUInt, udPBase > mapSumm;
	map< udUInt, udPBase >::iterator itSumm;
	//
	for( udArrBase::iterator i = arrPrice.begin( ); i != arrPrice.end( ); ++i ) {
		obj = *i;
		iCurrency = obj->get( "currency" )->toUInt( );
		itSumm = mapSumm.find( iCurrency );
		if ( itSumm == mapSumm.end( ) ) {
			add = new udProjectFinanceSumm;
			add->get( "summ" )->ini( obj->get( "amount" )->toFloat( ) );
			add->get( "currency" )->ini( iCurrency );
			add->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtTourCost );
			add->get( "obj" )->ini( tour->get( "id" )->toUInt( ) );
			mapSumm[ iCurrency ] = add;
			++currency_num;
		} else {
			s1 = itSumm->second->get( "summ" )->toFloat( );
			s2 = obj->get( "amount" )->toFloat( );
			itSumm->second->get( "summ" )->ini( s1 + s2 );
		}
	}
	//
	for( map< udUInt, udPBase >::iterator i = mapSumm.begin( ); i != mapSumm.end( ); ++i ) {
		arrResult.push_back( i->second );
	}
	mapSumm.clear( );
	//
	return currency_num;
} // udUInt CollectTourCost

static void CountServiceAmount( udUInt iGroupId, udPBase tour, udPBase pGroupTour, udArrBase& arrPeriod, map< udUInt, udUInt >& mapServiceAmount ) {
	//////////////////////////////////////////////////////////////////////////////////////////////
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udBaseConfig config;
	udArrBase arr;
	udArrError err;
	udPBase obj = NULL;
	udUInt iServiceId = 0;
	string szWhere = "", log = "";
	//////////////////////////////////////////////////////////////////////////////////////////////
	obj = new udProjectGroupTour;
	obj->GetConfig( config );
	udDELETE( obj );
	// входят в ту же группу и в тот же период
	szWhere = config.GetAttrIndex( "group" ) + "=" + toString( iGroupId ) + " AND " +
		config.GetAttrIndex( "period" ) + "=" + pGroupTour->get( "period" )->toString( );
	option.SetTable( "ud_grouptour" );
	option.SetClass( GetProjectGroupTour );
	handler->GetUdObject( arr, option, &err );
	if ( !err.empty( ) ) {
		//WriteErrors( __log, err );
	} else if ( !arr.empty( ) ) {
		/////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		// наполняем спектр услуг нулями
		for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
			obj = *i;
			iServiceId = obj->get( "service" )->toUInt( );
			mapServiceAmount[ iServiceId ] = 0;
			arrString.push_back( toString( iServiceId ) );
		}
		string szIn = "IN (" + Join( ",", arrString ) + ")";
		arrString.clear( );
		/////////////////////////////////////////////////////////////////////////////////////////
		// выгребаем услуги, которые вошли в другие туры
		udUInt iTourId = 0;
		map< udUInt, udUInt > mapTourId; // ID туров, а вдруг кто-то воткнет их несколько
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			iTourId = obj->get( "tour" )->toUInt( );
			mapTourId[ iTourId ] = iTourId;
		}
		udArrBase arrService;
		// для каждого тура выгребаем услуги, которые попадают в спект и принадлежат туру
		option.Clear( );
		option.SetTable( "ud_tourservice" );
		option.SetClass( GetProjectTourService );
		config.Clear( );
		obj = new udProjectTourService;
		obj->GetConfig( config );
		udDELETE( obj );
		udUInt iTouristNum = CountTourTourist( tour->get( "id" )->toUInt( ) );
		for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
			obj = *i;
			iServiceId = obj->get( "service" )->toUInt( );
			mapServiceAmount[ iServiceId ] = iTouristNum;
		}
		map< udUInt, udUInt >::iterator itService;
		for( map< udUInt, udUInt >::iterator i = mapTourId.begin( ); i != mapTourId.end( ); ++i ) {
			szWhere = config.GetAttrIndex( "tour" ) + "=" + toString( i->first ) + " AND " +
				config.GetAttrIndex( "service" ) + szIn;
			option.SetWhere( szWhere );
			handler->GetUdObject( arrService, option, &err );
			if ( !err.empty( ) ) {
				//WriteErrors( __log, err );
			} else if ( !arrService.empty( ) ) {
				iTouristNum = CountTourTourist( i->first );
				for( udArrBase::iterator j = arrService.begin( ); j != arrService.end( ); ++j ) {
					obj = *j;
					iServiceId = obj->get( "service" )->toUInt( );
					itService = mapServiceAmount.find( iServiceId );
					if ( itService != mapServiceAmount.end( ) ) {
						itService->second += iTouristNum;
					}
				}
			}
			ClearArrBase( arrService );
		}
		/////////////////////////////////////////////////////////////////////////////////////////
	} else { // тур в группе один, для каждой услуги втыкаем количество человек данного тура
		udUInt iTouristNum = CountTourTourist( tour->get( "id" )->toUInt( ) );
		for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
			obj = *i;
			iServiceId = obj->get( "service" )->toUInt( );
			mapServiceAmount[ iServiceId ] = iTouristNum;
		}
	}
	ClearArrBase( arr );
	//////////////////////////////////////////////////////////////////////////////////////////////
} // void CountServiceAmount

void Cost( udPBase obj ) {
	//////////////////////////////////////////////////////////////////////////////////////////////
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udBaseConfig config;
	udPBase obj1 = NULL;
	udArrBase arr;
	udArrError err;
	string szWhere = "", log = "";
	udArrString arrString;
	//////////////////////////////////////////////////////////////////////////////////////////////
	// проверяем, что тур входит в какую то группу
	obj1 = new udProjectGroupTour;
	obj1->GetConfig( config );
	udDELETE( obj1 );
	szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
	option.SetTable( "ud_grouptour" );
	option.SetClass( GetProjectGroupTour );
	option.SetWhere( szWhere );
	handler->GetUdObject( arr, option, &err );
	if ( !err.empty( ) ) {
		//WriteErrors( __log, err );
	} else if ( arr.empty( ) ) { // не состоит в группе
		udArrBase arrTourService;
		config.Clear( );
		obj1 = new udProjectTourService;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
		option.SetTable( "ud_tourservice" );
		option.SetClass( GetProjectTourService );
		option.SetWhere( szWhere );
		handler->GetUdObject( arrTourService, option, &err );
		if ( !err.empty( ) ) {
			//WriteErrors( __log, err );
		} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
			udArrBase arrServicePeriod; // периоды услуг
			for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
				arrString.push_back( ( *i )->get( "service" )->toString( ) );
			}
			config.Clear( );
			obj1 = new udProjectServicePeriod;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
			option.SetTable( "ud_serviceperiod" );
			option.SetClass( GetProjectServicePeriod );
			option.SetWhere( szWhere );
			handler->GetUdObject( arrServicePeriod, option, &err );
			if ( !err.empty( ) ) {
				//WriteErrors( __log, err );
			} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
				udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
				if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
					// выгребаем цены по периодам
					udUInt num = CountTourTourist( obj->get( "id" )->toUInt( ) );
					udArrBase arrPriceToTake, arrPrice;
					// подгружаем цены по услугам
					LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
					// находим цены по каждой услуге (период+количество человек)
					if ( FindPrice( num, arrPeriodToTake, arrPriceToTake, arrPrice ) > 0 ) {
						udArrBase arrSumm;
						if ( CollectTourCost( obj, arrPrice, arrSumm ) > 0 ) {
							// сносим старые
							g_pMySQL->Query(
								"DELETE FROM `ud_fsumm` WHERE "
								"`fsumm_at`=" + toString( udProjectFinanceSumm::SummAtTourCost ) + " AND "
								"`fsumm_obj`=" + obj->get( "id" )->toString( )
							);
							// добавляем суммы
							option.Clear( );
							option.SetTable( "ud_fsumm" );
							handler->AddUdObject( arrSumm, option, &err );
						}
						ClearArrBase( arrSumm );
					}
					ClearArrBase( arrPriceToTake );
					ClearArrBase( arrPrice );
				}
				ClearArrBase( arrPeriodToTake );
			}
			ClearArrBase( arrServicePeriod );
		}
		ClearArrBase( arrTourService );
	} else { // состоит в какой-то группе
		udPBase pGroupTour = arr[ 0 ];
		udUInt iGroupId = pGroupTour->get( "group" )->toUInt( );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrBase arrTourService;
		config.Clear( );
		obj1 = new udProjectTourService;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
		option.SetTable( "ud_tourservice" );
		option.SetClass( GetProjectTourService );
		option.SetWhere( szWhere );
		handler->GetUdObject( arrTourService, option, &err );
		if ( !err.empty( ) ) {
			//WriteErrors( __log, err );
		} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
			udArrBase arrServicePeriod; // периоды услуг
			for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
				arrString.push_back( ( *i )->get( "service" )->toString( ) );
			}
			config.Clear( );
			obj1 = new udProjectServicePeriod;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
			option.SetTable( "ud_serviceperiod" );
			option.SetClass( GetProjectServicePeriod );
			option.SetWhere( szWhere );
			handler->GetUdObject( arrServicePeriod, option, &err );
			if ( !err.empty( ) ) {
				//WriteErrors( __log, err );
			} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
				udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
				if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
					// выгребаем цены по периодам
					map< udUInt, udUInt > mapServiceAmount;
					udUInt num = CountTourTourist( obj->get( "id" )->toUInt( ) );
					udArrBase arrPriceToTake, arrPrice;
					// подгружаем цены по услугам
					LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
					//////////////////////////////////////////////////////////////////////////
					// необходимо по каждой услуге просчитать количество человек пертендующих на нее
					CountServiceAmount( iGroupId, obj, pGroupTour, arrPeriodToTake, mapServiceAmount );
					//////////////////////////////////////////////////////////////////////////
					// находим цены по каждой услуге (период+количество человек)
					if ( FindPrice( mapServiceAmount, arrPeriodToTake, arrPriceToTake, arrPrice ) > 0 ) {
						udArrBase arrSumm;
						if ( CollectTourCost( obj, arrPrice, arrSumm ) > 0 ) {
							// сносим старые
							g_pMySQL->Query(
								"DELETE FROM `ud_fsumm` WHERE "
								"`fsumm_at`=" + toString( udProjectFinanceSumm::SummAtTourCost ) + " AND "
								"`fsumm_obj`=" + obj->get( "id" )->toString( )
							);
							// добавляем новые
							option.Clear( );
							option.SetTable( "ud_fsumm" );
							handler->AddUdObject( arrSumm, option, &err );
						}
						ClearArrBase( arrSumm );
					}
					ClearArrBase( arrPriceToTake );
					ClearArrBase( arrPrice );
				}
				ClearArrBase( arrPeriodToTake );
			}
			ClearArrBase( arrServicePeriod );
		}
		ClearArrBase( arrTourService );
		//////////////////////////////////////////////////////////////////////////////////////////
	}
	ClearArrBase( arr );
	//////////////////////////////////////////////////////////////////////////////////////////////
} // void Cost




void UpdateTo20100924( ) {
	udPHandler handler = g_pHandler;
	udHandlerOption option;
	udBaseConfig config;
	udArrBase arr;
	//
	option.SetTable( "ud_tour" );
	option.SetClass( GetProjectTour );
	handler->GetUdObject( arr, option );
	if ( !arr.empty( ) ) {
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			Cost( *i );
		}
	}
	ClearArrBase( arr );
} // void UpdateTo20100924