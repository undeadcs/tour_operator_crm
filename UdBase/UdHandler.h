
#ifndef OBJHANDLER_H
#define OBJHANDLER_H

namespace UdSDK {

	/**
	 *	Обработчик объектов
	 */
	class CUdHandler {
		udPDatabase	m_pDatabase;	// в дальнейшем сделать полиморфным udPDatabase

	public:
		CUdHandler( udPDatabase pDatabase = NULL );
		CUdHandler( const CUdHandler& objCopy );

		virtual ~CUdHandler( );

		CUdHandler& operator =( const CUdHandler& objCopy );

		void		CheckTable( const udHandlerOption& objOption, udPArrError arrError = NULL, udPArrString arrQuery = NULL );
		void		GetUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError = NULL, udPArrString arrQuery = NULL );
		void		AddUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError = NULL, udPArrString arrQuery = NULL );
		void		DelUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError = NULL, udPArrString arrQuery = NULL );
		void		UpdUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udPArrError arrError = NULL, udPArrString arrQuery = NULL );
		udLongLong	CountUdObject( const udHandlerOption& objOption );

		void		AddUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udArrString& arrQuery );
		void		DelUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udArrString& arrQuery );
		void		UpdUdObject( udArrBase& arrObject, const udHandlerOption& objOption, udArrString& arrQuery );

		void		AddUdObject( udPBase obj, const udHandlerOption& option, udPArrError arrError = NULL, udPArrString arrQuery = NULL );
		void		UpdUdObject( udPBase obj, const udHandlerOption& option, udPArrError arrError = NULL, udPArrString arrQuery = NULL );
		void		DelUdObject( udPBase obj, const udHandlerOption& option, udPArrError arrError = NULL, udPArrString arrQuery = NULL );

	}; // class CUdHandler

} // namespace UdSDK

#endif
