//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QWidget>
#include <QVulkanWindow>
#include <QFutureWatcher>
#include "./_def.h"

namespace Sokar3D {

	class VulkanWidget : public QVulkanWindow {
		QVulkanWindowRenderer *renderer = nullptr;

	public:
		VulkanWidget();
		~VulkanWidget() override;
		QVulkanWindowRenderer *createRenderer() override;


		bool isDebugEnabled() const { return true; }

		template<class T>
		static QWidget *New(QVulkanInstance *inst = getVulkanInstance()) {

			auto *window = new VulkanWidget();
			window->setVulkanInstance(inst);
			window->renderer = new T(window);

			QWidget *wrapper = QWidget::createWindowContainer(window);
			wrapper->setFocusPolicy(Qt::StrongFocus);

			window->resize(1024, 768);
			wrapper->show();

			return wrapper;
		}
	};
}



