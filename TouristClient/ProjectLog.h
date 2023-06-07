
#ifndef PROJECTLOG_H
#define PROJECTLOG_H

namespace UdSDK {

	class CHLog : public udStateHandlerSimple {
	public:
		enum {
			fltDate1,
			fltDate2,
			fltUser,
			fltText,

			LIST_CONTROL_ID_LAST = fltText
		};

	private:

	public:
		CHLog( );
		CHLog( const CHLog& objCopy );

		virtual ~CHLog( );

		CHLog& operator =( const CHLog& objCopy );

		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	RequestList( );

	}; // class CHLog

} // namespace UdSDK

#endif
