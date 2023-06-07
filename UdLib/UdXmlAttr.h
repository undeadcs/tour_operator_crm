/**
	UndeadCS SDK
	Базовая библиотека

	xml - атрибут узла
*/
#ifndef UDNEADCS_SDK_LIBRARY_XML_ATTR_H
#define UDNEADCS_SDK_LIBRARY_XML_ATTR_H

namespace UdSDK {

	class CUdXmlAttr {
		wstring		m_szName,	// имя
					m_szValue;	// значение

	public:
		CUdXmlAttr( );
		CUdXmlAttr( const wstring& name, const wstring& val );
		CUdXmlAttr( const CUdXmlAttr& objCopy );

		virtual ~CUdXmlAttr( );

		CUdXmlAttr& operator =( const CUdXmlAttr& objCopy );

		udInt	SetName( const wstring& name );
		udInt	SetValue( const wstring& val );

		string	toString( ) const;
		wstring	toWString( ) const;

		wstring	GetName( )	const { return m_szName;	}
		wstring	GetValue( )	const { return m_szValue;	}

	}; // class CUdXmlAttr

} // namespace UdSDK

#endif
