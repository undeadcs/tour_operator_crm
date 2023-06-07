
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectDocTemplate( ) {
		return new udProjectDocTemplate;
	} // udPBase GetProjectDocTemplate

	CProjectDocTemplate::CProjectDocTemplate( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
		this->get( "type"	)->ini( ( udInt ) tplClassic );
		this->get( "text"	)->ini( wstring( L"" )	);
	}

	CProjectDocTemplate::~CProjectDocTemplate( ) {
	}

	void CProjectDocTemplate::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_doctpl" );
		objConfig.SetPrefix( "doctpl_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
	} // void GetConfig

} // namespace UdSDK
