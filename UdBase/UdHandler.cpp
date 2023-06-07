
#include "includes.h"

namespace UdSDK {

	CUdHandler::CUdHandler( udPDatabase pDatabase ) {
		m_pDatabase = pDatabase;
	}

	CUdHandler::CUdHandler( const CUdHandler& objCopy ) {
	}

	CUdHandler::~CUdHandler( ) {
		m_pDatabase = NULL;
	}

	CUdHandler& CUdHandler::operator =( const CUdHandler& objCopy ) {
		return *this;
	} // CHandler& operator =

	void CUdHandler::CheckTable( const udHandlerOption& objOption, udPArrError arrError, udPArrString arrQuery ) {
		if ( this->m_pDatabase && objOption.issetTable( ) && objOption.issetClass( ) ) {
			pfnGetObject vGetObject = objOption.GetClass( );
			udPBase pObject = vGetObject( );
			if ( pObject ) {
				udAscType	arrCreate	= pObject->GetSQLCreate( );
				string		szTable		= objOption.GetTable( ),
							szQuery		= arrCreate[ "query" ]->toString( );

				if ( objOption.issetIgnoreAttr( ) || objOption.issetOnlyAttr( ) || objOption.ForceTable( ) ) {
					udPArrPairStr	pValue		= ( udPArrPairStr ) arrCreate[ "values" ]->get( );
					udPArrString	pIgn		= NULL;
					udArrString		arrValue;
					bool			bDel		= false;
					udArrPairStr::iterator	i, j;

					if ( objOption.issetIgnoreAttr( ) ) {
						objOption.ApplyIgnoreAttr( pValue );
					}
					if ( objOption.issetOnlyAttr( ) ) {
					}

					if ( !pValue->empty( ) ) {
						for( i = pValue->begin( ), j = pValue->end( ); i != j; ++i ) {
							arrValue.push_back( i->second );
						}
						szQuery = string( "CREATE TABLE IF NOT EXISTS `" ) + szTable + string( "` (" ) +
							Join( ",", arrValue ) + string( ") CHARACTER SET=utf8 COLLATE utf8_bin" );
						arrValue.clear( );
					}
				}

				if ( arrQuery ) {
					arrQuery->push_back( szQuery );
				}

				if ( !m_pDatabase->Query( szQuery ) && arrError ) {
					arrError->push_back( m_pDatabase->GetError( ) );
				}

				udBase::ClearSQL( arrCreate );
				delete pObject;
			}
		}
	} // void CheckTable

	void CUdHandler::GetUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError, udPArrString arrQuery ) {
		if ( this->m_pDatabase && objOption.issetTable( ) && objOption.issetClass( ) ) {
			pfnGetObject	fnCreateObject	= objOption.GetClass( );
			string			szTable			= objOption.GetTable( ),
							szAttr			= "*";
			udPBase			pObject			= fnCreateObject( );
			udAscType		arrSelect		= pObject->GetSQLSelect( );
			udPArrPairStr	pAttr			= ( udPArrPairStr ) arrSelect[ "attr" ]->get( );
			assert( pObject );

			WriteUdBaseLog( "CUdHandler::GetUdObject, check ignore attributes\n" );
			// фильтруем по игнорируемым атрибутам
			if ( objOption.issetIgnoreAttr( ) || objOption.issetOnlyAttr( ) ) {
				if ( objOption.issetIgnoreAttr( ) ) {
					objOption.ApplyIgnoreAttr( pAttr );
				}
				if ( objOption.issetOnlyAttr( ) ) {
				}
			}
			if ( pAttr->empty( ) ) {
				return;
			} else {
				udArrString arrAttr;
				for( udArrPairStr::iterator i = pAttr->begin( ), j = pAttr->end( ); i != j; ++i ) {
					arrAttr.push_back( i->second );
				}
				szAttr = Join( ",", arrAttr );
				arrAttr.clear( );
				if ( objOption.issetLeftJoin( ) ) {
					string szJoinAttr = objOption.GetLeftJoinAttr( );
					if ( !szJoinAttr.empty( ) ) {
						szAttr += "," + szJoinAttr;
					}
				}
			}
			udBase::ClearSQL( arrSelect );
			udDELETE( pObject );
			WriteUdBaseLog( "CUdHandler::GetUdObject, build SQL query\n" );
			// строим запрос
			string		szQuery = "SELECT ";
			if ( objOption.issetSqlCalcFoundRows( ) ) {
				szQuery += "SQL_CALC_FOUND_ROWS ";
			}
			szQuery += szAttr + " FROM " + szTable;
			udArrString	arrTail;
			if ( objOption.issetLeftJoin( ) ) {
				szQuery += " " + objOption.GetLeftJoinTable( );
			}
			if ( objOption.issetWhere( ) ) {
				arrTail.push_back( string( "WHERE " ) + objOption.GetWhere( ) );
			}
			if ( objOption.issetGroup( ) ) {
				arrTail.push_back( string( "GROUP BY " ) + objOption.GetGroup( ) );
			}
			if ( objOption.issetOrder( ) ) {
				arrTail.push_back( string( "ORDER BY " ) + objOption.GetOrder( ) );
			}
			if ( objOption.issetLimit( ) ) {
				arrTail.push_back( string( "LIMIT " ) + objOption.GetLimit( ) );
			}
			if ( !arrTail.empty( ) ) {
				szQuery += string( " " ) + Join( " ", arrTail );
			}

			if ( arrQuery ) {
				arrQuery->push_back( szQuery );
			}

			WriteUdBaseLog( "CUdHandler::GetUdObject, executing query\n" );

			if ( objOption.issetLeftJoin( ) ) {
				udArrMorph arrRow;
				if ( m_pDatabase->GetAssoc( szQuery, arrRow ) ) {
					udPBase		pObject		= NULL;
					udPMorph	pDbObject	= NULL;
					for( udArrMorph::iterator i = arrRow.begin( ); i != arrRow.end( ); ++i ) {
						pDbObject = *i;
						pObject = fnCreateObject( );
						pObject->Create( pDbObject );
						arrObject.push_back( pObject );
					}
					ClearArrMorph( arrRow );
				} else if ( arrError ) {
					arrError->push_back( m_pDatabase->GetError( ) );
				}
			} else {
				udArrAsc arrRow;
				if ( m_pDatabase->GetAssoc( szQuery, arrRow ) ) {
					udPBase pObject = NULL;
					for( udArrAsc::iterator i = arrRow.begin( ); i != arrRow.end( ); ++i ) {
						pObject = fnCreateObject( );
						if ( pObject != NULL ) {
							pObject->Create( *i );
							arrObject.push_back( pObject );
						}
					}
					ClearArrAssoc( arrRow );
				} else if ( arrError ) {
					arrError->push_back( m_pDatabase->GetError( ) );
				}
			}
		} else if ( arrError ) {
			if ( !this->m_pDatabase ) {
				arrError->push_back( string( "отсутствует объект работы с СУБД\n" ) );
			}
			if ( !objOption.issetTable( ) ) {
				arrError->push_back( string( "не указана таблица для выборки объектов\n" ) );
			}
			if ( !objOption.issetClass( ) ) {
				arrError->push_back( string( "не указана функция создания объекта\n" ) );
			}
		}
	} // void GetUdObject

	void CUdHandler::AddUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError, udPArrString arrQuery ) {
		if ( this->m_pDatabase && objOption.issetTable( ) ) {
			udArrString		arrValue, arrAttr, arrValueTmp;
			udPArrPairStr	pAttr		= NULL,
							pValue		= NULL;
			string			szAttr		= "",
							szTable		= objOption.GetTable( );
			udPBase			pObject		= NULL;
			udAscType		arrInsert;

			udArrString::iterator v, w;
			udArrPairStr::iterator i1, j1;
			udPArrString	pIgn	= NULL;
			bool			bDel	= false;

			for( udArrBase::iterator i = arrObject.begin( ), j = arrObject.end( ); i != j; ++i ) {
				pObject = *i;
				if ( pObject ) {
					arrInsert	= pObject->GetSQLInsert( );
					pAttr		= ( udPArrPairStr ) arrInsert[ "attr"	]->get( );
					pValue		= ( udPArrPairStr ) arrInsert[ "values"	]->get( );

					if ( objOption.issetIgnoreAttr( ) ) {
						objOption.ApplyIgnoreAttr( pAttr, pValue );
					}
					if ( objOption.issetOnlyAttr( ) ) {
					}
					if ( !pAttr->empty( ) ) {
						for( i1 = pAttr->begin( ); i1 != pAttr->end( ); ++i1 ) {
							arrAttr.push_back( i1->second );
						}
						
						szAttr = Join( ",", arrAttr );
						arrAttr.clear( );
					}

					for( i1 = pValue->begin( ); i1 != pValue->end( ); ++i1 ) {
						arrValueTmp.push_back( i1->second );
					}

					if ( !arrValueTmp.empty( ) ) {
						arrValue.push_back( string( "(" ) + Join( ",", arrValueTmp ) + string( ")" ) );
					}

					arrValueTmp.clear( );
					udBase::ClearSQL( arrInsert );
				}
			}

			if ( !arrValue.empty( ) && !szAttr.empty( ) ) {
				string szQuery;

				szQuery = string( "INSERT INTO `" ) + szTable + string( "`(" ) + szAttr +
					string( ") VALUES " ) + Join( ",", arrValue );
				
				WriteUdLibLog( szQuery + "\n" );

				if ( arrQuery ) {
					arrQuery->push_back( szQuery );
				}

				if ( !m_pDatabase->Query( szQuery ) && arrError ) {
					arrError->push_back( m_pDatabase->GetError( ) );
				}
			}
		}
	} // void AddUdObject

	void CUdHandler::AddUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udArrString& arrQuery ) {
		if ( objOption.issetTable( ) ) {
			udArrString		arrValue, arrAttr, arrValueTmp;
			udPArrPairStr	pAttr		= NULL,
							pValue		= NULL;
			string			szAttr		= "",
							szTable		= objOption.GetTable( );
			udPBase			pObject		= NULL;
			udAscType		arrInsert;

			udArrString::iterator v, w;
			udArrPairStr::iterator i1, j1;

			for( udArrBase::iterator i = arrObject.begin( ), j = arrObject.end( ); i != j; ++i ) {
				pObject = *i;
				if ( pObject ) {
					arrInsert	= pObject->GetSQLInsert( );
					pAttr		= ( udPArrPairStr ) arrInsert[ "attr"	]->get( );
					pValue		= ( udPArrPairStr ) arrInsert[ "values"	]->get( );

					if ( objOption.issetIgnoreAttr( ) ) {
						objOption.ApplyIgnoreAttr( pAttr, pValue );
					}
					if ( objOption.issetOnlyAttr( ) ) {
					}
					if ( !pAttr->empty( ) ) {
						for( i1 = pAttr->begin( ); i1 != pAttr->end( ); ++i1 ) {
							arrAttr.push_back( i1->second );
						}
						
						szAttr = Join( ",", arrAttr );
						arrAttr.clear( );
					}

					for( i1 = pValue->begin( ); i1 != pValue->end( ); ++i1 ) {
						arrValueTmp.push_back( i1->second );
					}

					if ( !arrValueTmp.empty( ) ) {
						arrValue.push_back( string( "(" ) + Join( ",", arrValueTmp ) + string( ")" ) );
					}

					arrValueTmp.clear( );
					udBase::ClearSQL( arrInsert );
				}
			}

			if ( !arrValue.empty( ) && !szAttr.empty( ) ) {
				string szQuery = string( "INSERT INTO `" ) + szTable + string( "`(" ) + szAttr +
					string( ") VALUES " ) + Join( ",", arrValue );

				arrQuery.push_back( szQuery );
			}
		}
	} // void AddUdObject

	void CUdHandler::AddUdObject( udPBase obj, const udHandlerOption& option, udPArrError arrError, udPArrString arrQuery ) {
		if ( option.issetTable( ) && obj ) {
			udArrString		arrValue, arrAttr, arrValueTmp;
			udPArrPairStr	pAttr		= NULL,
							pValue		= NULL;
			string			szAttr		= "",
							szTable		= option.GetTable( );
			udPBase			pObject		= NULL;
			udAscType		arrInsert;

			udArrString::iterator v, w;
			udArrPairStr::iterator i1, j1;

			arrInsert	= obj->GetSQLInsert( );
			pAttr		= ( udPArrPairStr ) arrInsert[ "attr"	]->get( );
			pValue		= ( udPArrPairStr ) arrInsert[ "values"	]->get( );

			if ( option.issetIgnoreAttr( ) ) {
				option.ApplyIgnoreAttr( pAttr, pValue );
			}
			if ( option.issetOnlyAttr( ) ) {
			}
			if ( !pAttr->empty( ) ) {
				for( i1 = pAttr->begin( ); i1 != pAttr->end( ); ++i1 ) {
					arrAttr.push_back( i1->second );
				}
				
				szAttr = Join( ",", arrAttr );
				arrAttr.clear( );
			}
			for( i1 = pValue->begin( ); i1 != pValue->end( ); ++i1 ) {
				arrValueTmp.push_back( i1->second );
			}
			if ( !arrValueTmp.empty( ) ) {
				arrValue.push_back( string( "(" ) + Join( ",", arrValueTmp ) + string( ")" ) );
			}

			arrValueTmp.clear( );
			udBase::ClearSQL( arrInsert );
			
			if ( !arrValue.empty( ) && !szAttr.empty( ) ) {
				string szQuery = string( "INSERT INTO `" ) + szTable + string( "`(" ) + szAttr +
					string( ") VALUES " ) + Join( ",", arrValue );

				WriteUdLibLog( szQuery + "\n" );

				if ( arrQuery ) {
					arrQuery->push_back( szQuery );
				}
				if ( !m_pDatabase->Query( szQuery ) && arrError ) {
					arrError->push_back( m_pDatabase->GetError( ) );
				}
			}
		}
	} // void AddUdObject

	void CUdHandler::DelUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError, udPArrString arrQuery ) {
		if ( this->m_pDatabase && objOption.issetTable( ) ) {
			string			szAttrDel	= "",
							szTable		= objOption.GetTable( );
			udArrString		arrToDel;
			udPBase			pObject		= NULL;
			udAscType		arrDelete;
			udAscType::iterator	itDel;

			for( udArrBase::iterator i = arrObject.begin( ), j = arrObject.end( ); i != j; ++i ) {
				pObject = *i;
				if ( pObject ) {
					arrDelete = pObject->GetSQLDelete( );

					if ( szAttrDel.empty( ) && ( ( itDel = arrDelete.find( "attr" ) ) != arrDelete.end( ) ) ) {
						szAttrDel = ( *( ( udPArrPairStr ) itDel->second->get( ) ) )[ 0 ].second;
					}

					if ( ( itDel = arrDelete.find( "values" ) ) != arrDelete.end( ) ) {
						arrToDel.push_back( ( *( ( udPArrPairStr ) itDel->second->get( ) ) )[ 0 ].second );
					}

					udBase::ClearSQL( arrDelete );
				}
			}
			if ( !szTable.empty( ) && !szAttrDel.empty( ) && !arrToDel.empty( ) ) {
				string szQuery = "";

				szQuery = string( "DELETE FROM `" ) + szTable + string( "` WHERE " ) + szAttrDel +
					string( " IN(" ) + Join( ",", arrToDel ) + string( ")" );

				if ( arrQuery ) {
					arrQuery->push_back( szQuery );
				}

				if ( !m_pDatabase->Query( szQuery ) && arrError ) {
					arrError->push_back( m_pDatabase->GetError( ) );
				}
			}
		}
	} // void DelUdObject

	void CUdHandler::DelUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udArrString& arrQuery ) {
		if ( objOption.issetTable( ) ) {
			string			szAttrDel	= "",
							szTable		= objOption.GetTable( );
			udArrString		arrToDel;
			udPBase			pObject		= NULL;
			udAscType		arrDelete;
			udAscType::iterator	itDel;

			for( udArrBase::iterator i = arrObject.begin( ), j = arrObject.end( ); i != j; ++i ) {
				pObject = *i;
				if ( pObject ) {
					arrDelete = pObject->GetSQLDelete( );

					if ( szAttrDel.empty( ) && ( ( itDel = arrDelete.find( "attr" ) ) != arrDelete.end( ) ) ) {
						szAttrDel = ( *( ( udPArrPairStr ) itDel->second->get( ) ) )[ 0 ].second;
					}

					if ( ( itDel = arrDelete.find( "values" ) ) != arrDelete.end( ) ) {
						arrToDel.push_back( ( *( ( udPArrPairStr ) itDel->second->get( ) ) )[ 0 ].second );
					}

					udBase::ClearSQL( arrDelete );
				}
			}
			if ( !szTable.empty( ) && !szAttrDel.empty( ) && !arrToDel.empty( ) ) {
				string szQuery = string( "DELETE FROM `" ) + szTable + string( "` WHERE " ) + szAttrDel +
					string( " IN(" ) + Join( ",", arrToDel ) + string( ")" );

				arrQuery.push_back( szQuery );
			}
		}
	} // void DelUdObject

	void CUdHandler::DelUdObject( udPBase obj, const udHandlerOption& option, udPArrError arrError, udPArrString arrQuery ) {
		if ( this->m_pDatabase && option.issetTable( ) && obj ) {
			string			szAttrDel	= "",
							szTable		= option.GetTable( );
			udArrString		arrToDel;
			udPBase			pObject		= NULL;
			udAscType		arrDelete;
			udAscType::iterator	itDel;

			arrDelete = obj->GetSQLDelete( );
			if ( szAttrDel.empty( ) && ( ( itDel = arrDelete.find( "attr" ) ) != arrDelete.end( ) ) ) {
				szAttrDel = ( *( ( udPArrPairStr ) itDel->second->get( ) ) )[ 0 ].second;
			}
			if ( ( itDel = arrDelete.find( "values" ) ) != arrDelete.end( ) ) {
				arrToDel.push_back( ( *( ( udPArrPairStr ) itDel->second->get( ) ) )[ 0 ].second );
			}
			udBase::ClearSQL( arrDelete );

			if ( !szTable.empty( ) && !szAttrDel.empty( ) && !arrToDel.empty( ) ) {
				string szQuery = "";

				szQuery = string( "DELETE FROM `" ) + szTable + string( "` WHERE " ) + szAttrDel +
					string( " IN(" ) + Join( ",", arrToDel ) + string( ")" );

				WriteUdLibLog( szQuery + "\n" );

				if ( arrQuery ) {
					arrQuery->push_back( szQuery );
				}

				if ( !m_pDatabase->Query( szQuery ) && arrError ) {
					arrError->push_back( m_pDatabase->GetError( ) );
				}
			}
		}
	} // void DelUdObject

	void CUdHandler::UpdUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError, udPArrString arrQuery ) {
		if ( this->m_pDatabase && objOption.issetTable( ) && objOption.issetIndexAttr( ) ) {
			string			szQuery		= "",
							szUpdBy		= "",
							szTable		= objOption.GetTable( ),
							szIndex		= objOption.GetIndexAttr( );
			udPArrPairStr	pAttr		= NULL,
							pValue		= NULL;
			udAscType		arrUpdate;
			udPBase			pObject		= NULL;
			udDWord			iIndex		= 0;
			udArrPairStr::iterator	itAttr, itValue, itDel;
			udArrString::iterator	itStrIgn;
			udPArrString	pArrIgn = NULL;
			udArrString		arrStrTmp;

			for( udArrBase::iterator i = arrObject.begin( ), j = arrObject.end( ); i != j; ++i ) {
				pObject = *i;
				if ( pObject ) {
					arrUpdate	= pObject->GetSQLUpdate( );
					pAttr		= ( udPArrPairStr ) arrUpdate[ "attr"	]->get( );
					pValue		= ( udPArrPairStr ) arrUpdate[ "values"	]->get( );

					for( itAttr = pAttr->begin( ); itAttr != pAttr->end( ); ++itAttr ) {
						if ( itAttr->first == szIndex ) {
							break;
						}
					}
					for( itValue = pValue->begin( ); itValue != pValue->end( ); ++itValue ) {
						if ( itValue->first == szIndex ) {
							break;
						}
					}

					szUpdBy = itAttr->second + string( "=" ) + itValue->second;
					pAttr->erase( itAttr );
					pValue->erase( itValue );

					if ( objOption.issetIgnoreAttr( ) ) {
						objOption.ApplyIgnoreAttr( pAttr, pValue );
					}
					if ( objOption.issetOnlyAttr( ) ) {
					}

					for( iIndex = 0; iIndex < pAttr->size( ); ++iIndex ) {
						arrStrTmp.push_back( ( *pAttr )[ iIndex ].second + string( "=" ) + ( *pValue )[ iIndex ].second );
					}

					szQuery = string( "UPDATE `" ) + szTable + string( "` SET " ) + Join( ",", arrStrTmp ) +
						string( " WHERE " ) + szUpdBy;
					//WriteUdLibLog( szQuery + "\n" );

					if ( arrQuery ) {
						arrQuery->push_back( szQuery );
					}

					if ( !m_pDatabase->Query( szQuery ) && arrError ) {
						arrError->push_back( m_pDatabase->GetError( ) );
					}

					udBase::ClearSQL( arrUpdate );
					arrStrTmp.clear( );
				}
			}
		}
	} // void UpdUdObject

	void CUdHandler::UpdUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udArrString& arrQuery ) {
		if ( objOption.issetTable( ) && objOption.issetIndexAttr( ) ) {
			string			szQuery		= "",
							szUpdBy		= "",
							szTable		= objOption.GetTable( ),
							szIndex		= objOption.GetIndexAttr( );
			udPArrPairStr	pAttr		= NULL,
							pValue		= NULL;
			udAscType		arrUpdate;
			udPBase			pObject		= NULL;
			udDWord			iIndex		= 0;
			udArrPairStr::iterator	itAttr, itValue, itDel;
			udArrString::iterator	itStrIgn;
			udPArrString	pArrIgn = NULL;
			udArrString		arrStrTmp;

			for( udArrBase::iterator i = arrObject.begin( ), j = arrObject.end( ); i != j; ++i ) {
				pObject = *i;
				if ( pObject ) {
					arrUpdate	= pObject->GetSQLUpdate( );
					pAttr		= ( udPArrPairStr ) arrUpdate[ "attr"	]->get( );
					pValue		= ( udPArrPairStr ) arrUpdate[ "values"	]->get( );

					for( itAttr = pAttr->begin( ); itAttr != pAttr->end( ); ++itAttr ) {
						if ( itAttr->first == szIndex ) {
							break;
						}
					}
					for( itValue = pValue->begin( ); itValue != pValue->end( ); ++itValue ) {
						if ( itValue->first == szIndex ) {
							break;
						}
					}

					szUpdBy = itAttr->second + string( "=" ) + itValue->second;
					pAttr->erase( itAttr );
					pValue->erase( itValue );

					if ( objOption.issetIgnoreAttr( ) ) {
						objOption.ApplyIgnoreAttr( pAttr, pValue );
					}
					if ( objOption.issetOnlyAttr( ) ) {
					}

					for( iIndex = 0; iIndex < pAttr->size( ); ++iIndex ) {
						arrStrTmp.push_back( ( *pAttr )[ iIndex ].second + string( "=" ) + ( *pValue )[ iIndex ].second );
					}

					szQuery = string( "UPDATE `" ) + szTable + string( "` SET " ) + Join( ",", arrStrTmp ) +
						string( " WHERE " ) + szUpdBy;

					arrQuery.push_back( szQuery );

					udBase::ClearSQL( arrUpdate );
					arrStrTmp.clear( );
				}
			}
		}
	} // void UpdUdObject

	void CUdHandler::UpdUdObject( udPBase obj, const udHandlerOption& option, udPArrError arrError, udPArrString arrQuery ) {
		if ( this->m_pDatabase && option.issetTable( ) && option.issetIndexAttr( ) && obj ) {
			string			szQuery		= "",
							szUpdBy		= "",
							szTable		= option.GetTable( ),
							szIndex		= option.GetIndexAttr( );
			udPArrPairStr	pAttr		= NULL,
							pValue		= NULL;
			udAscType		arrUpdate;
			udPBase			pObject		= NULL;
			udDWord			iIndex		= 0;
			udArrPairStr::iterator	itAttr, itValue, itDel;
			udArrString::iterator	itStrIgn;
			udPArrString	pArrIgn = NULL;
			udArrString		arrStrTmp;
			
			arrUpdate	= obj->GetSQLUpdate( );
			pAttr		= ( udPArrPairStr ) arrUpdate[ "attr"	]->get( );
			pValue		= ( udPArrPairStr ) arrUpdate[ "values"	]->get( );

			for( itAttr = pAttr->begin( ); itAttr != pAttr->end( ); ++itAttr ) {
				if ( itAttr->first == szIndex ) {
					break;
				}
			}
			for( itValue = pValue->begin( ); itValue != pValue->end( ); ++itValue ) {
				if ( itValue->first == szIndex ) {
					break;
				}
			}

			szUpdBy = itAttr->second + string( "=" ) + itValue->second;
			pAttr->erase( itAttr );
			pValue->erase( itValue );

			if ( option.issetIgnoreAttr( ) ) {
				option.ApplyIgnoreAttr( pAttr, pValue );
			}
			if ( option.issetOnlyAttr( ) ) {
			}

			for( iIndex = 0; iIndex < pAttr->size( ); ++iIndex ) {
				arrStrTmp.push_back( ( *pAttr )[ iIndex ].second + string( "=" ) + ( *pValue )[ iIndex ].second );
			}

			szQuery = string( "UPDATE `" ) + szTable + string( "` SET " ) + Join( ",", arrStrTmp ) +
				string( " WHERE " ) + szUpdBy;
			
			WriteUdLibLog( szQuery + "\n" );

			if ( arrQuery ) {
				arrQuery->push_back( szQuery );
			}

			if ( !m_pDatabase->Query( szQuery ) && arrError ) {
				arrError->push_back( m_pDatabase->GetError( ) );
			}

			udBase::ClearSQL( arrUpdate );
			arrStrTmp.clear( );
		}
	} // void UpdUdObject

	udLongLong CUdHandler::CountUdObject( const udHandlerOption& objOption ) {
		if ( this->m_pDatabase && objOption.issetTable( ) ) {
			string szQuery = string( "SELECT COUNT(*) cnt FROM `" ) + objOption.GetTable( ) + string( "`" );
			if ( objOption.issetWhere( ) ) {
				szQuery += string( " WHERE " ) + objOption.GetWhere( );
			}
			udArrAsc arrRow;
			if ( m_pDatabase->GetAssoc( szQuery, arrRow ) && !arrRow.empty( ) ) {
				udLongLong lRet = arrRow[ 0 ][ "cnt" ]->toLLong( );
				ClearArrAssoc( arrRow );
				return lRet;
			}
		}
		return 0L;
	} // udDWord CountUdObject

} // namespace UdSDK
