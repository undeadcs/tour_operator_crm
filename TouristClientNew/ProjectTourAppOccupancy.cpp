
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourAppOccupancyList( ) {
		udPBase ret = GetProjectGCOccupancy( );
		return ret;
	} // udPBase GetProjectTourAppOccupancyList

	CHTourAppOccupancyForm::CHTourAppOccupancyForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Средство размещения" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblCategory( Glib::locale_to_utf8( "Категория" ) ),
		m_lblAmount( Glib::locale_to_utf8( "Количество проживающих" ) ),
		m_lblFeed( Glib::locale_to_utf8( "Питание" ) )
	{
		m_szName = "tourappoccupancy_form";

		m_pData->SetName( "tourappoccupancy" );
		//m_pData->SetGetObject( GetProjectTourAppOccupancy );
		m_pData->SetGetObject( GetProjectGCOccupancy );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHTourAppOccupancyForm

	CHTourAppOccupancyForm::CHTourAppOccupancyForm( const CHTourAppOccupancyForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppOccupancyForm

	CHTourAppOccupancyForm::~CHTourAppOccupancyForm( ) {
	} // ~CHTourAppOccupancyForm

	CHTourAppOccupancyForm& CHTourAppOccupancyForm::operator =( const CHTourAppOccupancyForm& objCopy ) {
		return *this;
	} // CHTourAppOccupancyForm& operator =

	void CHTourAppOccupancyForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Категория
		m_tblInput.attach( m_lblCategory, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCategory, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Количество проживающих
		m_tblInput.attach( m_lblAmount, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpAmount, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Питание
		m_tblInput.attach( m_lblFeed, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFeed, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHTourAppOccupancyForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpCategory.Reset( );
		m_inpAmount.Reset( );
		m_inpFeed.Reset( );
	} // void Clear

	void CHTourAppOccupancyForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpCategory.SetValue( Glib::locale_to_utf8( obj->get( "category" )->toString( ) ) );
		m_inpAmount.SetValue( Glib::locale_to_utf8( obj->get( "amount" )->toString( ) ) );
		m_inpFeed.SetValue( Glib::locale_to_utf8( obj->get( "feed" )->toString( ) ) );
	} // void Load

	udInt CHTourAppOccupancyForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szCategory,
						szAmount,
						szFeed;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		szCategory = m_inpCategory.GetValue( );
		if ( szCategory.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите категорию" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCategory.GetEntry( ) ) );
			return 2;
		}

		szAmount = m_inpAmount.GetValue( );
		if ( szAmount.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите количество проживающих" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpAmount.GetEntry( ) ) );
			return 3;
		}

		szFeed = m_inpFeed.GetValue( );
		if ( szFeed.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите питание" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFeed.GetEntry( ) ) );
			return 4;
		}

		return 0;
	} // udInt Validate

	void CHTourAppOccupancyForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szCategory,
						szAmount,
						szFeed;

		szName = m_inpName.GetValue( );
		szCategory = m_inpCategory.GetValue( );
		szAmount = m_inpAmount.GetValue( );
		szFeed = m_inpFeed.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "category" )->ini( ConvertString( szCategory.data( ), CP_UTF8 ) );
		obj->get( "amount" )->ini( ConvertString( szAmount.data( ), CP_UTF8 ) );
		obj->get( "feed" )->ini( ConvertString( szFeed.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppOccupancyFormModal::CHTourAppOccupancyFormModal( ) : CHTourAppOccupancyForm( ) {
	} // CHTourAppOccupancyFormModal

	CHTourAppOccupancyFormModal::CHTourAppOccupancyFormModal( const CHTourAppOccupancyFormModal& objCopy ) : CHTourAppOccupancyForm( ) {
	} // CHTourAppOccupancyFormModal

	CHTourAppOccupancyFormModal::~CHTourAppOccupancyFormModal( ) {
	} // ~CHTourAppOccupancyFormModal

	CHTourAppOccupancyFormModal& CHTourAppOccupancyFormModal::operator =( const CHTourAppOccupancyFormModal& objCopy ) {
		return *this;
	} // CHTourAppOccupancyFormModal& operator =

	void CHTourAppOccupancyFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourAppOccupancyFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourAppOccupancyForm

	void CHTourAppOccupancyFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpCategory.SetValue( Glib::locale_to_utf8( obj->get( "category" )->toString( ) ) );
		m_inpAmount.SetValue( Glib::locale_to_utf8( obj->get( "amount" )->toString( ) ) );
		m_inpFeed.SetValue( Glib::locale_to_utf8( obj->get( "feed" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
