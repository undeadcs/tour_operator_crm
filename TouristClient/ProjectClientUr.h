
#ifndef TOURIST_PROJECT_CLIENTUR_H
#define TOURIST_PROJECT_CLIENTUR_H

namespace UdSDK {

	class CHClientUr : public udStateHandlerSimple {
		enum {
			ctrlidName,
			ctrlidDfio,
			ctrlidCfio,
			ctrlidEmail,
			ctrlidPhone,
			ctrlidFax,
			ctrlidAddr,
			ctrlidComment
		};

	public:
		CHClientUr( );
		CHClientUr( const CHClientUr& objCopy );

		virtual ~CHClientUr( );

		CHClientUr& operator =( const CHClientUr& objCopy );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		//virtual void	OnLinkClick( udPWndLink lnk );

	}; // class CHClientUr

} // namespace UdSDK

#endif