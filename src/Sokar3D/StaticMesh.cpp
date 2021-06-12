//
// Created by adam on 29.03.2021.
//

#include "StaticMesh.hpp"
#include "./MeshVertex.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>

using namespace Sokar3D;

StaticMesh::StaticMesh() {
}

StaticMeshPtr StaticMesh::New() {
	return StaticMeshPtr(new StaticMesh);
}

const quint8 *StaticMesh::verticesData() const {
	return reinterpret_cast<const quint8 *>(vertices.data());
}


void StaticMesh::addTriangle(
		const MeshVertex &v0,
		const MeshVertex &v1,
		const MeshVertex &v2
) {
	vertices << v0 << v1 << v2;
}


void StaticMesh::addTriangle(
		const glm::vec3 &v0,
		const glm::vec3 &v1,
		const glm::vec3 &v2
) {
	auto norm = glm::triangleNormal(v0, v1, v2);

	addTriangle(
			{v0, norm},
			{v1, norm},
			{v2, norm}
	);
}

qsizetype StaticMesh::verticesSizeInBytes() const {
	return vertices.size() * sizeof(MeshVertex);
}

qsizetype StaticMesh::verticesCount() const {
	return vertices.size();
}

const QVector<MeshVertex> &StaticMesh::getVertices() const {
	return vertices;
}


StaticMesh *StaticMesh::createCubeMesh() {
	auto mesh = new Sokar3D::StaticMesh();

	// http://ilkinulas.github.io/development/unity/2016/04/30/cube-mesh-in-unity3d.html

	static glm::vec3 pos[8] = {
			glm::vec3{0, 0, 0}, // 0
			glm::vec3{1, 0, 0}, // 1
			glm::vec3{1, 1, 0}, // 2
			glm::vec3{0, 1, 0}, // 3
			glm::vec3{0, 1, 1}, // 4
			glm::vec3{1, 1, 1}, // 5
			glm::vec3{1, 0, 1}, // 6
			glm::vec3{0, 0, 1}, // 7
	};

	static glm::vec3 norm[6] = {
			glm::vec3{0, 0, -1}, // front
			glm::vec3{0, 1, 0}, // top
			glm::vec3{-1, 0, 0}, // right
			glm::vec3{1, 0, 0}, // left
			glm::vec3{0, 0, 1}, // back
			glm::vec3{0, -1, 0}, // bottom
	};

	static glm::vec2 tex[6] = {
			glm::vec2(0.16 * 0),
			glm::vec2(0.16 * 1),
			glm::vec2(0.16 * 2),
			glm::vec2(0.16 * 3),
			glm::vec2(0.16 * 4),
			glm::vec2(0.16 * 5),
	};

	//face front

	mesh->addTriangle(
			{pos[0], norm[0]},
			{pos[1], norm[0]},
			{pos[2], norm[0]}
	);

	mesh->addTriangle(
			{pos[0], norm[0]},
			{pos[2], norm[0]},
			{pos[3], norm[0]}
	);

	//face top

	mesh->addTriangle(
			{pos[2], norm[1]},
			{pos[4], norm[1]},
			{pos[3], norm[1]}
	);

	mesh->addTriangle(
			{pos[2], norm[1]},
			{pos[5], norm[1]},
			{pos[4], norm[1]}
	);

	//face right

	mesh->addTriangle(
			{pos[1], norm[2]},
			{pos[5], norm[2]},
			{pos[2], norm[2]}
	);

	mesh->addTriangle(
			{pos[1], norm[2]},
			{pos[6], norm[2]},
			{pos[5], norm[2]}
	);

	//face left

	mesh->addTriangle(
			{pos[0], norm[3]},
			{pos[4], norm[3]},
			{pos[7], norm[3]}
	);

	mesh->addTriangle(
			{pos[0], norm[3]},
			{pos[3], norm[3]},
			{pos[4], norm[3]}
	);

	//face back

	mesh->addTriangle(
			{pos[5], norm[4]},
			{pos[7], norm[4]},
			{pos[4], norm[4]}
	);

	mesh->addTriangle(
			{pos[5], norm[4]},
			{pos[6], norm[4]},
			{pos[7], norm[4]}
	);

	//face bottom

	mesh->addTriangle(
			{pos[0], norm[5]},
			{pos[7], norm[5]},
			{pos[6], norm[5]}
	);

	mesh->addTriangle(
			{pos[0], norm[5]},
			{pos[6], norm[5]},
			{pos[1], norm[5]}
	);


	return mesh;
}

void StaticMesh::dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const {
	auto iter = vertices.begin();

	while (iter != vertices.end()) {

		auto &v1 = (iter++)->pos;
		auto &v2 = (iter++)->pos;
		auto &v3 = (iter++)->pos;

		auto v1i = builder.addVertex(v1);
		auto v2i = builder.addVertex(v2);
		auto v3i = builder.addVertex(v3);

		builder.addFaceV(v1i, v2i, v3i);
	}
}
