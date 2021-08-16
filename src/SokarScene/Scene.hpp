#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarScene.hpp"

namespace SokarScene {

    class Scene : public QGraphicsScene {
    Q_OBJECT
    private:
        QVector<SceneIndicator *> indicators;

    public:

        explicit Scene(QObject *parent = nullptr);

        ~Scene() override;

        /**
         * Dodaje @class{SceneIndicator}
         * @param indicator
         */
        void addIndicator(SceneIndicator *indicator);

        void removeIndicator(SceneIndicator *indicator);

        virtual void reposItems();

    protected:

        SceneIndicator *findIndicatorByChild(QGraphicsItem *item);

    };
}