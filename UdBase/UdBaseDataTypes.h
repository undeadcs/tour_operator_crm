/**
	Базовые типы данных библиотеки
*/
#ifndef UNDEADCS_SDK_BASE_DATA_TYPES_H
#define UNDEADCS_SDK_BASE_DATA_TYPES_H

#include <my_global.h>
#include <mysql.h>

namespace UdSDK {

	typedef class CError		udError, *udPError, **udPPError;
	typedef class CFile			udFile, *udPFile, **udPPFile;
	typedef class CFileUtf32	udFileUtf32, *udPFileUtf32, **udPPFileUtf32;
	typedef class CUdLog		udLog, *udPLog, **udPPLog;
	typedef class CUdBaseConfig	udBaseConfig, *udPBaseConfig, **udPPBaseConfig;
	typedef class CUdBase		udBase, *udPBase, **udPPBase;
	typedef class CUdForm		udForm, *udPForm, **udPPForm;
	typedef class CUdDatabase	udDatabase, *udPDatabase, **udPPDatabase;
	typedef class CUdHandlerOption	udHandlerOption, *udPHandlerOption, **udPPHandlerOption;
	typedef class CUdHandler	udHandler, *udPHandler, **udPPHandler;
	typedef class CUdPager		udPager, *udPPager, **udPPPager;
	typedef class CMySql		udMySql, *udPMySql, **udPPMySql;
	typedef class CServerAccount	udServerAccount, *udPServerAccount, **udPPServerAccount;
	typedef class CDatabaseAccount	udDatabaseAccount, *udPDatabaseAccount, **udPPDatabaseAccount;

	extern void UdBaseEscapeString( char* out, const char* in, size_t len );

	typedef udPBase	( *pfnGetObject )( );

	typedef basic_string< udDWord >	udStlStr;

	typedef FILE				udStdFile, *udPStdFile, **udPPStdFile;
	typedef string				udString, *udPString, **udPPString;
	typedef wstring				udWString, *udPWString, **udPPWString;
	typedef FILE				udStdFile, *udPStdFile, **udPPStdFile;
	typedef clock_t				udClock, *udPClock, **udPPClock;
	typedef time_t				udTime, *udPTime, **udPPTime;

	typedef MYSQL								udStdMySQL, *udPStdMySQL, **udPPStdMySQL;
	typedef MYSQL_RES							udMySQLRes, *udPMySQLRes, **udPPMySQLRes;
	typedef MYSQL_ROW							udMySQLRow, *udPMySQLRow, **udPPMySQLRow;
	typedef MYSQL_FIELD							udMySQLField, *udPMySQLField, **udPPMySQLField;
	typedef MYSQL_FIELD_OFFSET					udMySQLFieldOffset, *udPMySQLFieldOffest, **udPPMySQLFieldOffset;

	typedef pair< string,	string			>	udPairStrStr;
	typedef pair< string,	wstring			>	udPairStrWstr;
	typedef pair< wstring,	wstring			>	udPairWstrWstr;
	typedef pair< wstring,	string			>	udPairWStrStr;
	typedef vector< udPMorph				>	udArrMorph, *udPArrMorph;
	typedef vector< udArrMorph				>	udArrArrMorph, *udPArrArrMorph;
	typedef vector< udPairStrStr			>	udArrPairStr, *udPArrPairStr;
	typedef vector< udPairWstrWstr			>	udArrPairWstr, *udPArrPairWstr;
	typedef vector< udError					>	udArrError, *udPArrError;
	typedef vector< udAscType				>	udArrAsc, *udPArrAsc;
	typedef map< string, udInt				>	udAscInt, *udPAscInt;
	typedef vector< udPBase					>	udArrBase, *udPArrBase;
	typedef map< string, string				>	udAscStr, *udPAscStr;
	typedef map< udInt, udArrMorph			>	udAscArrMorph, *udPAscArrMorph;
	typedef map< udDWord, udPBase			>	udDwAscBase, *udPDwAscBase;
	typedef map< string, udPBase			>	udAscBase, *udPAscBase;

} // namespace UdSDK

#endif
