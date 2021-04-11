//
// Created by adam on 29.03.2021.
//

#include "VulkanWidget.h"

using namespace Sokar3D;

VulkanWidget::VulkanWidget() {
}

VulkanWidget::~VulkanWidget() {
}

QVulkanWindowRenderer *VulkanWidget::createRenderer() {
	qDebug() << "HERE";
	return renderer;
}
