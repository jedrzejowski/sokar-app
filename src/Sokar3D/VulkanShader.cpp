//
// Created by adam on 29.03.2021.
//

#include "VulkanShader.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>
#include <QVulkanDeviceFunctions>

using namespace Sokar3D;

void VulkanShader::load(QVulkanInstance *inst, VkDevice dev, const QString &fn) {
	reset();
	QFile f(fn);

	if (!f.open(QIODevice::ReadOnly)) {
		qWarning("Failed to open %s", qPrintable(fn));
		return;
	}
	QByteArray blob = f.readAll();

	VkShaderModuleCreateInfo shaderInfo;
	memset(&shaderInfo, 0, sizeof(shaderInfo));
	shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.codeSize = blob.size();
	shaderInfo.pCode = reinterpret_cast<const uint32_t *>(blob.constData());
	VkResult err = inst->deviceFunctions(dev)->vkCreateShaderModule(dev, &shaderInfo, nullptr, &shaderModule);

	if (err != VK_SUCCESS) {
		qWarning("Failed to create shader module: %d", err);
	}
}

void VulkanShader::reset() {

}

VkShaderModule_T *VulkanShader::getShaderModule() const {
	return shaderModule;
}
