
#include "includes.h"

namespace UdSDK {

	CUdBaseConfig::CUdBaseConfig( ) {
	}

	CUdBaseConfig::CUdBaseConfig( const CUdBaseConfig& objCopy ) {
	}

	CUdBaseConfig::~CUdBaseConfig( ) {
		if ( !m_mapCommon.empty( ) ) {
			for( cfgAttr::iterator i = m_mapCommon.begin( ); i != m_mapCommon.end( ); ++i ) {
				delete i->second;
			}
			m_mapCommon.clear( );
		}
		if ( !m_mapAttr.empty( ) ) {
			cfgAttr::iterator j;
			for( cfgArray::iterator i = m_mapAttr.begin( ); i != m_mapAttr.end( ); ++i ) {
				for( j = i->second.begin( ); j != i->second.end( ); ++j ) {
					delete j->second;
				}
				i->second.clear( );
			}
			m_mapAttr.clear( );
		}
	}

	CUdBaseConfig& CUdBaseConfig::operator =( const CUdBaseConfig& objCopy ) {
		return *this;
	}

	void CUdBaseConfig::Clear( ) {
		if ( !m_mapCommon.empty( ) ) {
			for( cfgAttr::iterator i = m_mapCommon.begin( ); i != m_mapCommon.end( ); ++i ) {
				delete i->second;
			}
			m_mapCommon.clear( );
		}
		if ( !m_mapAttr.empty( ) ) {
			cfgAttr::iterator j;
			for( cfgArray::iterator i = m_mapAttr.begin( ); i != m_mapAttr.end( ); ++i ) {
				for( j = i->second.begin( ); j != i->second.end( ); ++j ) {
					delete j->second;
				}
				i->second.clear( );
			}
			m_mapAttr.clear( );
		}
	} // void Clear

	void CUdBaseConfig::SetSQLTable( const string& table ) {
		cfgAttr::iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_TABLE );
		if ( i != m_mapCommon.end( ) ) {
			i->second->ini( table );
		} else {
			m_mapCommon[ CUdBaseConfig::UDSDK_CONFIG_TABLE ] = new udType( table );
		}
	} // void SetTable

	void CUdBaseConfig::SetPrefix( const string& prefix ) {
		cfgAttr::iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_PREFIX );
		if ( i != m_mapCommon.end( ) ) {
			i->second->ini( prefix );
		} else {
			m_mapCommon[ CUdBaseConfig::UDSDK_CONFIG_PREFIX ] = new udType( prefix );
		}
	} // void SetPrefix

	void CUdBaseConfig::SetSelectBy( const string& name ) {
		cfgAttr::iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_SELECT );
		if ( i != m_mapCommon.end( ) ) {
			i->second->ini( name );
		} else {
			m_mapCommon[ CUdBaseConfig::UDSDK_CONFIG_SELECT ] = new udType( name );
		}
	} // void SetSelectBy

	void CUdBaseConfig::SetUpdateBy( const string& name ) {
		cfgAttr::iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_UPDATE );
		if ( i != m_mapCommon.end( ) ) {
			i->second->ini( name );
		} else {
			m_mapCommon[ CUdBaseConfig::UDSDK_CONFIG_UPDATE ] = new udType( name );
		}
	} // void SetUpdateBy

	void CUdBaseConfig::SetDeleteBy( const string& name ) {
		cfgAttr::iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_DELETE );
		if ( i != m_mapCommon.end( ) ) {
			i->second->ini( name );
		} else {
			m_mapCommon[ CUdBaseConfig::UDSDK_CONFIG_DELETE ] = new udType( name );
		}
	} // void SetDeleteBy

	void CUdBaseConfig::SetAttrType( const string& name, cfgType type ) {
		cfgArray::iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_TYPE );
			if ( j != i->second.end( ) ) {
				j->second->ini( ( udUInt ) type );
			} else {
				m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_TYPE ] = new udType( ( udUInt ) type );
			}
		} else {
			m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_TYPE ] = new udType( ( udUInt ) type );
		}
	} // void SetAttrType

	void CUdBaseConfig::SetAttrDigits( const string& name, udUInt digits ) {
		cfgArray::iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DIGITS );
			if ( j != i->second.end( ) ) {
				j->second->ini( digits );
			} else {
				m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_DIGITS ] = new udType( digits );
			}
		} else {
			m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_DIGITS ] = new udType( digits );
		}
		//m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_DIGITS ] = new udType( digits );
	} // void SetAttrDigits

	void CUdBaseConfig::SetAttrDecimals( const string& name, udUInt decimals ) {
		cfgArray::iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DECIMALS );
			if ( j != i->second.end( ) ) {
				j->second->ini( decimals );
			} else {
				m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_DECIMALS ] = new udType( decimals );
			}
		} else {
			m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_DECIMALS ] = new udType( decimals );
		}
		//m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_DECIMALS ] = new udType( decimals );
	} // void SetAttrDecimals

	void CUdBaseConfig::SetAttrLength( const string& name, udUInt length ) {
		cfgArray::iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_LENGTH );
			if ( j != i->second.end( ) ) {
				j->second->ini( length );
			} else {
				m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_LENGTH ] = new udType( length );
			}
		} else {
			m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_LENGTH ] = new udType( length );
		}
		//m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_LENGTH ] = new udType( length );
	} // void SetAttrLength

	void CUdBaseConfig::SetAttrTitle( const string& name, string title ) {
		cfgArray::iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_TITLE );
			if ( j != i->second.end( ) ) {
				j->second->ini( title );
			} else {
				m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_TITLE ] = new udType( title );
			}
		} else {
			m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_TITLE ] = new udType( title );
		}
		//m_mapAttr[ name ][ CUdBaseConfig::UDSDK_CONFIG_TITLE ] = new udType( title );
	} // void SetAttrTitle

	void CUdBaseConfig::SetXmlNodeName( const wstring& name ) {
		cfgAttr::iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_XMLNODENAME );
		if ( i != m_mapCommon.end( ) ) {
			i->second->ini( name );
		} else {
			m_mapCommon[ CUdBaseConfig::UDSDK_CONFIG_XMLNODENAME ] = new udType( name );
		}
	} // void SetXmlNodeName

	udUInt CUdBaseConfig::GetAttrType( const string& name ) const {
		cfgArray::const_iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::const_iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_TYPE );
			if ( j != i->second.end( ) ) {
				return j->second->toUInt( );
			}
		}
		return 0;
	} // udUInt GetAttrType

	string CUdBaseConfig::GetAttrPrefix( const string& name, cfgVal mode ) const {
		string ret = "";
		cfgArray::const_iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::const_iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_PREFIX );
			if ( j != i->second.end( ) ) {
				ret = j->second->toString( );
			} else {
				j = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_PREFIX );
				if ( j != m_mapCommon.end( ) ) {
					ret = j->second->toString( );
				}
			}
		}
		return ret;
	} // string GetAttrIndex

	string CUdBaseConfig::GetAttrIndex( const string& name, cfgVal mode ) const {
		string prefix = this->GetAttrPrefix( name, mode );
		return ( prefix + name );
	} // string GetAttrIndex

	wstring CUdBaseConfig::GetAttrTitle( const string& name, cfgVal mode ) const {
		wstring ret = L"";
		cfgArray::const_iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::const_iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_TITLE );
			if ( j != i->second.end( ) ) {
				ret = j->second->toWString( );
			} else {
				ret = ConvertString( name );
			}
		}
		return ret;
	} // wstring GetAttrTitle

	string CUdBaseConfig::GetSQLTableName( ) const {
		cfgAttr::const_iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_TABLE );
		if ( i != m_mapCommon.end( ) ) {
			return i->second->toString( );
		}
		return "";
	} // string GetSQLTableName

	string CUdBaseConfig::GetAttrSQLCreate( const string& name ) const {
		cfgArray::const_iterator i = m_mapAttr.find( name );
		if ( i != m_mapAttr.end( ) ) {
			cfgAttr::const_iterator j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_TYPE );
			if ( j != i->second.end( ) ) {
				udUInt		type		= j->second->toUInt( );
				udArrString	arrString;

				if ( type & CUdBaseConfig::UDSDK_TYPE_INT ) {
					string tmp( "INT" );
					j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DIGITS );
					if ( j != i->second.end( ) ) { // настройка есть
						tmp += '(';
						tmp += j->second->toString( );
						tmp += ')';
					}
					arrString.push_back( tmp );
					if ( type & CUdBaseConfig::UDSDK_TYPE_UNSIGNED ) {
						arrString.push_back( "UNSIGNED" );
					}
				} else if ( type & CUdBaseConfig::UDSDK_TYPE_FLOAT ) {
					string tmp( "FLOAT" );
					j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DIGITS );
					if ( j != i->second.end( ) ) { // настройка есть
						string szDigits = j->second->toString( );
						j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DECIMALS );
						if ( j != i->second.end( ) ) { // настройка есть
							tmp += '(';
							tmp += szDigits;
							tmp += ',';
							tmp += j->second->toString( );
							tmp += ')';
						}
					}
					arrString.push_back( tmp );
				} else if ( type & CUdBaseConfig::UDSDK_TYPE_DOUBLE ) {
					string tmp( "DOUBLE" );
					j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DIGITS );
					if ( j != i->second.end( ) ) { // настройка есть
						string szDigits = j->second->toString( );
						j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DECIMALS );
						if ( j != i->second.end( ) ) { // настройка есть
							tmp += '(';
							tmp += szDigits;
							tmp += ',';
							tmp += j->second->toString( );
							tmp += ')';
						}
					}
					arrString.push_back( tmp );
				} else if ( type & CUdBaseConfig::UDSDK_TYPE_STRING ) {
					string tmp( "VARCHAR" );
					j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_LENGTH );
					if ( j != i->second.end( ) ) { // настройка есть
						tmp += '(';
						tmp += j->second->toString( );
						tmp += ')';
					}
					arrString.push_back( tmp );
				} else if ( type & CUdBaseConfig::UDSDK_TYPE_TEXT ) {
					arrString.push_back( "TEXT" );
				} else if ( type & CUdBaseConfig::UDSDK_TYPE_DATE ) {
					if ( type & CUdBaseConfig::UDSDK_TYPE_TIME ) {
						arrString.push_back( "DATETIME" );
					} else {
						arrString.push_back( "DATE" );
					}
				} else if ( type & CUdBaseConfig::UDSDK_TYPE_TIME ) {
					arrString.push_back( "TIME" );
				} else if ( type & CUdBaseConfig::UDSDK_TYPE_BLOB ) {
					arrString.push_back( "BLOB" );
				} else {
					arrString.push_back( "VARCHAR(255)" );
				}
				if ( type & CUdBaseConfig::UDSDK_TYPE_NOTNULL && !( type & CUdBaseConfig::UDSDK_TYPE_BLOB ) ) {
					arrString.push_back( "NOT NULL" );
				}
				if ( type & CUdBaseConfig::UDSDK_TYPE_DEFAULT &&
					!( type & CUdBaseConfig::UDSDK_TYPE_BLOB ) &&
					!( type & CUdBaseConfig::UDSDK_TYPE_TEXT )
				) {
					j = i->second.find( CUdBaseConfig::UDSDK_CONFIG_DEFAULT );
					if ( j != i->second.end( ) ) { // настройка есть
						arrString.push_back(
							string( "DEFAULT '" ) + j->second->toString( ) + string( "'" )
						);
					}
				}
				if ( type & CUdBaseConfig::UDSDK_TYPE_AUTOINCREMENT &&
					!( type & CUdBaseConfig::UDSDK_TYPE_BLOB ) &&
					!( type & CUdBaseConfig::UDSDK_TYPE_TEXT )
				) {
					arrString.push_back( "AUTO_INCREMENT" );
				}
				if ( type & CUdBaseConfig::UDSDK_TYPE_PRIMARYKEY ) {
					arrString.push_back( "PRIMARY KEY" );
				}

				return (
					string( "`" ) + this->GetAttrIndex( name, CUdBaseConfig::UDSDK_CONFIG_DATABASE ) +
					string( "` " ) + Join( " ", arrString )
				);
			}
		}
		return "";
	} // string GetAttrSQLCreate

	string CUdBaseConfig::GetAttrSelect( ) const {
		cfgAttr::const_iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_SELECT );
		if ( i != m_mapCommon.end( ) ) {
			return i->second->toString( );
		}
		return "";
	} // string GetAttrSelect

	string CUdBaseConfig::GetAttrUpdate( ) const {
		cfgAttr::const_iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_UPDATE );
		if ( i != m_mapCommon.end( ) ) {
			return i->second->toString( );
		}
		return "";
	} // string GetAttrUpdate

	string CUdBaseConfig::GetAttrDelete( ) const {
		cfgAttr::const_iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_DELETE );
		if ( i != m_mapCommon.end( ) ) {
			return i->second->toString( );
		}
		return "";
	} // string GetAttrDelete

	wstring CUdBaseConfig::GetXmlNodeName( ) const {
		cfgAttr::const_iterator i = m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_XMLNODENAME );
		if ( i != m_mapCommon.end( ) ) {
			return i->second->toWString( );
		}
		return L"";
	} // wstring GetXmlNodeName

	bool CUdBaseConfig::issetSelect( ) const {
		return ( m_mapCommon.find( CUdBaseConfig::UDSDK_CONFIG_SELECT ) != m_mapCommon.end( ) );
	} // bool issetSelect

	void CUdBaseConfig::SetAttrCallback( const string& name, pfnGetObject fnGetObject ) {
	} // void SetAttrCallback

} // namespace UdSDK
