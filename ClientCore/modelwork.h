/**
	Main обработчик запускается на старте приложения
	При переключении обработчиков, сначала запускается очищение текущего
	потом новый обработчик загружает необходимые ему данные, потом запускается показ результатов
	Clear => Load => Show
	каждый обработчик ассоциируется со строковым именем состояния
	при переключении состояния меню происходит переключение обработчика
	некоторые индексы тулбара могут служить особыми переключателями состояния
	то есть сначала делается очистка текущего обработчика
	потом новый обработчик оповещается о том, что на него произошло переключение из тулбара
	а после запускается загрузка и показ
	Clear => SwitchedFromToolbar => Load => Show
	остальные клики по тулбару направляются текущему обработчику
*/
#ifndef MODELWORK_H
#define MODELWORK_H

namespace UdSDK {

	/**
	 *	Модель работы приложения
	 */
	class CModelWork {
		udAscStateHandler	m_arrHandler;
		udPStateHandler		m_pCurHandler,
							m_pMainHandler;
		udAscStr			m_mapTbState;		// соответствие индекса элемента тулбара индексу обработчика
		udPWndWork			m_pWndWork;
		string				m_szState;

	public:
		CModelWork( udPWndWork pWndWork );
		CModelWork( const CModelWork& objCopy );

		~CModelWork( );

		CModelWork& operator =( const CModelWork& objCopy );

		void	Show( );
		//void	Render( );
		void	Init( );
		void	InitSync( );
		void	SetState( string state );
		void	SetHandler( string state, udPStateHandler pHandler );
		udInt	ClickMenu( string index );
		udInt	ClickToolbar( string index );
		void	SwitchToMain( );
		void	SetToolbarSwitcher( string index, string state );

		void	SetMainHandler( udPStateHandler pHandler ) { m_pMainHandler = pHandler; }
		void	SetWndWork( udPWndWork pWndWork ) { m_pWndWork = pWndWork; }

		udPWndModel	GetWndModel( ) const;
		udPWndWork	GetWndWork( ) const { return m_pWndWork; }

		udPStateHandler	GetHandler( const string& state ) const;

	}; // class CModelWork

} // namespace UdSDK

#endif
