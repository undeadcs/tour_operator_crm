
#include "includes.h"

namespace UdSDK {

	CUdBase::CUdBase( ) {
	}

	CUdBase::CUdBase( const CUdBase& objCopy ) {
	}

	CUdBase::~CUdBase( ) {
	}

	CUdBase& CUdBase::operator =( const CUdBase& objCopy ) {
		return *this;
	}

	void CUdBase::Create( const udAscType& vctInput, cfgVal iMode ) {
		udBaseConfig objConfig;
		this->GetConfig( objConfig );

		for( udLstPairType::iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			this->InitAttr( i->first, vctInput, objConfig, iMode );
		}
	} // void Create

	void CUdBase::Create( udPMorph obj, cfgVal iMode ) {
		udBaseConfig objConfig;
		this->GetConfig( objConfig );

		for( udLstPairType::iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			this->InitAttr( i->first, obj, objConfig, iMode );
		}
	} // void Create

	void CUdBase::GetConfig( udBaseConfig& objConfig ) {
		udPType	pType	= NULL;
		string	szName	= "";

		for( udLstPairType::iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			szName	= i->first;
			pType	= i->second;

			if ( pType->isInt( ) ) {
				objConfig.SetAttrType( szName, udBaseConfig::UDSDK_TYPE_INT );
				objConfig.SetAttrDigits( szName, udBase::UDSDK_DEFAULT_DIGITS );
			} else if ( pType->isUInt( ) ) {
				objConfig.SetAttrType( szName, ( udBaseConfig::cfgType ) ( udBaseConfig::UDSDK_TYPE_INT | udBaseConfig::UDSDK_TYPE_UNSIGNED ) );
				objConfig.SetAttrDigits( szName, udBase::UDSDK_DEFAULT_DIGITS );
			} else if ( pType->isFloat( ) ) {
				objConfig.SetAttrType( szName, udBaseConfig::UDSDK_TYPE_FLOAT );
				objConfig.SetAttrDigits( szName, udBase::UDSDK_DEFAULT_DIGITS );
				objConfig.SetAttrDecimals( szName, udBase::UDSDK_DEFAULT_DECIMAL );
			} else if ( pType->isDouble( ) ) {
				objConfig.SetAttrType( szName, udBaseConfig::UDSDK_TYPE_DOUBLE );
				objConfig.SetAttrDigits( szName, udBase::UDSDK_DEFAULT_DIGITS );
				objConfig.SetAttrDecimals( szName, udBase::UDSDK_DEFAULT_DECIMAL );
			} else if ( pType->isWord( ) ) {
				objConfig.SetAttrType( szName, ( udBaseConfig::cfgType ) ( udBaseConfig::UDSDK_TYPE_INT | udBaseConfig::UDSDK_TYPE_UNSIGNED ) );
				objConfig.SetAttrDigits( szName, CUdBase::UDSDK_DEFAULT_DIGITS );
			} else if ( pType->isDWord( ) ) {
				objConfig.SetAttrType( szName, ( udBaseConfig::cfgType ) ( udBaseConfig::UDSDK_TYPE_INT | udBaseConfig::UDSDK_TYPE_UNSIGNED ) );
				objConfig.SetAttrDigits( szName, udBase::UDSDK_DEFAULT_DIGITS );
			} else if ( pType->isString( ) ) {
				objConfig.SetAttrType( szName, udBaseConfig::UDSDK_TYPE_STRING );
				objConfig.SetAttrLength( szName, udBase::UDSDK_DEFAULT_LENGTH );
			} else if ( pType->isTime( ) ) {
				objConfig.SetAttrType( szName, udBaseConfig::UDSDK_TYPE_DATETIME );
			} else if ( pType->isWString( ) ) {
				objConfig.SetAttrType( szName, udBaseConfig::UDSDK_TYPE_STRING );
				objConfig.SetAttrLength( szName, udBase::UDSDK_DEFAULT_LENGTH );
			} else if ( pType->isBinary( ) ) {
				objConfig.SetAttrType( szName, udBaseConfig::UDSDK_TYPE_BLOB );
			}
		}

		pType = NULL;
	} // void GetConfig

	udPType CUdBase::GetAttrValue( string name, const udBaseConfig& objConfig, cfgVal mode ) {
		return this->FilterAttr( name, objConfig, mode );
	} // udPType GetAttrValue

	udPXmlNode CUdBase::GetXml( udPXmlDoc doc, const udBaseConfig& config ) {
		udPXmlNode	ret			= doc->CreateNode( UdSDK::toWString( config.GetXmlNodeName( ) ) ),
					node		= NULL;
		string		szAttr		= "";
		udPType		pTmp		= NULL;
		if ( ret ) {
			for( udLstPairType::iterator i = m_lstAttr.begin( ), j = m_lstAttr.end( ); i != j; ++i ) {
				szAttr		= i->first;
				pTmp		= this->FilterAttr( szAttr, config, CUdBase::UDSDK_CONFIG_XML );

				if ( pTmp ) {
					if ( node = doc->CreateNode( UdSDK::toWString( config.GetAttrIndex( szAttr, udBaseConfig::UDSDK_CONFIG_XML ) ) ) ) {
						node->SetValue( wstring( L"<![CDATA[" ) + pTmp->toWString( ) + wstring( L"]]>" ) );
						ret->InsertChild( node );
					}

					delete pTmp;
					pTmp = NULL;
				}
			}
		}
		return ret;
	} // udPXmlNode GetXml

	udAscType CUdBase::GetSQLCreate( ) {
		udAscType arrRet;
		//	"query"		- строка для создания таблицы
		//	"attr"		- ассоциативный массив имен атрибутов
		//	"values"	- ассоциативный массив строк создания атрибутов
		udArrString		szValues;
		string			szTable		= "",
						szPrefix	= "",
						szAttr		= "",
						szName		= "",
						szAttrVal	= "",
						szValue		= "";
		udPType			pTmp		= NULL;
		udPArrPairStr	arrAttr		= new udArrPairStr,
						arrValues	= new udArrPairStr;
		udBaseConfig	objConfig;
		// выгребаем конфиг
		this->GetConfig( objConfig );
		szTable = objConfig.GetSQLTableName( );

		for( udLstPairType::iterator i = m_lstAttr.begin( ), j = m_lstAttr.end( ); i != j; ++i ) {
			szAttr		= i->first;
			szValue		= objConfig.GetAttrSQLCreate( szAttr );
			szAttrVal	= string( "`" ) + objConfig.GetAttrIndex( szAttr, udBaseConfig::UDSDK_CONFIG_DATABASE ) + string( "`" );
			
			arrAttr->push_back( udPairStrStr( szAttr, szAttrVal ) );
			arrValues->push_back( udPairStrStr( szAttr, szValue ) );
			szValues.push_back( szValue );
		}

		arrRet[ "attr"		] = new udType( ( udPVoid ) arrAttr		);
		arrRet[ "values"	] = new udType( ( udPVoid ) arrValues	);

		if ( !szTable.empty( ) && !szValues.empty( ) ) {
			arrRet[ "query" ] = new udType(
				string( "CREATE TABLE IF NOT EXISTS `" ) + szTable + string( "` (" ) +
				Join( ",", szValues ) + string( ") CHARACTER SET=utf8 COLLATE utf8_bin" )
			);
		}
		return arrRet;
	} // udMapType GetSQLCreate

	udAscType CUdBase::GetSQLSelect( ) {
		udAscType arrRet;
		//	"query"		- строка для создания таблицы
		//	"attr"		- ассоциативный массив имен атрибутов
		//	"values"	- ассоциативный массив строк создания атрибутов
		string			szTable		= "",
						szPrefix	= "",
						szAttr		= "",
						szName		= "",
						szAttrVal	= "",
						szValue		= "";
		udPType			pTmp		= NULL;
		udPArrPairStr	arrAttr		= new udArrPairStr,
						arrValues	= new udArrPairStr;
		udArrString		szAttrs, szValues;
		udBaseConfig	objConfig;
		// выгребаем конфиг
		this->GetConfig( objConfig );
		szTable = objConfig.GetSQLTableName( );

		for( udLstPairType::iterator i = m_lstAttr.begin( ), j = m_lstAttr.end( ); i != j; ++i ) {
			szAttr		= i->first;
			pTmp		= this->FilterAttr( szAttr, objConfig, CUdBase::UDSDK_CONFIG_DATABASE );

			if ( pTmp ) {
				szAttrVal	= string( "`" ) + objConfig.GetAttrIndex( szAttr, udBaseConfig::UDSDK_CONFIG_DATABASE ) + string( "`" );
				szValue		= pTmp->toString( );

				arrAttr->push_back( udPairStrStr( szAttr, szAttrVal ) );
				arrValues->push_back( udPairStrStr( szAttr, szValue ) );
				szAttrs.push_back( szAttrVal );
				szValues.push_back( szValue );
				delete pTmp;
				pTmp = NULL;
			}
		}

		arrRet[ "attr"		] = new udType( ( udPVoid ) arrAttr		);
		arrRet[ "values"	] = new udType( ( udPVoid ) arrValues	);

		if ( !szTable.empty( ) && !szAttrs.empty( ) && !szValues.empty( ) ) {
			string	szWhere		= "",
					szSelectBy	= objConfig.GetAttrSelect( );

			if ( !szSelectBy.empty( ) ) {
				szWhere = string( " WHERE `" ) + objConfig.GetAttrIndex( szSelectBy, udBaseConfig::UDSDK_CONFIG_DATABASE ) +
					string( "`=" ) + this->GetAttrValue( szSelectBy, objConfig, CUdBase::UDSDK_CONFIG_DATABASE )->toString( );
			}

			arrRet[ "table" ] = new udType( szTable );
			arrRet[ "query" ] = new udType(
				string( "SELECT " ) + Join( ",", szAttrs ) + string( " FROM `" ) + szTable +
				string( "`" ) + szWhere
			);
		}

		return arrRet;
	} // udMapType GetSQLSelect

	udAscType CUdBase::GetSQLInsert( ) {
		WriteUdBaseLog( "CUdBase::GetSQLInsert, enter\n" );
		udAscType arrRet;
		//	"query"		- строка для создания таблицы
		//	"attr"		- ассоциативный массив имен атрибутов
		//	"values"	- ассоциативный массив строк создания атрибутов
		string			szTable		= "",
						szPrefix	= "",
						szAttr		= "",
						szName		= "",
						szAttrVal	= "",
						szValue		= "";
		udPType			pTmp		= NULL;
		udPArrPairStr	arrAttr		= new udArrPairStr,
						arrValues	= new udArrPairStr;
		udArrString		szAttrs, szValues;
		udBaseConfig	objConfig;
		// выгребаем конфиг
		this->GetConfig( objConfig );
		szTable = objConfig.GetSQLTableName( );

		for( udLstPairType::iterator i = m_lstAttr.begin( ), j = m_lstAttr.end( ); i != j; ++i ) {
			szAttr		= i->first;
			pTmp		= this->FilterAttr( szAttr, objConfig, CUdBase::UDSDK_CONFIG_DATABASE );

			if ( pTmp ) {
				szAttrVal	= string( "`" ) + objConfig.GetAttrIndex( szAttr, udBaseConfig::UDSDK_CONFIG_DATABASE ) + string( "`" );
				szValue		= pTmp->toString( );

				arrAttr->push_back( udPairStrStr( szAttr, szAttrVal ) );
				arrValues->push_back( udPairStrStr( szAttr, szValue ) );
				szAttrs.push_back( szAttrVal );
				szValues.push_back( szValue );
				delete pTmp;
				pTmp = NULL;
			}
		}

		arrRet[ "attr"		] = new udType( ( udPVoid ) arrAttr	);
		arrRet[ "values"	] = new udType( ( udPVoid ) arrValues	);

		if ( !szTable.empty( ) && !szAttrs.empty( ) && !szValues.empty( ) ) {
			arrRet[ "query" ] = new udType(
				string( "INSERT INTO `" ) + szTable + string( "`(" ) + Join( ",", szAttrs ) +
				string( ") VALUES (" ) + Join( ",", szValues ) + string( ")" )
			);
		}

		WriteUdBaseLog( "CUdBase::GetSQLInsert, exit\n" );
		return arrRet;
	} // udMapType GetSQLInsert

	udAscType CUdBase::GetSQLUpdate( ) {
		udAscType arrRet;
		//	"query"		- строка для создания таблицы
		//	"attr"		- ассоциативный массив имен атрибутов
		//	"values"	- ассоциативный массив строк создания атрибутов
		string			szTable		= "",
						szPrefix	= "",
						szAttr		= "",
						szName		= "",
						szAttrVal	= "",
						szValue		= "";
		udPType			pTmp		= NULL;
		udPArrPairStr	arrAttr		= new udArrPairStr,
						arrValues	= new udArrPairStr;
		udArrString		szAttrs, szValues;
		udBaseConfig	objConfig;
		// выгребаем конфиг
		this->GetConfig( objConfig );
		szTable = objConfig.GetSQLTableName( );

		for( udLstPairType::iterator i = m_lstAttr.begin( ), j = m_lstAttr.end( ); i != j; ++i ) {
			szAttr		= i->first;
			//szPrefix	= this->GetAttributePrefix( szAttr, arrConfig, CUdBase::UDSDK_CONFIG_DATABASE );
			pTmp		= this->FilterAttr( szAttr, objConfig, CUdBase::UDSDK_CONFIG_DATABASE );

			if ( pTmp ) {
				szAttrVal	= string( "`" ) + objConfig.GetAttrIndex( szAttr, udBaseConfig::UDSDK_CONFIG_DATABASE ) + string( "`" );
				szValue		= pTmp->toString( );

				arrAttr->push_back( udPairStrStr( szAttr, szAttrVal ) );
				arrValues->push_back( udPairStrStr( szAttr, szValue ) );
				szAttrs.push_back( szAttrVal );
				szValues.push_back( szValue );
				delete pTmp;
				pTmp = NULL;
			}
		}

		arrRet[ "attr"		] = new udType( ( udPVoid ) arrAttr	);
		arrRet[ "values"	] = new udType( ( udPVoid ) arrValues	);

		if ( !szTable.empty( ) && !szAttrs.empty( ) && !szValues.empty( ) ) {
			arrRet[ "table" ] = new udType( szTable );

			udArrString arrUpdate;
			string		szUpdate	= "",
						szUpdateBy	= objConfig.GetAttrUpdate( );
			udInt		i,
						iNum		= szAttrs.size( );

			for( i = 0; i < iNum; ++i ) {
				arrUpdate.push_back( szAttrs[ i ] + string( "=" ) + szValues[ i ] );
			}

			szUpdate = string( "UPDATE `" ) + szTable + string( "` SET " ) + Join( ",", arrUpdate );

			if ( !szUpdateBy.empty( ) && ( pTmp = this->FilterAttr( szUpdateBy, objConfig, CUdBase::UDSDK_CONFIG_DATABASE ) ) ) {
				szUpdate += string( " WHERE `" ) + objConfig.GetAttrIndex( szUpdateBy, udBaseConfig::UDSDK_CONFIG_DATABASE ) + string( "`=" ) + pTmp->toString( );
			}

			arrRet[ "query" ] = new udType( szUpdate );
		}

		return arrRet;
	} // udMapType GetSQLUpdate

	udAscType CUdBase::GetSQLDelete( ) {
		udAscType arrRet;
		//	"query"		- строка для создания таблицы
		//	"attr"		- ассоциативный массив имен атрибутов
		//	"values"	- ассоциативный массив строк создания атрибутов
		//CUdBase::tConfig	arrConfig	= this->GetConfig( );
		//string	szTable		= this->GetSQLTableName( arrConfig );
		//CUdBase::cfgAttr::iterator i = arrConfig.common.find( CUdBase::UDSDK_CONFIG_DELETE );
		string			szTable		= "",
						szDeleteBy	= "";
		udBaseConfig	objConfig;
		// выгребаем конфиг
		this->GetConfig( objConfig );
		szTable = objConfig.GetSQLTableName( );
		szDeleteBy = objConfig.GetAttrDelete( );

		if ( !szDeleteBy.empty( ) ) {
			udPArrPairStr	arrAttr		= new udArrPairStr,
							arrValues	= new udArrPairStr;
			udPType		pType		= NULL;

			pType = this->FilterAttr( szDeleteBy, objConfig, CUdBase::UDSDK_CONFIG_DATABASE );
			if ( pType ) {
				arrAttr->push_back( udPairStrStr( szDeleteBy, string( "`" ) + objConfig.GetAttrIndex( szDeleteBy, udBaseConfig::UDSDK_CONFIG_DATABASE ) + string( "`" ) ) );
				arrValues->push_back( udPairStrStr( szDeleteBy, this->GetAttrValue( szDeleteBy, objConfig, CUdBase::UDSDK_CONFIG_DATABASE )->toString( ) ) );
			}

			arrRet[ "attr"		] = new udType( ( udPVoid ) arrAttr		);
			arrRet[ "values"	] = new udType( ( udPVoid ) arrValues	);
		}

		if ( !szTable.empty( ) && !szDeleteBy.empty( ) ) {
			udPType pType = this->GetAttrValue( szDeleteBy, objConfig, CUdBase::UDSDK_CONFIG_DATABASE );

			arrRet[ "table" ] = new udType( szTable );

			if ( pType ) {
				arrRet[ "query" ] = new udType(
					string( "DELETE FROM `" ) + szTable + string( "` WHERE `" ) +
					objConfig.GetAttrIndex( szDeleteBy, udBaseConfig::UDSDK_CONFIG_DATABASE ) +
					string( "`=" ) + pType->toString( )
				);
			}
		}

		return arrRet;
	} // udMapType GetSQLDelete

	udInt CUdBase::InitAttr( const string& name, const udAscType& vctInput, const udBaseConfig& objConfig, cfgVal iMode ) {
		string index = objConfig.GetAttrIndex( name, ( udBaseConfig::cfgVal ) iMode );
		udAscType::const_iterator i = vctInput.find( index );
		if ( i != vctInput.end( ) ) {
			udPType	pInput	= i->second,
					pCur	= m_mapAttr[ name ];
			udUInt	type	= objConfig.GetAttrType( name );
			// type будет использоваться, чтобы защититься от дурака, когда значение атрибута
			// будет не того типа, что указано в конфиге
			// приводим введенное значение к нужному типу
			if ( pCur->isInt( ) ) {
				pCur->ini( pInput->toInt( ) );
			} else if ( pCur->isFloat( ) ) {
				pCur->ini( pInput->toFloat( ) );
			} else if ( pCur->isDouble( ) ) {
				pCur->ini( pInput->toDouble( ) );
			} else if ( pCur->isUInt( ) ) {
				pCur->ini( pInput->toUInt( ) );
			} else if ( pCur->isWord( ) ) {
				pCur->ini( pInput->toWord( ) );
			} else if ( pCur->isDWord( ) ) {
				pCur->ini( pInput->toDWord( ) );
			} else if ( pCur->isString( ) ) {
				pCur->ini( pInput->toString( ) );
			} else if ( pCur->isWString( ) ) {
				pCur->ini( pInput->toWString( ) );
			} else if ( pCur->isTime( ) ) {
				pCur->ini( pInput->toTime( ) );
			} else if ( pCur->isBinary( ) ) {
				pCur->ini( pInput->toBinary( ) );
			}
		}

		return 0;
	} // udInt InitAttr

	udInt CUdBase::InitAttr( const string& name, udPMorph obj, const udBaseConfig& objConfig, cfgVal iMode ) {
		string index = objConfig.GetAttrIndex( name, ( udBaseConfig::cfgVal ) iMode );
		if ( obj->isset( index ) ) {
			udPType	pInput	= obj->get( index ),
					pCur	= m_mapAttr[ name ];
			udUInt	type	= objConfig.GetAttrType( name );
			// type будет использоваться, чтобы защититься от дурака, когда значение атрибута
			// будет не того типа, что указано в конфиге
			// приводим введенное значение к нужному типу
			if ( pCur->isInt( ) ) {
				pCur->ini( pInput->toInt( ) );
			} else if ( pCur->isFloat( ) ) {
				pCur->ini( pInput->toFloat( ) );
			} else if ( pCur->isDouble( ) ) {
				pCur->ini( pInput->toDouble( ) );
			} else if ( pCur->isUInt( ) ) {
				pCur->ini( pInput->toUInt( ) );
			} else if ( pCur->isWord( ) ) {
				pCur->ini( pInput->toWord( ) );
			} else if ( pCur->isDWord( ) ) {
				pCur->ini( pInput->toDWord( ) );
			} else if ( pCur->isString( ) ) {
				pCur->ini( pInput->toString( ) );
			} else if ( pCur->isWString( ) ) {
				pCur->ini( pInput->toWString( ) );
			} else if ( pCur->isTime( ) ) {
				pCur->ini( pInput->toTime( ) );
			} else if ( pCur->isBinary( ) ) {
				pCur->ini( pInput->toBinary( ) );
			}
		}

		return 0;
	} // udInt InitAttr

	udPType CUdBase::FilterAttr( const string& szName, const udBaseConfig& objConfig, cfgVal iMode ) {
		if ( this->isset( szName ) ) {
			udUInt	type	= objConfig.GetAttrType( szName );
			udPType	pInput	= m_mapAttr[ szName ];
			udPType	pRet	= new udType;

			// приведение типов
			if ( pInput->isInt( ) ) {
				pRet->ini( pInput->toInt( ) );
			} else if ( pInput->isFloat( ) ) {
				pRet->ini( pInput->toFloat( ) );
			} else if ( pInput->isDouble( ) ) {
				pRet->ini( pInput->toDouble( ) );
			} else if ( pInput->isUInt( ) ) {
				pRet->ini( pInput->toUInt( ) );
			} else if ( pInput->isWord( ) ) {
				pRet->ini( pInput->toWord( ) );
			} else if ( pInput->isDWord( ) ) {
				pRet->ini( pInput->toDWord( ) );
			} else if ( pInput->isString( ) ) {
				pRet->ini( pInput->toString( ) );
			} else if ( pInput->isWString( ) ) {
				pRet->ini( pInput->toWString( ) );
			} else if ( pInput->isTime( ) ) {
				pRet->ini( pInput->toTime( ) );
			} else if ( pInput->isBinary( ) ) {
				pRet->ini( pInput->toBinary( ) );
			}

			switch( iMode ) {
				case CUdBase::UDSDK_CONFIG_CPP:
					return pRet;
					break;

				case CUdBase::UDSDK_CONFIG_DATABASE:
					if ( ( type & udBaseConfig::UDSDK_TYPE_STRING ) || ( type & udBaseConfig::UDSDK_TYPE_TEXT ) ) {
						string szStr = ConvertString( pInput->toWString( ), CP_UTF8 );
						if ( szStr.size( ) ) {
							udDWord	dwLen	= szStr.size( ) * 2;
							udPChar szTmp	= new udChar[ dwLen + 1 ];

							memset( szTmp, 0, dwLen + 1 );
							UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

							szStr = szTmp;

							delete [ ] szTmp;
						}

						delete pRet;
						pRet = new udType( string( "'" ) + szStr + string( "'" ) );
					} else if ( type & udBaseConfig::UDSDK_TYPE_DATETIME ) {
						delete pRet;
						pRet = new udType( string( "'" ) + pInput->toString( ) + string( "'" ) );
					} else if ( type & udBaseConfig::UDSDK_TYPE_BLOB ) {
						delete pRet;
						pRet = new udType( string( "'" ) + pInput->toString( ) + string( "'" ) );
					}
					return pRet;
					break;

				default:
					break;
			}
			return pRet;
		}
		return NULL;
	} // udPType FilterAttr

	void CUdBase::ClearSQL( udAscType& arrData ) {
		udPType			pType	= NULL;
		udPArrPairStr	pArr	= NULL;
		udAscType::iterator i;

		if ( ( i = arrData.find( "attr" ) ) != arrData.end( ) ) {
			pType	= i->second;
			pArr	= ( udPArrPairStr ) pType->get( );

			pArr->clear( );	// сносим строки
			delete pArr;	// сносим массив строк из памяти
			pType->ini( );	// зануляем указатель на данные
			delete pType;	// сносим память, занятую параметром
		}

		if ( ( i = arrData.find( "values" ) ) != arrData.end( ) ) {
			pType	= i->second;
			pArr	= ( udPArrPairStr ) pType->get( );

			pArr->clear( );	// сносим строки
			delete pArr;	// сносим массив строк из памяти
			pType->ini( );	// зануляем указатель на данные
			delete pType;	// сносим память, занятую параметром
		}
		
		if ( ( i = arrData.find( "query" ) ) != arrData.end( ) ) {
			pType = i->second;
			string *pStr = ( string* ) pType->get( );

			pStr->clear( );
			delete pStr;
			pType->ini( );
			delete pType;
		}

		if ( ( i = arrData.find( "table" ) ) != arrData.end( ) ) {
			pType = i->second;
			string *pStr = ( string* ) pType->get( );

			pStr->clear( );
			delete pStr;
			pType->ini( );
			delete pType;
		}

		arrData.clear( );
	} // void ClearSQL

} // namespace UdSDK
