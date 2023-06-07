
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDatabaseSync::CDatabaseSync( ) {
	}

	CDatabaseSync::~CDatabaseSync( ) {
		if ( !m_arrTableInfo.empty( ) ) {
			ClearArrBase( m_arrTableInfo );
		}
		if ( !m_arrTableQuery.empty( ) ) {
			ClearArrBase( m_arrTableQuery );
		}
	}

	void CDatabaseSync::Init( ) {
		assert( g_pApp );

		udPHandler		handler = g_pApp->GetHandler( );
		udHandlerOption	option;

		assert( handler );

		option.SetTable( "ud_syncinfo" );
		option.SetClass( GetDatabaseSyncInfo );
		handler->CheckTable( option );

		option.SetTable( "ud_syncquery" );
		option.SetClass( GetDatabaseTableQuery );
		handler->CheckTable( option );

		udInt ret = this->LoadInfoData( );
		switch( ret ) {
			case -1:
				g_pApp->GetLog( )->Write( "В настройках синхронизатора отсутствуют таблицы\n" );
				break;

			case 0:
				g_pApp->GetLog( )->Write( "Возникли ошибки при загрузке метаданных синхронизатора\n" );
				break;

			case 1:
				this->CheckTables( );
				break;

			default:
				break;
		}
	} // void Init

	void CDatabaseSync::Sync( ) {
		g_pApp->GetLog( )->Write( "CDatabaseSync::Sync\n" );

		string log1;
		log1 += "всего метаданных: " + toString( m_arrTableInfo.size( ) ) + "\n";
		for( udArrBase::iterator i = m_arrTableInfo.begin( ); i != m_arrTableInfo.end( ); ++i ) {
			( *i )->dump( log1 );
			log1 += "\n";
		}
		g_pApp->GetLog( )->Write( log1 );

		udPAppConfig	config	= g_pApp->GetConfig( );
		string			name	= "country",
						host	= config->get( "server_host" )->toString( );
		udInt			port	= config->get( "server_port" )->toInt( ),
						clport	= config->get( "client_port" )->toInt( );
		udPSocket		sock	= new udSocket( udSocketInfo( host.data( ), clport ) );

		sock->Create( );
		sock->Shutdown( );
		delete sock;

		sock = new udSocket( udSocketInfo( host.data( ), port ) );
		if ( sock->Create( ) == 1 ) {
			if ( sock->Connect( ) == 1 ) {
				udBinaryData binSend, binMetadata;
				binMetadata.add( string( "#" ) );
				// собираем данные по таблицам
				udPBase obj = NULL;
				WriteUdLibLog( "//////////////////////////////////////////////////////////////////////////////////////////\n" );
				WriteUdLibLog( "// Собираем данные о таблицах\n" );
				string log = "\n";
				udUInt size_metadata = binMetadata.size( );
				for( udArrBase::iterator i = m_arrTableInfo.begin( ); i != m_arrTableInfo.end( ); ++i ) {
					obj = *i;
					obj->dump( binMetadata );
					obj->dump( log );
					log += "\r\n";
				}
				size_metadata = binMetadata.size( ) - size_metadata;
				WriteUdLibLog( "Объем метаданных, size_metadata=" + toString( size_metadata ) + " байт\n" );
				WriteUdLibLog( log );
				WriteUdLibLog( "//////////////////////////////////////////////////////////////////////////////////////////\n" );
				// отсылаем их
				WriteUdLibLog( "// Отправляем данные\n" );
				//binSend.add( ( udInt ) syncTables );
				//binSend.add( binMetadata.size( ) );
				binSend.add( binMetadata );
				binMetadata.clear( );

				udUInt size = binSend.size( );
				if ( sock->Send( binSend ) == size ) {
					char code = 0;
					sock->ReadByte( &code );
					WriteUdLibLog( "code=" + toString( ( udInt ) code ) + "\n" );
					sock->SendByte( ( char ) 110 );
					WriteUdLibLog( "//////////////////////////////////////////////////////////////////////////////////////////\n" );
					WriteUdLibLog( "// обрабатываем синхронизацию\n" );
					// передаем управление
					g_pApp->GetInitObj( )->HandleSync( sock, m_arrInfo );
					WriteUdLibLog( "//////////////////////////////////////////////////////////////////////////////////////////\n" );
					WriteUdLibLog( "// закрываем соединение\n" );
				} else {
					WriteUdLibLog( "отослали разное количество байт\n" );
				}
				// завершаем соединение
				sock->Shutdown( );
			}
			sock->Close( );
		}
		delete sock;
	} // void Sync

	udInt CDatabaseSync::Query( const udArrBase& arr ) {
		return 0;
	} // udInt Query

	void CDatabaseSync::SendMetadata( udPSocket sock ) {
		udBinaryData data;
		data.add( m_arrTableInfo.size( ) );
		for( udArrBase::iterator i = m_arrTableInfo.begin( );  i != m_arrTableInfo.end( ); ++i ) {
			( *i )->dump( data );
		}
		sock->Send( data );
	} // void SendMetadata

	void CDatabaseSync::UpdateVersion( udArrBase& metadata ) {
		udPBase obj = NULL;
		string name = "";
		for( udArrBase::iterator i = metadata.begin( ); i != metadata.end( ); ++i ) {
			obj = *i;
			name = obj->get( "table" )->toString( );
			if ( m_arrInfo.find( name ) != m_arrInfo.end( ) ) {
				obj->apply( m_arrInfo[ name ] );
			} else {
				g_pApp->GetLog( )->Write( "таблица '" + name + "' не найдена\n" );
			}
		}
		udArrError err;
		udHandlerOption option;
		udPHandler handler = g_pApp->GetHandler( );
		option.SetIndexAttr( "id" );
		option.SetTable( "ud_syncinfo" );
		option.SetClass( GetDatabaseSyncInfo );
		handler->UpdUdObject( m_arrTableInfo, option, &err );
		if ( !err.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				__log->Write( i->GetText( ) + "\r\n" );
			}
			err.clear( );
		}
	} // void UpdateVersion

	void CDatabaseSync::UpdateVersion( const string& name ) {
		udAscBase::iterator i = m_arrInfo.find( name );
		if ( i != m_arrInfo.end( ) ) {
			udDWord dwVersion = i->second->get( "version" )->toDWord( ) + 1;
			udPHandler handler = g_pApp->GetHandler( );
			udArrBase arr;
			udArrError err;
			udHandlerOption option;
			i->second->get( "version" )->ini( dwVersion );
			string log = "\n";
			i->second->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			arr.push_back( i->second );
			option.SetIndexAttr( "id" );
			option.SetTable( "ud_syncinfo" );
			option.SetClass( GetDatabaseSyncInfo );
			handler->UpdUdObject( arr, option, &err );
			if ( !err.empty( ) ) {
				udPLog __log = g_pApp->GetLog( );
				for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
					__log->Write( i->GetText( ) + "\r\n" );
				}
				err.clear( );
			}
		} else {
			g_pApp->GetLog( )->Write( "таблица '" + name + "' не найдена" );
		}
	} // void UpdateVersion

	void CDatabaseSync::AddSyncTable( const string& name, const string& handler ) {
		if ( m_arrTable.find( name ) == m_arrTable.end( ) ) {
			m_arrTable[ name ].cnt		= 0;
			m_arrTable[ name ].name		= name;
			m_arrTable[ name ].handler	= handler;
		}
	} // void AddSyncTable

	udInt CDatabaseSync::LoadInfoData( ) {
		if ( !m_arrTable.empty( ) ) { // подгружаем данные о таблицах
			udPHandler		handler	= g_pApp->GetHandler( );
			udHandlerOption	option;
			udPBase			obj		= NULL;
			udArrError		err;
			udBaseConfig	config;
			string			szWhere	= "";
			udArrString		arrString;
			for( _asc_tables::iterator i = m_arrTable.begin( ); i != m_arrTable.end( ); ++i ) {
				arrString.push_back( "'" + i->first + "'" );
			}
			obj = new udDatabaseSyncInfo;
			obj->GetConfig( config );
			delete obj;
			szWhere = config.GetAttrIndex( "table" ) + " IN(" + Join( ",", arrString ) + ")";
			config.Clear( );
			arrString.clear( );
			option.SetTable( "ud_syncinfo" );
			option.SetClass( GetDatabaseSyncInfo );
			option.SetWhere( szWhere );
			handler->GetUdObject( m_arrTableInfo, option, &err );
			if ( err.empty( ) ) {
				g_pApp->GetLog( )->Write(
					"подгружено метаданных: " + toString( m_arrTableInfo.size( ) ) + "\r\n"
				);

				string table = "";
				for( udArrBase::iterator i = m_arrTableInfo.begin( ); i != m_arrTableInfo.end( ); ++i ) {
					obj = *i;
					table = obj->get( "table" )->toString( );
					m_arrInfo[ table ] = obj;
					m_arrTable[ table ].cnt = 1;
				}

				if ( m_arrTableInfo.empty( ) ) {
					return 1; // надо создать метаданные
				} else {
					return 2; // все нормально
				}
			} else {
				udPLog log = g_pApp->GetLog( );
				for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
					log->Write( i->GetText( ) + "\r\n" );
				}
				err.clear( );
				return 0;
			}
		}
		return -1;
	} // udInt LoadInfoData

	void CDatabaseSync::CheckTables( ) {
		udPHandler		handler = g_pApp->GetHandler( );
		udHandlerOption	option;
		udArrBase		arrAdd;
		datetime		date;
		string			table = "", handler_name = "";
		udPBase			obj		= NULL;
		udArrError		err;
		date.load( );
		for( _asc_tables::iterator i = m_arrTable.begin( ); i != m_arrTable.end( ); ++i ) {
			if ( i->second.cnt == 0 ) {
				table = i->first;
				handler_name = i->second.handler;

				obj = new udDatabaseSyncInfo;
				obj->get( "table" )->ini( table );
				//obj->get( "version" )->ini( ( udDWord ) 1 );
				//obj->get( "lastid" )->ini( ( udUInt ) 1 );
				obj->get( "insdate" )->ini( date );
				obj->get( "upddate" )->ini( date );
				obj->get( "deldate" )->ini( date );
				obj->get( "handler" )->ini( handler_name );
				arrAdd.push_back( obj );
			}
		}
		option.SetTable( "ud_syncinfo" );
		handler->AddUdObject( arrAdd, option, &err );
		this->LoadInfoData( );
		ClearArrBase( arrAdd );
	} // void CheckTables




	udPBase GetDatabaseSyncInfo( ) {
		return new udDatabaseSyncInfo;
	}

	CDatabaseSyncInfo::CDatabaseSyncInfo( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "table"		)->ini( string( "" )	);	// имя таблицы
		this->get( "handler"	)->ini( string( "" )	);	// имя обработчика состояния
		this->get( "version"	)->ini( ( udDWord )	0	);	// версия
		this->get( "lastid"		)->ini( ( udUInt )	0	);	// последнее ID
		this->get( "count"		)->ini( ( udUInt )	0	);	// количество строк
		this->get( "insdate"	)->ini( udDatetime( )	);	// дата последней вставки
		this->get( "upddate"	)->ini( udDatetime( )	);	// дата последнего обновления
		this->get( "deldate"	)->ini( udDatetime( )	);	// дата последнего удаления
	}

	CDatabaseSyncInfo::~CDatabaseSyncInfo( ) {
	}

	void CDatabaseSyncInfo::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_syncinfo" );
		config.SetPrefix( "syncinfo_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"							);
		config.SetAttrTitle( "table",	"Таблица"						);
		config.SetAttrTitle( "handler",	"Имя обработчика"				);
		config.SetAttrTitle( "version",	"Версия"						);
		config.SetAttrTitle( "lastid",	"Последний ID"					);
		config.SetAttrTitle( "insdate",	"Дата последней вставки"		);
		config.SetAttrTitle( "upddate",	"Дата последнего обновления"	);
		config.SetAttrTitle( "deldate",	"Дата последнего удаления"		);
		config.SetAttrTitle( "count",	"Количество строк"				);
	} // void GetConfig




	udPBase GetDatabaseTableQuery( ) {
		return new udDatabaseTableQuery;
	}

	CDatabaseTableQuery::CDatabaseTableQuery( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "created"	)->ini( udDatetime( )	);	// дата создания
		this->get( "type"		)->ini( ( udInt )	0	);	// тип запроса
		this->get( "tableid"	)->ini( ( udUInt )	0	);	// ID таблицы
		this->get( "objid"		)->ini( ( udUInt )	0	);	// ID объекта
		this->get( "state"		)->ini( ( udInt )	0	);	// состояние
		this->get( "version"	)->ini( ( udDWord )	0	);	// версия таблицы при запросе
		this->get( "query"		)->ini( string( "" )	);	// текст запроса
		this->get( "bin"		)->ini( udBinaryData( )	);	// бинарный дамп объекта
	}

	CDatabaseTableQuery::~CDatabaseTableQuery( ) {
	}

	void CDatabaseTableQuery::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_syncquery" );
		config.SetPrefix( "syncquery_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"							);
		config.SetAttrTitle( "created",	"Дата создания"					);
		config.SetAttrTitle( "tableid",	"ID таблицы"					);
		config.SetAttrTitle( "version",	"Версия таблицы при запросе"	);
		config.SetAttrTitle( "query",	"Текст запроса"					);
		config.SetAttrTitle( "type",	"Тип запроса"					);
		config.SetAttrTitle( "objid",	"Объект запроса"				);
		config.SetAttrTitle( "state",	"Состояние запроса"				);
		config.SetAttrTitle( "bin",		"Бинарный дамп объекта"			);
	} // void GetConfig

} // namespace UdSDK
