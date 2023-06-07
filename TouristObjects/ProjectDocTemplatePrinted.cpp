
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectDocTemplatePrinted( ) {
		return new udProjectDocTemplatePrinted;
	} // udPBase GetProjectDocTemplatePrinted

	CProjectDocTemplatePrinted::CProjectDocTemplatePrinted( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// дата+время создания
		this->get( "createdby"	)->ini( ( udUInt )	0	);	// ID пользователя, создавшего распечатку
		this->get( "tour"		)->ini( ( udUInt )	0	);	// ID тура
		this->get( "series"		)->ini( wstring( L"" )	);	// серия
		this->get( "number"		)->ini( wstring( L"" )	);	// сгенерированный номер
		this->get( "name"		)->ini( wstring( L"" )	);	// имя шаблона
		this->get( "type"		)->ini( ( udInt ) udProjectDocTemplate::tplClassic );	// тип шаблона
		this->get( "text"		)->ini( wstring( L"" )	);	// текст шаблона
	}

	CProjectDocTemplatePrinted::~CProjectDocTemplatePrinted( ) {
	}

	void CProjectDocTemplatePrinted::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_doctplprinted" );
		objConfig.SetPrefix( "doctplprinted_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY		);
		objConfig.SetAttrType( "created",	udBaseConfig::UDSDK_TYPE_DATETIME	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
	} // void GetConfig

} // namespace UdSDK
