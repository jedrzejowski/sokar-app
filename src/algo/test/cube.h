//
// Created by adam on 30.04.2021.
//

#pragma once

#include <QtCore>
#include <QObject>
#include "../../3d/VulkanWidget.h"
#include "../../3d/StaticMesh.h"
#include "../../3d/MeshPipeline.h"
#include "../../3d/CenterCamera.h"
#include "../../3d/VulkanRenderer.h"
#include "../ExampleVolume.h"
#include "../MarchingCubes.h"

void sokarCubeTest() {


//	auto mesh1 = Sokar3D::StaticMesh::createCubeMesh();
//	auto mesh2 = Sokar3D::StaticMesh::createCubeMesh();

//	auto meshpw1 = new Sokar3D::MeshPipeline(mesh1);
//	auto meshpw2 = new Sokar3D::MeshPipeline(mesh2);

//	auto model1 = glm::mat4(1);
//	model1 = glm::translate(model1, glm::vec3(1, 0, 1));
//
//	meshpw1->setModelMatrix(model1);
//
//	auto model2 = glm::mat4(1);
//	model2 = glm::translate(model2, glm::vec3(-1, 0, -1));
//
//	qDebug() << "model1 = " << glm::to_string(model1).c_str();
//	qDebug() << "model2 = " << glm::to_string(model2).c_str();
//
//	meshpw2->setModelMatrix(model2);
//
//	renderer->addPipelineWrapper(meshpw1);
//	renderer->addPipelineWrapper(meshpw2);



	auto vv = SokarAlg::ExampleVolume::Sphere(100, 25, 1.f, 0.f);
//	auto vv = SokarAlg::ExampleVolume::Cube(glm::u32vec3(100), glm::vec3(25), 1.f, 0.f);

	auto mc = new SokarAlg::MarchingCubes();
	mc->setVirtualVolume(vv);
	mc->setIsoLevel(0.5);

	auto future = mc->exec();

	auto watcher = new QFutureWatcher<void>();

	QObject::connect(watcher, &QFutureWatcherBase::finished, [mc, vv]() {
		qDebug() << "here";
		auto mesh = mc->toStaticMesh();
		qDebug() << mesh->data()->sizeInBytes();

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

//	auto watcher = new QFutureWatcher<void>();
//
//	QObject::connect(watcher, &QFutureWatcherBase::finished, [mc, vv]() {
//		auto size = vv->getSize();
//		qDebug() << "end with " << mc->getTriangles().size();
//		qDebug() << std::max(std::max(size.x, size.y), size.z);
//
//		auto mesh = mc->toStaticMesh();
//
//		auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();
//		auto meshpw = new Sokar3D::MeshPipeline(mesh);
//
//		auto renderer = ret.renderer;
//		renderer->addPipelineWrapper(meshpw);
//
//		auto camera = new Sokar3D::CenterCamera(
//				glm::vec3(size.x / 2, size.y / 2, size.z / 2),
//				std::max(std::max(size.x, size.y), size.z) * 10
//		);
//
//		renderer->setCamera(camera);
//
//		ret.renderer->addPipelineWrapper(meshpw);
//	});
}