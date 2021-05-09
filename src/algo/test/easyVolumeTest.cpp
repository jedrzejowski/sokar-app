//
// Created by adam on 03.05.2021.
//

#include "./easyVolumeTest.h"

#include <QObject>
#include "../../3d/VulkanWidget.h"
#include "../../3d/StaticMesh.h"
#include "../../3d/MeshPipeline.h"
#include "../../3d/CenterCamera.h"
#include "../../3d/VulkanRenderer.h"
#include "../MarchingCubes.h"
#include "../VolumeEnv.h"


void easyVolumeTest(SokarAlg::VirtualVolume *vv) {

	auto mc = new SokarAlg::MarchingCubes();
	mc->setVirtualVolume(vv);
	mc->setCubeSize(glm::vec3(0.25f));
	mc->setIsoLevel(75.f);

	auto future = mc->exec();

	auto watcher = new QFutureWatcher<void>();

	QObject::connect(watcher, &QFutureWatcherBase::finished, [mc, vv]() {
		qDebug() << "here";
		auto mesh = mc->toStaticMesh();

		auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();
		auto renderer = ret.renderer;

		auto vvSize = vv->getSize();

		auto camera = new Sokar3D::CenterCamera(
				glm::vec3(vvSize) * 0.5f,
				std::max(std::max(vvSize.x, vvSize.y), vvSize.z) * 2.f
		);
		renderer->setCamera(camera);

		auto pipeline = new Sokar3D::MeshPipeline(mesh);
		renderer->addPipelineWrapper(pipeline);
	});

	watcher->setFuture(future);
}