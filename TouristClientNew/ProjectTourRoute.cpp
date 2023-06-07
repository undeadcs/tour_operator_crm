
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourRouteList( ) {
		udPBase ret = GetProjectTourRoute( );
		ret->get( "city_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourRouteList

} // namespace UdSDK
