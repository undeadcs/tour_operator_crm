
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectGroupContractApp1( ) {
		return new udProjectGroupContractApp1;
	} // udPBase GetProjectGroupContractApp1

	CProjectGroupContractApp1::CProjectGroupContractApp1( ) : udBase( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "tour"		)->ini( ( udUInt )	0	);	// ID тура
		this->get( "price"		)->ini( wstring( L"" )	);	// цена за тур
		this->get( "pay1"		)->ini( wstring( L"" )	);	// оплата 1
		this->get( "pay1date"	)->ini( udDatetime( 0, true ) );	// дата оплаты 1
		this->get( "pay2"		)->ini( wstring( L"" )	);	// оплата 2
		this->get( "pay2date"	)->ini( udDatetime( 0, true ) );	// дата оплаты 2
		this->get( "days1"		)->ini( wstring( L"" )	);	// За сколько дней до поездки предоставить документы (не позднее)
		this->get( "days2"		)->ini( wstring( L"" )	);	// В течение скольки дней после бронирования подтвердить тур
		this->get( "days3"		)->ini( wstring( L"" )	);	// В течение скольки дней после бронирования оплатить тур (не позднее)
		this->get( "product"	)->ini( wstring( L"" )	);	// Туристский продукт
	}

	CProjectGroupContractApp1::~CProjectGroupContractApp1( ) {
	}

	void CProjectGroupContractApp1::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gcapp1" );
		objConfig.SetPrefix( "gcapp1_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig



	udPBase GetProjectGCTouristInfo( ) {
		return new udProjectGCTouristInfo;
	} // udPBase GetProjectGCTouristInfo

	CProjectGCTouristInfo::CProjectGCTouristInfo( ) : udBase( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "app"		)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "fio"		)->ini( wstring( L"" )	);	// ФИО
		this->get( "sex"		)->ini( ( udInt ) udProjectClient::SEX_MAN );	// пол
		this->get( "born"		)->ini( wstring( L"" )	);	// Дата рождения
		this->get( "passport"	)->ini( wstring( L"" )	);	// Паспортные данные
	}

	CProjectGCTouristInfo::~CProjectGCTouristInfo( ) {
	}

	void CProjectGCTouristInfo::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gctouristinfo" );
		objConfig.SetPrefix( "gctouristinfo_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig



	udPBase GetProjectGCOccupancy( ) {
		return new udProjectGCOccupancy;
	} // udPBase GetProjectGCOccupancy

	CProjectGCOccupancy::CProjectGCOccupancy( ) : udBase( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "app"		)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "name"		)->ini( wstring( L"" )	);	// Наименование
		this->get( "category"	)->ini( wstring( L"" )	);	// Категория
		this->get( "amount"		)->ini( wstring( L"" )	);	// Количество проживающих
		this->get( "feed"		)->ini( wstring( L"" )	);	// Питание
	}

	CProjectGCOccupancy::~CProjectGCOccupancy( ) {
	}

	void CProjectGCOccupancy::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gcoccupancy" );
		objConfig.SetPrefix( "gcoccupancy_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig




	udPBase GetProjectGCProgram( ) {
		return new udProjectGCProgram;
	} // udPBase GetProjectGCProgram

	CProjectGCProgram::CProjectGCProgram( ) : udBase( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "app"		)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "text"		)->ini( wstring( L"" )	);	// Текст
	}

	CProjectGCProgram::~CProjectGCProgram( ) {
	}

	void CProjectGCProgram::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gcprogram" );
		objConfig.SetPrefix( "gcprogram_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		objConfig.SetAttrType( "text",		udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig



	udPBase GetProjectGCTransfer( ) {
		return new udProjectGCTransfer;
	} // udPBase GetProjectGCTransfer

	CProjectGCTransfer::CProjectGCTransfer( ) : udBase( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "app"	)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "fio"	)->ini( wstring( L"" )	);	// ФИО туриста
		this->get( "trans"	)->ini( wstring( L"" )	);	// Перевозчик
		this->get( "level"	)->ini( wstring( L"" )	);	// Уровень сервиса
		this->get( "date1"	)->ini( udDatetime( 0, true ) );	// Дата убытия
		this->get( "date2"	)->ini( udDatetime( 0, true ) );	// Дата прибытия
	}

	CProjectGCTransfer::~CProjectGCTransfer( ) {
	}

	void CProjectGCTransfer::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gctransfer" );
		objConfig.SetPrefix( "gctransfer_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig



	udPBase GetProjectGCCover( ) {
		return new udProjectGCCover;
	} // udPBase GetProjectGCCover

	CProjectGCCover::CProjectGCCover( ) : udBase( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "app"	)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "type"	)->ini( wstring( L"" )	);	// Тип страховки
		this->get( "name"	)->ini( wstring( L"" )	);	// Страховщик
	}

	CProjectGCCover::~CProjectGCCover( ) {
	}

	void CProjectGCCover::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gccover" );
		objConfig.SetPrefix( "gccover_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig



	udPBase GetProjectGCVisa( ) {
		return new udProjectGCVisa;
	} // udPBase GetProjectGCVisa

	CProjectGCVisa::CProjectGCVisa( ) : udBase( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "app"	)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "type"	)->ini( wstring( L"" )	);	// Тип услуги
	}

	CProjectGCVisa::~CProjectGCVisa( ) {
	}

	void CProjectGCVisa::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gcvisa" );
		objConfig.SetPrefix( "gcvisa_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig



	udPBase GetProjectGCService( ) {
		return new udProjectGCService;
	} // udPBase GetProjectGCService

	CProjectGCService::CProjectGCService( ) : udBase( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "app"	)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "name"	)->ini( wstring( L"" )	);	// Наименование услуги
		this->get( "char"	)->ini( wstring( L"" )	);	// Характеристики
	}

	CProjectGCService::~CProjectGCService( ) {
	}

	void CProjectGCService::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gcservice" );
		objConfig.SetPrefix( "gcservice_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig



	udPBase GetProjectGCTransfer2( ) {
		return new udProjectGCTransfer2;
	} // udPBase GetProjectGCTransfer2

	CProjectGCTransfer2::CProjectGCTransfer2( ) : udBase( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "app"	)->ini( ( udUInt )	0	);	// ID приложения 1 к туру
		this->get( "route"	)->ini( wstring( L"" )	);	// Маршрут
		this->get( "type"	)->ini( wstring( L"" )	);	// Тип трансфера
	}

	CProjectGCTransfer2::~CProjectGCTransfer2( ) {
	}

	void CProjectGCTransfer2::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_gctransfer2" );
		objConfig.SetPrefix( "gctransfer2_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig

} // namespace UdSDK
