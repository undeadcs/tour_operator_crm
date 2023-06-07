
#include "includes.h"

namespace UdSDK {

	CUdHandlerOption::CUdHandlerOption( ) {
		m_szTable			= "";
		m_szIndexAttr		= "";
		m_szWhere			= "";
		m_szGroup			= "";
		m_szOrder			= "";
		m_szLimit			= "";
		m_fnCreateObject	= NULL;
		m_bForceTable		= false;
		m_bSqlCalcFoundRows	= false;
	}

	CUdHandlerOption::CUdHandlerOption( const CUdHandlerOption& objCopy ) {
	}

	CUdHandlerOption::~CUdHandlerOption( ) {
		m_arrIgnoreAttr.clear( );
		m_arrOnlyAttr.clear( );
		m_arrLeftJoin.clear( );
	}

	CUdHandlerOption& CUdHandlerOption::operator =( const CUdHandlerOption& objCopy ) {
		return *this;
	}

	void CUdHandlerOption::Clear( ) {
		m_szTable			= "";
		m_szIndexAttr		= "";
		m_szWhere			= "";
		m_szGroup			= "";
		m_szOrder			= "";
		m_szLimit			= "";
		m_fnCreateObject	= NULL;
		m_bForceTable		= false;

		m_arrIgnoreAttr.clear( );
		m_arrOnlyAttr.clear( );
		m_arrLeftJoin.clear( );
	} // void Clear

	void CUdHandlerOption::ApplyIgnoreAttr( udPArrPairStr pAttr ) const {
		WriteUdLibLog( "CUdHandlerOption::ApplyIgnoreAttr( udPArrPairStr pAttr )\n" );
		udArrPairStr::iterator j, k;
		bool del = false;
		for( udAscInt::const_iterator i = m_arrIgnoreAttr.begin( ); i != m_arrIgnoreAttr.end( ); ++i ) {
			for( j = pAttr->begin( ); j != pAttr->end( ); ++j ) {
				if ( j->first == i->first ) {
					k = j;
					del = true;
					break;
				}
			}
			if ( del ) {
				pAttr->erase( k );
				del = false;
			}
		}
	} // void ApplyIgnoreAttr

	void CUdHandlerOption::ApplyIgnoreAttr( udPArrPairStr pAttr, udPArrPairStr pValue ) const {
		WriteUdLibLog( "CUdHandlerOption::ApplyIgnoreAttr( udPArrPairStr pAttr, udPArrPairStr pValue )\n" );
		udArrPairStr::iterator j, k;
		bool del = false;
		WriteUdLibLog( "удаление атрибутов\n" );
		for( udAscInt::const_iterator i = m_arrIgnoreAttr.begin( ); i != m_arrIgnoreAttr.end( ); ++i ) {
			// атрибут
			for( j = pAttr->begin( ); j != pAttr->end( ); ++j ) {
				if ( j->first == i->first ) {
					k = j;
					del = true;
					break;
				}
			}
			if ( del ) {
				pAttr->erase( k );
				del = false;
				continue;
			}
		}
		del = false;
		WriteUdLibLog( "удаление значений\n" );
		for( udAscInt::const_iterator i = m_arrIgnoreAttr.begin( ); i != m_arrIgnoreAttr.end( ); ++i ) {
			// значение
			for( j = pValue->begin( ); j != pValue->end( ); ++j ) {
				if ( j->first == i->first ) {
					k = j;
					del = true;
					break;
				}
			}
			if ( del ) {
				pValue->erase( k );
				del = false;
				continue;
			}
		}
	} // void ApplyIgnoreAttr

	void CUdHandlerOption::SetTable( string name ) {
		m_szTable = name;
	} // void SetTable

	void CUdHandlerOption::SetIndexAttr( string name ) {
		m_szIndexAttr = name;
	} // void SetIndexAttr

	void CUdHandlerOption::SetWhere( string szWhere ) {
		m_szWhere = szWhere;
	} // void SetWhere

	void CUdHandlerOption::SetGroup( string szGroup ) {
		m_szGroup = szGroup;
	} // void SetGroup

	void CUdHandlerOption::SetOrder( string szOrder ) {
		m_szOrder = szOrder;
	} // void SetOrder

	void CUdHandlerOption::SetLimit( string szLimit ) {
		m_szLimit = szLimit;
	} // void SetLimit

	void CUdHandlerOption::SetClass( pfnGetObject pfnCreateObject ) {
		m_fnCreateObject = pfnCreateObject;
	} // void SetClass

	void CUdHandlerOption::SetIgnoreAttr( string name ) {
		udAscInt::iterator i = m_arrIgnoreAttr.find( name );
		if ( i == m_arrIgnoreAttr.end( ) ) {
			m_arrIgnoreAttr[ name ] = 1;
		}
	} // void SetIgnoreAttr

	void CUdHandlerOption::SetOnlyAttr( string name ) {
		udAscInt::iterator i = m_arrOnlyAttr.find( name );
		if ( i == m_arrOnlyAttr.end( ) ) {
			m_arrOnlyAttr[ name ] = 1;
		}
	} // void SetIgnoreAttr

	void CUdHandlerOption::SetForceTable( bool force ) {
		m_bForceTable = force;
	} // void SetForceTable

	void CUdHandlerOption::SetSqlCalcFoundRows( bool calc ) {
		m_bSqlCalcFoundRows = calc;
	} // void SetSqlCalcFoundRows
	
	void CUdHandlerOption::AddLeftJoin( const string& attr, const string& alias, const string& table, const string& condition, pfnGetObject fnGetObject ) {
		_lji objAdd;
		objAdd.attr			= attr;
		objAdd.alias		= alias;
		objAdd.table		= table;
		objAdd.condition	= condition;
		objAdd.fnGetObject	= fnGetObject;
		m_arrLeftJoin.push_back( objAdd );
	} // void AddLeftJoin

	void CUdHandlerOption::AddLeftJoin( const string& custom_attr, const string& custom_join ) {
		_lji objAdd;
		objAdd.custom_attr	= custom_attr;
		objAdd.custom_join	= custom_join;
		m_arrLeftJoin.push_back( objAdd );
	} // void AddLeftJoin

	string CUdHandlerOption::GetLeftJoinAttr( ) const {
		string ret = "";
		if ( !m_arrLeftJoin.empty( ) ) {
			udPBase obj = NULL;
			udBaseConfig config;
			udArrString arr;
			for( _vct_info::const_iterator i = m_arrLeftJoin.begin( ); i != m_arrLeftJoin.end( ); ++i ) {
				if ( !i->table.empty( ) && !i->attr.empty( ) ) {
					obj = i->fnGetObject( );
					obj->GetConfig( config );
					arr.push_back( "`" + i->table + "`.`" + config.GetAttrIndex( i->attr ) + "` AS `" + i->alias + "`" );
					config.Clear( );
					udDELETE( obj );
				} else if ( !i->custom_attr.empty( ) ) {
					arr.push_back( i->custom_attr );
				}
			}
			ret = Join( ",", arr );
			arr.clear( );
		}
		return ret;
	} // string ApplyLeftJoinAttr

	string CUdHandlerOption::GetLeftJoinTable( ) const {
		string ret = "";
		if ( !m_arrLeftJoin.empty( ) ) {
			udPBase obj = NULL;
			udBaseConfig config;
			udArrString arr;
			for( _vct_info::const_iterator i = m_arrLeftJoin.begin( ); i != m_arrLeftJoin.end( ); ++i ) {
				if ( !i->table.empty( ) && !i->condition.empty( ) ) {
					obj = i->fnGetObject( );
					obj->GetConfig( config );
					arr.push_back( "LEFT JOIN `" + i->table + "` ON " + i->condition );
					config.Clear( );
					udDELETE( obj );
				} else  if ( !i->custom_join.empty( ) ) {
					arr.push_back( i->custom_join );
				}
			}
			ret = Join( " ", arr );
			arr.clear( );
		}
		return ret;
	} // string GetLeftJoinTable

} // namespace UdSDK
