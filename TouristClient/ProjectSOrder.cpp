
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHSOrder::CHSOrder( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_sorder";
		m_szUpdIndex	= "id";
		m_szName		= "sorder";
		m_fnGetObject	= GetProjectSOrder;
		m_szOrderBy		= "id";
		m_szAddModeless	= L"Новое состояние заявки";
		m_szAddModal	= L"Добавить состояние заявки";
		m_szFormTitle	= L"Состояние заявки";
		m_fnGetFormHandler	= GetHSOrderForm;
	}

	CHSOrder::CHSOrder( const CHSOrder& objCopy ) {
	}

	CHSOrder::~CHSOrder( ) {
	}

	CHSOrder& CHSOrder::operator =( const CHSOrder& objCopy ) {
		return *this;
	}

} // namespace UdSDK
