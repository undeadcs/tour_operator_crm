
#include "includes.h"

namespace UdSDK {

	CUdMorph::CUdMorph( ) {
	}

	CUdMorph::CUdMorph( const CUdMorph& objCopy ) {
	}

	CUdMorph::~CUdMorph( ) {
		for( udAscType::iterator i = m_mapAttr.begin( ); i != m_mapAttr.end( ); ++i ) {
			delete i->second;
		}
		m_mapAttr.clear( );
		m_lstAttr.clear( );
	}

	CUdMorph& CUdMorph::operator =( const CUdMorph& objCopy ) {
		objCopy.apply( this );
		return *this;
	}

	udPType CUdMorph::get( string szAttr ) {
		if ( !this->isset( szAttr ) ) { // если атрибута нет, то создадим его
			udPType pAttr = new udType;
			m_lstAttr.push_back( udPairType( szAttr, pAttr ) );
			m_mapAttr[ szAttr ] = pAttr;
			return pAttr;
		}
		return m_mapAttr[ szAttr ];
	} // udPType get

	void CUdMorph::set( string szAttr, udPType pValue ) {
		if ( this->isset( szAttr ) ) { // если атрибут существует, то надо снести его значение
			udPType pTmp = m_mapAttr[ szAttr ];
			delete pTmp;
			
			for( udLstPairType::iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
				if ( szAttr == i->first ) {
					i->second = pValue;
					break;
				}
			}
			
		} else { // если атрибута нет, то добавляем его
			m_lstAttr.push_back( udPairType( szAttr, pValue ) );
		}
		m_mapAttr[ szAttr ] = pValue;
	} // void set

	void CUdMorph::unset( string szAttr ) {
		if ( this->isset( szAttr ) ) {
			udPType pType = m_mapAttr[ szAttr ];
			delete pType;
			m_mapAttr.erase( szAttr );

			for( udLstPairType::iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
				if ( szAttr == i->first ) {
					m_lstAttr.erase( i );
					break;
				}
			}
		}
	} // void unset

	bool CUdMorph::isset( string szAttr ) {
		return ( m_mapAttr.find( szAttr ) != m_mapAttr.end( ) );
	} // bool isset

	void CUdMorph::reset( ) {
		for( udLstPairType::iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			i->second->reset( );
		}
	} // void reset

	void CUdMorph::dump( string& szOut ) const {
		szOut += string( "Morph[\r\n" );

		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			szOut += string( "\t" ) + i->first + string( " => " );
			i->second->dump( szOut );
			szOut += "\r\n";
		}

		szOut += string( "]" );
	} // void dump

	void CUdMorph::dump( wstring& szOut ) const {
		szOut += L"Morph[\r\n";

		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			szOut += L"\t" + toWString( i->first ) + L" => ";
			i->second->dump( szOut );
			szOut += L"\r\n";
		}

		szOut += L"]";
	} // void dump

	void CUdMorph::dump( udBinaryData& data ) const {
		//WriteUdLibLog( "-- CUdMorph::dump( udBinaryData& data ) --\n" );
		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			//WriteUdLibLog( i->first + " '" + i->second->toString( ) + "'\n" );
			i->second->dump( data );
		}
		//WriteUdLibLog( "-- exit --\n" );
	} // void dump

	void CUdMorph::dump( udPBinaryData data ) const {
		//WriteUdLibLog( "-- CUdMorph::dump( udPBinaryData data ) --\n" );
		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			//WriteUdLibLog( i->first + " '" + i->second->toString( ) + "'\n" );
			i->second->dump( data );
		}
		//WriteUdLibLog( "-- exit --\n" );
	} // void dump

	udPMorph CUdMorph::clone( ) const {
		udPMorph ret = new udMorph;
		this->apply( ret );
		return ret;
	} // udPMorph clone

	udInt CUdMorph::clone( const string& attr, const string& attrto ) {
		if ( this->isset( attr ) ) {
			udInt ret = ( this->isset( attrto ) ? -1 : 1 );
			udPType type = m_mapAttr[ attr ];
			*( this->get( attrto ) ) = *type;
			return ret;
		}
		return 0;
	} // udInt clone

	void CUdMorph::apply( udPMorph obj ) const {
		string name = "";
		udPType type = NULL;
		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			name = i->first;
			type = i->second;
			*( obj->get( name ) ) = *type;
		}
	} // void apply

	void CUdMorph::apply( udPMorph obj, bool lvalue, bool strict ) {
/*
	lvalue = true, присваиваем не атрибутам obj, а атрибутам this
	strict = true, приводим жестко к типу данных слева (при обычном присвоении может смениться тип)
*/
		string name = "";
		udPType ltype = NULL, rtype = NULL;
		udPMorph lobj = obj, robj = this;
		if ( lvalue ) {
			lobj = this;
			robj = obj;
		}
		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			name = i->first;
			ltype = lobj->get( name );
			rtype = robj->get( name );
			ltype->assign( rtype, strict );
		}
	} // void apply

	udInt CUdMorph::read( udPSocket sock ) {
		//WriteUdLibLog( "CUdMorph::read( udPSocket sock )\n" );
		/*
			считываем данные из сокета, при этом в итоге проверим все данные
			то есть все байты объекта, а вдруг туда часть записали, это чтобы
			не инициализировать часть объекта, когда данные потеряны
		*/
		udInt ret = 0, res = 0;
		udPMorph obj = new udMorph;
		udPType type = NULL;
		bool failed = false;
		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			type = i->second;
			res = type->read( sock );
			if ( res == 0 ) {
				failed = true;
				break;
			}
		}
		if ( !failed ) {
			obj->apply( this );
			ret = 1;
		}

		/*string log = "\r\nсчитанный объект:\r\n";
		obj->dump( log );
		log += "\r\n";
		WriteUdLibLog( log );*/

		delete obj;
		//WriteUdLibLog( "последний результат: " + toString( res ) + "\r\n" );
		//WriteUdLibLog( "CUdMorph::read( udPSocket sock ) exit\n" );
		return ret;
	} // udInt read

	udInt CUdMorph::read( udBinaryData& data ) {
		//WriteUdLibLog( "CUdMorph::read( udPSocket sock )\n" );
		/*
			используя итератор по бинарным данным инициализируем атрибуты
		*/
		udInt ret = 0, res = 0;
		udPMorph obj = new udMorph;
		udPType type = NULL;
		bool failed = false;
		udBinaryDataIterator itCur = data.begin( ), itEnd = data.end( );
		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			type = i->second;
			res = type->read( itCur, itEnd );
			if ( res == 0 ) {
				failed = true;
				break;
			}
			if ( itCur == itEnd ) {
				break;
			}
		}
		if ( !failed ) {
			obj->apply( this );
			ret = 1;
		}

		/*string log = "\r\nсчитанный объект:\r\n";
		obj->dump( log );
		log += "\r\n";
		WriteUdLibLog( log );*/

		delete obj;
		//WriteUdLibLog( "последний результат: " + toString( res ) + "\r\n" );
		//WriteUdLibLog( "CUdMorph::read( udPSocket sock ) exit\n" );
		return ret;
	} // udInt read

	udPXmlNode CUdMorph::toXml( udPXmlDoc doc ) {
		udPXmlNode node = doc->CreateNode( L"morph" ), tmp = NULL;
		for( udLstPairType::const_iterator i = m_lstAttr.begin( ); i != m_lstAttr.end( ); ++i ) {
			tmp = doc->CreateNode( toWString( i->first ) );
			if ( tmp ) {
				tmp->SetValue( L"<![CDATA[" + i->second->toWString( ) + L"]]>" );
				node->InsertChild( tmp );
				tmp = NULL;
			}
		}
		return node;
	} // udPXmlNode toXml

} // namespace UdSDK
