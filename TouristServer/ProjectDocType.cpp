
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHDocType::CHDocType( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_doctype";
		m_szUpdIndex	= "id";
		m_szName		= "doctype";
		m_fnGetObject	= GetProjectDocType;
	}

	CHDocType::CHDocType( const CHDocType& objCopy ) {
	}

	CHDocType::~CHDocType( ) {
	}

	CHDocType& CHDocType::operator =( const CHDocType& objCopy ) {
		return *this;
	}

} // namespace UdSDK
