
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
		m_pObjFilter	= new udMorph;
	}

	CStateHandlerSimple::~CStateHandlerSimple( ) {
	}

	void CStateHandlerSimple::AddManagedTable( const string& name, const string& upd_index, pfnGetObject fnGetObject ) {
		_managed_table tbl;
		tbl.szTableName = name;
		tbl.szUpdIndex = upd_index;
		tbl.fnGetObject = fnGetObject;
		m_arrAddTables.push_back( tbl );
	} // void AddManagedTable

	void CStateHandlerSimple::Init( ) {
		udHandlerOption	option;
		option.SetTable( m_szTableName );
		option.SetClass( m_fnGetObject );
		g_pApp->GetHandler( )->CheckTable( option );

		if ( !m_arrAddTables.empty( ) ) {
			option.Clear( );
			udPHandler handler = g_pApp->GetHandler( );
			for( _vct_mtables::iterator i = m_arrAddTables.begin( ); i != m_arrAddTables.end( ); ++i ) {
				option.SetTable( ( *i ).szTableName );
				option.SetClass( ( *i ).fnGetObject );
				handler->CheckTable( option );
				option.Clear( );
			}
		}
	} // void Init

	void CStateHandlerSimple::InitSync( udPDatabaseSync sync ) {
		sync->AddSyncTable( m_szTableName, m_szName );

		if ( !m_arrAddTables.empty( ) ) {
			for( _vct_mtables::iterator i = m_arrAddTables.begin( ); i != m_arrAddTables.end( ); ++i ) {
				sync->AddSyncTable( ( *i ).szTableName, m_szName );
			}
		}
	} // void InitSync

	void CStateHandlerSimple::AfterLoadList( udArrBase& arr ) {
	} // void AfterLoadList

	void CStateHandlerSimple::ProcRequest( udPSocket sock ) {
		wstring text = L"запрос принят\r\n";

		udType op( ( udInt ) 0 );
		if ( op.read( sock ) == 1 ) { // считали операцию
			//////////////////////////////////////////////////////////////////////////////////////
			// выводим наименование операции
			udInt opId = op.toInt( );
			switch( opId ) {
				case opidAdd: text += L"Добавление";	break;
				case opidDel: text += L"Удаление";		break;
				case opidUpd: text += L"Обновление";	break;

				default:
					this->SchemeCenter( opId, sock );
					return;
					break;
			}
			//////////////////////////////////////////////////////////////////////////////////////
			// выгребли объект
			udPBase obj = m_fnGetObject( );
			obj->read( sock );
			text += L"\r\n";
			obj->dump( text );
			text += L"\r\n";
			//////////////////////////////////////////////////////////////////////////////////////
			// выполняем операцию
			switch( opId ) {
				case opidAdd: this->SyncAdd( sock, obj );	break;
				case opidUpd: this->SyncUpd( sock, obj );	break;
				case opidDel: this->SyncDel( sock, obj );	break;
			}
			//////////////////////////////////////////////////////////////////////////////////////
			delete obj;
		} else {
			text += L"отсутствует индекс операции";
		}
		//g_pApp->GetWndServer( )->UpdateText( text );
	} // void ProcRequest

	void CStateHandlerSimple::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
	} // void ExtraDataRecieve

	void CStateHandlerSimple::SchemeCenter( udInt opId, udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::SchemeCenter, opId=" + toString( opId ) + "\n" );
		if ( opId == opidLst1 ) {
			this->HandleListRequest( sock );
		} else {
			switch( opId ) {
				case opidAdd1: this->HandleAddRequest( sock ); break;
				case opidUpd1: this->HandleUpdRequest( sock ); break;
				case opidDel1: this->HandleDelRequest( sock ); break;
				case opidEdt: this->HandleEdtRequest( sock ); break;
				case opidGet: this->HandleGetRequest( sock ); break;
			}

			g_pApp->GetLog( )->Write( "CStateHandlerSimple::SchemeCenter, отправляем подтверждающий байт\n" );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
		}
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::SchemeCenter, exit\n" );
	} // void SchemeCenter

	void CStateHandlerSimple::HandleAddRequest( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleAddRequest enter\n" );
		udPBase obj = m_fnGetObject( );
		if ( ( obj->read( sock ) == 1 ) && // скачали
			( this->FindObj( obj, "name" ) == 0 ) && // не нашли клона
			( this->AddObj( obj, true ) == 1 ) // добавили основу
		) {
			udPLog __log = g_pApp->GetLog( );
			string log = "\ngot object\n";
			obj->dump( log );
			__log->Write( log + "\n" );
			udPBase tmp = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////
			if ( !m_arrAppend.empty( ) ) { // проверяем наличие доп сущностей из списков
				udUInt id = obj->get( "id" )->toUInt( );
				udArrBase arr;
				udPBase obj1 = NULL;
				for( vctAppend::iterator i = m_arrAppend.begin( ); i != m_arrAppend.end( ); ++i ) {
					if ( i->type == ( udInt ) edit_append_array ) {
						log = "\nwait object\n";
						tmp = i->fnGetObject( );
						tmp->dump( log );
						__log->Write( log + "\n" );
						udDELETE( tmp );

						ReadInstances( sock, arr, i->fnGetObject );
						if ( !arr.empty( ) ) {
							if ( i->fnBeforeProc ) {
								__log->Write( "fnBeforeProc\n" );
								udArrBase arr2, arr3;
								i->fnBeforeProc( arr, arr2, arr3, obj );
							}
							__log->Write( "iterate over loaded objects\n" );
							for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
								obj1 = *j;
								obj1->get( i->fkey )->ini( id );
								this->AddObj( obj1, i->table, true );
							}
							if ( i->fnAfterProc ) {
								__log->Write( "fnAfterProc\n" );
								udArrBase arr2, arr3;
								i->fnAfterProc( arr, arr2, arr3, obj );
							}
						}
						ClearArrBase( arr );
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			this->ExtraDataRecieve( opidUpd1, sock, obj );
			//////////////////////////////////////////////////////////////////////////////////////////
			udBinaryData data;
			data.add( obj->get( "id" )->toUInt( ) );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////////////
		}
		delete obj;
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleAddRequest exit\n" );
	} // void HandleAddRequest

	void CStateHandlerSimple::HandleUpdRequest( udPSocket sock ) {
		udPLog __log = g_pApp->GetLog( );
		string log = "\nwait object:\n";
		udPBase obj = m_fnGetObject( );
		obj->dump( log );
		__log->Write( log + "\n" );
		if ( ( obj->read( sock ) == 1 ) && // считали объект
			( obj->get( "id" )->toUInt( ) > 0 ) && // объект на обновление
			( this->FindObj( obj, "id", false ) == 1 ) && // объект существует
			( this->UpdObj( obj, true ) == 1 ) // обновили объект
		) {
			log = "\ngot object\n";
			obj->dump( log );
			__log->Write( log + "\n" );
			udPBase tmp = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj, m_fnGetObject );
			//////////////////////////////////////////////////////////////////////////////////////////
			if ( !m_arrAppend.empty( ) ) { // проверяем наличие доп сущностей из списков
				udPHandler handler = g_pApp->GetHandler( );
				udPHObjectLock hLock = g_pApp->GetModel( )->GetLockHandler( );
				udUInt id = obj->get( "id" )->toUInt( );
				udArrBase arrLoaded, arrCurrent;
				udPBase obj1 = NULL;
				for( vctAppend::iterator i = m_arrAppend.begin( ); i != m_arrAppend.end( ); ++i ) {
					if ( i->type == ( udInt ) edit_append_array ) {
						//////////////////////////////////////////////////////////////////////////////
						log = "\nwait object\n";
						tmp = i->fnGetObject( );
						tmp->dump( log );
						__log->Write( log + "\n" );
						udDELETE( tmp );
						//////////////////////////////////////////////////////////////////////////////
						ReadInstances( sock, arrLoaded, i->fnGetObject );
						LoadInstanceByForeignKey( handler, obj, arrCurrent, i->fnGetObject, i->table, i->fkey, i->key, i->order, i->where1 );
						//////////////////////////////////////////////////////////////////////////////
						if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
							if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
								for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
									obj1 = *j;
									if ( obj1->get( i->fkey )->toUInt( ) == id ) { // вдруг прилетит херня
										this->DelObj( obj1, i->table, true );
										hLock->UnlockObject( obj1, i->fnGetObject );
									}
								}
							}
						} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
							for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
								obj1 = *j;
								obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
								obj1->get( i->fkey )->ini( id );
								this->AddObj( obj1, i->table, true );
							}
						} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
							udArrBase arrAdd, arrUpd, arrDel;
							udPBase objNew = NULL, objOld = NULL;
							udArrBase::iterator j, k;
							bool bFound = false;

							// добавление и обновление
							for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
								objNew = *j;
								bFound = false;

								for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
									objOld = *k;
									if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
										bFound = true;
										arrUpd.push_back( objNew );
										break;
									}
								}

								if ( !bFound ) {
									arrAdd.push_back( objNew );
								}
							}

							// удаление
							for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
								objOld = *j;
								bFound = false;

								for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
									objNew = *k;
									if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
										bFound = true;
										break;
									}
								}

								if ( !bFound ) {
									arrDel.push_back( objOld );
								}
							}

							// выполняем операции
							if ( i->fnBeforeProc ) {
								i->fnBeforeProc( arrAdd, arrUpd, arrDel, obj );
							}
							if ( !arrAdd.empty( ) ) {
								for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
									obj1 = *j;
									obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
									obj1->get( i->fkey )->ini( id );
									this->AddObj( obj1, i->table, true );
								}
							}
							if ( !arrUpd.empty( ) ) {
								for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
									obj1 = *j;
									obj1->get( i->fkey )->ini( id ); // вдруг прилетит херня
									this->UpdObj( obj1, i->table, "id", true );
									hLock->UnlockObject( obj1, i->fnGetObject );
								}
							}
							if ( !arrDel.empty( ) ) {
								for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
									obj1 = *j;
									if ( obj1->get( i->fkey )->toUInt( ) == id ) { // вдруг прилетит херня
										this->DelObj( obj1, i->table, true );
										hLock->UnlockObject( obj1, i->fnGetObject );
									}
								}
							}
							if ( i->fnAfterProc ) {
								i->fnAfterProc( arrAdd, arrUpd, arrDel, obj );
							}
							//ClearArrBase( arrAdd );
							//ClearArrBase( arrUpd );
							//ClearArrBase( arrDel );
						}
						ClearArrBase( arrLoaded );
						ClearArrBase( arrCurrent );
						//////////////////////////////////////////////////////////////////////////////
						/*ReadInstances( sock, arrAdd, i->fnGetObject );
						ReadInstances( sock, arrUpd, i->fnGetObject );
						ReadInstances( sock, arrDel, i->fnGetObject );
						if ( i->fnBeforeProc ) {
							i->fnBeforeProc( arrAdd, arrUpd, arrDel, obj );
						}
						if ( !arrAdd.empty( ) ) {
							for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
								obj1 = *j;
								obj1->get( i->fkey )->ini( id );
								this->AddObj( obj1, i->table, true );
							}
						}
						if ( !arrUpd.empty( ) ) {
							for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
								obj1 = *j;
								this->UpdObj( obj1, i->table, "id", true );
								g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj1, i->fnGetObject );
							}
						}
						if ( !arrDel.empty( ) ) {
							for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
								obj1 = *j;
								this->DelObj( obj1, i->table, true );
								g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj1, i->fnGetObject );
							}
						}
						if ( i->fnAfterProc ) {
							i->fnAfterProc( arrAdd, arrUpd, arrDel, obj );
						}
						ClearArrBase( arrAdd );
						ClearArrBase( arrUpd );
						ClearArrBase( arrDel );*/
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			this->ExtraDataRecieve( opidAdd1, sock, obj );
			//////////////////////////////////////////////////////////////////////////////////////////
			udBinaryData data;
			data.add( obj->get( "id" )->toUInt( ) );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////////////
		}
		delete obj;
	} // void HandleUpdRequest

	void CStateHandlerSimple::HandleDelRequest( udPSocket sock ) {
		udPBase obj = m_fnGetObject( );
		if ( ( obj->read( sock ) == 1 ) && ( obj->get( "id" )->toUInt( ) > 0 ) ) {
			if ( ( this->FindObj( obj, "id", false ) == 1 ) && ( this->DelObj( obj, true ) == 1 ) ) {
				g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj, m_fnGetObject );
			}
		}
		delete obj;
	} // void HandleDelRequest

	void CStateHandlerSimple::FilterEditData( udArrBase& arr ) {
	} // void FilterEditData

	/**
		присоединяет к бинарным данным дополнительные сущности на основании текущей
		data - бинарные данные
		obj - сущность, которая уходит на редактирование
	*/
	void CStateHandlerSimple::AppendDataEdt( udBinaryData& data, udPBase obj ) {
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::AppendDataEdt\n" );
		/**
			если есть присоединяемые данные
		*/
		if ( !m_arrAppend.empty( ) ) {
			udPHandler handler = g_pApp->GetHandler( );
			udPHObjectLock hLock = g_pApp->GetModel( )->GetLockHandler( );
			udPBase tmp = NULL;
			udArrBase arr;
			string log;

			for( vctAppend::iterator i = m_arrAppend.begin( ); i != m_arrAppend.end( ); ++i ) {
				if ( i->type == ( udInt ) edit_append_foreignkey ) {
					g_pApp->GetLog( )->Write( "CStateHandlerSimple::AppendDataEdt, edit_append_foreignkey\n" );
					// если есть особая функция загрузки, то используем ее
					if ( i->fnSpecAppend ) {
						tmp = i->fnSpecAppend( data, obj );
					} else {
						tmp = LoadInstanceByForeignKey( handler, obj, i->fnGetObject, i->table, i->fkey, i->key );
					}
					if ( tmp ) { // добавляем объект в данные
						data.add( ( char ) 1 );
						tmp->dump( data );
						hLock->LockObject( tmp, i->fnGetObject );

						log = "\n";
						tmp->dump( log );
						g_pApp->GetLog( )->Write( log + "\n" );

						udDELETE( tmp );
					} else { // объекта нет, информируем об этом
						data.add( ( char ) 0 );
					}
				} else if ( i->type == ( udInt ) edit_append_array ) {
					g_pApp->GetLog( )->Write( "CStateHandlerSimple::AppendDataEdt, edit_append_array\n" );
					// подгружаем дочерние сущности
					LoadInstanceByForeignKey( handler, obj, arr, i->fnGetObject, i->table, i->fkey, i->key, i->order, i->where1 );
					if ( arr.empty( ) ) { // потомков нет
						data.add( ( char ) 0 );
					} else { // потомки есть
						data.add( ( char ) 1 );
						if ( i->fnFilter ) { // если нужно фильтровать, то пожалуйста
							i->fnFilter( arr );
						}
						// лочим потомков
						for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
							hLock->LockObject( *j, i->fnGetObject );
						}
						// добавляем сущности
						WriteInstances( arr, data );
						ClearArrBase( arr );
					}
				}
			}
		}
	} // void AppendDataEdt

	void CStateHandlerSimple::ExtraDataSending( udPSocket sock, udPBase obj ) {
	} // void ExtraDataSending

	void CStateHandlerSimple::HandleEdtRequest( udPSocket sock ) {
		udPBase obj = m_fnGetObject( );
		if ( ( obj->read( sock ) == 1 ) && ( obj->get( "id" )->toUInt( ) > 0 ) ) {
			udPHandler handler = g_pApp->GetHandler( );
			udArrBase arr;
			udArrError err;
			udHandlerOption option;
			udBaseConfig config;
			string szWhere;
			obj->GetConfig( config );
			szWhere = config.GetAttrIndex( "id" ) + "=" + obj->get( "id" )->toString( );
			option.SetTable( m_szTableName );
			option.SetClass( m_fnGetObject );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( g_pApp->GetLog( ), err );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 0 );
			} else if ( arr.empty( ) ) {
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) -1 );
			} else {
				udPBase obj1 = arr[ 0 ];
				udBinaryData data;
				if ( g_pApp->GetModel( )->GetLockHandler( )->IsLockedObject( obj, m_fnGetObject ) ) {
					data.add( ( char ) 2 );
				} else {
					data.add( ( char ) 1 );
				}

				g_pApp->GetModel( )->GetLockHandler( )->LockObject( obj, m_fnGetObject );

				string log = "\nотправляем объект на редактирование\n";
				obj1->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				obj1->dump( data );
				this->AppendDataEdt( data, obj1 );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				this->ExtraDataSending( sock, obj1 );
			}
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			char code = 0;
			sock->ReadByte( &code );
		}
		delete obj;
	} // void HandleEdtRequest

	void CStateHandlerSimple::HandleListRequest( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, enter\n" );
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPPager pager = new udPager;
		string szOrder, szOrderBy, szOrderType = "DESC";
		udInt iOrderType = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		// номер страницы
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, номер страницы\n" );
		udType num( ( udInt ) 0 );
		num.read( sock );
		pager->SetPage( num.toInt( ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		// размер страницы
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, размер страницы\n" );
		num.read( sock );
		pager->SetPageSize( num.toInt( ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		// атрибут упорядочивания
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, атрибут упорядочивания\n" );
		udType order( string( "" ) );
		order.read( sock );
		szOrderBy = order.toString( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// тип упорядочивания
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, тип упорядочивания\n" );
		num.read( sock );
		iOrderType = num.toInt( );
		if ( iOrderType == 1 ) {
			szOrderType = "ASC";
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		// фильтр
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, фильтр\n" );
		string log = "";
		m_pObjFilter->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );

		m_pObjFilter->read( sock );

		log = "";
		m_pObjFilter->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		// загрузка данных
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, загрузка данных\n" );
		udPBase obj = m_fnGetObject( );
		udBaseConfig config;
		obj->GetConfig( config );
		udDELETE( obj );
		szOrder = config.GetAttrIndex( szOrderBy ) + " " + szOrderType;
		config.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udArrBase arr;
		udBinaryData data;

		option.SetTable( m_szTableName );
		option.SetClass( m_fnGetObject );
		if ( !szOrder.empty( ) ) {
			option.SetOrder( szOrder );
		}
		option.SetWhere( this->GetWhereCondition( ) );
		option.SetLimit( pager->GetSQLLimit( ) );
		udLongLong cnt = handler->CountUdObject( option );
		if ( !this->CustomLoadObjects( pager, arr, err, szOrderBy, iOrderType, cnt ) ) {
			handler->GetUdObject( arr, option, &err );
		}
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			data.add( ( char ) 0 );
		} else {
			data.add( ( char ) 1 );
			data.add( ( udUInt ) cnt );
			this->AfterLoadList( arr );
			WriteInstances( arr, data );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
		//////////////////////////////////////////////////////////////////////////////////////////
		delete pager;
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::HandleListRequest, exit\n" );
	} // void HandleListRequest

	void CStateHandlerSimple::HandleGetRequest( udPSocket sock ) {
		udType num( ( udUInt ) 0 );
		if ( num.read( sock ) != 1 ) {
			return;
		}
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udBaseConfig config;
		udPBase obj = NULL;
		string szWhere;
		obj = m_fnGetObject( );
		obj->GetConfig( config );
		udDELETE( obj );
		szWhere = config.GetAttrIndex( "id" ) + "=" + num.toString( );
		option.SetTable( m_szTableName );
		option.SetClass( m_fnGetObject );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		} else if ( arr.empty( ) ) {
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		} else {
			obj = arr[ 0 ];
			udBinaryData data;
			data.add( ( char ) 1 );
			obj->dump( data );
			ClearArrBase( arr );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
		}
	} // void HandleGetRequest

	udInt CStateHandlerSimple::FindObj( udPBase obj, const string& attr, bool apply ) {
		/*
		0 - клонов не найдено и все прошло хорошо
		1 - клон найден и он только 1
		2 - клон найден и их больше 1
		3 - произошли ошибки при поиске клонов
		*/
		//////////////////////////////////////////////////////////////////////////////////////////
		udPHandler		handler = g_pApp->GetHandler( );
		udBaseConfig	config;
		udHandlerOption	option;
		udArrBase		arr;
		udArrError		err;
		udPType			type	= NULL;
		string			szWhere = "";
		udInt			ret		= 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		obj->GetConfig( config );
		type = obj->GetAttrValue( attr, config, udBase::UDSDK_CONFIG_DATABASE );
		szWhere = config.GetAttrIndex( attr ) + "=" + type->toString( );
		config.Clear( );
		delete type;
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetTable( m_szTableName );
		option.SetClass( m_fnGetObject );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );

		if ( !err.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				__log->Write( i->GetText( ) + "\r\n" );
			}
			err.clear( );
			ret = 3;
		} else if ( arr.empty( ) ) { // объект не найден
			ret = 0;
		} else { // объект найден
			if ( apply ) {
				arr[ 0 ]->apply( obj );
			}
			if ( arr.size( ) > 1 ) {
				ret = 2;
			} else {
				ret = 1;
			}
		}
		ClearArrBase( arr );
		err.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // udInt FindObj

	udInt CStateHandlerSimple::SyncAdd( udPSocket sock, udPBase obj ) {
		char res = 0;
		switch( this->FindObj( obj, "name" ) ) {
			case 0: // объект не найден
				if ( this->AddObj( obj ) == 1 ) {
					res = 1;
				} else {
					res = 3;
				}
				break;

			case 1: // объект найден 1
			case 2: // объект найден > 1
				res = 1;
				break;

			case 3: // произошли ошибки
				res = 3;
				break;

			default:
				res = 3;
				break;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udBinaryData data;
		data.add( res );
		if ( res == 1 ) { // объект был  принят, высылаем актуальные данные
			obj->dump( data );
		}
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		return 0;
	} // void SyncAdd

	udInt CStateHandlerSimple::SyncUpd( udPSocket sock, udPBase obj ) {
		if ( obj->get( "id" )->toUInt( ) == 0 ) {
			sock->SendByte( ( char ) 3 );
			return 0;
		}

		char res = 0;
		switch( this->FindObj( obj, "id", false ) ) {
			case 0: // объект не найден
				res = 2; // возможно он был удален
				break;

			case 1: // обновляем объект
				if ( this->UpdObj( obj ) == 1 ) {
					res = 1;
				} else {
					res = 3;
				}
				break;

			case 2: // не должно быть больше 1 объекта
				res = 3;
				break;

			case 3:
				res = 3;
				break;

			default:
				res = 3;
				break;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udBinaryData data;
		data.add( res );
		if ( res == 1 ) { // объект был  принят, высылаем актуальные данные
			obj->dump( data );
		}
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		return 0;
	} // void SyncUpd

	udInt CStateHandlerSimple::SyncDel( udPSocket sock, udPBase obj ) {
		if ( obj->get( "id" )->toUInt( ) == 0 ) {
			sock->SendByte( ( char ) 3 );
			return 0;
		}

		udPHandler		handler = g_pApp->GetHandler( );
		udBaseConfig	config;
		udHandlerOption	option;
		udArrBase		arr;
		udArrError		err;
		udPType			type	= NULL;
		string			szWhere = "";
		char			res		= 0;

		obj->GetConfig( config );
		type = obj->GetAttrValue( "id", config, udBase::UDSDK_CONFIG_DATABASE );
		szWhere = config.GetAttrIndex( "id" ) + "=" + type->toString( );
		config.Clear( );
		delete type;

		option.SetTable( m_szTableName );
		option.SetClass( m_fnGetObject );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );

		if ( !err.empty( ) ) {
			udPLog __log = g_pApp->GetLog( );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				__log->Write( i->GetText( ) + "\r\n" );
			}
			err.clear( );
			res = 3;
		} else if ( arr.empty( ) ) { // объект не найден
			res = 2;
		} else { // объект найден
			if ( this->DelObj( obj ) == 1 ) {
				res = 1;
			} else {
				res = 3;
			}
		}
		ClearArrBase( arr );
		err.clear( );
		udBinaryData data;
		data.add( res );
		if ( res == 1 ) { // объект был  принят, высылаем актуальные данные
			obj->dump( data );
		}
		sock->Send( data );

		return 0;
	} // void SyncDel

	udInt CStateHandlerSimple::AddObj( udPBase obj, const string& table, bool save_info ) {
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::AddObj enter\n" );
		udArrBase		arr;
		udHandlerOption	option;
		udArrError		err;
		udPHandler		handler = g_pApp->GetHandler( );
		udArrString		arrQuery;

		udPLog __log = g_pApp->GetLog( );
		string log = "\nCStateHandlerSimple::AddObj, add object, ";
		obj->dump( log );
		__log->Write( log + "\n" );
		
		arr.push_back( obj );
		option.SetTable( table );
		handler->AddUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			return 0;
		} else {
			udUInt id = ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( );
			obj->get( "id" )->ini( id );
			arr.clear( );
			arr.push_back( obj );
			handler->AddUdObject( arr, option, arrQuery );
		}
		g_pApp->GetLog( )->Write( "CStateHandlerSimple::AddObj exit\n" );
		return 1;
	} // udInt AddObj

	udInt CStateHandlerSimple::UpdObj( udPBase obj, const string& table, const string& updattr, bool save_info ) {
		udArrBase		arr;
		udHandlerOption	option;
		udArrError		err;
		udPHandler		handler = g_pApp->GetHandler( );
		udArrString		arrQuery;
		
		arr.push_back( obj );
		option.SetTable( table );
		option.SetIndexAttr( updattr );
		handler->UpdUdObject( arr, option, &err, &arrQuery );
		if ( !err.empty( ) ) {
			udPLog log = g_pApp->GetLog( );
			log->Write( arrQuery[ 0 ] + "\n" );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				log->Write( i->GetText( ) + "\n" );
			}
			err.clear( );
			return 0;
		}
		return 1;
	} // udInt UpdObj

	udInt CStateHandlerSimple::DelObj( udPBase obj, const string& table, bool save_info ) {
		udArrBase		arr;
		udHandlerOption	option;
		udArrError		err;
		udPHandler		handler = g_pApp->GetHandler( );
		udArrString		arrQuery;
		
		arr.push_back( obj );
		option.SetTable( table );
		handler->DelUdObject( arr, option, &err, &arrQuery );
		if ( !err.empty( ) ) {
			udPLog log = g_pApp->GetLog( );
			log->Write( arrQuery[ 0 ] + "\n" );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				log->Write( i->GetText( ) + "\n" );
			}
			err.clear( );
			return 0;
		}
		return 1;
	} // udInt DelObj

	udInt CStateHandlerSimple::AddObj( udPBase obj, bool save_info ) {
		return this->AddObj( obj, m_szTableName, save_info );
	} // udInt AddObj

	udInt CStateHandlerSimple::UpdObj( udPBase obj, bool save_info ) {
		return this->UpdObj( obj, m_szTableName, m_szUpdIndex, save_info );
	} // udInt UpdObj

	udInt CStateHandlerSimple::DelObj( udPBase obj, bool save_info ) {
		return this->DelObj( obj, m_szTableName, save_info );
	} // udInt DelObj

	void CStateHandlerSimple::AddAppendData( udInt type, const string& table, const string& fkey, const string& key, pfnGetObject fnGetObject, const string& order, const string& where1, pfnFlt fnFilter, pfnFltExtra fnBeforeProc, pfnFltExtra fnAfterProc, pfnSpecAppend fnSpecAppend ) {
		_edit_append data;
		data.type = type;
		data.table = table;
		data.fkey = fkey;
		data.key = key;
		data.fnGetObject = fnGetObject;
		data.order = order;
		data.where1 = where1;
		data.fnFilter = fnFilter;
		data.fnBeforeProc = fnBeforeProc;
		data.fnAfterProc = fnAfterProc;
		data.fnSpecAppend = fnSpecAppend;
		m_arrAppend.push_back( data );
	} // void AddAppendData

	/**
		подгружает значения для элементов списка
		arr - набор сущностей, для которых будут пдгружаться данные
		handler - обработчик взаимодействия с СУБД
		fnGetObject - калбэк создания сущностей для подгрузки
		table - имя таблица сущностей для подгрузки
		attr - атрибут сущности (из arr), по значениям которых подгружать данные
		attrfrom - имя атрибута (подгруженных сущностей), из которого будут браться данные
		attrto - имя атрибута (из arr), в который будут сохраняться данные
	*/
	void LoadListDataByAttr( udArrBase& arr, udPHandler handler, pfnGetObject fnGetObject, string table, string attr, string attrfrom, string attrto ) {
		udHandlerOption option;
		udPBase obj = NULL, obj1 = fnGetObject( );
		udArrString arrString;
		udBaseConfig config;
		udArrBase arrObject;
		udArrError err;
		string szWhere = "";

		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			arrString.push_back( ( *i )->get( attr )->toString( ) );
		}

		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
		option.SetTable( table );
		option.SetClass( fnGetObject );
		option.SetWhere( szWhere );
		handler->GetUdObject( arrObject, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		} else {
			g_pApp->GetLog( )->Write( "LoadListDataByAttr, size=" + toString( arrObject.size( ) ) + "\n" );

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
		}
		ClearArrBase( arrObject );
	} // void LoadListDataByAttr

	void LoadListDataByAttr( udArrBase& arr, udPHandler handler, pfnGetObject fnGetObject, const string& table, const string& attr, udArrBase& attrs ) {
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
		szWhere = config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
		option.SetTable( table );
		option.SetClass( fnGetObject );
		option.SetWhere( szWhere );
		handler->GetUdObject( arrObject, option );
		udArrBase::iterator j;
		udUInt id = 0;
		string attrfrom, attrto;
		udInt save = 0;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			id = obj->get( attr )->toUInt( );
			for( udArrBase::iterator k = attrs.begin( ); k != attrs.end( ); ++k ) {
				attrfrom = ( *k )->get( "attr1" )->toString( );
				attrto = ( *k )->get( "attr2" )->toString( );
				*( obj->get( attrto ) ) = *( ( *k )->get( "type" ) );
				for( j = arrObject.begin( ); j != arrObject.end( ); ++j ) {
					obj1 = *j;
					if ( id == obj1->get( "id" )->toUInt( ) ) {
						obj->get( attrto )->assign( obj1->get( attrfrom ), true );
						/*if ( save ) {
							obj->get( attrto )->assign( obj1->get( attrfrom ), true );
						} else {
							obj->get( attrto )->ini( obj1->get( attrfrom )->toWString( ) );
						}*/
						break;
					}
				}
			}
			/*obj->get( attrto )->ini( wstring( L"" ) );
			for( j = arrObject.begin( ); j != arrObject.end( ); ++j ) {
				obj1 = *j;
				if ( id == obj1->get( "id" )->toUInt( ) ) {
					obj->get( attrto )->ini( obj1->get( attrfrom )->toWString( ) );
					break;
				}
			}*/
		}
		ClearArrBase( arrObject );
	} // void LoadListDataByAttr

	/**
		считывает сущности из сокета
	*/
	udInt ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов\n" );
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

	/**
		считывает объекты из сокета (с применением шаблона сущности)
		sock - сокет
		arr - массив для записи
		fnGetObject - калбэк создания сущности
		tpl - дополнительный шаблон сущности
	*/
	udInt ReadInstances( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject, udPBase tpl ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов\n" );
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

	/**
		записывает набор сущностей в бинарные данные
		arr - набор сущностей
		data - бинарные данные
	*/
	udInt WriteInstances( udArrBase& arr, udBinaryData& data ) {
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

	/**
		записывает набор сущностей в бинарные данные
		arr - набор сущностей
		data - бинарные данные
	*/
	udInt WriteInstances( udArrMorph& arr, udBinaryData& data ) {
		string log;
		udPMorph obj = NULL;

		log = "\nsize=" + toString( arr.size( ) ) + "\n";
		data.add( arr.size( ) );
		for( udArrMorph::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			obj->dump( data );
			obj->dump( log );
			log += "\n";
		}
		g_pApp->GetLog( )->Write( log );
		return 1;
	} // udInt WriteInstances

	/**
		записывает набор сущностей в бинарные данные
		arr - набор сущностей
		data - бинарные данные
	*/
	udInt WriteInstances( udDwAscBase& arr, udBinaryData& data ) {
		string log;
		udPMorph obj = NULL;

		log = "\nsize=" + toString( arr.size( ) ) + "\n";
		data.add( arr.size( ) );
		for( udDwAscBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = i->second;
			obj->dump( data );
			obj->dump( log );
			log += "\n";
		}
		g_pApp->GetLog( )->Write( log );
		return 1;
	} // udInt WriteInstances

	/**
		подгружает родительскую сущность по foreign key потомка
		handler - обработчик взаимодействия с СУБД
		obj - потомок
		fnGetObject - калбэк создания родительской сущности
		table - таблица родительской сущности
		fkey - имя атрибута потомка
		key - имя атрибута родителя

		если родитель не найден, то вовзращается NULL
	*/
	udPBase LoadInstanceByForeignKey( udPHandler handler, udPBase obj, pfnGetObject fnGetObject, const string& table, const string& fkey, const string& key ) {
		udPBase ret = NULL;
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;
		string szWhere;

		ret = fnGetObject( );
		ret->GetConfig( config );
		delete ret;
		ret = NULL;
		szWhere =
			config.GetAttrIndex( key, udBaseConfig::UDSDK_CONFIG_DATABASE ) + "=" +
			obj->get( fkey )->toString( );
		option.SetTable( table );
		option.SetClass( fnGetObject );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			ret = NULL;
		} else if ( arr.empty( ) ) {
			g_pApp->GetLog( )->Write( "объект не найден\n" );
			ret = NULL;
		} else {
			ret = arr[ 0 ];
		}
		return ret;
	} // udPBase LoadInstanceByForeignKey

	/**
		подгружает дочерние сущности по foreign key родителя
		obj - родительский объект
		handler - обработчик работы с СУБД
		arr - массив, в который будут сохранены подгруженные объекты
		fnGetObject - калбэк создания дочерних объектов
		table - таблица дочерних сущностей
		fkey - имя атрибута дочерней сущности
		key - имя атрибута родительской сущности
		order - упорядочивание (опционально)
		where1 - дополнительное условие (опционально)
	*/
	void LoadInstanceByForeignKey( udPHandler handler, udPBase obj, udArrBase& arr, pfnGetObject fnGetObject, const string& table, const string& fkey, const string& key, const string& order, const string& where1 ) {
		g_pApp->GetLog( )->Write( "LoadInstanceByForeignKey, enter\n" );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj1 = NULL;
		g_pApp->GetLog( )->Write( "LoadInstanceByForeignKey, 1\n" );
		string log = "\nLoadInstanceByForeignKey, 2, ";
		obj1 = fnGetObject( );
		obj->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );
		obj1->GetConfig( config );
		udDELETE( obj1 );
		g_pApp->GetLog( )->Write( "LoadInstanceByForeignKey, 3\n" );
		string szWhere = config.GetAttrIndex( fkey, udBaseConfig::UDSDK_CONFIG_DATABASE ) + "=" + obj->get( key )->toString( );
		if ( !where1.empty( ) ) {
			szWhere += " AND " + where1;
		}
		option.SetTable( table );
		option.SetClass( fnGetObject );
		option.SetWhere( szWhere );
		if ( !order.empty( ) ) {
			option.SetOrder( order );
		}
		g_pApp->GetLog( )->Write( "LoadInstanceByForeignKey, 4\n" );
		handler->GetUdObject( arr, option );
		g_pApp->GetLog( )->Write( "LoadInstanceByForeignKey, exit\n" );
	} // void LoadInstanceByForeignKey

} // namespace UdSDK
