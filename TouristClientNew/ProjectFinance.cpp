
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHFinanceSummAddForm::CHFinanceSummAddForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Зачисление" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Сумма" ) ),
		m_lblCurrency( Glib::locale_to_utf8( "Валюта" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblHow( Glib::locale_to_utf8( "Способ" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_inpType( udProjectFinanceSumm::SummWhite, udProjectFinanceSumm::SummWhite ),
		m_inpHow( udProjectFinanceOperation::fctCash, udProjectFinanceOperation::fctCash ),
		m_iType( udProjectFinanceSumm::SummTypeNone ),
		m_iHow( udProjectFinanceOperation::fctCash )
	{
		m_szName = "financesummadd_form";

		m_pData->SetName( "financesummadd" );
		m_pData->SetGetObject( GetProjectFinanceSummAdd );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblSumm.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpSumm.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpType.AddRow( udProjectFinanceSumm::SummWhite, Glib::locale_to_utf8( "Белые" ) );
		m_inpType.AddRow( udProjectFinanceSumm::SummBlack, Glib::locale_to_utf8( "Черные" ) );
		m_inpType.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpHow.AddRow( udProjectFinanceOperation::fctCash, Glib::locale_to_utf8( "Наличные" ) );
		m_inpHow.AddRow( udProjectFinanceOperation::fctCard, Glib::locale_to_utf8( "Карточка" ) );
		m_inpHow.AddRow( udProjectFinanceOperation::fctNonCash, Glib::locale_to_utf8( "Безналичные" ) );
		m_inpHow.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
	} // CHFinanceSummAddForm

	CHFinanceSummAddForm::CHFinanceSummAddForm( udUInt iType, udUInt iHow ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Зачисление" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Сумма" ) ),
		m_lblCurrency( Glib::locale_to_utf8( "Валюта" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblHow( Glib::locale_to_utf8( "Способ" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_iType( iType ),
		m_iHow( iHow )
	{
		m_szName = "financesummadd_form";

		m_pData->SetName( "financesummadd" );
		m_pData->SetGetObject( GetProjectFinanceSummAdd );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblSumm.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpSumm.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
	} // CHFinanceSummAddForm

	CHFinanceSummAddForm::CHFinanceSummAddForm( const CHFinanceSummAddForm& objCopy ) : udFormHandler( ) {
	} // CHFinanceSummAddForm

	CHFinanceSummAddForm::~CHFinanceSummAddForm( ) {
	} // ~CHFinanceSummAddForm

	CHFinanceSummAddForm& CHFinanceSummAddForm::operator =( const CHFinanceSummAddForm& objCopy ) {
		return *this;
	} // CHFinanceSummAddForm& operator =

	void CHFinanceSummAddForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта
		m_tblInput.attach( m_lblCurrency, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			//m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			//m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		}

		// Способ
		if ( m_iHow == udProjectFinanceOperation::fctNone ) {
			m_tblInput.attach( m_lblHow, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			m_tblInput.attach( m_inpHow, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		}

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	void CHFinanceSummAddForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpSumm.Reset( );
		//m_inpType.set_active( 0 );
		//m_inpHow.set_active( 0 );
		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			m_inpType.Reset( );
		}
		if ( m_iHow == udProjectFinanceOperation::fctNone ) {
			m_inpHow.Reset( );
		}
		m_inpCurrency.SetTitle( "" );
		m_inpComment.Reset( );
	} // void Clear

	void CHFinanceSummAddForm::Load( ) {
	} // void Load

	udInt CHFinanceSummAddForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udFloat fSumm;

		fSumm = toFloat( m_inpSumm.GetValue( ).data( ) );
		if ( !fSumm ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите сумму" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpSumm.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите валюту" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCurrency.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHFinanceSummAddForm::Send( ) {
		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Glib::ustring	szComment;
		udFloat fSumm;

		fSumm = toFloat( m_inpSumm.GetValue( ).data( ) );
		szComment = m_inpComment.GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udUInt type = udProjectFinanceSumm::SummTypeNone;

		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			type = udProjectFinanceSumm::SummWhite;
			//type = m_inpType.LoadValue( ).GetValue( );
		} else {
			type = m_iType;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udUInt how = udProjectFinanceOperation::fctNone;

		if ( m_iHow == udProjectFinanceOperation::fctNone ) {
			how = m_inpHow.LoadValue( ).GetValue( );
		} else {
			how = m_iHow;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "summ" )->ini( fSumm );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );
		obj->get( "currency" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "type" )->ini( type );
		obj->get( "how" )->ini( how );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummAddFormModal::CHFinanceSummAddFormModal( ) : CHFinanceSummAddForm( ) {
	} // CHFinanceSummAddFormModal

	CHFinanceSummAddFormModal::CHFinanceSummAddFormModal( const CHFinanceSummAddFormModal& objCopy ) : CHFinanceSummAddForm( ) {
	} // CHFinanceSummAddFormModal

	CHFinanceSummAddFormModal::CHFinanceSummAddFormModal( udUInt iType, udUInt iHow ) : CHFinanceSummAddForm( iType, iHow ) {
	} // CHFinanceSummAddFormModal

	CHFinanceSummAddFormModal::~CHFinanceSummAddFormModal( ) {
	} // ~CHFinanceSummAddFormModal

	CHFinanceSummAddFormModal& CHFinanceSummAddFormModal::operator =( const CHFinanceSummAddFormModal& objCopy ) {
		return *this;
	} // CHFinanceSummAddFormModal& operator =

	void CHFinanceSummAddFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHFinanceSummAddFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummBackForm::CHFinanceSummBackForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Снятие" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Сумма" ) ),
		m_lblCurrency( Glib::locale_to_utf8( "Валюта" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblHow( Glib::locale_to_utf8( "Способ" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_inpType( udProjectFinanceSumm::SummWhite, udProjectFinanceSumm::SummWhite ),
		m_inpHow( udProjectFinanceOperation::fctCash, udProjectFinanceOperation::fctCash ),
		m_iType( udProjectFinanceSumm::SummTypeNone ),
		m_iHow( udProjectFinanceOperation::fctCash )
	{
		m_szName = "financesummback_form";

		m_pData->SetName( "financesummback" );
		m_pData->SetGetObject( GetProjectFinanceSummBack );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblSumm.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpSumm.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpType.AddRow( udProjectFinanceSumm::SummWhite, Glib::locale_to_utf8( "Белые" ) );
		m_inpType.AddRow( udProjectFinanceSumm::SummBlack, Glib::locale_to_utf8( "Черные" ) );
		m_inpType.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpHow.AddRow( udProjectFinanceOperation::fctCash, Glib::locale_to_utf8( "Наличные" ) );
		m_inpHow.AddRow( udProjectFinanceOperation::fctCard, Glib::locale_to_utf8( "Карточка" ) );
		m_inpHow.AddRow( udProjectFinanceOperation::fctNonCash, Glib::locale_to_utf8( "Безналичные" ) );
		m_inpHow.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
	} // CHFinanceSummBackForm

	CHFinanceSummBackForm::CHFinanceSummBackForm( udUInt iType, udUInt iHow ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Снятие" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Сумма" ) ),
		m_lblCurrency( Glib::locale_to_utf8( "Валюта" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblHow( Glib::locale_to_utf8( "Способ" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_iType( iType ),
		m_iHow( iHow )
	{
		m_szName = "financesummback_form";

		m_pData->SetName( "financesummback" );
		m_pData->SetGetObject( GetProjectFinanceSummBack );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblSumm.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpSumm.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
	} // CHFinanceSummBackForm

	CHFinanceSummBackForm::CHFinanceSummBackForm( const CHFinanceSummBackForm& objCopy ) : udFormHandler( ) {
	} // CHFinanceSummBackForm

	CHFinanceSummBackForm::~CHFinanceSummBackForm( ) {
	} // ~CHFinanceSummBackForm

	CHFinanceSummBackForm& CHFinanceSummBackForm::operator =( const CHFinanceSummBackForm& objCopy ) {
		return *this;
	} // CHFinanceSummBackForm& operator =

	void CHFinanceSummBackForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта
		m_tblInput.attach( m_lblCurrency, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			//m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			//m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		}

		// Способ
		if ( m_iHow == udProjectFinanceOperation::fctNone ) {
			m_tblInput.attach( m_lblHow, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			m_tblInput.attach( m_inpHow, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		}

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	void CHFinanceSummBackForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpSumm.Reset( );
		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			m_inpType.Reset( );
		}
		if ( m_iHow == udProjectFinanceOperation::fctNone ) {
			m_inpHow.Reset( );
		}
		m_inpCurrency.SetTitle( "" );
		m_inpComment.Reset( );
	} // void Clear

	void CHFinanceSummBackForm::Load( ) {
	} // void Load

	udInt CHFinanceSummBackForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udFloat fSumm;

		fSumm = toFloat( m_inpSumm.GetValue( ).data( ) );
		if ( !fSumm ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите сумму" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpSumm.GetEntry( ) ));
			return 1;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите валюту" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCurrency.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHFinanceSummBackForm::Send( ) {
		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Glib::ustring	szComment;
		udFloat fSumm;

		fSumm = toFloat( m_inpSumm.GetValue( ).data( ) );
		szComment = m_inpComment.GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udUInt type = udProjectFinanceSumm::SummTypeNone;

		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			type = udProjectFinanceSumm::SummWhite;//m_inpType.LoadValue( ).GetValue( );
		} else {
			type = m_iType;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udUInt how = udProjectFinanceOperation::fctNone;

		if ( m_iHow == udProjectFinanceOperation::fctNone ) {
			how = m_inpHow.LoadValue( ).GetValue( );
		} else {
			how = m_iHow;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "summ" )->ini( fSumm );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );
		obj->get( "currency" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "type" )->ini( type );
		obj->get( "how" )->ini( how );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummBackFormModal::CHFinanceSummBackFormModal( ) : CHFinanceSummBackForm( ) {
	} // CHFinanceSummBackFormModal

	CHFinanceSummBackFormModal::CHFinanceSummBackFormModal( const CHFinanceSummBackFormModal& objCopy ) : CHFinanceSummBackForm( ) {
	} // CHFinanceSummBackFormModal

	CHFinanceSummBackFormModal::CHFinanceSummBackFormModal( udUInt iType, udUInt iHow ) : CHFinanceSummBackForm( iType, iHow ) {
	} // CHFinanceSummAddFormModal

	CHFinanceSummBackFormModal::~CHFinanceSummBackFormModal( ) {
	} // ~CHFinanceSummBackFormModal

	CHFinanceSummBackFormModal& CHFinanceSummBackFormModal::operator =( const CHFinanceSummBackFormModal& objCopy ) {
		return *this;
	} // CHFinanceSummBackFormModal& operator =

	void CHFinanceSummBackFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHFinanceSummBackFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummConvertForm::CHFinanceSummConvertForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Конвертация" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Сумма" ) ),
		m_lblCurrency1( Glib::locale_to_utf8( "Валюта из" ) ),
		m_lblCurrency2( Glib::locale_to_utf8( "Валюта в" ) ),
		m_lblType1( Glib::locale_to_utf8( "Тип из" ) ),
		m_lblType2( Glib::locale_to_utf8( "Тип в" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_inpType1( udProjectFinanceSumm::SummWhite, udProjectFinanceSumm::SummWhite ),
		m_inpType2( udProjectFinanceSumm::SummWhite, udProjectFinanceSumm::SummWhite ),
		m_iType( udProjectFinanceSumm::SummTypeNone )
	{
		m_szName = "financesummconvert_form";

		m_pData->SetName( "financesummconvert" );
		m_pData->SetGetObject( GetProjectFinanceSummConvert );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblSumm.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpSumm.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpType1.AddRow( udProjectFinanceSumm::SummWhite, Glib::locale_to_utf8( "Белые" ) );
		m_inpType1.AddRow( udProjectFinanceSumm::SummBlack, Glib::locale_to_utf8( "Черные" ) );
		m_inpType1.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpType2.AddRow( 0, Glib::locale_to_utf8( "-- нет --" ) );
		m_inpType2.AddRow( udProjectFinanceSumm::SummWhite, Glib::locale_to_utf8( "Белые" ) );
		m_inpType2.AddRow( udProjectFinanceSumm::SummBlack, Glib::locale_to_utf8( "Черные" ) );
		m_inpType2.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrency1 );
		m_pData->AddExtraData( pFde );
		m_inpCurrency1.SetDataObject( pFde );
		m_inpCurrency1.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrency1.SetAttr( "name" );
		m_inpCurrency1.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrency1.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrency1.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrency2 );
		m_pData->AddExtraData( pFde );
		m_inpCurrency2.SetDataObject( pFde );
		m_inpCurrency2.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrency2.SetAttr( "name" );
		m_inpCurrency2.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrency2.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrency2.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHFinanceSummConvertForm

	CHFinanceSummConvertForm::CHFinanceSummConvertForm( const CHFinanceSummConvertForm& objCopy ) : udFormHandler( ) {
	} // CHFinanceSummConvertForm

	CHFinanceSummConvertForm::CHFinanceSummConvertForm( udUInt iType ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Конвертация" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Сумма" ) ),
		m_lblCurrency1( Glib::locale_to_utf8( "Валюта из" ) ),
		m_lblCurrency2( Glib::locale_to_utf8( "Валюта в" ) ),
		m_lblType1( Glib::locale_to_utf8( "Тип из" ) ),
		m_lblType2( Glib::locale_to_utf8( "Тип в" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_iType( iType )
	{
		m_szName = "financesummconvert_form";

		m_pData->SetName( "financesummconvert" );
		m_pData->SetGetObject( GetProjectFinanceSummConvert );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblSumm.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpSumm.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrency1 );
		m_pData->AddExtraData( pFde );
		m_inpCurrency1.SetDataObject( pFde );
		m_inpCurrency1.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrency1.SetAttr( "name" );
		m_inpCurrency1.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrency1.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrency1.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrency2 );
		m_pData->AddExtraData( pFde );
		m_inpCurrency2.SetDataObject( pFde );
		m_inpCurrency2.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrency2.SetAttr( "name" );
		m_inpCurrency2.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrency2.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrency2.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHFinanceSummConvertForm

	CHFinanceSummConvertForm::~CHFinanceSummConvertForm( ) {
	} // ~CHFinanceSummConvertForm

	CHFinanceSummConvertForm& CHFinanceSummConvertForm::operator =( const CHFinanceSummConvertForm& objCopy ) {
		return *this;
	} // CHFinanceSummConvertForm& operator =

	void CHFinanceSummConvertForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта из
		m_tblInput.attach( m_lblCurrency1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта в
		m_tblInput.attach( m_lblCurrency2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			// Тип из
			//m_tblInput.attach( m_lblType1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			//m_tblInput.attach( m_inpType1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

			// Тип в
			//m_tblInput.attach( m_lblType2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			//m_tblInput.attach( m_inpType2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		}

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	void CHFinanceSummConvertForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpSumm.Reset( );
		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			m_inpType1.Reset( );
			m_inpType2.Reset( );
		}
		m_inpCurrency1.SetTitle( "" );
		m_inpCurrency2.SetTitle( "" );
		m_inpComment.Reset( );
	} // void Clear

	void CHFinanceSummConvertForm::Load( ) {
	} // void Load

	udInt CHFinanceSummConvertForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udFloat fSumm;

		fSumm = toFloat( m_inpSumm.GetValue( ).data( ) );
		if ( !fSumm ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите сумму" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpSumm.GetEntry( ) ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHFinanceSummConvertForm::Send( ) {
		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Glib::ustring	szComment;
		udFloat fSumm;

		fSumm = toFloat( m_inpSumm.GetValue( ).data( ) );
		szComment = m_inpComment.GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udUInt	type1 = udProjectFinanceSumm::SummTypeNone,
				type2 = udProjectFinanceSumm::SummTypeNone;

		if ( m_iType == udProjectFinanceSumm::SummTypeNone ) {
			type1 = udProjectFinanceSumm::SummWhite;//m_inpType1.LoadValue( ).GetValue( );
			type2 = udProjectFinanceSumm::SummWhite;//m_inpType2.LoadValue( ).GetValue( );
		} else {
			type1 = m_iType;
			type2 = m_iType;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "summ" )->ini( fSumm );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );
		obj->get( "currency1" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency1 ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "currency2" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency2 ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "type1" )->ini( type1 );
		obj->get( "type2" )->ini( type2 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummConvertFormModal::CHFinanceSummConvertFormModal( ) : CHFinanceSummConvertForm( ) {
	} // CHFinanceSummConvertFormModal

	CHFinanceSummConvertFormModal::CHFinanceSummConvertFormModal( const CHFinanceSummConvertFormModal& objCopy ) : CHFinanceSummConvertForm( ) {
	} // CHFinanceSummConvertFormModal

	CHFinanceSummConvertFormModal::CHFinanceSummConvertFormModal( udUInt iType ) : CHFinanceSummConvertForm( iType ) {
	} // CHFinanceSummConvertFormModal

	CHFinanceSummConvertFormModal::~CHFinanceSummConvertFormModal( ) {
	} // ~CHFinanceSummConvertFormModal

	CHFinanceSummConvertFormModal& CHFinanceSummConvertFormModal::operator =( const CHFinanceSummConvertFormModal& objCopy ) {
		return *this;
	} // CHFinanceSummConvertFormModal& operator =

	void CHFinanceSummConvertFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHFinanceSummConvertFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

} // namespace UdSDK
