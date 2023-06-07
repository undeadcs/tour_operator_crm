/**
 *	Базовый класс используется для хранения данных объекта, автоматизации работы с базой данных,
 *	оконным приложением, генерацией данных для внешних хранилищь.
 *	Для того, чтобы базовый класс мог построить конфиг по типам атрибутов, необходимо
 *	в конструкторе потомка произвести наполнение списка. Например:
 *
 *	class X : public udBase
 *	...
 *	X( ) {
 *		this->get( "some_attr" )->ini( ( udInt ) 125 );	// будет создан атрибут с именем some_attr и с целочисленным значением
 *	}
 *
 */

#ifndef UDBASE_H
#define UDBASE_H

namespace UdSDK {

	/**
	 *	Базовый класс
	 */
	class CUdBase : public udMorph {
	public:
		// настройки класса
		typedef enum cfgVal {
			UDSDK_CONFIG_CPP,			// режим исходного кода
			UDSDK_CONFIG_DATABASE,		// режим базы данных
			UDSDK_CONFIG_XML,			// режим XML
			UDSDK_CONFIG_FORM,			// режим формы
			UDSDK_CONFIG_HTML			// режим html
		} cfgVal;

		// значения по умолчанию
		typedef enum defVal {
			UDSDK_DEFAULT_DIGITS	= 10,
			UDSDK_DEFAULT_DECIMAL	= 2,
			UDSDK_DEFAULT_LENGTH	= 255
		} defVal;

	public:
		CUdBase( );
		CUdBase( const CUdBase& objCopy );

		virtual ~CUdBase( );

		CUdBase& operator =( const CUdBase& objCopy );

		virtual void	Create( const udAscType& vctInput, cfgVal iMode = CUdBase::UDSDK_CONFIG_CPP );
		virtual void	Create( udPMorph obj, cfgVal iMode = CUdBase::UDSDK_CONFIG_CPP );
		virtual void	GetConfig( udBaseConfig& objConfig );
		virtual udPType	GetAttrValue( string name, const udBaseConfig& objConfig, cfgVal mode = CUdBase::UDSDK_CONFIG_CPP );
		virtual udPXmlNode	GetXml( udPXmlDoc doc, const udBaseConfig& config );

		// функции получения данных SQL запроса
		virtual udAscType	GetSQLCreate( );	// запрос CREATE
		virtual udAscType	GetSQLSelect( );	// запрос SELECT
		virtual udAscType	GetSQLInsert( );	// запрос INSERT
		virtual udAscType	GetSQLUpdate( );	// запрос UPDATE
		virtual udAscType	GetSQLDelete( );	// запрос DELETE

		// функции очистки возвращаемых массивов
		static void	ClearSQL( udAscType& arrData );

	protected:
		virtual udInt	InitAttr( const string& name, const udAscType& vctInput, const udBaseConfig& objConfig, cfgVal iMode = CUdBase::UDSDK_CONFIG_CPP );
		virtual udInt	InitAttr( const string& name, udPMorph obj, const udBaseConfig& objConfig, cfgVal iMode = CUdBase::UDSDK_CONFIG_CPP );
		virtual udPType	FilterAttr( const string& szName, const udBaseConfig& objConfig, cfgVal iMode = CUdBase::UDSDK_CONFIG_CPP );

	}; // class CBase

} // namespace UdSDK

#endif
