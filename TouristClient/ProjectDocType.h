
#ifndef PROJECTDOCTYPE_H
#define PROJECTDOCTYPE_H

namespace UdSDK {

	class CHDocType : public udStateHandlerSimpleOneName {
		enum {
			ctrlidName
		};

	public:
		CHDocType( );
		CHDocType( const CHDocType& objCopy );

		virtual ~CHDocType( );

		CHDocType& operator =( const CHDocType& objCopy );
/*
		virtual void	LinkClick( udPWndLink lnk );
		virtual bool	ValidateObj( udPBase obj );
		virtual void	CreateWindowModeList( udPWndModel wnd );
		virtual void	CreateWindowModeForm( udPWndModel wnd );
*/
	}; // class CHDocType

} // namespace UdSDK

#endif
