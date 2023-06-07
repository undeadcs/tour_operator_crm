
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPersAcc( ) {
		return new udProjectPersAcc;
	} // udPBase GetProjectPersAcc

	CProjectPersAcc::CProjectPersAcc( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "type"		)->ini( ( udInt )	0	);	// тип
		this->get( "obj1"		)->ini( ( udUInt )	0	);	// ID объекта 1
		this->get( "obj2"		)->ini( ( udUInt )	0	);	// ID объекта 2
	}

	CProjectPersAcc::~CProjectPersAcc( ) {
	}

	void CProjectPersAcc::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_persacc" );
		objConfig.SetPrefix( "persacc_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"		);
		objConfig.SetAttrTitle( "type",	"Тип"		);
		objConfig.SetAttrTitle( "obj",	"Объект"	);
	} // void GetConfig

} // namespace UdSDK
