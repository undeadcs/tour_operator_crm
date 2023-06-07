
#include "includes.h"

namespace UdSDK {

	CAppConfig::CAppConfig( ) : udMorph( ) {
	} // CAppConfig

	CAppConfig::CAppConfig( const CAppConfig& objCopy ) : udMorph( objCopy ) {
	} // CAppConfig

	CAppConfig::~CAppConfig( ) {
	} // CAppConfig

	CAppConfig& CAppConfig::operator =( const CAppConfig& objCopy ) {
		return *this;
	} // CAppConfig& operator =

} // namespace UdSDK
