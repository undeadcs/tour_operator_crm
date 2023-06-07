
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

	CHReportGroupPay::CHReportGroupPay( ) {
		m_pObjFilter	= new udMorph;

		m_pObjFilter->get( "group" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "period" )->ini( ( udUInt ) 0 );
	}

	CHReportGroupPay::CHReportGroupPay( const CHReportGroupPay& objCopy ) {
	}

	CHReportGroupPay::~CHReportGroupPay( ) {
		udDELETE( m_pObjFilter );
	}

	CHReportGroupPay& CHReportGroupPay::operator =( const CHReportGroupPay& objCopy ) {
		return *this;
	}

	void CHReportGroupPay::Init( ) {
	} // void Init

	void CHReportGroupPay::InitSync( udPDatabaseSync sync ) {
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

	static void CollectTourData( udArrBase& arrResult, udPBase pTour, udPBase pGroup, udPBase pGroupPeriod ) {
		/**
		 *	1. Выгребаем туристов, добавляем их туда
		 *	2. Коллекционируем данные по операциям
		 */
		udArrBase arrClient, arrToResult;
		GetTourClient( arrClient, pTour->get( "id" )->toUInt( ) );

		g_pApp->GetLog( )->Write( "CollectTourData, arrClient.size=" + toString( arrClient.size( ) ) + "\n" );

		udPBase add				= NULL,
				pClient			= NULL;
		udFloat fCount			= toFloat( CountTourTourist( pTour->get( "id" )->toUInt( ) ) ),
				fFinalPrice		= floor( GetTourFinalPrice( pTour ) / fCount ),
				fPayPrice		= floor( GetTourPayPrice( pTour ) / fCount );
		wstring //szFinalPrice	= fFinalPrice ? toWString( fFinalPrice ) : wstring( L"" ),
				//szPayPrice		= fPayPrice ? toWString( fPayPrice ) : wstring( L"" ),
				szServiceHotel	= GetTourServiceHotel( pTour, pGroupPeriod ),
				szPlace			= L"";
		udDatetime	date;
		udPBase pLastOperation = GetLastTourPayOperation( pTour );
		if ( pLastOperation ) {
			szPlace = pLastOperation->get( "comment" )->toWString( );
			date = pLastOperation->get( "created" )->toTime( );
			delete pLastOperation;
		}

		for( udArrBase::iterator i = arrClient.begin( ); i != arrClient.end( ); ++i ) {
			pClient = *i;
			add = new udProjectReportGroupPay;
			add->get( "fio" )->ini( pClient->get( "fio" )->toWString( ) );
			add->get( "price" )->ini( fFinalPrice );
			add->get( "pay" )->ini( fPayPrice );
			add->get( "date" )->ini( date );
			add->get( "place" )->ini( szPlace );
			add->get( "hotel" )->ini( szServiceHotel );
			add->get( "tour" )->ini( pTour->get( "id" )->toUInt( ) );
			arrResult.push_back( add );
		}

		ClearArrBase( arrClient );
	} // void CollectTourData

	static void CollectData( udArrBase& arrResult, udArrBase& arrTour, udPBase pGroup, udPBase pGroupPeriod ) {
		for( udArrBase::iterator i = arrTour.begin( ); i != arrTour.end( ); ++i ) {
			CollectTourData( arrResult, *i, pGroup, pGroupPeriod );
		}
	} // void CollectData

	void CHReportGroupPay::ProcRequest( udPSocket sock ) {
		m_pObjFilter->read( sock );

		string log = "\nfilter, ";
		m_pObjFilter->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );

		//wstring text = L"отправка отчета\r\n";
		//////////////////////////////////////////////////////////////////////////////////
		string szQuery;
		//////////////////////////////////////////////////////////////////////////////////////////
		udUInt group, period;
		//////////////////////////////////////////////////////////////////////////////////////////
		group = m_pObjFilter->get( "group" )->toUInt( );
		period = m_pObjFilter->get( "period" )->toUInt( );
		//////////////////////////////////////////////////////////////////////////////////////////
		char ret = 80;
		udPHandler handler = g_pApp->GetHandler( );
		udPBase obj = NULL;
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( ( group > 0 ) && ( period > 0 ) ) {
			// выгребаем группу
			obj = new udProjectGroup;
			obj->GetConfig( config );
			udDELETE( obj );
			option.SetWhere( config.GetAttrIndex( "id" ) + "=" + m_pObjFilter->get( "group" )->toString( ) );
			option.SetTable( "ud_group" );
			option.SetClass( GetProjectGroup );
			option.SetLimit( "1" );
			handler->GetUdObject( arr, option );
			option.Clear( );
			config.Clear( );
			if ( arr.empty( ) ) {
				ret = 0;
			} else {
				udPBase pGroup = arr[ 0 ];
				arr.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				// выгребаем период
				obj = new udProjectGroupPeriod;
				obj->GetConfig( config );
				udDELETE( obj );
				option.SetWhere(
					config.GetAttrIndex( "id" ) + "=" + m_pObjFilter->get( "period" )->toString( ) + " AND " +
					config.GetAttrIndex( "group" ) + "=" + m_pObjFilter->get( "group" )->toString( )
				);
				option.SetTable( "ud_groupperiod" );
				option.SetClass( GetProjectGroupPeriod );
				option.SetLimit( "1" );
				handler->GetUdObject( arr, option );
				option.Clear( );
				config.Clear( );
				if ( arr.empty( ) ) {
					ret = 0;
				} else {
					udPBase pGroupPeriod = arr[ 0 ];
					arr.clear( );
					//////////////////////////////////////////////////////////////////////////////
					// выгребаем связи групп и туров и периода
					obj = new udProjectGroupTour;
					obj->GetConfig( config );
					udDELETE( obj );
					option.SetWhere(
						config.GetAttrIndex( "group" ) + "=" + m_pObjFilter->get( "group" )->toString( ) + " AND " +
						config.GetAttrIndex( "period" ) + "=" + pGroupPeriod->get( "index" )->toString( )
					);
					option.SetTable( "ud_grouptour" );
					option.SetClass( GetProjectGroupTour );
					option.SetOrder( "`" + config.GetAttrIndex( "id" ) + "` ASC" );
					handler->GetUdObject( arr, option );
					option.Clear( );
					config.Clear( );
					if ( arr.empty( ) ) {
						ret = 0;
					} else {
						//////////////////////////////////////////////////////////////////////////
						// составляем список ID туров и выгребаем их
						udArrBase arrTour;
						udArrString arrTourId;
						
						for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
							arrTourId.push_back( ( *i )->get( "tour" )->toString( ) );
						}
						ClearArrBase( arr );

						obj = new udProjectTour;
						obj->GetConfig( config );
						udDELETE( obj );
						option.SetWhere( config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrTourId ) + ")" );
						option.SetTable( "ud_tour" );
						option.SetClass( GetProjectTour );
						handler->GetUdObject( arrTour, option );
						option.Clear( );
						config.Clear( );
						if ( arrTour.empty( ) ) {
							ret = 0;
						} else {
							//////////////////////////////////////////////////////////////////////
							udPBase pClient = NULL;

							if ( pGroupPeriod->get( "client" )->toUInt( ) ) {
								obj = new udProjectClient;
								obj->GetConfig( config );
								udDELETE( obj );
								option.SetWhere( config.GetAttrIndex( "id" ) + "=" + pGroupPeriod->get( "client" )->toString( ) );
								option.SetTable( "ud_client" );
								option.SetClass( GetProjectClient );
								handler->GetUdObject( arr, option );
								if ( arr.empty( ) ) {
									ret = 0;
								} else {
									pClient = arr[ 0 ];
									arr.clear( );
								}
							}
							//////////////////////////////////////////////////////////////////////
							udArrBase arrResult;
							CollectData( arrResult, arrTour, pGroup, pGroupPeriod );
							//////////////////////////////////////////////////////////////////////
							udBinaryData data;
							data.add( ( char ) 80 );
							//////////////////////////////////////////////////////////////////////
							log = "\ngroup, ";
							pGroup->dump( log );
							g_pApp->GetLog( )->Write( log + "\n" );

							pGroup->dump( data );

							log = "\nperiod, ";
							pGroupPeriod->dump( log );
							g_pApp->GetLog( )->Write( log + "\n" );

							pGroupPeriod->dump( data );

							if ( pClient ) {
								log = "\nclient - group leader, ";
								pClient->dump( log );
								g_pApp->GetLog( )->Write( log + "\n" );

								data.add( ( char ) 1 );
								pClient->dump( data );
							} else {
								data.add( ( char ) 0 );
							}

							WriteInstances( arrResult, data );
							//////////////////////////////////////////////////////////////////////
							sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
							sock->Send( data );
							data.clear( );
							//////////////////////////////////////////////////////////////////////
							ClearArrBase( arrResult );
							ClearArrBase( arrTour );
							//////////////////////////////////////////////////////////////////////
						}
						//////////////////////////////////////////////////////////////////////////
					}
					//////////////////////////////////////////////////////////////////////////////
				}
				//////////////////////////////////////////////////////////////////////////////////
				udDELETE( pGroup );
				//////////////////////////////////////////////////////////////////////////////////
			}
		} else {
			ret = 0;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( ret != 80 ) { // костыль, убрать
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ret );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ProcRequest

} // namespace UdSDK
