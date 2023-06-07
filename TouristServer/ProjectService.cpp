
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void ServiceFilterPrice( udArrBase& arr ) {
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "currency_" )->ini( wstring( L"" ) );
			( *i )->get( "currency_recommend_" )->ini( wstring( L"" ) );
		}
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectCurrency, "ud_currency", "currency", "name", "currency_" );
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectCurrency, "ud_currency", "currency_recommend", "name", "currency_recommend_" );
	} // void ServiceFilterPrice

	udPBase GetProjectService2( ) {
		udPBase ret = GetProjectService( );
		ret->get( "name_" )->ini( wstring( L"" ) ) ;
		ret->get( "type_" )->ini( wstring( L"" ) ) ;
		ret->get( "partner_" )->ini( wstring( L"" ) ) ;
		ret->get( "period_" )->ini( wstring( L"" ) ) ;
		ret->get( "price_" )->ini( wstring( L"" ) ) ;
		return ret;
	} // udPBase GetProjectService2

	CHService::CHService( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_service";
		m_szUpdIndex	= "id";
		m_szName		= "service";
		m_fnGetObject	= GetProjectService;

		this->AddManagedTable( "ud_serviceperiod", "id", GetProjectServicePeriod );
		this->AddManagedTable( "ud_pricetableitem", "id", GetProjectPriceTableItem );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_nameservice", "name", "id", GetProjectNameService );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_tservice", "type", "id", GetProjectTService );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_country", "country", "id", GetProjectCountry );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_city", "city", "id", GetProjectCity );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_partner", "partner", "id", GetProjectPartner );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_muservice", "munit", "id", GetProjectMUService );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_serviceperiod", "service", "id", GetProjectServicePeriod, "`serviceperiod_id` ASC" );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_pricetableitem", "service", "id", GetProjectPriceTableItem, "`pricetableitem_id` ASC", "", ServiceFilterPrice );

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "name1" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "type" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "partner" )->ini( ( udUInt ) 0 );
	}

	CHService::CHService( const CHService& objCopy ) {
	}

	CHService::~CHService( ) {
	}

	CHService& CHService::operator =( const CHService& objCopy ) {
		return *this;
	}

	udInt CHService::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) { // при добавлении клонов среди услуг искать сложно
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj
	
	udInt CHService::SyncAdd( udPSocket sock, udPBase obj ) {
/*
	в дополнение к сущности услуги прикреплены остальные
	с прописанным количеством.
	порядок:
	1. периоды
	2. цены
*/
		g_pApp->GetLog( )->Write( "CHService::SyncAdd( udPSocket sock, udPBase obj )\n" );

		char res = 0, codePeriod = 0, codePrice = 0;
		udType size( ( udUInt ) 0 );
		udArrBase arrPeriod, arrPrice;
		string log = "\n";
		//////////////////////////////////////////////////////////////////////////////////////////
		// периоды
		if ( size.read( sock ) == 0 ) {
			codePeriod = 81;
			g_pApp->GetLog( )->Write( "не удалось считать заявленное количество периодов\n" );
		} else {
			udPBase objRead = NULL;
			log = "\nзаявлено периодов: " + size.toString( ) + "\n";
			while( arrPeriod.size( ) < size.toUInt( ) ) {
				objRead = new udProjectServicePeriod;
				if ( objRead->read( sock ) == 0 ) {
					delete objRead;
					break;
				}
				arrPeriod.push_back( objRead );
				objRead->dump( log );
				log += "\n";
			}
			//ClearArrBase( arrPeriod );
			g_pApp->GetLog( )->Write( log );
			codePeriod = 80;
		//////////////////////////////////////////////////////////////////////////////////////////
		// цены
			size.ini( ( udUInt ) 0 );
			if ( size.read( sock ) == 0 ) {
				g_pApp->GetLog( )->Write( "не удалось считать заявленное количество цен\n" );
				codePrice = 81;
			} else {
				log = "\nзаявлено цен: " + size.toString( ) + "\n";
				while( arrPrice.size( ) < size.toUInt( ) ) {
					objRead = new udProjectPriceTableItem;
					if ( objRead->read( sock ) == 0 ) {
						delete objRead;
						break;
					}
					arrPrice.push_back( objRead );
					objRead->dump( log );
					log += "\n";
				}
				//ClearArrBase( arrPrice );
				g_pApp->GetLog( )->Write( log );
			}
		}
		sock->SendByte( ( char ) codePeriod );
		sock->SendByte( ( char ) codePrice );
		//////////////////////////////////////////////////////////////////////////////////////////
/*
	поиском заниматься будем позже

	добавляем услугу
	добавляем периоды
	добавляем цены

	собираем все эти данные и отправляем обратно, ожидая закрытия соединения
*/
		//////////////////////////////////////////////////////////////////////////////////////////
		udStateHandlerSimple::AddObj( obj, true );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj1 = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
			obj1 = *i;
			obj1->get( "service" )->ini( obj->get( "id" )->toUInt( ) );
			udStateHandlerSimple::AddObj( obj1, "ud_serviceperiod", true );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		for( udArrBase::iterator i = arrPrice.begin( ); i != arrPrice.end( ); ++i ) {
			obj1 = *i;
			obj1->get( "service" )->ini( obj->get( "id" )->toUInt( ) );
			udStateHandlerSimple::AddObj( obj1, "ud_pricetableitem", true );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udBinaryData data;
		//////////////////////////////////////////////////////////////////////////////////////////
		data.add( ( char ) 1 );
		//////////////////////////////////////////////////////////////////////////////////////////
		log = "\nотправляем ответ\nуслуга:\n";
		obj->dump( log );
		log += "\n";
		obj->dump( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		data.add( arrPeriod.size( ) );
		log += "\nпериоды (" + toString( arrPeriod.size( ) ) + ")\n";
		for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( data );
			obj1->dump( log );
			log += "\n";
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		data.add( arrPrice.size( ) );
		log += "\nцены (" + toString( arrPeriod.size( ) ) + ")\n";
		for( udArrBase::iterator i = arrPrice.begin( ); i != arrPrice.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( data );
			obj1->dump( log );
			log += "\n";
		}
		g_pApp->GetLog( )->Write( log );
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		// ждем ответа от клиента, что он принял данные
		char code = 0;
		udInt read = 0, cnt = 0;
		bool finished = false;
		while( !finished ) {
			read = sock->ReadByte( &code );
			switch( read ) {
				case SOCKET_ERROR:
					g_pApp->GetLog( )->Write( "произошла ошибка при ожидании ответа от клиента\n" );
					finished = true;
					break;

				case 0:
					g_pApp->GetLog( )->Write( "было закрыто соедение с клиентом\n" );
					finished = true;
					break;

				default:
					g_pApp->GetLog( )->Write( "код ответа клиента: " + toString( ( int ) code ) + "\n" );
					++cnt;
					break;
			}
		}
		g_pApp->GetLog( )->Write( "было получено байт ответа: " + toString( cnt ) + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		ClearArrBase( arrPeriod );
		ClearArrBase( arrPrice );
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHService::SyncAdd( udPSocket sock, udPBase obj ) exit\n" );
		return 0;
	} // void SyncAdd

	udInt CHService::SyncUpd( udPSocket sock, udPBase obj ) {
		g_pApp->GetLog( )->Write( "CHService::SyncUpd( udPSocket sock, udPBase obj )\n" );

		if ( obj->get( "id" )->toUInt( ) == 0 ) {
			g_pApp->GetLog( )->Write( "получили объект с нулевым ID\n" );
			sock->SendByte( ( char ) 3 );
			return 0;
		}

		udStateHandlerSimple::UpdObj( obj, true );

		udBinaryData binPeriod, binPrice;
		string log;
		udPBase obj1 = NULL;
		udArrBase arrAdd, arrUpd, arrDel;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		//////////////////////////////////////////////////////////////////////////////////////////
		// периоды
		ReadInstances( sock, arrAdd, GetProjectServicePeriod );
		ReadInstances( sock, arrUpd, GetProjectServicePeriod );
		ReadInstances( sock, arrDel, GetProjectServicePeriod );
		log = "\nскачанные периоды:\n";
		log += "на добавление (" + toString( arrAdd.size( ) ) + ")\n";
		binPeriod.add( arrAdd.size( ) );
		for( udArrBase::iterator i = arrAdd.begin( ); i != arrAdd.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";

			obj1->get( "service" )->ini( obj->get( "id" )->toUInt( ) );
			udStateHandlerSimple::AddObj( obj1, "ud_serviceperiod", true );
			obj1->dump( binPeriod );
		}
		log += "на обновление (" + toString( arrUpd.size( ) ) + ")\n";
		for( udArrBase::iterator i = arrUpd.begin( ); i != arrUpd.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";
		}
		if ( !arrUpd.empty( ) ) {
			for( udArrBase::iterator i = arrUpd.begin( ); i != arrUpd.end( ); ++i ) {
				udStateHandlerSimple::UpdObj( *i, "ud_serviceperiod", "id", true );
			}
		}
		log += "на удаление (" + toString( arrDel.size( ) ) + ")\n";
		for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";
		}
		if ( !arrDel.empty( ) ) {
			for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
				udStateHandlerSimple::DelObj( *i, "ud_serviceperiod", true );
			}
		}
		g_pApp->GetLog( )->Write( log );
		ClearArrBase( arrAdd );
		ClearArrBase( arrUpd );
		ClearArrBase( arrDel );
		sock->SendByte( ( char ) 80 );
		//////////////////////////////////////////////////////////////////////////////////////////
		// цены
		ReadInstances( sock, arrAdd, GetProjectPriceTableItem );
		ReadInstances( sock, arrUpd, GetProjectPriceTableItem );
		ReadInstances( sock, arrDel, GetProjectPriceTableItem );
		log = "\nскачанные цены:\n";
		log += "на добавление (" + toString( arrAdd.size( ) ) + ")\n";
		binPrice.add( arrAdd.size( ) );
		for( udArrBase::iterator i = arrAdd.begin( ); i != arrAdd.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";

			obj1->get( "service" )->ini( obj->get( "id" )->toUInt( ) );
			udStateHandlerSimple::AddObj( obj1, "ud_pricetableitem", true );
			obj1->dump( binPrice );
		}
		log += "на обновление (" + toString( arrUpd.size( ) ) + ")\n";
		for( udArrBase::iterator i = arrUpd.begin( ); i != arrUpd.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";
		}
		if ( !arrUpd.empty( ) ) {
			for( udArrBase::iterator i = arrUpd.begin( ); i != arrUpd.end( ); ++i ) {
				udStateHandlerSimple::UpdObj( *i, "ud_pricetableitem", "id", true );
			}
		}
		log += "на удаление (" + toString( arrDel.size( ) ) + ")\n";
		for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";
		}
		if ( !arrDel.empty( ) ) {
			for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
				udStateHandlerSimple::DelObj( *i, "ud_pricetableitem", true );
			}
		}
		g_pApp->GetLog( )->Write( log );
		ClearArrBase( arrAdd );
		ClearArrBase( arrUpd );
		ClearArrBase( arrDel );
		sock->SendByte( ( char ) 80 );
		//////////////////////////////////////////////////////////////////////////////////////////
		char res = 1;
		udBinaryData data;
		data.add( res );
		obj->dump( data );
		data.add( binPeriod );
		data.add( binPrice );
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		// ждем ответа от клиента, что он принял данные
		char code = 0;
		udInt read = 0, cnt = 0;
		bool finished = false;
		while( !finished ) {
			read = sock->ReadByte( &code );
			switch( read ) {
				case SOCKET_ERROR:
					g_pApp->GetLog( )->Write( "произошла ошибка при ожидании ответа от клиента\n" );
					finished = true;
					break;

				case 0:
					g_pApp->GetLog( )->Write( "было закрыто соедение с клиентом\n" );
					finished = true;
					break;

				default:
					g_pApp->GetLog( )->Write( "код ответа клиента: " + toString( ( int ) code ) + "\n" );
					++cnt;
					break;
			}
		}
		g_pApp->GetLog( )->Write( "было получено байт ответа: " + toString( cnt ) + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHService::SyncUpd( udPSocket sock, udPBase obj ) exit\n" );
		return 0;
	} // void SyncUpd

	udInt CHService::DelObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::DelObj( obj, save_info ) == 1 ) {
			// удаляем связанные объекты
			this->DeleteLinkedObjects( obj );
			return 1;
		}
		return 0;
	} // udInt DelObj

	void CHService::DeleteLinkedObjects( udPBase obj ) {
/*
	для качественной синхронизации необходимо выгрести эти объекты, а потом снести, зарегав все запросы
*/
		udPDatabase db = g_pApp->GetDatabase( );
		udPHandler handler = g_pApp->GetHandler( );
		udArrError err;
		udArrBase arr;
		udHandlerOption option;
		udBaseConfig config;
		string /*query = "", */szWhere = "", log;
		udPBase obj1;
		udPType type = NULL;

		// периоды
		obj1 = new udProjectServicePeriod;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "service" ) + "=" + obj->get( "id" )->toString( );
		option.SetTable( "ud_serviceperiod" );
		option.SetClass( GetProjectServicePeriod );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		log = "\nпериоды на каскадное удаление:\n";
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";
			udStateHandlerSimple::DelObj( obj1, "ud_serviceperiod", true );
		}
		g_pApp->GetLog( )->Write( log );
		if ( !err.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				__log->Write( i->GetText( ) + "\n" );
			}
			err.clear( );
		}
		ClearArrBase( arr );
		/*szWhere = config.GetAttrIndex( "service" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );*/
		config.Clear( );

		// цены
		obj1 = new udProjectPriceTableItem;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "service" ) + "=" + obj->get( "id" )->toString( );
		option.SetTable( "ud_pricetableitem" );
		option.SetClass( GetProjectPriceTableItem );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		log = "\nцены на удаление:\n";
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj1 = *i;
			obj1->dump( log );
			log += "\n";
			udStateHandlerSimple::DelObj( obj1, "ud_pricetableitem", true );
		}
		g_pApp->GetLog( )->Write( log );
		if ( !err.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				__log->Write( i->GetText( ) + "\n" );
			}
			err.clear( );
		}
		ClearArrBase( arr );
		/*szWhere = config.GetAttrIndex( "service" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );*/
		config.Clear( );
	} // void DeleteLinkedObjects

	bool CHService::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_service" );
		option.SetClass( GetProjectService2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "name" ) {
			szOrder = "`service_name_` ";
		} else if ( orderby == "type" ) {
			szOrder = "`service_type_` ";
		} else if ( orderby == "partner" ) {
			szOrder = "`service_partner_` ";
		} else if ( ( orderby == "period" ) || ( orderby == "price" ) ) {
		} else {
			szOrder = "`service_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		option.AddLeftJoin(
			"name", "service_name_", "ud_nameservice",
			"`ud_nameservice`.`nameservice_id`=`ud_service`.`service_name`", GetProjectNameService
		);
		option.AddLeftJoin(
			"name", "service_type_", "ud_tservice",
			"`ud_tservice`.`tservice_id`=`ud_service`.`service_type`", GetProjectTService
		);
		option.AddLeftJoin(
			"name", "service_partner_", "ud_partner",
			"`ud_partner`.`partner_id`=`ud_service`.`service_partner`", GetProjectPartner
		);
		option.SetSqlCalcFoundRows( true );
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetIgnoreAttr( "name_" );
		option.SetIgnoreAttr( "type_" );
		option.SetIgnoreAttr( "partner_" );
		option.SetIgnoreAttr( "period_" );
		option.SetIgnoreAttr( "price_" );
		//////////////////////////////////////////////////////////////////////////////////////////
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPDatabase db = g_pApp->GetDatabase( );
		udArrMorph arr1;
		if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arr1 ) ) {
			cnt = arr1[ 0 ]->get( "cnt" )->toLLong( );
			ClearArrMorph( arr1 );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool CustomLoadObjects

	string CHService::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectService;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		udUInt id = 0;
		wstring str = L"";
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

			string add = "`ud_nameservice`.`nameservice_name` LIKE '";

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
		id = m_pObjFilter->get( "name1" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "name" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "type" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "type" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "partner" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "partner" ) + "=" + toString( id ) );
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

	void CHService::SchemeCenter( udInt opId, udPSocket sock ) {
		if ( opId > opidGet ) {
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			//////////////////////////////////////////////////////////////////////////////////////////
			udPPager pager = new udPager;
			string szOrder, szOrderBy, szOrderType = "DESC";
			udInt iOrderType = 0;
			//////////////////////////////////////////////////////////////////////////////////////////
			// номер страницы
			udType num( ( udInt ) 0 );
			num.read( sock );
			pager->SetPage( num.toInt( ) );
			// размер страницы
			num.read( sock );
			pager->SetPageSize( num.toInt( ) );
			// атрибут упорядочивания
			udType order( string( "" ) );
			order.read( sock );
			szOrderBy = order.toString( );
			// тип упорядочивания
			num.read( sock );
			iOrderType = num.toInt( );
			if ( iOrderType == 1 ) {
				szOrderType = "ASC";
			}
			// набор ID клиентов
			udType ids_num( ( udUInt ) 0 );
			ids_num.read( sock );
			g_pApp->GetLog( )->Write( "CHService::SchemeCenter, ids_num=" + ids_num.toString( ) + "\n" );
			map< udUInt, udUInt > ids;
			udUInt cnt1 = ids_num.toUInt( );
			for( udUInt i = 0; i < cnt1; ++i ) {
				if ( ids_num.read( sock ) == 1 ) {
					ids[ ids_num.toUInt( ) ] = 1;
				}
			}
			// фильтр
			m_pObjFilter->read( sock );
			//
			udPBase obj = m_fnGetObject( );
			udBaseConfig config;
			obj->GetConfig( config );
			udDELETE( obj );
			szOrder = config.GetAttrIndex( szOrderBy ) + " " + szOrderType;
			string szWhere = this->GetWhereCondition( );
			udArrBase arr;
			udBinaryData data;
			if ( ids.empty( ) ) {
				data.add( ( char ) 0 );
			} else {
				udArrString arrString;
				for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
					arrString.push_back( toString( i->first ) );
				}
				if ( szWhere.empty( ) ) {
					szWhere += config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
				} else {
					szWhere += " AND " + config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
				}

				g_pApp->GetLog( )->Write( "CHService::SchemeCenter, where=\"" + szWhere + "\n" );
				config.Clear( );
				//////////////////////////////////////////////////////////////////////////////////////
				udPHandler handler = g_pApp->GetHandler( );
				udHandlerOption option;
				udArrError err;
				udLongLong cnt = 0;

				option.SetTable( m_szTableName );
				option.SetClass( m_fnGetObject );
				if ( !szOrder.empty( ) ) {
					option.SetOrder( szOrder );
				}
				option.SetWhere( szWhere );
				//option.SetLimit( pager->GetSQLLimit( ) );
				//udLongLong cnt = handler->CountUdObject( option );
				string orderby = szOrderBy;
				szOrderType = ( iOrderType == 1 ) ? "ASC" : "DESC", szOrder = "";
				//////////////////////////////////////////////////////////////////////////////////////
				option.SetTable( "ud_service" );
				option.SetClass( GetProjectService2 );
				option.SetLimit( pager->GetSQLLimit( ) );
				if ( orderby == "name" ) {
					szOrder = "`service_name_` ";
				} else if ( orderby == "type" ) {
					szOrder = "`service_type_` ";
				} else if ( orderby == "partner" ) {
					szOrder = "`service_partner_` ";
				} else if ( ( orderby == "period" ) || ( orderby == "price" ) ) {
				} else {
					szOrder = "`service_" + orderby + "` ";
				}
				if ( !szOrder.empty( ) ) {
					szOrder += szOrderType;
				}
				option.SetOrder( szOrder );
				//option.SetWhere( this->GetWhereCondition( ) );
				option.SetWhere( szWhere );
				//////////////////////////////////////////////////////////////////////////////////////
				option.AddLeftJoin(
					"name", "service_name_", "ud_nameservice",
					"`ud_nameservice`.`nameservice_id`=`ud_service`.`service_name`", GetProjectNameService
				);
				option.AddLeftJoin(
					"name", "service_type_", "ud_tservice",
					"`ud_tservice`.`tservice_id`=`ud_service`.`service_type`", GetProjectTService
				);
				option.AddLeftJoin(
					"name", "service_partner_", "ud_partner",
					"`ud_partner`.`partner_id`=`ud_service`.`service_partner`", GetProjectPartner
				);
				option.SetSqlCalcFoundRows( true );
				//////////////////////////////////////////////////////////////////////////////////////
				option.SetIgnoreAttr( "name_" );
				option.SetIgnoreAttr( "type_" );
				option.SetIgnoreAttr( "partner_" );
				option.SetIgnoreAttr( "period_" );
				option.SetIgnoreAttr( "price_" );
				//////////////////////////////////////////////////////////////////////////////////////
				handler->GetUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
				}
				//////////////////////////////////////////////////////////////////////////////////////
				udPDatabase db = g_pApp->GetDatabase( );
				udArrMorph arr1;
				if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arr1 ) ) {
					cnt = arr1[ 0 ]->get( "cnt" )->toLLong( );
					ClearArrMorph( arr1 );
				}
				//////////////////////////////////////////////////////////////////////////////////////
				/*if ( !this->CustomLoadObjects( pager, arr, err, szOrderBy, iOrderType, cnt ) ) {
					handler->GetUdObject( arr, option, &err );
				}*/
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
					data.add( ( char ) 0 );
				} else {
					data.add( ( char ) 1 );
					data.add( ( udUInt ) cnt );
					this->AfterLoadList( arr );
					WriteInstances( arr, data );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			char code = 0;
			sock->ReadByte( &code );
			//////////////////////////////////////////////////////////////////////////////////////////
			delete pager;
			ClearArrBase( arr );
		} else {
			udStateHandlerSimple::SchemeCenter( opId, sock );
		}
	} // void SchemeCenter

} // namespace UdSDK
