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
		  dicomVolume(QSharedPointer<DicomVolume>::create()),
		  volumeInterpolator(QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create()),
		  volumeSegmentator(QSharedPointer<SokarAlg::MarchingCubes>::create()) {
}

QFuture<QSharedPointer<const SegmentationResult>> SegmentationPipeline::executePipeline() {
	return QtConcurrent::run([&]() -> QSharedPointer<const SegmentationResult> {
		QSharedPointer<const Volume> volume;
		auto result = QSharedPointer<SegmentationResult>::create();
		result->timeStarted = makeTimePoint();

		// volume

		dicomVolume->setRawDicomVolume(rawDicomVolume);
		dicomVolume->setInterpolator(volumeInterpolator);

		volume = dicomVolume;

		//endregion

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


		//region marching
		qDebug() << "marching ...";

		volumeSegmentator->setIsoLevel({0.5f, 20000.f});
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

const QColor &SegmentationPipeline::getColor() const {
	return color;
}

void SegmentationPipeline::setColor(const QColor &newColor) {
	color = newColor;
}

const QSharedPointer<const RawDicomVolume> &SegmentationPipeline::getRawDicomVolume() const {
	return rawDicomVolume;
}

void SegmentationPipeline::setRawDicomVolume(const QSharedPointer<const RawDicomVolume> &newRawDicomVolume) {
	rawDicomVolume = newRawDicomVolume;
}

const QSharedPointer<DicomVolume> &SegmentationPipeline::getDicomVolume() const {
	return dicomVolume;
}

void SegmentationPipeline::setDicomVolume(const QSharedPointer<DicomVolume> &newDicomVolume) {
	dicomVolume = newDicomVolume;
}

const QSharedPointer<VolumeInterpolator> &SegmentationPipeline::getVolumeInterpolator() const {
	return volumeInterpolator;
}

void SegmentationPipeline::setVolumeInterpolator(const QSharedPointer<VolumeInterpolator> &newVolumeInterpolator) {
	volumeInterpolator = newVolumeInterpolator;
}

const QSharedPointer<VolumeSegmentator> &SegmentationPipeline::getVolumeSegmentator() const {
	return volumeSegmentator;
}

void SegmentationPipeline::setVolumeSegmentator(const QSharedPointer<VolumeSegmentator> &newVolumeSegmentator) {
	volumeSegmentator = newVolumeSegmentator;
}

const QSharedPointer<MeshSimplificator> &SegmentationPipeline::getMeshSimplificator() const {
	return meshSimplificator;
}

void SegmentationPipeline::setMeshSimplificator(const QSharedPointer<MeshSimplificator> &newMeshSimplificator) {
	meshSimplificator = newMeshSimplificator;
}
