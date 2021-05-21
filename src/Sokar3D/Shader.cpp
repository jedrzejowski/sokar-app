//
// Created by adam on 29.03.2021.
//

#include "Shader.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>
#include <QVulkanDeviceFunctions>

using namespace Sokar3D;

void Shader::load(QVulkanInstance *inst, VkDevice dev, const QString &fn) {
	reset();
	m_maybeRunning = true;
	m_future = QtConcurrent::run([inst, dev, fn]() {
		ShaderData sd;
		QFile f(fn);
		if (!f.open(QIODevice::ReadOnly)) {
			qWarning("Failed to open %s", qPrintable(fn));
			return sd;
		}
		QByteArray blob = f.readAll();
		VkShaderModuleCreateInfo shaderInfo;
		memset(&shaderInfo, 0, sizeof(shaderInfo));
		shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderInfo.codeSize = blob.size();
		shaderInfo.pCode = reinterpret_cast<const uint32_t *>(blob.constData());
		VkResult err = inst->deviceFunctions(dev)->vkCreateShaderModule(dev, &shaderInfo, nullptr, &sd.shaderModule);
		if (err != VK_SUCCESS) {
			qWarning("Failed to create shader module: %d", err);
			return sd;
		}
		return sd;
	});
}

ShaderData *Shader::data() {
	if (m_maybeRunning && !m_data.isValid())
		m_data = m_future.result();

	return &m_data;
}

void Shader::reset() {
	*data() = ShaderData();
	m_maybeRunning = false;
}
