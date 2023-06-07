
#ifndef STATEHANDLER_H
#define STATEHANDLER_H

namespace UdSDK {

	class CStateHandler {

	public:
		CStateHandler( );
		CStateHandler( const CStateHandler& objCopy );

		virtual ~CStateHandler( );

		CStateHandler& operator =( const CStateHandler& objCopy );

		virtual void	Init( ) = 0;
		virtual void	InitSync( udPDatabaseSync sync ) = 0;
		virtual void	ProcRequest( udPSocket sock ) = 0;

	}; // class CStateHandler

	udInt	ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject );
	udInt	ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject, udPBase tpl );
	udInt	WriteInstances( udArrBase& arr, udBinaryData& data );
	udInt	WriteInstances( udArrMorph& arr, udBinaryData& data );
	udInt	WriteInstances( udDwAscBase& arr, udBinaryData& data );
	udPBase	LoadInstanceByForeignKey(
				udPHandler handler, udPBase obj,
				pfnGetObject fnGetObject, const string& table,
				const string& fkey, const string& key = "id"
			);
	void	LoadInstanceByForeignKey(
				udPHandler handler, udPBase obj,
				udArrBase& arr, pfnGetObject fnGetObject,
				const string& table, const string& fkey,
				const string& key, const string& order = "",
				const string& where1 = ""
			);
	void	LoadListDataByAttr(
				udArrBase& arr,
				udPHandler handler, pfnGetObject fnGetObject, // обработчик, калбэк создания объекта
				string table, string attr, // таблица сущностей, атрибут сущности из списка
				string attrfrom, string attrto // из атрибута выбранной сущности, в атрибут сущности списка
			);
	void	LoadListDataByAttr(
				udArrBase& arr,
				udPHandler handler, pfnGetObject fnGetObject,
				const string& table, const string& attr,
				udArrBase& attrs // из каких атрибутов в какие
			);

	class CStateHandlerSimple : public udStateHandler {
	protected:
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

		struct _managed_table {
			string szTableName, szUpdIndex;
			pfnGetObject fnGetObject;
		};
		typedef vector< _managed_table > _vct_mtables;

		enum edit_append_type {
			edit_append_foreignkey,
			edit_append_array,
		};

		// фильтрация перед добавлением в данные в ответ на запрос на редактирование
		typedef void	( *pfnFlt )( udArrBase& arr );
		// обработка доп сущностей при обработке запроса на обновление
		typedef void	( *pfnFltExtra )( udArrBase& arrAdd, udArrBase& arrUpd, udArrBase& arrDel, udPBase obj );
		// особая обработка подгрузки по foreign key
		typedef udPBase	( *pfnSpecAppend )( udBinaryData& data, udPBase obj );

		struct _edit_append {
			udInt type;
			string table, fkey, key, order, where1;
			pfnGetObject fnGetObject;
			pfnFlt fnFilter;
			pfnFltExtra fnBeforeProc, fnAfterProc;
			pfnSpecAppend fnSpecAppend;
		};
		typedef vector< _edit_append > vctAppend;

		string			m_szTableName,	// имя таблицы хранилища
						m_szUpdIndex,	// индекс обновления сущности
						m_szName;		// имя обработчика
		pfnGetObject	m_fnGetObject;	// калбэк создания объекта
		_vct_mtables	m_arrAddTables;	// дополнительные таблицы менеджмента
		vctAppend		m_arrAppend;	// дополнительные данные под редактирование
		udPMorph		m_pObjFilter;	// объект информации о фильтрации

	public:
		CStateHandlerSimple( );

		virtual ~CStateHandlerSimple( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void	AddManagedTable( const string& name, const string& upd_index, pfnGetObject fnGetObject );
		void	AddAppendData(
					udInt type, const string& table, const string& fkey, const string& key,
					pfnGetObject fnGetObject, const string& order = "", const string& where1 = "",
					pfnFlt fnFilter = NULL, pfnFltExtra fnBeforeProc = NULL, pfnFltExtra fnAfterProc = NULL,
					pfnSpecAppend fnSpecAppend = NULL
				);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	SchemeCenter( udInt opId, udPSocket sock );
		virtual void	AfterLoadList( udArrBase& arr );
		virtual void	AppendDataEdt( udBinaryData& data, udPBase obj );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) { return false; }
		virtual void	HandleListRequest( udPSocket sock );
		virtual void	HandleAddRequest( udPSocket sock );
		virtual void	HandleUpdRequest( udPSocket sock );
		virtual void	HandleDelRequest( udPSocket sock );
		virtual void	HandleEdtRequest( udPSocket sock );
		virtual void	HandleGetRequest( udPSocket sock );
		virtual void	FilterEditData( udArrBase& arr );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual string	GetWhereCondition( ) { return ""; }
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );
		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual udInt	SyncAdd( udPSocket sock, udPBase obj );
		virtual udInt	SyncUpd( udPSocket sock, udPBase obj );
		virtual udInt	SyncDel( udPSocket sock, udPBase obj );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	UpdObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual udInt	AddObj( udPBase obj, const string& table, bool save_info = true );
		virtual udInt	UpdObj( udPBase obj, const string& table, const string& updattr, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, const string& table, bool save_info = true );
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}; // class CStateHandlerSimple

} // namespace UdSDK

#endif
