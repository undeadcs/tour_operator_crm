
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHReportHotelPay::CHReportHotelPay( ) {
		m_pObjFilter	= new udMorph;

		m_pObjFilter->get( "date1" )->ini( udDatetime( ) );
		m_pObjFilter->get( "date2" )->ini( udDatetime( ) );
		m_pObjFilter->get( "direct" )->ini( ( udUInt ) 0 );
	}

	CHReportHotelPay::CHReportHotelPay( const CHReportHotelPay& objCopy ) {
	}

	CHReportHotelPay::~CHReportHotelPay( ) {
		udDELETE( m_pObjFilter );
	}

	CHReportHotelPay& CHReportHotelPay::operator =( const CHReportHotelPay& objCopy ) {
		return *this;
	}

	void CHReportHotelPay::Init( ) {
	} // void Init

	void CHReportHotelPay::InitSync( udPDatabaseSync sync ) {
	} // void InitSync

	class CGeneratorReportHotelPay {
		udDatetime	m_dtDate1,
					m_dtDate2;
		udUInt		m_iDirect;

	public:
		CGeneratorReportHotelPay( udDatetime& date1, udDatetime& date2, udUInt direct );

		~CGeneratorReportHotelPay( );

		int Proc( udArrBase& arrResult );

	private:
		void	ProcPeriod( udPBase pPeriod, udArrBase& arrResult );

	};

	void CHReportHotelPay::ProcRequest( udPSocket sock ) {
		m_pObjFilter->read( sock );

		string log = "\nfilter, ";
		m_pObjFilter->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udDatetime date1, date2;
		udUInt direct = 0;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		date1 = m_pObjFilter->get( "date1" )->toTime( );
		date2 = m_pObjFilter->get( "date2" )->toTime( );
		direct = m_pObjFilter->get( "direct" )->toUInt( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udArrBase arrResult;
		CGeneratorReportHotelPay objGenerator( date1, date2, direct );
		int ret = objGenerator.Proc( arrResult );

		g_pApp->GetLog( )->Write( "количество записей " + toString( arrResult.size( ) ) + "\n" );

		if ( arrResult.empty( ) ) {
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		} else {
			udBinaryData data;
			data.add( ( char ) 80 );
			WriteInstances( arrResult, data );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			data.clear( );
		}

		ClearArrBase( arrResult );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void ProcRequest

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CGeneratorReportHotelPay::CGeneratorReportHotelPay( udDatetime& date1, udDatetime& date2, udUInt direct ) :
	m_dtDate1( date1 ), m_dtDate2( date2 ), m_iDirect( direct )
	{
	} // CGeneratorReportHotelPay

	CGeneratorReportHotelPay::~CGeneratorReportHotelPay( ) {
	} // ~CGeneratorReportHotelPay

	int CGeneratorReportHotelPay::Proc( udArrBase& arrResult ) {
		udPLog __log = g_pApp->GetLog( );
		string log = "";
		udPHandler handler = g_pApp->GetHandler( );
		udPBase obj = NULL;
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;
		udArrBase arrPeriod;
		int ret = 0;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectGroupPeriod;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		option.SetWhere(
			"(" + config.GetAttrIndex( "date1" ) + ">='" + m_dtDate1.toString( "%Y-%m-%d" ) + "' AND " +
			config.GetAttrIndex( "date1" ) + "<='" + m_dtDate2.toString( "%Y-%m-%d" ) + "') OR (" +
			config.GetAttrIndex( "date2" ) + ">='" + m_dtDate1.toString( "%Y-%m-%d" ) + "' AND " +
			config.GetAttrIndex( "date2" ) + "<='" + m_dtDate2.toString( "%Y-%m-%d" ) + "')"
		);
		option.SetTable( "ud_groupperiod" );
		option.SetClass( GetProjectGroupPeriod );
		option.SetOrder( "`" + config.GetAttrIndex( "date1" ) + "` DESC" );
		handler->GetUdObject( arrPeriod, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			err.clear( );
		}
		if ( !arrPeriod.empty( ) ) {
			string log = "\nОтчет \"Оплата гостиниц\" периоды\n";

			for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
				( *i )->dump( log );
				log += "\n";
			}

			__log->Write( log );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// проверка направления, исключение из набора периодов тех, у которых группа по другому направлению
			if ( m_iDirect ) {
				obj = new udProjectGroup;
				obj->GetConfig( config );
				udDELETE( obj );

				udArrString arrGroupId;

				for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
					arrGroupId.push_back( ( *i )->get( "group" )->toString( ) );
				}

				option.SetWhere(
					config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrGroupId ) + ") AND " +
					config.GetAttrIndex( "direct" ) + "<>" + toString( m_iDirect )
				);
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
					udArrBase arrSave, arrDel;
					udPBase period = NULL, group = NULL;
					bool found = false;

					for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
						period = *i;
						found = false;

						for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
							group = *j;

							if ( period->get( "group" )->toUInt( ) == group->get( "id" )->toUInt( ) ) {
								found = true;
								break;
							}
						}

						if ( found ) {
							arrDel.push_back( period );
						} else {
							arrSave.push_back( period );
						}
					}

					log = "\nCGeneratorReportHotelPay::LoadTours, проверка групп\nпериоды на исключение\n";
					for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					log += "оставшиеся периоды\n";
					for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					__log->Write( log );

					arrPeriod.clear( );

					for( udArrBase::iterator i = arrSave.begin( ); i != arrSave.end( ); ++i ) {
						arrPeriod.push_back( *i );
					}

					arrSave.clear( );
					ClearArrBase( arrDel );
					ClearArrBase( arr );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
				ProcPeriod( *i, arrResult );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			ClearArrBase( arrPeriod );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // int Proc

	/**
	 *	Подсчет числа туристов в туре
	 */
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

	/**
	 *	Возвращает цену за тур из себестоимости
	 *	Ищет среди сумм ту, у которой валюта с кодом CNY
	 */
	static udFloat GetTourCostPrice( udUInt iTourId ) {
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
			config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourCost ) + " AND " +
			config.GetAttrIndex( "obj" ) + "=" + toString( iTourId )
		);

		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			udPBase pSumm = NULL;
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				pSumm = *i;

				if ( pSumm->get( "currency_" )->toWString( ) == wstring( L"CNY" ) ) {
					ret += pSumm->get( "summ" )->toFloat( );
				}
			}

			ClearArrBase( arr );
		}

		return ret;
	} // udFloat GetTourCostPrice

	/**
	 *	Возвращает цену за тур из итоговых
	 *	Ищет среди сумм ту, у которой валюта с кодом RUR
	 */
	static udFloat GetTourFinalPrice( udUInt iTourId ) {
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
			config.GetAttrIndex( "obj" ) + "=" + toString( iTourId )
		);

		handler->GetUdObject( arr, option );
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
	 *	Наполняет реззультирующий набор, услугами, у которых в типе содержится слово Гостиница
	 */
	static void GetTourServiceHotel( udUInt iTourId, udPBase pGroupPeriod, udArrBase& arrResult ) {
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
			config.GetAttrIndex( "tour" ) + "=" + toString( iTourId ) + " AND " +
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
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			err.clear( );
		}
		if ( !arr.empty( ) ) {
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
			if ( !err.empty( ) ) {
				WriteErrors( g_pApp->GetLog( ), err );
				err.clear( );
			}
			if ( !arrService.empty( ) ) {
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

				udPBase add = NULL;

				for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
					obj = *i;
					type = Glib::locale_to_utf8( obj->get( "type_" )->toString( ) );

					if ( re->match( type ) ) {
						add = new udProjectService;
						obj->apply( add );
						arrResult.push_back( add );
					}
				}

				ClearArrBase( arrService );
			}
		}
	} // void GetTourServiceHotel

	void CGeneratorReportHotelPay::ProcPeriod( udPBase pPeriod, udArrBase& arrResult ) {
		udPLog __log = g_pApp->GetLog( );
		string log = "";
		udPHandler handler = g_pApp->GetHandler( );
		udPBase obj = NULL;
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectGroupTour;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		option.SetWhere(
			config.GetAttrIndex( "group" ) + "=" + pPeriod->get( "group" )->toString( ) + " AND " +
			config.GetAttrIndex( "period" ) + "=" + pPeriod->get( "index" )->toString( )
		);
		option.SetTable( "ud_grouptour" );
		option.SetClass( GetProjectGroupTour );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		config.Clear( );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			err.clear( );
		}
		if ( !arr.empty( ) ) {
			//udDwAscBase dwascService;
			map< udUInt, map< udFloat, udPBase > > mapService;
			map< udUInt, map< udFloat, udPBase > >::iterator itServiceId;
			map< udFloat, udPBase >::iterator itServicePrice;
			udArrBase arrService, arrTmp;
			udPBase	tour			= NULL,
					service			= NULL,
					add				= NULL;
			udUInt	iTouristCount	= 0,
					iTourId			= 0;
			udDWord	dwServiceId		= 0;
			udFloat	fPrice			= 0.0f,	// цена в рублях
					fPriceCny		= 0.0f;	// цена в юанях
			udDwAscBase::iterator itFind;
			/*
				в период попадают туры, в турах находятся услуги
				необходимо для каждого тура выгрести все услуги с гостиницей
				сгруппировать их по услуге, подсчитать какое количество человек на нее претендует
				дополнительно услуги группировать по ценам, так что:
				если у одной услуги разные цены в турах, то генерировать 2 строки отчета
			*/
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				tour = *i;
				iTourId = ( *i )->get( "tour" )->toUInt( );
				GetTourServiceHotel( iTourId, pPeriod, arrTmp );
				if ( !arrTmp.empty( ) ) {
					iTouristCount = CountTourTourist( iTourId );
					fPrice = floor( GetTourFinalPrice( iTourId ) / toFloat( iTouristCount ) );
					fPriceCny = floor( GetTourCostPrice( iTourId ) / toFloat( iTouristCount ) );

					for( udArrBase::iterator j = arrTmp.begin( ); j != arrTmp.end( ); ++j ) {
						service = *j;
						dwServiceId = service->get( "id" )->toUInt( );

						itServiceId = mapService.find( dwServiceId );
						if ( itServiceId == mapService.end( ) ) { // новая услуга в наборе
							add = new udProjectService;
							service->apply( add );
							add->get( "#client_count" )->ini( iTouristCount );
							add->get( "#price" )->ini( fPrice );
							add->get( "#price_cny" )->ini( fPriceCny );
							arrService.push_back( add );
							mapService[ dwServiceId ][ fPrice ] = add;
						} else {
							itServicePrice = itServiceId->second.find( fPrice );
							if ( itServicePrice == itServiceId->second.end( ) ) { // другая цена
								add = new udProjectService;
								service->apply( add );
								add->get( "#client_count" )->ini( iTouristCount );
								add->get( "#price" )->ini( fPrice );
								add->get( "#price_cny" )->ini( fPriceCny );
								arrService.push_back( add );
								mapService[ dwServiceId ][ fPrice ] = add;
							} else {
								itServicePrice->second->get( "#client_count" )->ini(
									iTouristCount + itServicePrice->second->get( "#client_count" )->toUInt( )
								);
							}
						}

						/*itFind = dwascService.find( dwServiceId );
						if ( itFind == dwascService.end( ) ) {
							add = new udProjectService;
							service->apply( add );
							add->get( "#client_count" )->ini( iTouristCount );
							add->get( "#price" )->ini( fPrice );
							add->get( "#price_cny" )->ini( fPriceCny );
							dwascService[ dwServiceId ] = add;
						} else {
							if ( fPrice == itFind->second->get( "#price" )->toFloat( ) ) {
								itFind->second->get( "#client_count" )->ini( iTouristCount + itFind->second->get( "#client_count" )->toUInt( ) );
							} else {
							}
						}*/
					}

					ClearArrBase( arrTmp );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			wstring szLeaderName = L"";

			if ( pPeriod->get( "client" )->toUInt( ) ) {
				udArrBase arr1;

				obj = new udProjectClient;
				obj->GetConfig( config );
				udDELETE( obj );

				option.SetWhere( config.GetAttrIndex( "id" ) + "=" + pPeriod->get( "client" )->toString( ) );
				option.SetTable( "ud_client" );
				option.SetClass( GetProjectClient );
				handler->GetUdObject( arr1, option, &err );
				option.Clear( );
				config.Clear( );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
				if ( !arr1.empty( ) ) {
					szLeaderName = ( *( arr1.begin( ) ) )->get( "fio" )->toWString( );
					ClearArrBase( arr1 );
				}
			}

			wstring szDirectName = L"";

			if ( m_iDirect ) {
				udArrBase arr1;

				obj = new udProjectDirect;
				obj->GetConfig( config );
				udDELETE( obj );

				option.SetWhere( config.GetAttrIndex( "id" ) + "=" + toString( m_iDirect ) );
				option.SetTable( "ud_direct" );
				option.SetClass( GetProjectDirect );
				handler->GetUdObject( arr1, option, &err );
				option.Clear( );
				config.Clear( );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
				if ( !arr1.empty( ) ) {
					szDirectName = ( *( arr1.begin( ) ) )->get( "name" )->toWString( );
					ClearArrBase( arr );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// в конце концов берем все услуги, которые попали в период, составляем строки результата
			//for( udDwAscBase::iterator i = dwascService.begin( ); i != dwascService.end( ); ++i ) {
			for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
				service = *i;//->second;
				add = new udProjectReportHotelPay;
				add->get( "date1" )->ini( pPeriod->get( "date1" )->toTime( ) );
				add->get( "date2" )->ini( pPeriod->get( "date2" )->toTime( ) );
				add->get( "leader" )->ini( szLeaderName );
				add->get( "num" )->ini( service->get( "#client_count" )->toUInt( ) );
				add->get( "name" )->ini( service->get( "name_" )->toWString( ) );
				add->get( "price" )->ini( service->get( "#price" )->toFloat( ) );
				add->get( "price_cny" )->ini( service->get( "#price_cny" )->toFloat( ) );
				add->get( "period_date1" )->ini( m_dtDate1 );
				add->get( "period_date2" )->ini( m_dtDate2 );
				add->get( "direct" )->ini( szDirectName );
				arrResult.push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			//ClearDwAscBase( dwascService );
			ClearArrBase( arrService );
			ClearArrBase( arr );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void ProcPeriod

} // namespace UdSDK
