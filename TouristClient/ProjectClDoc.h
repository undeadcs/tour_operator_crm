
#ifndef PROJECTCLDOC_H
#define PROJECTCLDOC_H

namespace UdSDK {

	class CHClDoc : public udStateHandlerInfo {
	public:
		enum {
			ctrlidClient,
			ctrlidType,
			ctrlidSeries,
			ctrlidNumber,
			ctrlidIssuedate,
			ctrlidIssuedby,
			ctrlidBirthplace,
			ctrlidEnddate,
			ctrlidFio,
			ctrlidFiolatin,
		};

		enum {
			fltClient,
			fltType,
			fltSeries,
			fltNumber,

			LIST_CONTROL_ID_LAST = fltNumber
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHClDoc( );
		CHClDoc( const CHClDoc& objCopy );

		virtual ~CHClDoc( );

		CHClDoc& operator =( const CHClDoc& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual udInt	ProcForm( bool modal = false );
		virtual void	LoadAfterSetObject( );
		virtual udInt	CreateList( HWND hParent, bool modal = false );

	}; // class CHClDoc

} // namespace UdSDK

#endif
