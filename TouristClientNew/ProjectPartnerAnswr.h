/*
	Tourist application
	Клиентское приложение
	Ответы партнера
*/
#ifndef TOURIST_CLIENT_PARTNERANSWR_H
#define TOURIST_CLIENT_PARTNERANSWR_H

namespace UdSDK {

	udPBase	GetProjectPartnerAnswrList( );

	class CHPartnerAnswr : public udListHandlerBtnForm, public udListHandler {

	public:
		CHPartnerAnswr( );
		CHPartnerAnswr( const CHPartnerAnswr& objCopy );

		virtual ~CHPartnerAnswr( );

		CHPartnerAnswr& operator =( const CHPartnerAnswr& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelPartnerAnswr : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelPartnerAnswr( ) {
				add( m_col_id );
				add( m_col_created );
				add( m_col_partner );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_partner;
		};

	protected:
		ColModelPartnerAnswr *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		enum {
			fltPartner
		};

		Gtk::Label		m_lblFltPartner,
						m_lblFltDate1,
						m_lblFltDate2;
		udWgtListPick	m_inpFltPartner;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHPartnerAnswr

	class CHPartnerAnswrModeless : public udListHandlerModeless, public CHPartnerAnswr {
		Gtk::Button	m_btnRequest;

	public:
		CHPartnerAnswrModeless( );
		CHPartnerAnswrModeless( const CHPartnerAnswrModeless& objCopy );

		virtual ~CHPartnerAnswrModeless( );

		CHPartnerAnswrModeless& operator =( const CHPartnerAnswrModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHPartnerAnswrModeless

	class CHPartnerAnswrPick : public udListHandlerPick, public CHPartnerAnswr {

	public:
		CHPartnerAnswrPick( );
		CHPartnerAnswrPick( const CHPartnerAnswrPick& objCopy );

		virtual ~CHPartnerAnswrPick( );

		CHPartnerAnswrPick& operator =( const CHPartnerAnswrPick& objCopy );

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

	}; // class CHPartnerAnswrPick

	class CHPartnerAnswrForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblPartner,
						m_lblText;
		udWgtInput		m_inpText;
		Gtk::Table		m_tblInput;
		udWgtListPick	m_inpPartner;

	public:
		enum {
			ctrlidPartner
		};

	public:
		CHPartnerAnswrForm( );
		CHPartnerAnswrForm( const CHPartnerAnswrForm& objCopy );

		virtual ~CHPartnerAnswrForm( );

		CHPartnerAnswrForm& operator =( const CHPartnerAnswrForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHPartnerAnswrForm

	class CHPartnerAnswrFormModal : public CHPartnerAnswrForm {

	public:
		CHPartnerAnswrFormModal( );
		CHPartnerAnswrFormModal( const CHPartnerAnswrFormModal& objCopy );

		virtual ~CHPartnerAnswrFormModal( );

		CHPartnerAnswrFormModal& operator =( const CHPartnerAnswrFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPartnerAnswrFormModal

	class CHPartnerAnswrFormModeless : public CHPartnerAnswrForm {

	public:
		CHPartnerAnswrFormModeless( );
		CHPartnerAnswrFormModeless( const CHPartnerAnswrFormModeless& objCopy );

		virtual ~CHPartnerAnswrFormModeless( );

		CHPartnerAnswrFormModeless& operator =( const CHPartnerAnswrFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPartnerAnswrFormModal

} // namespace UdSDK

#endif
