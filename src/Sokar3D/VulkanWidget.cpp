//
// Created by adam on 29.03.2021.
//

#include "./VulkanWidget.hpp"
#include "./VulkanRenderer.h"
#include <QMouseEvent>

using namespace Sokar3D;

VulkanWidget::VulkanWidget() {
}

VulkanWidget::~VulkanWidget() {
}

QVulkanWindowRenderer *VulkanWidget::createRenderer() {
	return renderer;
}

bool VulkanWidget::event(QEvent *event) {
	if (renderer != nullptr && renderer->uiEvent(event)) {
		return true;
	}

	return QVulkanWindow::event(event);
}
