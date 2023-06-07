
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CStateHandler::CStateHandler( ) {
	} // CStateHandler

	CStateHandler::CStateHandler( const CStateHandler& objCopy ) {
	} // CStateHandler

	CStateHandler::~CStateHandler( ) {
		g_pApp->GetLog( )->Write( "CStateHandler::~CStateHandler\n" );
	} // ~CStateHandler

	CStateHandler& CStateHandler::operator =( const CStateHandler& objCopy ) {
		return *this;
	} // CStateHandler& operator =

} // namespace UdSDK
