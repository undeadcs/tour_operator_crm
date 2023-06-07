/**
	конфиг базового класса
	на первых парах будет оберткой над массивом, который использовался
*/

#ifndef UDBASECONFIG_H
#define UDBASECONFIG_H

namespace UdSDK {

	/**
	 *	Конфиг класса udBase
	 */
	class CUdBaseConfig {
	public:
		// настройки класса
		typedef enum cfgVal {
			UDSDK_CONFIG_CPP,			// режим исходного кода
			UDSDK_CONFIG_DATABASE,		// режим базы данных
			UDSDK_CONFIG_XML,			// режим XML
			UDSDK_CONFIG_FORM,			// режим формы
			UDSDK_CONFIG_HTML,			// режим html
			UDSDK_CONFIG_TYPE,			// тип атрибута
			UDSDK_CONFIG_TABLE,			// имя таблицы хранилища
			UDSDK_CONFIG_NAME,			// имя (зависит от контекста настройки)
			UDSDK_CONFIG_CREATE,		// настройки для запроса CREATE TABLE
			UDSDK_CONFIG_SELECT,		// настройки для запроса SELECT
			UDSDK_CONFIG_DELETE,		// настройки для запроса DELETE
			UDSDK_CONFIG_INSERT,		// настройки для запроса INSERT
			UDSDK_CONFIG_UPDATE,		// настройки для запроса UPDATE
			UDSDK_CONFIG_PREFIX,		// префикс для имени атрибута
			UDSDK_CONFIG_TITLE,			// название атрибута
			UDSDK_CONFIG_DEFAULT,		// значение по умолчанию
			UDSDK_CONFIG_LENGTH,		// длина строки (для VARCHAR)
			UDSDK_CONFIG_DIGITS,		// количество цифр
			UDSDK_CONFIG_DECIMALS,		// точность вещественного числа
			UDSDK_CONFIG_XMLNODENAME,	// имя узла xml
			UDSDK_CONFIG_CALLBACK		// калбэк создания объекта
		} cfgVal;

		// тип атрибута
		typedef enum cfgType {
			UDSDK_TYPE_UNSIGNED			= 1 << 0,
			UDSDK_TYPE_INT				= 1 << 1,
			UDSDK_TYPE_FLOAT			= 1 << 2,
			UDSDK_TYPE_DOUBLE			= 1 << 3,
			UDSDK_TYPE_STRING			= 1 << 4,
			UDSDK_TYPE_TEXT				= 1 << 5,
			UDSDK_TYPE_DATE				= 1 << 6,
			UDSDK_TYPE_TIME				= 1 << 7,
			UDSDK_TYPE_NOTNULL			= 1 << 8,
			UDSDK_TYPE_AUTOINCREMENT	= 1 << 9,
			UDSDK_TYPE_PRIMARYKEY		= 1 << 10,
			UDSDK_TYPE_DEFAULT			= 1 << 11,
			UDSDK_TYPE_BLOB				= 1 << 12,
			UDSDK_TYPE_OBJECT			= 1 << 13,
			UDSDK_TYPE_CHILDREN_OBJECT	= 1 << 14,
			UDSDK_TYPE_UINT				= UDSDK_TYPE_INT | UDSDK_TYPE_UNSIGNED,
			UDSDK_TYPE_DATETIME			= UDSDK_TYPE_DATE | UDSDK_TYPE_TIME,
			UDSDK_TYPE_INTKEY			= UDSDK_TYPE_INT | UDSDK_TYPE_UNSIGNED | UDSDK_TYPE_NOTNULL | UDSDK_TYPE_AUTOINCREMENT | UDSDK_TYPE_PRIMARYKEY
		} cfgType;

	private:
		typedef map< cfgVal,	udPType	> cfgAttr;
		typedef map< string,	cfgAttr	> cfgArray;

		cfgAttr		m_mapCommon;	// общие настройки
		cfgArray	m_mapAttr;		// настройки атрибутов

	public:
		CUdBaseConfig( );
		CUdBaseConfig( const CUdBaseConfig& objCopy );

		virtual ~CUdBaseConfig( );

		CUdBaseConfig& operator =( const CUdBaseConfig& objCopy );

		void	Clear( );

		void	SetSQLTable( const string& table );
		void	SetPrefix( const string& prefix );
		void	SetSelectBy( const string& name );
		void	SetUpdateBy( const string& name );
		void	SetDeleteBy( const string& name );
		void	SetAttrType( const string& name, cfgType type );
		void	SetAttrDigits( const string& name, udUInt digits );
		void	SetAttrDecimals( const string& name, udUInt decimals );
		void	SetAttrLength( const string& name, udUInt length );
		void	SetAttrTitle( const string& name, string title );
		void	SetXmlNodeName( const wstring& name );
		void	SetAttrCallback( const string& name, pfnGetObject fnGetObject );

		udUInt	GetAttrType( const string& name ) const;
		string	GetAttrPrefix( const string& name, cfgVal mode = CUdBaseConfig::UDSDK_CONFIG_CPP ) const;
		string	GetAttrIndex( const string& name, cfgVal mode = CUdBaseConfig::UDSDK_CONFIG_CPP ) const;
		wstring	GetAttrTitle( const string& name, cfgVal mode = CUdBaseConfig::UDSDK_CONFIG_CPP ) const;
		string	GetSQLTableName( ) const;
		string	GetAttrSQLCreate( const string& name ) const;
		string	GetAttrSelect( ) const;	// атрибут выборки
		string	GetAttrUpdate( ) const;	// атрибут обновления
		string	GetAttrDelete( ) const;	// атрибут удаления
		wstring	GetXmlNodeName( ) const;	// имя xml узла

		bool	issetSelect( ) const;

	}; // class CUdBaseConfig

} // namespace UdSDK

#endif
