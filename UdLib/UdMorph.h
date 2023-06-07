/**
	UndeadCS SDK
	Базовая библиотека

	класс морфного объекта
*/
#ifndef UDNEADCS_SDK_LIBRARY_MORPH_H
#define UDNEADCS_SDK_LIBRARY_MORPH_H

namespace UdSDK {

	class CUdMorph {
	protected:
		udLstPairType	m_lstAttr;	// список атрибутов ( для сохранения последовательности )
		udAscType		m_mapAttr;	// ассоциативный массив атрибутов ( для быстрого доступа )

	public:
		CUdMorph( );
		CUdMorph( const CUdMorph& objCopy );

		virtual ~CUdMorph( );

		CUdMorph& operator =( const CUdMorph& objCopy );

		virtual udPType		get( string szAttr );
		virtual void		set( string szAttr, udPType pValue );
		virtual void		unset( string szAttr );
		virtual bool		isset( string szAttr );
		virtual void		reset( );
		virtual void		dump( string& szOut ) const;
		virtual void		dump( wstring& szOut ) const;
		virtual void		dump( udBinaryData& data ) const;
		virtual void		dump( udPBinaryData data ) const;
		virtual udPMorph	clone( ) const;
		virtual udInt		clone( const string& attr, const string& attrto );
		virtual void		apply( udPMorph obj ) const;
		virtual void		apply( udPMorph obj, bool lvalue, bool strict = true );
		virtual udInt		read( udPSocket sock );
		virtual udInt		read( udBinaryData& data );
		virtual udPXmlNode	toXml( udPXmlDoc doc );

		const udLstPairType*	get_attr_list( ) const { return &m_lstAttr; }
		udLong	atrr_num( ) const { return m_mapAttr.size( ); }

	}; // class CUdMorph

} // namespace UdSDK

#endif
