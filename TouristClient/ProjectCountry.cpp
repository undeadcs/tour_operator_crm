
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHCountry::CHCountry( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_country";
		m_szUpdIndex	= "id";
		m_szName		= "country";
		m_fnGetObject	= GetProjectCountry;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHCountryForm;
		m_szAddModeless	= L"Новая страна";
		m_szAddModal	= L"Добавить страну";
		m_szFormTitle	= L"Страна";
	}

	CHCountry::CHCountry( const CHCountry& objCopy ) {
	}

	CHCountry::~CHCountry( ) {
	}

	CHCountry& CHCountry::operator =( const CHCountry& objCopy ) {
		return *this;
	}

} // namespace UdSDK
