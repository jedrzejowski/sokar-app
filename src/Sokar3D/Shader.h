//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QVulkanInstance>
#include <QFuture>
#include "Sokar3D.hpp"


namespace Sokar3D {

	struct ShaderData {

		[[nodiscard]]
		inline bool isValid() const {
			return shaderModule != VK_NULL_HANDLE;
		}

		VkShaderModule shaderModule = VK_NULL_HANDLE;
	};

	class Shader {
	public:
		void load(QVulkanInstance *inst, VkDevice dev, const QString &fn);

		ShaderData *data();

		[[nodiscard]]
		inline bool isValid() {
			return data()->isValid();
		}

		void reset();

	private:
		bool m_maybeRunning = false;
		QFuture<ShaderData> m_future;
		ShaderData m_data;
	};
}