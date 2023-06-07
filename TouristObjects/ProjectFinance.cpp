
#include "includes.h"

namespace UdSDK {

	udPBase GetProjectFinanceSumm( ) {
		return new udProjectFinanceSumm;
	} // udPBase GetProjectFinanceSumm

	CProjectFinanceSumm::CProjectFinanceSumm( ) {
		this->get( "id"			)->ini( ( udUInt )	0			);	// ID
		this->get( "summ"		)->ini( ( udFloat )	0			);	// сумма
		this->get( "currency"	)->ini( ( udUInt )	0			);	// валюта
		this->get( "type"		)->ini( ( udInt ) SummWhite		);	// бел/чер
		this->get( "at"			)->ini( ( udInt ) SummAtNone	);	// к чему привязана сумма
		this->get( "obj"		)->ini( ( udUInt )	0			);	// ID объекта, к которому привязана
	}

	CProjectFinanceSumm::~CProjectFinanceSumm( ) {
	}

	void CProjectFinanceSumm::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_fsumm" );
		objConfig.SetPrefix( "fsumm_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
	} // void GetConfig

	wstring CProjectFinanceSumm::GetTypeName( udInt type ) {
		switch( type ) {
			case SummWhite:
				return L"Белые";
				break;

			case SummBlack:
				return L"Черные";
				break;

			default:
				break;
		}
		return L"Неизвестный";
	} // wstring GetTypeName


	udPBase GetProjectFinanceOperation( ) {
		return new udProjectFinanceOperation;
	} // udPBase GetProjectFinanceOperation

	CProjectFinanceOperation::CProjectFinanceOperation( ) {
		this->get( "id"			)->ini( ( udUInt )	0		);	// ID
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// дата операции
		this->get( "acc"		)->ini( ( udUInt )	0		);	// ID лицевого счета
		this->get( "type"		)->ini( ( udInt )	fotNone	);	// тип
		this->get( "tour"		)->ini( ( udUInt )	0		);	// ID тура (опционально)
		this->get( "money"		)->ini( ( udInt )	fctNone	);	// тип поступления денег MoneyType
		this->get( "comment"	)->ini( wstring( L"" )		);	// комментарий
	}

	CProjectFinanceOperation::~CProjectFinanceOperation( ) {
	}

	void CProjectFinanceOperation::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_fop" );
		objConfig.SetPrefix( "fop_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
	} // void GetConfig

	wstring CProjectFinanceOperation::GetMoneyTypeName( udInt type ) {
		switch( type ) {
			case fctCash:
				return L"Наличные";
				break;

			case fctCard:
				return L"Карточка";
				break;

			case fctNonCash:
				return L"Безналичные";
				break;

			default:
				break;
		}
		return L"Неизвестный";
	} // wstring GetMoneyTypeName

	wstring CProjectFinanceOperation::GetOperationTypeName( udInt type ) {
		switch( type ) {
			case fotRoll:
				return L"зачисление";
				break;

			case fotCut:
				return L"снятие";
				break;

			case fotConv:
				return L"конвертация";
				break;

			case fotPayTour:
				return L"оплата тура";
				break;

			case fotPayServ:
				return L"оплата услуги";
				break;

			case fotBackTour:
				return L"возврат с тура";
				break;

			case fotBackServ:
				return L"возврат с услуг";
				break;

			default:
				break;
		}
		return L"неизв.";
	} // wstring GetOperationTypeName




	udPBase GetProjectFinanceSummAdd( ) {
		return new udProjectFinanceSummAdd;
	} // udPBase GetProjectFinanceSummAdd

	CProjectFinanceSummAdd::CProjectFinanceSummAdd( ) {
		this->get( "id"			)->ini( ( udUInt )	0				);
		this->get( "summ"		)->ini( ( udFloat )	0.0f			);
		this->get( "currency"	)->ini( ( udUInt )	0				);
		this->get( "type"		)->ini( ( udInt )	CProjectFinanceSumm::SummWhite		);
		this->get( "how"		)->ini( ( udInt )	udProjectFinanceOperation::fctNone	);
		this->get( "comment"	)->ini( wstring( L"" )				);
	}

	CProjectFinanceSummAdd::~CProjectFinanceSummAdd( ) {
	}

	void CProjectFinanceSummAdd::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_fsummadd" );
		config.SetPrefix( "fsummadd_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
	} // void GetConfig



	udPBase GetProjectFinanceSummBack( ) {
		return new udProjectFinanceSummBack;
	} // udPBase GetProjectFinanceSummBack

	CProjectFinanceSummBack::CProjectFinanceSummBack( ) {
		this->get( "id"			)->ini( ( udUInt )	0				);
		this->get( "summ"		)->ini( ( udFloat )	0.0f			);
		this->get( "currency"	)->ini( ( udUInt )	0				);
		this->get( "type"		)->ini( ( udInt )	CProjectFinanceSumm::SummWhite		);
		this->get( "how"		)->ini( ( udInt )	udProjectFinanceOperation::fctNone	);
		this->get( "comment"	)->ini( wstring( L"" )				);
	}

	CProjectFinanceSummBack::~CProjectFinanceSummBack( ) {
	}

	void CProjectFinanceSummBack::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_fsummback" );
		config.SetPrefix( "fsummback_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
	} // void GetConfig



	udPBase GetProjectFinanceSummConvert( ) {
		return new udProjectFinanceSummConvert;
	} // udPBase GetProjectFinanceSummBack

	CProjectFinanceSummConvert::CProjectFinanceSummConvert( ) {
		this->get( "id"			)->ini( ( udUInt )	0			);
		this->get( "summ"		)->ini( ( udFloat )	0.0f		);
		this->get( "currency1"	)->ini( ( udUInt )	0			);
		this->get( "currency2"	)->ini( ( udUInt )	0			);
		this->get( "type1"		)->ini( ( udInt ) udProjectFinanceSumm::SummWhite	);
		this->get( "type2"		)->ini( ( udInt ) udProjectFinanceSumm::SummTypeNone	);
	}

	CProjectFinanceSummConvert::~CProjectFinanceSummConvert( ) {
	}

	void CProjectFinanceSummConvert::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_fsummconvert" );
		config.SetPrefix( "fsummconvert_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
	} // void GetConfig



	udPBase GetProjectFinanceSummToPartner( ) {
		return new udProjectFinanceSummToPartner;
	} // udPBase GetProjectFinanceSummToPartner

	CProjectFinanceSummToPartner::CProjectFinanceSummToPartner( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);
		this->get( "summ_id"	)->ini( ( udUInt )	0	);
		this->get( "partner_id"	)->ini( ( udUInt )	0	);
	}

	CProjectFinanceSummToPartner::~CProjectFinanceSummToPartner( ) {
	}

	void CProjectFinanceSummToPartner::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_fsummtopartner" );
		config.SetPrefix( "fsummtopartner_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
	} // void GetConfig
	

} // namespace UdSDK
