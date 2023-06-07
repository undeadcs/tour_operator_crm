п»ї
#include "includes.h"

namespace UdSDK {

	udPApplication g_pApp = NULL;
	udPLog g_pLog = NULL;

} // namespace UdSDK

using namespace UdSDK;

extern int xmlLoadExtDtdDefaultValue;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow ) {
	OpenUdLibLog( "TouristClient_udlib_log.txt", "wb" );
	OpenUdBaseLog( "TouristClient_udbase_log.txt", "wb" );
	OpenUdUiLog( "TouristClient_udui_log.txt", "wb" );
	try {
		LIBXML_TEST_VERSION;
		xmlInitParser( );
		xmlSubstituteEntitiesDefault( 1 );
		xmlLoadExtDtdDefaultValue = 1;
		g_pApp = new udApplication( hInstance, new CAppInitProject );
		udInt iRet = g_pApp->Start( );

		//g_pApp->GetDatabase( )->Query( "DROP DATABASE " + g_pApp->GetConfig( )->get( "database_dbname" )->toString( ) );

		udDELETE( g_pApp );
		xsltCleanupGlobals( );
		xmlCleanupParser( );
		xmlMemoryDump( );
		CloseUdUiLog( );
		CloseUdBaseLog( );
		CloseUdLibLog( );
		return iRet;
	}
	catch ( ... ) {
		MessageBox( NULL, L"Exception generated", L"РџРѕРјР°РЅРѕ РёСЃРєР»СЋС‡РµРЅРёРµ", MB_OK );
		CloseUdUiLog( );
		CloseUdBaseLog( );
		CloseUdLibLog( );
		return 1;
	}
	CloseUdUiLog( );
	CloseUdBaseLog( );
	CloseUdLibLog( );
	return 0;
} // int WinMain

