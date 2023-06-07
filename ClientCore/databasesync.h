/**
	Tourist application
	
	синхронизатор баз данных
*/
#ifndef TOURIST_APPLICATION_CORE_DATABASE_SYNC_H
#define TOURIST_APPLICATION_CORE_DATABASE_SYNC_H

namespace UdSDK {

	class CDatabaseSync {
	public:
		enum {
			syncInsert,
			syncUpdate,
			syncDelete,
			syncTables,	// синк таблиц
			syncBlock,	// синк блокировки объекта
		};

	private:
		typedef struct {
			int cnt;
			string name;
			string handler;
		} _table_info;
		typedef map< string, _table_info > _asc_tables;
		//typedef map< string, int > _asc_tables;	// таблицы, которые синкаются
		//typedef map< string, udPBase > _asc_info;	// индексация инфы о таблицах
		
		_asc_tables	m_arrTable;			// таблицы синхронизации
		udAscBase	m_arrInfo;			// индексированная информация
		udArrBase	m_arrTableInfo;		// информация о таблицах
		udArrBase	m_arrTableQuery;	// кэш запросов к базе

	public:
		CDatabaseSync( );

		~CDatabaseSync( );

		void	SendMetadata( udPSocket sock );
		void	UpdateVersion( udArrBase& metadata );
		void	UpdateVersion( const string& name );
		void	AddSyncTable( const string& name, const string& handler );
		void	Init( );
		void	Sync( );
		udInt	Query( const udArrBase& arr );

	private:
		udInt	LoadInfoData( );
		void	CheckTables( );

	}; // class CDatabaseSync




	class CDatabaseSyncInfo : public udBase {

	public:
		CDatabaseSyncInfo( );

		virtual ~CDatabaseSyncInfo( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CDatabaseSyncInfo

	udPBase GetDatabaseSyncInfo( );





	class CDatabaseTableQuery : public udBase {
	public:
		// тип операции
		enum {
			QueryTypeInsert,
			QueryTypeUpdate,
			QueryTypeDelete
		};
		// состояние операции
		enum {
			QueryStateNormal,	// обычная операция
			QueryStateDeleted,	// объект был удален (в базе где-то должна быть запись об удалении)
			QueryStateLinked,	// было обновление более старшей сущности вместе с данной
		};

	public:
		CDatabaseTableQuery( );

		virtual ~CDatabaseTableQuery( );

		virtual void	GetConfig( udBaseConfig& objConfig );


	}; // class CDatabaseTableQuery

	udPBase GetDatabaseTableQuery( );

} // namespace UdSDK

#endif
