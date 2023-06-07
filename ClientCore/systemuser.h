
#ifndef SYSTEMUSER_H
#define SYSTEMUSER_H

namespace UdSDK {

	/**
	 *	Пользователь системы
	 */
	class CSystemUser : public udMorph {

	public:
		CSystemUser( );
		CSystemUser( const CSystemUser& objCopy );

		virtual ~CSystemUser( );

		CSystemUser& operator =( const CSystemUser& objCopy );

	}; // class CSystemUser

} // namespace UdSDK

#endif
