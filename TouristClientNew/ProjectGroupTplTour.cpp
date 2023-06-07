
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectGroupTplTourList( ) {
		udPBase ret = GetProjectGroupTplTour( );
		ret->get( "tpltour_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGroupTplTourList

} // namespace UdSDK
