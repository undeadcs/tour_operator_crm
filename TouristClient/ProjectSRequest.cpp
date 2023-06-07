
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHSRequest::CHSRequest( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_srequest";
		m_szUpdIndex	= "id";
		m_szName		= "srequest";
		m_fnGetObject	= GetProjectSRequest;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новое состояние обращения";
		m_szAddModal	= L"Добавить состояние обращения";
		m_szFormTitle	= L"Состояние обращения";
		m_fnGetFormHandler	= GetHSRequestForm;
	}

	CHSRequest::CHSRequest( const CHSRequest& objCopy ) {
	}

	CHSRequest::~CHSRequest( ) {
	}

	CHSRequest& CHSRequest::operator =( const CHSRequest& objCopy ) {
		return *this;
	}

} // namespace UdSDK
