
#ifndef PROJECTCLIENT_H
#define PROJECTCLIENT_H

namespace UdSDK {

	class CHClient : public udStateHandlerSimple {
	public:
		enum {
			opidGet2 = opidGet + 1,	// получение списка клиентов, по заданным ID
			opidGet3,				// получение клиентов по ID (без фильтров и пейджера)
		};

	private:

	public:
		CHClient( );
		CHClient( const CHClient& objCopy );

		virtual ~CHClient( );

		CHClient& operator =( const CHClient& objCopy );

		virtual void	Init( );
		virtual void	SchemeCenter( udInt opId, udPSocket sock );
		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual string	GetWhereCondition( );

		void	DeleteLinkedObjects( udPBase obj );
		void	CreateAccount( udPHandler handler, udUInt id );

	}; // class CHClient

} // namespace UdSDK

#endif
