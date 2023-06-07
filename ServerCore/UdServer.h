/**
	Сервер
*/
#ifndef UNDEADCS_SDK_SERVER_H
#define UNDEADCS_SDK_SERVER_H

namespace UdSDK {

	class CUdServer : public udMorph {
		enum {
			select_delay = 1000
		};

		udPSocket	m_pSocket;

	public:
		CUdServer( );
		CUdServer( const CUdServer& objCopy );

		virtual ~CUdServer( );

		CUdServer& operator =( const CUdServer& objCopy );

		bool	Init( );
		udDWord	Start( );

	private:
		void	HandleConnection( udPSocket pSocket );

	};

} // namespace UdSDK

#endif
