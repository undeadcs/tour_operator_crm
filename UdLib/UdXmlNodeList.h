/**
	UndeadCS SDK
	Базовая библиотека

	xml - список узлов
*/
#ifndef UDNEADCS_SDK_LIBRARY_XML_NODE_LIST_H
#define UDNEADCS_SDK_LIBRARY_XML_NODE_LIST_H

namespace UdSDK {

	class CUdXmlNodeList {
		map< udDWord, udPXmlNode >	m_dwaNode;

	public:
		CUdXmlNodeList( );
		CUdXmlNodeList( const CUdXmlNodeList& objCopy );

		virtual ~CUdXmlNodeList( );

		CUdXmlNodeList& operator =( const CUdXmlNodeList& objCopy );

		// проверка на пустоту
		bool			empty( ) const;
		// очистка
		void			clear( );
		// вставка
		void			insert( udPXmlNode node, udInt at = InsAtAfter );
		void			insert( udPXmlNode pos, udPXmlNode node, udInt at = InsAtAfter );
		// удаление
		void			del( udDWord index );
		void			del( udPXmlNode node );
		// получение по индексу
		udPXmlNode		get( udDWord index ) const;
		// выборка по паттерну
		udXmlNodeList	get( const wstring& pattern ) const;

		void	toString( string& out ) const;
		void	toString( wstring& out ) const;

		friend class CUdXmlNode;

	private:
		// сдвигает элементы вправо и на индексное место создает нулевой элемент
		void	ShiftForNew( udDWord index );
		void	reorder( );

	}; // class CUdXmlNodeList

} // namespace UdSDK

#endif
