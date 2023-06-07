/**
	UndeadCS SDK
	Базовая библиотека

	xml - узел
*/
#ifndef UDNEADCS_SDK_LIBRARY_XML_NODE_H
#define UDNEADCS_SDK_LIBRARY_XML_NODE_H

namespace UdSDK {

	class CUdXmlNode {
		wstring			m_szName,		// имя
						m_szValue;		// значение
		udPXmlNode		m_pParent;		// родительский узел
		udPXmlAttrList	m_lstAttr;		// атрибуты
		udPXmlNodeList	m_lstChildren;	// потомки

	public:
		CUdXmlNode( );
		CUdXmlNode( const wstring& name, udPXmlNode pParent );
		CUdXmlNode( const CUdXmlNode& objCopy );

		virtual ~CUdXmlNode( );

		CUdXmlNode& operator =( const CUdXmlNode& objCopy );

		void	SetName( const wstring& name );
		void	SetValue( const wstring& value );
		void	SetParentNode( udPXmlNode pParent );
		void	SetAttribute( const wstring& name, const wstring& value );
		void	InsertChild( udPXmlNode node, udInt at = InsAtAfter );
		void	InsertChild( udPXmlNode pos, udPXmlNode node, udInt at = InsAtAfter );
		void	RemoveChild( udDWord index );
		void	RemoveChild( udPXmlNode node );

		void	toString( string& out ) const;
		void	toString( wstring& out ) const;

		udPXmlAttr	GetAttribute( const wstring& name ) const;
		wstring		GetAttributeValue( const wstring& name ) const;
		wstring		GetName( )			const { return m_szName;	}
		udPXmlNode	GetParentNodex( )	const { return m_pParent;	}

	}; // class CUdXmlNode

} // namespace UdSDK

#endif
