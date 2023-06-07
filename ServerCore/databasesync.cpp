
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
				//g_pApp->GetWndServer( )->UpdateText( L"В настройках синхронизатора отсутствуют таблицы\n" );
				break;

			case 0:
				//g_pApp->GetWndServer( )->UpdateText( L"Возникли ошибки при загрузке метаданных синхронизатора\n" );
				break;

			case 1:
				this->CheckTables( );
				break;

			default:
				break;
		}
	} // void Init

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
		//wstring			text	= L"";
		udArrError		err;
		date.load( );
		for( _asc_tables::iterator i = m_arrTable.begin( ); i != m_arrTable.end( ); ++i ) {
			if ( i->second.cnt == 0 ) {
				table = i->first;
				handler_name = i->second.handler;
				//text += toWString( table  ) + L" - нет данных\r\n";

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
		//g_pApp->GetWndServer( )->UpdateText( text );
		ClearArrBase( arrAdd );
	} // void CheckTables

	void CDatabaseSync::Sync( udPSocket sock ) {
		//g_pApp->GetWndServer( )->UpdateText( L"CDatabaseSync::Sync" );
		// считываем состояние клиента
		wstring		text = L"Полученные метаданные:\r\n";
		udArrBase	arrClientTableInfo;
		udAscBase	arrClientInfo;
		udPBase		obj	= NULL;

		string log = "\n";
		log += "всего метаданных: " + toString( m_arrTableInfo.size( ) ) + "\n";
		for( udArrBase::iterator i = m_arrTableInfo.begin( ); i != m_arrTableInfo.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		g_pApp->GetLog( )->Write( log );

		while( arrClientTableInfo.size( ) < m_arrTableInfo.size( ) ) {
			obj = new udDatabaseSyncInfo;
			if ( obj->read( sock ) == 0 ) {
				text += L"не удалось выбрать объект";
				delete obj;
				break;
			}
			arrClientTableInfo.push_back( obj );
			arrClientInfo[ obj->get( "table" )->toString( ) ] = obj;

			obj->dump( text );
			text += L"\r\n";
		}
		text = L"всего прислано метаданных: " + toWString( arrClientTableInfo.size( ) ) + L"\r\n" + text;

		sock->SendByte( ( char ) 100 );
		char code = 0;
		sock->ReadByte( &code );

		text += L"код возврата: " + toWString( ( udInt ) code ) + L"\r\n";

		//g_pApp->GetWndServer( )->UpdateText( text );
		// передаем управление
		g_pApp->GetInitObj( )->HandleSync( sock, arrClientInfo, m_arrInfo );
		ClearArrBase( arrClientTableInfo );
		arrClientInfo.clear( );
	} // void Sync

	udInt CDatabaseSync::Query( const udArrBase& arr ) {
		return 0;
	} // udInt Query

	void CDatabaseSync::CollectMetadata( udBinaryData& data ) {
		WriteInstances( m_arrTableInfo, data );
		/*data.add( m_arrTableInfo.size( ) );
		for( udArrBase::iterator i = m_arrTableInfo.begin( );  i != m_arrTableInfo.end( ); ++i ) {
			( *i )->dump( data );
		}*/
	} // void CollectMetadata

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

	udPBase CDatabaseSync::MakeInfo( udPBase obj, udInt operation, const string& table, const string& query ) {
		udAscBase::iterator i = m_arrInfo.find( table );
		if ( i != m_arrInfo.end( ) ) {
			udPBase			meta	= i->second,
							dbmeta	= m_arrInfo[ "#database" ],
							info	= new udDatabaseTableQuery;
			udInt			state	= udDatabaseTableQuery::QueryStateNormal;
			udBinaryData	bin;
			datetime		date;

			date.load( );
			obj->dump( bin );
			info->get( "created" )->ini( date );
			info->get( "type" )->ini( operation );
			info->get( "tableid" )->ini( meta->get( "id" )->toUInt( ) );
			info->get( "objid" )->ini( obj->get( "id" )->toUInt( ) );
			info->get( "state" )->ini( state );
			info->get( "version" )->ini( dbmeta->get( "version" )->toDWord( ) );
			info->get( "query" )->ini( query );
			info->get( "bin" )->ini( bin );
			bin.clear( );

			return info;
		}
		return NULL;
	} // udPBase MakeInfo

	udInt CDatabaseSync::SaveInfo( udPBase obj, udInt operation, const string& table, const string& query ) {
		assert( obj );
		g_pApp->GetLog( )->Write( "CDatabaseSync::SaveInfo\n" );

		udAscBase::iterator i = m_arrInfo.find( table );
		if ( i != m_arrInfo.end( ) ) {
			udPBase			meta	= i->second,
							dbmeta	= m_arrInfo[ "#database" ],
							info	= NULL;
			udPHandler		handler	= g_pApp->GetHandler( );
			udHandlerOption	option;
			udArrError		err;
			//////////////////////////////////////////////////////////////////////////////////////
			// наполняем инфу
			g_pApp->GetLog( )->Write( "наполняем инфу\n" );
			info = this->MakeInfo( obj, operation, table, query );
			//////////////////////////////////////////////////////////////////////////////////////
			// сохраняем инфу
			g_pApp->GetLog( )->Write( "сохраняем инфу\n" );
			string log;
			log = "\nдобавляем информацию об операции над объектом\n";
			info->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			option.SetTable( "ud_syncquery" );
			handler->AddUdObject( info, option, &err );
			if ( !err.empty( ) ) {
				udPLog __log = g_pApp->GetLog( );
				for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
					__log->Write( i->GetText( ) + "\n" );
				}
				err.clear( );
			}
			// необходимо при удалении объекта пометить все его предыдущие операции как удаленные
			//////////////////////////////////////////////////////////////////////////////////////
			delete info;
			g_pApp->GetLog( )->Write( "CDatabaseSync::SaveInfo exit\n" );
			return 1;
		} else {
			g_pApp->GetLog( )->Write( "таблица '" + table + "' не найдена\n" );
		}
		g_pApp->GetLog( )->Write( "CDatabaseSync::SaveInfo exit\n" );
		return 0;
	} // udInt SaveInfo




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
