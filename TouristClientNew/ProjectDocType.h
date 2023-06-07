/*
	Tourist application
	Клиентское приложение
	Тип документа
*/
#ifndef TOURIST_CLIENT_DOC_TYPE_H
#define TOURIST_CLIENT_DOC_TYPE_H

namespace UdSDK {

	udPBase	GetProjectDocTypeList( );

	class CHDocType : public udListHandlerBtnForm, public udListHandler {

	public:
		CHDocType( );
		CHDocType( const CHDocType& objCopy );

		virtual ~CHDocType( );

		CHDocType& operator =( const CHDocType& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelDocType : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelDocType( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelDocType *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHDocType

	class CHDocTypeModeless : public udListHandlerModeless, public CHDocType {

	public:
		CHDocTypeModeless( );
		CHDocTypeModeless( const CHDocTypeModeless& objCopy );

		virtual ~CHDocTypeModeless( );

		CHDocTypeModeless& operator =( const CHDocTypeModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHDocTypeModeless

	class CHDocTypePick : public udListHandlerPick, public CHDocType {

	public:
		CHDocTypePick( );
		CHDocTypePick( const CHDocTypePick& objCopy );

		virtual ~CHDocTypePick( );

		CHDocTypePick& operator =( const CHDocTypePick& objCopy );

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

	}; // class CHDocTypePick

	class CHDocTypeForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;

	public:
		CHDocTypeForm( );
		CHDocTypeForm( const CHDocTypeForm& objCopy );

		virtual ~CHDocTypeForm( );

		CHDocTypeForm& operator =( const CHDocTypeForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHDocTypeForm

	class CHDocTypeFormModal : public CHDocTypeForm {

	public:
		CHDocTypeFormModal( );
		CHDocTypeFormModal( const CHDocTypeFormModal& objCopy );

		virtual ~CHDocTypeFormModal( );

		CHDocTypeFormModal& operator =( const CHDocTypeFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDocTypeFormModal

	class CHDocTypeFormModeless : public CHDocTypeForm {

	public:
		CHDocTypeFormModeless( );
		CHDocTypeFormModeless( const CHDocTypeFormModeless& objCopy );

		virtual ~CHDocTypeFormModeless( );

		CHDocTypeFormModeless& operator =( const CHDocTypeFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDocTypeFormModal

} // namespace UdSDK

#endif
