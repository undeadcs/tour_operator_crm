/*
	Tourist application
	Библиотека ядра клиентского приложения
	Обработчик состояния
*/
#ifndef TOURIST_CLIENT_CORE_STATE_HANDLER_H
#define TOURIST_CLIENT_CORE_STATE_HANDLER_H

namespace UdSDK {

	class CStateHandler {
	public:
		enum {
			opidAdd,	// добавление
			opidUpd,	// обновление
			opidDel,	// удаление
			opidAdd1,	// добавление (center)
			opidUpd1,	// обновление (center)
			opidDel1,	// удаление (center)
			opidLst1,	// подгрузка списка (center)
			opidEdt,	// запрос на редактирование
			opidGet,	// получение объекта по его ID
		};

	protected:
		string	m_szName;

	public:
		CStateHandler( );
		CStateHandler( const CStateHandler& objCopy );

		virtual ~CStateHandler( );

		CStateHandler& operator =( const CStateHandler& objCopy );

		virtual void	Init( ) = 0;
		virtual void	Load( ) = 0;
		virtual void	Show( ) = 0;
		virtual void	Hide( ) = 0;
		virtual void	Clear( ) = 0;

		string	GetName( ) const { return m_szName;	}

	}; // class CStateHandler

} // namespace UdSDK

#endif
