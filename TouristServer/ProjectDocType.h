
#ifndef PROJECTDOCTYPE_H
#define PROJECTDOCTYPE_H

namespace UdSDK {

	class CHDocType : public udStateHandlerSimpleOneName {

	public:
		CHDocType( );
		CHDocType( const CHDocType& objCopy );

		virtual ~CHDocType( );

		CHDocType& operator =( const CHDocType& objCopy );

	}; // class CHDocType

} // namespace UdSDK

#endif
