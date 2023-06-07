
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_WND_OPERATIONS_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_WND_OPERATIONS_H

namespace UdSDK {

	class CWndOperations : public udWndGraphic {
		typedef map< udDWord, udPWndStatic >	dwa_wrows;		// карта надписей

		dwa_wrows			m_mapRows;		// статики каждой строки
		udDwAscBase			m_ascRows;		// данные по каждой строке

	public:
		CWndOperations( );

		virtual ~CWndOperations( );

		void	SetRows( udArrBase& arr );
		void	ClearRows( );

		virtual void	Render( );

	}; // class CWndOperations

} // namespace UdSDK

#endif
