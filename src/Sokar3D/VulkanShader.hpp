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

	public:
		void load(QVulkanInstance *inst, VkDevice dev, const QString &fn);

		[[nodiscard]]
		inline bool isValid() {
			return shaderModule != VK_NULL_HANDLE;
		}

		void reset();

		VkShaderModule_T *getShaderModule() const;
	};
}