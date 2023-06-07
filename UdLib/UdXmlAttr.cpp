
#include "includes.h"

namespace UdSDK {

	CUdXmlAttr::CUdXmlAttr( ) {
		m_szName = m_szValue = L"";
	}

	CUdXmlAttr::CUdXmlAttr( const wstring& name, const wstring& val ) {
		m_szName	= name;
		m_szValue	= val;
	}

	CUdXmlAttr::CUdXmlAttr( const CUdXmlAttr& objCopy ) {
		m_szName	= objCopy.m_szName;
		m_szValue	= objCopy.m_szValue;
	}

	CUdXmlAttr::~CUdXmlAttr( ) {
	}

	CUdXmlAttr& CUdXmlAttr::operator =( const CUdXmlAttr& objCopy ) {
		m_szName	= objCopy.m_szName;
		m_szValue	= objCopy.m_szValue;
		return *this;
	}

	udInt CUdXmlAttr::SetName( const wstring& name ) {
		m_szName = name;
		return udXmlErrOk;
	} // udInt SetName

	udInt CUdXmlAttr::SetValue( const wstring& val ) {
		m_szValue = val;
		return udXmlErrOk;
	} // udInt SetValue

	string CUdXmlAttr::toString( ) const {
		return ConvertString( this->toWString( ), CP_UTF8 );
	} // string toString

	wstring CUdXmlAttr::toWString( ) const {
		return ( m_szName + L"=\"" + m_szValue + L"\"" );
	} // wstring toWString

} // namespace UdSDK