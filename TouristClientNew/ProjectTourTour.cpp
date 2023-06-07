
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourTourList( ) {
		udPBase ret = GetProjectTourTour( );
		ret->get( "tour2_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourTourList

} // namespace UdSDK
