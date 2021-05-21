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
		QWidget *wrapper;
		VulkanWidget *window;
		T *renderer;
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

			auto *window = new VulkanWidget();
			window->setVulkanInstance(inst);
			auto renderer = new T(window);
			window->renderer = renderer;

			QWidget *wrapper = QWidget::createWindowContainer(window);
			wrapper->setFocusPolicy(Qt::StrongFocus);

			window->resize(1024, 768);
			wrapper->show();

			return {wrapper, window, renderer};
		}
	};
}



