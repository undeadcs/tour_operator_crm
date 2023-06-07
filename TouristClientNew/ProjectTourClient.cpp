
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourClientList( ) {
		udPBase ret = GetProjectTourClient( );
		ret->get( "fio" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourClientList

} // namespace UdSDK
