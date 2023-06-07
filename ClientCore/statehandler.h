/**
	Tourist application
	Клиентское приложение

	обработчики состояния

	CStateHandlerSimple:
	в функция есть параметр modal - он означает, что текущий обработчик привязан к окну, которое
	является диалоговым, если modal = true, иначе это окно привязано к глобальному окну модели WndModel
*/
#ifndef STATEHANDLER_H
#define STATEHANDLER_H

namespace UdSDK {

	udInt	ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject );
	udInt	ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject, udPBase tpl );
	udInt	ReadInstances( udPSocket sock, udPArrBase arr, pfnGetObject fnGetObject );
	udInt	ReadInstances( udPSocket sock, udPArrBase arr, pfnGetObject fnGetObject, udPBase tpl );
	udInt	WriteInstances( udArrBase& arr, udBinaryData& data );
	udInt	WriteInstances( udPDwAscBase items, udBinaryData& data, pfnGetObject fnGetObject );
	void	PackFormListItems( udBinaryData& data, udPWndFormList frmlst, pfnGetObject fnGetObject );
	udPSocket	BeginRequest( bool login = true );
	udPBase	LoadById( const string& handler_name, udUInt id, pfnGetObject fnGetObject );

	void	LoadListDataByAttr(
				udArrBase& arr,
				udPHandler handler, pfnGetObject fnGetObject, // обработчик, калбэк создания объекта
				string table, string attr, // таблица сущностей, атрибут сущности из списка
				string attrfrom, string attrto // из атрибута выбранной сущности, в атрибут сущности списка
			);

	/**
	 *	Обработчик состояния
	 */
	class CStateHandler {

	public:
		CStateHandler( );
		CStateHandler( const CStateHandler& objCopy );

		virtual ~CStateHandler( );

		CStateHandler& operator =( const CStateHandler& objCopy );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	Show( udPWndModel pWndModel ) = 0;
		virtual void	Init( ) = 0;
		virtual void	Clear( ) = 0;
		virtual void	Load( ) = 0;
		virtual void	ClickToolbar( string index ) = 0;
		virtual udInt	ToolbarAction( string index ) = 0;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	LinkClick( udPWndLink lnk ) { }
		virtual void	ButtonClick( udPWndButton btn ) { }
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}; // class CStateHandler

	class CStateHandlerSimple : public udStateHandler, public udFormHandler, public udListHandler {
	public:
		typedef enum intState {
			INTSTATE_LIST,	// список
			INTSTATE_FORM	// форма
		} intState;

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
		string			m_szTableName,	// имя таблицы хранилища
						m_szUpdIndex,	// индекс обновления сущности
						m_szName;		// имя обработчика
		pfnGetObject	m_fnGetObject;	// калбэк создания объекта
		pfnGetFormHandler	m_fnGetFormHandler;	// калбэк создания текущего обработчика (для модальных форм)
		intState		m_iState;		// внутреннее состояние
		
	public:
		CStateHandlerSimple( );

		virtual ~CStateHandlerSimple( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void	SetInternalState( intState state ) { m_iState = state; }
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	PackForAdd( udBinaryData& data, udPBase obj );
		virtual void	PackForUpd( udBinaryData& data, udPBase obj );
		virtual void	PackForDel( udBinaryData& data, udPBase obj );
		virtual void	PackForEdt( udBinaryData& data, udPBase obj );
		virtual void	LoadExtraData( udPSocket sock, udPBase obj, udPWndModeForm wndForm );
		virtual void	ReadForEdit( udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );
		virtual void	RequestList( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	Show( udPWndModel wnd );
		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual void	ClickToolbar( string index );
		virtual udInt	ToolbarAction( string index );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	SwitchToForm( );
		virtual void	SwitchToList( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual udInt	ListOk( );
		virtual udInt	ListCancel( );
		virtual void	OnPagerSelect( udUInt page );
		virtual void	OnButtonClick( udPWndButton btn ) { }
		virtual void	OnHeaderClick( const string& name, const string& attr );
		virtual void	OnSelectRow( udLong row );
		virtual void	OnLinkClick( udPWndLink lnk );
		virtual void	LoadList( );
		virtual void	OnFilter( );
		virtual void	OnFilterReset( );
		virtual void	AfterLoadFilterData( ) { }
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual pfnGetObject	GetObjectCallback( ) { return m_fnGetObject; }
		virtual string			GetHandlerName( ) const { return m_szName; }
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}; // class CStateHandlerSimple

} // namespace UdSDK

#endif
