/*
	Tourist application
	Библиотека ядра клиентского приложения
	Дополнительные данные формы
*/
#ifndef TOURIST_CLIENT_CORE_FORM_DATA_EXTRA_H
#define TOURIST_CLIENT_CORE_FORM_DATA_EXTRA_H

namespace UdSDK {

	class CFormDataExtra {
	public:
		typedef enum FormDataExtraType {
			EXTRA_DATA_OBJECT,
			EXTRA_DATA_LIST,
			EXTRA_DATA_FILE
		} FormDataExtraType;

	protected:
		FormDataExtraType	m_iType;
		udInt				m_iMark;

	public:
		CFormDataExtra( );
		CFormDataExtra( const CFormDataExtra& objCopy );

		virtual ~CFormDataExtra( );

		CFormDataExtra& operator =( const CFormDataExtra& objCopy );

		virtual void	Load( udPSocket sock ) = 0;
		virtual void	Clear( ) = 0;
		virtual void	Send( udPSocket sock ) = 0;
		virtual void	Make( ) = 0;

		void	SetMark( udInt iMark ) { m_iMark = iMark; }

		udInt				GetMark( ) const { return m_iMark; }
		FormDataExtraType	GetType( ) const { return m_iType; }

	}; // class CFormDataExtra

	class CFDECallback : public CFormDataExtra {
	protected:
		pfnGetObject	m_fnGetObject;

	public:
		CFDECallback( );
		CFDECallback( const CFDECallback& objCopy );

		virtual ~CFDECallback( );

		CFDECallback& operator =( const CFDECallback& objCopy );

		void	SetCallback( pfnGetObject fnGetObject ) { m_fnGetObject = fnGetObject; }

	}; // class CFDECallback

	class CFDEObject : public CFDECallback {
	protected:
		udPBase	m_pObject;

	public:
		CFDEObject( );
		CFDEObject( const CFDEObject& objCopy );

		virtual ~CFDEObject( );

		CFDEObject& operator =( const CFDEObject& objCopy );

		virtual void	Load( udPSocket sock );
		virtual void	Clear( );
		virtual void	Send( udPSocket sock );
		virtual void	Make( );
		virtual void	Make( bool bDelete );

		udPBase	GetObject( ) const { return m_pObject; }

	}; // class CFDEObject

	class CFDEList : public CFDECallback {
	protected:
		udPArrBase		m_arrObject;
		pfnGetObject	m_fnGetObjectSend;

	public:
		CFDEList( );
		CFDEList( const CFDEList& objCopy );

		virtual ~CFDEList( );

		CFDEList& operator =( const CFDEList& objCopy );

		virtual void	Load( udPSocket sock );
		virtual void	Clear( );
		virtual void	Send( udPSocket sock );
		virtual void	Make( );

		void	SetCallbackSend( pfnGetObject fn ) { m_fnGetObjectSend = fn; }

		udPArrBase	GetArray( ) const { return m_arrObject; }

	}; // class CFDEList

	class CFDEFile : public CFormDataExtra {
	protected:
		wstring	m_szFolder,
				m_szName;

	public:
		CFDEFile( );
		CFDEFile( const CFDEFile& objCopy );
		
		virtual ~CFDEFile( );

		CFDEFile& operator =( const CFDEFile& objCopy );

		virtual void	Load( udPSocket sock );
		virtual void	Clear( );
		virtual void	Send( udPSocket sock );
		virtual void	Make( );

		void	SetName( const wstring& szName )		{ m_szName = szName;		}
		void	SetFolder( const wstring& szFolder )	{ m_szFolder = szFolder;	}

		wstring	GetFolder( )	const { return m_szFolder;	}
		wstring	GetName( )		const { return m_szName;	}

	}; // class CFDEFile

} // namespace UdSDK

#endif
