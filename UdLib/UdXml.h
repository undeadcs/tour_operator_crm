/**
	UndeadCS SDK
	Базовая библиотека

	реализация xml
*/
#ifndef UDNEADCS_SDK_LIBRARY_XML_H
#define UDNEADCS_SDK_LIBRARY_XML_H

enum udXmlError {
	udXmlErrOk,				// ошибок нет
	udXmlErrInvalidValue,	// неверное значение
};

enum udXmlInsert {
	InsAtAfter,
	InsAtBefore,
};

#include "UdXmlAttr.h"
#include "UdXmlNode.h"
#include "UdXmlDoc.h"
#include "UdXmlAttrList.h"
#include "UdXmlNodeList.h"

#endif
