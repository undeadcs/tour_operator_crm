
#include "includes.h"

namespace UdSDK {

	CUdXmlNodeList::CUdXmlNodeList( ) {
	}

	CUdXmlNodeList::CUdXmlNodeList( const CUdXmlNodeList& objCopy ) {
		for( map< udDWord, udPXmlNode >::const_iterator i = objCopy.m_dwaNode.begin( ); i != objCopy.m_dwaNode.end( ); ++i ) {
			m_dwaNode[ i->first ] = i->second;
		}
	}

	CUdXmlNodeList::~CUdXmlNodeList( ) {
		this->clear( );
	}

	CUdXmlNodeList& CUdXmlNodeList::operator =( const CUdXmlNodeList& objCopy ) {
		for( map< udDWord, udPXmlNode >::const_iterator i = objCopy.m_dwaNode.begin( ); i != objCopy.m_dwaNode.end( ); ++i ) {
			m_dwaNode[ i->first ] = i->second;
		}
		return *this;
	}

	bool CUdXmlNodeList::empty( ) const {
		return m_dwaNode.empty( );
	} // bool empty

	void CUdXmlNodeList::clear( ) {
		m_dwaNode.clear( );
	} // void clear

	void CUdXmlNodeList::insert( udPXmlNode node, udInt at ) {
		if ( at == InsAtAfter ) {
			m_dwaNode[ m_dwaNode.size( ) ] = node;
		} else if ( at == InsAtBefore ) {
			this->ShiftForNew( 0 );
			m_dwaNode[ 0 ] = node;
			/*vector< udPXmlNode > vctNode;
			vctNode.push_back( node );
			for( map< udDWord, udPXmlNode >::iterator i = m_dwaNode.begin( ); i != m_dwaNode.end( ); ++i ) {
				vctNode.push_back( i->second );
			}
			m_dwaNode.clear( );
			for( vector< udPXmlNode >::iterator i = vctNode.begin( ); i != vctNode.end( ); ++i ) {
				m_dwaNode[ m_dwaNode.size( ) ] = *i;
			}
			vctNode.clear( );*/
		}
	} // void insert

	void CUdXmlNodeList::insert( udPXmlNode pos, udPXmlNode node, udInt at ) {
		bool	found	= false;
		udDWord	index	= 0;
		for( map< udDWord, udPXmlNode >::iterator i = m_dwaNode.begin( ); i != m_dwaNode.end( ); ++i ) {
			if ( i->second == pos ) {
				found = true;
				index = i->first;
				break;
			}
		}
		if ( found ) { // относительный узел найден
			if ( at == InsAtBefore ) {
				if ( index == 0 ) { // вставка в начало
					this->insert( node, InsAtBefore );
				} else {
					this->ShiftForNew( index );
					m_dwaNode[ index ] = node;
				}
			} else if ( at == InsAtAfter ) {
				if ( index == ( m_dwaNode.size( ) - 1 ) ) { // вставка в конец
					this->insert( node, InsAtAfter );
				} else {
					this->ShiftForNew( index + 1 );
					m_dwaNode[ index + 1 ] = node;
				}
			}
		}
	} // void insert

	void CUdXmlNodeList::del( udDWord index ) {
		map< udDWord, udPXmlNode >::iterator i = m_dwaNode.find( index );
		if ( i != m_dwaNode.end( ) ) {
			m_dwaNode.erase( i );
			this->reorder( );
		}
	} // void del

	void CUdXmlNodeList::del( udPXmlNode node ) {
		for( map< udDWord, udPXmlNode >::iterator i = m_dwaNode.begin( ); i != m_dwaNode.end( ); ++i ) {
			if ( i->second == node ) {
				m_dwaNode.erase( i );
				this->reorder( );
				break;
			}
		}
	} // void del

	udPXmlNode CUdXmlNodeList::get( udDWord index ) const {
		map< udDWord, udPXmlNode >::const_iterator i = m_dwaNode.find( index );
		if ( i != m_dwaNode.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPXmlNode get

	udXmlNodeList CUdXmlNodeList::get( const wstring& pattern ) const {
		udXmlNodeList ret;
		for( map< udDWord, udPXmlNode >::const_iterator i  = m_dwaNode.begin( ); i != m_dwaNode.end( ); ++i ) {
			if ( i->second->GetName( ) == pattern ) {
				ret.insert( i->second );
			}
		}
		return ret;
	} // udXmlNodeList get

	void CUdXmlNodeList::toString( string& out ) const {
		for( map< udDWord, udPXmlNode >::const_iterator i  = m_dwaNode.begin( ); i != m_dwaNode.end( ); ++i ) {
			i->second->toString( out );
		}
	} // void toString

	void CUdXmlNodeList::toString( wstring& out ) const {
		for( map< udDWord, udPXmlNode >::const_iterator i  = m_dwaNode.begin( ); i != m_dwaNode.end( ); ++i ) {
			i->second->toString( out );
		}
	} // void toString

	void CUdXmlNodeList::ShiftForNew( udDWord index ) {
		vector< udPXmlNode > vctNode;
		for( udDWord i = index; i < m_dwaNode.size( ); ++i ) {
			vctNode.push_back( m_dwaNode[ i ] );
		}
		m_dwaNode[ index ] = NULL;
		for( udDWord i = 0; i < vctNode.size( ); ++i ) {
			m_dwaNode[ index + i + 1 ] = vctNode[ i ];
		}
		vctNode.clear( );
	} // void ShiftForNew

	void CUdXmlNodeList::reorder( ) {
		vector< udPXmlNode > vctNode;
		for( map< udDWord, udPXmlNode >::iterator i = m_dwaNode.begin( ); i != m_dwaNode.end( ); ++i ) {
			vctNode.push_back( i->second );
		}
		for( udDWord i = 0; i < vctNode.size( ); ++i ) {
			m_dwaNode[ i ] = vctNode[ i ];
		}
		vctNode.clear( );
	} // void reorder

} // namespace UdSDK
