п»ї
#include "includes.h"

namespace UdSDK {

	udPApplication g_pApp = NULL;
	udPLog g_pLog = NULL;

} // namespace UdSDK

using namespace UdSDK;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow ) {
	OpenUdLibLog( "TouristServer_udlib_log.txt", "wb" );
	OpenUdBaseLog( "TouristServer_udbase_log.txt", "wb" );
	try {
		Gtk::Main kit( __argc, __argv );
		g_pApp = new udApplication( hInstance, new CAppInitProject );
		udInt iRet = g_pApp->Start( );

		//CustomTest( );

		if ( g_pApp->GetConfig( )->isset( "drop_database_after_finish" ) ) {
			g_pApp->GetDatabase( )->Query( "DROP DATABASE " + g_pApp->GetConfig( )->get( "database_dbname" )->toString( ) );
		}

		udDELETE( g_pApp );
		CloseUdBaseLog( );
		CloseUdLibLog( );
		return iRet;
	}
	catch ( ... ) {
		MessageBox( NULL, L"Exception generated", L"РџРѕРјР°РЅРѕ РёСЃРєР»СЋС‡РµРЅРёРµ", MB_OK );
		CloseUdBaseLog( );
		CloseUdLibLog( );
		return 1;
	}
	CloseUdBaseLog( );
	CloseUdLibLog( );
	return 0;
} // int WinMain

