/*
	Tourist application
	Клиентское приложение
	Финансы
*/
#ifndef TOURIST_CLIENT_FINANCE_H
#define TOURIST_CLIENT_FINANCE_H

namespace UdSDK {

	/**
	 * Сумма на зачисление
	 */
	class CHFinanceSummAddForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblSumm,
						m_lblCurrency,
						m_lblType,
						m_lblHow,
						m_lblComment;
		udWgtInput		m_inpSumm,
						m_inpComment;
		udWgtListPick	m_inpCurrency;
		udWgtSelect		m_inpType,
						m_inpHow;
		Gtk::Table		m_tblInput;

		udUInt			m_iType,
						m_iHow;

	public:
		enum {
			ctrlidCurrency
		};

	public:
		CHFinanceSummAddForm( );
		CHFinanceSummAddForm( const CHFinanceSummAddForm& objCopy );
		CHFinanceSummAddForm( udUInt iType, udUInt iHow );

		virtual ~CHFinanceSummAddForm( );

		CHFinanceSummAddForm& operator =( const CHFinanceSummAddForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHFinanceSummAddForm

	class CHFinanceSummAddFormModal : public CHFinanceSummAddForm {

	public:
		CHFinanceSummAddFormModal( );
		CHFinanceSummAddFormModal( const CHFinanceSummAddFormModal& objCopy );
		CHFinanceSummAddFormModal( udUInt iType, udUInt iHow );

		virtual ~CHFinanceSummAddFormModal( );

		CHFinanceSummAddFormModal& operator =( const CHFinanceSummAddFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHFinanceSummAddFormModal

	/**
	 * Сумма на снятие
	 */
	class CHFinanceSummBackForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblSumm,
						m_lblCurrency,
						m_lblType,
						m_lblHow,
						m_lblComment;
		udWgtInput		m_inpSumm,
						m_inpComment;
		udWgtListPick	m_inpCurrency;
		udWgtSelect		m_inpType,
						m_inpHow;
		Gtk::Table		m_tblInput;

		udUInt			m_iType,
						m_iHow;

	public:
		enum {
			ctrlidCurrency
		};

	public:
		CHFinanceSummBackForm( );
		CHFinanceSummBackForm( const CHFinanceSummBackForm& objCopy );
		CHFinanceSummBackForm( udUInt iType, udUInt iHow );

		virtual ~CHFinanceSummBackForm( );

		CHFinanceSummBackForm& operator =( const CHFinanceSummBackForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHFinanceSummBackForm

	class CHFinanceSummBackFormModal : public CHFinanceSummBackForm {

	public:
		CHFinanceSummBackFormModal( );
		CHFinanceSummBackFormModal( const CHFinanceSummBackFormModal& objCopy );
		CHFinanceSummBackFormModal( udUInt iType, udUInt iHow );

		virtual ~CHFinanceSummBackFormModal( );

		CHFinanceSummBackFormModal& operator =( const CHFinanceSummBackFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHFinanceSummBackFormModal

	/**
	 * Сумма на конвертацию
	 */
	class CHFinanceSummConvertForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblSumm,
						m_lblCurrency1,
						m_lblCurrency2,
						m_lblType1,
						m_lblType2,
						m_lblComment;
		udWgtInput		m_inpSumm,
						m_inpComment;
		udWgtListPick	m_inpCurrency1,
						m_inpCurrency2;
		udWgtSelect		m_inpType1,
						m_inpType2;
		Gtk::Table		m_tblInput;

		udUInt			m_iType;

	public:
		enum {
			ctrlidCurrency1,
			ctrlidCurrency2
		};

	public:
		CHFinanceSummConvertForm( );
		CHFinanceSummConvertForm( const CHFinanceSummConvertForm& objCopy );
		CHFinanceSummConvertForm( udUInt iType );

		virtual ~CHFinanceSummConvertForm( );

		CHFinanceSummConvertForm& operator =( const CHFinanceSummConvertForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHFinanceSummConvertForm

	class CHFinanceSummConvertFormModal : public CHFinanceSummConvertForm {

	public:
		CHFinanceSummConvertFormModal( );
		CHFinanceSummConvertFormModal( const CHFinanceSummConvertFormModal& objCopy );
		CHFinanceSummConvertFormModal( udUInt iType );

		virtual ~CHFinanceSummConvertFormModal( );

		CHFinanceSummConvertFormModal& operator =( const CHFinanceSummConvertFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHFinanceSummConvertFormModal

} // namespace UdSDK

#endif
