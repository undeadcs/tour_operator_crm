
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPersAcc2( ) {
		udPBase ret = GetProjectPersAcc( );
		ret->get( "name" )->ini( wstring( L"" ) );
		/*ret->get( "company" )->ini( wstring( L"" ) );
		ret->get( "partner" )->ini( wstring( L"" ) );
		ret->get( "client" )->ini( wstring( L"" ) );
		ret->get( "clientur" )->ini( wstring( L"" ) );*/
		return ret;
	}

	udPBase GetProjectFinanceSumm2( ) {
		udPBase ret = GetProjectFinanceSumm( );
		ret->get( "currency_" )->ini( wstring( L"" ) );
		//ret->get( "type_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectFinanceSumm2

	udPBase GetProjectFinanceSumm3( ) {
		udPBase ret = GetProjectFinanceSumm2( );
		ret->get( "partner" )->ini( ( udUInt ) 0 );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectFinanceSumm3

	CHPersAcc::CHPersAcc( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_persacc";
		m_szUpdIndex	= "id";
		m_szName		= "persacc";
		m_fnGetObject	= GetProjectPersAcc;

		m_pObjFilter->get( "company" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "partner" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "client" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
	}

	CHPersAcc::CHPersAcc( const CHPersAcc& objCopy ) {
	}

	CHPersAcc::~CHPersAcc( ) {
	}

	CHPersAcc& CHPersAcc::operator =( const CHPersAcc& objCopy ) {
		return *this;
	}

	void CHPersAcc::Init( ) {
		udStateHandlerSimple::Init( );

		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm );
		handler->CheckTable( option );

		option.SetTable( "ud_fop" );
		option.SetClass( GetProjectFinanceOperation );
		handler->CheckTable( option );

		option.SetTable( "ud_fsummtopartner" );
		option.SetClass( GetProjectFinanceSummToPartner );
		handler->CheckTable( option );
	} // void Init

	udInt CHPersAcc::DelObj( udPBase obj, bool save_info ) {
		// ЛС удалять нельзя, они удаляются с родительской сущностью
		return 0;
	} // udInt DelObj

	bool CHPersAcc::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		/**
			джоиним таблицы компаний, клиентов, партнеров
			из них берем имена
		*/
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		option.SetTable( "ud_persacc" );
		option.SetClass( GetProjectPersAcc2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		option.SetWhere( this->GetWhereCondition( ) );
		option.SetSqlCalcFoundRows( true );
		option.SetIgnoreAttr( "name" );
		//////////////////////////////////////////////////////////////////////////////////////////
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		} else { // дополняем названия
			udPDatabase db = g_pApp->GetDatabase( );
			udArrMorph arrx;
			if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arrx ) ) {
				cnt = arrx[ 0 ]->get( "cnt" )->toLLong( );
				ClearArrMorph( arrx );
			}
			/**
				1. собираем ID всех сущностей, попавших в набор.
				2. выгребаем данные сущности.
				3. собираем названия счетов по ID сущностей в каждом ЛС.
				4. очищаем служебные массивы.
			*/
			option.Clear( );
			udPBase obj = NULL;
			udInt type = 0;
			udUInt id1 = 0, id2 = 0;
			map< udUInt, udUInt > idCompany, idPartner, idClient, idClientUr;
			map< udUInt, udPBase > mapCompany, mapPartner, mapClient, mapClientUr;
			udArrBase arr1;
			udArrString arrString;
			//////////////////////////////////////////////////////////////////////////////////////
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				id1 = obj->get( "obj1" )->toUInt( );
				id2 = obj->get( "obj2" )->toUInt( );
				type = obj->get( "type" )->toInt( );
				// определяем тип и записываем ID
				if ( type == udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) {
					idClient[ id1 ] = id1;
					idCompany[ id2 ] = id2;
				} else if ( type == udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) {
					idClientUr[ id1 ] = id1;
					idCompany[ id2 ] = id2;
				} else if ( type == udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) {
					idCompany[ id1 ] = id1;
					idPartner[ id2 ] = id2;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////
			for( map< udUInt, udUInt >::iterator i = idCompany.begin( ); i != idCompany.end( ); ++i ) {
				arrString.push_back( toString( i->first ) );
			}
			option.SetTable( "ud_company" );
			option.SetClass( GetProjectCompany );
			option.SetWhere( "`company_id` IN(" + Join( ",", arrString ) + ")" );
			arrString.clear( );
			handler->GetUdObject( arr1, option );
			if ( !arr1.empty( ) ) { // есть компании
				// сохраняем компании
				for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
					obj = *i;
					id1 = obj->get( "id" )->toUInt( );
					mapCompany[ id1 ] = obj;
				}
				arr1.clear( );
				// партнеры
				for( map< udUInt, udUInt >::iterator i = idPartner.begin( ); i != idPartner.end( ); ++i ) {
					arrString.push_back( toString( i->first ) );
				}
				option.SetTable( "ud_partner" );
				option.SetClass( GetProjectPartner );
				option.SetWhere( "`partner_id` IN(" + Join( ",", arrString)  + ")" );
				arrString.clear( );
				handler->GetUdObject( arr1, option );
				if ( !arr1.empty( ) ) {
					for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
						obj = *i;
						id1 = obj->get( "id" )->toUInt( );
						mapPartner[ id1 ] = obj;
					}
					arr1.clear( );
				}
				// физ.лица
				for( map< udUInt, udUInt >::iterator i = idClient.begin( ); i != idClient.end( ); ++i ) {
					arrString.push_back( toString( i->first ) );
				}
				option.SetTable( "ud_client" );
				option.SetClass( GetProjectClient );
				option.SetWhere( "`client_id` IN(" + Join( ",", arrString ) + ")" );
				arrString.clear( );
				handler->GetUdObject( arr1, option );
				if ( !arr1.empty( ) ) {
					for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
						obj = *i;
						id1 = obj->get( "id" )->toUInt( );
						mapClient[ id1 ] = obj;
					}
					arr1.clear( );
				}
				// юр.лица
				for( map< udUInt, udUInt >::iterator i = idClientUr.begin( ); i != idClientUr.end( ); ++i ) {
					arrString.push_back( toString( i->first ) );
				}
				option.SetTable( "ud_clientur" );
				option.SetClass( GetProjectClientUr );
				option.SetWhere( "`clientur_id` IN(" + Join( ",", arrString ) + ")" );
				arrString.clear( );
				handler->GetUdObject( arr1, option );
				if ( !arr1.empty( ) ) {
					for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
						obj = *i;
						id1 = obj->get( "id" )->toUInt( );
						mapClientUr[ id1 ] = obj;
					}
					arr1.clear( );
				}
				//////////////////////////////////////////////////////////////////////////////////
				// собираем названия
				wstring name, name1, name2;
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					id1 = obj->get( "obj1" )->toUInt( );
					id2 = obj->get( "obj2" )->toUInt( );
					type = obj->get( "type" )->toInt( );
					// определяем тип и записываем ID
					if ( type == udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) {
						name1 = mapClient[ id1 ]->get( "fio" )->toWString( );
						//idClient[ id1 ] = id1;
						name2 = mapCompany[ id2 ]->get( "name" )->toWString( );
						//idCompany[ id2 ] = id2;
					} else if ( type == udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) {
						name1 = mapClientUr[ id1 ]->get( "name" )->toWString( );
						//idClientUr[ id1 ] = id1;
						name2 = mapCompany[ id2 ]->get( "name" )->toWString( );
						//idCompany[ id2 ] = id2;
					} else if ( type == udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) {
						name1 = mapCompany[ id1 ]->get( "name" )->toWString( );
						//idCompany[ id1 ] = id1;
						name2 = mapPartner[ id2 ]->get( "name" )->toWString( );
						//idPartner[ id2 ] = id2;
					}
					name = name1 + L" - " + name2;
					obj->get( "name" )->ini( name );
				}
				//////////////////////////////////////////////////////////////////////////////////
				for( map< udUInt, udPBase >::iterator i = mapCompany.begin( ); i != mapCompany.end( ); ++i ) {
					delete i->second;
				}
				mapCompany.clear( );
				for( map< udUInt, udPBase >::iterator i = mapClient.begin( ); i != mapClient.end( ); ++i ) {
					delete i->second;
				}
				mapClient.clear( );
				for( map< udUInt, udPBase >::iterator i = mapClientUr.begin( ); i != mapClientUr.end( ); ++i ) {
					delete i->second;
				}
				mapClientUr.clear( );
				for( map< udUInt, udPBase >::iterator i = mapPartner.begin( ); i != mapPartner.end( ); ++i ) {
					delete i->second;
				}
				mapPartner.clear( );
				//////////////////////////////////////////////////////////////////////////////////
			}
			//////////////////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool CustomLoadObjects

	string CHPersAcc::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectPersAcc;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "company" )->toUInt( );
		if ( id > 0 ) {
			// клиент-компания или компания-партнер
			arrString.push_back(
				"("
				"(" + config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) + " AND " +
				config.GetAttrIndex( "obj2" ) + "=" + toString( id ) + ") OR (" +
				config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) + " AND " +
				config.GetAttrIndex( "obj2" ) + "=" + toString( id ) + ") OR (" +
				config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) + " AND " +
				config.GetAttrIndex( "obj1" ) + "=" + toString( id ) + ")"
				")"
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "partner" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back(
				"(" + config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) + " AND " +
				config.GetAttrIndex( "obj2" ) + "=" + toString( id ) + ")"
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "client" )->toUInt( );
		if ( id > 0 ) {
			udInt type = m_pObjFilter->get( "emptype" )->toInt( );
			if ( type == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
				arrString.push_back(
					"(" + config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) + " AND " +
					config.GetAttrIndex( "obj1" ) + "=" + toString( id ) + ")"
				);
			} else {
				arrString.push_back(
					"(" + config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) + " AND " +
					config.GetAttrIndex( "obj1" ) + "=" + toString( id ) + ")"
				);
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

	void CHPersAcc::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
	} // void ExtraDataRecieve

	void CHPersAcc::ExtraDataSending( udPSocket sock, udPBase obj ) {
		udPHandler handler = g_pApp->GetHandler( );
		udBinaryData data;
		udInt type = obj->get( "type" )->toInt( );
		udPBase obj1 = NULL, obj2 = NULL;
		udPLog __log = g_pApp->GetLog( );
		string log = "\n";

		if ( type == udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) {
			obj1 = LoadInstanceByForeignKey( handler, obj, GetProjectClient, "ud_client", "obj1", "id" );
			obj2 = LoadInstanceByForeignKey( handler, obj, GetProjectCompany, "ud_company", "obj2", "id" );
		} else if ( type == udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) {
			obj1 = LoadInstanceByForeignKey( handler, obj, GetProjectClientUr, "ud_clientur", "obj1", "id" );
			obj2 = LoadInstanceByForeignKey( handler, obj, GetProjectCompany, "ud_company", "obj2", "id" );
		} else if ( type = udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) {
			obj1 = LoadInstanceByForeignKey( handler, obj, GetProjectCompany, "ud_company", "obj1", "id" );
			obj2 = LoadInstanceByForeignKey( handler, obj, GetProjectPartner, "ud_partner", "obj2", "id" );
		}
		if ( obj1 ) {
			data.add( ( char ) 1 );
			obj1->dump( data );

			log = "\nobject1: ";
			obj1->dump( log );
			__log->Write( log + "\n" );
		} else {
			data.add( ( char ) 0 );

			__log->Write( "\nobject1: not found\n" );
		}
		if ( obj2 ) {
			data.add( ( char ) 1 );
			obj2->dump( data );

			log = "\nobject2: ";
			obj2->dump( log );
			__log->Write( log + "\n" );
		} else {
			data.add( ( char ) 0 );

			__log->Write( "\nobject2: not found\n" );
		}

		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );

		data.clear( );
		udDELETE( obj1 );
		udDELETE( obj2 );

		/**
			отправляем суммы и операции
		*/
		udArrBase arr;
		//char code  = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		data.add( ( char ) 1 );
		this->LoadAccSumm( arr, obj->get( "id" )->toUInt( ) );
		WriteInstances( arr, data );
		// отправляем суммы
		/*sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		// ждем подтверждение приема
		code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
		data.clear( );*/
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		data.add( ( char ) 1 );
		this->LoadAccOp( arr, obj->get( "id" )->toUInt( ) );
		WriteInstances( arr, data );
		// отправляем операции
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		// ждем подтверждение приема
		/*code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
		data.clear( );*/
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataRecieve

	void CHPersAcc::SchemeCenter( udInt opId, udPSocket sock ) {
		if ( opId > opidGet ) {
			g_pApp->GetLog( )->Write( "persacc, получили команду\n" );
			switch( opId ) {
				case opidSummAdd:		this->OpSummAdd( sock );		break;
				case opidSummBack:		this->OpSummBack( sock );		break;
				case opidSummConvert:	this->OpSummConvert( sock );	break;
				default:
					break;
			}
		} else {
			udStateHandlerSimple::SchemeCenter( opId, sock );
		}
	} // void SchemeCenter

	void CHPersAcc::OpSummAdd( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CHPersAcc::OpSummAdd enter\n" );
		/**
			получаем ID личевого счета
			считываем объект суммы
			аккумулируем сумму с текущими суммами на счету
			возвращаем новый список сумм клиенту
		*/
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) == 1 ) { // считали ID лицевого счета
			g_pApp->GetLog( )->Write( "считали ID лицевого счета, ID=" + id.toString( ) + "\n" );
			udPBase obj = GetProjectFinanceSummAdd( );
			if ( obj->read( sock ) == 1 ) {
				//char code = 0;
				string log = "\nсчитали сумму на добавление\n";
				obj->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );
				//sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				//sock->SendByte( ( char ) 80 );
				//////////////////////////////////////////////////////////////////////////////////
				this->AccumulateAdd( id.toUInt( ), obj );
				/**
					отправляем суммы и операции
				*/
				udBinaryData data;
				udArrBase arr;
				//////////////////////////////////////////////////////////////////////////////////
				data.add( ( char ) 1 );
				this->LoadAccSumm( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				ClearArrBase( arr );
				//////////////////////////////////////////////////////////////////////////////////
				data.add( ( char ) 1 );
				this->LoadAccOp( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				ClearArrBase( arr );
				//////////////////////////////////////////////////////////////////////////////////
				/*this->LoadAccSumm( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем подтверждающий байт приема данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				ClearArrBase( arr );
				data.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				this->LoadAccOp( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем подтверждающий байт приема данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				ClearArrBase( arr );
				data.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				ClearArrBase( arr );*/
			}
			udDELETE( obj );
		}
		g_pApp->GetLog( )->Write( "CHPersAcc::OpSummAdd exit\n" );
	} // void OpSummAdd

	void CHPersAcc::AccumulateAdd( udUInt id, udPBase add ) {
		/**
			выгребаем текущие суммы
			среди текущих ищем сумму в данной валюте и такого же типа
			если такая сумма имеется, то плюсуем
			если нет, то добавляем в набор
			при плюсовании создается информация об операции
			при добавлении создается новая сумма в наборе + информация об операции
		*/
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udBaseConfig confSumm;
		udPBase obj = NULL;
		string szWhere = "", log = "";
		// наполняем конфиг
		obj = new udProjectFinanceSumm;
		obj->GetConfig( confSumm );
		udDELETE( obj );
		// наполняем условие выборки
		szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
			confSumm.GetAttrIndex( "obj" ) + "=" + toString( id );
		confSumm.Clear( );
		g_pApp->GetLog( )->Write( "where=\"" + szWhere + "\"\n" );
		//
		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		option.SetWhere( "" );
		if ( err.empty( ) ) {
			if ( arr.empty( ) ) { // нет сумм
				g_pApp->GetLog( )->Write( "суммы не найдены\n" );
				obj = new udProjectFinanceSumm;
				obj->get( "summ" )->ini( add->get( "summ" )->toFloat( ) );
				obj->get( "currency" )->ini( add->get( "currency" )->toUInt( ) );
				obj->get( "type" )->ini( add->get( "type" )->toInt( ) );
				obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
				obj->get( "obj" )->ini( id );
				handler->AddUdObject( obj, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
					err.clear( );
				}
				udDELETE( obj );
			} else { // есть суммы, ищем похожую
				g_pApp->GetLog( )->Write( "суммы найдены\n" );
				bool found = false;
				udUInt c1 = 0, c2 = add->get( "currency" )->toUInt( );
				udInt t1 = 0, t2 = add->get( "type" )->toInt( );
				udFloat s1 = 0.0f, s2 = add->get( "summ" )->toFloat( );
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					c1 = obj->get( "currency" )->toUInt( );
					t1 = obj->get( "type" )->toInt( );
					if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
						s1 = obj->get( "summ" )->toFloat( );
						obj->get( "summ" )->ini( s1 + s2 );
						found = true;
						break;
					}
				}
				obj = NULL;
				if ( found ) { // если найдена, то обновляем
					option.SetIndexAttr( "id" );
					handler->UpdUdObject( arr, option );
					option.SetIndexAttr( "" );
				} else { // если не найдена, то добавляем
					obj = new udProjectFinanceSumm;
					obj->get( "summ" )->ini( add->get( "summ" )->toFloat( ) );
					obj->get( "currency" )->ini( add->get( "currency" )->toUInt( ) );
					obj->get( "type" )->ini( add->get( "type" )->toInt( ) );
					obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
					obj->get( "obj" )->ini( id );
					handler->AddUdObject( obj, option );
					udDELETE( obj );
					// подгружаем снова
					/*ClearArrBase( arr );
					option.SetWhere( szWhere );
					handler->GetUdObject( arr, option );*/
				}
				log = "\nсуммы на счету, ID=" + toString( id ) + "\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				g_pApp->GetLog( )->Write( log + "\n" );
			}
			// сохраняем информацию об операции (операция)
			obj = new udProjectFinanceOperation;
			datetime date;
			date.load( );
			obj->get( "created" )->ini( date );
			obj->get( "acc" )->ini( id );
			obj->get( "type" )->ini( ( udInt ) udProjectFinanceOperation::fotRoll );
			obj->get( "money" )->ini( add->get( "how" )->toInt( ) );
			obj->get( "comment" )->ini( add->get( "comment" )->toWString( ) );
			option.Clear( );
			option.SetTable( "ud_fop" );
			handler->AddUdObject( obj, option );
			udDELETE( obj );
			// сохраняем информацию об операции (сумма)
			obj = new udProjectFinanceSumm;
			obj->get( "summ" )->ini( add->get( "summ" )->toFloat( ) );
			obj->get( "currency" )->ini( add->get( "currency" )->toUInt( ) );
			obj->get( "type" )->ini( add->get( "type" )->toInt( ) );
			obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
			obj->get( "obj" )->ini( ( udUInt )( ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( ) );
			handler->AddUdObject( obj, option );
			udDELETE( obj );
			// подгружаем итоговые данные
			ClearArrBase( arr );
			option.Clear( );
		} else {
			g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
			WriteErrors( g_pApp->GetLog( ), err );
		}
	} // void AccumulateAdd

	void CHPersAcc::OpSummBack( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CHPersAcc::OpSummBack enter\n" );
		/**
			получаем ID личевого счета
			считываем объект суммы
			аккумулируем сумму с текущими суммами на счету
			возвращаем новый список сумм клиенту
		*/
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) == 1 ) { // считали ID лицевого счета
			g_pApp->GetLog( )->Write( "считали ID лицевого счета, ID=" + id.toString( ) + "\n" );
			udPBase obj = GetProjectFinanceSummBack( );
			if ( obj->read( sock ) == 1 ) {
				char code = 0;
				string log = "\nсчитали сумму на снятие\n";
				obj->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );
				//sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				//sock->SendByte( ( char ) 80 );
				//udArrBase arr;
				char ret = this->AccumulateBack( id.toUInt( ), obj );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ret );
				//udBinaryData data;
				//////////////////////////////////////////////////////////////////////////////////
				/**
					отправляем суммы и операции
				*/
				udBinaryData data;
				udArrBase arr;
				//////////////////////////////////////////////////////////////////////////////////
				data.add( ( char ) 1 );
				this->LoadAccSumm( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				ClearArrBase( arr );
				//////////////////////////////////////////////////////////////////////////////////
				data.add( ( char ) 1 );
				this->LoadAccOp( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				ClearArrBase( arr );
				/*this->LoadAccSumm( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем подтверждающий байт приема данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				ClearArrBase( arr );
				data.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				this->LoadAccOp( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем подтверждающий байт приема данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				ClearArrBase( arr );
				data.clear( );*/
				//////////////////////////////////////////////////////////////////////////////////
			}
			udDELETE( obj );
		}
		g_pApp->GetLog( )->Write( "CHPersAcc::OpSummBack exit\n" );
	} // void OpSummBack

	char CHPersAcc::AccumulateBack( udUInt id, udPBase back ) {
		/**
			выгребаем текущие суммы
			среди текущих ищем сумму в данной валюте и такого же типа
			если такая сумма имеется, то минусуем
			при минусовании создается информация об операции
		*/
		char ret = 80;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udBaseConfig confSumm;
		udPBase obj = NULL;
		string szWhere = "", log = "";
		// наполняем конфиг
		obj = new udProjectFinanceSumm;
		obj->GetConfig( confSumm );
		udDELETE( obj );
		// наполняем условие выборки
		szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
			confSumm.GetAttrIndex( "obj" ) + "=" + toString( id );
		confSumm.Clear( );
		g_pApp->GetLog( )->Write( "where=\"" + szWhere + "\"\n" );
		//
		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		option.SetWhere( "" );
		if ( err.empty( ) ) {
			if ( arr.empty( ) ) { // нет сумм
				ret = 0;
			} else { // есть суммы, ищем похожую
				g_pApp->GetLog( )->Write( "суммы найдены\n" );
				udPBase pFound = NULL;
				udUInt c1 = 0, c2 = back->get( "currency" )->toUInt( );
				udInt t1 = 0, t2 = back->get( "type" )->toInt( );
				udFloat s1 = 0.0f, s2 = back->get( "summ" )->toFloat( );
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					c1 = obj->get( "currency" )->toUInt( );
					t1 = obj->get( "type" )->toInt( );
					if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
						//s1 = obj->get( "summ" )->toFloat( );
						//obj->get( "summ" )->ini( s1 - s2 );
						//found = true;
						pFound = obj;
						break;
					}
				}
				obj = NULL;
				if ( pFound ) { // если найдена, то обновляем
					s1 = pFound->get( "summ" )->toFloat( );
					if ( s1 < s2 ) { // не достаточно средств для снятия
						ret = 0;
					} else {
						pFound->get( "summ" )->ini( s1 - s2 );
						option.SetIndexAttr( "id" );
						handler->UpdUdObject( arr, option );
						option.SetIndexAttr( "" );
					}
				} else { // сумма не найдена, снять нельзя
					ret = 0;
				}
				log = "\nсуммы на счету, ID=" + toString( id ) + "\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				g_pApp->GetLog( )->Write( log + "\n" );
			}
			if ( ret == 80 ) { // если все нормально
				// сохраняем информацию об операции (операция)
				obj = new udProjectFinanceOperation;
				datetime date;
				date.load( );
				obj->get( "created" )->ini( date );
				obj->get( "acc" )->ini( id );
				obj->get( "type" )->ini( ( udInt ) udProjectFinanceOperation::fotCut );
				obj->get( "money" )->ini( back->get( "how" )->toInt( ) );
				obj->get( "comment" )->ini( back->get( "comment" )->toWString( ) );
				option.Clear( );
				option.SetTable( "ud_fop" );
				handler->AddUdObject( obj, option );
				udDELETE( obj );
				// сохраняем информацию об операции (сумма)
				obj = new udProjectFinanceSumm;
				obj->get( "summ" )->ini( back->get( "summ" )->toFloat( ) );
				obj->get( "currency" )->ini( back->get( "currency" )->toUInt( ) );
				obj->get( "type" )->ini( back->get( "type" )->toInt( ) );
				obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
				obj->get( "obj" )->ini( ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( ) );
				handler->AddUdObject( obj, option );
				udDELETE( obj );
			}
			// подгружаем итоговые данные
			ClearArrBase( arr );
			option.Clear( );
		} else {
			g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return ret;
	} // char AccumulateBack

	void CHPersAcc::OpSummConvert( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CHPersAcc::OpSummConvert enter\n" );
		/**
			получаем ID личевого счета
			считываем объект суммы
			аккумулируем сумму с текущими суммами на счету
			возвращаем новый список сумм клиенту
		*/
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) == 1 ) { // считали ID лицевого счета
			g_pApp->GetLog( )->Write( "считали ID лицевого счета, ID=" + id.toString( ) + "\n" );
			udPBase obj = GetProjectFinanceSummConvert( );
			if ( obj->read( sock ) == 1 ) {
				char code = 0;
				string log = "\nсчитали сумму на конвертацию\n";
				obj->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );
				/*sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );*/
				//udArrBase arr;
				char ret = this->AccumulateConvert( id.toUInt( ), obj );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ret );
				//udBinaryData data;
				//////////////////////////////////////////////////////////////////////////////////
				/**
					отправляем суммы и операции
				*/
				udBinaryData data;
				udArrBase arr;
				//////////////////////////////////////////////////////////////////////////////////
				data.add( ( char ) 1 );
				this->LoadAccSumm( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				ClearArrBase( arr );
				//////////////////////////////////////////////////////////////////////////////////
				data.add( ( char ) 1 );
				this->LoadAccOp( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				ClearArrBase( arr );
				/*this->LoadAccSumm( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем подтверждающий байт приема данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				ClearArrBase( arr );
				data.clear( );
				//////////////////////////////////////////////////////////////////////////////////
				this->LoadAccOp( arr, id.toUInt( ) );
				WriteInstances( arr, data );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем подтверждающий байт приема данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				ClearArrBase( arr );
				data.clear( );*/
				//////////////////////////////////////////////////////////////////////////////////
			}
			udDELETE( obj );
		}
		g_pApp->GetLog( )->Write( "CHPersAcc::OpSummConvert exit\n" );
	} // void OpSummConvert

	char CHPersAcc::AccumulateConvert( udUInt id, udPBase conv ) {
		char ret = 80;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udBaseConfig confSumm;
		udPBase obj = NULL;
		string szWhere = "", log = "";
		// наполняем конфиг
		obj = new udProjectFinanceSumm;
		obj->GetConfig( confSumm );
		udDELETE( obj );
		// наполняем условие выборки
		szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
			confSumm.GetAttrIndex( "obj" ) + "=" + toString( id );
		confSumm.Clear( );
		g_pApp->GetLog( )->Write( "where=\"" + szWhere + "\"\n" );
		//
		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		option.SetWhere( "" );
		if ( err.empty( ) ) {
			udFloat fConvResult = 0.0f; // сумма результат конвертации
			// ID валют
			udUInt	c	= 0,
					c1	= conv->get( "currency1" )->toUInt( ),
					c2	= conv->get( "currency2" )->toUInt( );
			if ( c2 == 0 ) {
				c2 = c1;
			}
			// типы
			udInt	t	= 0,
					t1	= conv->get( "type1" )->toInt( ),
					t2	= conv->get( "type2" )->toInt( );
			if ( t2 == 0 ) {
				t2 = t1;
			}
			if ( arr.empty( ) ) { // нет сумм
				ret = 0;
			} else { // есть суммы, ищем похожую
				g_pApp->GetLog( )->Write( "суммы найдены\n" );
				udPBase pFrom = NULL, pTo = NULL;
				// ищем сумму из
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					c = obj->get( "currency" )->toUInt( );
					t = obj->get( "type" )->toInt( );
					if ( ( c == c1 ) && ( t == t1 ) ) {
						pFrom = obj;
						break;
					}
				}
				// исходная сумма обязательна при любых раскладах
				if ( pFrom ) { // если найдена сумма источник
					// ищем сумму в
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj = *i;
						c = obj->get( "currency" )->toUInt( );
						t = obj->get( "type" )->toInt( );
						if ( ( c == c2 ) && ( t == t2 ) ) {
							pTo = obj;
							break;
						}
					}
					/**
						1. обязательно должна быть сумма "из".
						2. сумма "из" должна быть больше, чем требуется для конвертации.
						3. если не найдена сумма "в", то создаем новую.
						4. если найдена сумма "в", то плюсуем.
						5. ищем валютную пару для двух валют.
					*/
					obj = NULL;
					udFloat s1 = pFrom->get( "summ" )->toFloat( ), s2 = conv->get( "summ" )->toFloat( );
					if ( s1 < s2 ) { // не достаточно средств для конвертации
						ret = 0;
					} else {
						if ( c1 == c2 ) { // при равенстве валют размен 1 к 1
							udFloat fNew = s2;
							fConvResult = fNew;
							// минусуем сумму источник
							pFrom->get( "summ" )->ini( s1 - s2 );
							if ( pTo ) { // плюсуем сумму результат
								fNew += pTo->get( "summ" )->toFloat( );
								pTo->get( "summ" )->ini( fNew );
							} else { // создаем новую сумму
								obj = new udProjectFinanceSumm;
								obj->get( "summ" )->ini( fNew );
								obj->get( "currency" )->ini( c2 );
								obj->get( "type" )->ini( t2 );
								obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
								obj->get( "obj" )->ini( id );
								handler->AddUdObject( obj, option );
								udDELETE( obj );
							}
							// обновляем суммы
							option.SetIndexAttr( "id" );
							handler->UpdUdObject( arr, option );
							option.SetIndexAttr( "" );
						} else {
							// ищем валютную пару для конвертации
							udPBase pPair = this->FindCurrencyPair( c1, c2 );
							if ( pPair ) { // валютная пара есть
								log = "\nнайденная валютная пара\n";
								pPair->dump( log );
								g_pApp->GetLog( )->Write( log + "\n" );
								udFloat base = pPair->get( "internal" )->toFloat( );
								if ( base == 0.0f ) {
									base = pPair->get( "course" )->toFloat( );
								}
								if ( base > 0.0f ) {
									udFloat fNew = s2 / base;
									fConvResult = fNew;
									// минусуем сумму источник
									pFrom->get( "summ" )->ini( s1 - s2 );
									if ( pTo ) { // плюсуем сумму результат
										fNew += pTo->get( "summ" )->toFloat( );
										pTo->get( "summ" )->ini( fNew );
									} else { // создаем новую сумму
										obj = new udProjectFinanceSumm;
										obj->get( "summ" )->ini( fNew );
										obj->get( "currency" )->ini( c2 );
										obj->get( "type" )->ini( t2 );
										obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
										obj->get( "obj" )->ini( id );
										handler->AddUdObject( obj, option );
										udDELETE( obj );
									}
									// обновляем суммы
									option.SetIndexAttr( "id" );
									handler->UpdUdObject( arr, option );
									option.SetIndexAttr( "" );
								} else { // не установлен курс
									ret = 2;
								}
								udDELETE( pPair );
							} else { // отсутствует валютная пара для конвертации
								ret = 1;
							}
						}
					}
					log = "\nсуммы на счету, ID=" + toString( id ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
				} else { // нет суммы - нет средств
					ret = 0;
				}
			}
			if ( ret == 80 ) {
				// сохраняем информацию об операции (операция)
				obj = new udProjectFinanceOperation;
				datetime date;
				date.load( );
				obj->get( "created" )->ini( date );
				obj->get( "acc" )->ini( id );
				obj->get( "type" )->ini( ( udInt ) udProjectFinanceOperation::fotConv );
				obj->get( "comment" )->ini( conv->get( "comment" )->toWString( ) );
				option.Clear( );
				option.SetTable( "ud_fop" );
				handler->AddUdObject( obj, option );
				udDELETE( obj );
				// id операции
				udUInt iOperationId = ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( );
				// сохраняем информацию об операции (сумма из)
				obj = new udProjectFinanceSumm;
				obj->get( "summ" )->ini( conv->get( "summ" )->toFloat( ) );
				obj->get( "currency" )->ini( c1 );
				obj->get( "type" )->ini( t1 );
				obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
				obj->get( "obj" )->ini( iOperationId );
				handler->AddUdObject( obj, option );
				udDELETE( obj );
				// сохраняем информацию об операции (сумма в)
				obj = new udProjectFinanceSumm;
				obj->get( "summ" )->ini( fConvResult );
				obj->get( "currency" )->ini( c2 );
				obj->get( "type" )->ini( t2 );
				obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
				obj->get( "obj" )->ini( iOperationId );
				handler->AddUdObject( obj, option );
				udDELETE( obj );
			}
			ClearArrBase( arr );
			option.Clear( );
		} else {
			g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return ret;
	} // void AccumulateConvert

	udPBase CHPersAcc::FindCurrencyPair( udUInt c1, udUInt c2 ) {
		udPBase ret = NULL;
		udPBase obj = NULL;
		udArrBase arr;
		udHandlerOption option;
		udBaseConfig config;
		string szWhere = "";
		obj = new udProjectCurrencyPair;
		obj->GetConfig( config );
		udDELETE( obj );
		szWhere = config.GetAttrIndex( "currency1" ) + "=" + toString( c1 ) + " AND " +
			config.GetAttrIndex( "currency2" ) + "=" + toString( c2 );
		config.Clear( );
		option.SetTable( "ud_currencypair" );
		option.SetClass( GetProjectCurrencyPair );
		g_pApp->GetHandler( )->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			ret = arr[ 0 ];
			arr.clear( );
		}
		return ret;
	} // udPBase FindCurrencyPair

	void CHPersAcc::LoadAccSumm( udArrBase& arr, udUInt iAccId ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udPBase obj = NULL;
		udBaseConfig config;
		string szWhere = "";
		//////////////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectFinanceSumm;
		obj->GetConfig( config );
		udDELETE( obj );
		szWhere = config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
			config.GetAttrIndex( "obj" ) + "=" + toString( iAccId );
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
				obj = *i;
				type = obj->get( "type" )->toInt( );
				obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( type ) );
			}
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadAccSumm

	void CHPersAcc::LoadAccOp( udArrBase& arr, udUInt iAccId ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrError err;
		udPBase obj = NULL;
		string szWhere = "";
		//////////////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectFinanceOperation;
		obj->GetConfig( config );
		udDELETE( obj );
		szWhere = config.GetAttrIndex( "acc" ) + "=" + toString( iAccId );
		config.Clear( );
		option.SetTable( "ud_fop" );
		option.SetClass( GetProjectFinanceOperation );
		option.SetWhere( szWhere );
		//////////////////////////////////////////////////////////////////////////////////////////
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			err.clear( );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadOp

} // namespace UdSDK
