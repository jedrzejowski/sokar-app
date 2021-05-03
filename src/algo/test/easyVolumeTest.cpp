//
// Created by adam on 03.05.2021.
//

#include "./easyVolumeTest.h"

#include <QtCore>
#include <QObject>
#include "../../3d/VulkanWidget.h"
#include "../../3d/StaticMesh.h"
#include "../../3d/MeshPipeline.h"
#include "../../3d/CenterCamera.h"
#include "../../3d/VulkanRenderer.h"
#include "../ExampleVolume.h"
#include "../MarchingCubes.h"


void easyVolumeTest(const SokarAlg::VirtualVolume *vv) {

	auto mc = new SokarAlg::MarchingCubes();
	mc->setVirtualVolume(vv);
	mc->setIsoLevel(400);

	auto future = mc->exec();

	auto watcher = new QFutureWatcher<void>();

	QObject::connect(watcher, &QFutureWatcherBase::finished, [mc, vv]() {
		qDebug() << "here";
		auto mesh = mc->toStaticMesh();

		auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();
		auto renderer = ret.renderer;

		auto camera = new Sokar3D::CenterCamera(
				{50, 50, 50},
				100
		);
		renderer->setCamera(camera);

		auto pipeline = new Sokar3D::MeshPipeline(mesh);
		renderer->addPipelineWrapper(pipeline);
	});

	watcher->setFuture(future);

}