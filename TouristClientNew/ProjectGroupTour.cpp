
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectGroupTourList( ) {
		udPBase ret = GetProjectGroupTour( );
		ret->get( "name" )->ini( wstring( L"" ) );
		ret->get( "date1" )->ini( datetime( ) );
		ret->get( "date2" )->ini( datetime( ) );
		ret->get( "num" )->ini( ( udUInt ) 0 );
		return ret;
	} // udPBase GetProjectGroupTourList

	CHGroupTourForm::CHGroupTourForm( udPHGroupForm hGroup ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Тур группы" ) ),
		m_lblTour( Glib::locale_to_utf8( "Тур" ) ),
		m_lblPeriod( Glib::locale_to_utf8( "Период" ) ),
		m_hGroup( hGroup )
	{
		m_szName = "grouptour_form";

		m_pData->SetName( "grouptour" );
		m_pData->SetGetObject( GetProjectGroupTourList );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblPeriod.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblPeriod.set_size_request( 250 );

		udPFDEObject pFde = NULL;

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTour );
		pFde->SetMark( ctrlidTour );
		m_pData->AddExtraData( pFde );
		m_inpTour.SetDataObject( pFde );
		m_inpTour.SetWindowTitle( Glib::locale_to_utf8( "Выбор тура" ) );
		m_inpTour.SetAttr( "name" );
		m_inpTour.SetGetListHandler( GetHTourListPick );
		m_inpTour.SetGetFormHandler( GetHTourForm );
		m_inpTour.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование тура" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectGroupPeriod );
		pFde->SetMark( ctrlidPeriod );
		m_pData->AddExtraData( pFde );
		m_inpPeriod.SetDataObject( pFde );
		m_inpPeriod.SetWindowTitle( Glib::locale_to_utf8( "Выбор периода" ) );
		m_inpPeriod.SetAttr( "name" );
		m_inpPeriod.SetGetListHandler( GetHGroupPeriodListPick );
		m_inpPeriod.SetGroupForm( m_hGroup );
	} // CHGroupTourForm

	CHGroupTourForm::CHGroupTourForm( const CHGroupTourForm& objCopy ) : udFormHandler( ) {
	} // CHGroupTourForm

	CHGroupTourForm::~CHGroupTourForm( ) {
	} // ~CHGroupTourForm

	CHGroupTourForm& CHGroupTourForm::operator =( const CHGroupTourForm& objCopy ) {
		return *this;
	} // CHGroupTourForm& operator =

	void CHGroupTourForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Валюта
		m_tblInput.attach( m_lblTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpTour, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Период
		m_tblInput.attach( m_lblPeriod, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPeriod, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHGroupTourForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpPeriod.SetTitle( "" );
		m_inpTour.SetTitle( "" );
	} // void Clear

	void CHGroupTourForm::Load( ) {
	} // void Load

	udInt CHGroupTourForm::Validate( ) {
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

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidTour ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тур" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpTour.GetButton( ) ) );
			return 4;
		}

		return 0;
	} // udInt Validate

	void CHGroupTourForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "period" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPeriod ) )->GetObject( )->get( "index" )->toUInt( )
		);
		obj->get( "tour" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidTour ) )->GetObject( )->get( "id" )->toUInt( )
		);

		//m_pData->Send( );
	} // void Send

	




	CHGroupTourFormModal::CHGroupTourFormModal( udPHGroupForm hGroup ) : CHGroupTourForm( hGroup ) {
	} // CHGroupTourFormModal

	CHGroupTourFormModal::CHGroupTourFormModal( const CHGroupTourFormModal& objCopy ) : CHGroupTourForm( ) {
	} // CHGroupTourFormModal

	CHGroupTourFormModal::~CHGroupTourFormModal( ) {
	} // ~CHGroupTourFormModal

	CHGroupTourFormModal& CHGroupTourFormModal::operator =( const CHGroupTourFormModal& objCopy ) {
		return *this;
	} // CHGroupTourFormModal& operator =

	void CHGroupTourFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHGroupTourFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHGroupTourForm

	void CHGroupTourFormModal::LoadFrom( udPBase from ) {
		//udFormHandler::Load( );
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		udPBase	pPeriod	= ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPeriod ) )->GetObject( );
		udUInt	id		= pPeriod->get( "id" )->toUInt( ),
				type	= obj->get( "period" )->toUInt( );
		
		if ( ( id == 0 ) && ( type > 0 ) && m_hGroup ) {
			udPArrBase arrObject = ( ( udPFDEList ) m_hGroup->GetData( )->GetExtraData( udHGroupForm::ctrlidPeriod ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					if ( ( *i )->get( "index" )->toUInt( ) == type ) {
						pPeriod->apply( *i, true, true );
						break;
					}
				}
			}
		}

		udPBase pTour = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidTour ) )->GetObject( );
		id = pTour->get( "id" )->toUInt( );
		type = obj->get( "tour" )->toUInt( );
		if ( ( id == 0 ) && ( type > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "tour", type, GetProjectTour );
			if ( pLoaded ) {
				pTour->apply( pLoaded, true, true );
				udDELETE( pLoaded );
			}
		}

		m_inpTour.SetTitle( Glib::locale_to_utf8(
			pTour->get( "name" )->toString( )
		) );
		m_inpPeriod.SetTitle( Glib::locale_to_utf8(
			pPeriod->get( "date1" )->toTime( ).toString( "%d.%m.%Y" )
		) );
	} // void LoadFrom

} // namespace UdSDK
