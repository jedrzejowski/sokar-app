//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QVulkanInstance>
#include <QFuture>
#include "Sokar3D.hpp"


namespace Sokar3D {

	class VulkanShader {
		VkShaderModule shaderModule = VK_NULL_HANDLE;
		QVulkanInstance *vkQInst = nullptr;
		VkDevice vkDevice = VK_NULL_HANDLE;

	public:
		void load(QVulkanInstance *vkQInst, VkDevice vkDevice, const QString &fn);

		[[nodiscard]]
		inline bool isValid() {
			return shaderModule != VK_NULL_HANDLE;
		}

		void release();

		[[nodiscard]]
		VkShaderModule_T *getShaderModule() const;
	};
}