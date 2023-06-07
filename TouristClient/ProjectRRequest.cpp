
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHRRequest::CHRRequest( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_rrequest";
		m_szUpdIndex	= "id";
		m_szName		= "rrequest";
		m_fnGetObject	= GetProjectRRequest;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новый результат обращения";
		m_szAddModal	= L"Добавить результат обращения";
		m_szFormTitle	= L"Результат обращения";
		m_fnGetFormHandler	= GetHRRequestForm;
	}

	CHRRequest::CHRRequest( const CHRRequest& objCopy ) {
	}

	CHRRequest::~CHRRequest( ) {
	}

	CHRRequest& CHRRequest::operator =( const CHRRequest& objCopy ) {
		return *this;
	}

} // namespace UdSDK
