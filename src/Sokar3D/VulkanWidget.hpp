//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QWidget>
#include <QVulkanWindow>
#include <QFutureWatcher>
#include "./Sokar3D.hpp"

namespace Sokar3D {

    template<class T>
    struct VulkanWidgetReturn {
        QWidget *widget;
        VulkanWidget *vulkanWidget;
        T *vulkanRenderer;
    };

    class VulkanWidget : public QVulkanWindow {
        VulkanRenderer *renderer = nullptr;

    protected:
        bool event(QEvent *event) override;

    public:
        VulkanWidget();
        ~VulkanWidget() override;
        QVulkanWindowRenderer *createRenderer() override;

        bool isDebugEnabled() const { return true; }

        template<class T>
        static VulkanWidgetReturn<T> New(QVulkanInstance *inst = getVulkanInstance()) {

            auto *widget = new VulkanWidget();
            widget->setVulkanInstance(inst);
            auto renderer = new T(widget);
            widget->renderer = renderer;

            QWidget *wrapper = QWidget::createWindowContainer(widget);
            wrapper->setFocusPolicy(Qt::StrongFocus);

            return {wrapper, widget, renderer};
        }
    };
}



