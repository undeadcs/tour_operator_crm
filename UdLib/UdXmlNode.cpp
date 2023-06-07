
#include "includes.h"

namespace UdSDK {

	CUdXmlNode::CUdXmlNode( ) {
		m_szName		= L"";
		m_pParent		= NULL;
		m_lstAttr		= new udXmlAttrList;
		m_lstChildren	= new udXmlNodeList;
	}

	CUdXmlNode::CUdXmlNode( const wstring& name, udPXmlNode pParent ) {
		m_szName	= name;
		m_pParent	= pParent;
		m_lstAttr		= new udXmlAttrList;
		m_lstChildren	= new udXmlNodeList;
	}

	CUdXmlNode::CUdXmlNode( const CUdXmlNode& objCopy ) {
		m_szName		= objCopy.m_szName;
		m_pParent		= objCopy.m_pParent;
		m_lstAttr		= new udXmlAttrList;
		m_lstChildren	= new udXmlNodeList;
		*m_lstAttr		= *( objCopy.m_lstAttr );
		*m_lstChildren	= *( objCopy.m_lstChildren );
	}

	CUdXmlNode::~CUdXmlNode( ) {
		udDELETE( m_lstAttr );
		udDELETE( m_lstChildren );
	}

	CUdXmlNode& CUdXmlNode::operator =( const CUdXmlNode& objCopy ) {
		m_szName		= objCopy.m_szName;
		m_pParent		= objCopy.m_pParent;
		m_lstAttr->clear( );
		m_lstChildren->clear( );
		*m_lstAttr		= *( objCopy.m_lstAttr );
		*m_lstChildren	= *( objCopy.m_lstChildren );
		return *this;
	}

	void CUdXmlNode::SetName( const wstring& name ) {
		m_szName = name;
	} // void SetName

	void CUdXmlNode::SetValue( const wstring& value ) {
		m_szValue = value;
	} // void SetValue

	void CUdXmlNode::SetParentNode( udPXmlNode pParent ) {
		m_pParent = pParent;
		/*if ( m_pParent ) {
			m_pParent->RemoveChild( this );
		}
		if ( pParent ) {
			pParent->InsertChild( this );
			m_pParent = pParent;
		}*/
	} // void SetParentNode

	void CUdXmlNode::SetAttribute( const wstring& name, const wstring& value ) {
		m_lstAttr->set( name, value );
	} // void SetAttribute

	void CUdXmlNode::InsertChild( udPXmlNode node, udInt at ) {
		m_lstChildren->insert( node, at );
	} // void InsertChild

	void CUdXmlNode::InsertChild( udPXmlNode pos, udPXmlNode node, udInt at ) {
		m_lstChildren->insert( pos, node, at );
	} // void InsertChild

	void CUdXmlNode::RemoveChild( udDWord index ) {
		m_lstChildren->del( index );
	} // void RemoveChild

	void CUdXmlNode::RemoveChild( udPXmlNode node ) {
		m_lstChildren->del( node );
	} // void RemoveChild

	void CUdXmlNode::toString( string& out ) const {
		wstring str;
		this->toString( str );
		out += ConvertString( str, CP_UTF8 );
	} // void toString

	void CUdXmlNode::toString( wstring& out ) const {
		out += L"<" + m_szName;
		if ( !m_lstAttr->empty( ) ) {
			out += L" ";
			m_lstAttr->toString( out );
		}
		if ( m_lstChildren->empty( ) && m_szValue.empty( ) ) {
			out += L" />";
		} else {
			out += L">" + m_szValue;
			m_lstChildren->toString( out );
			out += L"</" + m_szName + L">";
		}
	} // void toString

	udPXmlAttr CUdXmlNode::GetAttribute( const wstring& name ) const {
		return m_lstAttr->get( name );
	} // udPXmlAttr GetAttribute

	wstring CUdXmlNode::GetAttributeValue( const wstring& name ) const {
		udPXmlAttr attr = m_lstAttr->get( name );
		if ( attr ) {
			return attr->GetValue( );
		}
		return L"";
	} // wstring GetAttributeValue

} // namespace UdSDK
