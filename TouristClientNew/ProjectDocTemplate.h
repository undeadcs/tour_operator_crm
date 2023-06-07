/*
	Tourist application
	Клиентское приложение
	Шаблоны документов
*/
#ifndef TOURIST_CLIENT_DOC_TEMPLATE_H
#define TOURIST_CLIENT_DOC_TEMPLATE_H

namespace UdSDK {

	udPBase	GetProjectDocTemplateList( );

	class CHDocTemplate : public udListHandlerBtnForm, public udListHandler {
	public:
		enum {
			opidGetFile = opidGet + 1,	// подгрузка шаблона
			opidGetResult,				// получение результата распечатки (договор,путевка)
		};

	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHDocTemplate( );
		CHDocTemplate( const CHDocTemplate& objCopy );

		virtual ~CHDocTemplate( );

		CHDocTemplate& operator =( const CHDocTemplate& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelDocTemplate : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelDocTemplate( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelDocTemplate *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	public:
		static void	ProcContractTemplate(
						udPFormHandler hForm, udPBase pClient, udPBase pTpl,
						udPBase pClDoc, /*const wstring& szAmount,*/
						udPArrBase pClients = NULL
					);

	}; // class CHDocTemplate

	class CHDocTemplateModeless : public udListHandlerModeless, public CHDocTemplate {

	public:
		CHDocTemplateModeless( );
		CHDocTemplateModeless( const CHDocTemplateModeless& objCopy );

		virtual ~CHDocTemplateModeless( );

		CHDocTemplateModeless& operator =( const CHDocTemplateModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHDocTemplateModeless

	class CHDocTemplatePick : public udListHandlerPick, public CHDocTemplate {

	public:
		CHDocTemplatePick( );
		CHDocTemplatePick( const CHDocTemplatePick& objCopy );

		virtual ~CHDocTemplatePick( );

		CHDocTemplatePick& operator =( const CHDocTemplatePick& objCopy );

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

	}; // class CHDocTemplatePick

	class CHDocTemplateForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblType,
						m_lblFile;
		udWgtInput		m_inpName;
		udWgtFilePick	m_inpFile;
		udWgtSelect		m_inpType;
		Gtk::Table		m_tblInput;

	public:
		enum {
			ctrlidFile
		};

	public:
		CHDocTemplateForm( );
		CHDocTemplateForm( const CHDocTemplateForm& objCopy );

		virtual ~CHDocTemplateForm( );

		CHDocTemplateForm& operator =( const CHDocTemplateForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHDocTemplateForm

	class CHDocTemplateFormModal : public CHDocTemplateForm {

	public:
		CHDocTemplateFormModal( );
		CHDocTemplateFormModal( const CHDocTemplateFormModal& objCopy );

		virtual ~CHDocTemplateFormModal( );

		CHDocTemplateFormModal& operator =( const CHDocTemplateFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDocTemplateFormModal

	class CHDocTemplateFormModeless : public CHDocTemplateForm {

	public:
		CHDocTemplateFormModeless( );
		CHDocTemplateFormModeless( const CHDocTemplateFormModeless& objCopy );

		virtual ~CHDocTemplateFormModeless( );

		CHDocTemplateFormModeless& operator =( const CHDocTemplateFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDocTemplateFormModal

} // namespace UdSDK

#endif
