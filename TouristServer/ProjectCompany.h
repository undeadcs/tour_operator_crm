
#ifndef PROJECTCOMPANY_H
#define PROJECTCOMPANY_H

namespace UdSDK {

	class CHCompany : public udStateHandlerSimple {

	public:
		CHCompany( );
		CHCompany( const CHCompany& objCopy );

		virtual ~CHCompany( );

		CHCompany& operator =( const CHCompany& objCopy );

		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual string	GetWhereCondition( );

		void	DeleteLinkedObjects( udPBase obj );
		void	CreateAccount( udPHandler handler, udUInt id );

	}; // class CHCompany

} // namespace UdSDK

#endif
