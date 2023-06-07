/*
	Tourist application
	Библиотека ядра клиентского приложения
	Виджет выпадающего списка
*/
#ifndef TOURIST_CLIENT_CORE_WGT_SELECT_H
#define TOURIST_CLIENT_CORE_WGT_SELECT_H

namespace UdSDK {

	class CWgtSelect : public Gtk::ComboBox {
		udUInt	m_iDefault,
				m_iValue;

		// модель колонок
		class ColModelSelect : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelSelect( ) {
				add( m_col_code );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_code;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

		ColModelSelect*	m_pColModelSelect;
		Glib::RefPtr< Gtk::ListStore >
							m_objSelectModel;

	public:
		CWgtSelect( );
		CWgtSelect( const CWgtSelect& objCopy );
		CWgtSelect( udUInt iValue, udUInt iDefaultValue );

		virtual ~CWgtSelect( );

		CWgtSelect& operator =( const CWgtSelect& objCopy );

		void	AddRow( udUInt iValue, const Glib::ustring& szTitle );
		void	Reset( );
		void	SetDefaultValue( udUInt iValue ) { m_iDefault = iValue; }
		void	SetValue( udUInt iValue );
		CWgtSelect&	LoadValue( );

		udUInt	GetValue( )			const { return m_iValue;	}
		udUInt	GetDefaultValue( )	const { return m_iDefault;	}

	}; // class CWgtSelect

} // namespace UdSDK

#endif
