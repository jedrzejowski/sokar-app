#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarScene.hpp"

namespace SokarScene {

    class Scene : public QGraphicsScene {
    Q_OBJECT
    private:
        QVector<Indicator *> indicators;

    public:

        explicit Scene(QObject *parent = nullptr);

        ~Scene() override;

        /**
         * Dodaje @class{SceneIndicator}
         * @param indicator
         */
        void addIndicator(Indicator *indicator);

        void removeIndicator(Indicator *indicator);

        virtual void reposItems();

    protected:

        Indicator *findIndicatorByChild(QGraphicsItem *item);

    };
}