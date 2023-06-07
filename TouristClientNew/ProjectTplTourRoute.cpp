
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTplTourRouteList( ) {
		udPBase ret = GetProjectTplTourRoute( );
		ret->get( "city_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTplTourRouteList

} // namespace UdSDK
