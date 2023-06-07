
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHMUService::CHMUService( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_muservice";
		m_szUpdIndex	= "id";
		m_szName		= "muservice";
		m_fnGetObject	= GetProjectMUService;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новая единица измерения услуги";
		m_szAddModal	= L"Добавить единицу измерения услуги";
		m_szFormTitle	= L"Елиница измерения услуги";
		m_fnGetFormHandler	= GetHMUServiceForm;
	}

	CHMUService::CHMUService( const CHMUService& objCopy ) {
	}

	CHMUService::~CHMUService( ) {
	}

	CHMUService& CHMUService::operator =( const CHMUService& objCopy ) {
		return *this;
	}

} // namespace UdSDK
