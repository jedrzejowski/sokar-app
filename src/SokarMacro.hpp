//
// Created by adam on 09.06.2021.
//

#pragma once

#include <iostream>
#include <QDebug>

// https://en.cppreference.com/w/cpp/utility/source_location

#define sokarTrace() qDebug() << this << __PRETTY_FUNCTION__

#define declareSharedClass(classname) \
class classname; \
using classname##Ptr = QSharedPointer<classname>; \
using classname##CPtr = QSharedPointer<const classname>; \
using classname##WPtr = QWeakPointer<classname>;
