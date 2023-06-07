
#ifndef STD_H
#define STD_H

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

}

#endif
