//
// Created by adam on 20.05.2021.
//

#pragma once

#include <QJsonObject>

namespace SokarLib {

    class Jsonable {
    public:
        virtual QJsonObject dumpJson() = 0;
        virtual void loadJson(const QJsonObject &json) = 0;
    };
}



