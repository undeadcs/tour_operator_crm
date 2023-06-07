/**
	UndeadCS SDK
	Базовая библиотека

	типы данных базовой библиотеки
*/
#ifndef UNDEADCS_SDK_LIBRARY_DATA_TYPES_H
#define UNDEADCS_SDK_LIBRARY_DATA_TYPES_H

namespace UdSDK {

	typedef char				udChar, *udPChar, **udPPChar;
	typedef unsigned char		udUChar, *udPUChar, **udPPUChar,
								udByte, *udPByte, **udPPByte;
	typedef wchar_t				udWChar, *udPWChar, **udPPWChar;
	typedef short				udShort, *udPShort, **udPPShort;
	typedef unsigned short		udUShort, *udPUShort, **udPPUShort,
								udWord, *udPWord, **udPPWord;
	typedef int					udInt, *udPInt, **udPPInt;
	typedef unsigned			udUInt, *udPUInt, **udPPUInt;
	typedef long				udLong, *udPLong, **udPPLong;
	typedef unsigned long		udULong, *udPULong, **udPPULong,
								udDWord, *udPDWord, **udPPDWord;
	typedef long long			udLongLong, *udPLongLong, **udPPLongLong,
								udInt64, *udPInt64, **udPPInt64;
	typedef unsigned long long	udULongLong, *udPULongLong, **udPPULongLong,
								udQWord, *udPQWord, **udPPQWord;
	typedef float				udFloat, *udPFloat, **udPPFloat;
	typedef double				udDouble, *udPDouble, **udPPDouble;
	typedef void				udVoid, *udPVoid, **udPPVoid;

	typedef class CBinaryData	udBinaryData, *udPBinaryData, **udPPBinaryData;
	typedef class CBinaryDataIterator	udBinaryDataIterator, *udPBinaryDataIterator, **udPPBinaryDataIterator;
	typedef class CUdSocketInfo	udSocketInfo, *udPSocketInfo, **udPPSocketInfo;
	typedef class CUdSocket		udSocket, *udPSocket, **udPPSocket;
	typedef class CDatetime		udDatetime, *udPDatetime, **udPPDatetime,
								datetime, *pdatetime, **ppdatetime;
	typedef class CUdType		udType, *udPType, **udPPType;
	typedef class CUdMorph		udMorph, *udPMorph, **udPPMorph;
	typedef class CUdFileTransfer	udFileTransfer, *udPFileTransfer, **udPPFileTransfer;
	typedef class CUdXmlNode	udXmlNode, *udPXmlNode, **udPPXmlNode;
	typedef class CUdXmlAttr	udXmlAttr, *udPXmlAttr, **udPPXmlAttr;
	typedef class CUdXmlDoc		udXmlDoc, *udPXmlDoc, **udPPXmlDoc;
	typedef class CUdXmlAttrList	udXmlAttrList, *udPXmlAttrList, **udPPXmlAttrList;
	typedef class CUdXmlNodeList	udXmlNodeList, *udPXmlNodeList, **udPPXmlNodeList;

	typedef FILE				udStdFile, *udPStdFile, **udPPStdFile;

	typedef pair< string,	udPType			>	udPairType, *udPPairType;
	typedef list< udPairType				>	udLstPairType, *udPLstPairType;
	typedef map< string,	udPType			>	udAscType, *udPAscType;
	typedef map< string,	udPMorph		>	udAscMorph, *udPAscMorph;
	typedef map< string, udPSocket			>	udAscSocket, *udPAscSocket;
	typedef map< udDWord, udPSocket			>	udDwAscSocket, *udPDwAscSocket;
	typedef vector< udXmlAttr				>	udArrXmlAttr, *udPArrXmlAttr;
	typedef vector< udPXmlAttr				>	udArrPXmlAttr, *udPArrPXmlAttr;
	typedef vector< udXmlNode				>	udArrXmlNode, *udPArrXmlNode;
	typedef vector< udPXmlNode				>	udArrPXmlNode, *udPArrPXmlNode;
	typedef map< udDWord, udXmlAttr			>	udDwAscXmlAttr, *udPDwAscXmlAttr;
	typedef map< udDWord, udPXmlAttr		>	udDwAscPXmlAttr, *udPDwAscPXmlAttr;
	typedef map< udDWord, udXmlNode			>	udDwAscXmlNode, *udPDwAscXmlNode;
	typedef map< udDWord, udPXmlNode		>	udDwAscPXmlNode, *udPDwAscPXmlNode;
	typedef vector< string					>	udArrString, *udPArrString;
	typedef vector< wstring					>	udArrWString, *udPArrWString;

} // namespace UdSDK

#endif
