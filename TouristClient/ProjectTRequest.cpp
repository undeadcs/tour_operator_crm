
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHTRequest::CHTRequest( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_trequest";
		m_szUpdIndex	= "id";
		m_szName		= "trequest";
		m_fnGetObject	= GetProjectTRequest;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новый тип обращения";
		m_szAddModal	= L"Добавить тип обращения";
		m_szFormTitle	= L"Тип обращения";
		m_fnGetFormHandler	= GetHTRequestForm;
	}

	CHTRequest::CHTRequest( const CHTRequest& objCopy ) {
	}

	CHTRequest::~CHTRequest( ) {
	}

	CHTRequest& CHTRequest::operator =( const CHTRequest& objCopy ) {
		return *this;
	}

} // namespace UdSDK
