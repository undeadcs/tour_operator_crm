/*
	Tourist application
	Клиентское приложение
	Периоды группы
*/
#ifndef TOURIST_CLIENT_PERIOD_GROUP_H
#define TOURIST_CLIENT_PERIOD_GROUP_H

namespace UdSDK {

	udPBase GetProjectGroupPeriodList( );

	class CWgtGroupPeriodPick : public udWgtListPick {
		udPHGroupForm	m_pGroupForm;

	public:
		CWgtGroupPeriodPick( );
		CWgtGroupPeriodPick( const CWgtGroupPeriodPick& objCopy );

		virtual ~CWgtGroupPeriodPick( );

		CWgtGroupPeriodPick& operator =( const CWgtGroupPeriodPick& objCopy );

		virtual void	OnButtonClick( );

		void	SetGroupForm( udPHGroupForm pGroupForm ) { m_pGroupForm = pGroupForm; }

	}; // class CWgtGroupPeriodPick

	class CDAListGroupPeriod : public udDAList {
		udPHGroupForm	m_pGroupForm;

	public:
		CDAListGroupPeriod( );
		CDAListGroupPeriod( const CDAListGroupPeriod& objCopy );

		virtual ~CDAListGroupPeriod( );

		CDAListGroupPeriod& operator =( const CDAListGroupPeriod& objCopy );

		virtual void	Load( );

		void	SetGroupForm( udPHGroupForm pGroupForm ) { m_pGroupForm = pGroupForm; }

	}; // class CDAListGroupPeriod

	class CHGroupPeriod : public udListHandlerBtnForm, public udListHandler {
	protected:
		udPHGroupForm m_pGroupForm;
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHGroupPeriod( );
		CHGroupPeriod( const CHGroupPeriod& objCopy );

		virtual ~CHGroupPeriod( );

		CHGroupPeriod& operator =( const CHGroupPeriod& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		void	SetGroupForm( udPHGroupForm pGroupForm );

		class ColModelGroupPeriod : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelGroupPeriod( ) {
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_min );
				add( m_col_max );
				add( m_col_client );
			}

			Gtk::TreeModelColumn< Glib::ustring	> m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date2;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_min;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_max;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_client;
		};

	protected:
		ColModelGroupPeriod *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHGroupPeriod

	class CHGroupPeriodPick : public udListHandlerPick, public CHGroupPeriod {
		udPHGroupTourForm	m_pGroupTourForm;

	public:
		CHGroupPeriodPick( );
		CHGroupPeriodPick( const CHGroupPeriodPick& objCopy );

		virtual ~CHGroupPeriodPick( );

		CHGroupPeriodPick& operator =( const CHGroupPeriodPick& objCopy );

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

		void	SetGroupTourForm( udPHGroupTourForm pGroupTourForm ) { m_pGroupTourForm = pGroupTourForm; }

	}; // class CHGroupPeriodPick

	class CHGroupPeriodForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblDate1,
						m_lblDate2,
						m_lblMin,
						m_lblMax,
						m_lblClient;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		udWgtInput		m_inpMin,
						m_inpMax;
		udWgtListPick	m_inpClient;
		Gtk::Table		m_tblInput;

		udPHGroupForm m_pGroupForm;

	public:
		enum {
			ctrlidClient
		};

	public:
		CHGroupPeriodForm( udPHGroupForm pGroupForm = NULL );
		CHGroupPeriodForm( const CHGroupPeriodForm& objCopy );

		virtual ~CHGroupPeriodForm( );

		CHGroupPeriodForm& operator =( const CHGroupPeriodForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHGroupPeriodForm

	class CHGroupPeriodFormModal : public CHGroupPeriodForm {

	public:
		CHGroupPeriodFormModal( udPHGroupForm pGroupForm = NULL );
		CHGroupPeriodFormModal( const CHGroupPeriodFormModal& objCopy );

		virtual ~CHGroupPeriodFormModal( );

		CHGroupPeriodFormModal& operator =( const CHGroupPeriodFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHGroupPeriodFormModal

	udPBase GetProjectGroupPeriodLoadList( );

	class CHGroupPeriodLoad : public udListHandler {

	public:
		CHGroupPeriodLoad( );
		CHGroupPeriodLoad( const CHGroupPeriodLoad& objCopy );

		virtual ~CHGroupPeriodLoad( );

		CHGroupPeriodLoad& operator =( const CHGroupPeriodLoad& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		void	SetGroupId( udUInt id );

		class ColModelGroupPeriodLoad : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelGroupPeriodLoad( ) {
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_min );
				add( m_col_max );
				add( m_col_client );
				add( m_col_object );
			}

			Gtk::TreeModelColumn< Glib::ustring	> m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date2;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_min;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_max;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_client;
			Gtk::TreeModelColumn< udPBase		> m_col_object;
		};

	protected:
		ColModelGroupPeriodLoad *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHGroupPeriodLoad

	class CHGroupPeriodLoadPick : public udListHandlerPick, public CHGroupPeriodLoad {
		udPHTourForm	m_pTourForm;

	public:
		CHGroupPeriodLoadPick( );
		CHGroupPeriodLoadPick( const CHGroupPeriodLoadPick& objCopy );

		virtual ~CHGroupPeriodLoadPick( );

		CHGroupPeriodLoadPick& operator =( const CHGroupPeriodLoadPick& objCopy );

		virtual void	Init( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual void BeforeCreate( ) {
			Init( );
			Load( );
		} // void BeforeCreate

		//virtual udPWndHandler	GetWindowPick( ) const { return m_wndListWrap; }
		virtual udPWndHandler	GetWindow( ) const { return m_wndListWrap; }

		void	SetTourForm( udPHTourForm pTourForm ) { m_pTourForm = pTourForm; }

	}; // class CHGroupPeriodLoadPick

	class CWgtGroupPeriodLoadPick : public udWgtListPick {
		udPDAList	m_pGroupFrom; // объект, у которого будет браться атрибут group

	public:
		CWgtGroupPeriodLoadPick( );
		CWgtGroupPeriodLoadPick( const CWgtGroupPeriodLoadPick& objCopy );

		virtual ~CWgtGroupPeriodLoadPick( );

		CWgtGroupPeriodLoadPick& operator =( const CWgtGroupPeriodLoadPick& objCopy );

		virtual void	OnButtonClick( );

		void	SetGroupFrom( udPDAList pGroupFrom ) { m_pGroupFrom = pGroupFrom; }

	}; // class CWgtGroupPeriodLoadPick

} // namespace UdSDK

#endif
