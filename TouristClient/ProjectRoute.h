
#ifndef PROJECTROUTE_H
#define PROJECTROUTE_H

namespace UdSDK {

	class CHRoute : public udStateHandlerSimpleOneName {

	public:
		CHRoute( );
		CHRoute( const CHRoute& objCopy );

		virtual ~CHRoute( );

		CHRoute& operator =( const CHRoute& objCopy );

	}; // class CHRoute



	class CHOrderRoute : public udFormHandler {
	public:
		enum {
			ctrlidName,
		};

	public:
		CHOrderRoute( );

		virtual ~CHOrderRoute( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectOrderRoute; }

	}; // class CHOrderRoute

} // namespace UdSDK

#endif
