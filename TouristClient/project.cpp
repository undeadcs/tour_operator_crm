
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void LoadInstancesFromSocket2( udPSocket sock, const string& table_name, pfnGetObject fnGetObject, udArrBase& arr ) {
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
	} // void LoadInstancesFromSocket2

	static void LoadInstancesFromSocket( udPSocket sock, pfnGetObject fnGetObject, udArrBase& arr ) {
		udBinaryData	data;						// данные для чтения
		udArrBase		arrClientObj;				// считанные объекты
		udPBase			obj		= NULL;				// объект для чтения
		udType			objNum( ( udUInt ) 0 );		// число объектов для чтения
		udUInt			cnt = 0,					// подсчет считанных объектов
						num = 0;					// заявленное количество
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
	}

	CAppInitProject::CAppInitProject( const CAppInitProject& objCopy ) {
	}

	CAppInitProject::~CAppInitProject( ) {
		m_arrTables.clear( );
	}

	CAppInitProject& CAppInitProject::operator =( const CAppInitProject& objCopy ) {
		return *this;
	}

	void CAppInitProject::LoadFonts( udPWndSplash pSplash ) {
		pSplash->SetState( L"Загрузка шрифтов .", 5.0f );
		Sleep( 100 );
		
		// загружаем шрифты от 8 до 24
		for( udUInt i = 8; i < 25; ++i ) {
			g_pApp->GetFont( L"Tahoma", i );
		}

		pSplash->SetState( L"Загрузка шрифтов ..", 10.0f );
		Sleep( 100 );

		// загружаем болженные шрифты от 8 до 17
		for( udUInt i = 8; i < 18; ++i ) {
			g_pApp->GetFont( L"Tahoma", i, FontStyleBold );
		}

		pSplash->SetState( L"Загрузка шрифтов ...", 15.0f );
		Sleep( 100 );
	} // void LoadFonts

	void CAppInitProject::LoadImages( udPWndSplash pSplash ) {
		pSplash->SetState( L"Загрузка изображений .", 20.0f );
		Sleep( 100 );
		pSplash->SetState( L"Загрузка изображений ..", 25.0f );
		Sleep( 100 );
		pSplash->SetState( L"Загрузка изображений ...", 30.0f );
		Sleep( 100 );
	} // void LoadImages

	void CAppInitProject::InitToolbar( udPWndSplash pSplash, udPWndToolbar pToolbar ) {
		udPWndToolbarBtn item = NULL;
		Rect rct;

		item = pToolbar->AddItem( "tour", L"Создать тур", true, false );
		rct = item->GetRectNormal( );
		rct.X = 47;
		rct.Y = 13;
		rct.Width = 33;
		rct.Height = 31;
		item->SetRectNormal( rct );
		item->SetBackX( 12 );
		item->SetBackY( 8 );
		item->get( "width" )->ini( ( udInt ) 54 );
		item->get( "padding-top" )->ini( ( udInt ) 40 );
		item->get( "margin-right" )->ini( ( udInt ) 52 );
		item->Create( pToolbar->GetHWnd( ), ( HMENU ) item->get( "#index" )->toDWord( ) );

		item = pToolbar->AddItem( "order", L"Создать заявку", true, false );
		rct = item->GetRectNormal( );
		rct.X = 151;
		rct.Y = 9;
		rct.Width = 31;
		rct.Height = 36;
		item->SetRectNormal( rct );
		item->SetBackX( 10 );
		item->SetBackY( 4 );
		item->get( "width" )->ini( ( udInt ) 56 );
		item->get( "padding-top" )->ini( ( udInt ) 40 );
		item->get( "margin-right" )->ini( ( udInt ) 42 );
		item->Create( pToolbar->GetHWnd( ), ( HMENU ) item->get( "#index" )->toDWord( ) );

		item = pToolbar->AddItem( "request", L"Создать обращение", true, false );
		rct = item->GetRectNormal( );
		rct.X = 253;
		rct.Y = 8;
		rct.Width = 33;
		rct.Height = 36;
		item->SetRectNormal( rct );
		item->SetBackX( 17 );
		item->SetBackY( 3 );
		item->get( "width" )->ini( ( udInt ) 76 );
		item->get( "padding-top" )->ini( ( udInt ) 40 );
		item->get( "margin-right" )->ini( ( udInt ) 26 );
		item->Create( pToolbar->GetHWnd( ), ( HMENU ) item->get( "#index" )->toDWord( ) );

		item = pToolbar->AddItem( "edit", L"Редактировать", false, false );
		rct = item->GetRectNormal( );
		rct.X = 376;
		rct.Y = 20;
		rct.Width = 30;
		rct.Height = 32;
		item->SetRectNormal( rct );
		rct = item->GetRectDisabled( );
		rct.X = 376;
		rct.Y = 73;
		rct.Width = 30;
		rct.Height = 32;
		item->SetRectDisabled( rct );
		item->SetBackX( 31 );
		item->SetBackY( 1 );
		item->get( "width" )->ini( ( udInt ) 94 );
		item->get( "padding-top" )->ini( ( udInt ) 40 );
		item->get( "margin-right" )->ini( ( udInt ) 21 );
		item->Create( pToolbar->GetHWnd( ), ( HMENU ) item->get( "#index" )->toDWord( ) );

		item = pToolbar->AddItem( "delete", L"Удалить", false, false );
		rct = item->GetRectNormal( );
		rct.X = 476;
		rct.Y = 20;
		rct.Width = 25;
		rct.Height = 32;
		item->SetRectNormal( rct );
		rct = item->GetRectDisabled( );
		rct.X = 476;
		rct.Y = 73;
		rct.Width = 30;
		rct.Height = 32;
		item->SetRectDisabled( rct );
		item->SetBackX( 16 );
		item->SetBackY( 1 );
		item->get( "width" )->ini( ( udInt ) 58 );
		item->get( "padding-top" )->ini( ( udInt ) 40 );
		item->Create( pToolbar->GetHWnd( ), ( HMENU ) item->get( "#index" )->toDWord( ) );

		pToolbar->ShowChildren( );
	} // void InitToolbar

	void CAppInitProject::InitMenu( udPWndSplash pSplash, udPWndMenu pMenu ) {
		udPWndMenuBtn pItem = NULL;
		Rect rct;

		pSplash->SetState( L"Инициализация меню", 40.0f );
		Sleep( 100 );

		// наполнеяем меню пунктами
		pMenu->AddItem( "order",	L"Заявки"			);
		pMenu->AddItem( "request",	L"Обращения"		);
		pMenu->AddItem( "tour",		L"Туры"				);
		pMenu->AddItem( "tpltour",	L"Шаблоны туров"	);
		pMenu->AddItem( "group",	L"Группы"			)->get( "margin-bottom" )->ini( ( udInt ) 33 );

		pItem = pMenu->AddItem( "persacc", L"Лицевые счета", false, false );
		rct = pItem->GetRectNormal( );
		rct.X = 685;
		pItem->SetRectNormal( rct );
		rct.X = 684;
		rct.Y = 89;
		pItem->SetRectHovered( rct );
		pItem->get( "color" )->ini( ( udDWord ) Color::MakeARGB( 255, 255, 255, 255 ) );
		pItem->Create( pMenu->GetHWnd( ), ( HMENU ) pItem->get( "#index" )->toDWord( ) );

		pItem = pMenu->AddItem( "client", L"Клиенты", false, false );
		rct = pItem->GetRectNormal( );
		rct.X = 918;
		pItem->SetRectNormal( rct );
		rct.Y = 89;
		pItem->SetRectHovered( rct );
		pItem->get( "color" )->ini( ( udDWord ) Color::MakeARGB( 255, 255, 255, 255 ) );
		pItem->Create( pMenu->GetHWnd( ), ( HMENU ) pItem->get( "#index" )->toDWord( ) );

		pItem = pMenu->AddItem( "#info", L"Справочники", false, false );
		rct = pItem->GetRectNormal( );
		rct.X = 1148;
		pItem->SetRectNormal( rct );
		rct.Y = 89;
		pItem->SetRectHovered( rct );
		pItem->get( "color" )->ini( ( udDWord ) Color::MakeARGB( 255, 255, 255, 255 ) );
		pItem->get( "margin-bottom" )->ini( ( udInt ) 33 );
		pItem->Create( pMenu->GetHWnd( ), ( HMENU ) pItem->get( "#index" )->toDWord( ) );

		pMenu->AddItem( "report",	L"Отчеты"			);
		pMenu->AddItem( "log",		L"Логи системы"		);
		pMenu->AddItem( "user",		L"Пользователи"		);

		pMenu->ShowChildren( );
	} // void InitMenu

	void CAppInitProject::InitSync( udPDatabaseSync sync ) {
		udPModelWork model = g_pApp->GetModel( );
		model->InitSync( );
	} // void InitSync

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

	void CAppInitProject::HandleSync( udPSocket sock, udAscBase& ascClient ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::HandleSync\n" );
/*
	пока не закрыли соединение или не послали команду пшел нах (0), пытаемся выгрести команду и данные
*/
		//////////////////////////////////////////////////////////////////////////////////////////
		// ждем команды сервера
		// 0 - выйти, 1 - отправить сущности, 2 - принять сущности
		char	cmd = 0;
		udType	type( string( "" ) );
		string	table_name = "";
		bool	finished = false;

		while( !finished ) {
			//////////////////////////////////////////////////////////////////////////////////////
			// считываем команду
			g_pApp->GetLog( )->Write( "ждем команду\n" );
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			if ( ( sock->ReadByte( &cmd ) != 1 ) || ( cmd == 0 ) ) {
				finished = true;
				break;
			}
			g_pApp->GetLog( )->Write( "приняли команду " + toString( cmd ) + "\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			// считываем таблицу
			if ( type.read( sock ) == 0 ) {
				finished = true;
				break;
			}
			g_pApp->GetLog( )->Write( "считали таблицу '" + type.toString( ) + "'\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			// обрабатываем команду
			if ( cmd == 0 ) {
				finished = true;
				break;
			} else if ( cmd == 1 ) { // пустой сервер
				this->FeedServerMode( sock );
				break;
			} else if ( cmd == 2 ) { // пустой клиент
				this->FeedClientMode( sock );
				break;
			} else if ( cmd == 3 ) { // поступила команда принять всю базу от сервера
				//this->ReadServerData( sock );
				break;
			} else if ( cmd == 4 ) {
				this->LessClientMode( sock );
				break;
			} else {
				g_pApp->GetLog( )->Write( "Поступила неизвестная команда\n" );
				finished = true;
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CAppInitProject::HandleSync exit\n" );
	} // void HandleSync

	void CAppInitProject::FeedServerMode( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::FeedServerMode\n" );
	} // void FeedServerMode

	void CAppInitProject::FeedClientMode( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::FeedClientMode\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		// сливаем базы с сервером
		udPHandler handler = g_pApp->GetHandler( );
		udPDatabase db = g_pApp->GetDatabase( );
		// отправляем клиентскую базу на сервак
		//this->SendDatabase( sock, handler );
		// обнуляем все таблицы
		this->TruncateTables( handler, db );
		// получаем данные от сервера
		this->ReadAndSaveDatabase( sock, handler );
		// получаем метаданные от сервера
		this->ReadAndSaveMetadata( sock, handler );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void FeedClientMode

	void CAppInitProject::ReadAndSaveMetadata( udPSocket sock, udPHandler handler ) {
		udArrBase meta;
		g_pApp->GetLog( )->Write( "//////////////////////////////////////////////////////////////////////////////////////////\n" );
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		ReadInstances( sock, meta, GetDatabaseSyncInfo );
		g_pApp->GetSync( )->UpdateVersion( meta );
		ClearArrBase( meta );
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
	} // void ReadAndSaveMetadata

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

	void CAppInitProject::LessClientMode( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::LessClientMode\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrBase arr, meta;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		// считываем запросы
		g_pApp->GetLog( )->Write( "//////////////////////////////////////////////////////////////////////////////////////////\n" );
		ReadInstances( sock, arr, GetDatabaseTableQuery );
		// считываем метаданные
		g_pApp->GetLog( )->Write( "//////////////////////////////////////////////////////////////////////////////////////////\n" );
		ReadInstances( sock, meta, GetDatabaseSyncInfo );
		//////////////////////////////////////////////////////////////////////////////////////////
		// отправляем подтверждающие байты
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
		sock->SendByte( ( char ) 80 );
		//////////////////////////////////////////////////////////////////////////////////////////
		// выполняем запросы
		udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
		string query = "", log = "";
		udPBase obj = NULL;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			query = obj->get( "query" )->toString( );
			log = query;
			//g_pApp->GetLog( )->Write( query + "\n" );
			if ( db->Query( query ) ) {
				log += ", выполнен\n";
			} else {
				log += ", провалился\n";
			}
			g_pApp->GetLog( )->Write( log );
		}
		// обновляем метаданные
		g_pApp->GetSync( )->UpdateVersion( meta );
		//////////////////////////////////////////////////////////////////////////////////////////
		ClearArrBase( arr );
		ClearArrBase( meta );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LessClientMode

	void CAppInitProject::InitModel( udPWndSplash pSplash, udPModelWork pModel ) {
		pSplash->SetState( L"Инициализация модели", 45.0f );
		Sleep( 100 );

		pModel->SetHandler( "request",		new udHRequest		);	// обращение
		pModel->SetHandler( "order",		new udHOrder		);	// заявка
		pModel->SetHandler( "tour",			new udHTour			);	// тур
		pModel->SetHandler( "tpltour",		new udHTplTour		);	// шаблон тура
		pModel->SetHandler( "group",		new udHGroup		);	// группа
		pModel->SetHandler( "#info",		new udHInfoPage		);	// справочники
		pModel->SetHandler( "country",		new udHCountry		);	// страна
		pModel->SetHandler( "tservice",		new udHTService		);	// тип услуги
		pModel->SetHandler( "sorder",		new udHSOrder		);	// статус заявки
		pModel->SetHandler( "rrequest",		new udHRRequest		);	// результат обращения
		pModel->SetHandler( "stour",		new udHSTour		);	// статус тура
		pModel->SetHandler( "trequest",		new udHTRequest		);	// тип обращения
		pModel->SetHandler( "muservice",	new udHMUService	);	// единица измерения услуги
		pModel->SetHandler( "direct",		new udHDirect		);	// направление
		pModel->SetHandler( "nameservice",	new udHNameService	);	// имя услуги
		pModel->SetHandler( "doctype",		new udHDocType		);	// тип документа
		pModel->SetHandler( "currency",		new udHCurrency		);	// валюта
		pModel->SetHandler( "user",			new udHUser			);	// пользователь
		pModel->SetHandler( "log",			new udHLog			);	// лог
		pModel->SetHandler( "city",			new udHCity			);	// город
		pModel->SetHandler( "currencypair",	new udHCurrencyPair	);	// валютная пара
		pModel->SetHandler( "company",		new udHCompany		);	// компания
		pModel->SetHandler( "cldoc",		new udHClDoc		);	// документы клиента
		pModel->SetHandler( "partner",		new udHPartner		);	// партнеры
		pModel->SetHandler( "partnerreq",	new udHPartnerReq	);	// запросы партнеру
		pModel->SetHandler( "partneranswr",	new udHPartnerAnswr	);	// ответ партнера
		pModel->SetHandler( "service",		new udHService		);	// услуга
		pModel->SetHandler( "client",		new udHClient		);	// клиент
		pModel->SetHandler( "srequest",		new udHSRequest		);	// состояние обращения
		pModel->SetHandler( "clientur",		new udHClientUr		);	// клиенты юр. лица
		pModel->SetHandler( "persacc",		new udHPersAcc		);	// лицевые счета
		pModel->SetHandler( "report",		new udHReport		);	// отчеты
		pModel->SetHandler( "doctpl",		new udHDocTemplate	);	// шаблоны документов
		pModel->SetHandler( "tourpayorder",	new udHTourPayOrder	);	// указание на оплату тура
		pModel->SetHandler( "doctplprinted",	new udHDocTemplatePrinted	);	// распечатанные документы
		pModel->SetHandler( "transferpoint",	new udHTransferPoint	);	// пункт перехода
		pModel->SetHandler( "route",		new udHRoute		);	// маршруты
		pModel->SetHandler( "hostside",		new udHHostSide		);	// принимающая сторона
		pModel->SetHandler( "touristlist",	new udHTouristList	);	// список туристов
		pModel->SetToolbarSwitcher( "tour",		"tour"		);
		pModel->SetToolbarSwitcher( "order",	"order"		);
		pModel->SetToolbarSwitcher( "request",	"request"	);
		pModel->SetMainHandler( new udHMainPage );

		pSplash->SetState( L"Инициализация модели .", 45.0f );
		Sleep( 100 );
		pModel->Init( );
	} // void InitModel

	udInt CAppInitProject::Login( udPWndLogin wnd, wstring login, wstring password ) {
		udInt ret = 0;
		if ( login.empty( ) ) {
			mb::err( wnd->GetHWnd( ), L"Введите имя пользователя (Логин)" );
		} else if ( password.empty( ) ) {
			mb::err( wnd->GetHWnd( ), L"Введите пароль" );
		/*} else if ( ( login == wstring( L"1" ) ) && ( password == wstring( L"1" ) ) ) {
			// тестовый режим
			udPSystemUser user = ( udPSystemUser ) GetProjectUser( );
			user->get( "id" )->ini( ( udUInt ) 0 );
			user->get( "login" )->ini( wstring( L"1" ) );
			user->get( "email" )->ini( wstring( L"test@test.ru" ) );
			user->get( "fio" )->ini( wstring( L"Тестовый пользователь" ) );
			user->get( "rank" )->ini( ( udInt ) 1000 );	// суперский юзверь
			g_pApp->SetUser( user );
			ret = 1;*/
		} else {
			g_pApp->GetLog( )->Write( "логинимся...\n" );
			bool show_error = false;
			udPSocket sock = BeginRequest( false );
			if ( sock ) {
				g_pApp->GetLog( )->Write( "подсоединились к серверу...\n" );
				// производим операции
				udBinaryData data;
				data.add( string( "" ) );
				data.add( login );
				data.add( password );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					// ждем ответа - ID сессии, данные клиента
					udType id( string( "" ) );
					id.read( sock );
					g_pApp->GetLog( )->Write( "получили ID сессии '" + id.toString( ) + "'\n" );
					udPBase client = GetProjectUser( );
					client->read( sock );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );
					m_dtConnect.load( );
					m_szSessionId = id.toString( );
					g_pApp->SetUser( client );
					ret = 1;
				} else {
					g_pApp->GetLog( )->Write( "не удалось залогиниться\n" );
					show_error = true;
				}
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось подсоединиться к серверу...\n" );
			}
			if ( show_error ) {
				mb::err( wnd->GetHWnd( ), L"Данный пользователь не существует или был введен неверный пароль." );
			}
			/*udArrBase			arrObject;
			//udHandler::optArray	arrOption;
			udPBase				pObject		= GetProjectUser( );
			udAscType			arrInput;
			string				szWhere		= "";
			udBaseConfig		objConfig;
			udHandlerOption		objOption;

			pObject->GetConfig( objConfig );

			arrInput[ "user_login"		] = new udType( login );
			arrInput[ "user_password"	] = new udType( password );
			
			pObject->Create( arrInput );

			szWhere = string( "`user_login`=" ) + pObject->GetAttrValue( "login", objConfig, udBase::UDSDK_CONFIG_DATABASE )->toString( );

			objOption.SetTable( "ud_user" );
			objOption.SetClass( GetProjectUser );
			objOption.SetWhere( szWhere );

			g_pApp->GetHandler( )->GetUdObject( arrObject, objOption );
			if ( arrObject.empty( ) ) {
				mb::err( wnd->GetHWnd( ), L"Пользователь не зарегистрирован в системе" );
			} else {
				udPSystemUser user = ( udPSystemUser ) arrObject[ 0 ];
				if ( user->get( "password" )->toWString( ) == password ) {
					g_pApp->SetUser( user );
					ret = 1;
				} else {
					mb::err( wnd->GetHWnd( ), L"Неверный пароль" );
					delete user;
				}
			}

			arrObject.clear( );
			ClearArrType( arrInput );
			delete pObject;*/
		}
		return ret;
	} // udInt Login

	udInt CAppInitProject::Login( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "отправляем ID сессии '" + m_szSessionId + "'\n" );
		udBinaryData data;
		data.add( m_szSessionId );
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
		if ( code == 80 ) {
			return 1;
		}
		return 0;
	} // udInt Login



	CHMainPage::CHMainPage( ) {
	}

	CHMainPage::CHMainPage( const CHMainPage& objCopy ) {
	}

	CHMainPage::~CHMainPage( ) {
	}

	CHMainPage& CHMainPage::operator =( const CHMainPage& objCopy ) {
		return *this;
	}

	void CHMainPage::Show( udPWndModel pWndModel ) {
	} // void Show

	void CHMainPage::Init( ) {
	} // void Init

	void CHMainPage::Clear( ) {
	} // void Clear

	void CHMainPage::Load( ) {
	} // void Load

	void CHMainPage::ClickToolbar( string index ) {
	} // void ClickToolbar

	udInt CHMainPage::ToolbarAction( string index ) {
		return 0;
	} // udInt ToolbarAction

	//_def_handler( HMainPage );
	_def_handler( Request );
	_def_handler( Order );
	_def_handler( Tour );
	_def_handler( TplTour );
	_def_handler( Group );
	//_def_handler( HInfoPage );
	_def_handler( Country );
	_def_handler( TService );
	_def_handler( SOrder );
	_def_handler( RRequest );
	_def_handler( STour );
	_def_handler( TRequest );
	_def_handler( MUService );
	_def_handler( Direct );
	_def_handler( NameService );
	_def_handler( DocType );
	_def_handler( Currency );
	_def_handler( User );
	_def_handler( Log );
	_def_handler( CurrencyPair );
	_def_handler( Company );
	_def_handler( ClDoc );
	_def_handler( Partner );
	_def_handler( PartnerReq );
	_def_handler( PartnerAnswr );
	_def_handler( Service );
	_def_handler( Client );
	_def_handler( SRequest );
	_def_handler( ClientUr );
	_def_handler( PersAcc );
	_def_handler( DocTemplate );
	_def_handler( TourPayOrder );
	_def_handler( City );

	defineFormHandler( OrderRoute );
	defineFormHandler( OrderService );
	defineFormHandler( TourService );
	_def_handler( ServicePeriod );
	defineFormHandler( ServicePrice );
	_def_handler( GroupPeriod );
	defineFormHandler( GroupTour );
	defineFormHandler( TplTourService );
	defineFormHandler( SummAdd );
	defineFormHandler( SummBack );
	defineFormHandler( SummConvert );
	defineFormHandler( TourPrice );
	//defineFormHandler( GroupContractF );
	defineFormHandler( GroupContractApp1 );
	defineFormHandler( GCTouristInfo );
	defineFormHandler( GCOccupancy );
	defineFormHandler( GCProgram );
	defineFormHandler( GCTransfer );
	defineFormHandler( GCCover );
	defineFormHandler( GCVisa );
	defineFormHandler( GCService );
	//defineFormHandler( GroupContractApp2F );
	defineFormHandler( GCTransfer2 );
	_def_handler( DocTemplatePrinted );
	_def_handler( TransferPoint );
	_def_handler( Route );
	_def_handler( HostSide );
	_def_handler( TouristList );
	//defineFormHandler( TouristListLink );

} // namespace UdSDK
