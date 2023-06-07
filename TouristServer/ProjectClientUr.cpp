
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHClientUr::CHClientUr( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_clientur";
		m_szUpdIndex	= "id";
		m_szName		= "clientur";
		m_fnGetObject	= GetProjectClientUr;

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "cfio" )->ini( wstring( L"" ) );
	}

	CHClientUr::CHClientUr( const CHClientUr& objCopy ) {
	}

	CHClientUr::~CHClientUr( ) {
	}

	CHClientUr& CHClientUr::operator =( const CHClientUr& objCopy ) {
		return *this;
	}

	udInt CHClientUr::AddObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::AddObj( obj, save_info ) == 1 ) {
			// дополнительно создаем лицевой счет
			this->CreateAccount( g_pApp->GetHandler( ), obj->get( "id" )->toUInt( ) );
			return 1;
		}
		return 0;
	} // udInt AddObj

	udInt CHClientUr::DelObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::DelObj( obj, save_info ) == 1 ) {
			// удаляем связанные объекты
			this->DeleteLinkedObjects( obj );
			return 1;
		}
		return 0;
	} // udInt DelObj

	void CHClientUr::DeleteLinkedObjects( udPBase obj ) {
		// лицевой счет
		udPDatabase db = g_pApp->GetDatabase( );
		string query = "", szWhere = "";
		udBaseConfig config;
		udPBase obj1;
		obj1 = new udProjectPersAcc;
		obj1->GetConfig( config );
		delete obj1;
		udPType type = new udType( ( udInt ) ( udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) + " AND " +
			config.GetAttrIndex( "obj1" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
	} // void DeleteLinkedObjects

	void CHClientUr::CreateAccount( udPHandler handler, udUInt id ) {
		udArrBase arr, arr1;
		udHandlerOption option;
		udPBase obj = NULL;
		option.SetTable( "ud_company" );
		option.SetClass( GetProjectCompany );
		handler->GetUdObject( arr1, option );
		option.Clear( );
		for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
			obj = new udProjectPersAcc;
			obj->get( "type" )->ini( ( udInt ) ( udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) );
			obj->get( "obj1" )->ini( id );
			obj->get( "obj2" )->ini( ( *i )->get( "id" )->toUInt( ) );
			arr.push_back( obj );
		}
		option.SetTable( "ud_persacc" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr1 );
		ClearArrBase( arr );
	} // void CreateAccount

	string CHClientUr::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectClientUr;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str;
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "name" )->toWString( );
		if ( !str.empty( ) ) {
			string szStr = ConvertString( str, CP_UTF8 );
			if ( szStr.size( ) ) {
				udDWord	dwLen	= szStr.size( ) * 2;
				udPChar szTmp	= new udChar[ dwLen + 1 ];

				memset( szTmp, 0, dwLen + 1 );
				UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

				szStr = szTmp;

				delete [ ] szTmp;
			}
			string add = config.GetAttrIndex( "name" ) + " LIKE '";

			int pos = 0;
			for( string::iterator i = szStr.begin( ); i != szStr.end( ); ++i, ++pos ) {
				if ( *i == '*' ) {
					add += '%';
				} else if ( *i == '%' ) {
				} else {
					add += *i;
				}
			}

			add += "'";
			arrString.push_back( add );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "cfio" )->toWString( );
		if ( !str.empty( ) ) {
			string szStr = ConvertString( str, CP_UTF8 );
			if ( szStr.size( ) ) {
				udDWord	dwLen	= szStr.size( ) * 2;
				udPChar szTmp	= new udChar[ dwLen + 1 ];

				memset( szTmp, 0, dwLen + 1 );
				UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

				szStr = szTmp;

				delete [ ] szTmp;
			}
			string add = config.GetAttrIndex( "cfio" ) + " LIKE '";

			int pos = 0;
			for( string::iterator i = szStr.begin( ); i != szStr.end( ); ++i, ++pos ) {
				if ( *i == '*' ) {
					add += '%';
				} else if ( *i == '%' ) {
				} else {
					add += *i;
				}
			}

			add += "'";
			arrString.push_back( add );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

} // namepsace UdSDK
