
#include "includes.h"

namespace UdSDK {

	udPBase GetProjectObjectLink( ) {
		return new udProjectObjectLink;
	} // udPBase GetProjectObjectLink

	CProjectObjectLink::CProjectObjectLink( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "id_src"		)->ini( ( udUInt )	0	);	// ID сущности источника
		this->get( "id_dst"		)->ini( ( udUInt )	0	);	// ID сущности результата
		this->get( "type_src"	)->ini( ( udInt )	0	);	// тип сущности источника
		this->get( "type_dst"	)->ini( ( udInt )	0	);	// тип сущности результата
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// дата+время совершения операции
	}

	CProjectObjectLink::~CProjectObjectLink( ) {
	}

	void CProjectObjectLink::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_objlnk" );
		objConfig.SetPrefix( "objlnk_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
	} // void GetConfig

} // namespace UdSDK
