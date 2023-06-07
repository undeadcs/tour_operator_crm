
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTouristList( ) {
		return new udProjectTouristList;
	} // udPBase GetProjectTouristList

	CProjectTouristList::CProjectTouristList( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "date1"	)->ini( udDatetime( 0, true ) );	// Дата выезда
		this->get( "date2"	)->ini( udDatetime( 0, true ) );	// Дата въезда
		this->get( "exit"	)->ini( ( udUInt )	0	);	// (выезд) ID пункт перехода
		this->get( "enter"	)->ini( ( udUInt )	0	);	// (въезд) ID пункт перехода
		this->get( "host"	)->ini( ( udUInt )	0	);	// ID принимающая сторона
		this->get( "leader"	)->ini( ( udUInt )	0	);	// ID клиента руководителя группы
		this->get( "route"	)->ini( ( udUInt )	0	);	// ID маршрут
	}

	CProjectTouristList::~CProjectTouristList( ) {
	}

	void CProjectTouristList::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_touristlist" );
		objConfig.SetPrefix( "touristlist_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
	} // void GetConfig



	udPBase GetProjectTouristListLink( ) {
		return new udProjectTouristListLink;
	} // udPBase GetProjectTouristListLink

	CProjectTouristListLink::CProjectTouristListLink( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);	// ID
		this->get( "list"	)->ini( ( udUInt )	0	);	// ID списка
		this->get( "client"	)->ini( ( udUInt )	0	);	// ID клиента
	}

	CProjectTouristListLink::~CProjectTouristListLink( ) {
	}

	void CProjectTouristListLink::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_touristlistlink" );
		objConfig.SetPrefix( "touristlistlink_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
	} // void GetConfig

} // namespace UdSDK
