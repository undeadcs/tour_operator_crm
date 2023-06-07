
#ifndef PROJECTCOUNTRY_H
#define PROJECTCOUNTRY_H

namespace UdSDK {

	class CHCountry : public CStateHandlerSimpleOneName {

	public:
		CHCountry( );
		CHCountry( const CHCountry& objCopy );

		virtual ~CHCountry( );

		CHCountry& operator =( const CHCountry& objCopy );

	}; // class CHCountry

} // namespace UdSDK

#endif
