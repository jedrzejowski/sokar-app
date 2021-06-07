//
// Created by adam on 20.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "SegmentationPipeline.hpp"
#include "MarchingCubes.hpp"
#include "ExampleVolume.hpp"
#include "CachedVolume.hpp"

using namespace SokarAlg;


SegmentationPipeline::SegmentationPipeline()
		: QObject(nullptr),
		  volumeInterpolator(QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create()),
		  volumeSegmentator(QSharedPointer<SokarAlg::MarchingCubes>::create()) {
}

QFuture<QSharedPointer<const SegmentationResult>> SegmentationPipeline::executePipeline() {
	return QtConcurrent::run([&]() -> QSharedPointer<const SegmentationResult> {
		QSharedPointer<const Volume> volume;
		auto result = QSharedPointer<SegmentationResult>::create();
		result->timeStarted = makeTimePoint();


		auto dicomVolume = QSharedPointer<DicomVolume>::create();
		dicomVolume->setRawDicomVolume(rawDicomVolume);
		dicomVolume->setCubesPerMM(cubesPerMm);
		dicomVolume->setInterpolator(volumeInterpolator);

		volume = dicomVolume;

		//region caching

		result->timePreCache = makeTimePoint();
		if (usingCache) {
			qDebug() << "caching ...";
			auto cachedVolume = QSharedPointer<CachedVolume>::create();
			cachedVolume->setVolume(volume, true);
			volume = cachedVolume;
		}
		result->timePostCache = makeTimePoint();

		//endregion

//		mc->setIsoLevel({0.5f, 20000.f});

		//region marching
		qDebug() << "marching ...";

		volumeSegmentator->setVolume(volume);
		volumeSegmentator->execSync();

		//endregion

		auto mesh = volumeSegmentator->getMesh();

//		auto simplifier = new SokarAlg::VertexClustering({0, 0, 0}, {10, 10, 10});
//		auto f2 = simplifier->simplify(mesh);
//		f2.waitForFinished();
//		mesh = f2.result();


//		auto mesh2 = mesh->toIndexedStaticMesh();

//		qDebug() << mesh->verticesCount();
//		qDebug() << mesh2->indexCount() << mesh2->verticesCount();

//		result.mesh = mesh->toStaticMash();
		result->mesh = mesh;

		result->timeEnded = makeTimePoint();

		result->proposeCameraCenter = glm::vec3(volume->getSize()) / 2.f;
		result->proposeCameraDistance = glm::length(result->proposeCameraCenter) * 2;

		return result;
	});
}
