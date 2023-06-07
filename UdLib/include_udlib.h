/**
	UndeadCS SDK
	Базовая библиоткеа

	forward - предварительные объявления функций и типов
	tplfunc - шаблонные функции, которые помогают изменять данные, без написания дополнительного кода
	typeconvert - функции конвертации данных из одного типа в другой
	datetime - тип данных дата+время
	UdType - класс типа данных
	UdMorph - класс морфного объекта
	UdSocket - класс работы с сокетом
	UdFileTransfer - класс пересылки файлов
	util - набор вспомогательных функций
*/
#ifndef UNDEADCS_SDK_LIBRARY_H
#define UNDEADCS_SDK_LIBRARY_H

#include "UdLibDataTypes.h"
#include "macro.h"
#include "forward.h"
#include "tplfunc.h"
#include "util.h"
#include "typeconvert.h"
#include "datetime.h"
#include "UdBinaryData.h"
#include "UdBinaryDataIterator.h"
#include "UdType.h"
#include "UdMorph.h"
#include "UdSocketAddress.h"
#include "UdSocketInfo.h"
#include "UdSocket.h"
#include "UdFileTransfer.h"
#include "UdXml.h"

#endif
