
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	// глупо, но сделаем пока так
	static udUInt CountTourTourist( udUInt id ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udPBase obj = NULL;
		udUInt ret = 0;
		obj = new udProjectTourClient;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_tourclient" );
		option.SetClass( GetProjectTourClient );
		option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + toString( id ) );
		handler->GetUdObject( arr, option );
		ret = arr.size( );
		ClearArrBase( arr );
		return ret;
	} // udUInt CountTourTourist

	CHReportDayPay::CHReportDayPay( ) {
		m_pObjFilter	= new udMorph;

		m_pObjFilter->get( "date1" )->ini( udDatetime( ) );
		m_pObjFilter->get( "date2" )->ini( udDatetime( ) );
	}

	CHReportDayPay::CHReportDayPay( const CHReportDayPay& objCopy ) {
	}

	CHReportDayPay::~CHReportDayPay( ) {
		udDELETE( m_pObjFilter );
	}

	CHReportDayPay& CHReportDayPay::operator =( const CHReportDayPay& objCopy ) {
		return *this;
	}

	void CHReportDayPay::Init( ) {
	} // void Init

	void CHReportDayPay::InitSync( udPDatabaseSync sync ) {
	} // void InitSync

	static void GetTourClient( udArrBase& arrResult, udUInt id ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj = NULL;
		udArrBase arr;

		obj = new udProjectTourClient;
		obj->GetConfig( config );
		udDELETE( obj );

		option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + toString( id ) );
		option.SetTable( "ud_tourclient" );
		option.SetClass( GetProjectTourClient );
		handler->GetUdObject( arr, option );
		option.Clear( );
		config.Clear( );
		if ( !arr.empty( ) ) {
			udArrString arrClientId;

			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				arrClientId.push_back( ( *i )->get( "client" )->toString( ) );
			}

			obj = new udProjectClient;
			obj->GetConfig( config );
			udDELETE( obj );

			option.SetWhere( config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrClientId ) + ")" );
			option.SetTable( "ud_client" );
			option.SetClass( GetProjectClient );
			handler->GetUdObject( arrResult, option );

			ClearArrBase( arr );
		}
	} // void GetTourClient

	/**
	 *	Возвращает цену за тур из итоговых
	 *	Ищет среди сумм ту, у которой валюта с кодом RUR
	 */
	static udFloat GetTourFinalPrice( udPBase pTour ) {
		udFloat ret = 0.0f;

		// загружаем суммы из итоговой цены
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj1 = NULL;
		udArrBase arr;
		
		// наполняем конфиг
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		// наполняем опции
		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm2 );
		option.SetIgnoreAttr( "currency_" );
		option.AddLeftJoin(
			"code", "fsumm_currency_", "ud_currency",
			"`ud_currency`.`currency_id`=`ud_fsumm`.`fsumm_currency`", GetProjectCurrency
		);
		option.SetWhere(
			config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourPrice ) + " AND " +
			config.GetAttrIndex( "obj" ) + "=" + pTour->get( "id" )->toString( )
		);

		handler->GetUdObject( arr, option );

		g_pApp->GetLog( )->Write( "GetTourFinalPrice, arr.size=" + toString( arr.size( ) ) + "\n" );

		if ( !arr.empty( ) ) {
			udPBase pSumm = NULL;
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				pSumm = *i;

				if ( pSumm->get( "currency_" )->toWString( ) == wstring( L"RUR" ) ) {
					ret = pSumm->get( "summ" )->toFloat( );
					break;
				}
			}

			ClearArrBase( arr );
		}

		return ret;
	} // udFloat GetTourFinalPrice

	/**
	 *	Возвращает цену за тур из оплаты
	 *	Ищет среди сумм ту, у которой валюта с кодом RUR
	 */
	static udFloat GetTourPayPrice( udPBase pTour ) {
		udFloat ret = 0.0f;

		// загружаем суммы из итоговой цены
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj1 = NULL;
		udArrBase arr;
		
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		
		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm2 );
		option.SetIgnoreAttr( "currency_" );
		option.AddLeftJoin(
			"code", "fsumm_currency_", "ud_currency",
			"`ud_currency`.`currency_id`=`ud_fsumm`.`fsumm_currency`", GetProjectCurrency
		);
		option.SetWhere(
			config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTour ) + " AND " +
			config.GetAttrIndex( "obj" ) + "=" + pTour->get( "id" )->toString( )
		);

		handler->GetUdObject( arr, option );

		g_pApp->GetLog( )->Write( "GetTourPayPrice, arr.size=" + toString( arr.size( ) ) + "\n" );

		if ( !arr.empty( ) ) {
			udPBase pSumm = NULL;
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				pSumm = *i;

				if ( pSumm->get( "currency_" )->toWString( ) == wstring( L"RUR" ) ) {
					ret = pSumm->get( "summ" )->toFloat( );
					break;
				}
			}

			ClearArrBase( arr );
		}

		return ret;
	} // udFloat GetTourPayPrice

	/**
	 *	Возвращает наименование услуги, у которой тип содержит Гостиница
	 */
	static wstring GetTourServiceHotel( udPBase pTour, udPBase pGroupPeriod ) {
		g_pApp->GetLog( )->Write( "GetTourServiceHotel\n" );
		wstring szRet( L"" );
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udPBase obj = NULL;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;

		obj = new udProjectTourService;
		obj->GetConfig( config );
		udDELETE( obj );

		option.SetTable( "ud_tourservice" );
		option.SetClass( GetProjectTourService );
		option.SetWhere(
			config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( ) + " AND " +
			// случай когда начало периода услуги находится в периоде
			"((" + config.GetAttrIndex( "date1" ) + ">='" + pGroupPeriod->get( "date1" )->toString( ) + "' AND " +
			config.GetAttrIndex( "date1" ) + "<='" + pGroupPeriod->get( "date2" )->toString( ) + "') OR (" +
			// случай когда конец периода услуги находится в периоде
			config.GetAttrIndex( "date2" ) + ">='" + pGroupPeriod->get( "date1" )->toString( ) + "' AND " +
			config.GetAttrIndex( "date2" ) + "<='" + pGroupPeriod->get( "date2" )->toString( ) + "') OR (" +
			// случай когда период группы попадает в период оказания услуги
			config.GetAttrIndex( "date1" ) + "<='" + pGroupPeriod->get( "date1" )->toString( ) + "' AND " +
			config.GetAttrIndex( "date2" ) + ">='" + pGroupPeriod->get( "date2" )->toString( ) + "'))"
		);
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		config.Clear( );
		if ( arr.empty( ) ) {
			g_pApp->GetLog( )->Write( "GetTourServiceHotel, tour services not found\n" );
			if ( !err.empty( ) ) {
				WriteErrors( g_pApp->GetLog( ), err );
			}
		} else {
			g_pApp->GetLog( )->Write( "GetTourServiceHotel, tour services found, size=" + toString( arr.size( ) ) + "\n" );

			udArrBase arrService;
			udArrString arrId;

			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				arrId.push_back( ( *i )->get( "service" )->toString( ) );
			}

			ClearArrBase( arr );

			obj = new udProjectService;
			obj->GetConfig( config );
			udDELETE( obj );

			option.SetTable( "ud_service" );
			option.SetClass( GetProjectService );
			option.SetWhere( config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrId ) + ")" );
			handler->GetUdObject( arrService, option, &err );
			option.Clear( );
			config.Clear( );
			arrId.clear( );
			if ( arrService.empty( ) ) {
				g_pApp->GetLog( )->Write( "GetTourServiceHotel, services not found\n" );
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
				}
			} else {
				g_pApp->GetLog( )->Write( "GetTourServiceHotel, services found, size=" + toString( arrService.size( ) ) + "\n" );

				LoadListDataByAttr( arrService, handler, GetProjectNameService, "ud_nameservice", "name", "name", "name_" );
				LoadListDataByAttr( arrService, handler, GetProjectTService, "ud_tservice", "type", "name", "type_" );

				Glib::ustring type;
				Glib::RefPtr< Glib::Regex > re = Glib::Regex::create( Glib::locale_to_utf8( "Гостиница" ), Glib::REGEX_CASELESS );

				string log = "\nservices:\n";
				for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				g_pApp->GetLog( )->Write( log );

				for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
					obj = *i;
					type = Glib::locale_to_utf8( obj->get( "type_" )->toString( ) );

					if ( re->match( type ) ) {
						szRet = obj->get( "name_" )->toWString( );
						break;
					}
				}

				ClearArrBase( arrService );
			}
		}

		return szRet;
	} // wstring GetTourServiceHotel

	static udPBase GetLastTourPayOperation( udPBase pTour ) {
		udPBase ret = NULL;

		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj = NULL;
		udArrBase arr;

		// операции по оплате тура
		obj = new udProjectFinanceOperation;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_fop" );
		option.SetClass( GetProjectFinanceOperation );
		option.SetOrder( config.GetAttrIndex( "created" ) + " DESC" );
		option.SetWhere(
			config.GetAttrIndex( "type" ) + "=" + toString( ( udInt ) udProjectFinanceOperation::fotPayTour ) + " AND " +
			config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( )
		);
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option );
		option.Clear( );
		config.Clear( );
		if ( !arr.empty( ) ) {
			ret = arr[ 0 ];
			arr.clear( );
		}

		return ret;
	} // udPBase GetLastTourPayOperation

	static void GetTourAdditionalData( udPBase pTour, udPPBase ppGroup, udPPBase ppPeriod, udPPBase ppManager ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udBaseConfig config;
		udPBase obj = NULL;
		udPLog __log = g_pApp->GetLog( );
		string log = "";
		//////////////////////////////////////////////////////////////////////////////////
		*ppGroup = new udProjectGroup;
		*ppPeriod = new udProjectGroupPeriod;
		*ppManager = new udProjectUser;
		//////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectGroupTour;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( ) );
		option.SetTable( "ud_grouptour" );
		option.SetClass( GetProjectGroupTour );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option, &err );
		config.Clear( );
		option.Clear( );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			err.clear( );
		}
		if ( !arr.empty( ) ) {
			udPBase pLink = new udProjectGroupTour;
			( *( arr.begin( ) ) )->apply( pLink );
			ClearArrBase( arr );

			obj = new udProjectGroup;
			obj->GetConfig( config );
			udDELETE( obj );

			option.SetWhere( config.GetAttrIndex( "id" ) + "=" + pLink->get( "group" )->toString( ) );
			option.SetTable( "ud_group" );
			option.SetClass( GetProjectGroup );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			if ( !arr.empty( ) ) {
				( *( arr.begin( ) ) )->apply( *ppGroup );
				ClearArrBase( arr );
			}

			obj = new udProjectGroupPeriod;
			obj->GetConfig( config );
			udDELETE( obj );

			option.SetWhere(
				config.GetAttrIndex( "group" ) + "=" + pLink->get( "group" )->toString( ) + " AND " +
				config.GetAttrIndex( "index" ) + "=" + pLink->get( "period" )->toString( )
			);
			option.SetTable( "ud_groupperiod" );
			option.SetClass( GetProjectGroupPeriod );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			if ( !arr.empty( ) ) {
				( *( arr.begin( ) ) )->apply( *ppPeriod );
				ClearArrBase( arr );
			}

			udDELETE( pLink );
		}
		//////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectUser;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + pTour->get( "manager2" )->toString( ) );
		option.SetTable( "ud_user" );
		option.SetClass( GetProjectUser );
		option.SetLimit( "1" );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			err.clear( );
		}
		if ( !arr.empty( ) ) {
			( *( arr.begin( ) ) )->apply( *ppManager );

			ClearArrBase( arr );
		}
		//////////////////////////////////////////////////////////////////////////////////
	} // void GetTourAdditionalData

	static void CollectTourData( udArrBase& arrResult, udPBase pTour ) {
		/**
		 *	1. Выгребаем туристов, добавляем их туда
		 *	2. Коллекционируем данные по операциям
		 */
		udPLog __log = g_pApp->GetLog( );
		string log = "";

		log = "CollectTourData\ntour, ";
		pTour->dump( log );
		__log->Write( log + "\n" );

		udArrBase arrClient;
		GetTourClient( arrClient, pTour->get( "id" )->toUInt( ) );

		log = "\nclient size=" + toString( arrClient.size( ) ) + "\n";
		for( udArrBase::iterator i = arrClient.begin( ); i != arrClient.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );

		udPBase pGroup			= NULL,
				pGroupPeriod	= NULL,
				pManager		= NULL;
		GetTourAdditionalData( pTour, &pGroup, &pGroupPeriod, &pManager );

		log = "group, ";
		pGroup->dump( log );
		log += "\nperiod, ";
		pGroupPeriod->dump( log );
		log += "\nmanager, ";
		pManager->dump( log );
		__log->Write( log + "\n" );

		udPBase add				= NULL,
				pClient			= NULL;
		udFloat fCount			= toFloat( CountTourTourist( pTour->get( "id" )->toUInt( ) ) ),
				fFinalPrice		= floor( GetTourFinalPrice( pTour ) / fCount ),
				fPayPrice		= floor( GetTourPayPrice( pTour ) / fCount );
		wstring szServiceHotel	= GetTourServiceHotel( pTour, pGroupPeriod ),
				szPlace			= L"";
		udDatetime	date;

		udPBase pLastOperation = GetLastTourPayOperation( pTour );
		if ( pLastOperation ) {
			szPlace = pLastOperation->get( "comment" )->toWString( );
			date = pLastOperation->get( "created" )->toTime( );
			delete pLastOperation;
		}

		log = "results\n";

		wstring szPeriod = L"";

		if ( pGroupPeriod->get( "id" )->toUInt( ) ) {
			szPeriod = pGroupPeriod->get( "date1" )->toWString( L"%d.%m.%Y" ) + L" - " + pGroupPeriod->get( "date2" )->toWString( L"%d.%m.%Y" );
		}

		for( udArrBase::iterator i = arrClient.begin( ); i != arrClient.end( ); ++i ) {
			pClient = *i;
			add = new udProjectReportDayPay;
			add->get( "date" )->ini( pTour->get( "created" )->toTime( ) );
			add->get( "fio" )->ini( pClient->get( "fio" )->toWString( ) );
			add->get( "price" )->ini( fFinalPrice );
			add->get( "pay" )->ini( fPayPrice );
			add->get( "date" )->ini( date );
			add->get( "place" )->ini( szPlace );
			add->get( "hotel" )->ini( szServiceHotel );
			add->get( "tour" )->ini( pTour->get( "id" )->toUInt( ) );
			add->get( "group" )->ini( pGroup->get( "name" )->toWString( ) );
			add->get( "period" )->ini( szPeriod );
			add->get( "manager" )->ini( pManager->get( "fio" )->toWString( ) );
			arrResult.push_back( add );

			add->dump( log );
			log += "\n";
		}

		__log->Write( log );

		udDELETE( pGroup );
		udDELETE( pGroupPeriod );
		udDELETE( pManager );
		ClearArrBase( arrClient );
	} // void CollectTourData

	static void CollectData( udArrBase& arrResult, udArrBase& arrTour ) {
		for( udArrBase::iterator i = arrTour.begin( ); i != arrTour.end( ); ++i ) {
			CollectTourData( arrResult, *i );
		}
	} // void CollectData

	void CHReportDayPay::ProcRequest( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CHReportDayPay\n//////////////////////////////////////////////////////////////////////////////////////////\n" );

		m_pObjFilter->read( sock );

		string log = "\nfilter, ";
		m_pObjFilter->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );

		//////////////////////////////////////////////////////////////////////////////////////////
		string szQuery;
		//////////////////////////////////////////////////////////////////////////////////////////
		udDatetime date1, date2;
		//////////////////////////////////////////////////////////////////////////////////////////
		date1 = m_pObjFilter->get( "date1" )->toUInt( );
		date2 = m_pObjFilter->get( "date2" )->toUInt( );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPHandler handler = g_pApp->GetHandler( );
		udPLog __log = g_pApp->GetLog( );
		udPBase obj = NULL;
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;
		//////////////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectTour;
		obj->GetConfig( config );
		udDELETE( obj );

		option.SetWhere(
			config.GetAttrIndex( "created" ) + ">='" + date1.toString( "%Y-%m-%d" ) + "' AND " +
			config.GetAttrIndex( "created" ) + "<='" + date2.toString( "%Y-%m-%d" ) + " 23:59:59'"
		);
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) || arr.empty( ) ) {
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		} else {
			udArrBase arrResult;

			CollectData( arrResult, arr );

			__log->Write( "result size=" + toString( arrResult.size( ) ) + "\n" );

			if ( arrResult.empty( ) ) {
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 0 );
			} else {
				for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
					obj = *i;
					obj->get( "date1" )->ini( date1 );
					obj->get( "date2" )->ini( date2 );
				}

				udBinaryData data;
				data.add( ( char ) 80 );
				WriteInstances( arrResult, data );

				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );

				data.clear( );
				ClearArrBase( arrResult );
			}
			
			ClearArrBase( arr );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "\n//////////////////////////////////////////////////////////////////////////////////////////\n" );
	} // void ProcRequest

} // namespace UdSDK
