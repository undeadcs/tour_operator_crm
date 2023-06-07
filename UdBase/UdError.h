
#ifndef ERROR_H
#define ERROR_H

namespace UdSDK {

	/**
	 *	Класс информации об ошибке
	 */
	class CError {
		udLong	m_iCode;
		string	m_szText;

	public:
		CError( );
		CError( const CError& objErr );
		CError( udLong iCode );
		CError( string szText );
		CError( udLong iCode, const string& szText );

		virtual ~CError( );

		udLong	GetCode( )	const { return m_iCode;		}
		string	GetText( )	const { return m_szText;	}

		void	SetCode( udLong iCode )		{ m_iCode	= iCode;	}
		void	SetText( string szText )	{ m_szText	= szText;	}

	}; // class CError

	void	WriteErrors( udPLog log, const udArrError& err );

} // namespace UdSDK

#endif
