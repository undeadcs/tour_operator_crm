/**
	UndeadCS SDK
	Базовая библиотека

	xml - список атрибутов
*/
#ifndef UDNEADCS_SDK_LIBRARY_XML_ATTR_LIST_H
#define UDNEADCS_SDK_LIBRARY_XML_ATTR_LIST_H

namespace UdSDK {

	class CUdXmlAttrList {
		map< wstring, udPXmlAttr >	m_ascAttr;
		map< udDWord, udPXmlAttr >	m_dwaAttr;

	public:
		CUdXmlAttrList( );
		CUdXmlAttrList( const CUdXmlAttrList& objCopy );

		virtual ~CUdXmlAttrList( );

		CUdXmlAttrList& operator =( const CUdXmlAttrList& objCopy );

		// проверка на пустоту
		bool			empty( ) const;
		// очистка
		void			clear( );
		// добавление
		void			set( const wstring& name, const wstring& value );
		// получение по индексу
		udPXmlAttr		get( udDWord index ) const;
		// получение по имени
		udPXmlAttr		get( const wstring& name ) const;
		// удаление по индексу
		udInt			del( udDWord index );
		// удаление по имени
		udInt			del( const wstring& name );
		// выборка по паттерну
		udArrXmlAttr	match_name( const wstring& pattern );
		udArrXmlAttr	match_value( const wstring& pattern );

		void	toString( string& out ) const;
		void	toString( wstring& out ) const;

		friend class CUdXmlNode;

	private:
		// переписывает целочисленную индексацию
		void	reorder( );

	}; // class CUdXmlAttrList

} // namespace UdSDK

#endif
