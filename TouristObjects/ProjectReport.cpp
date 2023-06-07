
#include "includes.h"

namespace UdSDK {

	udPBase GetProjectReportGroup( ) {
		return new udProjectReportGroup;
	} // udPBase GetProjectReportGroup

	CProjectReportGroup::CProjectReportGroup( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID сущности
		this->get( "name"		)->ini( wstring( L"" )	);	// название
		this->get( "date1"		)->ini( udDatetime( 0, true ) );	// дата отправления
		this->get( "date2"		)->ini( udDatetime( 0, true ) );	// дата возвращения
		this->get( "min"		)->ini( ( udUInt )	0	);	// минимальное количество мест
		this->get( "max"		)->ini( ( udUInt )	0	);	// максимальное количество мест
	}

	CProjectReportGroup::~CProjectReportGroup( ) {
	}

	void CProjectReportGroup::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_report_group" );
		objConfig.SetPrefix( "report_group_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		//objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
	} // void GetConfig

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportLanding( ) {
		return new udProjectReportLanding;
	} // udPBase GetProjectReportLanding

	CProjectReportLanding::CProjectReportLanding( ) {
		this->get( "id"				)->ini(	( udUInt )	0	);	// ID
		this->get( "group"			)->ini( ( udUInt )	0	);	// группа
		this->get( "tour"			)->ini( ( udUInt )	0	);	// тур
		this->get( "client"			)->ini( ( udUInt )	0	);	// клиент
		this->get( "direct"			)->ini( ( udUInt )	0	);	// направление
		this->get( "period"			)->ini( ( udUInt )	0	);	// период
		this->get( "date1"			)->ini( udDatetime( 0, true ) );	// date1 - периода (GroupPeriod)
		this->get( "fio"			)->ini( wstring( L"" )	);	// ФИО клиента
		this->get( "phone"			)->ini( wstring( L"" )	);	// телефон клиента
		this->get( "state"			)->ini( wstring( L"" )	);	// состояние тура
		this->get( "has_packet"		)->ini( wstring( L"" )	);	// наименование услуги с типом пакет
		this->get( "has_landing"	)->ini( wstring( L"" )	);	// наименование услуги с типом посадка
		this->get( "comment"		)->ini( wstring( L"" )	);	// комментарий из тура
	}

	CProjectReportLanding::~CProjectReportLanding( ) {
	}

	void CProjectReportLanding::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_report_landing" );
		objConfig.SetPrefix( "report_landing_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
	} // void GetConfig

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportFilling( ) {
		return new udProjectReportFilling;
	} // udPBase GetProjectReportFilling

	CProjectReportFilling::CProjectReportFilling( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);	// ID
		this->get( "group"	)->ini( ( udUInt )	0	);	// ID группы
		this->get( "period"	)->ini( ( udUInt )	0	);	// ID периода
		this->get( "date1"	)->ini( udDatetime( 0, true ) );	// date1 периода
		this->get( "min"	)->ini( ( udUInt )	0	);	// минимум мест
		this->get( "max"	)->ini( ( udUInt )	0	);	// максимум мест
		this->get( "direct"	)->ini( ( udUInt )	0	);	// направление
		this->get( "state"	)->ini( wstring( L"" )	);	// состояние
		this->get( "amount"	)->ini( ( udDWord )	0	);	// количество туристов в данном состоянии
	}

	CProjectReportFilling::~CProjectReportFilling( ) {
	}

	void CProjectReportFilling::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_report_filling" );
		objConfig.SetPrefix( "report_filling_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
	} // void GetConfig

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportGroupPay( ) {
		return new udProjectReportGroupPay;
	} // udPBase GetProjectReportGroupPay

	CProjectReportGroupPay::CProjectReportGroupPay( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);	// ID
		this->get( "fio"	)->ini( wstring( L"" )	);	// ФИО
		this->get( "price"	)->ini( ( udFloat ) 0	);	// Итоговая цена
		this->get( "pay"	)->ini( ( udFloat ) 0	);	// Оплата за тур
		this->get( "date"	)->ini( udDatetime( )	);	// Дата оплаты
		this->get( "place"	)->ini( wstring( L"" )	);	// Место оплаты
		this->get( "hotel"	)->ini( wstring( L"" )	);	// Гостиница
		this->get( "tour"	)->ini( ( udUInt ) 0		);	// ID тура
	} // CProjectReportGroupPay

	CProjectReportGroupPay::~CProjectReportGroupPay( ) {
	} // ~CProjectReportGroupPay

	void CProjectReportGroupPay::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_report_group_pay" );
		objConfig.SetPrefix( "report_group_pay_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
	} // void GetConfig

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportHotelPay( ) {
		return new udProjectReportHotelPay;
	} // udPBase GetProjectReportHotelPay

	CProjectReportHotelPay::CProjectReportHotelPay( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "date1"		)->ini( udDatetime( )	);	// Начальная дата
		this->get( "date2"		)->ini( udDatetime( )	);	// Конечная дата
		this->get( "leader"		)->ini( wstring( L"" )	);	// ФИО руководителя
		this->get( "num"		)->ini( ( udUInt )	0	);	// Количество человек
		this->get( "name"		)->ini( wstring( L"" )	);	// Наименование гостиницы
		this->get( "price"		)->ini( ( udFloat )	0	);	// Цена
		this->get( "price_cny"	)->ini( ( udFloat )	0	);	// Цена в Юанях
		// данные по которым составлялся отчет
		this->get( "period_date1"	)->ini( udDatetime( )	);	// Начальная дата
		this->get( "period_date2"	)->ini( udDatetime( )	);	// Конечная дата
		this->get( "direct"			)->ini( wstring( L"" )	);	// Наименование периода
	} // CProjectReportHotelPay

	CProjectReportHotelPay::~CProjectReportHotelPay( ) {
	} // ~CProjectReportHotelPay

	void CProjectReportHotelPay::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_report_hotel_pay" );
		objConfig.SetPrefix( "report_hotel_pay_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
	} // void GetConfig

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportDayPay( ) {
		return new udProjectReportDayPay;
	} // udPBase GetProjectReportDayPay

	CProjectReportDayPay::CProjectReportDayPay( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);	// ID
		this->get( "date"	)->ini( udDatetime( )	);	// Дата создания тура
		this->get( "fio"	)->ini( wstring( L"" )	);	// ФИО
		this->get( "price"	)->ini( ( udFloat ) 0	);	// Итоговая цена
		this->get( "pay"	)->ini( ( udFloat ) 0	);	// Оплата за тур
		this->get( "date"	)->ini( udDatetime( )	);	// Дата оплаты
		this->get( "place"	)->ini( wstring( L"" )	);	// Место оплаты
		this->get( "hotel"	)->ini( wstring( L"" )	);	// Гостиница
		this->get( "tour"	)->ini( ( udUInt ) 0		);	// ID тура
		this->get( "group"	)->ini( wstring( L"" )	);	// Группа
		this->get( "period"	)->ini( wstring( L"" )	);	// Период группы
		this->get( "manager"	)->ini( wstring( L"" )	);	// Ответственный
		this->get( "date1"	)->ini( udDatetime( )	);	// дата "с"
		this->get( "date2"	)->ini( udDatetime( )	);	// дата "по"
	} // CProjectReportDayPay

	CProjectReportDayPay::~CProjectReportDayPay( ) {
	} // ~CProjectReportDayPay

	void CProjectReportDayPay::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_report_day_pay" );
		objConfig.SetPrefix( "report_day_pay_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
	} // void GetConfig

} // namespace UdSDK
