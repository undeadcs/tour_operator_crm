
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHCompany::CHCompany( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_company";
		m_szUpdIndex	= "id";
		m_szName		= "company";
		m_fnGetObject	= GetProjectCompany;

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "email" )->ini( wstring( L"" ) );
	}

	CHCompany::CHCompany( const CHCompany& objCopy ) {
	}

	CHCompany::~CHCompany( ) {
	}

	CHCompany& CHCompany::operator =( const CHCompany& objCopy ) {
		return *this;
	}

	udInt CHCompany::AddObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::AddObj( obj, save_info ) == 1 ) {
			// дооплнительно создаем лицевой счет
			this->CreateAccount( g_pApp->GetHandler( ), obj->get( "id" )->toUInt( ) );
			return 1;
		}
		return 0;
	} // udInt AddObj

	udInt CHCompany::DelObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::DelObj( obj, save_info ) == 1 ) {
			// удаляем связанные объекты
			this->DeleteLinkedObjects( obj );
			return 1;
		}
		return 0;
	} // udInt DelObj

	void CHCompany::DeleteLinkedObjects( udPBase obj ) {
		// лицевой счет
		udPDatabase db = g_pApp->GetDatabase( );
		string query = "", szWhere = "";
		udBaseConfig config;
		udPBase obj1;
		obj1 = new udProjectPersAcc;
		obj1->GetConfig( config );
		delete obj1;
		udPType type = new udType( ( udInt ) ( udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) + " AND " +
			config.GetAttrIndex( "obj1" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
	} // void DeleteLinkedObjects

	void CHCompany::CreateAccount( udPHandler handler, udUInt id ) {
		g_pApp->GetLog( )->Write( "CHCompany::CreateAccount, enter\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrError err;
		udArrBase arr, arr1;
		udHandlerOption option;
		udPBase obj = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		// счета у партнеров
		option.SetTable( "ud_partner" );
		option.SetClass( GetProjectPartner );
		handler->GetUdObject( arr1, option );
		option.Clear( );
		for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
			obj = new udProjectPersAcc;
			obj->get( "type" )->ini( ( udInt ) ( udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) );
			obj->get( "obj1" )->ini( id );
			obj->get( "obj2" )->ini( ( *i )->get( "id" )->toUInt( ) );
			arr.push_back( obj );
		}
		ClearArrBase( arr1 );
		option.SetTable( "ud_persacc" );
		handler->AddUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			err.clear( );
		}
		ClearArrBase( arr );
		ClearArrBase( arr1 );
		//////////////////////////////////////////////////////////////////////////////////////////
		// счета физ лиц
		option.SetTable( "ud_client" );
		option.SetClass( GetProjectClient );
		handler->GetUdObject( arr1, option );
		option.Clear( );
		for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
			obj = new udProjectPersAcc;
			obj->get( "type" )->ini( ( udInt ) ( udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) );
			obj->get( "obj1" )->ini( ( *i )->get( "id" )->toUInt( ) );
			obj->get( "obj2" )->ini( id );
			arr.push_back( obj );
		}
		ClearArrBase( arr1 );
		option.SetTable( "ud_persacc" );
		handler->AddUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			err.clear( );
		}
		ClearArrBase( arr );
		ClearArrBase( arr1 );
		//////////////////////////////////////////////////////////////////////////////////////////
		// счета юр лиц
		option.SetTable( "ud_clientur" );
		option.SetClass( GetProjectClientUr );
		handler->GetUdObject( arr1, option );
		option.Clear( );
		for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
			obj = new udProjectPersAcc;
			obj->get( "type" )->ini( ( udInt ) ( udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) );
			obj->get( "obj1" )->ini( ( *i )->get( "id" )->toUInt( ) );
			obj->get( "obj2" )->ini( id );
			arr.push_back( obj );
		}
		ClearArrBase( arr1 );
		option.SetTable( "ud_persacc" );
		handler->AddUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			err.clear( );
		}
		ClearArrBase( arr );
		ClearArrBase( arr1 );
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHCompany::CreateAccount, exit\n" );
	} // void CreateAccount

	string CHCompany::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectCompany;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		string attrs[ 2 ] = { "name", "email" };
		//////////////////////////////////////////////////////////////////////////////////////////
		for( int i = 0; i < 2; ++i ) {
			str = m_pObjFilter->get( attrs[ i ] )->toWString( );
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

				string add = config.GetAttrIndex( attrs[ i ] ) + " LIKE '";

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

} // namespace UdSDK
