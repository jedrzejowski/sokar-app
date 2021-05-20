//
// Created by adam on 03.05.2021.
//

#include "./easyVolumeTest.h"

#include <QObject>
#include "../../3d/VulkanWidget.h"
#include "../../3d/StaticMesh.hpp"
#include "../../3d/MeshPipeline.h"
#include "../../3d/CenterCamera.h"
#include "../../3d/VulkanRenderer.h"
#include "../MeshSimplification/VertexClustering.h"
#include "../MarchingCubes.h"
#include "../Volume/VolumeEnv.h"
#include "../Volume/DicomVolume.h"
#include "../IndexedMesh.h"


void easyVolumeTest(SokarAlg::DicomVolume *vv) {

	auto mc = new SokarAlg::MarchingCubes();
	mc->setVirtualVolume(vv);
	vv->setCubesPerMM(0.25f);
//	vv->setInterpolator(new SokarAlg::NearestVolumeInterpolator());
	vv->setInterpolator(new SokarAlg::LinearValueInterpolator());
//	vv->setInterpolator(new SokarAlg::PolynomialVolumeInterpolator1());
//	vv->setInterpolator(new SokarAlg::PolynomialValueInterpolator2());
//	vv->setInterpolator(new SokarAlg::AkimaVolumeInterpolator());
//	vv->setInterpolator(new SokarAlg::CubicVolumeInterpolator(false));
//	vv->setInterpolator(new SokarAlg::CubicVolumeInterpolator(true));
	mc->setIsoLevel({100.f, 200.f});

	auto future = mc->exec();

	auto watcher = new QFutureWatcher<void>();

	QObject::connect(watcher, &QFutureWatcherBase::finished, [mc, vv]() {
		qDebug() << "here";
		auto mesh = mc->getMesh();

//		auto simplifier = new SokarAlg::VertexClustering({0, 0, 0}, {10, 10, 10});
//		auto f2 = simplifier->simplify(mesh);
//		f2.waitForFinished();
//		mesh = f2.result();

		auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();
		auto renderer = ret.renderer;

		auto vvSize = vv->getSize();

		auto camera = new Sokar3D::CenterCamera(
				glm::vec3(vvSize) * 0.5f,
				std::max(std::max(vvSize.x, vvSize.y), vvSize.z) * 2.f
		);
		renderer->setCamera(camera);

//		auto mesh2 = mesh->toIndexedStaticMesh();

//		qDebug() << mesh->verticesCount();
//		qDebug() << mesh2->indexCount() << mesh2->verticesCount();

		auto pipeline = new Sokar3D::MeshPipeline(mesh->toStaticMash());
		renderer->addPipelineWrapper(pipeline);
	});

	watcher->setFuture(future);
}