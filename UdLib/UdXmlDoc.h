/**
	UndeadCS SDK
	Базовая библиотека

	xml - документ
*/
#ifndef UDNEADCS_SDK_LIBRARY_XML_DOC_H
#define UDNEADCS_SDK_LIBRARY_XML_DOC_H

namespace UdSDK {

	class CUdXmlDoc {
		udArrPXmlNode	m_arrNode;
		udPXmlNode		m_pRoot;
		
		typedef pair< wstring, udPMorph > xml_decl;
		typedef vector< xml_decl > vct_xml_decl;

		vct_xml_decl	m_arrDeclaration;	// типа таких: <?xml...

	public:
		CUdXmlDoc( );
		CUdXmlDoc( const CUdXmlDoc& objCopy );

		virtual ~CUdXmlDoc( );

		CUdXmlDoc& operator =( const CUdXmlDoc& objCopy );

		void		AddDeclaration( const wstring& name, udPMorph attr );
		void		SetRoot( udPXmlNode node );
		udPXmlNode	CreateNode( const wstring& name );
		void		toString( string& out ) const;
		void		toString( wstring& out ) const;

	}; // class CUdXmlDoc

} // namespace UdSDK

#endif
