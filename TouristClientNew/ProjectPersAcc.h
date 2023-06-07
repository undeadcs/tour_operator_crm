/*
	Tourist application
	Клиентское приложение
	Лицевые счета
*/
#ifndef TOURIST_CLIENT_PERSACC_H
#define TOURIST_CLIENT_PERSACC_H

namespace UdSDK {

	udPBase	GetProjectPersAccList( );
	udPBase GetProjectPersAccSummList( );
	udPBase GetProjectPersAccOperationList( );

	class CHPersAcc : public udListHandler {

	public:
		CHPersAcc( );
		CHPersAcc( const CHPersAcc& objCopy );

		virtual ~CHPersAcc( );

		CHPersAcc& operator =( const CHPersAcc& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelPersAcc : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelPersAcc( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelPersAcc *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltCompany,
						m_lblFltPartner,
						m_lblFltClient;
		udWgtListPick	m_inpFltCompany,
						m_inpFltPartner;
		udPWgtEmployerPick
						m_inpFltClient;

		enum {
			fltCompany,
			fltPartner,
			fltClient
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHPersAcc

	class CHPersAccModeless : public udListHandlerModeless, public CHPersAcc {

	public:
		CHPersAccModeless( );
		CHPersAccModeless( const CHPersAccModeless& objCopy );

		virtual ~CHPersAccModeless( );

		CHPersAccModeless& operator =( const CHPersAccModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHPersAccModeless

	class CHPersAccPick : public udListHandlerPick, public CHPersAcc {

	public:
		CHPersAccPick( );
		CHPersAccPick( const CHPersAccPick& objCopy );

		virtual ~CHPersAccPick( );

		CHPersAccPick& operator =( const CHPersAccPick& objCopy );

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

	}; // class CHPersAccPick

	class CFDEPersAccObject : public udFormDataExtra {
		udPDAForm	m_pData;

	public:
		CFDEPersAccObject( );
		CFDEPersAccObject( const CFDEPersAccObject& objCopy );

		virtual ~CFDEPersAccObject( );

		CFDEPersAccObject& operator =( const CFDEPersAccObject& objCopy );

		virtual void	Load( udPSocket sock );
		virtual void	Clear( ) { }
		virtual void	Send( udPSocket sock ) { }
		virtual void	Make( ) { }

		void	SetDataObject( udPDAForm obj ) { m_pData = obj; }

	}; // class CDAPersAccObject

	class CHPersAccForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						// наименования типов объектов
						m_lblObject1,
						m_lblObject2,
						m_lblSumm,
						m_lblOperation,
						// наименования объектов
						m_inpObject1,
						m_inpObject2;
		Gtk::Table		m_tblInput;

	public:
		enum {
			ctrlidPersAccConfig,
			ctrlidObject1,
			ctrlidObject2,
			ctrlidSumm,
			ctrlidOperation
		};

		enum {
			opidSummAdd = opidGet + 1,	// операция добавления суммы
			opidSummBack,				// снятие
			opidSummConvert,			// конвертация
		};

	public:
		CHPersAccForm( );
		CHPersAccForm( const CHPersAccForm& objCopy );

		virtual ~CHPersAccForm( );

		CHPersAccForm& operator =( const CHPersAccForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListSumm : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListSumm( ) {
				add( m_col_summ );
				add( m_col_currency );
				add( m_col_type );
				add( m_col_object );
			} // ColModelFormListSumm

			Gtk::TreeModelColumn< Glib::ustring > m_col_summ;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListSumm

	protected:
		ColModelFormListSumm* m_pModelFormListSumm;
		Glib::RefPtr< Gtk::ListStore > m_objSummModel;
		udWgtFormList m_inpSumm;
		Gtk::Button	*m_btnSummAdd,
					*m_btnSummBack,
					*m_btnSummConvert;

		void	OnFormListSumm_Add( );
		void	OnFormListSumm_Back( );
		void	OnFormListSumm_Convert( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListOperation : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListOperation( ) {
				add( m_col_created );
				add( m_col_type );
				add( m_col_money );
				add( m_col_object );
			} // ColModelFormListOperation

			Gtk::TreeModelColumn< Glib::ustring > m_col_created;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< Glib::ustring > m_col_money;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListOperation

	protected:
		ColModelFormListOperation* m_pModelFormListOperation;
		Glib::RefPtr< Gtk::ListStore > m_objOperationModel;
		udWgtFormList m_inpOperation;
		/*Gtk::Button	*m_btnOperationAdd,
					*m_btnOperationDel,
					*m_btnOperationEdit;

		void	OnFormListOperation_Select( );
		void	OnFormListOperation_Add( );
		void	OnFormListOperation_Edit( );
		void	OnFormListOperation_Del( );*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHPersAccForm

	class CHPersAccFormModal : public CHPersAccForm {

	public:
		CHPersAccFormModal( );
		CHPersAccFormModal( const CHPersAccFormModal& objCopy );

		virtual ~CHPersAccFormModal( );

		CHPersAccFormModal& operator =( const CHPersAccFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPersAccFormModal

	class CHPersAccFormModeless : public CHPersAccForm {

	public:
		CHPersAccFormModeless( );
		CHPersAccFormModeless( const CHPersAccFormModeless& objCopy );

		virtual ~CHPersAccFormModeless( );

		CHPersAccFormModeless& operator =( const CHPersAccFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPersAccFormModal

} // namespace UdSDK

#endif
