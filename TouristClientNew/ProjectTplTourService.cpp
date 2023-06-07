
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTplTourServiceList( ) {
		udPBase ret = GetProjectTplTourService( );
		ret->get( "service_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTplTourServiceList

	CHTplTourServiceForm::CHTplTourServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Примерная услуга" ) ),
		m_lblService( Glib::locale_to_utf8( "Услуга" ) ),
		m_lblNum( Glib::locale_to_utf8( "Количество человек" ) ),
		m_lblDate1( Glib::locale_to_utf8( "С" ) ),
		m_lblDate2( Glib::locale_to_utf8( "По" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) )
	{
		m_szName = "tpltourservice_form";

		m_pData->SetName( "tpltourservice" );
		m_pData->SetGetObject( GetProjectTplTourService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblService.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblService.set_size_request( 250 );

		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectService );
		pFde->SetMark( ctrlidService );
		m_pData->AddExtraData( pFde );
		m_inpService.SetDataObject( pFde );
		m_inpService.SetWindowTitle( Glib::locale_to_utf8( "Выбор услуги" ) );
		m_inpService.SetAttr( "name_" );
		m_inpService.SetGetListHandler( GetHServiceListPick );
		m_inpService.SetGetFormHandler( GetHServiceForm );
		m_inpService.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование услуги" ) );
	} // CHTplTourServiceForm

	CHTplTourServiceForm::CHTplTourServiceForm( const CHTplTourServiceForm& objCopy ) : udFormHandler( ) {
	} // CHTplTourServiceForm

	CHTplTourServiceForm::~CHTplTourServiceForm( ) {
	} // ~CHTplTourServiceForm

	CHTplTourServiceForm& CHTplTourServiceForm::operator =( const CHTplTourServiceForm& objCopy ) {
		return *this;
	} // CHTplTourServiceForm& operator =

	void CHTplTourServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Услуга
		m_tblInput.attach( m_lblService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpService, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Количество человек
		m_tblInput.attach( m_lblNum, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNum, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата "с"
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата "по"
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHTplTourServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpNum.Reset( );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
	} // void Clear

	void CHTplTourServiceForm::Load( ) {
	} // void Load

	udInt CHTplTourServiceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szNum;

		szNum = m_inpNum.GetValue( );
		if ( szNum.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите количество человек" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpNum.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidService ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите услугу" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpService.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHTplTourServiceForm::Send( ) {
		m_pData->MakeObject( );

		udUInt	iNum;

		iNum = toUInt( m_inpNum.GetValue( ).data( ) );

		udPBase obj = m_pData->GetData( );

		obj->get( "num" )->ini( iNum );
		obj->get( "service" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidService ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate1.GetValue( ) );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTplTourServiceFormModal::CHTplTourServiceFormModal( ) : CHTplTourServiceForm( ) {
	} // CHTplTourServiceFormModal

	CHTplTourServiceFormModal::CHTplTourServiceFormModal( const CHTplTourServiceFormModal& objCopy ) : CHTplTourServiceForm( ) {
	} // CHTplTourServiceFormModal

	CHTplTourServiceFormModal::~CHTplTourServiceFormModal( ) {
	} // ~CHTplTourServiceFormModal

	CHTplTourServiceFormModal& CHTplTourServiceFormModal::operator =( const CHTplTourServiceFormModal& objCopy ) {
		return *this;
	} // CHTplTourServiceFormModal& operator =

	void CHTplTourServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTplTourServiceFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTplTourServiceForm

	void CHTplTourServiceFormModal::LoadFrom( udPBase from ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		udPBase	pService	= ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidService ) )->GetObject( );
		udUInt	id			= pService->get( "id" )->toUInt( ),
				key			= obj->get( "service" )->toUInt( );
		
		if ( ( id == 0 ) && ( key > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "service", key, GetProjectService );
			if ( pLoaded ) {
				pService->apply( pLoaded, true, true );

				udUInt idName = pLoaded->get( "name" )->toUInt( );
				if ( idName > 0 ) {
					udPBase pName = udDataAccess::LoadById( "nameservice", idName, GetProjectNameService );
					if ( pName ) {
						pService->get( "name_" )->ini( pName->get( "name" )->toWString( ) );
						udDELETE( pName );
					}
				}
				
				udDELETE( pLoaded );
			}
		}

		m_inpService.SetTitle( Glib::locale_to_utf8(
			pService->get( "name_" )->toString( )
		) );

		m_inpNum.SetValue( Glib::locale_to_utf8( obj->get( "num" )->toString( ) ) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
	} // void LoadFrom

} // namespace UdSDK
