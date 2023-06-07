
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDAReportLanding::CDAReportLanding( ) : udDAList( ) {
	} // CDAReportLanding

	CDAReportLanding::CDAReportLanding( const CDAReportLanding& objCopy ) : udDAList( objCopy ) {
	} // CDAReportLanding

	CDAReportLanding::~CDAReportLanding( ) {
	} // ~CDAReportLanding

	CDAReportLanding& CDAReportLanding::operator =( const CDAReportLanding& objCopy ) {
		udDAList::operator =( objCopy );
		return *this;
	} // CDAReportLanding& operator =

	void CDAReportLanding::Load( ) {
		/*
		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			// ListRequest
			udBinaryData data;
			data.add( m_szName );
			data.add( ( udInt ) opidLst1 );
			data.add( m_pPager->GetPage( ) );
			data.add( m_pPager->GetPageSize( ) );
			data.add( m_szOrderBy );
			data.add( m_iOrder );

			string log = "\nфильтр, ";
			m_pFilter->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			m_pFilter->dump( data );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udType total( ( udUInt ) 0 );
				total.read( sock );
				m_pPager->SetTotal( total.toUInt( ) );
				udDataAccess::Read( sock, *m_arrObject, m_fnGetObject );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
		//*/
	} // void Load

	void CDAReportLanding::LoadRemote( ) {
	} // void LoadRemote

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectReportLandingList( ) {
		udPBase ret = GetProjectReportLanding( );
		ret->get( "group_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectReportLandingList

	CHReportLanding::CHReportLanding( ) :
		udListHandler( ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата начала" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "Дата окончания" ) ),
		m_lblFltGroup( Glib::locale_to_utf8( "Группа" ) ),
		m_lblFltDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_btnLoad( Glib::locale_to_utf8( "Загрузить" ) ),
		m_btnExport( Glib::locale_to_utf8( "Экспорт" ) )
	{
		m_btnLoad.set_name( "TouristListButtonFilter" );
		m_btnLoad.set_size_request( 151, 34 );
		m_btnExport.set_name( "TouristListButtonReset" );
		m_btnExport.set_size_request( 151, 34 );

		m_szName = "report_landing";

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "group" )->ini( ( udUInt ) 0 );
		filter->get( "direct" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHReportLanding::ColModelReportLanding( );

		udDELETE( m_pData );

		m_pData = new udDAReportLanding;
		//m_pData->SetName( "report_landing" );
		//m_pData->SetGetObject( GetProjectReportLandingList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectGroup );
		pFde->SetMark( fltGroup );
		m_pData->AddExtraData( pFde );
		m_inpFltGroup.SetDataObject( pFde );
		m_inpFltGroup.SetWindowTitle( Glib::locale_to_utf8( "Выбор группы" ) );
		m_inpFltGroup.SetAttr( "name" );
		m_inpFltGroup.SetGetListHandler( GetHGroupListPick );
		m_inpFltGroup.SetGetFormHandler( GetHGroupForm );
		m_inpFltGroup.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование группы" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDirect );
		pFde->SetMark( fltDirect );
		m_pData->AddExtraData( pFde );
		m_inpFltDirect.SetDataObject( pFde );
		m_inpFltDirect.SetWindowTitle( Glib::locale_to_utf8( "Выбор направления" ) );
		m_inpFltDirect.SetAttr( "name" );
		m_inpFltDirect.SetGetListHandler( GetHDirectListPick );
		m_inpFltDirect.SetGetFormHandler( GetHDirectForm );
		m_inpFltDirect.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование направления" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Дата начала
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Группа
		m_tblFilter.attach( m_lblFltGroup, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltGroup, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Дата окончания
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Направление
		m_tblFilter.attach( m_lblFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblFilter.attach( m_btnLoad, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnExport, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// дополнительные кнопки
		m_btnLoad.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportLanding::OnButtonLoad ) );
		m_btnExport.signal_clicked( ).connect( sigc::mem_fun( this, &CHReportLanding::OnButtonExport ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHReportLanding

	CHReportLanding::CHReportLanding( const CHReportLanding& objCopy ) : udListHandler( ) {
		m_szName = "report_landing";
	} // CHReportLanding

	CHReportLanding::~CHReportLanding( ) {
	} // ~CReportLanding

	CHReportLanding& CHReportLanding::operator =( const CHReportLanding& objCopy ) {
		return *this;
	} // CHReportLanding& operator =

	void CHReportLanding::Init( ) {
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Группа" ), m_pColModel->m_col_group );
		m_objTreeView.append_column( Glib::locale_to_utf8( "ФИО" ), m_pColModel->m_col_fio );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Телефон(ы)" ), m_pColModel->m_col_phone );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Состояние" ), m_pColModel->m_col_state );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Услуга типа \"Пакет\"" ), m_pColModel->m_col_has_packet );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Услуга типа \"Посадка\"" ), m_pColModel->m_col_has_landing );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Примечание" ), m_pColModel->m_col_comment );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHReportLanding::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id			] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_fio			] = Glib::locale_to_utf8( obj->get( "fio" )->toString( ) );
			row[ m_pColModel->m_col_phone		] = Glib::locale_to_utf8( obj->get( "phone" )->toString( ) );
			row[ m_pColModel->m_col_state		] = Glib::locale_to_utf8( obj->get( "state" )->toString( ) );
			row[ m_pColModel->m_col_has_packet	] = Glib::locale_to_utf8( obj->get( "has_packet" )->toString( ) );
			row[ m_pColModel->m_col_has_landing	] = Glib::locale_to_utf8( obj->get( "has_landing" )->toString( ) );
			row[ m_pColModel->m_col_comment		] = Glib::locale_to_utf8( obj->get( "comment" )->toString( ) );
		}
	} // void Load

	void CHReportLanding::FilterUiClear( ) {
	} // void FilterUiClear

	void CHReportLanding::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
		filter->get( "group" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltGroup ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	void CHReportLanding::OnButtonLoad( ) {
	} // void OnButtonLoad

	void CHReportLanding::OnButtonExport( ) {
	} // void OnButtonExport

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHReportLandingModeless::CHReportLandingModeless( ) :
		udListHandlerModeless( ),
		CHReportLanding( )
	{
	} // CHReportLandingModeless

	CHReportLandingModeless::CHReportLandingModeless( const CHReportLandingModeless& objCopy ) : CHReportLanding( ) {
	} // CHReportLandingModeless

	CHReportLandingModeless::~CHReportLandingModeless( ) {
	} // ~CHReportLandingModeless

	CHReportLandingModeless& CHReportLandingModeless::operator =( const CHReportLandingModeless& objCopy ) {
		return *this;
	} // CHReportLandingModeless& operator =

	void CHReportLandingModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHReportLanding::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace UdSDK
