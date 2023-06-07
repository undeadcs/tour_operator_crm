
#ifndef PROJECTCLIENT_H
#define PROJECTCLIENT_H

namespace UdSDK {

	void ClientAfterLoadClDoc( udArrBase& arr );

	class CHClient : public udStateHandlerSimple {
	public:
		enum {
			ctrlidFio,
			ctrlidFiolatin,
			ctrlidBorn,
			ctrlidSex,
			ctrlidDoc,
			ctrlidApp,
			ctrlidEmail,
			ctrlidAddr,
			ctrlidPhone,
		};
		
		enum {
			fltFio,
			fltDate1,
			fltDate2,

			LIST_CONTROL_ID_LAST = fltDate2
		};

		enum {
			ctrlidUrName,
			ctrlidUrDfio,
			ctrlidUrCfio,
			ctrlidUrEmail,
			ctrlidUrPhone,
			ctrlidUrFax,
			ctrlidUrAddr,
			ctrlidUrComment
		};

		enum {
			fltUrName,
			fltUrCfio,

			LIST_CONTROL_ID_LAST_UR = fltUrCfio
		};

		enum {
			stateFiz,	// физ лица
			stateUr,	// юр лица
		};

		enum {
			opidGet2 = opidGet + 1,	// получение списка клиентов, по заданным ID
			opidGet3,				// получение клиентов по ID (без фильтров и пейджера)
		};

	private:
		udInt		m_iClState;		// какого типа клиентов показывать
		udPWndLink	m_lnkPrintFiz,	// ссылка распечатать (физ.лицо)
					m_lnkPrintUr;	// ссылка распечатать (юр.лицо)

	public:
		CHClient( );
		CHClient( const CHClient& objCopy );

		virtual ~CHClient( );

		CHClient& operator =( const CHClient& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual void	RequestList( );
		virtual void	ClearFilter( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	OnLinkClick( udPWndLink lnk );
		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );

		virtual udInt	ListFizModel( );
		virtual udInt	ListFizView( );
		virtual udInt	FormFizModel( );
		virtual udInt	FormFizView( );
		virtual udInt	FormFizValidate( );
		virtual udInt	ListUrModel( );
		virtual udInt	ListUrView( );
		virtual udInt	FormUrModel( );
		virtual udInt	FormUrView( );
		virtual udInt	FormUrValidate( );

	}; // class CHClient

} // namespace UdSDK

#endif
