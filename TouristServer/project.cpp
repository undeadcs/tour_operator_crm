
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void LoadInstancesFromSocket( udPSocket sock, const string& table_name, pfnGetObject fnGetObject, udArrBase& arr ) {
		udBinaryData	data;						// данные для чтения
		udArrBase		arrClientObj;				// считанные объекты
		udPBase			obj		= NULL;				// объект для чтения
		udType			objNum( ( udUInt ) 0 );		// число объектов для чтения
		udUInt			cnt = 0,					// подсчет считанных объектов
						num = 0;					// заявленное количество

		//////////////////////////////////////////////////////////////////////////////////////
		// страны
		// запрашиваем у клиента
		data.add( ( char ) 1 );
		data.add( table_name );
		sock->Send( data );
		data.clear( );
		// считываем ответ
		objNum.read( sock );
		cnt = 0;
		num = objNum.toUInt( );
		// считываем объекты
		while( cnt < num ) {
			obj = fnGetObject( );
			if ( obj->read( sock ) == 0 ) {
				delete obj;
				break;
			}
			arr.push_back( obj );
			++cnt;
		}
	} // void LoadInstancesFromSocket




	CAppInitProject::CAppInitProject( ) {
		this->InitTablesMap( );

		m_dwTimeout			= 36000;
		m_szCurrentSession	= "";
	}

	CAppInitProject::CAppInitProject( const CAppInitProject& objCopy ) {
	}

	CAppInitProject::~CAppInitProject( ) {
		m_arrTables.clear( );
	}

	CAppInitProject& CAppInitProject::operator =( const CAppInitProject& objCopy ) {
		return *this;
	}

	void CAppInitProject::LoadFonts( ) {
	} // void LoadFonts

	void CAppInitProject::LoadImages( ) {
	} // void LoadImages

	udInt CAppInitProject::Login( udPSocket sock ) {
		udType id( string( "" ) );
		if ( id.read( sock ) == 1 ) { // считали ID сессии
			g_pApp->GetLog( )->Write( "session ID='" + id.toString( ) + "'\n" );
			_asc_session::iterator itSession = m_ascSession.find( id.toString( ) );
			if ( itSession == m_ascSession.end( ) ) { // сессии не существует
				g_pApp->GetLog( )->Write( "сессия не найдена\n" );
				// в таком случае следом должны идти логин и пароль
				udType tmp( wstring( L"" ) );
				wstring login, password;
				if ( tmp.read( sock ) != 1 ) {
					return 0;
				}
				login = tmp.toWString( );
				if ( tmp.read( sock ) != 1 ) {
					return 0;
				}
				password = tmp.toWString( );
				//g_pApp->GetLog( )->Write( "login='" + toString( login ) + "', password='" + toString( password ) + "'\n" );
				// ищем пользователя с таким логином
				udPBase obj = new udProjectUser;
				udBaseConfig config;
				obj->GetConfig( config );
				obj->get( "login" )->ini( login );
				udPHandler handler = g_pApp->GetHandler( );
				udHandlerOption option;
				udArrBase arr;
				udArrError err;
				option.SetTable( "ud_user" );
				option.SetClass( GetProjectUser );
				option.SetWhere( config.GetAttrIndex( "login" ) + "=" + obj->GetAttrValue( "login", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) );
				handler->GetUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
				}
				if ( !arr.empty( ) ) {
					udPBase client = arr[ 0 ]; // пользователь

					string log = "\nнайден пользователь\n";
					client->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );

					if ( password == client->get( "password" )->toWString( ) ) {
						datetime date;
						date.load( );
						SessionData session;
						session.created	= datetime( date.get( ) );
						session.ip		= sock->GetInfo( )->GetIp( );
						session.user	= client;
						if ( m_ascSession.empty( ) ) {
							session.id	= "1";
						} else {
							itSession = --( m_ascSession.end( ) );
							udUInt id = toUInt( itSession->first );
							++id;
							session.id	= toString( id );
						}
						m_ascSession[ session.id ] = session;
						// отвечаем клиенту
						sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
						sock->SendByte( ( char ) 80 );
						udBinaryData data;
						data.add( session.id );
						client->dump( data );
						sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
						sock->Send( data );
						data.clear( );
						char code = 0;
						sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
						sock->ReadByte( &code );
					} else {
						g_pApp->GetLog( )->Write( "неверный пароль\n" );
						ClearArrBase( arr );
						sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
						sock->SendByte( ( char ) 0 );
					}
				} else {
					g_pApp->GetLog( )->Write( "пользователь не найден\n" );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 0 );
				}
			} else { // сессия существует
				g_pApp->GetLog( )->Write( "сессия найдена\n" );
				m_szCurrentSession = id.toString( ); // устанавливаем текущую сессию
				// необходимо проверить таймаут, если он не вышел, то обновить дату
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				return 1;
			}
		}
		return 0;
	} // udInt Login

	SessionData CAppInitProject::GetCurrentSessionData( ) const {
		SessionData ret;
		if ( !m_szCurrentSession.empty( ) ) {
			_asc_session::const_iterator i = m_ascSession.find( m_szCurrentSession );
			if ( i != m_ascSession.end( ) ) {
				ret = i->second;
			}
		}
		return ret;
	} // SessionData GetCurrentSessionData

	static void MakeAccounts( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arrCompany;
		// выгребаем компании
		option.SetTable( "ud_company" );
		option.SetClass( GetProjectCompany );
		handler->GetUdObject( arrCompany, option );
		if ( !arrCompany.empty( ) ) {
			udArrBase arr, arr1;
			udPBase obj = NULL;
			unsigned count = 0;
			// компания-партнер
			option.SetTable( "ud_partner" );
			option.SetClass( GetProjectPartner );
			handler->GetUdObject( arr, option );
			option.Clear( );
			for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
				for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
					obj = new udProjectPersAcc;
					obj->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_COMPANY_PARTNER );
					// компания
					obj->get( "obj1" )->ini( ( *i )->get( "id" )->toUInt( ) );
					// партнер
					obj->get( "obj2" )->ini( ( *j )->get( "id" )->toUInt( ) );
					arr1.push_back( obj );
				}
			}
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			handler->AddUdObject( arr1, option );
			option.Clear( );
			ClearArrBase( arr );
			ClearArrBase( arr1 );
			// физ.лицо-компания
			option.SetTable( "ud_client" );
			option.SetClass( GetProjectClient );
			handler->GetUdObject( arr, option );
			option.Clear( );
			for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
				for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
					obj = new udProjectPersAcc;
					obj->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENT_COMPANY );
					// клиент
					obj->get( "obj1" )->ini( ( *j )->get( "id" )->toUInt( ) );
					// компания
					obj->get( "obj2" )->ini( ( *i )->get( "id" )->toUInt( ) );
					arr1.push_back( obj );
				}
			}
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			handler->AddUdObject( arr1, option );
			option.Clear( );
			ClearArrBase( arr );
			ClearArrBase( arr1 );
			// юр.лицо-компания
			option.SetTable( "ud_clientur" );
			option.SetClass( GetProjectClientUr );
			handler->GetUdObject( arr, option );
			option.Clear( );
			for( udArrBase::iterator i = arrCompany.begin( ); i != arrCompany.end( ); ++i ) {
				for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
					obj = new udProjectPersAcc;
					obj->get( "type" )->ini( ( udInt ) udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY );
					// клиент
					obj->get( "obj1" )->ini( ( *j )->get( "id" )->toUInt( ) );
					// компания
					obj->get( "obj2" )->ini( ( *i )->get( "id" )->toUInt( ) );
					arr1.push_back( obj );
				}
			}
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			handler->AddUdObject( arr1, option );
			option.Clear( );
			ClearArrBase( arr );
			ClearArrBase( arr1 );
			ClearArrBase( arrCompany );
			// наполняем счета
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			handler->GetUdObject( arr, option );
			if ( !arr.empty( ) ) {
				udPBase acc = NULL;
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					acc = *i;
					obj = new udProjectFinanceSumm;
					obj->get( "summ" )->ini( ( udFloat ) 100 );
					obj->get( "currency" )->ini( ( udUInt ) 1 );
					obj->get( "type" )->ini( ( udInt ) udProjectFinanceSumm::SummWhite );
					obj->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
					obj->get( "obj" )->ini( acc->get( "id" )->toUInt( ) );
					arr1.push_back( obj );
				}
				option.Clear( );
				option.SetTable( "ud_fsumm" );
				handler->AddUdObject( arr1, option );
			}
			ClearArrBase( arr );
			ClearArrBase( arr1 );
		}
	} // void MakeAccounts

	static void GeneratePriceTable( udUInt iServiceId, udUInt iPeriodIndex ) {
		// необходимо сгенерировать ряд чисел, представляющих количество людей
		// затем для каждого количества генерируем случайную сумму и валюту
		// 1. случайное количество чисел в ряду
		// 2. случайное значение для каждого количества человек
		//////////////////////////////////////////////////////////////////////////////////////////////
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udArrBase arr;
		udPBase obj = NULL;
		string log = "";
		int n = 0, m = 0; // количество чисел, количество человек
		udUInt c = 0;
		udFloat summ = 0.0f;
		//////////////////////////////////////////////////////////////////////////////////////////////
		//__log->Write( "GeneratePriceTable, iServiceId=" + toString( iServiceId ) + ", iPeriodIndex=" + toString( iPeriodIndex ) + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////////
		n = ( rand( ) % 10 ) + 2;
		for( int i = 0; i < n; ++i ) {
			obj = new udProjectPriceTableItem;
			obj->get( "service" )->ini( iServiceId );
			obj->get( "period" )->ini( iPeriodIndex );
			m = ( rand( ) % 19 ) + 2;
			obj->get( "num" )->ini( m );
			summ = ( ( float ) rand( ) ) / 100.0f;
			obj->get( "amount" )->ini( summ );
			c = rand( ) % 10 + 1;
			obj->get( "currency" )->ini( c );
			obj->get( "amount_recommend" )->ini( summ );
			obj->get( "currency_recommend" )->ini( c );
			arr.push_back( obj );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		option.SetTable( "ud_pricetableitem" );
		handler->AddUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			__log->Write( "GeneratePriceTable, ошибки при добавлении цен\n" );
			WriteErrors( __log, err );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////////
	} // void GeneratePriceTable

	static void CalculateCost( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		//
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				udHTour::Cost( *i );
			}
		}
		ClearArrBase( arr );
	} // void CalculateCost

	static void CalculateFinal( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		//
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			udArrBase arrResult;

			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				udHTour::CalculateFinal( arrResult, *i );

				if ( !arrResult.empty( ) ) {
					// сносим старые
					( ( udPMySql ) g_pApp->GetDatabase( ) )->Query(
						"DELETE FROM `ud_fsumm` WHERE "
						"`fsumm_at`=" + toString( udProjectFinanceSumm::SummAtTourPrice ) + " AND "
						"`fsumm_obj`=" + ( *i )->get( "id" )->toString( )
					);
					// добавляем новые
					option.SetTable( "ud_fsumm" );
					handler->AddUdObject( arrResult, option );
					ClearArrBase( arrResult );
				}
			}
		}
		ClearArrBase( arr );
	} // void CalculateFinal

	void CAppInitProject::FillTestData( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udPBase obj = NULL;
		wstring name = L"";
		int cnt = 11;
		srand( timeGetTime( ) );
		// направление
		for( int i = 1; i < cnt; ++i ) {
			name = L"направление " + toWString( i );
			obj = new udProjectDirect;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_direct" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// тип обращения
		for( int i = 1; i < cnt; ++i ) {
			name = L"тип обращения " + toWString( i );
			obj = new udProjectTRequest;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_trequest" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// тип услуги
		int take_packet_or_landing = 0;
		bool bWasHotel = false;
		for( int i = 1; i < cnt; ++i ) {
			name = L"тип услуги " + toWString( i );
			take_packet_or_landing = rand( ) % 8;
			if ( take_packet_or_landing == 1 ) {
				name += L" Пакет";
			} else if ( take_packet_or_landing == 2 ) {
				name += L" Посадка";
			} else if ( take_packet_or_landing == 3 ) {
				name += L" Гостиница";
				bWasHotel = true;
			}

			if ( !bWasHotel ) {
				name = L" Гостиница";
				bWasHotel = true;
			}

			obj = new udProjectTService;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_tservice" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// название услуги
		for( int i = 1; i < cnt; ++i ) {
			name = L"услуга " + toWString( i );
			obj = new udProjectNameService;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_nameservice" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// единица изм услуги
		for( int i = 1; i < cnt; ++i ) {
			name = L"ед. изм. услуги " + toWString( i );
			obj = new udProjectMUService;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_muservice" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// страна
		for( int i = 1; i < cnt; ++i ) {
			name = L"страна " + toWString( i );
			obj = new udProjectCountry;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_country" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// результат обращения
		for( int i = 1; i < cnt; ++i ) {
			name = L"рез. обращения " + toWString( i );
			obj = new udProjectRRequest;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_rrequest" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// состояние тура
		for( int i = 1; i < cnt; ++i ) {
			name = L"сост. тура " + toWString( i );
			obj = new udProjectSTour;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_stour" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// состояние заявки
		for( int i = 1; i < cnt; ++i ) {
			name = L"сост. заявки " + toWString( i );
			obj = new udProjectSOrder;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_sorder" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// тип документа
		for( int i = 1; i < cnt; ++i ) {
			name = L"тип документа " + toWString( i );
			obj = new udProjectDocType;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_doctype" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// состояние обращения
		for( int i = 1; i < cnt; ++i ) {
			name = L"сост. обращения " + toWString( i );
			obj = new udProjectSRequest;
			obj->get( "name" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_srequest" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// партнер
		wstring val;
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectPartner;
			name = L"партнер " + val;
			obj->get( "name" )->ini( name );
			name = L"руководитель " + val;
			obj->get( "dfio" )->ini( name );
			name = L"контактное лицо " + val;
			obj->get( "cfio" )->ini( name );
			name = L"email" + val + L"@ru.ru";
			obj->get( "email" )->ini( name );
			name = L"phone " + val;
			obj->get( "phone" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_partner" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// компания
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectCompany;
			name = L"компания " + val;
			obj->get( "name" )->ini( name );
			name = L"email" + val + L"@ru.ru";
			obj->get( "email" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_company" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// клиен юр. лицо
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectClientUr;
			name = L"клиент юр. лицо " + val;
			obj->get( "name" )->ini( name );
			name = L"руководитель " + val;
			obj->get( "dfio" )->ini( name );
			name = L"контактное лицо " + val;
			obj->get( "cfio" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_clientur" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// валюта
		obj = new udProjectCurrency;
		name = L"Рубли";
		obj->get( "name" )->ini( name );
		name = L"RUR";
		obj->get( "code" )->ini( name );
		arr.push_back( obj );

		obj = new udProjectCurrency;
		name = L"Доллары США";
		obj->get( "name" )->ini( name );
		name = L"USD";
		obj->get( "code" )->ini( name );
		arr.push_back( obj );

		obj = new udProjectCurrency;
		name = L"Юани";
		obj->get( "name" )->ini( name );
		name = L"CNY";
		obj->get( "code" )->ini( name );
		arr.push_back( obj );

		for( int i = 3; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectCurrency;
			name = L"валюта " + val;
			obj->get( "name" )->ini( name );
			name = L"кд" + val;
			obj->get( "code" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_currency" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// пользователь
		obj = new udProjectUser;
		obj->get( "login" )->ini( wstring( L"1" ) );
		obj->get( "password" )->ini( wstring( L"1" ) );
		obj->get( "email" )->ini( wstring( L"1@1.1" ) );
		obj->get( "fio" )->ini( wstring( L"1" ) );
		obj->get( "rank" )->ini( ( udInt ) udProjectUser::USERRANK_ADMINISTRATOR );
		obj->get( "series" )->ini( wstring( L"МА" ) );
		arr.push_back( obj );
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectUser;
			name = L"user" + val;
			obj->get( "login" )->ini( name );
			name = val;
			obj->get( "password" )->ini( name );
			name = L"email" + val + L"@ru.ru";
			obj->get( "email" )->ini( name );
			name = L"ФИО " + val;
			obj->get( "fio" )->ini( name );
			obj->get( "rank" )->ini( ( udInt ) udProjectUser::USERRANK_MANAGER );
			obj->get( "series" )->ini( val + val );
			arr.push_back( obj );
		}
		option.SetTable( "ud_user" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// город
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectCity;
			name = L"город " + val;
			obj->get( "name" )->ini( name );
			obj->get( "country" )->ini( ( udUInt ) i );
			arr.push_back( obj );
		}
		option.SetTable( "ud_city" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// валютные пары
		datetime date;
		date.load( );
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectCurrencyPair;
			obj->get( "currency1" )->ini( i );
			if ( i == 10 ) {
				obj->get( "currency2" )->ini( i );
			} else {
				obj->get( "currency2" )->ini( i + 1 );
			}
			obj->get( "course" )->ini( toFloat( i ) );
			obj->get( "set" )->ini( date );
			obj->get( "internal" )->ini( toFloat( i + 1 ) );
			arr.push_back( obj );
		}
		option.SetTable( "ud_currencypair" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// услуги
		cnt = 201;
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectService;
			obj->get( "name" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "type" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "country" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "city" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "partner" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "munit" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			arr.push_back( obj );
		}
		option.SetTable( "ud_service" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// периоды услуги
		char tmp[ 1024 ];
		int day, mon, year, min, max, rnd2, rnd1 = 0;
		for( int i = 1; i < cnt; ++i ) {
			rnd1 = ( rand( ) % 4 ) + 2;
			for( int j = 1; j < rnd1; ++j ) {
				obj = new udProjectServicePeriod;
				obj->get( "service" )->ini( ( udUInt ) i );
				day = rand( ) % 28 + 1;
				mon = rand( ) % 12 + 1;
				year = ( rand( ) % 40 ) + 1971;
				sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
				date.load( tmp );
				obj->get( "date1" )->ini( date );
				day = rand( ) % 7 + 1;
				date.inc( day, datetime::datetimeDay );
				obj->get( "date2" )->ini( date );
				obj->get( "index" )->ini( ( udUInt ) j );
				arr.push_back( obj );
				// для каждого периода необходимо сгенерировать таблицу цен
				GeneratePriceTable( i, j );
			}
		}
		option.SetTable( "ud_serviceperiod" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		cnt = 11;
		// клиенты
		udInt j = 0;
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectClient;
			name = L"клиент " + val;
			obj->get( "fio" )->ini( name );
			name = L"client " + val;
			obj->get( "fiolatin" )->ini( name );
			name = L"email" + val + L"@mail" + val + L".ru";
			obj->get( "lemail" )->ini( name );
			name = L"телефон " + val;
			obj->get( "lphone" )->ini( name );
			name = L"адрес " + val;
			obj->get( "laddr" )->ini( name );
			day = rand( ) % 28 + 1;
			mon = rand( ) % 12 + 1;
			year = ( rand( ) % 40 ) + 1900;
			sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
			//obj->get( "born" )->ini( date );
			obj->get( "born" )->ini( toWString( tmp ) );
			name = L"место рождения " + val;
			obj->get( "bornplace" )->ini( name );
			name = L"born place " + val;
			obj->get( "bornplacelatin" )->ini( name );
			obj->get( "sex" )->ini( ( udInt ) ( j + 1 ) );
			arr.push_back( obj );
			j = 1 - j;
		}
		option.SetTable( "ud_client" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// обращения
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectRequest;
			obj->get( "status" )->ini( ( udInt ) udProjectRequest::REQUEST_STATUS_OPENED );
			obj->get( "state" )->ini( ( udUInt ) i );
			obj->get( "type" )->ini( ( udUInt ) i );
			name = L"абонент " + val;
			obj->get( "fio" )->ini( name );
			name = L"контактные данные " + val;
			obj->get( "contact" )->ini( name );
			obj->get( "direct" )->ini( ( udUInt ) i );
			obj->get( "manager" )->ini( ( udUInt ) i );
			obj->get( "manager2" )->ini( ( udUInt ) i );
			obj->get( "result" )->ini( ( udUInt ) i );
			arr.push_back( obj );
		}
		option.SetTable( "ud_request" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// заявки
		udUInt amount = 0;
		udInt emptype = udProjectOrder::ORDER_EMPLOYER_CLIENT;
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectOrder;
			obj->get( "created" )->ini( date );
			obj->get( "manager" )->ini( ( udUInt ) i );
			obj->get( "manager2" )->ini( ( udUInt ) i );
			obj->get( "status" )->ini( ( udInt ) udProjectOrder::ORDER_STATUS_OPENED );
			obj->get( "state" )->ini( ( udUInt ) i );
			amount = ( udUInt ) ( rand( ) % 100 );
			obj->get( "amount" )->ini( amount );
			obj->get( "employer" )->ini( ( udUInt ) i );
			obj->get( "emptype" )->ini( emptype );
			obj->get( "date1" )->ini( date );
			obj->get( "date2" )->ini( date );
			obj->get( "direct" )->ini( ( udUInt ) i );
			name = L"комментарий " + val;
			obj->get( "comment" )->ini( name );
			arr.push_back( obj );
			emptype = 1 - emptype;
		}
		option.SetTable( "ud_order" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// маршрут заявки
		cnt = 11;
		for( int i = 1; i < cnt; ++i ) {
			rnd1 = ( rand( ) % 5 ) + 1;
			for( int j = 1; j < rnd1; ++j ) {
				val = toWString( j );
				obj = new udProjectOrderRoute;
				name = L"пункт " + val;
				obj->get( "name" )->ini( name );
				obj->get( "order" )->ini( ( udUInt ) i );
				arr.push_back( obj );
			}
		}
		option.SetTable( "ud_orderroute" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// услуги заявки
		for( int i = 1; i < cnt; ++i ) {
			rnd1 = ( rand( ) % 4 ) + 2;
			for( int j = 1; j < rnd1; ++j ) {
				val = toWString( j );
				obj = new udProjectOrderService;
				obj->get( "order" )->ini( ( udUInt ) i );
				day = rand( ) % 28 + 1;
				mon = rand( ) % 12 + 1;
				year = ( rand( ) % 40 ) + 1971;
				sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
				date.load( tmp );
				obj->get( "date1" )->ini( date );
				day = rand( ) % 7 + 1;
				date.inc( day, datetime::datetimeDay );
				obj->get( "date2" )->ini( date );
				rnd2 = ( rand( ) % 10 ) + 1;
				obj->get( "type" )->ini( rnd2 );
				rnd2 = ( rand( ) % 10 ) + 1;
				obj->get( "city" )->ini( rnd2 );
				name = L"комментарий " + val;
				obj->get( "comment" )->ini( name );
				arr.push_back( obj );
			}
		}
		option.SetTable( "ud_orderservice" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// тур
		emptype = udProjectTour::TOUR_EMPLOYER_CLIENT;
		cnt = 201;
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			date.set( 0 );
			obj = new udProjectTour;
			day = rand( ) % 28 + 1;
			mon = rand( ) % 12 + 1;
			year = ( rand( ) % 40 ) + 1971;
			sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
			date.load( tmp );
			obj->get( "created" )->ini( date );
			name = L"тур " + val;
			obj->get( "name" )->ini( name );
			day = rand( ) % 28 + 1;
			mon = rand( ) % 12 + 1;
			year = ( rand( ) % 40 ) + 1971;
			sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
			date.load( tmp );
			obj->get( "date1" )->ini( date );
			day = rand( ) % 7 + 1;
			date.inc( day, datetime::datetimeDay );
			obj->get( "date2" )->ini( date );
			amount = rand( ) % 1000;
			obj->get( "cost" )->ini( ( udFloat ) amount );
			obj->get( "price" )->ini( ( udFloat ) amount );
			obj->get( "company" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "status" )->ini( ( udInt ) udProjectTour::TOUR_STATUS_OPENED );
			obj->get( "employer" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "emptype" )->ini( emptype );
			obj->get( "manager" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "manager2" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			obj->get( "direct" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			name = L"комментарий " + val;
			obj->get( "comment" )->ini( name );
			obj->get( "state" )->ini( ( udUInt ) ( ( i % 10 ) + 1 ) );
			arr.push_back( obj );
			emptype = 1 - emptype;
		}
		cnt = 11;
		option.SetTable( "ud_tour" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// шаблон тура
		udDWord num = 0;
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectTplTour;
			name = L"шаблон тура " + val;
			obj->get( "name" )->ini( name );
			amount = rand( ) % 1000;
			obj->get( "cost" )->ini( ( udFloat ) amount );
			obj->get( "price" )->ini( ( udFloat ) amount );
			obj->get( "direct" )->ini( ( udUInt ) i );
			num = ( udDWord ) rand( ) % 366;
			obj->get( "daynum" )->ini( num );
			arr.push_back( obj );
		}
		option.SetTable( "ud_tpltour" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// маршрут шаблона тура
		for( int i = 1; i < cnt; ++i ) {
			day = ( rand( ) % 9 ) + 2;
			for( int j = 1; j < day; ++j ) {
				obj = new udProjectTplTourRoute;
				amount = ( rand( ) % 10 ) + 1;
				obj->get( "tpltour" )->ini( ( udUInt ) i );
				obj->get( "city" )->ini( amount );
				arr.push_back( obj );
			}
		}
		option.SetTable( "ud_tpltourroute" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// услуги в шаблонах тура
		int num1 = 0;
		for( int i = 1; i < cnt; ++i ) {
			num1 = ( rand( ) % 10 ) + 5;
			for( int j = 1; j < num1; ++j ) {
				obj = new udProjectTplTourService;
				obj->get( "tpltour" )->ini( ( udUInt ) i );
				amount = ( rand( ) % 200 ) + 1;
				obj->get( "service" )->ini( amount );
				amount = ( rand( ) % 10 ) + 1;
				obj->get( "num" )->ini( amount );
				day = rand( ) % 28 + 1;
				mon = rand( ) % 12 + 1;
				year = ( rand( ) % 40 ) + 1971;
				sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
				date.load( tmp );
				obj->get( "date1" )->ini( date );
				day = rand( ) % 7 + 1;
				date.inc( day, datetime::datetimeDay );
				obj->get( "date2" )->ini( date );
				arr.push_back( obj );
			}
		}
		option.SetTable( "ud_tpltourservice" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// группа
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectGroup;
			name = L"группа " + val;
			obj->get( "name" )->ini( name );
			name = L"комментарий " + val;
			obj->get( "comment" )->ini( name );
			obj->get( "direct" )->ini( ( udUInt ) i );
			arr.push_back( obj );
		}
		option.SetTable( "ud_group" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// периоды группы
		for( int j = 1; j < cnt; ++j ) { // группы
			for( int i = 1; i < cnt; ++i ) { // периоды в группах
				val = toWString( i );
				day = rand( ) % 28 + 1;
				mon = rand( ) % 12 + 1;
				year = ( rand( ) % 40 ) + 1971;
				min = ( rand( ) % 10 + 1 );
				max = rand( ) % 30 + 11;
				obj = new udProjectGroupPeriod;
				obj->get( "group" )->ini( ( udUInt ) j );
				sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
				date.load( string( tmp ) );
				obj->get( "date1" )->ini( date );
				date.inc( 7, udDatetime::datetimeDay );
				obj->get( "date2" )->ini( date );
				obj->get( "min" )->ini( ( udUInt ) min );
				obj->get( "max" )->ini( ( udUInt ) max );
				obj->get( "index" )->ini( ( udUInt ) i );
				obj->get( "client" )->ini( ( udUInt ) 1 );
				arr.push_back( obj );
			}
		}
		option.SetTable( "ud_groupperiod" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// туристы в турах
		cnt = 201;
		for( int i = 1; i < cnt; ++i ) {
			for( int j = 1; j < 11; ++j ) {
				obj = new udProjectTourClient;
				obj->get( "tour" )->ini( ( udUInt ) i );
				obj->get( "client" )->ini( ( udUInt ) j );
				arr.push_back( obj );
			}
		}
		cnt = 11;
		option.SetTable( "ud_tourclient" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// туры в группах
		for( int i = 1; i < cnt; ++i ) { // группы
			for( int j = 1; j < cnt; ++j ) { // периоды
				obj = new udProjectGroupTour;
				obj->get( "group" )->ini( ( udUInt ) i );
				obj->get( "tour" )->ini( ( udUInt ) ( i * 10 + j ) );
				obj->get( "period" )->ini( ( udUInt ) j );
				arr.push_back( obj );
			}
			for( int j = 1; j < cnt; ++j ) { // периоды
				obj = new udProjectGroupTour;
				obj->get( "group" )->ini( ( udUInt ) i );
				obj->get( "tour" )->ini( ( udUInt ) ( i * 5 + j ) );
				obj->get( "period" )->ini( ( udUInt ) j );
				arr.push_back( obj );
			}
		}
		cnt = 11;
		option.SetTable( "ud_grouptour" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// услуги в турах
		cnt = 201;
		date.set( 0 );
		int daynum = 0;
		for( int i = 1; i < cnt; ++i ) { // туры
			for( int j = 1; j < 11; ++j ) { // услуги
				obj = new udProjectTourService;
				obj->get( "tour" )->ini( ( udUInt ) i );
				obj->get( "service" )->ini( ( udUInt ) j );
				amount = rand( ) % 30;
				obj->get( "num" )->ini( amount );
				day = rand( ) % 28 + 1;
				mon = rand( ) % 12 + 1;
				year = ( rand( ) % 40 ) + 1971;
				sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
				date.load( string( tmp ) );
				obj->get( "date1" )->ini( date );
				daynum = ( rand( ) % 7 ) + 1;
				date.inc( daynum, datetime::datetimeDay );
				obj->get( "date2" )->ini( date );
				obj->get( "ready" )->ini( ( udInt ) 1 );
				arr.push_back( obj );
			}
		}
		cnt = 11;
		option.SetTable( "ud_tourservice" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// документ клиента
		date.load( "2009-05-07" );
		datetime date1( "2010-05-07" );
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectClDoc;
			obj->get( "client" )->ini( ( udUInt ) i );
			obj->get( "type" )->ini( ( udUInt ) i );
			name = val + val;
			obj->get( "series" )->ini( name );
			name = val + val + val;
			obj->get( "number" )->ini( name );
			obj->get( "issuedate" )->ini( date );
			name = L"выдан " + val;
			obj->get( "issuedby" )->ini( name );
			name = L"место " + val;
			obj->get( "birthplace" )->ini( name );
			obj->get( "enddate" )->ini( date1 );
			name = L"ФИО " + val;
			obj->get( "fio" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_cldoc" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// ответ партнера
		date.load( );
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectPartnerAnswr;
			obj->get( "created" )->ini( date );
			obj->get( "partner" )->ini( ( udUInt ) i );
			name = L"ответ " + val;
			obj->get( "text" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_partneranswr" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// запрос партнеру
		date.load( );
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectPartnerReq;
			obj->get( "partner" )->ini( ( udUInt ) i );
			name = L"комментарий " + val;
			obj->get( "comment" )->ini( name );
			obj->get( "manager" )->ini( ( udUInt ) i );
			obj->get( "created" )->ini( date );
			name = L"текст " + val;
			obj->get( "text" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_partnerreq" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// лог
		date.load( );
		for( int i = 1; i < cnt; ++i ) {
			val = toWString( i );
			obj = new udProjectLog;
			obj->get( "created" )->ini( date );
			obj->get( "user" )->ini( ( udUInt ) i );
			name = L"действие " + val;
			obj->get( "action" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_log" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// пункт перехода
		for( int i = 1; i < cnt; ++i ) {
			name = L"пункт перехода " + toWString( i );
			obj = new udProjectTransferPoint;
			obj->get( "name" )->ini( name );
			name = L"transfer point " + toWString( i );
			obj->get( "namelatin" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_transferpoint" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// марщрут
		for( int i = 1; i < cnt; ++i ) {
			name = L"маршрут " + toWString( i );
			obj = new udProjectRoute;
			obj->get( "name" )->ini( name );
			name = L"route " + toWString( i );
			obj->get( "namelatin" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_route" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// принимающая сторона
		for( int i = 1; i < cnt; ++i ) {
			name = L"прин. сторона " + toWString( i );
			obj = new udProjectHostSide;
			obj->get( "name" )->ini( name );
			name = L"hostside " + toWString( i );
			obj->get( "namelatin" )->ini( name );
			arr.push_back( obj );
		}
		option.SetTable( "ud_hostside" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// списки туристов
		for( int i = 1; i < cnt; ++i ) {
			obj = new udProjectTouristList;
			day = rand( ) % 28 + 1;
			mon = rand( ) % 12 + 1;
			year = ( rand( ) % 40 ) + 1971;
			sprintf_s( tmp, 1024, "%d-%02d-%02d", year, mon, day );
			date.load( string( tmp ) );
			obj->get( "date1" )->ini( date );
			daynum = ( rand( ) % 7 ) + 1;
			date.inc( daynum, datetime::datetimeDay );
			obj->get( "date2" )->ini( date );
			obj->get( "exit" )->ini( ( udUInt ) i );
			obj->get( "enter" )->ini( ( udUInt ) i );
			obj->get( "host" )->ini( ( udUInt ) i );
			obj->get( "route" )->ini( ( udUInt ) i );
			obj->get( "leader" )->ini( ( udUInt ) i );
			arr.push_back( obj );
		}
		option.SetTable( "ud_touristlist" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// клиенты в списках туристов
		for( int i = 1; i < cnt; ++i ) {
			obj = new udProjectTouristListLink;
			obj->get( "list" )->ini( ( udUInt ) i );
			obj->get( "client" )->ini( ( udUInt ) i );
			arr.push_back( obj );
		}
		option.SetTable( "ud_touristlistlink" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr );
		// создаем лицевые счета
		MakeAccounts( );
		// подсчитываем себестоимость туров
		CalculateCost( );
		// Подсчитываем итоговые цены туров
		CalculateFinal( );
	} // void FillTestData

	void CAppInitProject::CollectDatabases( const string& srcdb ) {
		this->TruncateTables( g_pApp->GetHandler( ), g_pApp->GetDatabase( ) );

		udPMySql db = new udMySql;
		udPAppConfig app_config	= g_pApp->GetConfig( );
		udPLog __log = g_pApp->GetLog( );
		string	host		= app_config->get( "database_host"		)->toString( ),
				user		= app_config->get( "database_user"		)->toString( ),
				password	= app_config->get( "database_password"	)->toString( ),
				dbname		= srcdb;
		udUInt	port		= app_config->get( "database_port"		)->toUInt( );
		db->SetAccData( host, user, password, dbname );
		db->SetPortNumber( port );
		udMySql::cmysqlError iErr = db->Connect( true );//db->Create( host, user, password, dbname, port );
		if ( iErr == udMySql::UDSDK_MYSQL_ERROR_OK ) {
			db->SetCharacterSet( "utf8" );
			__log->Write( "подсоединились\n" );
			udPHandler handler = new udHandler( db ),
				ghandler = g_pApp->GetHandler( );
			udHandlerOption option;
			udArrBase arr;
			udArrError err;

			udUInt cnt = 0;
			string log;
			for( _asc_tables::iterator i = m_arrTables.begin( ); i != m_arrTables.end( ); ++i ) {
				option.SetTable( i->first );
				option.SetClass( i->second );
				handler->GetUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
				} else if ( arr.empty( ) ) {
					__log->Write( i->first + " - пусто\n" );
				} else {
					log = "\nсущности таблицы: '" + i->first + "'\n";
					for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
						( *j )->dump( log );
						log += "\n";
					}
					__log->Write( log );
					ghandler->AddUdObject( arr, option );
				}
				ClearArrBase( arr );
				err.clear( );
			}

			delete handler;
		} else {
			__log->Write( "не удалось подсоединиться\n" );
		}
		delete db;
	} // void CollectDatabases

	void CAppInitProject::InitModel( udPModelWork pModel ) {
		pModel->SetHandler( "request",		new udHRequest		);	// обращение
		pModel->SetHandler( "order",			new udHOrder		);	// заявка
		pModel->SetHandler( "tour",			new udHTour			);	// тур
		pModel->SetHandler( "tpltour",		new udHTplTour		);	// шаблон тура
		pModel->SetHandler( "group",			new udHGroup		);	// группа
		pModel->SetHandler( "#info",			new udHInfoPage		);	// справочники
		pModel->SetHandler( "country",		new udHCountry		);	// страна
		pModel->SetHandler( "tservice",		new udHTService		);	// тип услуги
		pModel->SetHandler( "sorder",		new udHSOrder		);	// статус заявки
		pModel->SetHandler( "rrequest",		new udHRRequest		);	// результат обращения
		pModel->SetHandler( "stour",			new udHSTour		);	// статус тура
		pModel->SetHandler( "trequest",		new udHTRequest		);	// тип обращения
		pModel->SetHandler( "muservice",		new udHMUService	);	// единица измерения услуги
		pModel->SetHandler( "direct",		new udHDirect		);	// направление
		pModel->SetHandler( "nameservice",	new udHNameService	);	// имя услуги
		pModel->SetHandler( "doctype",		new udHDocType		);	// тип документа
		pModel->SetHandler( "currency",		new udHCurrency		);	// валюта
		pModel->SetHandler( "user",			new udHUser			);	// пользователь
		pModel->SetHandler( "log",			new udHLog			);	// лог
		pModel->SetHandler( "city",			new udHCity			);	// город
		pModel->SetHandler( "currencypair",	new udHCurrencyPair	);	// валютная пара
		pModel->SetHandler( "company",		new udHCompany		);	// компания
		pModel->SetHandler( "cldoc",			new udHClDoc		);	// документы клиента
		pModel->SetHandler( "partner",		new udHPartner		);	// партнеры
		pModel->SetHandler( "partnerreq",	new udHPartnerReq	);	// запросы партнеру
		pModel->SetHandler( "partneranswr",	new udHPartnerAnswr	);	// ответ партнера
		pModel->SetHandler( "service",		new udHService		);	// услуга
		pModel->SetHandler( "client",		new udHClient		);	// клиент
		pModel->SetHandler( "srequest",		new udHSRequest		);	// состояние обращения
		pModel->SetHandler( "clientur",		new udHClientUr		);	// клиенты юр. лица
		pModel->SetHandler( "persacc",		new udHPersAcc		);	// лицевые счета
		//pModel->SetHandler( "report",		new udHReport		);	// отчеты
		pModel->SetHandler( "doctpl",			new udHDocTemplate	);	// шаблоны документов
		pModel->SetHandler( "tourpayorder",		new udHTourPayOrder	);	// указание на оплату тура
		pModel->SetHandler( "doctplprinted",		new udHDocTemplatePrinted	);	// распечатанные шаблоны документов
		pModel->SetHandler( "transferpoint",		new udHTransferPoint		);	// пункт перехода
		pModel->SetHandler( "route",				new udHRoute			);	// маршрут
		pModel->SetHandler( "hostside",			new udHHostSide			);	// принимающая сторона
		pModel->SetHandler( "touristlist",		new udHTouristList		);	// список туристов
		pModel->SetHandler( "groupperiod",		new udHGroupPeriod		);	// список периодов групп
		pModel->SetHandler( "report_landing",	new udHReportLanding	);	// Отчет "Посадочная ведомость"
		pModel->SetHandler( "report_group_pay",	new udHReportGroupPay	);	// Отчет "Оплата группы"
		pModel->SetHandler( "report_hotel_pay",	new udHReportHotelPay	);	// Отчет "Оплата гостиниц"
		pModel->SetHandler( "report_day_pay",	new udHReportDayPay		);	// Отчет "Оплата по дням"
		pModel->Init( );

		pModel->SetLockHandler( new udHObjectLock2 ); // блокировки

		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		// связи объектов
		option.SetTable( "ud_objlnk" );
		option.SetClass( GetProjectObjectLink );
		handler->CheckTable( option );

		udPAppConfig config = g_pApp->GetConfig( );
		if ( config->isset( "collect_databases" ) ) {
			this->CollectDatabases( config->get( "collect_databases" )->toString( ) );
		}
		if ( config->isset( "fill_test_data" ) ) {
			this->FillTestData( );
		}
	} // void InitModel

	void CAppInitProject::InitTablesMap( ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_tour"				] = GetProjectTour;
		m_arrTables[ "ud_tourroute"			] = GetProjectTourRoute;
		m_arrTables[ "ud_tourservice"		] = GetProjectTourService;
		m_arrTables[ "ud_tourclient"		] = GetProjectTourClient;
		m_arrTables[ "ud_tourtour"			] = GetProjectTourTour;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_group"				] = GetProjectGroup;
		m_arrTables[ "ud_grouptour"			] = GetProjectGroupTour;
		m_arrTables[ "ud_groupperiod"		] = GetProjectGroupPeriod;
		m_arrTables[ "ud_grouptpltour"		] = GetProjectGroupTplTour;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_request"			] = GetProjectRequest;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_order"				] = GetProjectOrder;
		m_arrTables[ "ud_orderroute"		] = GetProjectOrderRoute;
		m_arrTables[ "ud_orderservice"		] = GetProjectOrderService;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_tpltour"			] = GetProjectTplTour;
		m_arrTables[ "ud_tpltourroute"		] = GetProjectTplTourRoute;
		m_arrTables[ "ud_tpltourservice"	] = GetProjectTplTourService;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_client"			] = GetProjectClient;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_clientur"			] = GetProjectClientUr;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_service"			] = GetProjectService;
		m_arrTables[ "ud_serviceperiod"		] = GetProjectServicePeriod;
		m_arrTables[ "ud_pricetableitem"	] = GetProjectPriceTableItem;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_direct"			] = GetProjectDirect;
		m_arrTables[ "ud_trequest"			] = GetProjectTRequest;
		m_arrTables[ "ud_tservice"			] = GetProjectTService;
		m_arrTables[ "ud_nameservice"		] = GetProjectNameService;
		m_arrTables[ "ud_muservice"			] = GetProjectMUService;
		m_arrTables[ "ud_country"			] = GetProjectCountry;
		m_arrTables[ "ud_rrequest"			] = GetProjectRRequest;
		m_arrTables[ "ud_stour"				] = GetProjectSTour;
		m_arrTables[ "ud_city"				] = GetProjectCity;
		m_arrTables[ "ud_sorder"			] = GetProjectSOrder;
		m_arrTables[ "ud_cldoc"				] = GetProjectClDoc;
		m_arrTables[ "ud_doctype"			] = GetProjectDocType;
		m_arrTables[ "ud_currency"			] = GetProjectCurrency;
		m_arrTables[ "ud_currencypair"		] = GetProjectCurrencyPair;
		m_arrTables[ "ud_company"			] = GetProjectCompany;
		m_arrTables[ "ud_partner"			] = GetProjectPartner;
		m_arrTables[ "ud_partnerreq"		] = GetProjectPartnerReq;
		m_arrTables[ "ud_partneranswr"		] = GetProjectPartnerAnswr;
		m_arrTables[ "ud_srequest"			] = GetProjectSRequest;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_persacc"			] = GetProjectPersAcc;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_user"				] = GetProjectUser;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_arrTables[ "ud_attach"			] = GetProjectAttach;
	} // void InitTablesMap

	void CAppInitProject::InitSync( udPDatabaseSync sync ) {
		udPModelWork model = g_pApp->GetModel( );
		model->InitSync( );
		sync->AddSyncTable( "#database", "main" );
	} // void InitSync

	void CAppInitProject::HandleSync( udPSocket sock, udAscBase& ascClient, udAscBase& ascServer ) {
		udPHandler		handler = g_pApp->GetHandler( );
		udArrBase		arr;
		udArrError		err;
		udHandlerOption	option;
		udPBase			obj1 = NULL, obj2 = NULL;
		wstring			text = L"";
		string			log = "";
		udPWndServer	wndServer	= g_pApp->GetWndServer( );
/*
как будем сравнивать и что будем пересылать на клиент и ждать в ответ?
равенство версий должно вызывать подозрение, что потребует дополнительной проверки (по количеству записей
и по auto_increment таблиц, то есть это надо будет спросить у клиента)
сначала сравниваем справочники:

страна { город, услуга }
тип услуги { услуга, примерная услуга (в заявке) }
состояние заявки { заявка }
результат обращения { обращение }
состояние тура { тур }
тип обращения { обращение }
направление { группа, шаблон тура, тур, обращение, заявка }
название услуги { услуга }
тип документа { документ клиента }
валюта { валютная пара, цена услуги }
город { услуга, примерная услуга (в заявке), маршрут (в туре), маршрут (в шаблоне тура) }
компания { тур }
партнер { услуга, запрос/ответ партнера }
состояние обращения { обращение }
единица измерения услуги { услуга }
пользователь { лог, заявка (менеджер), обращение (менеджер), тур (менеджер) } [надо поискать еще сущности, где по умолчанию прописывается их автор.]
запрос партнеру { пока нигде }
ответ партнера { пока нигде }
документ клиента { клиент физ. }
клиент физ. { заявка (заказчик), тур (заказчик, турист) }
клиент юр. { заявка (заказчик), тур (заказчик) }
услуга { тур (информация об услуге), шаблон тура }
обращение { пока нигде }
заявка { пока нигде }
тур { группа }
шаблон тура { группа }
группа { пока нигде }

признаки разницы:
1. у сервера версия таблицы равна 0, то сервак чист и с клиента можно скачивать всю таблицу.
2. у клиента версия меньше чем у сервера, берем все, что есть на серваке и сравниваем с клиентом.
3. у клиента версия больше, то аномалия (мануал по борьбе с аномалиями позже).

1. запрашиваем у клиента сущности.
2. сравниваем их с сущностями из базы.

сущность есть на обоих базах (оставляем как есть).
сущность есть на сервере, но нет на клиенте (отправить клиенту в ответе).
сущность есть на клиенте, но нет на сервере (добавляем на сервак, возвращаем их с новыми ID).

объекты, которые совпадают именами, но рознятся в ID приводим к единому ID
объекты, которые отсутствуют у сервера, делаем их добавление и записываем ID каждого
объекты, которые отсутствуют у клиента, записываем их в массив добавления
с клиента так же необходимо принимать объекты удаления (надо это отслеживать)
на клиенте необходимо отслеживать удаление объектов при отсутствии сервера
чтобы серверу не отсылать оффлайн-объекты на удаление (а вдруг совпадут ID с реальными)
необходимо на серваке отслеживать удаленные объекты
при холостом старте (пустой сервак или клиент) индексировать текущую базу и синкать их
при добавлении объекта необходимо проверить, чтобы такого не было в удалении (по ID)
пока удаление не журналируем, а при удалении общаемся с сервером (нет связи - нет удаления)

*/
		//////////////////////////////////////////////////////////////////////////////////////////
		// вся база
		obj1 = ascClient[ "#database" ];
		obj2 = ascServer[ "#database" ];
		udDWord	dwClientVersion	= obj1->get( "version" )->toDWord( ),
				dwServerVersion	= obj2->get( "version" )->toDWord( );

		if ( dwServerVersion == 0 ) {
			//////////////////////////////////////////////////////////////////////////////////////
			// тут пустой сервак
			this->FeedServerMode( sock, dwClientVersion, ascClient, ascServer ); // скачиваем все данные с клиента
			/////////////////////////////////////////////////////////////////////////////////////
		} else if ( dwClientVersion == 0 ) {
			//////////////////////////////////////////////////////////////////////////////////////
			// тут пустой клиент
/*
	дополнительно стоит все таки с клиента скачать все его данные, а то вдруг он сначала поработал оффлайн
	и базу наполнил, что может привести к потери данных, если данные проигнорировать.
*/
			this->FeedClientMode( sock, ascClient, ascServer );
			/////////////////////////////////////////////////////////////////////////////////////
		} else if ( dwClientVersion > dwServerVersion ) {
			//////////////////////////////////////////////////////////////////////////////////////
			// тут подключили жирного клиента
			//////////////////////////////////////////////////////////////////////////////////////
		} else if ( dwServerVersion > dwClientVersion ) {
			//////////////////////////////////////////////////////////////////////////////////////
			// тут подключили младшего клиента
			this->LessClientMode( sock, dwClientVersion, ascClient, ascServer );
			//////////////////////////////////////////////////////////////////////////////////////
		} else {
			//////////////////////////////////////////////////////////////////////////////////////
			// тут клиент с таким же номером
			g_pApp->GetWndServer( )->UpdateText( L"Такой же клиент" );
/*
	необходимо сверить метаданные + реальные данные, то есть при обмене метаданными
	клиент должен будет наполнить поля lastid и count, которые позволят сделать заключение
	при остальных условиях невозможно определить разные ли базы, при равенстве версий
*/
			/////////////////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CAppInitProject::HandleSync exit\r\n" );
	} // void HandleSync

	void CAppInitProject::FeedClientMode( udPSocket sock, udAscBase& ascClient, udAscBase& ascServer ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::FeedClientMode\n" );
/*
	чтобы не похерить данные оффлайн клиента, у которого наполнена база
	необходимо все данные запросить у него, а потом заставить его затранкейтить все таблицы
	затем уже отсылать ему всю базу

	для всех этих сущностей необходимо будет переколбашивать ID и связи по ним
*/
		//////////////////////////////////////////////////////////////////////////////////////////
		udBinaryData data;
		data.add( ( char ) 2 );
		data.add( string( "#database" ) );
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		data.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// наполняем базу новыми данными от клиента и отправляем ему ее всю обратно
		udPHandler handler = g_pApp->GetHandler( );
		// сливаем две базы в одну
		//this->MergeDatabase( sock, ascClient );
		// отправляем базу клиенту
		this->SendDatabase( sock, handler );
		// отправляем метаданные
		this->SendMetadata( sock );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void FeedClientMode

	void CAppInitProject::SendDatabase( udPSocket sock, udPHandler handler ) {
		udPLog log = g_pApp->GetLog( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udBinaryData data;
		char code = 0;

		for( _asc_tables::iterator i = m_arrTables.begin( ); i != m_arrTables.end( ); ++i ) {
			option.SetTable( i->first );
			option.SetClass( i->second );
			handler->GetUdObject( arr, option, &err );
			if ( err.empty( ) ) { // все пучком, отсылаем сущности
				// строим данные
				log->Write( "таблица: '" + i->first + "'\n" );
				data.add( i->first );
				WriteInstances( arr, data );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				data.clear( );
				// ждем ответ и очищаемся
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				if ( ( sock->ReadByte( &code ) == 0 ) || ( code != 80 ) ) {
					break;
				}
			} else {
				for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
					log->Write( i->GetText( ) + "\n" );
				}
				err.clear( );
			}
			ClearArrBase( arr );
			option.Clear( );
		}

		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
	} // void SendDatabase

	void CAppInitProject::ReadAndSaveDatabase( udPSocket sock, udPHandler handler ) {
		udPLog log = g_pApp->GetLog( );
		udArrError err;
		udHandlerOption option;
		udType table( string( "" ) );
		udArrBase arr;
		bool finished = false;
		_asc_tables::iterator i;
		udUInt cnt = 0;
		while( !finished ) {
			if ( sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 ) != 1 ) {
				finished = true;
				break;
			}
			if ( table.read( sock ) == 0 ) {
				finished = true;
				break;
			}
			log->Write( "таблица: '" + table.toString( ) + "'\n" );
			i = m_arrTables.find( table.toString( ) );
			if ( i != m_arrTables.end( ) ) {
				ReadInstances( sock, arr, i->second );
				if ( !arr.empty( ) ) {
					option.SetTable( i->first );
					handler->AddUdObject( arr, option, &err );
					if ( !err.empty( ) ) {
						for( udArrError::iterator j = err.begin( ); j != err.end( ); ++j ) {
							log->Write( j->GetText( ) + "\n" );
						}
						err.clear( );
					}
					ClearArrBase( arr );
				}
				++cnt;
			}
			sock->SendByte( ( char ) 80 );
			if ( cnt >= m_arrTables.size( ) ) {
				finished = true;
				break;
			}
		}
		sock->SendByte( ( char ) 80 );
	} // void ReadAndSaveDatabase

	void CAppInitProject::TruncateTables( udPHandler handler, udPDatabase db ) {
		udPLog log = g_pApp->GetLog( );
		udArrError err;
		udHandlerOption option;

		for( _asc_tables::iterator i = m_arrTables.begin( ); i != m_arrTables.end( ); ++i ) {
			option.SetTable( i->first );
			option.SetClass( i->second );
			handler->CheckTable( option, &err );
			if ( !err.empty( ) ) {
				for( udArrError::iterator j = err.begin( ); j != err.end( ); ++j ) {
					log->Write( j->GetText( ) + "\n" );
				}
				err.clear( );
			}
			if ( db->Query( "TRUNCATE TABLE `" + i->first + "`" ) ) {
				log->Write( "таблица '" + i->first + "' очищена\n" );
			} else {
				log->Write( "таблица '" + i->first + "' НЕ очищена\n" );
			}
		}
	} // void TruncateTables

	void CAppInitProject::MergeDatabase( udPSocket sock, udAscBase& ascClient ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// создаем временную базу куда запихнем клиентскую
		udPMySql db = new udMySql;
		udPAppConfig appConfig = g_pApp->GetConfig( );
		string	host		= appConfig->get( "database_host"		)->toString( ),
				user		= appConfig->get( "database_user"		)->toString( ),
				password	= appConfig->get( "database_password"	)->toString( ),
				dbname		= appConfig->get( "database_tmpdbname"	)->toString( );
		udUInt	port		= appConfig->get( "database_port"		)->toUInt( );
		db->SetAccData( host, user, password, dbname );
		db->SetPortNumber( port );
		udMySql::cmysqlError iErr = db->Connect( true );//db->Create( host, user, password, dbname, port );
		if ( iErr != udMySql::UDSDK_MYSQL_ERROR_OK ) {
			delete db;
			g_pApp->GetLog( )->Write( "не удалось создать новое соединение, ошибка: " + toString( ( int ) iErr ) + "\n" );
			return;
		}
		db->SetCharacterSet( "utf8" );
		
		udPHandler handler = new udHandler( db );
		//////////////////////////////////////////////////////////////////////////////////////////
		// очищаем таблицы
		this->TruncateTables( handler, db );
		//////////////////////////////////////////////////////////////////////////////////////////
		// закачиваем данные клиента и добавляем их во временную базу
		this->ReadAndSaveDatabase( sock, handler );
		//////////////////////////////////////////////////////////////////////////////////////////
		// простые сущности с одной связью
		// состояние заявки
		this->MergeSimpleObjects( handler,
			"ud_sorder", GetProjectSOrder, "name",
			"ud_order", GetProjectOrder, "state"
		);
		// состояние тура
		this->MergeSimpleObjects( handler,
			"ud_stour", GetProjectSTour, "name",
			"ud_tour", GetProjectTour, "state"
		);
		// название услуги
		this->MergeSimpleObjects( handler,
			"ud_nameservice", GetProjectNameService, "name",
			"ud_service", GetProjectService, "name"
		);
		// единица измерения услуги
		this->MergeSimpleObjects( handler,
			"ud_muservice", GetProjectMUService, "name",
			"ud_service", GetProjectService, "munit"
		);
		// состояние обращения
		this->MergeSimpleObjects( handler,
			"ud_srequest", GetProjectSRequest, "name",
			"ud_request", GetProjectRequest, "state"
		);
		// тип обращения
		this->MergeSimpleObjects( handler,
			"ud_trequest", GetProjectTRequest, "name",
			"ud_request", GetProjectRequest, "type"
		);
		// результат обращения
		this->MergeSimpleObjects( handler,
			"ud_rrequest", GetProjectRRequest, "name",
			"ud_request", GetProjectRequest, "result"
		);
		// тип документа клиента
		this->MergeSimpleObjects( handler,
			"ud_doctype", GetProjectDocType, "name",
			"ud_cldoc", GetProjectClDoc, "type"
		);
		//////////////////////////////////////////////////////////////////////////////////////////
		// простые сущности с несколькими связями
		vctmerge arrMerge;
		_tomerge objMerge;

		// страна
		objMerge.table			= "ud_service";
		objMerge.fnGetObject	= GetProjectService;
		objMerge.attr			= "country";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_city";
		objMerge.fnGetObject	= GetProjectCity;
		objMerge.attr			= "country";
		arrMerge.push_back( objMerge );
		this->MergeSimpleObjectsMultiple( handler, "ud_country", GetProjectCountry, "name", arrMerge );
		arrMerge.clear( );

		// тип услуги
		objMerge.table			= "ud_service";
		objMerge.fnGetObject	= GetProjectService;
		objMerge.attr			= "type";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_orderservice";
		objMerge.fnGetObject	= GetProjectOrderService;
		objMerge.attr			= "type";
		arrMerge.push_back( objMerge );
		this->MergeSimpleObjectsMultiple( handler, "ud_tservice", GetProjectTService, "name", arrMerge );
		arrMerge.clear( );

		// направление
		objMerge.table			= "ud_request";
		objMerge.fnGetObject	= GetProjectRequest;
		objMerge.attr			= "direct";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_order";
		objMerge.fnGetObject	= GetProjectOrder;
		objMerge.attr			= "direct";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_tour";
		objMerge.fnGetObject	= GetProjectTour;
		objMerge.attr			= "direct";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_tpltour";
		objMerge.fnGetObject	= GetProjectTplTour;
		objMerge.attr			= "direct";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_group";
		objMerge.fnGetObject	= GetProjectGroup;
		objMerge.attr			= "direct";
		arrMerge.push_back( objMerge );
		this->MergeSimpleObjectsMultiple( handler, "ud_direct", GetProjectDirect, "name", arrMerge );
		arrMerge.clear( );

		// пользователь
		objMerge.table			= "ud_request";
		objMerge.fnGetObject	= GetProjectRequest;
		objMerge.attr			= "manager";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_request";
		objMerge.fnGetObject	= GetProjectRequest;
		objMerge.attr			= "manager2";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_order";
		objMerge.fnGetObject	= GetProjectOrder;
		objMerge.attr			= "manager";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_order";
		objMerge.fnGetObject	= GetProjectOrder;
		objMerge.attr			= "manager2";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_tour";
		objMerge.fnGetObject	= GetProjectTour;
		objMerge.attr			= "manager";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_tour";
		objMerge.fnGetObject	= GetProjectTour;
		objMerge.attr			= "manager2";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_partneranswr";
		objMerge.fnGetObject	= GetProjectPartnerAnswr;
		objMerge.attr			= "user";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_partnerreq";
		objMerge.fnGetObject	= GetProjectPartnerReq;
		objMerge.attr			= "manager";
		arrMerge.push_back( objMerge );
		this->MergeSimpleObjectsMultiple( handler, "ud_user", GetProjectUser, "login", arrMerge );
		arrMerge.clear( );

		// обращение (необходимо определить критерии равенства объектов)
		this->MergeObjects( handler, "ud_request", GetProjectRequest );

		// валюта
		objMerge.table			= "ud_currencypair";
		objMerge.fnGetObject	= GetProjectCurrencyPair;
		objMerge.attr			= "currency1";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_currencypair";
		objMerge.fnGetObject	= GetProjectCurrencyPair;
		objMerge.attr			= "currency2";
		arrMerge.push_back( objMerge );
		objMerge.table			= "ud_pricetableitem";
		objMerge.fnGetObject	= GetProjectPriceTableItem;
		objMerge.attr			= "currency";
		arrMerge.push_back( objMerge );
		this->MergeSimpleObjectsMultiple( handler, "ud_currency", GetProjectCurrency, "code", arrMerge );
		arrMerge.clear( );

		// валютная пара
		this->MergeObjects( handler, "ud_currencypair", GetProjectCurrencyPair );

		// город
		// компания
		// партнер
		// запрос партнеру
		// ответ партнера
		// клиент физ лицо
		// документ клиента
		// клиент юр лицо
		// услуга
		// заявка
		// тур
		// шаблон тура
		// группа
		// приложение
		// пункт маршрута заявки
		// запись списка услуг заявки
		// период услуги
		// цена услуги
		// лицевой счет
		// запись списка туристов тура
		// запись списка услуг тура
		// пункт маршрута тура
		// связь туров
		// пункт маршрута шаблона тура
		// запись списка услуг тура
		// запись списка туров группы
		// запись списка периодов группы
		//////////////////////////////////////////////////////////////////////////////////////////
		delete handler;
		delete db;
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void MergeDatabase

	void CAppInitProject::MergeObjects( udPHandler tmphandler, const string& table, pfnGetObject fnGetObject ) {
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
		string log;
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		//////////////////////////////////////////////////////////////////////////////////////////
		// выгребаем временные сущности
		option.SetTable( table );
		option.SetClass( fnGetObject );
		tmphandler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			err.clear( );
		} else if ( !arr.empty( ) ) {
			udPBase obj = NULL;
			udArrString arrQuery;
			udArrBase arr1;
			
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				obj->get( "id" )->ini( ( udUInt ) 0 );
				handler->AddUdObject( obj, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
					break;
				} else {
					udUInt id = ( udUInt ) db->GetInsertId( );
					obj->get( "id" )->ini( id );
					arr1.push_back( obj );
					handler->AddUdObject( arr, option, arrQuery );
					arr1.clear( );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			ClearArrBase( arr );
		}
	} // void MergeObjects

	void CAppInitProject::MergeSimpleObjects( udPHandler tmphandler, const string& table, pfnGetObject fnGetObject, const string& attr_find_clone, const string& table_parent, pfnGetObject fnGetObjectParent, const string& attr_parent ) {
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
		string log;
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		//////////////////////////////////////////////////////////////////////////////////////////
		// выгребаем временные сущности
		option.SetTable( table );
		option.SetClass( fnGetObject );
		tmphandler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			err.clear( );
		} else if ( !arr.empty( ) ) {
			udPBase obj = NULL;
			udArrString arrQuery;
			udUInt id = 0, id_old = 0;
			udArrBase arr1;
			udBaseConfig config;
			string szWhere;

			obj = fnGetObject( );
			obj->GetConfig( config );
			delete obj;
			obj = NULL;
			//
			// необходимо осуществлять поиск объектов
			// если находим, то только прописываем ID
			//
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				// ищем такой же объект
				szWhere = config.GetAttrIndex( attr_find_clone ) + "=" + obj->GetAttrValue( attr_find_clone, config, udBase::UDSDK_CONFIG_DATABASE )->toString( );
				option.SetTable( table );
				option.SetClass( fnGetObject );
				handler->GetUdObject( arr1, option );
				if ( arr1.empty( ) ) {
					// добавляем новый объект
					id_old = obj->get( "id" )->toUInt( );
					obj->get( "id" )->ini( ( udUInt ) 0 );
					handler->AddUdObject( obj, option, &err );
					if ( !err.empty( ) ) {
						WriteErrors( __log, err );
						err.clear( );
						break;
					} else {
						id = ( udUInt ) db->GetInsertId( );
						obj->get( "id" )->ini( id );
						arr1.push_back( obj );
						handler->AddUdObject( arr, option, arrQuery );
						arr1.clear( );
						obj->get( "id_old" )->ini( id_old );
					}
				} else { // записываем ID уже существующего объекта
					obj->get( "id_old" )->ini( obj->get( "id" )->toUInt( ) );
					obj->get( "id" )->ini( arr1[ 0 ]->get( "id" )->toUInt( ) );
					ClearArrBase( arr1 );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			// выгребаем временные объекты и переписываем у них ID связей
			arr1.clear( );
			option.Clear( );
			option.SetTable( table_parent );
			option.SetClass( fnGetObjectParent );
			tmphandler->GetUdObject( arr1, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
			} else if ( !arr1.empty( ) ) {
				udPBase	obj1 = NULL;
				for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
					obj = *i;
					id = obj->get( attr_parent )->toUInt( );
					for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
						obj1 = *i;
						if ( id == obj1->get( "id_old" )->toUInt( ) ) {
							obj->get( attr_parent )->ini( obj1->get( "id" )->toUInt( ) );
							break;
						}
					}
				}
				ClearArrBase( arr1 );
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			ClearArrBase( arr );
		}
	} // void MergeSimpleObjects

	void CAppInitProject::MergeSimpleObjectsMultiple( udPHandler tmphandler, const string& table, pfnGetObject fnGetObject, const string& attr_find_clone, const vctmerge& arrMerge ) {
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
		string log;
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		//////////////////////////////////////////////////////////////////////////////////////////
		// выгребаем временные сущности
		option.SetTable( table );
		option.SetClass( fnGetObject );
		tmphandler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			err.clear( );
		} else if ( !arr.empty( ) ) {
			udPBase obj = NULL;
			udArrString arrQuery;
			udUInt id = 0, id_old = 0;
			udArrBase arr1;
			udBaseConfig config;
			string szWhere;

			obj = fnGetObject( );
			obj->GetConfig( config );
			delete obj;
			obj = NULL;
			//
			// необходимо осуществлять поиск объектов
			// если находим, то только прописываем ID
			//
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				// ищем такой же объект
				szWhere = config.GetAttrIndex( attr_find_clone ) + "=" + obj->GetAttrValue( attr_find_clone, config, udBase::UDSDK_CONFIG_DATABASE )->toString( );
				option.SetTable( table );
				option.SetClass( fnGetObject );
				handler->GetUdObject( arr1, option );
				if ( arr1.empty( ) ) {
					// добавляем новый объект
					id_old = obj->get( "id" )->toUInt( );
					obj->get( "id" )->ini( ( udUInt ) 0 );
					handler->AddUdObject( obj, option, &err );
					if ( !err.empty( ) ) {
						WriteErrors( __log, err );
						err.clear( );
						break;
					} else {
						id = ( udUInt ) db->GetInsertId( );
						obj->get( "id" )->ini( id );
						arr1.push_back( obj );
						handler->AddUdObject( arr, option, arrQuery );
						arr1.clear( );
						obj->get( "id_old" )->ini( id_old );
					}
				} else { // записываем ID уже существующего объекта
					obj->get( "id_old" )->ini( obj->get( "id" )->toUInt( ) );
					obj->get( "id" )->ini( arr1[ 0 ]->get( "id" )->toUInt( ) );
					ClearArrBase( arr1 );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			// выгребаем временные объекты и переписываем у них ID связей
			for( vctmerge::const_iterator i = arrMerge.begin( ); i != arrMerge.end( ); ++i ) {
				arr1.clear( );
				option.Clear( );
				option.SetTable( i->table );
				option.SetClass( i->fnGetObject );
				tmphandler->GetUdObject( arr1, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
				} else if ( !arr1.empty( ) ) {
					udPBase	obj1 = NULL;
					for( udArrBase::iterator j = arr1.begin( ); j != arr1.end( ); ++j ) {
						obj = *j;
						id = obj->get( i->attr )->toUInt( );
						for( udArrBase::iterator k = arr.begin( ); k != arr.end( ); ++k ) {
							obj1 = *k;
							if ( id == obj1->get( "id_old" )->toUInt( ) ) {
								obj->get( i->attr )->ini( obj1->get( "id" )->toUInt( ) );
								break;
							}
						}
					}
					ClearArrBase( arr1 );
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			ClearArrBase( arr );
		}
	} // void MergeSimpleObjectsMultiple

	void CAppInitProject::LessClientMode( udPSocket sock, udDWord dwClientVersion, udAscBase& ascClient, udAscBase& ascServer ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::LessClientMode\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		// пишем команду
		udBinaryData data;
		data.add( ( char ) 4 );
		data.add( string( "#database" ) );
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		data.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// собираем сущности
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udArrBase arr;
		udPBase obj = NULL;
		udBaseConfig config;
		string szWhere = "", szOrder = "";

		obj = new udDatabaseTableQuery;
		obj->GetConfig( config );
		delete obj;
		szWhere = config.GetAttrIndex( "version" ) + ">=" + toString( dwClientVersion );
		szOrder = config.GetAttrIndex( "id" ) + " ASC";
		option.SetTable( "ud_syncquery" );
		option.SetClass( GetDatabaseTableQuery );
		option.SetWhere( szWhere );
		option.SetOrder( szOrder );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			for( udArrError::iterator i = err.begin( );  i != err.end( ); ++i ) {
				__log->Write( i->GetText( ) + "\n" );
			}
			err.clear( );
		} else {
			WriteInstances( arr, data );
			/*string log = "\nзапросы для клиента:\n";
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				obj->dump( log );
				obj->dump( data );
				log += "\n";
			}
			g_pApp->GetLog( )->Write( log );*/
			ClearArrBase( arr );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		data.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// ждем ответа по каждой порции данных
		char code = 0;
		udInt read = 0, cnt = 0, cnt80 = 0;
		bool finished = false;
		while( !finished ) {
			read = sock->ReadByte( &code );
			switch( read ) {
				case SOCKET_ERROR:
					finished = true;
					g_pApp->GetLog( )->Write( "была сгенерирована ошибка\n" );
					break;

				case 0:
					finished = true;
					g_pApp->GetLog( )->Write( "было закрыто соединение\n" );
					break;

				default:
					if ( code == 80 ) {
						++cnt80;
					}
					++cnt;
					break;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LessClientMode

	void CAppInitProject::FeedServerMode( udPSocket sock, udDWord dwClientVersion, udAscBase& ascClient, udAscBase& ascServer ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::FeedServerMode\n" );
		udBinaryData data;
		data.add( ( char ) 1 );
		data.add( string( "#database" ) );
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		data.clear( );
		/*udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		wstring text = L"";
		udArrBase arr;
		for( _asc_tables::iterator i = m_arrTables.begin( ); i != m_arrTables.end( ); ++i ) {
			LoadInstancesFromSocket( sock, i->first, i->second, arr );
			option.SetTable( i->first );
			option.SetClass( i->second );
			handler->AddUdObject( arr, option );
			text += toWString( i->first ) + L" " + toWString( arr.size( ) ) + L"\r\n";
			ClearArrBase( arr );
		}
		g_pApp->GetWndServer( )->UpdateText( text );*/
	} // void FeedServerMode

	void CAppInitProject::SendMetadata( udPSocket sock ) {
		udBinaryData data;
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		data.clear( );
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
	} // void SendMetadata

	void CAppInitProject::CollectInstances( udBinaryData& data, const string& name ) {
		_asc_tables::iterator itTable = m_arrTables.find( name );
		if ( itTable != m_arrTables.end( ) ) {
			udPHandler		handler = g_pApp->GetHandler( );
			udHandlerOption	option;
			udArrError		err;
			udArrBase		arr;

			option.SetTable( name );
			option.SetClass( itTable->second );
			handler->GetUdObject( arr, option, &err );
			if ( err.empty( ) ) {
				g_pApp->GetLog( )->Write( "сущности таблицы: '" + name + "'\r\n" );
				WriteInstances( arr, data );
			} else {
				udPLog log = g_pApp->GetLog( );
				for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
					log->Write( i->GetText( ) + "\r\n" );
				}
				err.clear( );
			}
			ClearArrBase( arr );
		}
	} // void CollectInstances



	CHMainPage::CHMainPage( ) {
	}

	CHMainPage::CHMainPage( const CHMainPage& objCopy ) {
	}

	CHMainPage::~CHMainPage( ) {
	}

	CHMainPage& CHMainPage::operator =( const CHMainPage& objCopy ) {
		return *this;
	}

	void CHMainPage::Init( ) {
	} // void Init

	void CHMainPage::InitSync( udPDatabaseSync sync ) {
	} // void InitSync

	void CHMainPage::ProcRequest( udPSocket sock ) {
	} // void ProcRequest

} // namespace UdSDK
