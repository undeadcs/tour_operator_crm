
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectLog( ) {
		return new udProjectLog;
	} // udPBase GetProjectLog

	CProjectLog::CProjectLog( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// время создания
		this->get( "user"		)->ini( ( udUInt )	0	);	// ID пользователя
		this->get( "action"		)->ini( wstring( L"" )	);	// операция
	}

	CProjectLog::~CProjectLog( ) {
	}

	void CProjectLog::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_log" );
		objConfig.SetPrefix( "log_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		objConfig.SetAttrType( "action",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
		objConfig.SetAttrTitle( "created",	"Дата создания"	);
		objConfig.SetAttrTitle( "user",		"Пользователь"	);
		objConfig.SetAttrTitle( "action",	"Операция"		);
	} // void GetConfig

} // namespace UdSDK
