
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void ClientFilterAttach( udArrBase& arrAdd, udArrBase& arrUpd, udArrBase& arrDel, udPBase obj ) {
		udPAppConfig pAppConfig = g_pApp->GetConfig( );
		wstring tmp_dir = pAppConfig->get( "temporary_dir" )->toWString( ),
			client_dir = pAppConfig->get( "files_dir" )->toWString( ) + L"\\client",
			dst_dir, filename, src;
		HANDLE hFile = NULL;
		udPLog __log = g_pApp->GetLog( );
		//string log = "\nClientFilterAttach\n";
		udPBase app = NULL;
		/*log += "сам объект, ";
		obj->dump( log );*/

		dst_dir = client_dir + L"\\" + obj->get( "id" )->toWString( );
		CreateDirectory( dst_dir.data( ), NULL );
		//__log->Write( "создаем папку '" + toString( dst_dir ) + "'\n" );

		//log += "на удаление:\n";
		for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
			app = *i;
			/*app->dump( log );
			log += "\n";*/

			filename = dst_dir + L"\\" + app->get( "name" )->toWString( );
			__log->Write( "удаляем файл '" + toString( filename ) + "'\n" );
			DeleteFile( filename.data( ) );
		}
		//log += "\nна добавление:\n";
		for( udArrBase::iterator i = arrAdd.begin( ); i != arrAdd.end( ); ++i ) {
			app = *i;
			/*app->dump( log );
			log += "\n";*/

			src = tmp_dir + L"\\" + app->get( "name" )->toWString( );
			filename = dst_dir + L"\\" + app->get( "name" )->toWString( );
			/*__log->Write( "добавляем файл '" + toString( filename ) + "', копируем из временного '" +
				toString( src ) + "'\n" );*/
			/*if ( CopyFile( src.data( ), filename.data( ), FALSE ) == 0 ) {
				__log->Write( "CopyFile, error code=" + toString( GetLastError( ) ) + "\n" );
			}*/
		}
		//log += "на обновление:\n";
		for( udArrBase::iterator i = arrUpd.begin( ); i != arrUpd.end( ); ++i ) {
			app = *i;
			/*app->dump( log );
			log += "\n";*/

			src = tmp_dir + L"\\" + app->get( "name" )->toWString( );
			filename = dst_dir + L"\\" + app->get( "name" )->toWString( );
			//__log->Write( "удаляем файл '" + toString( filename ) + "'\n" );
			//DeleteFile( filename.data( ) );
			/*if ( CopyFile( src.data( ), filename.data( ), FALSE ) == 0 ) {
				__log->Write( "CopyFile, error code=" + toString( GetLastError( ) ) + "\n" );
			}*/
		}
		
		//__log->Write( log );
	} // void ClientFilterAttach

	static void ClientFilterDoc( udArrBase& arr ) {
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectClient, "ud_client", "client", "fio", "client_" );
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectDocType, "ud_doctype", "type", "name", "type_" );
	} // void ClientFilterDoc

	static udPBase ClientSpecLoadDoc( udBinaryData& data, udPBase obj ) {
		udPHandler handler = g_pApp->GetHandler( );
		udPBase ret = LoadInstanceByForeignKey( handler, obj, GetProjectClDoc, "ud_cldoc", "doc", "id" );

		if ( ret ) {
			string log = "\n";
			ret->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			udArrBase arr;
			arr.push_back( ret );
			LoadListDataByAttr( arr, handler, GetProjectClient, "ud_client", "client", "fio", "client_" );
			LoadListDataByAttr( arr, handler, GetProjectDocType, "ud_doctype", "type", "name", "type_" );
		}

		return ret;
	} // udPBase ClientSpecLoadDoc

	CHClient::CHClient( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_client";
		m_szUpdIndex	= "id";
		m_szName		= "client";
		m_fnGetObject	= GetProjectClient;

		this->AddManagedTable( "ud_attach", "id", GetProjectAttach );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_cldoc", "doc", "id", GetProjectClDoc, "", "", NULL, NULL, NULL, ClientSpecLoadDoc );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_cldoc", "client", "id", GetProjectClDoc, "`cldoc_id` ASC", "", ClientFilterDoc );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_attach", "obj", "id", GetProjectAttach, "`attach_id` ASC", "`attach_type`=" + toString( udProjectAttach::ATTACH_CLIENT ) );//,
			//NULL, NULL, ClientFilterAttach );

		m_pObjFilter->get( "fio" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "date1" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "date2" )->ini( wstring( L"" ) );
		//m_pObjFilter->get( "date1" )->ini( datetime( ) );
		//m_pObjFilter->get( "date2" )->ini( datetime( ) );
	}

	CHClient::CHClient( const CHClient& objCopy ) {
	}

	CHClient::~CHClient( ) {
	}

	CHClient& CHClient::operator =( const CHClient& objCopy ) {
		return *this;
	}

	void CHClient::Init( ) {
		udStateHandlerSimple::Init( );
		
		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		//folder = appConfig->get( "temporary_dir" )->toWString( ) + L"\\client";
		//CreateDirectory( folder.data( ), NULL );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\client";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	udInt CHClient::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;//return udStateHandlerSimple::FindObj( obj, "fio", apply );
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	void CHClient::DeleteLinkedObjects( udPBase obj ) {
		udPDatabase db = g_pApp->GetDatabase( );
		string query = "", szWhere = "";
		udBaseConfig config;
		udPBase obj1;
		udPType type = NULL;

		// документы
		obj1 = new udProjectClDoc;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "client" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// приложение
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		delete obj1;
		type = new udType( ( udInt ) ( udProjectAttach::ATTACH_CLIENT ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) +
			config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
		wstring folder = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
			L"\\client\\" + obj->get( "id" )->toWString( );
		// папку удалить
		DirectoryClear( folder );
		RemoveDirectory( folder.data( ) );

		// лицевой счет
		obj1 = new udProjectPersAcc;
		obj1->GetConfig( config );
		delete obj1;
		type = new udType( ( udInt ) ( udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) + " AND " +
			config.GetAttrIndex( "obj1" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
	} // void DeleteLinkedObjects

	void CHClient::CreateAccount( udPHandler handler, udUInt id ) {
		udArrBase arr, arr1;
		udHandlerOption option;
		udPBase obj = NULL;
		option.SetTable( "ud_company" );
		option.SetClass( GetProjectCompany );
		handler->GetUdObject( arr1, option );
		option.Clear( );
		for( udArrBase::iterator i = arr1.begin( ); i != arr1.end( ); ++i ) {
			obj = new udProjectPersAcc;
			obj->get( "type" )->ini( ( udInt ) ( udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) );
			obj->get( "obj1" )->ini( id );
			obj->get( "obj2" )->ini( ( *i )->get( "id" )->toUInt( ) );
			arr.push_back( obj );
		}
		option.SetTable( "ud_persacc" );
		handler->AddUdObject( arr, option );
		ClearArrBase( arr1 );
		ClearArrBase( arr );
	} // void CreateAccount

	udInt CHClient::AddObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::AddObj( obj, save_info ) == 1 ) {
			// дополнительно создаем лицевой счет
			this->CreateAccount( g_pApp->GetHandler( ), obj->get( "id" )->toUInt( ) );
			// дополнительно создаем папку для приложений
			wstring dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\client\\" + obj->get( "id" )->toWString( );
			CreateDirectory( dir.data( ), NULL );
			g_pApp->GetLog( )->Write( "'" + toString( dir ) + "'\n" );
			return 1;
		}
		return 0;
	} // udInt AddObj

	udInt CHClient::DelObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::DelObj( obj, save_info ) == 1 ) {
			// удаляем связанные объекты
			this->DeleteLinkedObjects( obj );
			return 1;
		}
		return 0;
	} // udInt DelObj

	void CHClient::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		if ( ( opId == opidAdd1 ) || ( opId == opidUpd1 ) ) {
		//////////////////////////////////////////////////////////////////////////////////////////
			// оставить на откуп аксессору данных
			char code = 0;
			sock->ReadByte( &code );
			if ( code == 80 ) {
				udType num( ( udUInt ) 0 );
				if ( num.read( sock ) != 1 ) {
					return;
				}
				if ( num.toUInt( ) > 0 ) {
					//////////////////////////////////////////////////////////////////////////////////
					udPLog __log = g_pApp->GetLog( );
					udUInt cnt = 0, num1 = num.toUInt( );
					string log;
					wstring tmp_dir = g_pApp->GetConfig( )->get( "temporary_dir" )->toWString( );
					udInt err = 0;
					udFileTransfer objFileTransfer;
					// очищаем временную папку от левых файлов
					DirectoryClear( tmp_dir );
					objFileTransfer.SetTmpDir( tmp_dir );
					while( cnt < num1 ) {
						err = objFileTransfer.Recv( sock );
						log = udFileTransfer::ErrorExplain( err ) + "\n";
						__log->Write( log );
						if ( err != udFileTransfer::FileTransferErrorOk ) {
							break;
						}
						++cnt;
					}
					//////////////////////////////////////////////////////////////////////////////////
					// копируем файлы
					wstring dst_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) + L"\\client\\" + obj->get( "id" )->toWString( );
					CreateDirectory( dst_dir.data( ), NULL );
					DirectoryCopyFiles( tmp_dir, dst_dir );
					DirectoryClear( tmp_dir );
					//////////////////////////////////////////////////////////////////////////////////
				}
			}

			if ( obj->get( "doc" )->toUInt( ) == 0 ) {
				udPHandler handler = g_pApp->GetHandler( );
				udHandlerOption option;
				udArrBase arr;
				udArrError err;
				udBaseConfig config;
				udPBase doc = new udProjectClDoc;
				doc->GetConfig( config );
				udDELETE( doc );

				option.SetWhere(
					config.GetAttrIndex( "client" ) + "=" + obj->get( "id" )->toString( ) + " AND " +
					config.GetAttrIndex( "is_default" ) + "=1"
				);
				option.SetTable( "ud_cldoc" );
				option.SetClass( GetProjectClDoc );
				handler->GetUdObject( arr, option, &err );
				option.Clear( );
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
				} else if ( !arr.empty( ) ) {
					string log = "\nnew doc was set as default, list of marked as default:\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log );

					doc = *( arr.begin( ) );
					option.SetTable( "ud_client" );
					option.SetIndexAttr( "id" );
					obj->get( "doc" )->ini( doc->get( "id" )->toUInt( ) );
					handler->UpdUdObject( obj, option );

					ClearArrBase( arr );
				}
			}
		//////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void ExtraDataRecieve

	void CHClient::ExtraDataSending( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPHandler handler = g_pApp->GetHandler( );
		udPLog __log = g_pApp->GetLog( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;
		string szWhere;
		udPBase obj1 = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( ) + " AND " +
			config.GetAttrIndex( "type" ) + "=" + toString( udProjectAttach::ATTACH_CLIENT );
		option.SetTable( "ud_attach" );
		option.SetClass( GetProjectAttach );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			sock->SendByte( ( char ) 0 );
		} else if ( arr.empty( ) ) {
			sock->SendByte( ( char ) 0 );
			g_pApp->GetLog( )->Write( "не найдено приложений\n" );
		} else {
			sock->SendByte( ( char ) 1 ); // есть файлы
			udBinaryData data;
			data.add( arr.size( ) );
			sock->Send( data );
			udInt err = 0;
			udFileTransfer objFileTransfer;
			udPBase obj1 = NULL;
			string log = "\n";
			wstring filename, path,
				client_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\client\\" + obj->get( "id" )->toWString( );
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj1 = *i;
				obj1->dump( log );
				log += "\n";
				filename = obj1->get( "name" )->toWString( );
				path = client_dir + L"\\" + filename;
				objFileTransfer.SetFileName( filename );
				objFileTransfer.SetFilePath( path );
				err = objFileTransfer.Send( sock );
				log += udFileTransfer::ErrorExplain( err ) + "\n";
			}
			__log->Write( log );
		}
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	string CHClient::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectClient;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		//datetime date;
		wstring fio, date;
		//////////////////////////////////////////////////////////////////////////////////////////
		fio = m_pObjFilter->get( "fio" )->toWString( );
		if ( !fio.empty( ) ) {
			string szStr = ConvertString( fio, CP_UTF8 );
			if ( szStr.size( ) ) {
				udDWord	dwLen	= szStr.size( ) * 2;
				udPChar szTmp	= new udChar[ dwLen + 1 ];

				memset( szTmp, 0, dwLen + 1 );
				UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

				szStr = szTmp;

				delete [ ] szTmp;
			}
			string add = config.GetAttrIndex( "fio" ) + " LIKE '";

			int pos = 0;
			for( string::iterator i = szStr.begin( ); i != szStr.end( ); ++i, ++pos ) {
				if ( *i == '*' ) {
					add += '%';
				} else if ( *i == '%' ) {
				} else {
					add += *i;
				}
			}

			add += "'";
			arrString.push_back( add );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//date = m_pObjFilter->get( "date1" )->toTime( );
		date = m_pObjFilter->get( "date1" )->toWString( );
		if ( !date.empty( ) && ( date != L"1900-01-01" ) ) {
		//if ( date.get( ) > 0 ) {
			//arrString.push_back( config.GetAttrIndex( "born" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
			arrString.push_back( config.GetAttrIndex( "born" ) + ">='" + toString( date ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//date = m_pObjFilter->get( "date2" )->toTime( );
		date = m_pObjFilter->get( "date2" )->toWString( );
		if ( !date.empty( ) && ( date != L"1900-01-01" ) ) {
			arrString.push_back( config.GetAttrIndex( "born" ) + "<='" + toString( date ) + "'" );
		}
		/*if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "born" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

	void CHClient::SchemeCenter( udInt opId, udPSocket sock ) {
		if ( opId > opidGet ) {
			if ( opId == opidGet2 ) {
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				//////////////////////////////////////////////////////////////////////////////////////
				udPPager pager = new udPager;
				string szOrder, szOrderBy, szOrderType = "DESC";
				udInt iOrderType = 0;
				//////////////////////////////////////////////////////////////////////////////////////
				// номер страницы
				udType num( ( udInt ) 0 );
				num.read( sock );
				pager->SetPage( num.toInt( ) );
				// размер страницы
				num.read( sock );
				pager->SetPageSize( num.toInt( ) );
				// атрибут упорядочивания
				udType order( string( "" ) );
				order.read( sock );
				szOrderBy = order.toString( );
				// тип упорядочивания
				num.read( sock );
				iOrderType = num.toInt( );
				if ( iOrderType == 1 ) {
					szOrderType = "ASC";
				}
				// набор ID клиентов
				udType ids_num( ( udUInt ) 0 );
				ids_num.read( sock );
				g_pApp->GetLog( )->Write( "ids_num=" + ids_num.toString( ) + "\n" );
				map< udUInt, udUInt > ids;
				udUInt cnt1 = ids_num.toUInt( );
				for( udUInt i = 0; i < cnt1; ++i ) {
					if ( ids_num.read( sock ) == 1 ) {
						ids[ ids_num.toUInt( ) ] = 1;
					}
				}
				// фильтр
				m_pObjFilter->read( sock );
				//
				udPBase obj = m_fnGetObject( );
				udBaseConfig config;
				obj->GetConfig( config );
				udDELETE( obj );
				szOrder = config.GetAttrIndex( szOrderBy ) + " " + szOrderType;
				string szWhere = this->GetWhereCondition( );
				udBinaryData data;
				udArrBase arr;
				if ( ids.empty( ) ) {
					data.add( ( char ) 0 );
				} else {
					udArrString arrString;
					for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
						arrString.push_back( toString( i->first ) );
					}
					if ( szWhere.empty( ) ) {
						szWhere = config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
					} else {
						szWhere += " AND " + config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
					}

					config.Clear( );
					//////////////////////////////////////////////////////////////////////////////////
					udPHandler handler = g_pApp->GetHandler( );
					udHandlerOption option;
					udArrError err;

					option.SetTable( m_szTableName );
					option.SetClass( m_fnGetObject );
					if ( !szOrder.empty( ) ) {
						option.SetOrder( szOrder );
					}
					option.SetWhere( szWhere );
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
					//////////////////////////////////////////////////////////////////////////////////
				}
				//////////////////////////////////////////////////////////////////////////////////////
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				char code = 0;
				sock->ReadByte( &code );
				//////////////////////////////////////////////////////////////////////////////////////
				delete pager;
				ClearArrBase( arr );
			} else if ( opId == opidGet3 ) {
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				//////////////////////////////////////////////////////////////////////////////////////
				// набор ID клиентов
				udType ids_num( ( udUInt ) 0 );
				ids_num.read( sock );
				map< udUInt, udUInt > ids;
				udUInt cnt1 = ids_num.toUInt( );
				for( udUInt i = 0; i < cnt1; ++i ) {
					if ( ids_num.read( sock ) == 1 ) {
						ids[ ids_num.toUInt( ) ] = 1;
					}
				}
				//
				udPBase obj = m_fnGetObject( );
				udBaseConfig config;
				obj->GetConfig( config );
				udDELETE( obj );
				string szWhere = "";
				if ( !ids.empty( ) ) {
					udArrString arrString;
					for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
						arrString.push_back( toString( i->first ) );
					}
					szWhere = config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
				}
				config.Clear( );
				//////////////////////////////////////////////////////////////////////////////////////
				udPHandler handler = g_pApp->GetHandler( );
				udHandlerOption option;
				udArrError err;
				udArrBase arr;
				udBinaryData data;

				option.SetTable( m_szTableName );
				option.SetClass( m_fnGetObject );
				option.SetWhere( szWhere );
				udLongLong cnt = handler->CountUdObject( option );
				//if ( !this->CustomLoadObjects( pager, arr, err, szOrderBy, iOrderType, cnt ) ) {
				handler->GetUdObject( arr, option, &err );
				//}
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
					data.add( ( char ) 0 );
				} else {
					data.add( ( char ) 1 );
					data.add( ( udUInt ) cnt );
					this->AfterLoadList( arr );
					WriteInstances( arr, data );
				}
				//////////////////////////////////////////////////////////////////////////////////////
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				char code = 0;
				sock->ReadByte( &code );
				//////////////////////////////////////////////////////////////////////////////////////
				ClearArrBase( arr );
			} else {
				udStateHandlerSimple::SchemeCenter( opId, sock );
			}
		} else {
			udStateHandlerSimple::SchemeCenter( opId, sock );
		}
	} // void SchemeCenter

} // namespace UdSDK
