//
// Created by adam on 09.06.2021.
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <QDebug>

// https://en.cppreference.com/w/cpp/utility/source_location

#define sokarTrace() qDebug() << this << __PRETTY_FUNCTION__
#define sokarTraceLvl(lvl) sokarTrace()

#define declareSharedClass(classname) \
class classname; \
using classname##Ptr = QSharedPointer<classname>; \
using classname##CPtr = QSharedPointer<const classname>; \
using classname##WPtr = QWeakPointer<classname>;

#define declareSharedClassT1(classname) \
template<typename T>                    \
class classname; \
using classname##Ptr<T> = QSharedPointer<classname<T>>; \
using classname##CPtr<T> = QSharedPointer<const classname<T>>; \
using classname##WPtr<T> = QWeakPointer<classname<T>>;


#define sokarNotImplemented() throw std::runtime_error("not implemented");
