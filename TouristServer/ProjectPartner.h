
#ifndef PROJECTPARTNER_H
#define PROJECTPARTNER_H

namespace UdSDK {

	class CHPartner : public udStateHandlerSimple {

	public:
		CHPartner( );
		CHPartner( const CHPartner& objCopy );

		virtual ~CHPartner( );

		CHPartner& operator =( const CHPartner& objCopy );

		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual string	GetWhereCondition( );

		void	DeleteLinkedObjects( udPBase obj );
		void	CreateAccount( udPHandler handler, udUInt id );

	}; // class CHPartner

} // namespace UdSDK

#endif
