/*
	Tourist application
	Клиентское приложение
	Документ заказчика
*/
#ifndef TOURIST_CLIENT_EMPLOYER_DOC_H
#define TOURIST_CLIENT_EMPLOYER_DOC_H

namespace UdSDK {

	class CWgtEmployerDocPick : public udWgtListPick {

	public:
		CWgtEmployerDocPick( );
		CWgtEmployerDocPick( const CWgtEmployerDocPick& objCopy );

		virtual ~CWgtEmployerDocPick( );

		CWgtEmployerDocPick& operator =( const CWgtEmployerDocPick& objCopy );

		void	CallClick( ) { OnButtonClick( ); }

		class Handler {

		public:
			Handler( ) { }

			virtual ~Handler( ) { }

			virtual bool	BeforeCreatePickHandler( udPWgtEmployerDocPick wgt, udPHEmployerDocPick handler ) = 0;
			virtual udPHClDocFormModal GetEditHandler( udPWgtEmployerDocPick wgt ) = 0;
		};

	private:
		Handler* m_pHandler;

	public:
		void	SetHandler( Handler* pHandler ) { m_pHandler = pHandler; }

	protected:
		virtual void	OnButtonClick( );
		virtual bool	OnClick( GdkEventButton* evtClick );

	}; // class CWgtEmployerDocPick

	class CHEmployerDoc : public udListHandlerBtnForm, public udListHandler {
	protected:
		udUInt m_iEmployerId;
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHEmployerDoc( );
		CHEmployerDoc( const CHEmployerDoc& objCopy );

		virtual ~CHEmployerDoc( );

		CHEmployerDoc& operator =( const CHEmployerDoc& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );

		void	SetEmployerId( udUInt iEmployerId );

		class ColModelEmployerDoc : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelEmployerDoc( ) {
				add( m_col_id );
				add( m_col_type );
				add( m_col_series );
				add( m_col_number );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_type;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_series;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_number;
		};

	protected:
		ColModelEmployerDoc *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltType,
						m_lblFltSeries,
						m_lblFltNumber;
		udWgtInput		m_inpFltSeries,
						m_inpFltNumber;
		udWgtListPick	m_inpFltType;

		enum {
			fltClient,
			fltType
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHEmployerDoc

	class CHEmployerDocPick : public udListHandlerPick, public CHEmployerDoc {

	public:
		CHEmployerDocPick( );
		CHEmployerDocPick( const CHEmployerDocPick& objCopy );

		virtual ~CHEmployerDocPick( );

		CHEmployerDocPick& operator =( const CHEmployerDocPick& objCopy );

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

	}; // class CHEmployerDocPick

} // namespace UdSDK

#endif
