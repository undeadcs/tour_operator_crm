/*
	Tourist application
	Клиентское приложение
	Период услуги
*/
#ifndef TOURIST_CLIENT_SERVICE_PERIOD_H
#define TOURIST_CLIENT_SERVICE_PERIOD_H

namespace UdSDK {

	udPBase GetProjectServicePeriodList( );

	class CWgtServicePeriodPick : public udWgtListPick {
		udPHServiceForm	m_pServiceForm;

	public:
		CWgtServicePeriodPick( );
		CWgtServicePeriodPick( const CWgtServicePeriodPick& objCopy );

		virtual ~CWgtServicePeriodPick( );

		CWgtServicePeriodPick& operator =( const CWgtServicePeriodPick& objCopy );

		virtual void	OnButtonClick( );

		void	SetServiceForm( udPHServiceForm pServiceForm ) { m_pServiceForm = pServiceForm; }

	}; // class CWgtServicePeriodPick

	class CDAListServicePeriod : public udDAList {
		udPHServiceForm	m_pServiceForm;

	public:
		CDAListServicePeriod( );
		CDAListServicePeriod( const CDAListServicePeriod& objCopy );

		virtual ~CDAListServicePeriod( );

		CDAListServicePeriod& operator =( const CDAListServicePeriod& objCopy );

		virtual void	Load( );

		void	SetServiceForm( udPHServiceForm pServiceForm ) { m_pServiceForm = pServiceForm; }

	}; // class CDAListServicePeriod

	class CHServicePeriod : public udListHandlerBtnForm, public udListHandler {
	protected:
		udPHServiceForm m_pServiceForm;
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHServicePeriod( );
		CHServicePeriod( const CHServicePeriod& objCopy );

		virtual ~CHServicePeriod( );

		CHServicePeriod& operator =( const CHServicePeriod& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		void	SetServiceForm( udPHServiceForm pServiceForm );

		class ColModelServicePeriod : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelServicePeriod( ) {
				//add( m_col_id );
				add( m_col_date1 );
				add( m_col_date2 );
			}

			//Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date2;
		};

	protected:
		ColModelServicePeriod *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHServicePeriod

	class CHServicePeriodPick : public udListHandlerPick, public CHServicePeriod {

	public:
		CHServicePeriodPick( );
		CHServicePeriodPick( const CHServicePeriodPick& objCopy );

		virtual ~CHServicePeriodPick( );

		CHServicePeriodPick& operator =( const CHServicePeriodPick& objCopy );

		virtual void	Init( );
		virtual void	OnButtonForm( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual void BeforeCreate( ) {
			Init( );
			Load( );
		} // void BeforeCreate

		//virtual udPWndHandler	GetWindowPick( ) const { return m_wndListWrap; }
		virtual udPWndHandler	GetWindow( ) const { return m_wndListWrap; }

	}; // class CHServicePeriodPick

	class CHServicePeriodForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblDate1,
						m_lblDate2;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		Gtk::Table		m_tblInput;

		udPHServiceForm m_pServiceForm;

	public:
		CHServicePeriodForm( udPHServiceForm pServiceForm = NULL );
		CHServicePeriodForm( const CHServicePeriodForm& objCopy );

		virtual ~CHServicePeriodForm( );

		CHServicePeriodForm& operator =( const CHServicePeriodForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHServicePeriodForm

	class CHServicePeriodFormModal : public CHServicePeriodForm {

	public:
		CHServicePeriodFormModal( udPHServiceForm pServiceForm = NULL );
		CHServicePeriodFormModal( const CHServicePeriodFormModal& objCopy );

		virtual ~CHServicePeriodFormModal( );

		CHServicePeriodFormModal& operator =( const CHServicePeriodFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHServicePeriodFormModal

} // namespace UdSDK

#endif
