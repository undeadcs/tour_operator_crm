
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourPayOrder2( ) {
		udPBase ret = GetProjectTourPayOrder( );
		ret->get( "tour_" )->ini( wstring( L"" ) );
		ret->get( "employer_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourPayOrder2

	CHTourPayOrder::CHTourPayOrder( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_tourpayorder";
		m_szUpdIndex	= "id";
		m_szName		= "tourpayorder";
		m_fnGetObject	= GetProjectTourPayOrder;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_tour", "tour", "id", GetProjectTour );

		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "tour" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "state" )->ini( ( udInt ) 0 );
	}

	CHTourPayOrder::~CHTourPayOrder( ) {
	}

	CHTourPayOrder::CHTourPayOrder( const CHTourPayOrder& objCopy ) {
	}

	CHTourPayOrder& CHTourPayOrder::operator =( const CHTourPayOrder& objCopy ) {
		return *this;
	}

	udInt CHTourPayOrder::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;
		}
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // udInt FindObj

	static void LoadEmployer( udArrBase& arrResult ) {
		udPHandler handler = g_pApp->GetHandler( );
		udArrBase arrAttr, arrClient, arrClientUr;
		udPBase attr = NULL;

		// 1. загружаем ID заказчика и его тип из тура
		attr = new udBase;
		attr->get( "attr1" )->ini( wstring( L"employer" ) );
		attr->get( "attr2" )->ini( wstring( L"#employer_id" ) );
		attr->get( "type" )->ini( ( udUInt ) 0 );
		arrAttr.push_back( attr );

		attr = new udBase;
		attr->get( "attr1" )->ini( wstring( L"emptype" ) );
		attr->get( "attr2" )->ini( wstring( L"#employer_type" ) );
		attr->get( "type" )->ini( ( udUInt ) 0 );
		arrAttr.push_back( attr );
		// загрузка
		LoadListDataByAttr( arrResult, handler, GetProjectTour, "ud_tour", "tour", arrAttr );

		udPBase obj = NULL;
		udUInt type = 0;
		// разбиение на подмножества заказчиков юр и физ лиц
		for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
			obj = *i;
			type = obj->get( "#employer_type" )->toUInt( );

			if ( type == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
				arrClient.push_back( obj );
			} else if ( type == udProjectTour::TOUR_EMPLOYER_CLIENTUR ) {
				arrClientUr.push_back( obj );
			}
		}

		// загрузка заказчиков
		// физ лица
		LoadListDataByAttr( arrClient, handler, GetProjectClient, "ud_client", "#employer_id", "fio", "employer_" );
		// юр лица
		LoadListDataByAttr( arrClientUr, handler, GetProjectClientUr, "ud_clientur", "#employer_id", "name", "employer_" );

		// убираем фиктивные атрибуты
		for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
			obj = *i;
			obj->unset( "#employer_id" );
			obj->unset( "#employer_type" );
		}

		ClearArrBase( arrAttr );
	} // void LoadEmployer

	bool CHTourPayOrder::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_tourpayorder" );
		option.SetClass( GetProjectTourPayOrder2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "tour" ) {
			szOrder = "`tourpayorder_tour_` ";
		} else {
			szOrder = "`tourpayorder_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"name", "tourpayorder_tour_", "ud_tour",
			"`ud_tour`.`tour_id`=`ud_tourpayorder`.`tourpayorder_tour`", GetProjectTour
		);
		option.SetSqlCalcFoundRows( true );
		option.SetIgnoreAttr( "tour_" );
		option.SetIgnoreAttr( "employer_" );
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
		if ( !arr.empty( ) ) {
			LoadEmployer( arr );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool CustomLoadObjects

	void CHTourPayOrder::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arrLoaded, arrCurrent;
		udArrError err;
		udPBase obj1 = NULL;
		udBaseConfig config;
		string szWhere = "";
		udUInt id = obj->get( "id" )->toUInt( );
		udPHObjectLock hLock = g_pApp->GetModel( )->GetLockHandler( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// суммы
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourPayOrder );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Средства размещения
		ReadInstances( sock, arrLoaded, GetProjectFinanceSumm );
		LoadInstanceByForeignKey( handler, obj, arrCurrent, GetProjectFinanceSumm, "ud_fsumm", "obj", "id", "", szWhere );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "obj" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_fsumm", true );
						hLock->UnlockObject( obj1, GetProjectFinanceSumm );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "obj" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_fsumm", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "obj" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_fsumm", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "obj" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_fsumm", "id", true );
					hLock->UnlockObject( obj1, GetProjectFinanceSumm );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "obj" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_fsumm", true );
						hLock->UnlockObject( obj1, GetProjectFinanceSumm );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		/*udArrBase arr;
		char from_tour = 0;
		// считываем суммы
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &from_tour );
		ReadInstances( sock, arr, GetProjectFinanceSumm );
		if ( from_tour == 1 ) {
			if ( !arr.empty( ) ) {
				// проставляем связи в суммы
				string log = "\ntourpayordersumm:\n";
				udPBase obj1 = NULL;
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj1 = *i;
					obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtTourPayOrder );
					obj1->get( "obj" )->ini( obj->get( "id" )->toUInt( ) );
					obj1->dump( log );
					log += "\n";
				}
				g_pApp->GetLog( )->Write( log );
				// сохраняем данные
				udHandlerOption option;
				option.SetTable( "ud_fsumm" );
				g_pApp->GetHandler( )->AddUdObject( arr, option );
			}
		}
		ClearArrBase( arr );
		// подтверждаем прием
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );*/
	} // void ExtraDataRecieve

	void CHTourPayOrder::ExtraDataSending( udPSocket sock, udPBase obj ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udPBase obj1 = NULL;
		udBaseConfig config;
		string szWhere = "";
		//////////////////////////////////////////////////////////////////////////////////////////
		// суммы
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourPayOrder ) + " AND " +
			config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( );
		g_pApp->GetLog( )->Write( "where=\"" + szWhere + "\"\n" );
		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm2 );
		option.SetWhere( szWhere );
		option.SetIgnoreAttr( "currency_" );
		//option.SetIgnoreAttr( "type_" );
		option.AddLeftJoin(
			"name", "fsumm_currency_", "ud_currency",
			"`ud_currency`.`currency_id`=`ud_fsumm`.`fsumm_currency`", GetProjectCurrency
		);
		//////////////////////////////////////////////////////////////////////////////////////////
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			err.clear( );
		}
		/*if ( !arr.empty( ) ) {
			udInt type = 0;
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj1 = *i;
				type = obj1->get( "type" )->toInt( );
				obj1->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( type ) );
			}
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////
		char code  = 0;
		udBinaryData data;
		//////////////////////////////////////////////////////////////////////////////////////////
		data.add( ( char ) 1 );
		WriteInstances( arr, data );
		// отправляем суммы
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		// ждем подтверждение приема
		/*code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );*/
		data.clear( );
		ClearArrBase( arr );
		option.Clear( );
		config.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// тур
		/*string log = "\n";
		obj1 = NULL;
		udPBase tour = GetProjectTour( );
		tour->GetConfig( config );
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + obj->get( "tour" )->toString( ) );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			tour = arr[ 0 ];
			arr.clear( );
		}
		tour->dump( data );
		log += "\ntour: ";
		tour->dump( log );
		log += "\n";
		ClearArrBase( arr );
		option.Clear( );
		config.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// заказчик
		if ( tour->get( "emptype" )->toInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
			obj1 = GetProjectClient( );
			obj1->GetConfig( config );
			option.SetTable( "ud_client" );
			option.SetClass( GetProjectClient );
		} else {
			obj1 = GetProjectClientUr( );
			obj1->GetConfig( config );
			option.SetTable( "ud_clientur" );
			option.SetClass( GetProjectClientUr );
		}
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + tour->get( "employer" )->toString( ) );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			arr[ 0 ]->apply( obj1 );
		}
		obj1->dump( data );
		log += "\nemployer: ";
		obj1->dump( log );
		log += "\n";
		ClearArrBase( arr );
		option.Clear( );
		config.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// компания
		obj1 = GetProjectCompany( );
		obj1->GetConfig( config );
		option.SetTable( "ud_company" );
		option.SetClass( GetProjectCompany );
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + tour->get( "company" )->toString( ) );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			arr[ 0 ]->apply( obj1 );
		}
		obj1->dump( data );
		log += "\ncompany: ";
		obj1->dump( log );
		log += "\n";
		ClearArrBase( arr );
		option.Clear( );
		config.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// лицевой счет
		obj1 = GetProjectPersAcc( );
		obj1->GetConfig( config );
		option.SetTable( "ud_persacc" );
		option.SetClass( GetProjectPersAcc );

		if ( tour->get( "emptype" )->toInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
			option.SetWhere(
				config.GetAttrIndex( "type" ) + "=" + toString( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) + " AND " +
				config.GetAttrIndex( "obj1" ) + "=" + tour->get( "employer" )->toString( ) + " AND " +
				config.GetAttrIndex( "obj2" ) + "=" + tour->get( "company" )->toString( )
			);
		} else {
			option.SetWhere(
				config.GetAttrIndex( "type" ) + "=" + toString( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) + " AND " +
				config.GetAttrIndex( "obj1" ) + "=" + tour->get( "employer" )->toString( ) + " AND " +
				config.GetAttrIndex( "obj2" ) + "=" + tour->get( "company" )->toString( )
			);
		}

		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			arr[ 0 ]->apply( obj1 );
		}
		obj1->dump( data );
		log += "\npers_acc: ";
		obj1->dump( log );
		log += "\n";
		ClearArrBase( arr );
		option.Clear( );
		config.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		udDELETE( tour );
		udDELETE( obj1 );
		//////////////////////////////////////////////////////////////////////////////////////////
		// отправляем суммы
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		// ждем подтверждение приема
		code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
		data.clear( );
		ClearArrBase( arr );
		option.Clear( );
		config.Clear( );*/
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	string CHTourPayOrder::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectTourPayOrder;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udUInt id = 0;
		udInt state = 0;
		datetime date;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "tour" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "tour" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "created" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "created" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		state = m_pObjFilter->get( "state" )->toInt( );
		if ( state > 0 ) {
			arrString.push_back( config.GetAttrIndex( "state" ) + "=" + toString( state ) );
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
