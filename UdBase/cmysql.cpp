
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CMySql::CMySql( ) : udDatabase( ) {
		m_pConnection	= NULL;
		m_pAccount		= new udDatabaseAccount;
		m_iPort			= 0;
	}

	CMySql::~CMySql( ) {
		if ( m_pConnection ) {
			mysql_close( m_pConnection );
			m_pConnection = NULL;
		}
		udDELETE( m_pAccount );
		m_iPort = 0;
	}

	void CMySql::SetAccData( const std::string& host, const std::string& user, const std::string& passw, const std::string& db ) {
		assert( m_pAccount );
		m_pAccount->SetHostname( host );
		m_pAccount->SetUsername( user );
		m_pAccount->SetPassword( passw );
		m_pAccount->SetDatabaseName( db );
	} // void SetAccData

	CMySql::udMySQLError CMySql::Connect( bool create_db ) {
		assert( m_pConnection == NULL );
		assert( m_pAccount );
		m_pConnection = mysql_init( NULL );
		if ( !m_pConnection ) {
			return CMySql::UDSDK_MYSQL_ERROR_NOINIT;
		}
		if ( !mysql_real_connect( m_pConnection,
			m_pAccount->GetHostname( ).data( ),
			m_pAccount->GetUsername( ).data( ),
			m_pAccount->GetPassword( ).data( ),
			"", m_iPort, NULL, 0
		) ) {
			return CMySql::UDSDK_MYSQL_ERROR_NOCONNECT;
		}
		string dbname = m_pAccount->GetDatabaseName( );
		if ( dbname.empty( ) ) {
			return CMySql::UDSDK_MYSQL_ERROR_EMPTYDBNAME;
		}
		if ( this->SelectDb( dbname ) ) { // база не существует
			if ( create_db ) {
				WriteUdBaseLog( "база данных '" + dbname + "' не существует, пытаемся ее создать\n" );
				string query = "CREATE DATABASE IF NOT EXISTS `" + dbname + "`";
				WriteUdBaseLog( "query='" + query + "'\n" );
				if ( mysql_real_query( m_pConnection, query.data( ), query.size( ) ) ) {
					WriteUdBaseLog( "не удалось создать базу данных '" + dbname + "', код ошибки=" + toString( mysql_errno( m_pConnection ) ) + "\n" );
					return CMySql::UDSDK_MYSQL_ERROR_CREATEDBFAIL;
				}
				if ( mysql_select_db( m_pConnection, dbname.data( ) ) ) {
					return CMySql::UDSDK_MYSQL_ERROR_SELECTFAIL;
				}
			} else {
				return CMySql::UDSDK_MYSQL_ERROR_NODATABASE;
			}
		}/* else {
			return CMySql::UDSDK_MYSQL_ERROR_NODATABASE;
		}*/
		if ( !m_szCharacterSet.empty( ) ) {
			mysql_set_character_set( m_pConnection, m_szCharacterSet.data( ) );
		}

		return CMySql::UDSDK_MYSQL_ERROR_OK;
	} // udMySQLError Connect

	/*CMySql::udMySQLError CMySql::Create( const string& szHost, const string& szUser, const string& szPass, const string& szDb, const udUInt iPort ) {
		m_pConnection = mysql_init( NULL );
		if ( !m_pConnection ) {
			return CMySql::UDSDK_MYSQL_ERROR_NOINIT;
		}

		if ( !mysql_real_connect( m_pConnection, szHost.data( ), szUser.data( ), szPass.data( ), "", iPort, NULL, 0 ) ) {
			return CMySql::UDSDK_MYSQL_ERROR_NOCONNECT;
		}

		if ( szDb.size( ) > 0 ) {
			if ( this->SelectDb( szDb ) ) {	// не удалось выбрать БД, значит ее нет
				string szQuery = "CREATE DATABASE `";
				szQuery += szDb + string( "`" );

				WriteUdBaseLog( "query='" + szQuery + "'\n" );
				mysql_real_query( m_pConnection, szQuery.c_str( ), szQuery.size( ) );
				WriteUdBaseLog( "selecting database query='" + szDb + "'\n" );
				mysql_select_db( m_pConnection, szDb.data( ) );
			}
		} else {
			mysql_close( m_pConnection );
			return CMySql::UDSDK_MYSQL_ERROR_EMPTYDBNAME;
		}

		m_pAccount	= new udDatabaseAccount( szHost, szUser, szPass, szDb );
		m_iPort		= iPort;

		return CMySql::UDSDK_MYSQL_ERROR_OK;
	} // bool Create*/

	void CMySql::Close( ) {
		if ( m_pConnection ) {
			mysql_close( m_pConnection );
			m_pConnection = NULL;
		}
	} // void Close

	udUInt CMySql::GetErrorNumber( ) {
		return mysql_errno( m_pConnection );
	} // udUInt GetErrorNumber

	string CMySql::GetErrorMessage( ) {
		return string( mysql_error( m_pConnection ) );
	} // string GetErrorMessage

	udULongLong CMySql::GetAffectedRows( ) {
		return mysql_affected_rows( m_pConnection );
	} // udULongLong GetAffectedRows

	udULongLong CMySql::GetInsertId( ) {
		return mysql_insert_id( m_pConnection );
	} // udULongLong GetInsertId

	udInt CMySql::SetCharacterSet( const udPChar szCharacterSet ) {
		return mysql_set_character_set( m_pConnection, szCharacterSet );
	} // udInt SetCharacterSet

	udInt CMySql::SelectDb( const string& szDb ) {
		return mysql_select_db( m_pConnection, szDb.data( ) );
	} // udInt SelectDb

	udPMySQLRes CMySql::UseResult( ) {
		return mysql_use_result( m_pConnection );
	} // udPMySQLRes UseResult

	udPMySQLRes CMySql::StoreResult( ) {
		return mysql_store_result( m_pConnection );
	} // udPMySQLRes StoreResult

	void CMySql::FreeResult( udPMySQLRes pResult ) {
		mysql_free_result( pResult );
	} // void FreeResult

	udUInt CMySql::FieldCount( ) {
		return mysql_field_count( m_pConnection );
	} // udUInt FieldCount

	static string FieldType( enum_field_types iType ) {
		switch( iType ) {
			case MYSQL_TYPE_DECIMAL:
				return "MYSQL_TYPE_DECIMAL";
				break;
				
			case MYSQL_TYPE_TINY:
				return "MYSQL_TYPE_TINY";
				break;

			case MYSQL_TYPE_SHORT:
				return "MYSQL_TYPE_SHORT";
				break;

			case MYSQL_TYPE_LONG:
				return "MYSQL_TYPE_LONG";
				break;
			
			case MYSQL_TYPE_FLOAT:
				return "MYSQL_TYPE_FLOAT";
				break;
				
			case MYSQL_TYPE_DOUBLE:
				return "MYSQL_TYPE_DOUBLE";
				break;

			case MYSQL_TYPE_NULL:
				return "MYSQL_TYPE_NULL";
				break;
				
			case MYSQL_TYPE_TIMESTAMP:
				return "MYSQL_TYPE_TIMESTAMP";
				break;

			case MYSQL_TYPE_LONGLONG:
				return "MYSQL_TYPE_LONGLONG";
				break;
				
			case MYSQL_TYPE_INT24:
				return "MYSQL_TYPE_INT24";
				break;

			case MYSQL_TYPE_DATE:
				return "MYSQL_TYPE_DATE";
				break;
				
			case MYSQL_TYPE_TIME:
				return "MYSQL_TYPE_TIME";
				break;

			case MYSQL_TYPE_DATETIME:
				return "MYSQL_TYPE_DATETIME";
				break;
				
			case MYSQL_TYPE_YEAR:
				return "MYSQL_TYPE_YEAR";
				break;

			case MYSQL_TYPE_NEWDATE:
				return "MYSQL_TYPE_NEWDATE";
				break;
				
			case MYSQL_TYPE_VARCHAR:
				return "MYSQL_TYPE_VARCHAR";
				break;

			case MYSQL_TYPE_BIT:
				return "MYSQL_TYPE_BIT";
				break;

			case MYSQL_TYPE_NEWDECIMAL:
				return "MYSQL_TYPE_NEWDECIMAL";
				break;

			case MYSQL_TYPE_ENUM:
				return "MYSQL_TYPE_ENUM";
				break;

			case MYSQL_TYPE_SET:
				return "MYSQL_TYPE_SET";
				break;

			case MYSQL_TYPE_TINY_BLOB:
				return "MYSQL_TYPE_TINY_BLOB";
				break;

			case MYSQL_TYPE_MEDIUM_BLOB:
				return "MYSQL_TYPE_MEDIUM_BLOB";
				break;

			case MYSQL_TYPE_LONG_BLOB:
				return "MYSQL_TYPE_LONG_BLOB";
				break;

			case MYSQL_TYPE_BLOB:
				return "MYSQL_TYPE_BLOB";
				break;

			case MYSQL_TYPE_VAR_STRING:
				return "MYSQL_TYPE_VAR_STRING";
				break;

			case MYSQL_TYPE_STRING:
				return "MYSQL_TYPE_STRING";
				break;

			case MYSQL_TYPE_GEOMETRY:
				return "MYSQL_TYPE_GEOMETRY";
				break;
		}
		return "unknown";
	}

	static udPType ConvertFieldToValue( udPMySQLField pField, udULong iLen, udPChar pBytes ) {
		WriteUdBaseLog( "ConvertFieldToValue, enter\n" );
		WriteUdBaseLog( "ConvertFieldToValue, type is '" + FieldType( pField->type ) + "'\n" );

		udChar tmp[ 1024 ];
		switch( pField->type ) {
			case MYSQL_TYPE_LONG: {	// INTEGER 4 байта
				udUInt iValue = 0;
				if ( pBytes ) {
					sprintf_s( tmp, 1024, "%s", pBytes );
					sscanf_s( tmp, "%u", &iValue );
				}
				return new udType( iValue );
			} break;

			case MYSQL_TYPE_VAR_STRING: {	// VARCHAR
				wstring szRet( L"" );
				if ( pBytes ) {
					if ( pField->charsetnr == 63 ) { // бинарная строка
						WriteUdBaseLog( "binary\n" );
					} else {
						if ( pBytes ) {
							szRet = ConvertString( string( pBytes ), CP_UTF8 );
						}
					}
				}
				return new udType( szRet );
			} break;

			case MYSQL_TYPE_LONGLONG: {	// BIGINT 8 байт
				udLongLong lValue = 0;
				if ( pBytes ) {
					sprintf_s( tmp, 1024, "%s", pBytes );
					sscanf_s( tmp, "%I64d", &lValue );
				}
				return new udType( lValue );
			} break;

			case MYSQL_TYPE_FLOAT: {
				udFloat fValue = 0.0f;
				if ( pBytes ) {
					sprintf_s( tmp, 1024, "%s", pBytes );
					sscanf_s( tmp, "%f", &fValue );
				}
				return new udType( fValue );
			} break;
				
			case MYSQL_TYPE_DOUBLE: {
				udDouble dValue = 0.0;
				if ( pBytes ) {
					sprintf_s( tmp, 1024, "%s", pBytes );
					sscanf_s( tmp, "%lf", &dValue );
				}
				return new udType( dValue );
			} break;

			case MYSQL_TYPE_BLOB: {
				wstring szRet( L"" );
				if ( pBytes ) {
					if ( pField->charsetnr == 63 ) { // бинарная строка
						WriteUdBaseLog( "binary\n" );
					} else {
						szRet = ConvertString( string( pBytes ), CP_UTF8 );
					}
				}
				return new udType( szRet );
			} break;

			case MYSQL_TYPE_DATE:
			case MYSQL_TYPE_TIME:
			case MYSQL_TYPE_DATETIME: {
				udDatetime date;
				if ( pBytes ) {
					date.load( pBytes );
				}
				return new udType( date );
			} break;

			default:
				sprintf_s( tmp, 1024, "field type: %d\n", ( int ) pField->type );
				WriteUdBaseLog( tmp );
				break;
		}

		WriteUdBaseLog( "ConvertFieldToValue, exit\n" );

		return NULL;
	} // udPXType ConvertFieldToValue

	static udAscType ConvertRowToAssoc( udPPMySQLField arrField, udUInt iFldNum, udPULong arrLen, udMySQLRow pRow ) {
		WriteUdBaseLog( "ConvertRowToAssoc, enter\n" );

		udAscType	arrRet;
		udPType		pValue	= NULL;

		for( udUInt i = 0; i < iFldNum; ++i ) {
			WriteUdBaseLog( "ConvertRowToAssoc, field name is '" + string( arrField[ i ]->name ) + "'\n" );

			if ( pValue = ConvertFieldToValue( arrField[ i ], arrLen[ i ], pRow[ i ] ) ) {
				arrRet[ string( arrField[ i ]->name ) ] = pValue;
			}
		}

		WriteUdBaseLog( "ConvertRowToAssoc, exit\n" );

		return arrRet;
	} // udMapXType ConvertRowToAssoc

	static udPMorph ConvertRowToMorph( udPPMySQLField arrField, udUInt iFldNum, udPULong arrLen, udMySQLRow pRow ) {
		udPMorph	ret	= new udMorph;
		udPType		tmp	= NULL;

		for( udUInt i = 0; i < iFldNum; ++i ) {
			if ( tmp = ConvertFieldToValue( arrField[ i ], arrLen[ i ], pRow[ i ] ) ) {
				*( ret->get( string( arrField[ i ]->name ) ) ) = *tmp;
				delete tmp;
			}
		}

		return ret;
	} // udPMorph ConvertRowToMorph

	void CMySql::EnumFields( udPMySQLRes pResult ) {
		udChar log[ 1024 ];
		udPMySQLField pField = NULL;
		string szIndex;

		while( pField = mysql_fetch_field( pResult ) ) {
			szIndex = FieldType( pField->type );
			sprintf_s( log, 1024, "name='%s', length='%d', type='%s'\n", pField->name, pField->length, szIndex.c_str( ) );
			WriteUdBaseLog( log );
		}
	} // void EnumFields

	bool CMySql::Query( string query ) {
		WriteUdBaseLog( "query='" + query + "'\n" );
		bool ret = ( mysql_real_query( m_pConnection, query.c_str( ), query.size( ) ) == 0 );
		if ( !ret && ( mysql_errno( m_pConnection ) == 2006 ) ) { // попробуем восстановить соединение
			WriteUdBaseLog( "потеряно соединение, восстанавливаем, закрыв и создав заново\n" );
			this->Close( );
			if ( this->Connect( true ) == UDSDK_MYSQL_ERROR_OK ) { // получилось подсоединиться
				WriteUdBaseLog( "восстановили соединение, выполняем запрос\n" );
				ret = ( mysql_real_query( m_pConnection, query.c_str( ), query.size( ) ) == 0 );
				if ( !ret ) {
					WriteUdBaseLog( "ошибка при выполнении запрос сразу после восстановления, код=" +
						toString( mysql_errno( m_pConnection ) ) + ", сообщение='" +
						this->GetErrorMessage( ) + "'\n" );
				}
			}
		}
		return ret;
	} // bool Query

	bool CMySql::GetAssoc( string query, udArrAsc& out ) {
		WriteUdBaseLog( "CMySql::GetAssoc, enter\n" );
		if ( !this->Query( query ) ) {
			return false;
		}

		WriteUdBaseLog( "CMySql::GetAssoc, storing result\n" );

		udPMySQLRes pResult = mysql_store_result( m_pConnection );
		if ( !pResult ) {
			return false;
		}

		WriteUdBaseLog( "CMySql::GetAssoc, processing rows\n" );

		udULongLong iRowNum = mysql_num_rows( pResult );	// количество строк
		if ( iRowNum ) {
			udUInt			iFldNum		= mysql_num_fields( pResult );		// количество полей
			udMySQLRow		objRow		= NULL;								// строка для обработки

			udAscType mapInp;
			string szIndex;
			udUInt i = 0;
			udPULong pLen;
			udPPMySQLField arrField = new udPMySQLField[ iFldNum ];
			udPMySQLField pField = NULL;
			
			while( pField = mysql_fetch_field( pResult ) ) {
				arrField[ i++ ] = pField;
			}

			while( objRow = mysql_fetch_row( pResult ) ) {
				// преобразуем строку таблицы в ассоциативный массив
				pLen = mysql_fetch_lengths( pResult );
				mapInp = ConvertRowToAssoc( arrField, iFldNum, pLen, objRow );
				if ( !mapInp.empty( ) ) {
					out.push_back( mapInp );
				}
			}

			delete [ ] arrField;
		}

		WriteUdBaseLog( "CMySql::GetAssoc, free result\n" );

		mysql_free_result( pResult );

		WriteUdBaseLog( "CMySql::GetAssoc, exit\n" );

		return true;
	} // bool GetAssoc

	bool CMySql::GetAssoc( string query, udArrMorph& out ) {
		if ( !this->Query( query ) ) {
			return false;
		}

		udPMySQLRes pResult = mysql_store_result( m_pConnection );
		if ( !pResult ) {
			return false;
		}

		udULongLong iRowNum = mysql_num_rows( pResult );	// количество строк
		WriteUdBaseLog( "CMySql::GetAssoc, iRowNum=" + toString( iRowNum ) + "\n" );
		if ( iRowNum ) {
			udUInt		iFldNum		= mysql_num_fields( pResult );		// количество полей
			WriteUdBaseLog( "CMySql::GetAssoc, iFldNum=" + toString( iFldNum ) + "\n" );
			udMySQLRow	objRow		= NULL;								// строка для обработки
			udPMorph	obj			= NULL;
			string		index;
			udUInt		i = 0;
			udPULong	pLen;
			udPPMySQLField arrField = new udPMySQLField[ iFldNum ];
			udPMySQLField pField = NULL;
			
			while( pField = mysql_fetch_field( pResult ) ) {
				arrField[ i++ ] = pField;
			}

			while( objRow = mysql_fetch_row( pResult ) ) {
				// преобразуем строку таблицы в ассоциативный массив
				pLen = mysql_fetch_lengths( pResult );
				obj = ConvertRowToMorph( arrField, iFldNum, pLen, objRow );
				if ( obj ) {
					out.push_back( obj );
				}
			}

			delete [ ] arrField;
		}

		mysql_free_result( pResult );

		return true;
	} // bool GetAssoc

	udError CMySql::GetError( ) {
		return udError( this->GetErrorNumber( ), this->GetErrorMessage( ) );
	} // udError GetError

	bool CMySql::EscapeString( char* dest, const char* src, size_t len ) {
		mysql_real_escape_string( m_pConnection, dest, src, len );
		return true;
	} // bool EscapeString

} // namespace UdSDK
