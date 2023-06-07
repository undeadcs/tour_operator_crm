/*
	Tourist application
	Клиентское приложение
	Тип обращения
*/
#ifndef TOURIST_CLIENT_TREQUEST_H
#define TOURIST_CLIENT_TREQUEST_H

namespace UdSDK {

	udPBase	GetProjectTRequestList( );

	class CHTRequest : public udListHandlerBtnForm, public udListHandler {

	public:
		CHTRequest( );
		CHTRequest( const CHTRequest& objCopy );

		virtual ~CHTRequest( );

		CHTRequest& operator =( const CHTRequest& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelTRequest : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTRequest( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelTRequest *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTRequest

	class CHTRequestModeless : public udListHandlerModeless, public CHTRequest {

	public:
		CHTRequestModeless( );
		CHTRequestModeless( const CHTRequestModeless& objCopy );

		virtual ~CHTRequestModeless( );

		CHTRequestModeless& operator =( const CHTRequestModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHTRequestModeless

	class CHTRequestPick : public udListHandlerPick, public CHTRequest {

	public:
		CHTRequestPick( );
		CHTRequestPick( const CHTRequestPick& objCopy );

		virtual ~CHTRequestPick( );

		CHTRequestPick& operator =( const CHTRequestPick& objCopy );

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

	}; // class CHTRequestPick

	class CHTRequestForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;

	public:
		CHTRequestForm( );
		CHTRequestForm( const CHTRequestForm& objCopy );

		virtual ~CHTRequestForm( );

		CHTRequestForm& operator =( const CHTRequestForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTRequestForm

	class CHTRequestFormModal : public CHTRequestForm {

	public:
		CHTRequestFormModal( );
		CHTRequestFormModal( const CHTRequestFormModal& objCopy );

		virtual ~CHTRequestFormModal( );

		CHTRequestFormModal& operator =( const CHTRequestFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTRequestFormModal

	class CHTRequestFormModeless : public CHTRequestForm {

	public:
		CHTRequestFormModeless( );
		CHTRequestFormModeless( const CHTRequestFormModeless& objCopy );

		virtual ~CHTRequestFormModeless( );

		CHTRequestFormModeless& operator =( const CHTRequestFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTRequestFormModal

} // namespace UdSDK

#endif
