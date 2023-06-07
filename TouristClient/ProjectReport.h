/**
	Tourist application
	Клиентское приложение

	отчеты
*/
#ifndef TOURIST_APPLICATION_CLIENT_REPORTS_H
#define TOURIST_APPLICATION_CLIENT_REPORTS_H

namespace UdSDK {

	udPBase	GetProjectReportLanding2( );
	udPBase	GetProjectReportFilling2( );

	class CHReport : public udStateHandler, public udListHandler {
	public:
		enum {
			opidLanding,
			opidFilling,
		};

		enum {
			reportLanding,	// Посадочная ведомость
			reportFilling,	// Ведомость текущего наполнения групп
		};
		// landing - Посадочная ведомость
		enum {
			landingDate1,
			landingDate2,
			landingGroup,
			landingDirect,

			LAST_CONTROL_ID_LANDING
		};
		// filling - Ведомость текущего наполнения групп
		enum {
			fillingDate1,
			fillingDate2,
			fillingGroup,
			fillingDirect,

			LAST_CONTROL_ID_FILLING
		};

		enum {
			btnidFilter,	// кнопка фильтрации
			btnidExport,	// кнопка экспорта
		};

		typedef vector< udPWndButton > vct_btn;
		typedef map< udInt, map< udInt, udPWndButton > > mtx_btn;

	private:
		vct_btn			m_arrButton;	// кнопки
		mtx_btn			m_mtxButton;	// матрица указателей на кнопки
		udInt			m_iState2;		// внутреннее состояние
		string			m_szOrderBy;	// атрибут сортировки
		udInt			m_iOrderType;	// тип упорядочивания

	public:
		CHReport( );
		CHReport( const CHReport& objCopy );

		virtual ~CHReport( );

		CHReport& operator =( const CHReport& objCopy );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	Show( udPWndModel wnd );
		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual void	ClickToolbar( string index );
		virtual udInt	ToolbarAction( string index );
		virtual void	LinkClick( udPWndLink lnk );
		virtual void	ButtonClick( udPWndButton btn );

		virtual void	LoadList( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	ListOk( );
		virtual udInt	ListCancel( );
		virtual void	OnLinkClick( udPWndLink lnk );
		virtual void	OnSelectRow( udLong row );
		virtual void	OnHeaderClick( const string& name, const string& attr );
		virtual void	OnPagerSelect( udUInt page );
		virtual void	OnButtonClick( udPWndButton btn );
		virtual void	OnFilter( );
		virtual void	OnFilterReset( );
		virtual void	ClearFilter( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		udInt			ListLandingModel( );
		udInt			ListLandingView( );
		udInt			ListFillingModel( );
		udInt			ListFillingView( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		string			GetWhereCondition( );
		udInt			MakeExportLandingFile( );
		udInt			MakeExportFillingFile( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}; // class CHReport

} // namespace UdSDK

#endif
