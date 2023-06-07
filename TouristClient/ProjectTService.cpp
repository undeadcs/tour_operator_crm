
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHTService::CHTService( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_tservice";
		m_szUpdIndex	= "id";
		m_szName		= "tservice";
		m_fnGetObject	= GetProjectTService;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новый тип услуги";
		m_szAddModal	= L"Добавить тип услуги";
		m_szFormTitle	= L"Тип услуги";
		m_fnGetFormHandler	= GetHTServiceForm;
	}

	CHTService::CHTService( const CHTService& objCopy ) {
	}

	CHTService::~CHTService( ) {
	}

	CHTService& CHTService::operator =( const CHTService& objCopy ) {
		return *this;
	}

} // namespace UdSDK
