
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHSTour::CHSTour( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_stour";
		m_szUpdIndex	= "id";
		m_szName		= "stour";
		m_fnGetObject	= GetProjectSTour;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новое состояние тура";
		m_szAddModal	= L"Добавить состояние тура";
		m_szFormTitle	= L"Состояние тура";
		m_fnGetFormHandler	= GetHSTourForm;
	}

	CHSTour::CHSTour( const CHSTour& objCopy ) {
	}

	CHSTour::~CHSTour( ) {
	}

	CHSTour& CHSTour::operator =( const CHSTour& objCopy ) {
		return *this;
	}

} // namespace UdSDK