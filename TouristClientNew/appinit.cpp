
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CAppInitProject::CAppInitProject( ) :
		udAppInit( ),
		m_imgTour( "./images/addtour.png" ),
		m_imgOrder( "./images/addorder.png" ),
		m_imgRequest( "./images/addrequest.png" ),
		m_imgEdit( "./images/edit_active.png" ),
		m_imgDel( "./images/rec_active.png" )
	{
	} // CAppInitProject

	CAppInitProject::CAppInitProject( const CAppInitProject& objCopy ) : udAppInit( ) {
	} // CAppInitProject

	CAppInitProject::~CAppInitProject( ) {
	} // ~CAppInitProject

	CAppInitProject& CAppInitProject::operator =( const CAppInitProject& objCopy ) {
		return *this;
	} // CAppInitProject& operator =

	void CAppInitProject::InitToolbar( ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::InitToolbar, enter\n" );

		udPWndToolbar pToolbar = g_pApp->GetWndWork( )->GetWndToolbar( );
		Gtk::Button* btn = NULL;

		btn = pToolbar->add( Glib::locale_to_utf8( "Создать\nтур" ), "tour_form", 15 );
		btn->set_image_position( Gtk::POS_TOP );
		btn->set_image( m_imgTour );
		m_imgTour.show( );

		btn = pToolbar->add( Glib::locale_to_utf8( "Создать\nзаявку" ), "order_form", 15 );
		btn->set_image_position( Gtk::POS_TOP );
		btn->set_image( m_imgOrder );
		m_imgOrder.show( );

		btn = pToolbar->add( Glib::locale_to_utf8( "Создать\nобращение" ), "request_form", 15 );
		btn->set_image_position( Gtk::POS_TOP );
		btn->set_image( m_imgRequest );
		m_imgRequest.show( );

		btn = pToolbar->add( Glib::locale_to_utf8( "Редактировать" ), "", 15 );
		btn->set_sensitive( false );
		btn->set_image_position( Gtk::POS_TOP );
		btn->set_image( m_imgEdit );
		m_imgEdit.show( );

		btn = pToolbar->add( Glib::locale_to_utf8( "Удалить" ), "", 15 );
		btn->set_sensitive( false );
		btn->set_image_position( Gtk::POS_TOP );
		btn->set_image( m_imgDel );
		m_imgDel.show( );

		g_pApp->GetLog( )->Write( "CAppInitProject::InitToolbar, exit\n" );
	} // void InitToolbar

	void CAppInitProject::InitMenu( ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::InitMenu, enter\n" );

		udPWndMenu pMenu = g_pApp->GetWndWork( )->GetWndMenu( );
		Gtk::Button* btn = NULL;

		btn = pMenu->add( Glib::locale_to_utf8( "Заявки" ), "order" );

		g_pApp->GetWndWork( )->set_focus( *btn );

		pMenu->add( Glib::locale_to_utf8( "Обращения"		), "request"	);
		pMenu->add( Glib::locale_to_utf8( "Туры"			), "tour"		);
		pMenu->add( Glib::locale_to_utf8( "Шаблоны туров"	), "tpltour"	);
		pMenu->add( Glib::locale_to_utf8( "Группы"			), "group"		);
		pMenu->add_sep( 33 );

		btn = pMenu->add( Glib::locale_to_utf8( "Лицевые счета" ), "persacc" );
		btn->set_name( "TouristMenuPersAcc" );

		btn = pMenu->add( Glib::locale_to_utf8( "Клиенты" ), "client" );
		btn->set_name( "TouristMenuClient" );

		btn = pMenu->add( Glib::locale_to_utf8( "Справочники" ), "reference" );
		btn->set_name( "TouristMenuReference" );

		pMenu->add_sep( 33 );

		udInt rank = g_pApp->GetUser( )->get( "rank" )->toInt( );

		if ( ( rank == udProjectUser::USERRANK_ADMINISTRATOR ) ||
			( rank == udProjectUser::USERRANK_ACCOUNTANT ) ||
			( rank == udProjectUser::USERRANK_CHIEF ) ||
			( rank == udProjectUser::USERRANK_SUPERADMIN )
		) {
			pMenu->add( Glib::locale_to_utf8( "Отчеты"			), "report"		);
		}
		pMenu->add( Glib::locale_to_utf8( "Логи системы"	), "log"		);
		pMenu->add( Glib::locale_to_utf8( "Пользователи"	), "user"		);

		g_pApp->GetLog( )->Write( "CAppInitProject::InitMenu, exit\n" );
	} // void InitMenu

	void CAppInitProject::InitModel( ) {
		g_pApp->GetLog( )->Write( "CAppInitProject::InitModel, enter\n" );

		udPHandlerManager pManager = g_pApp->GetHandlerManager( );

		g_pApp->GetLog( )->Write( "CAppInitProject::InitModel, init list handlers\n" );
		// обработчики списков
		pManager->SetHandler( "main",			new udHMain							);	// главная
		pManager->SetHandler( "reference",		new udHReference					);	// справочники
		pManager->SetHandler( "order",			new udHOrderModeless				);	// заявки
		pManager->SetHandler( "request",		new udHRequestModeless				);	// обращения
		pManager->SetHandler( "tour",			new udHTourModeless					);	// туры
		pManager->SetHandler( "tpltour",		new udHTplTourModeless				);	// шаблоны туров
		pManager->SetHandler( "group",			new udHGroupModeless				);	// группы
		pManager->SetHandler( "user",			new udHUserModeless					);	// пользователи
		pManager->SetHandler( "log",			new udHLogModeless					);	// логи
		pManager->SetHandler( "persacc",		new udHPersAccModeless				);	// лицевые счета
		pManager->SetHandler( "company",		new udHCompanyModeless				);	// компании
		pManager->SetHandler( "partner",		new udHPartnerModeless				);	// партнеры
		pManager->SetHandler( "service",		new udHServiceModeless				);	// услуги
		pManager->SetHandler( "client",			new udHClientModeless				);	// клиенты
		pManager->SetHandler( "country",		new udHCountryModeless				);	// страны
		pManager->SetHandler( "tservice",		new udHTServiceModeless				);	// тип услуги
		pManager->SetHandler( "sorder",			new udHSOrderModeless				);	// состояние заявки
		pManager->SetHandler( "rrequest",		new udHRRequestModeless				);	// результат обращения
		pManager->SetHandler( "stour",			new udHSTourModeless				);	// состояние тура
		pManager->SetHandler( "trequest",		new udHTRequestModeless				);	// тип обращения
		pManager->SetHandler( "direct",			new udHDirectModeless				);	// направление
		pManager->SetHandler( "nameservice",	new udHNameServiceModeless			);	// наименование услуги
		pManager->SetHandler( "doctype",		new udHDocTypeModeless				);	// тип документа
		pManager->SetHandler( "srequest",		new udHSRequestModeless				);	// состояние обращения
		pManager->SetHandler( "muservice",		new udHMUServiceModeless			);	// единица измерения услуги
		pManager->SetHandler( "route",			new udHRouteModeless				);	// маршрут
		pManager->SetHandler( "transferpoint",	new udHTransferPointModeless		);	// пункт перехода
		pManager->SetHandler( "hostside",		new udHHostSideModeless				);	// принимающая сторона
		pManager->SetHandler( "clientur",		new udHClientUrModeless				);	// клиенты юр. лица
		pManager->SetHandler( "city",			new udHCityModeless					);	// город
		pManager->SetHandler( "currency",		new udHCurrencyModeless				);	// валюта
		pManager->SetHandler( "currencypair",	new udHCurrencyPairModeless			);	// валютная пара
		pManager->SetHandler( "touristlist",	new udHTouristListModeless			);	// список туристов
		pManager->SetHandler( "cldoc",			new udHClDocModeless				);	// документ клиента
		pManager->SetHandler( "partnerreq",		new udHPartnerReqModeless			);	// запрос партнеру
		pManager->SetHandler( "partneranswr",	new udHPartnerAnswrModeless			);	// ответ партнера
		pManager->SetHandler( "doctpl",			new udHDocTemplateModeless			);	// шаблон документа
		pManager->SetHandler( "doctplprinted",	new udHDocTemplatePrintedModeless	);	// распечатанный шаблон документа
		pManager->SetHandler( "tourpayorder",	new udHTourPayOrderModeless			);	// указание на оплату тура
		pManager->SetHandler( "report",			new udHReport						);	// Отчеты
		pManager->SetHandler( "report_landing",	new udHReportLandingModeless		);	// Отчет "Посадочная ведомость"
		pManager->SetHandler( "report_group_pay",	new udHReportGroupPayModeless		);	// Отчет "Оплата группы"
		pManager->SetHandler( "report_hotel_pay",	new udHReportHotelPayModeless		);	// Отчет "Оплата гостиниц"
		pManager->SetHandler( "report_day_pay",	new udHReportDayPayModeless		);	// Отчет "Оплата за день"

		g_pApp->GetLog( )->Write( "CAppInitProject::InitModel, init form handlers\n" );
		// обработчики форм
		pManager->SetHandler( "country_form",		new udHCountryFormModeless				);	// страна
		pManager->SetHandler( "direct_form",		new udHDirectFormModeless				);	// направление
		pManager->SetHandler( "tservice_form",		new udHTServiceFormModeless				);	// тип услуги
		pManager->SetHandler( "trequest_form",		new udHTRequestFormModeless				);	// тип обращения
		pManager->SetHandler( "doctype_form",		new udHDocTypeFormModeless				);	// тип документа
		pManager->SetHandler( "hostside_form",		new udHHostSideFormModeless				);	// принимающая сторона
		pManager->SetHandler( "muservice_form",		new udHMUServiceFormModeless			);	// единица измерения услуг
		pManager->SetHandler( "nameservice_form",	new udHNameServiceFormModeless			);	// наименование услуг
		pManager->SetHandler( "route_form",			new udHRouteFormModeless				);	// маршрут
		pManager->SetHandler( "rrequest_form",		new udHRRequestFormModeless				);	// результат обращения
		pManager->SetHandler( "sorder_form",		new udHSOrderFormModeless				);	// состояние заявки
		pManager->SetHandler( "srequest_form",		new udHSRequestFormModeless				);	// состояние обращения
		pManager->SetHandler( "stour_form",			new udHSTourFormModeless				);	// состояние тура
		pManager->SetHandler( "transferpoint_form",	new udHTransferPointFormModeless		);	// пункт перехода
		pManager->SetHandler( "currency_form",		new udHCurrencyFormModeless				);	// валюта
		pManager->SetHandler( "clientur_form",		new udHClientUrFormModeless				);	// клиент юр. лица
		pManager->SetHandler( "company_form",		new udHCompanyFormModeless				);	// компании
		pManager->SetHandler( "user_form",			new udHUserFormModeless					);	// пользователи
		pManager->SetHandler( "partner_form",		new udHPartnerFormModeless				);	// партнеры
		pManager->SetHandler( "city_form",			new udHCityFormModeless					);	// города
		pManager->SetHandler( "partneranswr_form",	new udHPartnerAnswrFormModeless			);	// ответ партнера
		pManager->SetHandler( "partnerreq_form",	new udHPartnerReqFormModeless			);	// запрос партнеру
		pManager->SetHandler( "currencypair_form",	new udHCurrencyPairFormModeless			);	// валютные пары
		pManager->SetHandler( "request_form",		new udHRequestFormModeless				);	// обращение
		pManager->SetHandler( "cldoc_form",			new udHClDocFormModeless				);	// документ клиента
		pManager->SetHandler( "client_form",		new udHClientFormModeless				);	// клиент
		pManager->SetHandler( "order_form",			new udHOrderFormModeless				);	// заявки
		pManager->SetHandler( "service_form",		new udHServiceFormModeless				);	// услуги
		pManager->SetHandler( "tpltour_form",		new udHTplTourFormModeless				);	// шаблоны туров
		pManager->SetHandler( "group_form",			new udHGroupFormModeless				);	// группы
		pManager->SetHandler( "tour_form",			new udHTourFormModeless					);	// туры
		pManager->SetHandler( "doctpl_form",		new udHDocTemplateFormModeless			);	// шаблоны документов
		pManager->SetHandler( "doctplprinted_form",	new udHDocTemplatePrintedFormModeless	);	// распечатанный шаблоны документов
		pManager->SetHandler( "touristlist_form",	new udHTouristListFormModeless			);	// список туристов
		pManager->SetHandler( "persacc_form",		new udHPersAccFormModeless				);	// лицевой счет
		pManager->SetHandler( "tourpayorder_form",	new udHTourPayOrderFormModeless			);	// указание на оплату тура

		pManager->SetDefaultHandler( "main" );
		pManager->Init( );

		g_pApp->GetLog( )->Write( "CAppInitProject::InitModel, exit\n" );
	} // void InitModel

} // namespace UdSDK
