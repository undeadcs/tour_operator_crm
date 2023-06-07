
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHClDocListPick( ) {
		return new CHClDocPick;
	} // udPListHandlerPick GetHClDocListPick

	udPBase GetProjectClDocList( ) {
		udPBase ret = GetProjectClDoc( );
		ret->get( "client_" )->ini( wstring( L"" ) );
		ret->get( "type_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectClDocList

	CHClDoc::CHClDoc( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltClient( Glib::locale_to_utf8( "Клиент" ) ),
		m_lblFltType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblFltSeries( Glib::locale_to_utf8( "Серия" ) ),
		m_lblFltNumber( Glib::locale_to_utf8( "Номер" ) )
	{
		m_szName = "cldoc";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать документ клиента" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "series" )->ini( wstring( L"" ) );
		filter->get( "number" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHClDoc::ColModelClDoc( );

		m_pData->SetName( "cldoc" );
		m_pData->SetGetObject( GetProjectClDocList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClient );
		pFde->SetMark( fltClient );
		m_pData->AddExtraData( pFde );
		m_inpFltClient.SetDataObject( pFde );
		m_inpFltClient.SetWindowTitle( Glib::locale_to_utf8( "Выбор клиента" ) );
		m_inpFltClient.SetAttr( "fio" );
		m_inpFltClient.SetGetListHandler( GetHClientListPick );
		m_inpFltClient.SetGetFormHandler( GetHClientForm );
		m_inpFltClient.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование клиента" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDocType );
		pFde->SetMark( fltType );
		m_pData->AddExtraData( pFde );
		m_inpFltType.SetDataObject( pFde );
		m_inpFltType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа" ) );
		m_inpFltType.SetAttr( "name" );
		m_inpFltType.SetGetListHandler( GetHDocTypeListPick );
		m_inpFltType.SetGetFormHandler( GetHDocTypeForm );
		m_inpFltType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование типа документа" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Клиент
		m_tblFilter.attach( m_lblFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Тип
		m_tblFilter.attach( m_lblFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Серия
		m_tblFilter.attach( m_lblFltSeries, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltSeries, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Номер
		m_tblFilter.attach( m_lblFltNumber, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltNumber, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHClDoc

	CHClDoc::CHClDoc( const CHClDoc& objCopy ) : udListHandler( ) {
		m_szName = "cldoc";
	} // CHClDoc

	CHClDoc::~CHClDoc( ) {
	} // ~CClDoc

	CHClDoc& CHClDoc::operator =( const CHClDoc& objCopy ) {
		return *this;
	} // CHClDoc& operator =

	void CHClDoc::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Клиент" ), m_pColModel->m_col_client );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тип" ), m_pColModel->m_col_type );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Серия" ), m_pColModel->m_col_series );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Номер" ), m_pColModel->m_col_number );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHClDoc::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_client ] = Glib::locale_to_utf8( obj->get( "client_" )->toString( ) );
			row[ m_pColModel->m_col_type ] = Glib::locale_to_utf8( obj->get( "type_" )->toString( ) );
			row[ m_pColModel->m_col_series ] = Glib::locale_to_utf8( obj->get( "series" )->toString( ) );
			row[ m_pColModel->m_col_number ] = Glib::locale_to_utf8( obj->get( "number" )->toString( ) );
			row[ m_pColModel->m_col_object ] = obj;
		}
	} // void Load

	void CHClDoc::FilterUiClear( ) {
		m_inpFltClient.SetTitle( "" );
		m_inpFltType.SetTitle( "" );
		m_inpFltNumber.Reset( );
		m_inpFltSeries.Reset( );
	} // void FilterUiClear

	void CHClDoc::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szNumber,
						szSeries;

		szSeries = m_inpFltSeries.GetValue( );
		szNumber = m_inpFltNumber.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "client" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltClient ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "series" )->ini( ConvertString( szSeries.data( ), CP_UTF8 ) );
		filter->get( "number" )->ini( ConvertString( szNumber.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClDocModeless::CHClDocModeless( ) :
		udListHandlerModeless( ),
		CHClDoc( )
	{
	} // CHClDocModeless

	CHClDocModeless::CHClDocModeless( const CHClDocModeless& objCopy ) : CHClDoc( ) {
	} // CHClDocModeless

	CHClDocModeless::~CHClDocModeless( ) {
	} // ~CHClDocModeless

	CHClDocModeless& CHClDocModeless::operator =( const CHClDocModeless& objCopy ) {
		return *this;
	} // CHClDocModeless& operator =

	void CHClDocModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHClDoc::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHClDocModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "cldoc_form" );
	} // void OnButtonForm

	void CHClDocModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "cldoc_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "cldoc_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClDocPick::CHClDocPick( ) : CHClDoc( ) {
	} // CHClDocPick

	CHClDocPick::CHClDocPick( const CHClDocPick& objCopy ) : CHClDoc( ) {
	} // CHClDocPick

	CHClDocPick::~CHClDocPick( ) {
	} // ~CHClDocPick

	CHClDocPick& CHClDocPick::operator =( const CHClDocPick& objCopy ) {
		return *this;
	} // CHClDocPick& operator =

	void CHClDocPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHClDoc::Init( );
	} // void CHClDocPick

	void CHClDocPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHClDocFormModal handler = new udHClDocFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание документа клиента" ) );
		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание документа клиента" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHClDocPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectClDoc( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите документ клиента" ),
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

	void CHClDocPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHClDocPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHClDocFormModal handler = new udHClDocFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование документа клиента" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование документа клиента" ) );
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

	udPFormHandler GetHClDocForm( ) {
		return new CHClDocFormModal;
	} // udPFormHandler GetHClDocForm

	CHClDocForm::CHClDocForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Документ клиента" ) ),
		m_lblClient( Glib::locale_to_utf8( "Клиент" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblSeries( Glib::locale_to_utf8( "Серия" ) ),
		m_lblNumber( Glib::locale_to_utf8( "Номер" ) ),
		m_lblIssueDate( Glib::locale_to_utf8( "Дата выдачи" ) ),
		m_lblEndDate( Glib::locale_to_utf8( "Дата окончания" ) ),
		m_lblIssuedBy( Glib::locale_to_utf8( "Кем выдан" ) ),
		m_lblBirthPlace( Glib::locale_to_utf8( "Место рождения" ) ),
		m_lblFio( Glib::locale_to_utf8( "ФИО" ) ),
		m_lblFioLatin( Glib::locale_to_utf8( "ФИО (латиницей)" ) ),
		m_inpIssueDate( datetime( 0, true ), datetime( 0, true ) ),
		m_inpEndDate( datetime( 0, true ), datetime( 0, true ) )
	{
		m_szName = "cldoc_form";

		m_pData->SetName( "cldoc" );
		m_pData->SetGetObject( GetProjectClDoc );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );

		m_lblClient.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblType.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblSeries.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblNumber.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblIssueDate.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblEndDate.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblIssuedBy.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblBirthPlace.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblFio.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblFioLatin.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );

		udPFDEObject pFde = NULL;

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClient );
		pFde->SetMark( ctrlidClient );
		m_pData->AddExtraData( pFde );
		m_inpClient.SetDataObject( pFde );
		m_inpClient.SetWindowTitle( Glib::locale_to_utf8( "Выбор клиента" ) );
		m_inpClient.SetAttr( "fio" );
		m_inpClient.SetGetListHandler( GetHClientListPick );
		m_inpClient.SetGetFormHandler( GetHClientForm );
		m_inpClient.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование клиента" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDocType );
		pFde->SetMark( ctrlidType );
		m_pData->AddExtraData( pFde );
		m_inpType.SetDataObject( pFde );
		m_inpType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа" ) );
		m_inpType.SetAttr( "name" );
		m_inpType.SetGetListHandler( GetHDocTypeListPick );
		m_inpType.SetGetFormHandler( GetHDocTypeForm );
		m_inpType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование типа документа" ) );
	} // CHClDocForm

	CHClDocForm::CHClDocForm( const CHClDocForm& objCopy ) : udFormHandler( ) {
	} // CHClDocForm

	CHClDocForm::~CHClDocForm( ) {
	} // ~CHClDocForm

	CHClDocForm& CHClDocForm::operator =( const CHClDocForm& objCopy ) {
		return *this;
	} // CHClDocForm& operator =

	void CHClDocForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Клиент
		m_tblInput.attach( m_lblClient, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpClient, 1, 2, 0, 1, Gtk::FILL, Gtk::SHRINK );

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK );

		// Серия
		m_tblInput.attach( m_lblSeries, 0, 1, 2, 3, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSeries, 1, 2, 2, 3, Gtk::FILL, Gtk::SHRINK );

		// Номер
		m_tblInput.attach( m_lblNumber, 0, 1, 3, 4, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNumber, 1, 2, 3, 4, Gtk::FILL, Gtk::SHRINK );

		// Дата выдачи
		m_tblInput.attach( m_lblIssueDate, 0, 1, 4, 5, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpIssueDate, 1, 2, 4, 5, Gtk::FILL, Gtk::SHRINK );

		// Дата окончания
		m_tblInput.attach( m_lblEndDate, 0, 1, 5, 6, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpEndDate, 1, 2, 5, 6, Gtk::FILL, Gtk::SHRINK );

		// Кем выдан
		m_tblInput.attach( m_lblIssuedBy, 0, 1, 6, 7, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpIssuedBy, 1, 2, 6, 7, Gtk::FILL, Gtk::SHRINK );

		// Место рождения
		m_tblInput.attach( m_lblBirthPlace, 0, 1, 7, 8, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpBirthPlace, 1, 2, 7, 8, Gtk::FILL, Gtk::SHRINK );

		// ФИО
		m_tblInput.attach( m_lblFio, 0, 1, 8, 9, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFio, 1, 2, 8, 9, Gtk::FILL, Gtk::SHRINK );

		// ФИО (латиницей)
		m_tblInput.attach( m_lblFioLatin, 0, 1, 9, 10, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFioLatin, 1, 2, 9, 10, Gtk::FILL, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 0, 2, 10, 11, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHClDocForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpSeries.Reset( );
		m_inpNumber.Reset( );
		m_inpIssuedBy.Reset( );
		m_inpBirthPlace.Reset( );
		m_inpFio.Reset( );
		m_inpFioLatin.Reset( );
		m_inpClient.SetTitle( "" );
		m_inpType.SetTitle( "" );
		m_inpIssueDate.Reset( );
		m_inpEndDate.Reset( );
	} // void Clear

	void CHClDocForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpSeries.SetValue( Glib::locale_to_utf8( obj->get( "series" )->toString( ) ) );
		m_inpNumber.SetValue( Glib::locale_to_utf8( obj->get( "number" )->toString( ) ) );
		m_inpIssuedBy.SetValue( Glib::locale_to_utf8( obj->get( "issuedby" )->toString( ) ) );
		m_inpBirthPlace.SetValue( Glib::locale_to_utf8( obj->get( "birthplace" )->toString( ) ) );
		m_inpFio.SetValue( Glib::locale_to_utf8( obj->get( "fio" )->toString( ) ) );
		m_inpFioLatin.SetValue( Glib::locale_to_utf8( obj->get( "fiolatin" )->toString( ) ) );
		m_inpClient.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( )->get( "fio" )->toString( )
		) );
		m_inpType.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpIssueDate.SetValue( obj->get( "issuedate" )->toTime( ) );
		m_inpEndDate.SetValue( obj->get( "enddate" )->toTime( ) );
	} // void Load

	udInt CHClDocForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szSeries,
						szNumber,
						szIssuedBy,
						szBirthPlace,
						szFio,
						szFioLatin;
		
		szSeries = m_inpSeries.GetValue( );
		if ( szSeries.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите серию" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpSeries ).GetEntry( ) );
			return 1;
		}

		szNumber = m_inpNumber.GetValue( );
		if ( szNumber.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите номер" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpNumber ).GetEntry( ) );
			return 2;
		}

		szIssuedBy = m_inpIssuedBy.GetValue( );
		if ( szIssuedBy.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите кем выдан" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpIssuedBy ).GetEntry( ) );
			return 3;
		}

		szBirthPlace = m_inpBirthPlace.GetValue( );
		if ( szBirthPlace.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите место рождения" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpBirthPlace ).GetEntry( ) );
			return 4;
		}

		szFio = m_inpFio.GetValue( );
		if ( szFio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFio ).GetEntry( ) );
			return 5;
		}

		szFioLatin = m_inpFioLatin.GetValue( );
		if ( szFioLatin.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО латиницей" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFioLatin ).GetEntry( ) );
			return 6;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите клиента" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpClient.GetButton( ) ) );
			return 7;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тип" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType ).GetButton( ) );
			return 8;
		}

		return 0;
	} // udInt Validate

	void CHClDocForm::Send( ) {
		Glib::ustring	szSeries,
						szNumber,
						szIssuedBy,
						szBirthPlace,
						szFio,
						szFioLatin;
		
		szSeries = m_inpSeries.GetValue( );
		szNumber = m_inpNumber.GetValue( );
		szIssuedBy = m_inpIssuedBy.GetValue( );
		szBirthPlace = m_inpBirthPlace.GetValue( );
		szFio = m_inpFio.GetValue( );
		szFioLatin = m_inpFioLatin.GetValue( );
		
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		
		obj->get( "series" )->ini( ConvertString( szSeries.data( ), CP_UTF8 ) );
		obj->get( "number" )->ini( ConvertString( szNumber.data( ), CP_UTF8 ) );
		obj->get( "issuedby" )->ini( ConvertString( szIssuedBy.data( ), CP_UTF8 ) );
		obj->get( "birthplace" )->ini( ConvertString( szBirthPlace.data( ), CP_UTF8 ) );
		obj->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		obj->get( "fiolatin" )->ini( ConvertString( szFioLatin.data( ), CP_UTF8 ) );
		obj->get( "client" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "issuedate" )->ini( m_inpIssueDate.GetValue( ) );
		obj->get( "enddate" )->ini( m_inpEndDate.GetValue( ) );
		
		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClDocFormModal::CHClDocFormModal( ) : CHClDocForm( ), m_iClientId( 0 ) {
	} // CHClDocFormModal

	CHClDocFormModal::CHClDocFormModal( udUInt iClientId, bool bShowClientField, bool bSend ) :
		CHClDocForm( ),
		m_iClientId( iClientId ),
		m_bSend( bSend ),
		m_bShowClientField( bShowClientField )
	{
	} // CHClDocFormModal

	CHClDocFormModal::CHClDocFormModal( const CHClDocFormModal& objCopy ) : CHClDocForm( ) {
	} // CHClDocFormModal

	CHClDocFormModal::~CHClDocFormModal( ) {
	} // ~CHClDocFormModal

	CHClDocFormModal& CHClDocFormModal::operator =( const CHClDocFormModal& objCopy ) {
		return *this;
	} // CHClDocFormModal& operator =

	void CHClDocFormModal::Init( ) {
		udInt from = 0, to = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		if ( m_bShowClientField ) {
			// Клиент
			m_tblInput.attach( m_lblClient, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			m_tblInput.attach( m_inpClient, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );
		}

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// Серия
		m_tblInput.attach( m_lblSeries, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSeries, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// Номер
		m_tblInput.attach( m_lblNumber, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNumber, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата выдачи
		m_tblInput.attach( m_lblIssueDate, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpIssueDate, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата окончания
		m_tblInput.attach( m_lblEndDate, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpEndDate, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// Кем выдан
		m_tblInput.attach( m_lblIssuedBy, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpIssuedBy, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// Место рождения
		m_tblInput.attach( m_lblBirthPlace, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpBirthPlace, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// ФИО
		m_tblInput.attach( m_lblFio, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFio, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// ФИО (латиницей)
		m_tblInput.attach( m_lblFioLatin, 0, 1, from, to, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFioLatin, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, from++, to++, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	void CHClDocFormModal::Clear( ) {
		udFormHandler::Clear( );

		m_inpSeries.Reset( );
		m_inpNumber.Reset( );
		m_inpIssuedBy.Reset( );
		m_inpBirthPlace.Reset( );
		m_inpFio.Reset( );
		m_inpFioLatin.Reset( );
		if ( m_bShowClientField ) {
			m_inpClient.SetTitle( "" );
		}
		m_inpType.SetTitle( "" );

		m_inpIssueDate.Reset( );
		m_inpEndDate.Reset( );
	} // void Clear

	udInt CHClDocFormModal::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szSeries,
						szNumber,
						szIssuedBy,
						szBirthPlace,
						szFio,
						szFioLatin;
		
		szSeries = m_inpSeries.GetValue( );
		if ( szSeries.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите серию" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpSeries.GetEntry( ) ) );
			return 1;
		}

		szNumber = m_inpNumber.GetValue( );
		if ( szNumber.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите номер" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpNumber.GetEntry( ) ) );
			return 2;
		}

		szIssuedBy = m_inpIssuedBy.GetValue( );
		if ( szIssuedBy.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите кем выдан" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpIssuedBy.GetEntry( ) ) );
			return 3;
		}

		szBirthPlace = m_inpBirthPlace.GetValue( );
		if ( szBirthPlace.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите место рождения" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpBirthPlace.GetEntry( ) ) );
			return 4;
		}

		szFio = m_inpFio.GetValue( );
		if ( szFio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFio.GetEntry( ) ) );
			return 5;
		}

		szFioLatin = m_inpFioLatin.GetValue( );
		if ( szFioLatin.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО латиницей" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFioLatin.GetEntry( ) ) );
			return 6;
		}

		udPBase obj = NULL;
		
		if ( m_bShowClientField ) {
			obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( );
			if ( !obj || !obj->get( "id" )->toUInt( ) ) {
				mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите клиента" ) );
				g_pApp->GetWndWork( )->set_focus( *( m_inpClient.GetButton( ) ) );
				return 7;
			}
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тип" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType.GetButton( ) ) );
			return 8;
		}

		return 0;
	} // udInt Validate

	void CHClDocFormModal::Send( ) {
		Glib::ustring	szSeries,
						szNumber,
						szIssuedBy,
						szBirthPlace,
						szFio,
						szFioLatin;
		
		szSeries = m_inpSeries.GetValue( );
		szNumber = m_inpNumber.GetValue( );
		szIssuedBy = m_inpIssuedBy.GetValue( );
		szBirthPlace = m_inpBirthPlace.GetValue( );
		szFio = m_inpFio.GetValue( );
		szFioLatin = m_inpFioLatin.GetValue( );
		
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		
		obj->get( "series" )->ini( ConvertString( szSeries.data( ), CP_UTF8 ) );
		obj->get( "number" )->ini( ConvertString( szNumber.data( ), CP_UTF8 ) );
		obj->get( "issuedby" )->ini( ConvertString( szIssuedBy.data( ), CP_UTF8 ) );
		obj->get( "birthplace" )->ini( ConvertString( szBirthPlace.data( ), CP_UTF8 ) );
		obj->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		obj->get( "fiolatin" )->ini( ConvertString( szFioLatin.data( ), CP_UTF8 ) );
		if ( m_bShowClientField ) {
			obj->get( "client" )->ini(
				( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( )->get( "id" )->toUInt( )
			);
		} else {
			obj->get( "client" )->ini( m_iClientId );
		}
		obj->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "issuedate" )->ini( m_inpIssueDate.GetValue( ) );
		obj->get( "enddate" )->ini( m_inpEndDate.GetValue( ) );
		
		if ( m_bSend ) {
			m_pData->Send( );
		}
	} // void Send

	void CHClDocFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHClDocFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHClDocForm

	void CHClDocFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		// тип документа
		udPBase	pDocType	= ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( );
		udUInt	id			= pDocType->get( "id" )->toUInt( ),
				type		= obj->get( "type" )->toUInt( );
		
		if ( ( id == 0 ) && ( type > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "doctype", type, GetProjectDocType );
			if ( pLoaded ) {
				pDocType->apply( pLoaded, true, true );
				udDELETE( pLoaded );
			}
		}

		m_inpSeries.SetValue( Glib::locale_to_utf8( obj->get( "series" )->toString( ) ) );
		m_inpNumber.SetValue( Glib::locale_to_utf8( obj->get( "number" )->toString( ) ) );
		m_inpIssuedBy.SetValue( Glib::locale_to_utf8( obj->get( "issuedby" )->toString( ) ) );
		m_inpBirthPlace.SetValue( Glib::locale_to_utf8( obj->get( "birthplace" )->toString( ) ) );
		m_inpFio.SetValue( Glib::locale_to_utf8( obj->get( "fio" )->toString( ) ) );
		m_inpFioLatin.SetValue( Glib::locale_to_utf8( obj->get( "fiolatin" )->toString( ) ) );
		if ( m_bShowClientField ) {
			m_inpClient.SetTitle( Glib::locale_to_utf8(
				( ( udPFDEObject ) m_pData->GetExtraData( ctrlidClient ) )->GetObject( )->get( "fio" )->toString( )
			) );
		}
		m_inpType.SetTitle( Glib::locale_to_utf8(
			pDocType->get( "name" )->toString( )
		) );

		m_inpIssueDate.SetValue( obj->get( "issuedate" )->toTime( ) );
		m_inpEndDate.SetValue( obj->get( "enddate" )->toTime( ) );
	} // void LoadFrom

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClDocFormModeless::CHClDocFormModeless( ) : CHClDocForm( ) {
	} // CHClDocFormModeless

	CHClDocFormModeless::CHClDocFormModeless( const CHClDocFormModeless& objCopy ) : CHClDocForm( ) {
	} // CHClDocFormModeless

	CHClDocFormModeless::~CHClDocFormModeless( ) {
	} // ~CHClDocFormModeless

	CHClDocFormModeless& CHClDocFormModeless::operator =( const CHClDocFormModeless& objCopy ) {
		return *this;
	} // CHClDocFormModeless& operator =

	void CHClDocFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "cldoc" );
	} // void OnButtonOk

	void CHClDocFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "cldoc" );
	} // void CHClDocForm

	void CHClDocFormModeless::Init( ) {
		CHClDocForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHClDocFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHClDocFormModeless

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CWgtClDocFromForm::CWgtClDocFromForm( ) : udWgtListPick( ), m_pForm( NULL ) {
	} // CWgtClDocFromForm

	CWgtClDocFromForm::CWgtClDocFromForm( const CWgtClDocFromForm& objCopy ) : udWgtListPick( ), m_pForm( NULL ) {
	} // CWgtClDocFromForm

	CWgtClDocFromForm::~CWgtClDocFromForm( ) {
	} // ~CWgtClDocFromForm

	CWgtClDocFromForm& CWgtClDocFromForm::operator =( const CWgtClDocFromForm& objCopy ) {
		return *this;
	} // CWgtClDocFromForm& operator =

	void CWgtClDocFromForm::OnButtonClick( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		udPHClDocFromFormPick handler = new udHClDocFromFormPick;
		handler->SetForm( m_pForm );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szWindowTitle );

		udPBase obj = handler->GetObject( );
		if ( obj && m_pFde ) {
			string log = "picked object, ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			if ( m_pForm ) {
				udPBase add = NULL,
						obj1 = NULL,
						doc = ( udPBase ) obj->get( "orig_object" )->get( );

				if ( doc ) {
					log = "doc, ";
					doc->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );
				} else {
					g_pApp->GetLog( )->Write( "orig_object is NULL\n" );
				}

				udPArrBase arrObject = ( ( udPFDEList ) m_pForm->GetData( )->GetExtraData( udHClientForm::ctrlidDoc ) )->GetArray( );
				if ( arrObject && !arrObject->empty( ) ) {
					//char tmp[ 256 ];
					for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
						obj1 = *i;

						if ( doc == obj1 ) {
							obj1->get( "is_default" )->ini( ( udUInt ) 1 );
						} else {
							obj1->get( "is_default" )->ini( ( udUInt ) 0 );
						}
						/*log = "\n";
						( *i )->dump( log );
						memset( tmp, 0, 256 );
						sprintf_s( tmp, 256, "%p, %p", doc, obj1 );
						g_pApp->GetLog( )->Write( string( tmp ) + log + "\n" );//*/
					}
				}
			}

			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );
			
			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8( obj->get( "type_" )->toString( ) ) );
			}
		}

		udDELETE( handler );
	} // void OnButtonClick

	bool CWgtClDocFromForm::OnClick( GdkEventButton* evtClick ) {
		/*if ( !m_pFde || !m_pFde->GetObject( ) || !m_pFde->GetObject( )->get( "id" )->toUInt( ) || !m_fnMakeFormHandler ) {
			return false;
		}

		Gtk::Window *wndParent = ( Gtk::Window* ) get_toplevel( );
		udPFormHandler handler = m_fnMakeFormHandler( );
		handler->Init( );
		handler->GetData( )->SetId( m_pFde->GetObject( )->get( "id" )->toUInt( ) );
		handler->Load( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szFormWindowTitle );

		udPBase obj = handler->GetData( )->GetData( );
		if ( obj ) {
			string log = "modified object, ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );
			
			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8( obj->get( m_szAttr )->toString( ) ) );
			}
		}

		udDELETE( handler );//*/

		return true;
	} // bool OnClick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CDAListClDocFromForm::CDAListClDocFromForm( ) : udDAList( ), m_pForm( NULL ) {
	} // CDAListClDocFromForms

	CDAListClDocFromForm::CDAListClDocFromForm( const CDAListClDocFromForm& objCopy ) : udDAList( ), m_pForm( NULL ) {
	} // CDAListClDocFromForms

	CDAListClDocFromForm::~CDAListClDocFromForm( ) {
	} // ~CDAListClDocFromForm

	CDAListClDocFromForm& CDAListClDocFromForm::operator =( const CDAListClDocFromForm& objCopy ) {
		return *this;
	} // CDAListClDocFromForm& operator =

	void CDAListClDocFromForm::Load( ) {
		if ( m_pForm ) {
			udPBase add = NULL;
			udPArrBase arrObject = ( ( udPFDEList ) m_pForm->GetData( )->GetExtraData( udHClientForm::ctrlidDoc ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = new udProjectClDoc;
					( *i )->apply( add );
					add->get( "orig_object" )->ini( ( udPVoid ) *i );
					m_arrObject->push_back( add );
				}
			}
		}
	} // void Load

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPListHandlerPick GetHClDocFromFormListPick( ) {
		return new CHClDocFromFormPick;
	} // udPHListHandlerPick GetHClDocFromFormListPick

	CHClDocFromFormPick::CHClDocFromFormPick( ) : CHClDoc( ), m_pForm( NULL ) {
		udDELETE( m_pData );
		m_pData = new udDAListClDocFromForm;
	} // CHCHClDocFromFormPick

	CHClDocFromFormPick::CHClDocFromFormPick( const CHClDocFromFormPick& objCopy ) : CHClDoc( ) {
	} // CHCHClDocFromFormPick

	CHClDocFromFormPick::~CHClDocFromFormPick( ) {
	} // ~CHCHClDocFromFormPick

	CHClDocFromFormPick& CHClDocFromFormPick::operator =( const CHClDocFromFormPick& objCopy ) {
		return *this;
	} // CHCHClDocFromFormPick& operator =

	void CHClDocFromFormPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		//m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		//m_objTreeView.append_column( Glib::locale_to_utf8( "Клиент" ), m_pColModel->m_col_client );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тип" ), m_pColModel->m_col_type );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Серия" ), m_pColModel->m_col_series );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Номер" ), m_pColModel->m_col_number );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void CHCHClDocFromFormPick

	void CHClDocFromFormPick::OnButtonForm( ) {
		/*Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHClDocFormModal handler = new udHClDocFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание документа клиента" ) );
		
		udDELETE( handler );

		Clear( );
		Load( );//*/
	} // void OnButtonForm

	void CHClDocFromFormPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			udPBase obj = row[ m_pColModel->m_col_object ];
			m_pPickedObject = GetProjectClDoc( );
			obj->apply( m_pPickedObject );
			m_pPickedObject->get( "orig_object" )->ini( ( udPVoid ) obj->get( "orig_object" )->get( ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите документ клиента" ),
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

	void CHClDocFromFormPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHClDocFromFormPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		/*Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHClDocFormModal handler = new udHClDocFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование документа клиента" ) );

			udDELETE( handler );

			Clear( );
			Load( );
		}//*/
	} // void OnRowActivated

	void CHClDocFromFormPick::SetForm( udPHClientForm pForm ) {
		m_pForm = pForm;
		( ( udPDAListClDocFromForm ) m_pData )->SetForm( pForm );
	} // void SetForm

} // namespace UdSDK
