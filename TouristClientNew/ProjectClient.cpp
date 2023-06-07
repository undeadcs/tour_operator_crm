
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHClientListPick( ) {
		return new CHClientPick;
	} // udPListHandlerPick GetHClientListPick

	udPBase GetProjectClientList( ) {
		udPBase ret = GetProjectClient( );
		return ret;
	} // udPBase GetProjectClientList

	CHClient::CHClient( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltFio( Glib::locale_to_utf8( "ФИО" ) ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата рождения с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) )
	{
		m_szName = "client";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать клиента" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "fio" )->ini( wstring( L"" ) );
		filter->get( "date1" )->ini( wstring( L"" ) );
		filter->get( "date2" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHClient::ColModelClient( );

		m_pData->SetName( "client" );
		m_pData->SetGetObject( GetProjectClientList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// ФИО
		m_tblFilter.attach( m_lblFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата рождения с
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата рождения по
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHClient

	CHClient::CHClient( const CHClient& objCopy ) : udListHandler( ) {
		m_szName = "client";
	} // CHClient

	CHClient::~CHClient( ) {
	} // ~CClient

	CHClient& CHClient::operator =( const CHClient& objCopy ) {
		return *this;
	} // CHClient& operator =

	void CHClient::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "ФИО" ), m_pColModel->m_col_fio );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата рождения" ), m_pColModel->m_col_born );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHClient::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_fio ] = Glib::locale_to_utf8( obj->get( "fio" )->toString( ) );
			row[ m_pColModel->m_col_born ] = Glib::locale_to_utf8( obj->get( "born" )->toString( ) );
		}
	} // void Load

	void CHClient::FilterUiClear( ) {
		m_inpFltFio.Reset( );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHClient::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szFio;
		wstring			szDate1,
						szDate2;

		szFio = m_inpFltFio.GetValue( );
		szDate1 = m_inpFltDate1.GetValue( ).toWString( "%Y-%m-%d" );
		szDate2 = m_inpFltDate2.GetValue( ).toWString( "%Y-%m-%d" );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		filter->get( "date1" )->ini( szDate1 );
		filter->get( "date2" )->ini( szDate2 );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientModeless::CHClientModeless( ) :
		udListHandlerModeless( ),
		CHClient( ),
		m_btnDoc( Glib::locale_to_utf8( "Документы" ) ),
		m_btnUr( Glib::locale_to_utf8( "Клиенты (юр.)" ) )
	{
		m_btnDoc.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"cldoc"
		) );
		m_btnUr.signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			"clientur"
		) );
	} // CHClientModeless

	CHClientModeless::CHClientModeless( const CHClientModeless& objCopy ) : CHClient( ) {
	} // CHClientModeless

	CHClientModeless::~CHClientModeless( ) {
	} // ~CHClientModeless

	CHClientModeless& CHClientModeless::operator =( const CHClientModeless& objCopy ) {
		return *this;
	} // CHClientModeless& operator =

	void CHClientModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		m_hbbTop.pack_start( m_btnDoc, false, false, 0 );
		m_hbbTop.pack_start( m_btnUr, false, false, 0 );
		CHClient::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHClientModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "client_form" );
	} // void OnButtonForm

	void CHClientModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "client_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "client_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientPick::CHClientPick( ) : CHClient( ) {
	} // CHClientPick

	CHClientPick::CHClientPick( const CHClientPick& objCopy ) : CHClient( ) {
	} // CHClientPick

	CHClientPick::~CHClientPick( ) {
	} // ~CHClientPick

	CHClientPick& CHClientPick::operator =( const CHClientPick& objCopy ) {
		return *this;
	} // CHClientPick& operator =

	void CHClientPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHClient::Init( );
	} // void CHClientPick

	void CHClientPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHClientFormModal handler = new udHClientFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание клиента" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание клиента" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHClientPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectClient( );
			Glib::ustring szFio = row[ m_pColModel->m_col_fio ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			mb::err(
				*wndParent,
				Glib::locale_to_utf8( "Выберите клиента" ),
				Glib::locale_to_utf8( "Щелкните по строке списка и нажмите кнопку \"ОК\"" )
			);
		}
	} // void OnButtonOk

	void CHClientPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHClientPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHClientFormModal handler = new udHClientFormModal;
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование клиента" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование клиента" ) );
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

	udPBase GetProjectClDoc2( ) {
		udPBase ret = GetProjectClDoc( );
		ret->get( "client_" )->ini( wstring( L"" ) );
		ret->get( "type_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectClDoc2

	udPFormHandler GetHClientForm( ) {
		return new CHClientFormModal;
	} // udPFormHandler GetHClientForm

	CHClientForm::CHClientForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Клиент" ) ),
		m_lblFio( Glib::locale_to_utf8( "ФИО" ) ),
		m_lblFioLatin( Glib::locale_to_utf8( "ФИО (латиницей)" ) ),
		m_lblSex( Glib::locale_to_utf8( "Пол" ) ),
		m_lblEmail( Glib::locale_to_utf8( "E-mail" ) ),
		m_lblPhone( Glib::locale_to_utf8( "Телефон" ) ),
		m_lblAddr( Glib::locale_to_utf8( "Адрес" ) ),
		m_lblBorn( Glib::locale_to_utf8( "Дата рождения" ) ),
		m_lblDoc( Glib::locale_to_utf8( "Документы" ) ),
		m_lblApp( Glib::locale_to_utf8( "Приложение" ) ),
		m_lblBornPlace( Glib::locale_to_utf8( "Место рождения" ) ),
		m_lblBornPlaceLatin( Glib::locale_to_utf8( "Место рождения латиницей" ) ),
		m_lblDefaultDoc( Glib::locale_to_utf8( "Документ по умолчанию" ) ),
		m_inpBorn( datetime( 0, true ), datetime( 0, true ) )
	{
		m_szName = "client_form";
		
		m_pData->SetName( "client" );
		m_pData->SetGetObject( GetProjectClient );
		
		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblFio.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDoc.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblApp.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblFio.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpSex.AddRow( udProjectClient::SEX_MAN, Glib::locale_to_utf8( "Мужской" ) );
		m_inpSex.AddRow( udProjectClient::SEX_WOMAN, Glib::locale_to_utf8( "Женский" ) );
		m_inpSex.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClDoc2 );
		pFde->SetMark( ctrlidDefaultDoc );
		m_pData->AddExtraData( pFde );
		m_inpDefaultDoc.SetDataObject( pFde );
		m_inpDefaultDoc.SetWindowTitle( Glib::locale_to_utf8( "Выбор документа" ) );
		m_inpDefaultDoc.SetAttr( "type_" );
		m_inpDefaultDoc.SetForm( this );
		//m_inpDefaultDoc.SetGetListHandler( GetHCountryListPick );
		//m_inpDefaultDoc.SetGetFormHandler( GetHCountryForm );
		m_inpDefaultDoc.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование документа" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Gtk::TreeView *pTree = NULL;
		udPFDEList pFdeList = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// документы
		m_inpDoc.set_size_request( 400, 200 );
		m_pModelFormListDoc = new ColModelFormListDoc;
		m_objDocModel = Gtk::ListStore::create( *m_pModelFormListDoc );
		
		pTree = m_inpDoc.GetTreeView( );
		pTree->set_model( m_objDocModel );
		pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListDoc->m_col_type );
		pTree->append_column( Glib::locale_to_utf8( "Серия" ), m_pModelFormListDoc->m_col_series );
		pTree->append_column( Glib::locale_to_utf8( "Номер" ), m_pModelFormListDoc->m_col_number );
		m_inpDoc.SetColumnDefaults( );

		m_btnDocEdit = m_inpDoc.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnDocEdit->set_sensitive( false );
		m_btnDocEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListDoc_Edit ) );

		m_btnDocDel = m_inpDoc.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnDocDel->set_sensitive( false );
		m_btnDocDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListDoc_Del ) );

		m_btnDocAdd = m_inpDoc.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnDocAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListDoc_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListDoc_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectClDocList );
		pFdeList->SetCallbackSend( GetProjectClDoc );
		pFdeList->SetMark( ctrlidDoc );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// приложения
		m_inpApp.set_size_request( 400, 200 );
		m_pModelFormListApp = new ColModelFormListApp;
		m_objAppModel = Gtk::ListStore::create( *m_pModelFormListApp );

		pTree = m_inpApp.GetTreeView( );
		pTree->set_model( m_objAppModel );
		pTree->append_column( Glib::locale_to_utf8( "Имя файла" ), m_pModelFormListApp->m_col_name );
		m_inpApp.SetColumnDefaults( );

		m_btnAppEdit = m_inpApp.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnAppEdit->set_sensitive( false );
		m_btnAppEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListApp_Edit ) );

		m_btnAppDel = m_inpApp.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnAppDel->set_sensitive( false );
		m_btnAppDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListApp_Del ) );

		m_btnAppReplace = m_inpApp.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnAppReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListApp_Replace ) );
		m_btnAppReplace->set_sensitive( false );

		m_btnAppAdd = m_inpApp.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnAppAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListApp_Add ) );

		m_btnFolder = m_inpApp.AddButton( Glib::locale_to_utf8( "Открыть папку" ) );
		m_btnFolder->signal_clicked( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListApp_Folder ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHClientForm::OnFormListApp_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectAttach );
		pFdeList->SetMark( ctrlidApp );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHClientForm

	CHClientForm::CHClientForm( const CHClientForm& objCopy ) : udFormHandler( ) {
	} // CHClientForm

	CHClientForm::~CHClientForm( ) {
	} // ~CHClientForm

	CHClientForm& CHClientForm::operator =( const CHClientForm& objCopy ) {
		return *this;
	} // CHClientForm& operator =

	void CHClientForm::Init( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// приложение
		m_szFolderApp = g_pApp->GetUniqueTmpFolder( );
		m_pData->SetHasFiles( true );
		m_pData->SetFolder( m_szFolderApp );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// ФИО
		m_tblInput.attach( m_lblFio, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFio, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// ФИО (латиницей)
		m_tblInput.attach( m_lblFioLatin, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFioLatin, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Пол
		m_tblInput.attach( m_lblSex, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSex, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Email
		m_tblInput.attach( m_lblEmail, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpEmail, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Телефон
		m_tblInput.attach( m_lblPhone, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPhone, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Адрес
		m_tblInput.attach( m_lblAddr, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpAddr, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата рождения
		m_tblInput.attach( m_lblBorn, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpBorn, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Место рождения
		m_tblInput.attach( m_lblBornPlace, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpBornPlace, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Место рождения латиницей
		m_tblInput.attach( m_lblBornPlaceLatin, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpBornPlaceLatin, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Документ по умолчанию
		m_tblInput.attach( m_lblDefaultDoc, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDefaultDoc, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Документы
		m_tblInput.attach( m_lblDoc, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL, 10 );
		m_tblInput.attach( m_inpDoc, 1, 2, l++, r++, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );

		// Приложения
		m_tblInput.attach( m_lblApp, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL, 10 );
		m_tblInput.attach( m_inpApp, 1, 2, l++, r++, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHClientForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpFio.Reset( );
		m_inpFioLatin.Reset( );
		m_inpEmail.Reset( );
		m_inpPhone.Reset( );
		m_inpAddr.Reset( );
		m_inpSex.set_active( 0 );
		m_inpBornPlace.Reset( );
		m_inpBornPlaceLatin.Reset( );
		m_inpBorn.Reset( );
		m_inpDefaultDoc.SetTitle( "" );
		m_objDocModel->clear( );
		m_objAppModel->clear( );
	} // void Clear

	void CHClientForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		string born = obj->get( "born" )->toString( );
		if ( !born.empty( ) ) {
			m_inpBorn.SetValue( born );
		}

		m_inpFio.SetValue( Glib::locale_to_utf8( obj->get( "fio" )->toString( ) ) );
		m_inpFioLatin.SetValue( Glib::locale_to_utf8( obj->get( "fiolatin" )->toString( ) ) );
		m_inpEmail.SetValue( Glib::locale_to_utf8( obj->get( "lemail" )->toString( ) ) );
		m_inpPhone.SetValue( Glib::locale_to_utf8( obj->get( "lphone" )->toString( ) ) );
		m_inpAddr.SetValue( Glib::locale_to_utf8( obj->get( "laddr" )->toString( ) ) );
		if ( obj->get( "sex" )->toInt( ) > 0 ) {
			m_inpSex.set_active( obj->get( "sex" )->toInt( ) - 1 );
		}
		m_inpBornPlace.SetValue( Glib::locale_to_utf8( obj->get( "bornplace" )->toString( ) ) );
		m_inpBornPlaceLatin.SetValue( Glib::locale_to_utf8( obj->get( "bornplacelatin" )->toString( ) ) );
		m_inpDefaultDoc.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDefaultDoc ) )->GetObject( )->get( "type_" )->toString( )
		) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidDoc ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objDocModel->append( ) );
				row[ m_pModelFormListDoc->m_col_type ] = Glib::locale_to_utf8( add->get( "type_" )->toString( ) );
				row[ m_pModelFormListDoc->m_col_series ] = Glib::locale_to_utf8( add->get( "series" )->toString( ) );
				row[ m_pModelFormListDoc->m_col_number ] = Glib::locale_to_utf8( add->get( "number" )->toString( ) );
				row[ m_pModelFormListDoc->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objAppModel->append( ) );
				row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
				row[ m_pModelFormListApp->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHClientForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szFio,
						szFioLatin,
						szEmail,
						szPhone,
						szAddr,
						szBornPlace,
						szBornPlaceLatin;

		szFio = m_inpFio.GetValue( );
		if ( szFio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFio ).GetEntry( ) );
			return 1;
		}

		szFioLatin = m_inpFioLatin.GetValue( );
		if ( szFioLatin.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО латиницей" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFioLatin ).GetEntry( ) );
			return 2;
		}

		/*szEmail = m_inpEmail.LoadValue( ).GetValue( );
		if ( szEmail.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите E-mail" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEmail ).GetEntry( ) );
			return 3;
		}*/

		szPhone = m_inpPhone.GetValue( );
		if ( szPhone.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите телефон" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpPhone ).GetEntry( ) );
			return 4;
		}

		szAddr = m_inpAddr.GetValue( );
		if ( szAddr.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите адрес" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpAddr ).GetEntry( ) );
			return 5;
		}

		/*szBornPlace = m_inpBornPlace.LoadValue( ).GetValue( );
		if ( szAddr.empty( ) ) {
			return 6;
		}*/

		/*szBornPlaceLatin = m_inpBornPlaceLatin.LoadValue( ).GetValue( );
		if ( szBornPlaceLatin.empty( ) ) {
			return 7;
		}*/

		return 0;
	} // udInt Validate

	void CHClientForm::Send( ) {
		Glib::ustring	szFio,
						szFioLatin,
						szEmail,
						szPhone,
						szAddr,
						szBornPlace,
						szBornPlaceLatin;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		szFio = m_inpFio.GetValue( );
		szFioLatin = m_inpFioLatin.GetValue( );
		szEmail = m_inpEmail.GetValue( );
		szPhone = m_inpPhone.GetValue( );
		szAddr = m_inpAddr.GetValue( );
		szBornPlace = m_inpBornPlace.GetValue( );
		szBornPlaceLatin = m_inpBornPlaceLatin.GetValue( );
		udUInt sex = m_inpSex.LoadValue( ).GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		obj->get( "fiolatin" )->ini( ConvertString( szFioLatin.data( ), CP_UTF8 ) );
		obj->get( "born" )->ini( m_inpBorn.GetValue( ).toWString( "%Y-%m-%d" ) );
		obj->get( "lemail" )->ini( ConvertString( szEmail.data( ), CP_UTF8 ) );
		obj->get( "lphone" )->ini( ConvertString( szPhone.data( ), CP_UTF8 ) );
		obj->get( "laddr" )->ini( ConvertString( szAddr.data( ), CP_UTF8 ) );
		obj->get( "bornplace" )->ini( ConvertString( szBornPlace.data( ), CP_UTF8 ) );
		obj->get( "bornplacelatin" )->ini( ConvertString( szBornPlaceLatin.data( ), CP_UTF8 ) );
		obj->get( "sex" )->ini( sex );
		obj->get( "doc" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDefaultDoc ) )->GetObject( )->get( "id" )->toUInt( )
		);

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pData->Send( ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHClientForm::OnFormListDoc_Select( ) {
		Gtk::TreeModel::iterator i = m_inpDoc.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnDocEdit->set_sensitive( true );
			m_btnDocDel->set_sensitive( true );
		} else {
			m_btnDocEdit->set_sensitive( false );
			m_btnDocDel->set_sensitive( false );
		}
	} // void OnFormListDoc_Select

	void CHClientForm::OnFormListDoc_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHClDocFormModal handler = new udHClDocFormModal( m_pData->GetData( )->get( "id" )->toUInt( ), false, false );
		handler->Init( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Glib::ustring	szFio,
						szFioLatin,
						szBornPlace;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		szFio = m_inpFio.GetValue( );
		szFioLatin = m_inpFioLatin.GetValue( );
		szBornPlace = m_inpBornPlace.GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase pFakeDoc = new udProjectClDoc;
		pFakeDoc->get( "birthplace" )->ini( ConvertString( szBornPlace.data( ), CP_UTF8 ) );
		pFakeDoc->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		pFakeDoc->get( "fiolatin" )->ini( ConvertString( szFioLatin.data( ), CP_UTF8 ) );
		handler->LoadFrom( pFakeDoc );
		udDELETE( pFakeDoc );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание документа" ) );

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectClDoc;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidDoc );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			arrObject->push_back( add );

			Gtk::TreeModel::iterator i = m_objDocModel->append( );
			Gtk::TreeModel::Row row = *i;
			Gtk::TreePath objPath = m_objDocModel->get_path( i );

			udPFDEObject pFdeDocType = ( udPFDEObject ) pDa->GetExtraData( udHClDocForm::ctrlidType );
			if ( pFdeDocType ) {
				udPBase pDocType = pFdeDocType->GetObject( );
				if ( pDocType ) {
					row[ m_pModelFormListDoc->m_col_type ] = Glib::locale_to_utf8( pDocType->get( "name" )->toString( ) );

					add->get( "type_" )->ini( pDocType->get( "name" )->toWString( ) );
				}
			}

			row[ m_pModelFormListDoc->m_col_series ] = Glib::locale_to_utf8( add->get( "series" )->toString( ) );
			row[ m_pModelFormListDoc->m_col_number ] = Glib::locale_to_utf8( add->get( "number" )->toString( ) );
			row[ m_pModelFormListDoc->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListDoc_Add

	void CHClientForm::OnFormListDoc_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpDoc.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListDoc->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHClDocFormModal handler = new udHClDocFormModal( m_pData->GetData( )->get( "id" )->toUInt( ), false, false );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование документа" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование документа" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				udPFDEObject pFdeDocType = ( udPFDEObject ) pDa->GetExtraData( udHClDocForm::ctrlidType );
				if ( pFdeDocType ) {
					udPBase pDocType = pFdeDocType->GetObject( );
					if ( pDocType ) {
						row[ m_pModelFormListDoc->m_col_type ] = Glib::locale_to_utf8( pDocType->get( "name" )->toString( ) );
					}
				}

				row[ m_pModelFormListDoc->m_col_series ] = Glib::locale_to_utf8( edit->get( "series" )->toString( ) );
				row[ m_pModelFormListDoc->m_col_number ] = Glib::locale_to_utf8( edit->get( "number" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListDoc_Edit

	void CHClientForm::OnFormListDoc_Del( ) {
		Gtk::TreeModel::iterator i = m_inpDoc.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListDoc->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidDoc ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objDocModel->erase( i );
		}
	} // void OnFormListDoc_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHClientForm::OnFormListApp_Select( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnAppEdit->set_sensitive( true );
			m_btnAppDel->set_sensitive( true );
			m_btnAppReplace->set_sensitive( true );
		} else {
			m_btnAppEdit->set_sensitive( false );
			m_btnAppDel->set_sensitive( false );
			m_btnAppReplace->set_sensitive( false );
		}
	} // void OnFormListApp_Select

	void CHClientForm::OnFormListApp_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Gtk::FileChooserDialog dlgPickFile( *wndParent, Glib::locale_to_utf8( "Выберите файл" ) );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

		int code = dlgPickFile.run( );

		if ( code == Gtk::RESPONSE_OK ) {
			Glib::RefPtr< Gio::File > file = dlgPickFile.get_file( );
			wstring szFileName = ConvertString( file->get_basename( ), CP_UTF8 );
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
			udPBase pCurApp = NULL;
			bool bFound = false;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				pCurApp = *i;
				if ( pCurApp->get( "name" )->toWString( ) == szFileName ) {
					bFound = true;
					break;
				}
			}

			if ( bFound ) {
				Gtk::MessageDialog dialog(
					*wndParent,
					Glib::locale_to_utf8( "Файл уже присутствует в списке" ),
					false,
					Gtk::MESSAGE_WARNING,
					Gtk::BUTTONS_OK
				);
				dialog.set_secondary_text(
					Glib::locale_to_utf8( "Файл '" ) +
					file->get_basename( ) +
					Glib::locale_to_utf8( "'\nСмените имя или выберите другой." )
				);
				dialog.run( );
			} else {
				wstring szFilePath;
				if ( file->has_parent( ) ) {
					szFilePath = ConvertString( file->get_parent( )->get_path( ), CP_UTF8 );
				}
				udPBase add = new udProjectAttach;
				add->get( "obj" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
				add->get( "type" )->ini( udProjectAttach::ATTACH_CLIENT );
				add->get( "name" )->ini( szFileName );
				add->get( "orig" )->ini( szFilePath );

				wstring wFileTmpPath = m_szFolderApp + L"\\" + szFileName;
				Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

				if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
					add->get( "tmp_path" )->ini( wFileTmpPath );
					arrObject->push_back( add );

					Gtk::TreeModel::Row row = *( m_objAppModel->append( ) );

					row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
					row[ m_pModelFormListApp->m_col_object ] = add;
				} else {
					udDELETE( add );
					Gtk::MessageDialog dialog(
						*wndParent,
						Glib::locale_to_utf8( "Не удалось скопировать файл" ),
						false,
						Gtk::MESSAGE_ERROR,
						Gtk::BUTTONS_OK
					);
					dialog.set_secondary_text(
						Glib::locale_to_utf8( "Файл '" ) +
						file->get_basename( ) +
						Glib::locale_to_utf8( "'\nПовторите попытку или выберите другой." )
					);
					dialog.run( );
				}
			}
		}
	} // void OnFormListApp_Add

	void CHClientForm::OnFormListApp_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListApp->m_col_object ];
			ExtOpenFile( edit->get( "tmp_path" )->toWString( ), false );
		}
	} // void OnFormListApp_Edit

	void CHClientForm::OnFormListApp_Del( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListApp->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objAppModel->erase( i );
		}
	} // void OnFormListApp_Del

	void CHClientForm::OnFormListApp_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListApp->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			Gtk::FileChooserDialog dlgPickFile( *wndParent, Glib::locale_to_utf8( "Выберите файл" ) );
			dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
			dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

			int code = dlgPickFile.run( );

			if ( code == Gtk::RESPONSE_OK ) {
				Glib::RefPtr< Gio::File > file = dlgPickFile.get_file( );
				wstring szFileName = ConvertString( file->get_basename( ), CP_UTF8 );
				udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
				udPBase pCurApp = NULL;
				bool bFound = false;

				for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
					pCurApp = *k;
					if ( pCurApp->get( "name" )->toWString( ) == szFileName ) {
						bFound = true;
						break;
					}
				}

				if ( bFound ) {
					Gtk::MessageDialog dialog(
						*wndParent,
						Glib::locale_to_utf8( "Файл уже присутствует в списке" ),
						false,
						Gtk::MESSAGE_WARNING,
						Gtk::BUTTONS_OK
					);
					dialog.set_secondary_text(
						Glib::locale_to_utf8( "Файл '" ) +
						file->get_basename( ) +
						Glib::locale_to_utf8( "'\nСмените имя или выберите другой." )
					);
					dialog.run( );
				} else {
					wstring szFilePath;
					if ( file->has_parent( ) ) {
						szFilePath = ConvertString( file->get_parent( )->get_path( ), CP_UTF8 );
					}

					wstring wFileTmpPath = m_szFolderApp + L"\\" + szFileName;
					Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

					if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
						replace->get( "tmp_path" )->ini( wFileTmpPath );
						replace->get( "name" )->ini( szFileName );
						replace->get( "orig" )->ini( szFilePath );

						Gtk::TreeModel::Row row = *i;

						row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( replace->get( "name" )->toString( ) );
					} else {
						Gtk::MessageDialog dialog(
							*wndParent,
							Glib::locale_to_utf8( "Не удалось скопировать файл" ),
							false,
							Gtk::MESSAGE_ERROR,
							Gtk::BUTTONS_OK
						);
						dialog.set_secondary_text(
							Glib::locale_to_utf8( "Файл '" ) +
							file->get_basename( ) +
							Glib::locale_to_utf8( "'\nПовторите попытку или выберите другой." )
						);
						dialog.run( );
					}
				}
			}
		}
	} // void OnFormListApp_Del

	void CHClientForm::OnFormListApp_Folder( ) {
		ExtOpenFolder( m_szFolderApp );
	} // void OnFormListApp_Folder

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientFormModal::CHClientFormModal( ) : CHClientForm( ) {
	} // CHClientFormModal

	CHClientFormModal::CHClientFormModal( const CHClientFormModal& objCopy ) : CHClientForm( ) {
	} // CHClientFormModal

	CHClientFormModal::~CHClientFormModal( ) {
	} // ~CHClientFormModal

	CHClientFormModal& CHClientFormModal::operator =( const CHClientFormModal& objCopy ) {
		return *this;
	} // CHClientFormModal& operator =

	void CHClientFormModal::OnButtonOk( ) {
		g_pApp->GetLog( )->Write( "CHClientFormModal::OnButtonOk, enter\n" );

		if ( Validate( ) != 0 ) {
			g_pApp->GetLog( )->Write( "CHClientFormModal::OnButtonOk, return\n" );
			return;
		}

		g_pApp->GetLog( )->Write( "CHClientFormModal::OnButtonOk, Send\n" );
		Send( );
		g_pApp->GetLog( )->Write( "CHClientFormModal::OnButtonOk, Send\n" );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );

		g_pApp->GetLog( )->Write( "CHClientFormModal::OnButtonOk, exit\n" );
	} // void OnButtonOk

	void CHClientFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHClientForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHClientFormModeless::CHClientFormModeless( ) : CHClientForm( ) {
	} // CHClientFormModeless

	CHClientFormModeless::CHClientFormModeless( const CHClientFormModeless& objCopy ) : CHClientForm( ) {
	} // CHClientFormModeless

	CHClientFormModeless::~CHClientFormModeless( ) {
	} // ~CHClientFormModeless

	CHClientFormModeless& CHClientFormModeless::operator =( const CHClientFormModeless& objCopy ) {
		return *this;
	} // CHClientFormModeless& operator =

	void CHClientFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "client" );
	} // void OnButtonOk

	void CHClientFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "client" );
	} // void CHClientForm

	void CHClientFormModeless::Init( ) {
		CHClientForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHClientFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHClientFormModeless

} // namespace UdSDK
