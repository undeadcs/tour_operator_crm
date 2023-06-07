/*
	Tourist application
	Клиентское приложение
	Пункт перехода
*/
#ifndef TOURIST_CLIENT_TRANSFER_POINT_H
#define TOURIST_CLIENT_TRANSFER_POINT_H

namespace UdSDK {

	udPBase	GetProjectTransferPointList( );

	class CHTransferPoint : public udListHandlerBtnForm, public udListHandler {

	public:
		CHTransferPoint( );
		CHTransferPoint( const CHTransferPoint& objCopy );

		virtual ~CHTransferPoint( );

		CHTransferPoint& operator =( const CHTransferPoint& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelTransferPoint : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTransferPoint( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelTransferPoint *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTransferPoint

	class CHTransferPointModeless : public udListHandlerModeless, public CHTransferPoint {

	public:
		CHTransferPointModeless( );
		CHTransferPointModeless( const CHTransferPointModeless& objCopy );

		virtual ~CHTransferPointModeless( );

		CHTransferPointModeless& operator =( const CHTransferPointModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHTransferPointModeless

	class CHTransferPointPick : public udListHandlerPick, public CHTransferPoint {

	public:
		CHTransferPointPick( );
		CHTransferPointPick( const CHTransferPointPick& objCopy );

		virtual ~CHTransferPointPick( );

		CHTransferPointPick& operator =( const CHTransferPointPick& objCopy );

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

	}; // class CHTransferPointPick

	class CHTransferPointForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblNameLatin;
		udWgtInput		m_inpName,
						m_inpNameLatin;
		Gtk::Table		m_tblInput;


	public:
		CHTransferPointForm( );
		CHTransferPointForm( const CHTransferPointForm& objCopy );

		virtual ~CHTransferPointForm( );

		CHTransferPointForm& operator =( const CHTransferPointForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTransferPointForm

	class CHTransferPointFormModal : public CHTransferPointForm {

	public:
		CHTransferPointFormModal( );
		CHTransferPointFormModal( const CHTransferPointFormModal& objCopy );

		virtual ~CHTransferPointFormModal( );

		CHTransferPointFormModal& operator =( const CHTransferPointFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTransferPointFormModal

	class CHTransferPointFormModeless : public CHTransferPointForm {

	public:
		CHTransferPointFormModeless( );
		CHTransferPointFormModeless( const CHTransferPointFormModeless& objCopy );

		virtual ~CHTransferPointFormModeless( );

		CHTransferPointFormModeless& operator =( const CHTransferPointFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTransferPointFormModal

} // namespace UdSDK

#endif
