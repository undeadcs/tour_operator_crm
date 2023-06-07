
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHDirect::CHDirect( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_direct";
		m_szUpdIndex	= "id";
		m_szName		= "direct";
		m_fnGetObject	= GetProjectDirect;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новое направление";
		m_szAddModal	= L"Добавить направление";
		m_szFormTitle	= L"Направление";
		m_fnGetFormHandler	= GetHDirectForm;
	}

	CHDirect::CHDirect( const CHDirect& objCopy ) {
	}

	CHDirect::~CHDirect( ) {
	}

	CHDirect& CHDirect::operator =( const CHDirect& objCopy ) {
		return *this;
	}

} // namespace UdSDK
