//
// Created by adam on 12.06.2021.
//

#pragma once

#include <QString>
#include <QFile>
#include <QTextStream>

namespace SokarLib {
    class SaveableAsText {
    public:
        void save2file(QFile &file) const;
        virtual void dump2stream(QTextStream &stream) const = 0;
    };
}



