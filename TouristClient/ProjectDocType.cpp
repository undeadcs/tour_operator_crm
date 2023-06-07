
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHDocType::CHDocType( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_doctype";
		m_szUpdIndex	= "id";
		m_szName		= "doctype";
		m_fnGetObject	= GetProjectDocType;
		m_szOrderBy		= "id";
		m_szFormTitle	= L"Тип документа";
		m_szAddModeless	= L"Новый тип документа";
		m_szAddModal	= L"Добавить тип документа";
		m_fnGetFormHandler	= GetHDocTypeForm;
	}

	CHDocType::CHDocType( const CHDocType& objCopy ) {
	}

	CHDocType::~CHDocType( ) {
	}

	CHDocType& CHDocType::operator =( const CHDocType& objCopy ) {
		return *this;
	}

} // namespace UdSDK
