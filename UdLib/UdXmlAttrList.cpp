
#include "includes.h"

namespace UdSDK {

	CUdXmlAttrList::CUdXmlAttrList( ) {
	}

	CUdXmlAttrList::CUdXmlAttrList( const CUdXmlAttrList& objCopy ) {
	}

	CUdXmlAttrList::~CUdXmlAttrList( ) {
		this->clear( );
	}

	CUdXmlAttrList& CUdXmlAttrList::operator =( const CUdXmlAttrList& objCopy ) {
		udPXmlAttr attr = NULL;
		for( map< udDWord, udPXmlAttr >::const_iterator i = objCopy.m_dwaAttr.begin( ); i != objCopy.m_dwaAttr.end( ); ++i ) {
			attr = new udXmlAttr;
			*attr = *( i->second );
			m_dwaAttr[ i->first ] = attr;
			m_ascAttr[ attr->GetName( ) ] = attr;
		}
		return *this;
	}

	bool CUdXmlAttrList::empty( ) const {
		return m_dwaAttr.empty( );
	} // bool empty

	void CUdXmlAttrList::clear( ) {
		for( map< udDWord, udPXmlAttr >::iterator i = m_dwaAttr.begin( ); i != m_dwaAttr.end( ); ++i ) {
			delete i->second;
		}
		m_dwaAttr.clear( );
		m_ascAttr.clear( );
	} // void clear

	void CUdXmlAttrList::set( const wstring& name, const wstring& value ) {
		map< wstring, udPXmlAttr >::iterator i = m_ascAttr.find( name );
		if ( i == m_ascAttr.end( ) ) { // атрибут отсутствует
			udPXmlAttr attr = new udXmlAttr( name, value );
			m_dwaAttr[ m_dwaAttr.size( ) ] = attr; // при каждом удалении происходит перетасовка
			m_ascAttr[ name ] = attr;
		} else {
			m_ascAttr[ name ]->SetValue( value );
		}
	} // void set

	udPXmlAttr CUdXmlAttrList::get( udDWord index ) const {
		map< udDWord, udPXmlAttr >::const_iterator i = m_dwaAttr.find( index );
		if ( i != m_dwaAttr.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPXmlAttr get

	udPXmlAttr CUdXmlAttrList::get( const wstring& name ) const {
		map< wstring, udPXmlAttr >::const_iterator i = m_ascAttr.find( name );
		if ( i != m_ascAttr.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPXmlAttr get

	udInt CUdXmlAttrList::del( udDWord index ) {
		map< udDWord, udPXmlAttr >::iterator i = m_dwaAttr.find( index );
		if ( i != m_dwaAttr.end( ) ) {
			map< wstring, udPXmlAttr >::iterator j = m_ascAttr.find( i->second->GetName( ) );
			if ( j != m_ascAttr.end( ) ) {
				m_ascAttr.erase( j );
			}
			delete i->second;
			m_dwaAttr.erase( i );
			this->reorder( );
			return 1;
		}
		return 0;
	} // udInt del

	udInt CUdXmlAttrList::del( const wstring& name ) {
		map< wstring, udPXmlAttr >::iterator i = m_ascAttr.find( name );
		if ( i != m_ascAttr.end( ) ) {
			for( map< udDWord, udPXmlAttr >::iterator j = m_dwaAttr.begin( ); j != m_dwaAttr.end( ); ++j ) {
				if ( j->second->GetName( ) == name ) {
					m_dwaAttr.erase( j );
					break;
				}
			}
			delete i->second;
			m_ascAttr.erase( i );
			this->reorder( );
			return 1;
		}
		return 0;
	} // udInt del

	udArrXmlAttr CUdXmlAttrList::match_name( const wstring& pattern ) {
		udArrXmlAttr ret;
		udXmlAttr attr;
		for( map< udDWord, udPXmlAttr >::iterator i = m_dwaAttr.begin( ); i != m_dwaAttr.end( ); ++i ) {
			if ( i->second->GetName( ) == pattern ) {
				attr = *( i->second );
				ret.push_back( attr );
			}
		}
		return ret;
	} // udArrXmlAttr match_name
	
	udArrXmlAttr CUdXmlAttrList::match_value( const wstring& pattern ) {
		udArrXmlAttr ret;
		udXmlAttr attr;
		for( map< udDWord, udPXmlAttr >::iterator i = m_dwaAttr.begin( ); i != m_dwaAttr.end( ); ++i ) {
			if ( i->second->GetValue( ) == pattern ) {
				attr = *( i->second );
				ret.push_back( attr );
			}
		}
		return ret;
	} // udArrXmlAttr match_value

	void CUdXmlAttrList::toString( string& out ) const {
		wstring str;
		this->toString( str );
		out += ConvertString( str, CP_UTF8 );
	} // void toString

	void CUdXmlAttrList::toString( wstring& out ) const {
		udArrWString arrString;
		for( map< udDWord, udPXmlAttr >::const_iterator i = m_dwaAttr.begin( ); i != m_dwaAttr.end( ); ++i ) {
			arrString.push_back( i->second->toWString( ) );
		}
		out += Join( L" ", arrString );
	} // void toString

	void CUdXmlAttrList::reorder( ) {
		vector< udPXmlAttr > vctAttr;
		for( map< udDWord, udPXmlAttr >::iterator i = m_dwaAttr.begin( ); i != m_dwaAttr.end( ); ++i ) {
			vctAttr.push_back( i->second );
		}
		m_dwaAttr.clear( );
		for( vector< udPXmlAttr >::iterator i = vctAttr.begin( ); i != vctAttr.end( ); ++i ) {
			m_dwaAttr[ m_dwaAttr.size( ) ] = *i;
		}
		vctAttr.clear( );
	} // void reorder

} // namespace UdSDK
