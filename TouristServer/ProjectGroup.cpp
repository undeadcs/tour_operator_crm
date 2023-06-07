
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	// глупо, но сделаем пока так
	static udUInt CountTourTourist( udUInt id ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udPBase obj = NULL;
		udUInt ret = 0;
		obj = new udProjectTourClient;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_tourclient" );
		option.SetClass( GetProjectTourClient );
		option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + toString( id ) );
		handler->GetUdObject( arr, option );
		ret = arr.size( );
		ClearArrBase( arr );
		return ret;
	} // udUInt CountTourTourist

	static void GroupFilterTour( udArrBase& arr ) {
		g_pApp->GetLog( )->Write( "GroupFilterTour\n" );
		if ( arr.empty( ) ) {
			return;
		}
		//LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectTour, "ud_tour", "tour", "name", "name" );
		udArrBase attrs;
		udPBase obj = NULL;
		// name => name
		obj = new udBase;
		obj->get( "attr1" )->ini( string( "name" ) );
		obj->get( "attr2" )->ini( string( "name" ) );
		obj->get( "type" )->ini( wstring( L"" ) );
		attrs.push_back( obj );
		// name => name
		obj = new udBase;
		obj->get( "attr1" )->ini( string( "date1" ) );
		obj->get( "attr2" )->ini( string( "date1" ) );
		obj->get( "type" )->ini( datetime( ) );
		attrs.push_back( obj );
		// name => name
		obj = new udBase;
		obj->get( "attr1" )->ini( string( "date2" ) );
		obj->get( "attr2" )->ini( string( "date2" ) );
		obj->get( "type" )->ini( datetime( ) );
		attrs.push_back( obj );
		//
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectTour, "ud_tour", "tour", attrs );
		//
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			obj->get( "num" )->ini( CountTourTourist( obj->get( "tour" )->toUInt( ) ) );
			/*obj->get( "name" )->ini(
				obj->get( "name" )->toWString( ) + L" (" + 
				obj->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L" - " +
				obj->get( "date2" )->toTime( ).toWString( "%d.%m.%Y" ) + L") " +
				obj->get( "num" )->toWString( )
			);*/
		}
	} // void GroupFilterTour

	static void GroupFilterPeriod( udArrBase& arr ) {
		g_pApp->GetLog( )->Write( "GroupFilterTour\n" );
		if ( arr.empty( ) ) {
			return;
		}
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "client_" )->ini( wstring( L"" ) );
		}
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectClient, "ud_client", "client", "fio", "client_" );
	} // void GroupFilterPeriod

	static void GroupFilterTplTour( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "tpltour_" )->ini( wstring( L"" ) );
		}
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectTplTour, "ud_tpltour", "tpltour", "name", "tpltour_" );
	} // void GroupFilterTplTour

	static void GroupFilterAttach( udArrBase& arrAdd, udArrBase& arrUpd, udArrBase& arrDel, udPBase obj ) {
		udPAppConfig pAppConfig = g_pApp->GetConfig( );
		wstring tmp_dir = pAppConfig->get( "temporary_dir" )->toWString( ),
			client_dir = pAppConfig->get( "files_dir" )->toWString( ) + L"\\group",
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
	} // void GroupFilterAttach

	udPBase GetProjectGroup2( ) {
		udPBase ret = GetProjectGroup( );
		//ret->get( "tpltour_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGroup2

	CHGroup::CHGroup( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_group";
		m_szUpdIndex	= "id";
		m_szName		= "group";
		m_fnGetObject	= GetProjectGroup;

		this->AddManagedTable( "ud_grouptour", "id", GetProjectGroupTour );
		this->AddManagedTable( "ud_groupperiod", "id", GetProjectGroupPeriod );
		this->AddManagedTable( "ud_attach", "id", GetProjectAttach );
		this->AddManagedTable( "ud_grouptpltour", "id", GetProjectGroupTplTour );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_direct", "direct", "id", GetProjectDirect );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_grouptour", "group", "id", GetProjectGroupTour, "`grouptour_id` ASC", "", GroupFilterTour );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_groupperiod", "group", "id", GetProjectGroupPeriod, "`groupperiod_id` ASC", "", GroupFilterPeriod );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_attach", "obj", "id", GetProjectAttach, "`attach_id` ASC", "`attach_type`=" + toString( udProjectAttach::ATTACH_GROUP ) );//,
			//NULL, NULL, GroupFilterAttach );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_grouptpltour", "group", "id", GetProjectGroupTplTour, "`grouptpltour_id` ASC", "", GroupFilterTplTour );

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		//m_pObjFilter->get( "tpltour" )->ini( ( udUInt ) 0 );
	}

	CHGroup::CHGroup( const CHGroup& objCopy ) {
	}

	CHGroup::~CHGroup( ) {
	}

	CHGroup& CHGroup::operator =( const CHGroup& objCopy ) {
		return *this;
	}

	void CHGroup::Init( ) {
		udStateHandlerSimple::Init( );

		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		//folder = appConfig->get( "temporary_dir" )->toWString( ) + L"\\group";
		//CreateDirectory( folder.data( ), NULL );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\group";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	udInt CHGroup::AddObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::AddObj( obj, save_info ) == 1 ) {
			// дополнительно создаем папку для приложений
			wstring dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\group\\" + obj->get( "id" )->toWString( );
			CreateDirectory( dir.data( ), NULL );
			g_pApp->GetLog( )->Write( "'" + toString( dir ) + "'\n" );
			return 1;
		}
		return 0;
	} // udInt AddObj

	void CHGroup::DeleteLinkedObjects( udPBase obj ) {
		udPDatabase db = g_pApp->GetDatabase( );
		string query = "", szWhere = "";
		udBaseConfig config;
		udPBase obj1;
		udPType type = NULL;

		// туры
		obj1 = new udProjectGroupTour;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "group" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// приложение
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		delete obj1;
		type = new udType( ( udInt ) ( udProjectAttach::ATTACH_GROUP ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) +
			config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
		wstring folder = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
			L"\\group\\" + obj->get( "id" )->toWString( );
		// папку удалить
		DirectoryClear( folder );
		RemoveDirectory( folder.data( ) );

		// периодичность
		obj1 = new udProjectGroupPeriod;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "group" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
	} // void DeleteLinkedObjects

	udInt CHGroup::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	udInt CHGroup::DelObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::DelObj( obj, save_info ) == 1 ) {
			// удаляем связанные объекты
			this->DeleteLinkedObjects( obj );
			return 1;
		}
		return 0;
	} // udInt DelObj

	void CHGroup::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		if ( ( opId == opidAdd1 ) || ( opId == opidUpd1 ) ) {
		//////////////////////////////////////////////////////////////////////////////////////////
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
				wstring dst_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) + L"\\group\\" + obj->get( "id" )->toWString( );
				CreateDirectory( dst_dir.data( ), NULL );
				DirectoryCopyFiles( tmp_dir, dst_dir );
				DirectoryClear( tmp_dir );
				//////////////////////////////////////////////////////////////////////////////////
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void ExtraDataRecieve

	void CHGroup::ExtraDataSending( udPSocket sock, udPBase obj ) {
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
			config.GetAttrIndex( "type" ) + "=" + toString( udProjectAttach::ATTACH_GROUP );
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
				L"\\group\\" + obj->get( "id" )->toWString( );
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

	bool CHGroup::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_group" );
		option.SetClass( GetProjectGroup2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "tpltour" ) {
			//szOrder = "`group_tpltour_` ";
		} else if ( ( orderby == "placenum" ) || ( orderby == "lperiod" ) ) {
		} else {
			szOrder = "`group_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		/*option.AddLeftJoin(
			"name", "group_tpltour_", "ud_tpltour",
			"`ud_tpltour`.`tpltoure_id`=`ud_group`.`group_tpltour`", GetProjectTplTour
		);*/
		//option.SetIgnoreAttr( "tpltour_" );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return true;
	} // bool CustomLoadObjects

	string CHGroup::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectGroup;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "name" )->toWString( );
		if ( !str.empty( ) ) {
			string szStr = ConvertString( str, CP_UTF8 );
			if ( szStr.size( ) ) {
				udDWord	dwLen	= szStr.size( ) * 2;
				udPChar szTmp	= new udChar[ dwLen + 1 ];

				memset( szTmp, 0, dwLen + 1 );
				UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

				szStr = szTmp;

				delete [ ] szTmp;
			}

			string add = config.GetAttrIndex( "name" ) + " LIKE '";

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
		/*id = m_pObjFilter->get( "tpltour" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "tpltour" ) + "=" + toString( id ) );
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

	void CHGroup::SchemeCenter( udInt opId, udPSocket sock ) {
		if ( opId > opidGet ) {
			switch( opId ) {
				case opidGetTouristList:	this->OpGetTouristList( sock );	break;
				default:
					break;
			}
		} else {
			udStateHandlerSimple::SchemeCenter( opId, sock );
		}
	} // void SchemeCenter

	void CHGroup::OpGetTouristList( udPSocket sock ) {
		udType idGroup( ( udUInt ) 0 );

		if ( idGroup.read( sock ) != 1 ) {
			return;
		}

		udType indexPeriod( ( udUInt ) 0 );

		if ( indexPeriod.read( sock ) != 1 ) {
			return;
		}

		// 1. выгребаем туры, которые завязаны на период.
		// 2. выгребаем туристов по каждому туру, составляем список туристов.
		// 3. выгребаем туристов из базы, составляем ответный список

		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrBase arr;
		udArrError err;
		udPLog __log = g_pApp->GetLog( );
		string log = "";
		udBaseConfig config;
		udPBase obj = NULL;

		obj = new udProjectGroupTour;
		obj->GetConfig( config );
		udDELETE( obj );

		option.SetWhere(
			config.GetAttrIndex( "group" ) + "=" + idGroup.toString( ) + " AND " +
			config.GetAttrIndex( "period" ) + "=" + indexPeriod.toString( )
		);
		option.SetTable( "ud_grouptour" );
		option.SetClass( GetProjectGroupTour );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		config.Clear( );
		if ( !err.empty( ) || arr.empty( ) ) {
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		} else {
			map< udUInt, udUInt > ids;
			udUInt id = 0;

			log = "\n";

			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				obj->dump( log );
				log += "\n";
				id = obj->get( "tour" )->toUInt( );
				ids[ id ] = id;
			}

			__log->Write( log );

			ClearArrBase( arr );

			udArrString arrStrId;

			for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
				arrStrId.push_back( toString( i->first ) );
			}

			ids.clear( );

			obj = new udProjectTourClient;
			obj->GetConfig( config );
			udDELETE( obj );

			option.SetWhere( config.GetAttrIndex( "tour" ) + " IN(" + Join( ",", arrStrId ) + ")" );
			arrStrId.clear( );
			option.SetTable( "ud_tourclient" );
			option.SetClass( GetProjectTourClient );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) || arr.empty( ) ) {
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
				}

				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 0 );
			} else {
				log = "\n";

				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					obj->dump( log );
					log += "\n";
					id = obj->get( "client" )->toUInt( );
					ids[ id ] = id;
				}

				__log->Write( log );

				ClearArrBase( arr );

				for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
					arrStrId.push_back( toString( i->first ) );
				}

				obj = new udProjectClient;
				obj->GetConfig( config );
				udDELETE( obj );

				udArrBase arrClient;

				option.SetWhere( config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrStrId ) + ")" );
				arrStrId.clear( );
				option.SetTable( "ud_client" );
				option.SetClass( GetProjectClient );
				handler->GetUdObject( arrClient, option, &err );
				option.Clear( );
				config.Clear( );
				if ( !err.empty( ) || arrClient.empty( ) ) {
					if ( !err.empty( ) ) {
						WriteErrors( __log, err );
					}

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 0 );
				} else {
					log = "\n";

					ids.clear( );

					for( udArrBase::iterator i = arrClient.begin( ); i != arrClient.end( ); ++i ) {
						obj = *i;
						obj->dump( log );
						log += "\n";

						id = obj->get( "doc" )->toUInt( );
						ids[ id ] = id;
					}

					__log->Write( log );

					udArrBase arrDoc;

					for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
						arrStrId.push_back( toString( i->first ) );
					}

					obj = new udProjectClDoc;
					obj->GetConfig( config );
					udDELETE( obj );

					option.SetWhere( config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrStrId ) + ")" );
					option.SetTable( "ud_cldoc" );
					option.SetClass( GetProjectClDoc );
					handler->GetUdObject( arrDoc, option, &err );
					if ( !err.empty( ) ) {
						WriteErrors( __log, err );
					}

					udBinaryData data;
					data.add( ( char ) 80 );

					WriteInstances( arrClient, data );
					WriteInstances( arrDoc, data );

					ClearArrBase( arrClient );

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					data.clear( );
				}
			}
		}
	} // void OpGetTouristList

} // namespace UdSDK
