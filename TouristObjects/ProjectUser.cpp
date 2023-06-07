
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectUser( ) {
		return new udProjectUser;
	} // udPBase GetProjectCity

	CProjectUser::CProjectUser( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "login"		)->ini( wstring( L"" )	);	// логин
		this->get( "password"	)->ini( wstring( L"" )	);	// пароль
		this->get( "email"		)->ini( wstring( L"" )	);	// e-mail
		this->get( "fio"		)->ini( wstring( L"" )	);	// ФИО
		this->get( "rank"		)->ini( ( udInt )	0	);	// ранг
		this->get( "series"		)->ini( wstring( L"" )	);	// серия для распечатки шаблонов документов
	}

	CProjectUser::~CProjectUser( ) {
	}

	void CProjectUser::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_user" );
		objConfig.SetPrefix( "user_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"		);
		objConfig.SetAttrTitle( "login",	"Логин"		);
		objConfig.SetAttrTitle( "password",	"Пароль"	);
		objConfig.SetAttrTitle( "email",	"E-mail"	);
		objConfig.SetAttrTitle( "fio",		"ФИО"		);
		objConfig.SetAttrTitle( "rank",		"Ранг"		);
	} // void GetConfig

} // namespace UdSDK
