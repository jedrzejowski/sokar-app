//
// Created by adam on 13.05.2021.
//

#include <QtConcurrent/QtConcurrent>
#include "VertexClustering.h"


QFuture<Sokar3D::StaticMesh *> SokarAlg::VertexClustering::simplify(const Sokar3D::IndexedStaticMesh *mesh) {
	return QtConcurrent::run([this, mesh]() {

		auto newMesh = new Sokar3D::StaticMesh();
		auto extrema = findExtrema(mesh);

		std::map<glm::u32vec3, std::vector<qsizetype>> clusterMap;
		std::map<qsizetype, qsizetype> oldVertex2newVertexMap;

		// iterujemy po werzchołkach

		for (auto iter = mesh->getVertices().begin(); iter != mesh->getVertices().end(); ++iter) {
//			iter->pos
		}

		// wyliczmy nowe wirzchołki



		return newMesh;
	});
}
