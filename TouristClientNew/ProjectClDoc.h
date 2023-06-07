/*
	Tourist application
	Клиентское приложение
	Документ клиента
*/
#ifndef TOURIST_CLIENT_CLDOC_H
#define TOURIST_CLIENT_CLDOC_H

namespace UdSDK {

	udPBase	GetProjectClDocList( );

	class CHClDoc : public udListHandlerBtnForm, public udListHandler {

	public:
		CHClDoc( );
		CHClDoc( const CHClDoc& objCopy );

		virtual ~CHClDoc( );

		CHClDoc& operator =( const CHClDoc& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelClDoc : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelClDoc( ) {
				add( m_col_id );
				add( m_col_client );
				add( m_col_type );
				add( m_col_series );
				add( m_col_number );
				add( m_col_object );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_client;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_type;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_series;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_number;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		};

	protected:
		ColModelClDoc *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltClient,
						m_lblFltType,
						m_lblFltSeries,
						m_lblFltNumber;
		udWgtInput		m_inpFltSeries,
						m_inpFltNumber;
		udWgtListPick	m_inpFltClient,
						m_inpFltType;

		enum {
			fltClient,
			fltType
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHClDoc

	class CHClDocModeless : public udListHandlerModeless, public CHClDoc {

	public:
		CHClDocModeless( );
		CHClDocModeless( const CHClDocModeless& objCopy );

		virtual ~CHClDocModeless( );

		CHClDocModeless& operator =( const CHClDocModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHClDocModeless

	class CHClDocPick : public udListHandlerPick, public CHClDoc {

	public:
		CHClDocPick( );
		CHClDocPick( const CHClDocPick& objCopy );

		virtual ~CHClDocPick( );

		CHClDocPick& operator =( const CHClDocPick& objCopy );

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

	}; // class CHClDocPick

	class CHClDocForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblClient,
						m_lblType,
						m_lblSeries,
						m_lblNumber,
						m_lblIssueDate,
						m_lblEndDate,
						m_lblIssuedBy,
						m_lblBirthPlace,
						m_lblFio,
						m_lblFioLatin;
		udWgtInput		m_inpSeries,
						m_inpNumber,
						m_inpIssuedBy,
						m_inpBirthPlace,
						m_inpFio,
						m_inpFioLatin;
		udWgtDatePick	m_inpIssueDate,
						m_inpEndDate;
		Gtk::Table		m_tblInput;
		udWgtListPick	m_inpClient,
						m_inpType;
	public:
		enum {
			ctrlidClient,
			ctrlidType
		};

	public:
		CHClDocForm( );
		CHClDocForm( const CHClDocForm& objCopy );

		virtual ~CHClDocForm( );

		CHClDocForm& operator =( const CHClDocForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHClDocForm

	class CHClDocFormModal : public CHClDocForm {
		udUInt	m_iClientId;
		bool	m_bSend,
				m_bShowClientField;

	public:
		CHClDocFormModal( );
		CHClDocFormModal( const CHClDocFormModal& objCopy );
		CHClDocFormModal( udUInt iClientId, bool bShowClientField = true, bool bSend = true );

		virtual ~CHClDocFormModal( );

		CHClDocFormModal& operator =( const CHClDocFormModal& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual udInt	Validate( );
		virtual void	Send( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHClDocFormModal

	class CHClDocFormModeless : public CHClDocForm {

	public:
		CHClDocFormModeless( );
		CHClDocFormModeless( const CHClDocFormModeless& objCopy );

		virtual ~CHClDocFormModeless( );

		CHClDocFormModeless& operator =( const CHClDocFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHClDocFormModal

	class CWgtClDocFromForm : public udWgtListPick {
		udPHClientForm	m_pForm;

	public:
		CWgtClDocFromForm( );
		CWgtClDocFromForm( const CWgtClDocFromForm& objCopy );

		virtual ~CWgtClDocFromForm( );

		CWgtClDocFromForm& operator =( const CWgtClDocFromForm& objCopy );

		void	SetForm( udPHClientForm pForm ) { m_pForm = pForm; }

	protected:
		virtual void	OnButtonClick( );
		virtual bool	OnClick( GdkEventButton* evtClick );

	}; // class CWgtClDocFromForm 

	class CDAListClDocFromForm : public udDAList {
		udPHClientForm	m_pForm;

	public:
		CDAListClDocFromForm( );
		CDAListClDocFromForm( const CDAListClDocFromForm& objCopy );

		virtual ~CDAListClDocFromForm( );

		CDAListClDocFromForm& operator =( const CDAListClDocFromForm& objCopy );

		virtual void	Load( );

		void	SetForm( udPHClientForm pForm ) { m_pForm = pForm; }

	}; // class CDAListClDocFromForm

	class CHClDocFromFormPick : public udListHandlerPick, public CHClDoc {
		udPHClientForm	m_pForm;

	public:
		CHClDocFromFormPick( );
		CHClDocFromFormPick( const CHClDocFromFormPick& objCopy );

		virtual ~CHClDocFromFormPick( );

		CHClDocFromFormPick& operator =( const CHClDocFromFormPick& objCopy );

		void	SetForm( udPHClientForm pForm );

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

	};

} // namespace UdSDK

#endif
