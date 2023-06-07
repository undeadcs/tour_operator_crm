
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHDocTemplatePrintedListPick( ) {
		return new CHDocTemplatePrintedPick;
	} // udPListHandlerPick GetHDocTemplatePrintedListPick

	udPBase GetProjectDocTemplatePrintedList( ) {
		udPBase ret = GetProjectDocTemplatePrinted( );
		ret->get( "createdby_" )->ini( wstring( L"" ) );
		ret->get( "tour_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectDocTemplatePrintedList

	CHDocTemplatePrinted::CHDocTemplatePrinted( ) :
		udListHandler( ),
		m_lblFltSeries( Glib::locale_to_utf8( "Серия" ) ),
		m_lblFltNumber( Glib::locale_to_utf8( "Номер" ) ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltCreatedBy( Glib::locale_to_utf8( "Кем создан" ) ),
		m_lblFltType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblFltTour( Glib::locale_to_utf8( "Тур" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) ),
		m_inpFltType( udProjectDocTemplate::tplClassic, udProjectDocTemplate::tplClassic )
	{
		m_szName = "doctplprinted";

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "series" )->ini( wstring( L"" ) );
		filter->get( "number" )->ini( wstring( L"" ) );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "createdby" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHDocTemplatePrinted::ColModelDocTemplatePrinted( );

		m_pData->SetName( "doctplprinted" );
		m_pData->SetGetObject( GetProjectDocTemplatePrintedList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpFltType.AddRow( udProjectDocTemplate::tplClassic, Glib::locale_to_utf8( "-- нет --" ) );
		m_inpFltType.AddRow( udProjectDocTemplate::tplContract, Glib::locale_to_utf8( "Договор" ) );
		m_inpFltType.AddRow( udProjectDocTemplate::tplPermit, Glib::locale_to_utf8( "Путевка" ) );
		m_inpFltType.Reset( );
		/*m_pColModelFltType = new CHDocTemplatePrinted::ColModelFltType;
		m_objFltTypeModel = Gtk::ListStore::create( *m_pColModelFltType );

		Gtk::TreeModel::Row row;

		row = *( m_objFltTypeModel->append( ) );
		row[ m_pColModelFltType->m_col_code ] = udProjectDocTemplate::tplClassic;
		row[ m_pColModelFltType->m_col_name ] = Glib::locale_to_utf8( "-- нет --" );

		row = *( m_objFltTypeModel->append( ) );
		row[ m_pColModelFltType->m_col_code ] = udProjectDocTemplate::tplContract;
		row[ m_pColModelFltType->m_col_name ] = Glib::locale_to_utf8( "Договор" );

		row = *( m_objFltTypeModel->append( ) );
		row[ m_pColModelFltType->m_col_code ] = udProjectDocTemplate::tplPermit;
		row[ m_pColModelFltType->m_col_name ] = Glib::locale_to_utf8( "Путевка" );

		row = *( m_objFltTypeModel->append( ) );
		row[ m_pColModelFltType->m_col_code ] = udProjectDocTemplate::tplContract2;
		row[ m_pColModelFltType->m_col_name ] = Glib::locale_to_utf8( "Групповой договор" );

		m_inpFltType.set_model( m_objFltTypeModel );

		m_inpFltType.pack_start( m_pColModelFltType->m_col_name );
		m_inpFltType.set_active( 0 );*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( fltCreatedBy );
		m_pData->AddExtraData( pFde );
		m_inpFltCreatedBy.SetDataObject( pFde );
		m_inpFltCreatedBy.SetWindowTitle( Glib::locale_to_utf8( "Выбор пользователя" ) );
		m_inpFltCreatedBy.SetAttr( "fio" );
		m_inpFltCreatedBy.SetGetListHandler( GetHUserListPick );
		m_inpFltCreatedBy.SetGetFormHandler( GetHUserForm );
		m_inpFltCreatedBy.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пользователя" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTour );
		pFde->SetMark( fltTour );
		m_pData->AddExtraData( pFde );
		m_inpFltTour.SetDataObject( pFde );
		m_inpFltTour.SetWindowTitle( Glib::locale_to_utf8( "Выбор тура" ) );
		m_inpFltTour.SetAttr( "name" );
		m_inpFltTour.SetGetListHandler( GetHTourListPick );
		m_inpFltTour.SetGetFormHandler( GetHTourForm );
		m_inpFltTour.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование тура" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Серия
		m_tblFilter.attach( m_lblFltSeries, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltSeries, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Номер
		m_tblFilter.attach( m_lblFltNumber, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltNumber, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Дата с
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата по
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Кем создана
		m_tblFilter.attach( m_lblFltCreatedBy, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCreatedBy, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		
		// Тур
		m_tblFilter.attach( m_lblFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Тип
		m_tblFilter.attach( m_lblFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHDocTemplatePrinted

	CHDocTemplatePrinted::CHDocTemplatePrinted( const CHDocTemplatePrinted& objCopy ) : udListHandler( ) {
		m_szName = "country";
	} // CHDocTemplatePrinted

	CHDocTemplatePrinted::~CHDocTemplatePrinted( ) {
	} // ~CDocTemplatePrinted

	CHDocTemplatePrinted& CHDocTemplatePrinted::operator =( const CHDocTemplatePrinted& objCopy ) {
		return *this;
	} // CHDocTemplatePrinted& operator =

	void CHDocTemplatePrinted::Init( ) {
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата создания" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Кем создан" ), m_pColModel->m_col_createdby );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тур" ), m_pColModel->m_col_tour );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Серия" ), m_pColModel->m_col_series );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Номер" ), m_pColModel->m_col_number );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тип" ), m_pColModel->m_col_type );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHDocTemplatePrinted::Load( ) {
		udListHandler::Load( );

		map< udInt, string > mapType;
		mapType[ udProjectDocTemplate::tplClassic	] = "";
		mapType[ udProjectDocTemplate::tplContract	] = "Договор";
		mapType[ udProjectDocTemplate::tplPermit	] = "Путевка";
		mapType[ udProjectDocTemplate::tplContract2	] = "Групповой договор";
		map< udInt, string >::iterator itType;

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_created ] = Glib::locale_to_utf8( obj->get( "createdby" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_createdby ] = Glib::locale_to_utf8( obj->get( "createdby_" )->toString( ) );
			row[ m_pColModel->m_col_tour ] = Glib::locale_to_utf8( obj->get( "tour_" )->toString( ) );
			row[ m_pColModel->m_col_series ] = Glib::locale_to_utf8( obj->get( "series" )->toString( ) );
			row[ m_pColModel->m_col_number ] = Glib::locale_to_utf8( obj->get( "number" )->toString( ) );

			itType = mapType.find( obj->get( "type" )->toInt( ) );
			if ( itType != mapType.end( ) ) {
				row[ m_pColModel->m_col_type ] = Glib::locale_to_utf8( itType->second );
			}
		}
	} // void Load

	void CHDocTemplatePrinted::FilterUiClear( ) {
		m_inpFltSeries.Reset( );
		m_inpFltNumber.Reset( );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
		//m_inpFltType.set_active( 0 );
		m_inpFltType.Reset( );
		m_inpFltCreatedBy.SetTitle( "" );
		m_inpFltTour.SetTitle( "" );
	} // void FilterUiClear

	void CHDocTemplatePrinted::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szSeries,
						szNumber;

		szSeries = m_inpFltSeries.GetValue( );
		szNumber = m_inpFltNumber.GetValue( );

		udInt type = m_inpFltType.LoadValue( ).GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "series" )->ini( ConvertString( szSeries.data( ), CP_UTF8 ) );
		filter->get( "number" )->ini( ConvertString( szNumber.data( ), CP_UTF8 ) );
		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
		filter->get( "type" )->ini( type );
		filter->get( "createdby" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltCreatedBy ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "tour" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltTour ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTemplatePrintedModeless::CHDocTemplatePrintedModeless( ) :
		udListHandlerModeless( ),
		CHDocTemplatePrinted( )
	{
	} // CHDocTemplatePrintedModeless

	CHDocTemplatePrintedModeless::CHDocTemplatePrintedModeless( const CHDocTemplatePrintedModeless& objCopy ) : CHDocTemplatePrinted( ) {
	} // CHDocTemplatePrintedModeless

	CHDocTemplatePrintedModeless::~CHDocTemplatePrintedModeless( ) {
	} // ~CHDocTemplatePrintedModeless

	CHDocTemplatePrintedModeless& CHDocTemplatePrintedModeless::operator =( const CHDocTemplatePrintedModeless& objCopy ) {
		return *this;
	} // CHDocTemplatePrintedModeless& operator =

	void CHDocTemplatePrintedModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHDocTemplatePrinted::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHDocTemplatePrintedModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "doctplprinted_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "doctplprinted_form" );
		}
	} // void OnRowActivated
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTemplatePrintedPick::CHDocTemplatePrintedPick( ) : CHDocTemplatePrinted( ) {
	} // CHDocTemplatePrintedPick

	CHDocTemplatePrintedPick::CHDocTemplatePrintedPick( const CHDocTemplatePrintedPick& objCopy ) : CHDocTemplatePrinted( ) {
	} // CHDocTemplatePrintedPick

	CHDocTemplatePrintedPick::~CHDocTemplatePrintedPick( ) {
	} // ~CHDocTemplatePrintedPick

	CHDocTemplatePrintedPick& CHDocTemplatePrintedPick::operator =( const CHDocTemplatePrintedPick& objCopy ) {
		return *this;
	} // CHDocTemplatePrintedPick& operator =

	void CHDocTemplatePrintedPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHDocTemplatePrinted::Init( );
	} // void CHDocTemplatePrintedPick

	void CHDocTemplatePrintedPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectDocTemplatePrinted( );
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите распечатанный шаблон документа" ),
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

	void CHDocTemplatePrintedPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHDocTemplatePrintedPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHDocTemplatePrintedFormModal handler = new udHDocTemplatePrintedFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование распечатанного шаблона документа" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование распечатанного шаблона документа" ) );
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

	udPFormHandler GetHDocTemplatePrintedForm( ) {
		return new CHDocTemplatePrintedFormModal;
	} // udPFormHandler GetHDocTemplatePrintedForm

	CHDocTemplatePrintedForm::CHDocTemplatePrintedForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Распечатанный документ" ) ),
		m_lblCreated( Glib::locale_to_utf8( "Дата распечатки" ) ),
		m_lblCreatedBy( Glib::locale_to_utf8( "Кем распечатан" ) ),
		m_lblTour( Glib::locale_to_utf8( "Тур" ) ),
		m_lblSeries( Glib::locale_to_utf8( "Серия" ) ),
		m_lblNumber( Glib::locale_to_utf8( "Номер" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование шаблона документа" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип шаблона документа" ) ),
		m_btnPrintDoc( Glib::locale_to_utf8( "Распечатать" ) ),
		m_btnOpenFolder( Glib::locale_to_utf8( "Открыть папку" ) )
	{
		m_szName = "doctplprinted_form";

		m_pData->SetName( "doctplprinted" );
		m_pData->SetGetObject( GetProjectDocTemplatePrinted );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblCreated.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblCreated.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( ctrlidCreatedBy );
		m_pData->AddExtraData( pFde );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTour );
		pFde->SetMark( ctrlidTour );
		m_pData->AddExtraData( pFde );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_szFolder = g_pApp->GetUniqueTmpFolder( );
		udPFDEFile pFdeFile = new udFDEFile;
		pFdeFile->SetMark( ctrlidXsl );
		pFdeFile->SetFolder( m_szFolder );
		m_pData->AddExtraData( pFdeFile );
		/*udPFDEFile*/ pFdeFile = new udFDEFile;
		pFdeFile->SetMark( ctrlidXml );
		pFdeFile->SetFolder( m_szFolder );
		m_pData->AddExtraData( pFdeFile );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_btnPrintDoc.signal_clicked( ).connect( sigc::mem_fun( this, &CHDocTemplatePrintedForm::OnButtonPrintDoc ) );
		m_btnOpenFolder.signal_clicked( ).connect( sigc::mem_fun( this, &CHDocTemplatePrintedForm::OnButtonOpenFolder ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHDocTemplatePrintedForm

	CHDocTemplatePrintedForm::CHDocTemplatePrintedForm( const CHDocTemplatePrintedForm& objCopy ) : udFormHandler( ) {
	} // CHDocTemplatePrintedForm

	CHDocTemplatePrintedForm::~CHDocTemplatePrintedForm( ) {
	} // ~CHDocTemplatePrintedForm

	CHDocTemplatePrintedForm& CHDocTemplatePrintedForm::operator =( const CHDocTemplatePrintedForm& objCopy ) {
		return *this;
	} // CHDocTemplatePrintedForm& operator =

	void CHDocTemplatePrintedForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Дата распечатки
		m_tblInput.attach( m_lblCreated, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCreated, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Кем распечатан
		m_tblInput.attach( m_lblCreatedBy, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCreatedBy, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тур
		m_tblInput.attach( m_lblTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpTour, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Серия
		m_tblInput.attach( m_lblSeries, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSeries, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Номер
		m_tblInput.attach( m_lblNumber, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNumber, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Наименование шаблона документа
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип шаблона документа
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		//m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnPrintDoc, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnOpenFolder, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHDocTemplatePrintedForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpCreated.set_text( "" );
		m_inpCreatedBy.set_text( "" );
		m_inpTour.set_text( "" );
		m_inpSeries.set_text( "" );
		m_inpNumber.set_text( "" );
		m_inpName.set_text( "" );
		m_inpType.set_text( "" );
	} // void Clear

	void CHDocTemplatePrintedForm::Load( ) {
		udFormHandler::Load( );

		map< udInt, string > mapType;
		mapType[ udProjectDocTemplate::tplClassic	] = "";
		mapType[ udProjectDocTemplate::tplContract	] = "Договор";
		mapType[ udProjectDocTemplate::tplPermit	] = "Путевка";
		mapType[ udProjectDocTemplate::tplContract2	] = "Групповой договор";
		map< udInt, string >::iterator itType;

		udPBase obj = m_pData->GetData( );

		m_inpCreated.set_text( Glib::locale_to_utf8( obj->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) ) );
		m_inpCreatedBy.set_text( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCreatedBy ) )->GetObject( )->get( "fio" )->toString( )
		) );
		m_inpTour.set_text( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidTour ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpSeries.set_text( Glib::locale_to_utf8( obj->get( "series" )->toString( ) ) );
		m_inpNumber.set_text( Glib::locale_to_utf8( obj->get( "number" )->toString( ) ) );
		m_inpName.set_text( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );

		itType = mapType.find( obj->get( "type" )->toInt( ) );
		if ( itType != mapType.end( ) ) {
			m_inpType.set_text( Glib::locale_to_utf8( itType->second ) );
		}
	} // void Load

	udInt CHDocTemplatePrintedForm::Validate( ) {
		return 0;
	} // udInt Validate

	void CHDocTemplatePrintedForm::Send( ) {
	} // void Send

	void CHDocTemplatePrintedForm::OnButtonPrintDoc( ) {
		ExtOpenFile( m_szFolder + L"\\" + ( ( udPFDEFile ) m_pData->GetExtraData( ctrlidXml ) )->GetName( ) );
	} // void OnButtonPrintDoc

	void CHDocTemplatePrintedForm::OnButtonOpenFolder( ) {
		ExtOpenFolder( m_szFolder );
	} // void OnButtonOpenFolder

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHDocTemplatePrintedFormModal::CHDocTemplatePrintedFormModal( ) : CHDocTemplatePrintedForm( ) {
	} // CHDocTemplatePrintedFormModal

	CHDocTemplatePrintedFormModal::CHDocTemplatePrintedFormModal( const CHDocTemplatePrintedFormModal& objCopy ) : CHDocTemplatePrintedForm( ) {
	} // CHDocTemplatePrintedFormModal

	CHDocTemplatePrintedFormModal::~CHDocTemplatePrintedFormModal( ) {
	} // ~CHDocTemplatePrintedFormModal

	CHDocTemplatePrintedFormModal& CHDocTemplatePrintedFormModal::operator =( const CHDocTemplatePrintedFormModal& objCopy ) {
		return *this;
	} // CHDocTemplatePrintedFormModal& operator =

	void CHDocTemplatePrintedFormModal::OnButtonOk( ) {
		/*if ( Validate( ) != 0 ) {
			return;
		}

		Send( );*/
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHDocTemplatePrintedFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHDocTemplatePrintedForm




	CHDocTemplatePrintedFormModeless::CHDocTemplatePrintedFormModeless( ) : CHDocTemplatePrintedForm( ) {
	} // CHDocTemplatePrintedFormModeless

	CHDocTemplatePrintedFormModeless::CHDocTemplatePrintedFormModeless( const CHDocTemplatePrintedFormModeless& objCopy ) : CHDocTemplatePrintedForm( ) {
	} // CHDocTemplatePrintedFormModeless

	CHDocTemplatePrintedFormModeless::~CHDocTemplatePrintedFormModeless( ) {
	} // ~CHDocTemplatePrintedFormModeless

	CHDocTemplatePrintedFormModeless& CHDocTemplatePrintedFormModeless::operator =( const CHDocTemplatePrintedFormModeless& objCopy ) {
		return *this;
	} // CHDocTemplatePrintedFormModeless& operator =

	void CHDocTemplatePrintedFormModeless::OnButtonOk( ) {
		/*if ( Validate( ) != 0 ) {
			return;
		}

		Send( );*/
		g_pApp->GetHandlerManager( )->SetState( "doctplprinted" );
	} // void OnButtonOk

	void CHDocTemplatePrintedFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "doctplprinted" );
	} // void CHDocTemplatePrintedForm

	void CHDocTemplatePrintedFormModeless::Init( ) {
		CHDocTemplatePrintedForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHDocTemplatePrintedFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHDocTemplatePrintedFormModeless

} // namespace UdSDK
