/*
	Tourist application
	Клиентское приложение
	Направление
*/
#ifndef TOURIST_CLIENT_DIRECT_H
#define TOURIST_CLIENT_DIRECT_H

namespace UdSDK {

	udPBase	GetProjectDirectList( );

	class CHDirect : public udListHandlerBtnForm, public udListHandler {

	public:
		CHDirect( );
		CHDirect( const CHDirect& objCopy );

		virtual ~CHDirect( );

		CHDirect& operator =( const CHDirect& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelDirect : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelDirect( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelDirect *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHDirect

	class CHDirectModeless : public udListHandlerModeless, public CHDirect {

	public:
		CHDirectModeless( );
		CHDirectModeless( const CHDirectModeless& objCopy );

		virtual ~CHDirectModeless( );

		CHDirectModeless& operator =( const CHDirectModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHDirectModeless

	class CHDirectPick : public udListHandlerPick, public CHDirect {

	public:
		CHDirectPick( );
		CHDirectPick( const CHDirectPick& objCopy );

		virtual ~CHDirectPick( );

		CHDirectPick& operator =( const CHDirectPick& objCopy );

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

	}; // class CHDirectPick

	class CHDirectForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;		// наименование
		Gtk::Table		m_tblInput;


	public:
		CHDirectForm( );
		CHDirectForm( const CHDirectForm& objCopy );

		virtual ~CHDirectForm( );

		CHDirectForm& operator =( const CHDirectForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHDirectForm

	class CHDirectFormModal : public CHDirectForm {

	public:
		CHDirectFormModal( );
		CHDirectFormModal( const CHDirectFormModal& objCopy );

		virtual ~CHDirectFormModal( );

		CHDirectFormModal& operator =( const CHDirectFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDirectFormModal

	class CHDirectFormModeless : public CHDirectForm {

	public:
		CHDirectFormModeless( );
		CHDirectFormModeless( const CHDirectFormModeless& objCopy );

		virtual ~CHDirectFormModeless( );

		CHDirectFormModeless& operator =( const CHDirectFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDirectFormModal

} // namespace UdSDK

#endif
