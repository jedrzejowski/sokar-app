//
// Created by adam on 30.04.2021.
//

#pragma once

#include <QtCore>
#include <QObject>
#include "3d/VulkanWidget.h"

void sokarCubeTest(){

	auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();

	auto mesh = Sokar3D::StaticMesh::createCubeMesh();

	auto meshpw = new Sokar3D::MeshPipeline(mesh);

	auto renderer = ret.renderer;
	renderer->addPipelineWrapper(meshpw);
	auto camera = new Sokar3D::CenterCamera(
			{0.5, 0.5, 0.5},
			10
	);
	renderer->setCamera(camera);

	auto vv = new SokarAlg::DicomVolume();
	vv->setSceneSet(getDicomView()->getDicomSceneSet());
	auto mc = new SokarAlg::MarchingCubes();
	mc->setVirtualVolume(vv);
	mc->setIsoLevel(200);

	auto future = mc->marchingCubes();

	auto watcher = new QFutureWatcher<void>();

	QObject::connect(watcher, &QFutureWatcherBase::finished, [mc, vv]() {
		auto size = vv->getSize();
		qDebug() << "end with " << mc->getTriangles().size();
		qDebug() << std::max(std::max(size.x, size.y), size.z);

		auto mesh = mc->toStaticMesh();

		auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();
		auto meshpw = new Sokar3D::MeshPipeline(mesh);

		auto renderer = ret.renderer;
		renderer->addPipelineWrapper(meshpw);

		auto camera = new Sokar3D::CenterCamera(
				glm::vec3(size.x / 2, size.y / 2, size.z / 2),
				std::max(std::max(size.x, size.y), size.z) * 10
		);

		renderer->setCamera(camera);

		ret.renderer->addPipelineWrapper(meshpw);
	});

}