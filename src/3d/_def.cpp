//
// Created by adam on 11.04.2021.
//

#include "./_def.h"
#include <QLoggingCategory>
#include <QVulkanInstance>

QVulkanInstance *Sokar3D::getVulkanInstance() {

	class SokarQVkInstance {
	public:
		QVulkanInstance instance;

		SokarQVkInstance(){
			const bool dbg = qEnvironmentVariableIntValue("QT_VK_DEBUG");

			if (dbg) {
				QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

				instance.setLayers(QByteArrayList()
							   << "VK_LAYER_GOOGLE_threading"
							   << "VK_LAYER_LUNARG_parameter_validation"
							   << "VK_LAYER_LUNARG_object_tracker"
							   << "VK_LAYER_LUNARG_core_validation"
							   << "VK_LAYER_LUNARG_image"
							   << "VK_LAYER_LUNARG_swapchain"
							   << "VK_LAYER_GOOGLE_unique_objects");
			}

			if (!instance.create())
				qFatal("Failed to create Vulkan instance: %d", instance.errorCode());
		}
	};


	static auto middleware = new SokarQVkInstance();

	return &middleware->instance;
}