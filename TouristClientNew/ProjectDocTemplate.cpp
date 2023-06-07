
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHDocTemplateListPick( ) {
		return new CHDocTemplatePick;
	} // udPListHandlerPick GetHDocTemplateListPick

	udPBase GetProjectDocTemplateList( ) {
		udPBase ret = GetProjectDocTemplate( );
		return ret;
	} // udPBase GetProjectDocTemplateList

	CHDocTemplate::CHDocTemplate( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "doctpl";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать шаблон документа" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHDocTemplate::ColModelDocTemplate( );

		m_pData->SetName( "doctpl" );
		m_pData->SetGetObject( GetProjectDocTemplateList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHDocTemplate

	CHDocTemplate::CHDocTemplate( const CHDocTemplate& objCopy ) : udListHandler( ) {
		m_szName = "doctpl";
	} // CHDocTemplate

	CHDocTemplate::~CHDocTemplate( ) {
	} // ~CDocTemplate

	CHDocTemplate& CHDocTemplate::operator =( const CHDocTemplate& objCopy ) {
		return *this;
	} // CHDocTemplate& operator =

	void CHDocTemplate::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHDocTemplate::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		Gtk::TreeModel::iterator itRow;
		Gtk::TreePath objPath;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			itRow = m_objTreeModel->append( );
			objPath = m_objTreeModel->get_path( itRow );
			m_mapObject[ objPath.to_string( ) ] = obj;
			row = *itRow;
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
		}
	} // void Load

	void CHDocTemplate::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHDocTemplate::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	void CHDocTemplate::ProcContractTemplate( udPFormHandler hForm, udPBase pClient, udPBase pTpl, udPBase pClDoc, /*const wstring& szAmount,*/ udPArrBase pClients ) {
		assert( hForm );
		assert( pTpl );
		//////////////////////////////////////////////////////////////////////////////////////////
		wstring folder = g_pApp->GetUniqueTmpFolder( ), text = L"", szHead = L"";
		string out = "", log = "";
		udPBase pTour	= hForm->GetData( )->GetData( ),
				pApp1	= ( ( udPFDEObject ) hForm->GetData( )->GetExtraData( udHTourForm::ctrlidApp1 ) )->GetObject( ),
				obj		= NULL;
		udPArrBase items = NULL;
		udPLog __log = g_pApp->GetLog( );
		udPSocket sock = NULL;
		wstring szAmount = L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		// загрузка услуг
		udArrBase arrService;
		( ( udPHTourForm ) hForm )->GetServicesPrices( arrService );

		log = "\nservices2: полученные услуги\n";
		for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );
		/*g_pApp->GetLog( )->Write( "CHDocTemplate::ProcContractTemplate, 1 загрузка услуг\n" );
		udArrBase arrService;
		map< udUInt, udUInt > ids;
		log = "\nservices:\n";
		items = ( ( udPFDEList ) hForm->GetData( )->GetExtraData( udHTourForm::ctrlidService ) )->GetArray( );
		for( udArrBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			ids[ ( *i )->get( "service" )->toUInt( ) ] = 1;
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );
		udHService::GetServicesById( ids, arrService );
		log = "\nservices2: полученные услуги\n";
		for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );*/
		//////////////////////////////////////////////////////////////////////////////////////////
		// генерируем xml
		datetime dcur, dt1 = pTour->get( "date1" )->toTime( ), dt2 = pTour->get( "date2" )->toTime( );
		time_t days = 0, nights = 0, t1 = dt1.get( ), t2 = dt2.get( );
		dcur.load( );
		//nights = ( time_t ) ceil( ( double ) ( ( double ) t2 - ( double ) t1 ) / ( double ) 86400 );
		nights = ( t2 - t1 ) / 86400;
		days = nights + 1;
		__log->Write(
			"date1=" + dt1.toString( "%Y-%m-%d %H-%M-%S" ) + ", "
			"date2=" + dt2.toString( "%Y-%m-%d %H-%M-%S" ) + ", "
			"t1=" + toString( t1 ) + ", "
			"t2=" + toString( t2 ) + ", "
			"diff=" + toString( t2 - t1 ) + ", "
			"nights=" + toString( nights ) + ", "
			"days=" + toString( days ) + "\n"
		);
		szHead = L"<?xml version=\"1.0\"?><?xml-stylesheet type=\"text/xsl\" href=\"" + pTpl->get( "text" )->toWString( ) + L"\"?><tpldata>";
		text += L"<current_date date=\"" + dcur.toWString( "%d/%m/%Y" ) + L"\"/>";
		if ( pApp1 ) {
			szAmount = pApp1->get( "price" )->toWString( );
			text += L"<info>"
				L"<days1><![CDATA[" + pApp1->get( "days1" )->toWString( ) + L"]]></days1>"
				L"<days2><![CDATA[" + pApp1->get( "days2" )->toWString( ) + L"]]></days2>"
				L"<days3><![CDATA[" + pApp1->get( "days3" )->toWString( ) + L"]]></days3>"
				L"</info>";
		}
		text += L"<tour "
			L"days=\"" + toWString( days ) + L"\" "
			L"nights=\"" + toWString( nights ) + L"\" "
			L"date_begin=\"" + dt1.toWString( "%d/%m/%Y" ) + L"\" "
			L"date_end=\"" + dt2.toWString( "%d/%m/%Y" ) + L"\" "
			L"><name><![CDATA[" + pTour->get( "name" )->toWString( ) + L"]]></name>"
			L"<amount><![CDATA[" + szAmount + L"]]></amount>"
			L"</tour>";
		text += L"<company><name><![CDATA[" +
			( ( udPFDEObject ) hForm->GetData( )->GetExtraData( udHTourForm::ctrlidCompany ) )->GetObject( )->get( "name" )->toWString( )
			+ L"]]></name></company>";
		text += L"<user><fio><![CDATA[" +
			( ( udPFDEObject ) hForm->GetData( )->GetExtraData( udHTourForm::ctrlidUser ) )->GetObject( )->get( "fio" )->toWString( )
			+ L"]]></fio></user>";
		if ( pClient ) { // если распечатка группового договора, то клиент не турист не выбирается
			text += L"<tourist>"
				L"<fio><![CDATA[" + pClient->get( "fio" )->toWString( ) + L"]]></fio>"
				L"<email><![CDATA[" + pClient->get( "lemail" )->toWString( ) + L"]]></email>"
				L"<phone><![CDATA[" + pClient->get( "lphone" )->toWString( ) + L"]]></phone>"
				L"<addr><![CDATA[" + pClient->get( "laddr" )->toWString( ) + L"]]></addr>"
				L"</tourist>";
		}

		obj = ( ( udPFDEObject ) hForm->GetData( )->GetExtraData( udHTourForm::ctrlidEmployer ) )->GetObject( );
		text += L"<employer>"
			L"<fio><![CDATA[" + ( obj->isset( "fio" ) ? obj->get( "fio" )->toWString( ) : L"" ) + L"]]></fio>"
			L"<name><![CDATA[" + ( obj->isset( "name" ) ? obj->get( "name" )->toWString( ) : L"" ) + L"]]></name>"
			L"<email><![CDATA[" + obj->get( "lemail" )->toWString( ) + L"]]></email>"
			L"<phone><![CDATA[" + obj->get( "lphone" )->toWString( ) + L"]]></phone>"
			L"<addr><![CDATA[" + obj->get( "laddr" )->toWString( ) + L"]]></addr>"
			L"</employer>";
		if ( pClDoc ) {
			text += L"<cldoc>"
				L"<type><![CDATA[" + pClDoc->get( "type_" )->toWString( ) + L"]]></type>"
				L"<series><![CDATA[" + pClDoc->get( "series" )->toWString( ) + L"]]></series>"
				L"<number><![CDATA[" + pClDoc->get( "number" )->toWString( ) + L"]]></number>"
				L"<issuedate><![CDATA[" + pClDoc->get( "issuedate" )->toWString( ) + L"]]></issuedate>"
				L"<issuedby><![CDATA[" + pClDoc->get( "issuedby" )->toWString( ) + L"]]></issuedby>"
				L"<birthplace><![CDATA[" + pClDoc->get( "birthplace" )->toWString( ) + L"]]></birthplace>"
				L"<enddate><![CDATA[" + pClDoc->get( "enddate" )->toWString( ) + L"]]></enddate>"
				L"<fio><![CDATA[" + pClDoc->get( "fio" )->toWString( ) + L"]]></fio>"
				L"</cldoc>";
		}
		text += L"<services>";
		udPBase service = NULL;
		wstring name = L"", type_name = L"";
		for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
			service = *i;
			name = service->get( "name_" )->toWString( );
			type_name = service->get( "type_" )->toWString( );
			text += wstring( L"<service live=\"" ) + ( type_name == L"Гостиница" ? L"1" : L"0" ) + L"\" " +
				L"trans=\"" + ( type_name == L"Внутренний трансфер" ? L"1" : L"0" ) + L"\">"
				L"<name><![CDATA[" + name + L"]]></name>"
				L"<num>" + service->get( "num" )->toWString( ) + L"</num>"
				L"<amount>" + service->get( "amount" )->toWString( ) + L"</amount>"
				L"<currency><![CDATA[" + service->get( "currency_" )->toWString( ) + L"]]></currency>"
				L"<amount_recommend>" + service->get( "amount_recommend" )->toWString( ) + L"</amount_recommend>"
				L"<currency_recommend><![CDATA[" + service->get( "currency_recommend_" )->toWString( ) + L"]]></currency_recommend>"
				L"</service>";
		}
		ClearArrBase( arrService );
		text += L"</services>";
		//////////////////////////////////////////////////////////////////////////////////////////
		map< udUInt, udUInt > idsCity;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		text += L"<route>";
		items = ( ( udPFDEList ) hForm->GetData( )->GetExtraData( udHTourForm::ctrlidRoute ) )->GetArray( );
		for( udArrBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			id = ( *i )->get( "city" )->toUInt( );
			idsCity[ id ] = id;
			text += L"<city><name><![CDATA[" + ( *i )->get( "city_" )->toWString( ) + L"]]></name></city>";
		}
		text += L"</route>";
		udPArrBase pArrCities = udHCity::LoadCitiesById( idsCity );
		if ( pArrCities ) {
			__log->Write( "pArrCities->size()=" + toString( pArrCities->size( ) ) + "\n" );
			__log->Write( "items->size()=" + toString( items->size( ) ) + "\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			udPBase city = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			text += L"<route2>";
			for( udArrBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				id = ( *i )->get( "city" )->toUInt( );
				__log->Write( "id=" + toString( id ) + "\n" );
				for( udArrBase::iterator j = pArrCities->begin( ); j != pArrCities->end( ); ++j ) {
					city = *j;
					__log->Write( "citi->id=" + city->get( "id" )->toString( ) + "\n" );
					if ( id == city->get( "id" )->toUInt( ) ) {
						text += L"<city>"
							L"<name><![CDATA[" + city->get( "name" )->toWString( ) + L"]]></name>"
							L"<country><![CDATA[" + city->get( "country_" )->toWString( ) + L"]]></country>"
							L"</city>";
						break;
					}
				}
			}
			text += L"</route2>";
			//////////////////////////////////////////////////////////////////////////////////////
			udUInt iLastCountry = 0, iCurCountry = 0;;
			text += L"<route3>";
			for( udArrBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				id = ( *i )->get( "city" )->toUInt( );
				for( udArrBase::iterator j = pArrCities->begin( ); j != pArrCities->end( ); ++j ) {
					city = *j;
					if ( id == city->get( "id" )->toUInt( ) ) {
						iCurCountry = city->get( "country" )->toUInt( );
						if ( ( iLastCountry == 0 ) || ( iCurCountry != iLastCountry ) ) {
							text += L"<country><![CDATA[" + city->get( "country_" )->toWString( ) + L"]]></country>";
						}
						iLastCountry = iCurCountry;
						break;
					}
				}
			}
			text += L"</route3>";
			//////////////////////////////////////////////////////////////////////////////////////
			for( udArrBase::iterator i = pArrCities->begin( ); i != pArrCities->end( ); ++i ) {
				udDELETE( *i );
			}
			//////////////////////////////////////////////////////////////////////////////////////
		}
		udDELETE( pArrCities );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( pTpl->get( "type" )->toInt( ) == udProjectDocTemplate::tplContract2 ) {
			if ( pApp1 ) {
				text += L"<app1>"
					L"<pay1><![CDATA[" + pApp1->get( "pay1" )->toWString( ) + L"]]></pay1>"
					L"<pay1date><![CDATA[" + pApp1->get( "pay1date" )->toWString( ) + L"]]></pay1date>"
					L"<pay2><![CDATA[" + pApp1->get( "pay2" )->toWString( ) + L"]]></pay2>"
					L"<pay2date><![CDATA[" + pApp1->get( "pay2date" )->toWString( ) + L"]]></pay2date>";
				//////////////////////////////////////////////////////////////////////////////////////////
				udPArrBase pArr = NULL;
				udPBase pObj = NULL;
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "occupancy" )->get( );
				text += L"<occupancy_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<occupancy>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"<category><![CDATA[" + pObj->get( "category" )->toWString( ) + L"]]></category>"
						L"<amount><![CDATA[" + pObj->get( "amount" )->toWString( ) + L"]]></amount>"
						L"<feed><![CDATA[" + pObj->get( "feed" )->toWString( ) + L"]]></feed>"
						L"</occupancy>";
				}
				text += L"</occupancy_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "program" )->get( );
				text += L"<program_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<program>"
						L"<text><![CDATA[" + pObj->get( "text" )->toWString( ) + L"]]></text>"
						L"</program>";
				}
				text += L"</program_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "transfer" )->get( );
				text += L"<transfer_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<transfer>"
						L"<fio><![CDATA[" + pObj->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<trans><![CDATA[" + pObj->get( "trans" )->toWString( ) + L"]]></trans>"
						L"<level><![CDATA[" + pObj->get( "level" )->toWString( ) + L"]]></level>"
						L"<date1><![CDATA[" + pObj->get( "date1" )->toWString( ) + L"]]></date1>"
						L"<date2><![CDATA[" + pObj->get( "date2" )->toWString( ) + L"]]></date2>"
						L"</transfer>";
				}
				text += L"</transfer_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "cover" )->get( );
				text += L"<cover_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<cover>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"</cover>";
				}
				text += L"</cover_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "visa" )->get( );
				text += L"<visa_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<visa>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"</visa>";
				}
				text += L"</visa_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "service" )->get( );
				text += L"<service_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<service>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"<char><![CDATA[" + pObj->get( "char" )->toWString( ) + L"]]></char>"
						L"</service>";
				}
				text += L"</service_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "transfer2" )->get( );
				text += L"<transfer2_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<transfer2>"
						L"<route><![CDATA[" + pObj->get( "route" )->toWString( ) + L"]]></route>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"</transfer2>";
				}
				text += L"</transfer2_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				text += L"</app1>";
				//////////////////////////////////////////////////////////////////////////////////////////
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPHTourFinanceForm wndFinance = ( ( udPHTourForm ) hForm )->GetFinanceForm( );
		if ( wndFinance ) {
			udPArrBase arrObject = ( ( udPFDEList ) wndFinance->GetData( )->GetExtraData( udHTourFinanceForm::ctrlidFinal ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				text += L"<finance>";

				// итоговая цена
				text += L"<final>";
				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					text += L"<price>"
						L"<summ>" + ( *i )->get( "summ" )->toWString( L"%.2f" ) + L"</summ>"
						L"<currency><![CDATA[" + ( *i )->get( "currency_" )->toWString( ) + L"]]></currency>"
						L"</price>";

				}
				text += L"</final>";

				text += L"</finance>";
			} else {
				__log->Write( "final price is empty\n" );
			}
		} else {
			__log->Write( "finance form is NULL\n" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( pClients ) {
			text += L"<clients>";
			if ( pTour->get( "emptype" )->toInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
				udPBase client = NULL;
				udUInt iEmployerId = pTour->get( "employer" )->toUInt( );
				for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
					client = *i;
					text += L"<client";
					if ( iEmployerId == client->get( "id" )->toUInt( ) ) {
						text += L" employer=\"1\"";
					}
					text += L">";
					text +=	L"<fio><![CDATA[" + client->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<fiolatin><![CDATA[" + client->get( "fiolatin" )->toWString( ) + L"]]></fiolatin>"
						L"<born><![CDATA[" + client->get( "born" )->toWString( ) + L"]]></born>"
						L"<sex><![CDATA[" + client->get( "sex" )->toWString( ) + L"]]></sex>"
						L"<lemail><![CDATA[" + client->get( "lemail" )->toWString( ) + L"]]></lemail>"
						L"<lphone><![CDATA[" + client->get( "lphone" )->toWString( ) + L"]]></lphone>"
						L"<laddr><![CDATA[" + client->get( "laddr" )->toWString( ) + L"]]></laddr>"
						L"</client>";
				}
			} else {
				udPBase client = NULL;
				for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
					client = *i;
					text += L"<client>"
						L"<fio><![CDATA[" + client->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<fiolatin><![CDATA[" + client->get( "fiolatin" )->toWString( ) + L"]]></fiolatin>"
						L"<born><![CDATA[" + client->get( "born" )->toWString( ) + L"]]></born>"
						L"<sex><![CDATA[" + client->get( "sex" )->toWString( ) + L"]]></sex>"
						L"<lemail><![CDATA[" + client->get( "lemail" )->toWString( ) + L"]]></lemail>"
						L"<lphone><![CDATA[" + client->get( "lphone" )->toWString( ) + L"]]></lphone>"
						L"<laddr><![CDATA[" + client->get( "laddr" )->toWString( ) + L"]]></laddr>"
						L"</client>";
				}
			}
			text += L"</clients>";
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock = g_pApp->BeginRequest( );
		if ( sock ) {
			udPBase pObjWrite = NULL;
			char code = 0;
			string log = "";
			udBinaryData data;
			data.add( string( "doctpl" ) );
			data.add( ( udInt ) opidGetResult );
			//////////////////////////////////////////////////////////////////////////////////////
			pObjWrite = GetProjectTour( );
			pObjWrite->apply( pTour, true, true );
			pObjWrite->dump( data );
			udDELETE( pObjWrite );
			//////////////////////////////////////////////////////////////////////////////////////
			pObjWrite = GetProjectDocTemplate( );
			pObjWrite->apply( pTpl, true, true );
			pObjWrite->dump( data );
			udDELETE( pObjWrite );
			//////////////////////////////////////////////////////////////////////////////////////
			data.add( szHead );
			data.add( text );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) {
				//////////////////////////////////////////////////////////////////////////////////
				udType szret( wstring( L"" ) );
				szret.read( sock );
				szHead += szret.toWString( );
				//////////////////////////////////////////////////////////////////////////////////
			}
			//////////////////////////////////////////////////////////////////////////////////////
			// закрываем соединение
			sock->Shutdown( );
			sock->Close( );
			udDELETE( sock );
		}
		text = szHead + text + L"</tpldata>";
		out = ConvertString( text, CP_UTF8 );
		WriteToFile( folder + L"\\out.xml", out );
		out.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// загружаем xsl
		sock = g_pApp->BeginRequest( );
		if ( sock ) {
			char code = 0;
			string log = "";
			udBinaryData data;
			data.add( string( "doctpl" ) );
			data.add( ( udInt ) opidGetFile );
			data.add( pTpl->get( "id" )->toUInt( ) );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->ReadByte( &code );
			if ( code > 0 ) {
				udInt errCode = 0;
				udFileTransfer objFileTransfer;
				// очищаем временную папку от левых файлов
				objFileTransfer.SetTmpDir( folder );
				errCode = objFileTransfer.Recv( sock );
				log = udFileTransfer::ErrorExplain( errCode ) + "\n";
				__log->Write( log );
			}
			//////////////////////////////////////////////////////////////////////////////////////
			// закрываем соединение
			sock->Shutdown( );
			sock->Close( );
			udDELETE( sock );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		ExtOpenFile( folder + L"\\out.xml" );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ProcContractTemplate

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTemplateModeless::CHDocTemplateModeless( ) :
		udListHandlerModeless( ),
		CHDocTemplate( )
	{
	} // CHDocTemplateModeless

	CHDocTemplateModeless::CHDocTemplateModeless( const CHDocTemplateModeless& objCopy ) : CHDocTemplate( ) {
	} // CHDocTemplateModeless

	CHDocTemplateModeless::~CHDocTemplateModeless( ) {
	} // ~CHDocTemplateModeless

	CHDocTemplateModeless& CHDocTemplateModeless::operator =( const CHDocTemplateModeless& objCopy ) {
		return *this;
	} // CHDocTemplateModeless& operator =

	void CHDocTemplateModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHDocTemplate::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHDocTemplateModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "doctpl_form" );
	} // void OnButtonForm

	void CHDocTemplateModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "doctpl_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "doctpl_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTemplatePick::CHDocTemplatePick( ) : CHDocTemplate( ) {
	} // CHDocTemplatePick

	CHDocTemplatePick::CHDocTemplatePick( const CHDocTemplatePick& objCopy ) : CHDocTemplate( ) {
	} // CHDocTemplatePick

	CHDocTemplatePick::~CHDocTemplatePick( ) {
	} // ~CHDocTemplatePick

	CHDocTemplatePick& CHDocTemplatePick::operator =( const CHDocTemplatePick& objCopy ) {
		return *this;
	} // CHDocTemplatePick& operator =

	void CHDocTemplatePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHDocTemplate::Init( );
	} // void CHDocTemplatePick

	void CHDocTemplatePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHDocTemplateFormModal handler = new udHDocTemplateFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание шаблона документа" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание шаблона документа" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHDocTemplatePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectDocTemplate( );
				m_pPickedObject->apply( itObj->second, true, true );
			}
			
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите шаблон документа" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( Glib::locale_to_utf8(
				"Щелкните по строке списка и нажмите кнопку \"ОК\""
			) );
			dialog.run( );
		}
	} // void OnButtonOk

	void CHDocTemplatePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHDocTemplatePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHDocTemplateFormModal handler = new udHDocTemplateFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование шаблона документа" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование шаблона документа" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/
			udDELETE( handler );

			Clear( );
			Load( );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPFormHandler GetHDocTemplateForm( ) {
		return new CHDocTemplateFormModal;
	} // udPFormHandler GetHDocTemplateForm

	CHDocTemplateForm::CHDocTemplateForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Шаблон документа" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblFile( Glib::locale_to_utf8( "Файл" ) ),
		m_inpType( udProjectDocTemplate::tplContract, udProjectDocTemplate::tplContract )
	{
		m_szName = "doctpl_form";

		m_pData->SetName( "doctpl" );
		m_pData->SetGetObject( GetProjectDocTemplate );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpType.AddRow( udProjectDocTemplate::tplContract, Glib::locale_to_utf8( "Договор" ) );
		m_inpType.AddRow( udProjectDocTemplate::tplPermit, Glib::locale_to_utf8( "Путевка" ) );
		m_inpType.AddRow( udProjectDocTemplate::tplContract2, Glib::locale_to_utf8( "Групповой договор" ) );
		m_inpType.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEFile pFde = new udFDEFile;
		pFde->SetMark( ctrlidFile );
		pFde->SetFolder( g_pApp->GetUniqueTmpFolder( ) );
		m_pData->AddExtraData( pFde );
		m_inpFile.SetDataObject( pFde );
		m_inpFile.SetWindowTitle( Glib::locale_to_utf8( "Выбор файла" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHDocTemplateForm

	CHDocTemplateForm::CHDocTemplateForm( const CHDocTemplateForm& objCopy ) : udFormHandler( ) {
	} // CHDocTemplateForm

	CHDocTemplateForm::~CHDocTemplateForm( ) {
	} // ~CHDocTemplateForm

	CHDocTemplateForm& CHDocTemplateForm::operator =( const CHDocTemplateForm& objCopy ) {
		return *this;
	} // CHDocTemplateForm& operator =

	void CHDocTemplateForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Файл
		m_tblInput.attach( m_lblFile, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFile, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHDocTemplateForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		//m_inpType.set_active( 0 );
		m_inpType.Reset( );
		m_inpFile.SetTitle( "" );
	} // void Clear

	void CHDocTemplateForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		if ( obj->get( "type" )->toInt( ) > 0 ) {
			//m_inpType.set_active( obj->get( "type" )->toInt( ) - 1 );
			m_inpType.SetValue( obj->get( "type" )->toUInt( ) - 1 );
		}
		m_inpFile.SetTitle( Glib::locale_to_utf8(
			ConvertString( ( ( udPFDEFile ) m_pData->GetExtraData( ctrlidFile ) )->GetName( ), CP_UTF8 )
		) );
	} // void Load

	udInt CHDocTemplateForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;
		wstring szFileName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName ).GetEntry( ) );
			return 1;
		}

		szFileName = ( ( udPFDEFile ) m_pData->GetExtraData( ctrlidFile ) )->GetName( );
		if ( szFileName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите файл" ) );
			//g_pApp->GetWndWork( )->set_focus( *( m_inpFile.GetEn);
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHDocTemplateForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;
		
		szName = m_inpName.GetValue( );

		udUInt type = m_inpType.LoadValue( ).GetValue( );

		udPBase obj = m_pData->GetData( );
		
		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "type" )->ini( type );
		obj->get( "text" )->ini(
			( ( udPFDEFile ) m_pData->GetExtraData( ctrlidFile ) )->GetName( )
		);

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTemplateFormModal::CHDocTemplateFormModal( ) : CHDocTemplateForm( ) {
	} // CHDocTemplateFormModal

	CHDocTemplateFormModal::CHDocTemplateFormModal( const CHDocTemplateFormModal& objCopy ) : CHDocTemplateForm( ) {
	} // CHDocTemplateFormModal

	CHDocTemplateFormModal::~CHDocTemplateFormModal( ) {
	} // ~CHDocTemplateFormModal

	CHDocTemplateFormModal& CHDocTemplateFormModal::operator =( const CHDocTemplateFormModal& objCopy ) {
		return *this;
	} // CHDocTemplateFormModal& operator =

	void CHDocTemplateFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHDocTemplateFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHDocTemplateForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTemplateFormModeless::CHDocTemplateFormModeless( ) : CHDocTemplateForm( ) {
	} // CHDocTemplateFormModeless

	CHDocTemplateFormModeless::CHDocTemplateFormModeless( const CHDocTemplateFormModeless& objCopy ) : CHDocTemplateForm( ) {
	} // CHDocTemplateFormModeless

	CHDocTemplateFormModeless::~CHDocTemplateFormModeless( ) {
	} // ~CHDocTemplateFormModeless

	CHDocTemplateFormModeless& CHDocTemplateFormModeless::operator =( const CHDocTemplateFormModeless& objCopy ) {
		return *this;
	} // CHDocTemplateFormModeless& operator =

	void CHDocTemplateFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "doctpl" );
	} // void OnButtonOk

	void CHDocTemplateFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "doctpl" );
	} // void CHDocTemplateForm

	void CHDocTemplateFormModeless::Init( ) {
		CHDocTemplateForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHDocTemplateFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHDocTemplateFormModeless

} // namespace UdSDK
