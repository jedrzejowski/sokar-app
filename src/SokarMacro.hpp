//
// Created by adam on 09.06.2021.
//

#pragma once

#include <iostream>
#include <QDebug>

// https://en.cppreference.com/w/cpp/utility/source_location

#define sokarTrace() qDebug() << this << __PRETTY_FUNCTION__
