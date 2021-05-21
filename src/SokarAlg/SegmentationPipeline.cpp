//
// Created by adam on 20.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "SegmentationPipeline.hpp"
#include "MarchingCubes.hpp"
#include "IndexedMesh.hpp"
#include "ExampleVolume.hpp"

QFuture<QSharedPointer<Sokar3D::StaticMesh>> SokarAlg::SegmentationPipeline::executePipeline() {
	return QtConcurrent::run([&]() -> QSharedPointer<Sokar3D::StaticMesh> {

		auto interpolator = QSharedPointer<SokarAlg::LinearValueInterpolator>(new SokarAlg::LinearValueInterpolator());

		auto dicomVolume = QSharedPointer<SokarAlg::DicomVolume>::create();
		dicomVolume->setRawDicomVolume(rawDicomVolume);
		dicomVolume->setCubesPerMM(0.25f);
		dicomVolume->setInterpolator(interpolator);

		auto vol = QSharedPointer<Volume>(ExampleVolume::Sphere(20, 8));

		auto mc = new SokarAlg::MarchingCubes();
		mc->setVolume(vol);
//	vv->setInterpolator(new SokarAlg::NearestVolumeInterpolator());
//		vv->setInterpolator(new SokarAlg::LinearValueInterpolator());
//	vv->setInterpolator(new SokarAlg::PolynomialVolumeInterpolator1());
//	vv->setInterpolator(new SokarAlg::PolynomialValueInterpolator2());
//	vv->setInterpolator(new SokarAlg::AkimaVolumeInterpolator());
//	vv->setInterpolator(new SokarAlg::CubicVolumeInterpolator(false));
//	vv->setInterpolator(new SokarAlg::CubicVolumeInterpolator(true));
		mc->setIsoLevel({0.5f, 20000.f});

		qDebug() << "start";
		mc->execSync();
		qDebug() << "end";

		auto mesh = mc->getMesh();

//		auto simplifier = new SokarAlg::VertexClustering({0, 0, 0}, {10, 10, 10});
//		auto f2 = simplifier->simplify(mesh);
//		f2.waitForFinished();
//		mesh = f2.result();


//		auto mesh2 = mesh->toIndexedStaticMesh();

//		qDebug() << mesh->verticesCount();
//		qDebug() << mesh2->indexCount() << mesh2->verticesCount();

		return mesh->toStaticMash();
	});
}
