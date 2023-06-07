
#include "includes.h"

namespace UdSDK {

	udPBase GetProjectTourPayOrder( ) {
		return new udProjectTourPayOrder;
	} // udPBase GetProjectTourPayOrder

	CProjectTourPayOrder::CProjectTourPayOrder( ) {
		this->get( "id"			)->ini( ( udUInt )	0			);	// ID
		this->get( "created"	)->ini( udDatetime( 0, true )	);	// Дата+время создания
		this->get( "state"		)->ini( ( udInt )	stOpened	);	// состояние
		this->get( "tour"		)->ini( ( udUInt )	0			);	// ID тура
	}

	CProjectTourPayOrder::~CProjectTourPayOrder( ) {
	}

	void CProjectTourPayOrder::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tourpayorder" );
		config.SetPrefix( "tourpayorder_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",	"ID"			);
	} // void GetConfig

} // namespace UdSDK
