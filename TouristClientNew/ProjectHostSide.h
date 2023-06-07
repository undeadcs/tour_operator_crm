/*
	Tourist application
	Клиентское приложение
	Принимающие стороны
*/
#ifndef TOURIST_CLIENT_HOST_SIDE_H
#define TOURIST_CLIENT_HOST_SIDE_H

namespace UdSDK {

	udPBase	GetProjectHostSideList( );

	class CHHostSide : public udListHandlerBtnForm, public udListHandler {

	public:
		CHHostSide( );
		CHHostSide( const CHHostSide& objCopy );

		virtual ~CHHostSide( );

		CHHostSide& operator =( const CHHostSide& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelHostSide : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelHostSide( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelHostSide *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHHostSide

	class CHHostSideModeless : public udListHandlerModeless, public CHHostSide {

	public:
		CHHostSideModeless( );
		CHHostSideModeless( const CHHostSideModeless& objCopy );

		virtual ~CHHostSideModeless( );

		CHHostSideModeless& operator =( const CHHostSideModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHHostSideModeless

	class CHHostSidePick : public udListHandlerPick, public CHHostSide {

	public:
		CHHostSidePick( );
		CHHostSidePick( const CHHostSidePick& objCopy );

		virtual ~CHHostSidePick( );

		CHHostSidePick& operator =( const CHHostSidePick& objCopy );

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

	}; // class CHHostSidePick

	class CHHostSideForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblNameLatin;
		udWgtInput		m_inpName,
						m_inpNameLatin;
		Gtk::Table		m_tblInput;


	public:
		CHHostSideForm( );
		CHHostSideForm( const CHHostSideForm& objCopy );

		virtual ~CHHostSideForm( );

		CHHostSideForm& operator =( const CHHostSideForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHHostSideForm

	class CHHostSideFormModal : public CHHostSideForm {

	public:
		CHHostSideFormModal( );
		CHHostSideFormModal( const CHHostSideFormModal& objCopy );

		virtual ~CHHostSideFormModal( );

		CHHostSideFormModal& operator =( const CHHostSideFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHHostSideFormModal

	class CHHostSideFormModeless : public CHHostSideForm {

	public:
		CHHostSideFormModeless( );
		CHHostSideFormModeless( const CHHostSideFormModeless& objCopy );

		virtual ~CHHostSideFormModeless( );

		CHHostSideFormModeless& operator =( const CHHostSideFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHHostSideFormModal

} // namespace UdSDK

#endif
