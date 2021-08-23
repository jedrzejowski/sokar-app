//
// Created by adam on 12.06.2021.
//

#pragma once

#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QSharedPointer>
#include "SokarAlg.hpp"
#include "SokarLib/Displayable.hpp"

namespace SokarAlg {

    template<typename Output>
    class Algorithm : public QEnableSharedFromThis<Algorithm<Output>>, public SokarLib::Displayable {
    protected:

        virtual void execBefore() {};
        virtual Output exec() = 0;
        virtual void execAfter() {};

    public:
        [[nodiscard]]
        Output execSync();

        [[nodiscard]]
        QFuture<Output> execAsync();
    };

    template<typename Output>
    Output Algorithm<Output>::execSync() {

        execBefore();
        auto output = exec();
        execAfter();
        return output;
    }

    template<typename Output>
    QFuture<Output> Algorithm<Output>::execAsync() {

        auto self = this->sharedFromThis();

        return QtConcurrent::run([self]() {
            return self->execSync();
        });
    }
}



