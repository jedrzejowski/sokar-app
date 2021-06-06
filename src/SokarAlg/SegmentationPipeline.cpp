//
// Created by adam on 20.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "SegmentationPipeline.hpp"
#include "MarchingCubes.hpp"
#include "IndexedMesh.hpp"
#include "ExampleVolume.hpp"
#include "CachedVolume.hpp"

using namespace SokarAlg;

QFuture<SegmentationResult> SegmentationPipeline::executePipeline() {
	return QtConcurrent::run([&]() -> SegmentationResult {
		QSharedPointer<const Volume> volume;
		SegmentationResult result;
		result.timeStarted = makeTimePoint();

		auto interpolator = QSharedPointer<LinearValueInterpolator>::create();

		auto dicomVolume = QSharedPointer<DicomVolume>::create();
		dicomVolume->setRawDicomVolume(rawDicomVolume);
		dicomVolume->setCubesPerMM(1.f);
		dicomVolume->setInterpolator(interpolator);

		volume = dicomVolume;

//		volume = ExampleVolume::Sphere(50, 20);

		//region caching

		result.timePreCache = makeTimePoint();
		if (usingCache) {
			qDebug() << "caching ...";
			auto cachedVolume = QSharedPointer<CachedVolume>::create();
			cachedVolume->setVolume(volume, true);
			volume = cachedVolume;
		}
		result.timePostCache = makeTimePoint();

		//endregion

		auto mc = new SokarAlg::MarchingCubes();
		mc->setVolume(volume);
//	vv->setInterpolator(new SokarAlg::NearestVolumeInterpolator());
//		vv->setInterpolator(new SokarAlg::LinearValueInterpolator());
//	vv->setInterpolator(new SokarAlg::PolynomialVolumeInterpolator1());
//	vv->setInterpolator(new SokarAlg::PolynomialValueInterpolator2());
//	vv->setInterpolator(new SokarAlg::AkimaVolumeInterpolator());
//	vv->setInterpolator(new SokarAlg::CubicVolumeInterpolator(false));
//	vv->setInterpolator(new SokarAlg::CubicVolumeInterpolator(true));
		mc->setIsoLevel({100.f, 9999999.f});
//		mc->setIsoLevel({0.5f, 20000.f});

		//region marching
		qDebug() << "marching ...";

		mc->execSync();

		//endregion

		auto mesh = mc->getMesh();

//		auto simplifier = new SokarAlg::VertexClustering({0, 0, 0}, {10, 10, 10});
//		auto f2 = simplifier->simplify(mesh);
//		f2.waitForFinished();
//		mesh = f2.result();


//		auto mesh2 = mesh->toIndexedStaticMesh();

//		qDebug() << mesh->verticesCount();
//		qDebug() << mesh2->indexCount() << mesh2->verticesCount();

//		result.mesh = mesh->toStaticMash();
		result.mesh = mesh;

		result.timeEnded = makeTimePoint();

		result.proposeCameraCenter = glm::vec3(volume->getSize()) / 2.f;
		result.proposeCameraDistance = glm::length(result.proposeCameraCenter) * 2;

		return result;
	});
}
