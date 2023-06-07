/**
	Контрол, выбор значения которого происходит из всплывающего окна со списком сущностей
	Пользователь кликает
	Появляется диалоговое окно (собственного производства), где отображается список
	и кнопки ОК и ОТМЕНА
	Если пользователь выбрал элемент из списка и нажал Ок, то объект копируется в значение списка
	И значение атрибута выводится как надпись
*/

#ifndef UDSDK_EXTLIST_H
#define UDSDK_EXTLIST_H

namespace UdSDK {

	/**
	 *	Выбор элемента из внешнего списка
	 */
	class CExtList : public udWndGraphic {
		bool		m_bReadOnly;		// только чтение
		pfnGetListHandler
					m_fnGetListHandler;	// функция создания временного обработчика
		pfnExtLstAfterPick
					m_fnAfterPick;		// функция для пост обработки выбора
		string		m_szAttr,			// имя атрибута, значение которого будет показываться
					m_szIndex;			// индекс контрола
		udPBase		m_pObject;			// объект, выбранный из списка
		HWND		m_hWndDlgParent;	// родительское окно для диалогового окна
		udPMorph	m_pOptHandler;		// опции обработчика

	public:
		CExtList( );
		CExtList( const CExtList& objCopy );

		virtual ~CExtList( );

		CExtList& operator =( const CExtList& objCopy );

		void	ClearSelection( );

		virtual void	Render( );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgSetCursor( WPARAM wParam, LPARAM lParam );

		void	SetObj( udPBase obj );
		void	SetDialogParent( HWND hWnd ) { m_hWndDlgParent = hWnd; }
		void	SetIndex( const string& index ) { m_szIndex = index; }
		void	SetAttr( const string& attr ) { m_szAttr = attr; }
		void	SetHandler( pfnGetListHandler fnGetListHandler ) { m_fnGetListHandler = fnGetListHandler; }
		void	SetOpt( udPMorph opt ) { m_pOptHandler = opt; }
		void	SetAfterPick( pfnExtLstAfterPick fnAfterPick ) { m_fnAfterPick = fnAfterPick; }
		void	SetReadOnly( bool bReadOnly ) { m_bReadOnly = bReadOnly; }

		udPBase		GetObj( )			const { return m_pObject;		}
		string		GetIndex( )			const { return m_szIndex;		}
		string		GetAttr( )			const { return m_szAttr;		}

	}; // class CExtList

} // namespace UdSDK

#endif
