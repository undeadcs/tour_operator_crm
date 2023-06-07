/*
	Tourist application
	Клиентское приложение
	Распечатанные документы
*/
#ifndef TOURIST_CLIENT_DOC_TEMPLATE_PRINTED_H
#define TOURIST_CLIENT_DOC_TEMPLATE_PRINTED_H

namespace UdSDK {

	udPBase	GetProjectDocTemplatePrintedList( );

	class CHDocTemplatePrinted : public udListHandler {

	public:
		CHDocTemplatePrinted( );
		CHDocTemplatePrinted( const CHDocTemplatePrinted& objCopy );

		virtual ~CHDocTemplatePrinted( );

		CHDocTemplatePrinted& operator =( const CHDocTemplatePrinted& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelDocTemplatePrinted : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelDocTemplatePrinted( ) {
				add( m_col_id );
				add( m_col_created );
				add( m_col_createdby );
				add( m_col_tour );
				add( m_col_series );
				add( m_col_number );
				add( m_col_type );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_createdby;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_tour;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_series;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_number;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_type;
		};

	protected:
		ColModelDocTemplatePrinted *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltSeries,
						m_lblFltNumber,
						m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltCreatedBy,
						m_lblFltType,
						m_lblFltTour;
		udWgtInput		m_inpFltSeries,
						m_inpFltNumber;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtListPick	m_inpFltCreatedBy,
						m_inpFltTour;
		udWgtSelect		m_inpFltType;

		enum {
			fltCreatedBy,
			fltTour
		};

	}; // class CHDocTemplatePrinted

	class CHDocTemplatePrintedModeless : public udListHandlerModeless, public CHDocTemplatePrinted {

	public:
		CHDocTemplatePrintedModeless( );
		CHDocTemplatePrintedModeless( const CHDocTemplatePrintedModeless& objCopy );

		virtual ~CHDocTemplatePrintedModeless( );

		CHDocTemplatePrintedModeless& operator =( const CHDocTemplatePrintedModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHDocTemplatePrintedModeless

	class CHDocTemplatePrintedPick : public udListHandlerPick, public CHDocTemplatePrinted {

	public:
		CHDocTemplatePrintedPick( );
		CHDocTemplatePrintedPick( const CHDocTemplatePrintedPick& objCopy );

		virtual ~CHDocTemplatePrintedPick( );

		CHDocTemplatePrintedPick& operator =( const CHDocTemplatePrintedPick& objCopy );

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

	}; // class CHDocTemplatePrintedPick

	class CHDocTemplatePrintedForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblCreated,
						m_lblCreatedBy,
						m_lblTour,
						m_lblSeries,
						m_lblNumber,
						m_lblName,
						m_lblType,
						// а это уже значения атрибутов, сущность не редактируемая
						m_inpCreated,
						m_inpCreatedBy,
						m_inpTour,
						m_inpSeries,
						m_inpNumber,
						m_inpName,
						m_inpType;
		Gtk::Table		m_tblInput;

		wstring			m_szFolder;
		Gtk::Button		m_btnPrintDoc,
						m_btnOpenFolder;

	public:
		enum {
			ctrlidCreatedBy,
			ctrlidTour,
			ctrlidXsl,
			ctrlidXml
		};

	public:
		CHDocTemplatePrintedForm( );
		CHDocTemplatePrintedForm( const CHDocTemplatePrintedForm& objCopy );

		virtual ~CHDocTemplatePrintedForm( );

		CHDocTemplatePrintedForm& operator =( const CHDocTemplatePrintedForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		virtual void	OnButtonPrintDoc( );
		virtual void	OnButtonOpenFolder( );

	}; // class CHDocTemplatePrintedForm

	class CHDocTemplatePrintedFormModal : public CHDocTemplatePrintedForm {

	public:
		CHDocTemplatePrintedFormModal( );
		CHDocTemplatePrintedFormModal( const CHDocTemplatePrintedFormModal& objCopy );

		virtual ~CHDocTemplatePrintedFormModal( );

		CHDocTemplatePrintedFormModal& operator =( const CHDocTemplatePrintedFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDocTemplatePrintedFormModal

	class CHDocTemplatePrintedFormModeless : public CHDocTemplatePrintedForm {

	public:
		CHDocTemplatePrintedFormModeless( );
		CHDocTemplatePrintedFormModeless( const CHDocTemplatePrintedFormModeless& objCopy );

		virtual ~CHDocTemplatePrintedFormModeless( );

		CHDocTemplatePrintedFormModeless& operator =( const CHDocTemplatePrintedFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHDocTemplatePrintedFormModal

} // namespace UdSDK

#endif
