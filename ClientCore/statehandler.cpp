
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CStateHandler::CStateHandler( ) {
	}

	CStateHandler::CStateHandler( const CStateHandler& objCopy ) {
	}

	CStateHandler::~CStateHandler( ) {
	}

	CStateHandler& CStateHandler::operator =( const CStateHandler& objCopy ) {
		return *this;
	}




	CStateHandlerSimple::CStateHandlerSimple( ) {
		m_iState			= INTSTATE_LIST;
		m_fnGetFormHandler	= NULL;
		m_fnGetObject		= NULL;
	}

	CStateHandlerSimple::~CStateHandlerSimple( ) {
	}

	void CStateHandlerSimple::Show( udPWndModel wnd ) {
		udPLog __log = g_pApp->GetLog( );
		__log->Write( "CStateHandlerSimple::Show enter\n" );
		if ( m_iState == INTSTATE_LIST ) {
			if ( m_wndModeList ) {
				__log->Write( "CStateHandlerSimple::Show, m_wndModeList show\n" );
				m_wndModeList->Show( );
			}
		} else if ( m_iState == INTSTATE_FORM ) {
			if ( m_wndForm ) {
				__log->Write( "CStateHandlerSimple::Show, m_wndForm show\n" );
				m_wndForm->Show( );
			}
		} else {
			g_pApp->GetLog( )->Write( "Неизвестное состояние\n" );
		}
		__log->Write( "CStateHandlerSimple::Show exit\n" );
	} // void Show

	void CStateHandlerSimple::Init( ) {
		CreateList( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), false );
		CreateForm( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), false );
	} // void Init

	void CStateHandlerSimple::OnPagerSelect( udUInt page ) {
		if ( m_iState == INTSTATE_LIST ) {
			this->ClearList( );
			if ( !m_wndModeList->IsModal( ) ) {
				udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
				tbr->EnableItem( "edit",	false );
				tbr->EnableItem( "delete",	false );
			}
			m_pPager->SetPage( page );
			this->LoadList( );
			m_wndModeList->Show( );
		}
	} // void OnPagerSelect

	void CStateHandlerSimple::Clear( ) {
		m_iState = INTSTATE_LIST;
		this->ClearList( );
		if ( m_wndForm ) {
			m_wndForm->Hide( );
			m_wndForm->Clear( );
		}
		if ( !m_wndModeList->IsModal( ) ) {
			udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
			tbr->EnableItem( "edit",	false );
			tbr->EnableItem( "delete",	false );
			m_wndModeList->GetList( )->ClearRows( );
			m_wndModeList->GetPager( )->Clear( );
		}
	} // void Clear

	void CStateHandlerSimple::AddExtraAttributes( udPBase tpl ) {
	} // void AddExtraAttributes

	void CStateHandlerSimple::RequestList( ) {
		udPSocket sock = BeginRequest( );
		if ( sock ) {
			// ListRequest
			udBinaryData data;
			data.add( m_szName );
			data.add( ( udInt ) opidLst1 );
			data.add( m_pPager->GetPage( ) );
			data.add( m_pPager->GetPageSize( ) );
			//data.add( m_szOrder );
			data.add( m_szOrderBy );
			data.add( m_iOrderType );

			string log = "\nфильтр, ";
			m_wndModeList->GetObjFilter( )->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			m_wndModeList->GetObjFilter( )->dump( data );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udType total( ( udUInt ) 0 );
				total.read( sock );
				m_pPager->SetTotal( total.toUInt( ) );
				udPBase tpl = m_fnGetObject( );
				this->AddExtraAttributes( tpl );
				ReadInstances( sock, m_arrObject, m_fnGetObject, tpl );
				delete tpl;
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
	} // void RequestList

	udInt CStateHandlerSimple::ListOk( ) {
		assert( m_wndModeList );
		if ( m_wndModeList->IsModal( ) ) {
			::PostMessage( m_wndModeList->GetHWndParent( ), WM_COMMAND, MAKEWPARAM( IDOK, 0 ), ( LPARAM ) m_wndModeList );
		}
		return ListHandlerErrorOk;
	} // udInt ListOk

	udInt CStateHandlerSimple::ListCancel( ) {
		assert( m_wndModeList );
		if ( m_wndModeList->IsModal( ) ) {
			::PostMessage( m_wndModeList->GetHWndParent( ), WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), ( LPARAM ) m_wndModeList );
		}
		return ListHandlerErrorOk;
	} // udInt ListCancel

	void CStateHandlerSimple::LoadList( ) {
		this->RequestList( );
		udPWndInstanceList wndInslst = m_wndModeList->GetList( );
		assert( wndInslst );
		wndInslst->GenerateRows( &m_arrObject );
		udPWndPager wndPager = m_wndModeList->GetPager( );
		assert( wndPager );
		wndPager->Generate( m_pPager );
		udInt y = wndInslst->get( "y" )->toInt( ) + wndInslst->get( "height" )->toInt( ) + 5;
		wndPager->get( "y" )->ini( y );
		wndPager->Move( );
		// модальный режим
		if ( m_wndModeList->IsModal( ) ) {
			udInt x = 32, m = 5;
			HWND hWndDialog = m_wndModeList->GetHWndParent( );
			udPWndButton btn = m_wndModeList->GetButtonOk( );
			y += wndPager->get( "height" )->toInt( ) + m;
			btn->get( "x" )->ini( x );
			btn->get( "y" )->ini( y );
			btn->Move( );
			x += btn->get( "width" )->toInt( ) + m;
			btn = m_wndModeList->GetButtonCancel( );
			btn->get( "x" )->ini( x );
			btn->get( "y" )->ini( y );
			btn->Move( );
			// перерисовываем основное окно
			udInt	prev_w	= m_wndModeList->get( "width" )->toInt( ),
					prev_h	= m_wndModeList->get( "height" )->toInt( ),
					w		= wndInslst->get( "width" )->toInt( ) + m * 2,
					h		= btn->get( "y" )->toInt( ) + btn->get( "height" )->toInt( ) + m * 2;
			h += GetSystemMetrics( SM_CYCAPTION );
			m_wndModeList->get( "width" )->ini( w );
			m_wndModeList->get( "height" )->ini( h );
			m_wndModeList->RemakeBuffer( prev_w, prev_h, w, h );
			m_wndModeList->Move( );
			m_wndModeList->RenderControls( );
			// двигаем диалоговое окно
			SetWindowPos( hWndDialog, NULL, 0, 0,
				m_wndModeList->get( "width" )->toInt( ),
				m_wndModeList->get( "height" )->toInt( ),
				SWP_NOMOVE
			);
			CenterOneWindowAboveParent( hWndDialog );
		}
	} // void LoadList

	void CStateHandlerSimple::Load( ) {
		if ( m_iState == INTSTATE_LIST ) {
			this->LoadList( );
		} else if ( m_iState == INTSTATE_FORM ) {
			m_wndForm->FillControls( );
		}
	} // void Load

	void CStateHandlerSimple::LoadExtraData( udPSocket sock, udPBase obj, udPWndModeForm wndForm ) {
		if ( !m_arrExtraLoad.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			char code = 0;
			for( vctExtraLoad::iterator i = m_arrExtraLoad.begin( ); i != m_arrExtraLoad.end( ); ++i ) {
				__log->Write( "ctrlid=" + toString( i->ctrlid ) + "\n" );
				code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				if ( sock->ReadByte( &code ) != 1 ) {
					__log->Write( "не удалось считать код\n" );
					break;
				}
				__log->Write( "код: " + toString( ( int ) code ) + "\n" );
				if ( code > 0 ) { // объект находится в сокете
					if ( i->type == ( udInt ) extra_load_extlst ) {
						udPBase obj1 = i->fnGetObject( );
						if ( i->fnSpecLoad ) {
							i->fnSpecLoad( obj, obj1, wndForm, sock );
						}
						if ( obj1->read( sock ) == 1 ) {
							string log = "\n";
							obj1->dump( log );
							__log->Write( log + "\n" );

							ModeFormControlInfo *info = wndForm->GetInfo( i->ctrlid );
							info->extlst->extlst->SetObj( obj1 );
						}
					} else if ( i->type == ( udInt ) extra_load_frmlst ) {
						udArrBase arr;
						if ( i->fnAssignTpl ) {
							udPBase tpl = i->fnGetObject( );
							i->fnAssignTpl( tpl );
							ReadInstances( sock, arr, i->fnGetObject, tpl );
							delete tpl;
						} else {
							ReadInstances( sock, arr, i->fnGetObject );
						}
						if ( !arr.empty( ) ) {
							if ( i->fnAfterLoad ) {
								i->fnAfterLoad( arr );
							}
							ModeFormControlInfo *info = wndForm->GetInfo( i->ctrlid );
							for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
								info->frmlst->AddItem( *i );
							}
							arr.clear( );
						}
					}
				}
			}
		}
	} // void LoadExtraData

	void CStateHandlerSimple::ReadForEdit( udPSocket sock, udPBase obj ) {
		udPBase obj1 = m_fnGetObject( );
		obj1->read( sock );
		obj1->apply( obj );
		delete obj1;
	} // void ReadForEdit

	void CStateHandlerSimple::ClickToolbar( string index ) {
		if ( index == "delete" ) {
			if ( m_iState == INTSTATE_LIST ) {
				udPAppConfig	appConfig = g_pApp->GetConfig( );
				udPWndModel		wnd			= g_pApp->GetModel( )->GetWndModel( );
				udLong			sel			= m_wndModeList->GetList( )->GetSelected( );
				udPSocket sock = BeginRequest( );
				if ( sock ) {
					udPBase obj = m_arrObject[ sel ];
					udBinaryData data;
					data.add( m_szName );
					data.add( ( udInt ) opidDel1 );
					this->PackForDel( data, obj );
					//obj->dump( data );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
					char code = 0;
					sock->ReadByte( &code );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				}
				// обновляем список
				this->Clear( );
				this->Load( );
				this->Show( wnd );
			}
		} else if ( index == "edit" ) {
			if ( m_iState == INTSTATE_LIST ) {
				udPAppConfig appConfig = g_pApp->GetConfig( );
				udLong		sel	= m_wndModeList->GetList( )->GetSelected( );
				udPWndModel	wnd	= g_pApp->GetModel( )->GetWndModel( );
				udPBase		tmp	= m_arrObject[ sel ],
							obj	= m_fnGetObject( );
				bool		watch_mode = false; // режим просмотра, редактирование невозможно
				
				tmp->apply( obj );
				this->Clear( );
				m_iState = INTSTATE_FORM;
				this->Load( );
				
				udPSocket sock = BeginRequest( );
				if ( sock ) {
					udBinaryData data;
					data.add( m_szName );
					data.add( ( udInt ) opidEdt );
					this->PackForEdt( data, obj );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
					char code = 0;
					sock->ReadByte( &code );
					if ( code > 0 ) {
						if ( code == 2 ) {
							watch_mode = true;
						}
						this->ReadForEdit( sock, obj );
						this->LoadExtraData( sock, obj, m_wndForm );
						this->ExtraDataRecieve( sock, obj );
					}
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				} else {
					g_pApp->GetLog( )->Write( "не удалось подсоединиться" );
				}

				m_wndForm->SetIsWatch( watch_mode );
				m_wndForm->SetObj( obj );
				this->Show( wnd );
			}
		}
	} // void ClickToolbar

	void CStateHandlerSimple::OnSelectRow( udLong row ) {
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		if ( wndInsLst ) {
			wndInsLst->SelectRow( row );
			if ( !m_wndModeList->IsModal( ) ) {
				udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
				tbr->EnableItem( "edit"		);
				tbr->EnableItem( "delete"	);
			}
		}
	} // void OnSelectRow

	void CStateHandlerSimple::OnHeaderClick( const string& name, const string& attr ) {
		assert( m_wndModeList );
		assert( m_wndModeList->GetList( ) );
		assert( m_wndModeList->GetPager( ) );
		this->ClearList( );
		udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
		tbr->EnableItem( "edit",	false );
		tbr->EnableItem( "delete",	false );
		if ( m_szOrderBy == attr ) {
			m_iOrderType = 1 - m_iOrderType;
		} else {
			m_szOrderBy = attr;
			m_iOrderType = ordDesc;
		}
		this->LoadList( );
		m_wndModeList->Show( );
	} // void OnHeaderClick

	udInt CStateHandlerSimple::ToolbarAction( string index ) {
		return 0;
	} // udInt ToolbarAction

	void CStateHandlerSimple::OnLinkClick( udPWndLink lnk ) {
		if ( m_iState == INTSTATE_LIST ) {
			string url = lnk->get( "url" )->toString( );
			if ( m_wndModeList->IsModal( ) ) {
				// тут необходимо создавать новое окно с формой в модальном режиме
				if ( url == "#form" ) {
					if ( m_fnGetFormHandler ) {
						udPFormHandler handler = m_fnGetFormHandler( );
						udPMorph obj = DialogForm( m_wndModeList->GetHWndParent( ), handler );
						delete handler;
						udDELETE( obj );
						//
						this->ClearList( );
						this->LoadList( );
						m_wndModeList->Show( );
					}
				}
			} else {
				if ( url == "main" ) {
					g_pApp->GetModel( )->SwitchToMain( );
				} else if ( url == "#form" ) {
					this->SwitchToForm( );
				}
			}
		} else {
		}
	} // void OnLinkClick

	udInt CStateHandlerSimple::FormOk( ) {
		udInt err = ProcForm( );
		if ( err == FormHandlerErrorOk ) {
			if ( !m_wndForm->IsModal( ) ) {
				this->SwitchToList( );
			}
		}
		return err;
	} // void FormOk

	udInt CStateHandlerSimple::FormCancel( ) {
		if ( !m_wndForm->IsModal( ) ) {
			this->SwitchToList( );
		}
		return FormHandlerErrorOk;
	} // void FormCancel

	void CStateHandlerSimple::SwitchToList( ) {
		this->Clear( );
		m_iState = INTSTATE_LIST;
		this->Load( );
		this->Show( g_pApp->GetModel( )->GetWndModel( ) );
	} // void SwitchToList

	void CStateHandlerSimple::SwitchToForm( ) {
		this->Clear( );
		m_iState = INTSTATE_FORM;
		this->Load( );
		this->Show( g_pApp->GetModel( )->GetWndModel( ) );
	} // void SwitchToForm

	void CStateHandlerSimple::PackForAdd( udBinaryData& data, udPBase obj ) {
		udPBase tmp = m_fnGetObject( );
		tmp->apply( obj, true, true );
		tmp->dump( data );
		delete tmp;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !m_arrExtraLoad.empty( ) ) { // запихиваем дополнительные сущности (из списков)
			udPLog __log = g_pApp->GetLog( );
			ModeFormControlInfo *info = NULL;
			for( vctExtraLoad::iterator i = m_arrExtraLoad.begin( ); i != m_arrExtraLoad.end( ); ++i ) {
				if ( i->type == ( udInt ) extra_load_frmlst ) { // если реально доп сущности
					info = m_wndForm->GetInfo( i->ctrlid );
					if ( info && info->frmlst ) {
						WriteInstances( info->frmlst->GetItems( ), data, i->fnGetObject );
					} else {
						data.add( ( udUInt ) 0 );
					}
				}
			}
		}
	} // void PackForAdd

	void CStateHandlerSimple::PackForUpd( udBinaryData& data, udPBase obj ) {
		udPBase tmp = m_fnGetObject( );
		tmp->apply( obj, true, true );
		tmp->dump( data );
		delete tmp;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !m_arrExtraLoad.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			ModeFormControlInfo *info = NULL;
			for( vctExtraLoad::iterator i = m_arrExtraLoad.begin( ); i != m_arrExtraLoad.end( ); ++i ) {
				if ( i->type == ( udInt ) extra_load_frmlst ) { // если реально доп сущности
					info = m_wndForm->GetInfo( i->ctrlid );
					if ( info && info->frmlst ) {
						PackFormListItems( data, info->frmlst, i->fnGetObject );
					}
				}
			}
		}
	} // void PackForUpd

	void CStateHandlerSimple::PackForDel( udBinaryData& data, udPBase obj ) {
		udPBase tmp = m_fnGetObject( );
		tmp->apply( obj, true, true );
		tmp->dump( data );
		delete tmp;
	} // void PackForDel

	void CStateHandlerSimple::PackForEdt( udBinaryData& data, udPBase obj ) {
		udPBase tmp = m_fnGetObject( );
		tmp->apply( obj, true, true );
		tmp->dump( data );
		delete tmp;
	} // void PackForEdt

	void CStateHandlerSimple::ExtraDataSending( udPSocket sock, udPBase obj ) {
	} // void ExtraDataSending

	void CStateHandlerSimple::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
	} // void ExtraDataRecieve

	udInt CStateHandlerSimple::ProcForm( bool modal ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// подгрузка данных формы
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// проверка данных формы
		udInt err = ValidateForm( );
		if ( err != FormHandlerErrorOk ) {
			return err;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		// выполнение операции
		udPBase obj = m_wndForm->GetObj( );
		udPSocket sock = BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( m_szName );
			if ( obj->get( "id" )->toUInt( ) > 0 ) {
				data.add( ( udInt ) opidUpd1 );
				this->PackForUpd( data, obj );
			} else {
				data.add( ( udInt ) opidAdd1 );
				this->PackForAdd( data, obj );
			}
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			this->ExtraDataSending( sock, obj );
			//sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			char code = 0;
			sock->ReadByte( &code );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // bool ProcForm

	void CStateHandlerSimple::OnFilter( ) {
		//if ( m_iState == INTSTATE_LIST ) {
			this->ClearList( );
			m_wndModeList->LoadControlData( );
			this->AfterLoadFilterData( );
			this->LoadList( );
			m_wndModeList->Show( );
		//}
	} // void OnFilter

	void CStateHandlerSimple::OnFilterReset( ) {
		//if ( m_iState == INTSTATE_LIST ) {
			this->ClearList( );
			this->ClearFilter( );
			m_wndModeList->ClearFilter( );
			m_wndModeList->FillControls( );
			//m_wndModeList->LoadControlData( );
			//this->AfterLoadFilterData( );
			this->LoadList( );
			m_wndModeList->Show( );
		//}
	} // void OnFilterReset






	udPBase LoadById( const string& handler_name, udUInt id, pfnGetObject fnGetObject ) {
		udPBase ret = NULL;
		udPSocket sock = BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( handler_name );
			data.add( ( udInt ) udStateHandlerSimple::opidGet );
			data.add( ( udUInt ) id );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			char code = 0;
			sock->ReadByte( &code );
			if ( code > 0 ) {
				udPBase obj = fnGetObject( );
				if ( obj->read( sock ) == 1 ) {
					ret = obj;
				} else {
					udDELETE( obj );
				}
			}
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
		return ret;
	} // udPBase LoadById

	udInt ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов" );
			return -1;
		}
		string log;
		log = "заявлено объектов: " + num.toString( ) + "\n";
		udPBase obj = NULL;
		while( cnt < num.toUInt( ) ) {
			obj = fnGetObject( );
			if ( obj->read( sock ) == 0 ) {
				delete obj;
				break;
			}
			obj->dump( log );
			log += "\n";
			arr.push_back( obj );
			++cnt;
		}
		log += "считано объектов: " + toString( cnt ) + "\n";
		g_pApp->GetLog( )->Write( log );

		return cnt;
	} // udInt ReadInstances

	udInt ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject, udPBase tpl ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов" );
			return -1;
		}
		string log;
		log = "заявлено объектов: " + num.toString( ) + "\n";
		udPBase obj = NULL;
		while( cnt < num.toUInt( ) ) {
			obj = fnGetObject( );
			tpl->apply( obj );
			if ( obj->read( sock ) == 0 ) {
				delete obj;
				break;
			}
			obj->dump( log );
			log += "\n";
			arr.push_back( obj );
			++cnt;
		}
		log += "считано объектов: " + toString( cnt ) + "\n";
		g_pApp->GetLog( )->Write( log );

		return cnt;
	} // udInt ReadInstances

	udInt ReadInstances( udPSocket sock, udPArrBase arr, pfnGetObject fnGetObject ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов" );
			return -1;
		}
		string log;
		log = "заявлено объектов: " + num.toString( ) + "\n";
		udPBase obj = NULL;
		while( cnt < num.toUInt( ) ) {
			obj = fnGetObject( );
			if ( obj->read( sock ) == 0 ) {
				delete obj;
				break;
			}
			obj->dump( log );
			log += "\n";
			arr->push_back( obj );
			++cnt;
		}
		log += "считано объектов: " + toString( cnt ) + "\n";
		g_pApp->GetLog( )->Write( log );

		return cnt;
	} // udInt ReadInstances

	udInt ReadInstances( udPSocket sock, udPArrBase arr, pfnGetObject fnGetObject, udPBase tpl ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов" );
			return -1;
		}
		string log;
		log = "заявлено объектов: " + num.toString( ) + "\n";
		udPBase obj = NULL;
		while( cnt < num.toUInt( ) ) {
			obj = fnGetObject( );
			tpl->apply( obj );
			if ( obj->read( sock ) == 0 ) {
				delete obj;
				break;
			}
			obj->dump( log );
			log += "\n";
			arr->push_back( obj );
			++cnt;
		}
		log += "считано объектов: " + toString( cnt ) + "\n";
		g_pApp->GetLog( )->Write( log );

		return cnt;
	} // udInt ReadInstances

	udInt WriteInstances( udArrBase &arr, udBinaryData &data ) {
		string log;
		udPBase obj = NULL;

		log = "\nsize=" + toString( arr.size( ) ) + "\n";
		data.add( arr.size( ) );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			obj->dump( data );
			obj->dump( log );
			log += "\n";
		}
		g_pApp->GetLog( )->Write( log );
		return 1;
	} // udInt WriteInstances

	udInt WriteInstances( udPDwAscBase items, udBinaryData& data, pfnGetObject fnGetObject ) {
		if ( items == NULL ) {
			g_pApp->GetLog( )->Write( "WriteInstances( udPDwAscBase items, udBinaryData& data, pfnGetObject fnGetObject ), items is NULL\n" );
			return 0;
		}
		if ( fnGetObject == NULL ) {
			g_pApp->GetLog( )->Write( "WriteInstances( udPDwAscBase items, udBinaryData& data, pfnGetObject fnGetObject ), fnGetObject is NULL\n" );
			return 0;
		}
		string log;
		udPBase to = NULL, from = NULL;
		log = "\nsize=" + toString( items->size( ) ) + "\n";
		data.add( items->size( ) );
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			from = i->second;
			to = fnGetObject( );
			to->apply( from, true, true );
			to->dump( log );
			log += "\n";
			to->dump( data );
			udDELETE( to );
		}
		g_pApp->GetLog( )->Write( log );
		return 1;
	} // udInt WriteInstances
	
	void PackFormListItems( udBinaryData& data, udPWndFormList frmlst, pfnGetObject fnGetObject ) {
		udPLog __log = g_pApp->GetLog( );
		string log;
		udPBase to = NULL, from = NULL;
		udArrBase arrAdd, arrUpd, arrDel;
		udPDwAscBase items = frmlst->GetItems( );
		//////////////////////////////////////////////////////////////////////////////////////
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			from = i->second;
			to = fnGetObject( );
			to->apply( from, true, true );
			if ( to->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( to );
			} else {
				arrAdd.push_back( to );
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		log = "\nна добавление, size=" + toString( arrAdd.size( ) ) + "\n";
		data.add( arrAdd.size( ) );
		if ( !arrAdd.empty( ) ) {
			for( udArrBase::iterator i = arrAdd.begin( ); i != arrAdd.end( ); ++i ) {
				to = *i;
				to->dump( data );
				to->dump( log );
				log += "\n";
			}
			ClearArrBase( arrAdd );
		}
		//////////////////////////////////////////////////////////////////////////////////////
		log += "\nна обновление, size=" + toString( arrUpd.size( ) ) + "\n";
		data.add( arrUpd.size( ) );
		if ( !arrUpd.empty( ) ) {
			for( udArrBase::iterator i = arrUpd.begin( ); i != arrUpd.end( ); ++i ) {
				to = *i;
				to->dump( data );
				to->dump( log );
				log += "\n";
			}
			ClearArrBase( arrUpd );
		}
		//////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrdel = frmlst->GetDelItems( );
		log += "\nна удаление, size=" + toString( arrdel->size( ) ) + "\n";
		data.add( arrdel->size( ) );
		if ( !arrdel->empty( ) ) {
			for( udArrBase::iterator i = arrdel->begin( ); i != arrdel->end( ); ++i ) {
				to = *i;
				to->dump( data );
				to->dump( log );
				log += "\n";
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( log );
	} // void PackFormListItems

	udPSocket BeginRequest( bool login ) {
		udPAppConfig	config	= g_pApp->GetConfig( );
		string			host	= config->get( "server_host" )->toString( );
		udInt			port	= config->get( "server_port" )->toInt( ),
						clport	= config->get( "client_port" )->toInt( );
		udPSocket		sock	= new udSocket( udSocketInfo( host.data( ), port ) );

		if ( ( sock->Create( ) == 1 ) && ( sock->Connect( ) == 1 ) ) {
			if ( login ) {
				if ( g_pApp->GetInitObj( )->Login( sock ) ) {
					return sock;
				}
			} else {
				return sock;
			}
		}
		delete sock;

		return NULL;
	} // udPSocket BeginRequest

	void LoadListDataByAttr( udArrBase& arr, udPHandler handler, pfnGetObject fnGetObject, string table, string attr, string attrfrom, string attrto ) {
		udHandlerOption option;
		udPBase obj = NULL, obj1 = fnGetObject( );
		udArrString arrString;
		udBaseConfig config;
		udArrBase arrObject;
		string szWhere = "";

		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			arrString.push_back( ( *i )->get( attr )->toString( ) );
		}

		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "id" ) + " IN(" +
			Join( ",", arrString ) + ")";
		option.SetTable( table );
		option.SetClass( fnGetObject );
		option.SetWhere( szWhere );
		handler->GetUdObject( arrObject, option );
		udArrBase::iterator j;
		udUInt id = 0;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			id = obj->get( attr )->toUInt( );
			obj->get( attrto )->ini( wstring( L"" ) );
			for( j = arrObject.begin( ); j != arrObject.end( ); ++j ) {
				obj1 = *j;
				if ( id == obj1->get( "id" )->toUInt( ) ) {
					obj->get( attrto )->ini( obj1->get( attrfrom )->toWString( ) );
					break;
				}
			}
		}
		ClearArrBase( arrObject );
	} // void LoadListDataByAttr

} // namespace UdSDK
