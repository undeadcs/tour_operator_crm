
#include "includes.h"

namespace UdSDK {

	udPBase GetProjectObjectLock( ) {
		return new udProjectObjectLock;
	} // udPBase GetProjectObjectLock

	CProjectObjectLock::CProjectObjectLock( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);	// ID
		this->get( "type"	)->ini( ( udUInt )	0	);	// тип
		this->get( "obj"	)->ini( ( udUInt )	0	);	// ID объекта
		this->get( "date"	)->ini( udDatetime( 0, true ) );	// дата блокировки
	}

	CProjectObjectLock::~CProjectObjectLock( ) {
	}

	void CProjectObjectLock::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_lock" );
		objConfig.SetPrefix( "lock_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
	} // void GetConfig

} // namespace UdSDK
