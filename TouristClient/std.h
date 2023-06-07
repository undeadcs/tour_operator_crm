
#ifndef STD_H
#define STD_H

#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

namespace UdSDK {

// пары
typedef pair< udUInt,	udUInt			>	udPairUIntUInt, udPagePair;

// списки
typedef list< udPairStrWstr				>	udLstStrWstr, *udPLstStrWstr;

// обычные массивы
typedef vector< udStlStr				>	udArrWord;
typedef vector< udPStdFile				>	udArrPStdFile;
typedef vector< udPairStrWstr			>	udArrStrWstr, *udPArrStrWstr;

// ассоциативные массивы
typedef map< udWord, HWND				>	udWMapHWnd;
typedef map< udDWord, udDWord			>	udMapDWord;
typedef map< string, udError		>	udAssocError;
typedef map< string, HWND				>	udAscHWnd, *udPAscHWnd;
typedef map< string, string				>	udAscStr, *udPAscStr;
typedef map< string, udDWord			>	udAssocDWord;
typedef map< string, udPArrBase			>	udAssocArrBase, *udPAssocArrBase;
typedef map< wstring, udPWndClass		>	udAssocWndClass, *udPAssocWndClass;

}

#endif
