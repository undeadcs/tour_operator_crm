
#include "includes.h"

namespace UdSDK {

	udPApplication	g_pApp = NULL;

} // namespace UdSDK

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow ) {
	UdSDK::OpenUdLibLog( "TouristClient_udlib_log.txt", "wb" );
	UdSDK::OpenUdBaseLog( "TouristClient_udbase_log.txt", "wb" );

	Gtk::Main kit( __argc, __argv );

	UdSDK::udPApplication pApp = new UdSDK::udApplication( new UdSDK::CAppInitProject );
	UdSDK::g_pApp = pApp;

	int ret = pApp->Start( );

	UdSDK::CloseUdBaseLog( );
	UdSDK::CloseUdLibLog( );

	return ret;
} // int WinMain