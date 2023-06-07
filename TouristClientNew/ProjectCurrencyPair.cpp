
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHCurrencyPairListPick( ) {
		return new CHCurrencyPairPick;
	} // udPListHandlerPick GetHCurrencyPairListPick

	udPBase GetProjectCurrencyPairList( ) {
		udPBase ret = GetProjectCurrencyPair( );
		ret->get( "pair" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectCurrencyPairList

	CHCurrencyPair::CHCurrencyPair( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltDate1( Glib::locale_to_utf8( "Установлен с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltCourse1( Glib::locale_to_utf8( "Курс с" ) ),
		m_lblFltCourse2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltCb1( Glib::locale_to_utf8( "ЦИ курс с" ) ),
		m_lblFltCb2( Glib::locale_to_utf8( "по" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		m_szName = "currencypair";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать валютную пару" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "course1" )->ini( ( udFloat ) 0.0f );
		filter->get( "course2" )->ini( ( udFloat ) 0.0f );
		filter->get( "cb1" )->ini( ( udFloat ) 0.0f );
		filter->get( "cb2" )->ini( ( udFloat ) 0.0f );

		m_pRowModel = m_pColModel = new CHCurrencyPair::ColModelCurrencyPair( );

		m_pData->SetName( "currencypair" );
		m_pData->SetGetObject( GetProjectCurrencyPairList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Дата с
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата по
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Курс с
		m_tblFilter.attach( m_lblFltCourse1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCourse1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Курс по
		m_tblFilter.attach( m_lblFltCourse2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCourse2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Курс ЦБ с
		m_tblFilter.attach( m_lblFltCb1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCb1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Курс ЦБ по
		m_tblFilter.attach( m_lblFltCb2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCb2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHCurrencyPair

	CHCurrencyPair::CHCurrencyPair( const CHCurrencyPair& objCopy ) : udListHandler( ) {
		m_szName = "currencypair";
	} // CHCurrencyPair

	CHCurrencyPair::~CHCurrencyPair( ) {
	} // ~CCurrencyPair

	CHCurrencyPair& CHCurrencyPair::operator =( const CHCurrencyPair& objCopy ) {
		return *this;
	} // CHCurrencyPair& operator =

	void CHCurrencyPair::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Валюта 1/Валюта 2" ), m_pColModel->m_col_pair );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Курс ЦБ" ), m_pColModel->m_col_course );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата установки" ), m_pColModel->m_col_set );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Внутренний курс" ), m_pColModel->m_col_internal );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHCurrencyPair::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_pair ] = Glib::locale_to_utf8( obj->get( "pair" )->toString( ) );
			row[ m_pColModel->m_col_course ] = Glib::locale_to_utf8( obj->get( "course" )->toString( ) );
			row[ m_pColModel->m_col_set ] = Glib::locale_to_utf8( obj->get( "set" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_internal ] = Glib::locale_to_utf8( obj->get( "internal" )->toString( ) );
		}
	} // void Load

	void CHCurrencyPair::FilterUiClear( ) {
		m_inpFltCourse1.Reset( );
		m_inpFltCourse2.Reset( );
		m_inpFltCb1.Reset( );
		m_inpFltCb2.Reset( );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHCurrencyPair::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udFloat		fCourse1,
					fCourse2,
					fCb1,
					fCb2;

		fCourse1 = toFloat( m_inpFltCourse1.GetValue( ).data( ) );
		fCourse2 = toFloat( m_inpFltCourse2.GetValue( ).data( ) );
		fCb1 = toFloat( m_inpFltCb1.GetValue( ).data( ) );
		fCb2 = toFloat( m_inpFltCb2.GetValue( ).data( ) );
		
		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
		filter->get( "course1" )->ini( fCourse1 );
		filter->get( "course2" )->ini( fCourse2 );
		filter->get( "cb1" )->ini( fCb1 );
		filter->get( "cb2" )->ini( fCb2 );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyPairModeless::CHCurrencyPairModeless( ) :
		udListHandlerModeless( ),
		CHCurrencyPair( )
	{
	} // CHCurrencyPairModeless

	CHCurrencyPairModeless::CHCurrencyPairModeless( const CHCurrencyPairModeless& objCopy ) : CHCurrencyPair( ) {
	} // CHCurrencyPairModeless

	CHCurrencyPairModeless::~CHCurrencyPairModeless( ) {
	} // ~CHCurrencyPairModeless

	CHCurrencyPairModeless& CHCurrencyPairModeless::operator =( const CHCurrencyPairModeless& objCopy ) {
		return *this;
	} // CHCurrencyPairModeless& operator =

	void CHCurrencyPairModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHCurrencyPair::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHCurrencyPairModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "currencypair_form" );
	} // void OnButtonForm

	void CHCurrencyPairModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "currencypair_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "currencypair_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyPairPick::CHCurrencyPairPick( ) : CHCurrencyPair( ) {
	} // CHCurrencyPairPick

	CHCurrencyPairPick::CHCurrencyPairPick( const CHCurrencyPairPick& objCopy ) : CHCurrencyPair( ) {
	} // CHCurrencyPairPick

	CHCurrencyPairPick::~CHCurrencyPairPick( ) {
	} // ~CHCurrencyPairPick

	CHCurrencyPairPick& CHCurrencyPairPick::operator =( const CHCurrencyPairPick& objCopy ) {
		return *this;
	} // CHCurrencyPairPick& operator =

	void CHCurrencyPairPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHCurrencyPair::Init( );
	} // void CHCurrencyPairPick

	void CHCurrencyPairPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHCurrencyPairFormModal handler = new udHCurrencyPairFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание валютной пары" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание валютной пары" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHCurrencyPairPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectCurrencyPair( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			mb::err(
				*wndParent,
				Glib::locale_to_utf8( "Выберите валютную пару" ),
				Glib::locale_to_utf8( "Щелкните по строке списка и нажмите кнопку \"ОК\"" )
			);
		}
	} // void OnButtonOk

	void CHCurrencyPairPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHCurrencyPairPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHCurrencyPairFormModal handler = new udHCurrencyPairFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование валютной пары" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование валютной пары" ) );
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

	udPFormHandler GetHCurrencyPairForm( ) {
		return new CHCurrencyPairFormModal;
	} // udPFormHandler GetHCurrencyPairForm

	CHCurrencyPairForm::CHCurrencyPairForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Валютная пара" ) ),
		m_lblCurrency1( Glib::locale_to_utf8( "Первая валюта" ) ),
		m_lblCurrency2( Glib::locale_to_utf8( "Вторая валюта" ) ),
		m_lblCourse( Glib::locale_to_utf8( "Курс ЦБ" ) ),
		m_lblInternal( Glib::locale_to_utf8( "Внутренний курс" ) ),
		m_lblSet( Glib::locale_to_utf8( "Дата установки" ) ),
		m_inpSet( datetime( 0, true ), datetime( 0, true ) )
	{
		m_szName = "currencypair_form";

		m_pData->SetName( "currencypair" );
		m_pData->SetGetObject( GetProjectCurrencyPair );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblCurrency1.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblCurrency1.set_size_request( 250 );

		udPFDEObject pFde = NULL;

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrency1 );
		m_pData->AddExtraData( pFde );
		m_inpCurrency1.SetDataObject( pFde );
		m_inpCurrency1.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrency1.SetAttr( "name" );
		m_inpCurrency1.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrency1.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrency1.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCurrency );
		pFde->SetMark( ctrlidCurrency2 );
		m_pData->AddExtraData( pFde );
		m_inpCurrency2.SetDataObject( pFde );
		m_inpCurrency2.SetWindowTitle( Glib::locale_to_utf8( "Выбор валюты" ) );
		m_inpCurrency2.SetAttr( "name" );
		m_inpCurrency2.SetGetListHandler( GetHCurrencyListPick );
		m_inpCurrency2.SetGetFormHandler( GetHCurrencyForm );
		m_inpCurrency2.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование валюты" ) );
	} // CHCurrencyPairForm

	CHCurrencyPairForm::CHCurrencyPairForm( const CHCurrencyPairForm& objCopy ) : udFormHandler( ) {
	} // CHCurrencyPairForm

	CHCurrencyPairForm::~CHCurrencyPairForm( ) {
	} // ~CHCurrencyPairForm

	CHCurrencyPairForm& CHCurrencyPairForm::operator =( const CHCurrencyPairForm& objCopy ) {
		return *this;
	} // CHCurrencyPairForm& operator =

	void CHCurrencyPairForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Первая валюта
		m_tblInput.attach( m_lblCurrency1,	0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency1,	1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// Вторая валюта
		m_tblInput.attach( m_lblCurrency2,	0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency2,	1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK );

		// Курс ЦБ
		m_tblInput.attach( m_lblCourse, 0, 1, 2, 3, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCourse, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		// Внутренний курс
		m_tblInput.attach( m_lblInternal, 0, 1, 3, 4, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpInternal, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK );

		// Дата установки
		m_tblInput.attach( m_lblSet, 0, 1, 4, 5, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSet, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 5, 6, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		udFormHandler::Init( );
	} // void Init

	void CHCurrencyPairForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpCourse.Reset( );
		m_inpInternal.Reset( );
		m_inpCurrency1.SetTitle( "" );
		m_inpCurrency2.SetTitle( "" );
		m_inpSet.Reset( );
	} // void Clear

	void CHCurrencyPairForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpCourse.SetValue( Glib::locale_to_utf8( obj->get( "course" )->toString( ) ) );
		m_inpInternal.SetValue( Glib::locale_to_utf8( obj->get( "internal" )->toString( ) ) );
		m_inpSet.SetValue( obj->get( "set" )->toTime( ) );
		m_inpCurrency1.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency1 ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpCurrency2.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency2 ) )->GetObject( )->get( "name" )->toString( )
		) );
	} // void Load

	udInt CHCurrencyPairForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szCourse,
						szInternal;
		
		szCourse = m_inpCourse.GetValue( );
		if ( szCourse.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите курс ЦБ" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCourse ).GetEntry( ) );
			return 1;
		}

		szInternal = m_inpInternal.GetValue( );
		if ( szInternal.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите внутренний курс" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpInternal ).GetEntry( ) );
			return 2;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency1 ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите первичную валюту" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCurrency1.GetButton( ) ) );
			return 3;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency2 ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите вторичную валюту" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCurrency2.GetButton( ) ) );
			return 4;
		}

		return 0;
	} // udInt Validate

	void CHCurrencyPairForm::Send( ) {
		Glib::ustring	szCourse,
						szInternal;
		
		szCourse = m_inpCourse.GetValue( );
		szInternal = m_inpInternal.GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		
		obj->get( "currency1" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency1 ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "currency2" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCurrency2 ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "course" )->ini( toFloat( szCourse.data( ) ) );
		obj->get( "internal" )->ini( toFloat( szInternal.data( ) ) );
		obj->get( "set" )->ini( m_inpSet.GetValue( ) );
		
		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyPairFormModal::CHCurrencyPairFormModal( ) : CHCurrencyPairForm( ) {
	} // CHCurrencyPairFormModal

	CHCurrencyPairFormModal::CHCurrencyPairFormModal( const CHCurrencyPairFormModal& objCopy ) : CHCurrencyPairForm( ) {
	} // CHCurrencyPairFormModal

	CHCurrencyPairFormModal::~CHCurrencyPairFormModal( ) {
	} // ~CHCurrencyPairFormModal

	CHCurrencyPairFormModal& CHCurrencyPairFormModal::operator =( const CHCurrencyPairFormModal& objCopy ) {
		return *this;
	} // CHCurrencyPairFormModal& operator =

	void CHCurrencyPairFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHCurrencyPairFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHCurrencyPairForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHCurrencyPairFormModeless::CHCurrencyPairFormModeless( ) : CHCurrencyPairForm( ) {
	} // CHCurrencyPairFormModeless

	CHCurrencyPairFormModeless::CHCurrencyPairFormModeless( const CHCurrencyPairFormModeless& objCopy ) : CHCurrencyPairForm( ) {
	} // CHCurrencyPairFormModeless

	CHCurrencyPairFormModeless::~CHCurrencyPairFormModeless( ) {
	} // ~CHCurrencyPairFormModeless

	CHCurrencyPairFormModeless& CHCurrencyPairFormModeless::operator =( const CHCurrencyPairFormModeless& objCopy ) {
		return *this;
	} // CHCurrencyPairFormModeless& operator =

	void CHCurrencyPairFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "currencypair" );
	} // void OnButtonOk

	void CHCurrencyPairFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "currencypair" );
	} // void CHCurrencyPairForm

	void CHCurrencyPairFormModeless::Init( ) {
		CHCurrencyPairForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHCurrencyPairFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHCurrencyPairFormModeless

} // namespace UdSDK
