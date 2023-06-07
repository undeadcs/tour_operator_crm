
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHHostSide::CHHostSide( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_hostside";
		m_szUpdIndex	= "id";
		m_szName		= "hostside";
		m_fnGetObject	= GetProjectHostSide;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHHostSideForm;
		m_szAddModeless	= L"Новая прин. сторона";
		m_szAddModal	= L"Добавить прин. сторону";
		m_szFormTitle	= L"Принимающая сторона";
	}

	CHHostSide::CHHostSide( const CHHostSide& objCopy ) {
	}

	CHHostSide::~CHHostSide( ) {
	}

	CHHostSide& CHHostSide::operator =( const CHHostSide& objCopy ) {
		return *this;
	}

} // namespace UdSDK