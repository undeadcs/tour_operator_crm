
#ifndef PROJECTCOUNTRY_H
#define PROJECTCOUNTRY_H

namespace UdSDK {

	class CHCountry : public udStateHandlerSimpleOneName {

	public:
		CHCountry( );
		CHCountry( const CHCountry& objCopy );

		virtual ~CHCountry( );

		CHCountry& operator =( const CHCountry& objCopy );

	}; // class CHCountry

} // namespace UdSDK

#endif
