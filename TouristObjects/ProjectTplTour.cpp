
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTplTour( ) {
		return new udProjectTplTour;
	} // udPBase GetProjectTplTour

	CProjectTplTour::CProjectTplTour( ) {
		this->get( "id"			)->ini( ( udUInt )	0		);	// ID сущности
		this->get( "name"		)->ini( wstring( L"" )		);	// название
		this->get( "cost"		)->ini( ( udFloat )	0.0f	);	// себестоимость (deprecated)
		this->get( "price"		)->ini( ( udFloat )	0.0f	);	// итоговая цена (deprecated)
		this->get( "direct"		)->ini( ( udUInt )	0		);	// ID направления
		this->get( "daynum"		)->ini( ( udDWord )	0		);	// продолжительность
	}

	CProjectTplTour::~CProjectTplTour( ) {
	}

	void CProjectTplTour::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_tpltour" );
		objConfig.SetPrefix( "tpltour_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"				);
		objConfig.SetAttrTitle( "name",		"Название"			);
		objConfig.SetAttrTitle( "cost",		"Себестоимость"		);
		objConfig.SetAttrTitle( "price",	"Итоговая цена"		);
		objConfig.SetAttrTitle( "direct",	"Направление"		);
		objConfig.SetAttrTitle( "daynum",	"Продолжительность"	);
	} // void GetConfig




	udPBase GetProjectTplTourService( ) {
		return new udProjectTplTourService;
	}

	CProjectTplTourService::CProjectTplTourService( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "tpltour"	)->ini( ( udUInt )	0	);	// ID шаблона
		this->get( "service"	)->ini( ( udUInt )	0	);	// ID услуги
		this->get( "num"		)->ini( ( udUInt )	0	);	// количество
		this->get( "date1"		)->ini( udDatetime( 0, true ) );	// дата с
		this->get( "date2"		)->ini( udDatetime( 0, true ) );	// дата по
	}

	CProjectTplTourService::~CProjectTplTourService( ) {
	}

	void CProjectTplTourService::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tpltourservice" );
		config.SetPrefix( "tpltourservice_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
	} // void GetConfig

} // namespace UdSDK
