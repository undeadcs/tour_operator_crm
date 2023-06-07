
#include "includes.h"

namespace UdSDK {

	CUdXmlDoc::CUdXmlDoc( ) {
		m_pRoot	= NULL;
	}

	CUdXmlDoc::CUdXmlDoc( const CUdXmlDoc& objCopy ) {
	}

	CUdXmlDoc::~CUdXmlDoc( ) {
		for( vct_xml_decl::iterator i = m_arrDeclaration.begin( ); i != m_arrDeclaration.end( ); ++i ) {
			delete i->second;
		}
		m_arrDeclaration.clear( );
		for( udArrPXmlNode::iterator i = m_arrNode.begin( ); i != m_arrNode.end( ); ++i ) {
			delete *i;
		}
		m_arrNode.clear( );
	}

	CUdXmlDoc& CUdXmlDoc::operator =( const CUdXmlDoc& objCopy ) {
		return *this;
	}

	void CUdXmlDoc::AddDeclaration( const wstring& name, udPMorph attr ) {
		if ( !attr || name.empty( ) ) {
			return;
		}
		udPMorph obj = new udMorph;
		attr->apply( obj );
		m_arrDeclaration.push_back( xml_decl( name, obj ) );
	} // void AddDeclaration

	void CUdXmlDoc::SetRoot( udPXmlNode node ) {
		for( udArrPXmlNode::iterator i = m_arrNode.begin( ); i != m_arrNode.end( ); ++i ) {
			if ( *i == node ) {
				m_pRoot = node;
				break;
			}
		}
	} // void SetRoot

	udPXmlNode CUdXmlDoc::CreateNode( const wstring& name ) {
		udPXmlNode ret = new udXmlNode( name, NULL );
		m_arrNode.push_back( ret );
		return ret;
	} // udPXmlNode CreateNode

	void CUdXmlDoc::toString( string& out ) const {
		wstring str;
		this->toString( str );
		out += ConvertString( str, CP_UTF8 );
	} // void toString

	void CUdXmlDoc::toString( wstring& out ) const {
		const udLstPairType* lst = NULL;
		for( vct_xml_decl::const_iterator i = m_arrDeclaration.begin( ); i != m_arrDeclaration.end( ); ++i ) {
			out += L"<?" + i->first + L" ";
			lst = i->second->get_attr_list( );
			for( udLstPairType::const_iterator j = lst->begin( ); j != lst->end( ); ++j ) {
				out += toWString( j->first ) + L"=\"" + j->second->toWString( ) + L"\" ";
			}
			out += L"?>";
		}
		//out += L"<?xml version=\"1.0\"?>";
		if ( m_pRoot ) {
			m_pRoot->toString( out );
		}
	} // void toString

} // namespace UdSDK
