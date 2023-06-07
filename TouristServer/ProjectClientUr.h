
#ifndef TOURIST_PROJECT_CLIENTUR_H
#define TOURIST_PROJECT_CLIENTUR_H

namespace UdSDK {

	class CHClientUr : public udStateHandlerSimple {

	public:
		CHClientUr( );
		CHClientUr( const CHClientUr& objCopy );

		virtual ~CHClientUr( );

		CHClientUr& operator =( const CHClientUr& objCopy );

		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual string	GetWhereCondition( );

		void	DeleteLinkedObjects( udPBase obj );
		void	CreateAccount( udPHandler handler, udUInt id );

	}; // class CHClientUr

} // namespace UdSDK

#endif