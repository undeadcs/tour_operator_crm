
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectServicePriceList( ) {
		udPBase ret = GetProjectPriceTableItem( );//GetProjectServicePrice( );
		ret->get( "currency_" )->ini( wstring( L"" ) );
		ret->get( "currency_recommend_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectServicePriceList

	CHServicePriceForm::CHServicePriceForm( udPHServiceForm pService ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Цена услуги" ) ),
		m_lblPeriod( Glib::locale_to_utf8( "Период" ) ),
		m_lblNum( Glib::locale_to_utf8( "Количество человек" ) ),
		m_lblAmount( Glib::locale_to_utf8( "Сумма" ) ),
		m_lblCurrency( Glib::locale_to_utf8( "Валюта" ) ),
		m_lblAmountRecommend( Glib::locale_to_utf8( "Сумма \"рекомендованная\"" ) ),
		m_lblCurrencyRecommend( Glib::locale_to_utf8( "Валюта \"рекомендованная\"" ) ),
		m_pService( pService )
	{
		m_szName = "serviceprice_form";

		m_pData->SetName( "serviceprice" );
		//m_pData->SetGetObject( GetProjectServicePrice );
		m_pData->SetGetObject( GetProjectPriceTableItem );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_lblPeriod.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblNum.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblAmount.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblCurrency.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblAmountRecommend.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblCurrencyRecommend.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectServicePeriod );
		pFde->SetMark( ctrlidPeriod );
		m_pData->AddExtraData( pFde );
		m_inpPeriod.SetDataObject( pFde );
		m_inpPeriod.SetWindowTitle( Glib::locale_to_utf8( "Выбор периода" ) );
		m_inpPeriod.SetAttr( "id" );
		m_inpPeriod.SetGetListHandler( GetHServicePeriodListPick );
		m_inpPeriod.SetServiceForm( m_pService );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrency );
		m_pData->AddExtraData( pFde );
		m_inpCurrency.SetDataObject( pFde );
		m_inpCurrency.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrency.SetAttr( "name" );
		m_inpCurrency.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrency.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrency.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrencyRecommend );
		m_pData->AddExtraData( pFde );
		m_inpCurrencyRecommend.SetDataObject( pFde );
		m_inpCurrencyRecommend.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrencyRecommend.SetAttr( "name" );
		m_inpCurrencyRecommend.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrencyRecommend.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrencyRecommend.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHServicePriceForm

	CHServicePriceForm::CHServicePriceForm( const CHServicePriceForm& objCopy ) : udFormHandler( ) {
	} // CHServicePriceForm

	CHServicePriceForm::~CHServicePriceForm( ) {
	} // ~CHServicePriceForm

	CHServicePriceForm& CHServicePriceForm::operator =( const CHServicePriceForm& objCopy ) {
		return *this;
	} // CHServicePriceForm& operator =

	void CHServicePriceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Период
		m_tblInput.attach( m_lblPeriod, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPeriod, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Количество человек
		m_tblInput.attach( m_lblNum, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNum, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Сумма
		m_tblInput.attach( m_lblAmount, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpAmount, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Валюта
		m_tblInput.attach( m_lblCurrency, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Сумма "рекомендованная"
		m_tblInput.attach( m_lblAmountRecommend, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpAmountRecommend, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Валюта "рекомендованная"
		m_tblInput.attach( m_lblCurrencyRecommend, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrencyRecommend, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

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

	void CHServicePriceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpPeriod.SetTitle( "" );
		m_inpNum.Reset( );
		m_inpAmount.Reset( );
		m_inpCurrency.SetTitle( "" );
		m_inpAmountRecommend.Reset( );
		m_inpCurrencyRecommend.SetTitle( "" );
	} // void Clear

	void CHServicePriceForm::Load( ) {
	} // void Load

	udInt CHServicePriceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szNum,
						szAmount;

		udPBase obj = NULL;

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPeriod ) )->GetObject( );
		if ( !obj || !obj->get( "index" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите период" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpPeriod.GetButton( ) ) );
			return 1;
		}

		szNum = m_inpNum.GetValue( );
		if ( szNum.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите количество человек" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpNum.GetEntry( ) ) );
			return 2;
		}

		szAmount = m_inpAmount.GetValue( );
		if ( szAmount.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите сумму" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpAmount.GetEntry( ) ) );
			return 3;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите валюту" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCurrency.GetButton( ) ) );
			return 4;
		}

		return 0;
	} // udInt Validate

	void CHServicePriceForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szNum,
						szAmount,
						szAmountRecommend;
		
		szNum = m_inpNum.GetValue( );
		szAmount = m_inpAmount.GetValue( );
		szAmountRecommend = m_inpAmountRecommend.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "num" )->ini( ConvertString( szNum.data( ), CP_UTF8 ) );
		obj->get( "period" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPeriod ) )->GetObject( )->get( "index" )->toUInt( )
		);
		obj->get( "amount" )->ini( ConvertString( szAmount.data( ), CP_UTF8 ) );
		obj->get( "currency" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "amount_recommend" )->ini( ConvertString( szAmountRecommend.data( ), CP_UTF8 ) );
		obj->get( "currency_recommend" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrencyRecommend ) )->GetObject( )->get( "id" )->toUInt( )
		);

		//m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHServicePriceFormModal::CHServicePriceFormModal( udPHServiceForm pService ) : CHServicePriceForm( pService ) {
	} // CHServicePriceFormModal

	CHServicePriceFormModal::CHServicePriceFormModal( const CHServicePriceFormModal& objCopy ) : CHServicePriceForm( ) {
	} // CHServicePriceFormModal

	CHServicePriceFormModal::~CHServicePriceFormModal( ) {
	} // ~CHServicePriceFormModal

	CHServicePriceFormModal& CHServicePriceFormModal::operator =( const CHServicePriceFormModal& objCopy ) {
		return *this;
	} // CHServicePriceFormModal& operator =

	void CHServicePriceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHServicePriceFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHServicePriceForm

	void CHServicePriceFormModal::LoadFrom( udPBase from ) {
		//udFormHandler::Load( );
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		udPBase	pPeriod	= ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPeriod ) )->GetObject( );
		udUInt	id		= pPeriod->get( "id" )->toUInt( ),
				type	= obj->get( "period" )->toUInt( );
		
		if ( ( id == 0 ) && ( type > 0 ) && m_pService ) {
			udPArrBase arrObject = ( ( udPFDEList ) m_pService->GetData( )->GetExtraData( udHServiceForm::ctrlidPeriod ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					if ( ( *i )->get( "index" )->toUInt( ) == type ) {
						pPeriod->apply( *i, true, true );
						break;
					}
				}
			}
		}

		udPBase pCurrency = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency ) )->GetObject( );
		id = pCurrency->get( "id" )->toUInt( );
		type = obj->get( "currency" )->toUInt( );
		if ( ( id == 0 ) && ( type > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "currency", type, GetProjectCurrency );
			if ( pLoaded ) {
				pCurrency->apply( pLoaded, true, true );
				udDELETE( pLoaded );
			}
		}

		udPBase pCurrencyRecommend = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrencyRecommend ) )->GetObject( );
		id = pCurrencyRecommend->get( "id" )->toUInt( );
		type = obj->get( "currency_recommend" )->toUInt( );
		if ( ( id == 0 ) && ( type > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "currency", type, GetProjectCurrency );
			if ( pLoaded ) {
				pCurrencyRecommend->apply( pLoaded, true, true );
				udDELETE( pLoaded );
			}
		}

		m_inpNum.SetValue( Glib::locale_to_utf8( obj->get( "num" )->toString( ) ) );
		m_inpAmount.SetValue( Glib::locale_to_utf8( obj->get( "amount" )->toString( ) ) );
		m_inpAmountRecommend.SetValue( Glib::locale_to_utf8( obj->get( "amount_recommend" )->toString( ) ) );
		m_inpPeriod.SetTitle( Glib::locale_to_utf8(
			pPeriod->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) + " - " +
			pPeriod->get( "date2" )->toTime( ).toString( "%d.%m.%Y" )
		) );
		m_inpCurrency.SetTitle( Glib::locale_to_utf8(
			pCurrency->get( "name" )->toString( )
		) );
		m_inpCurrencyRecommend.SetTitle( Glib::locale_to_utf8(
			pCurrencyRecommend->get( "name" )->toString( )
		) );
	} // void LoadFrom

} // namespace UdSDK
