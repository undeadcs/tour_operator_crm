
#ifndef PROJECTPRICETABLEITEM_H
#define PROJECTPRICETABLEITEM_H

namespace UdSDK {

	class CHServicePrice : public udFormHandler {
		enum {
			ctrlidPeriod,
			ctrlidNum,
			ctrlidAmount,
			ctrlidCurrency,
		};

	public:
		CHServicePrice( );

		virtual ~CHServicePrice( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual void	LoadAfterSetObject( );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectPriceTableItem; }

	}; // class CHServicePrice

} // namespace UdSDK

#endif
