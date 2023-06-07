/*
	Tourist application
	Клиентское приложение
	Запрос партнеру
*/
#ifndef TOURIST_CLIENT_PARTNERREQ_H
#define TOURIST_CLIENT_PARTNERREQ_H

namespace UdSDK {

	udPBase	GetProjectPartnerReqList( );

	class CHPartnerReq : public udListHandlerBtnForm, public udListHandler {

	public:
		CHPartnerReq( );
		CHPartnerReq( const CHPartnerReq& objCopy );

		virtual ~CHPartnerReq( );

		CHPartnerReq& operator =( const CHPartnerReq& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelPartnerReq : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelPartnerReq( ) {
				add( m_col_id );
				add( m_col_partner );
				add( m_col_created );
				add( m_col_manager );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_partner;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_manager;
		};

	protected:
		ColModelPartnerReq *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltPartner,
						m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltUser,
						m_lblFltOrder;

		udWgtListPick	m_inpFltPartner,
						m_inpFltUser,
						m_inpFltOrder;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;

		enum {
			fltPartner,
			fltUser,
			fltOrder
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHPartnerReq

	class CHPartnerReqModeless : public udListHandlerModeless, public CHPartnerReq {
		Gtk::Button	m_btnAnswr;

	public:
		CHPartnerReqModeless( );
		CHPartnerReqModeless( const CHPartnerReqModeless& objCopy );

		virtual ~CHPartnerReqModeless( );

		CHPartnerReqModeless& operator =( const CHPartnerReqModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHPartnerReqModeless

	class CHPartnerReqPick : public udListHandlerPick, public CHPartnerReq {

	public:
		CHPartnerReqPick( );
		CHPartnerReqPick( const CHPartnerReqPick& objCopy );

		virtual ~CHPartnerReqPick( );

		CHPartnerReqPick& operator =( const CHPartnerReqPick& objCopy );

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

	}; // class CHPartnerReqPick

	class CHPartnerReqForm : public udFormHandler {
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
		CHPartnerReqForm( );
		CHPartnerReqForm( const CHPartnerReqForm& objCopy );

		virtual ~CHPartnerReqForm( );

		CHPartnerReqForm& operator =( const CHPartnerReqForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHPartnerReqForm

	class CHPartnerReqFormModal : public CHPartnerReqForm {

	public:
		CHPartnerReqFormModal( );
		CHPartnerReqFormModal( const CHPartnerReqFormModal& objCopy );

		virtual ~CHPartnerReqFormModal( );

		CHPartnerReqFormModal& operator =( const CHPartnerReqFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPartnerReqFormModal

	class CHPartnerReqFormModeless : public CHPartnerReqForm {

	public:
		CHPartnerReqFormModeless( );
		CHPartnerReqFormModeless( const CHPartnerReqFormModeless& objCopy );

		virtual ~CHPartnerReqFormModeless( );

		CHPartnerReqFormModeless& operator =( const CHPartnerReqFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPartnerReqFormModal

} // namespace UdSDK

#endif
