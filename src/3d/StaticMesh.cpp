//
// Created by adam on 29.03.2021.
//

#include "StaticMesh.h"
#include "./MeshVertex.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>

using namespace Sokar3D;

MeshData *StaticMesh::data() {
	if (m_maybeRunning && !m_data.isValid())
		m_data = m_future.result();

	return &m_data;
}

void StaticMesh::reset() {
	*data() = MeshData();
	m_maybeRunning = false;
}

void StaticMesh::addTriangle(MeshVertex v0, MeshVertex v1, MeshVertex v2) {
//	qDebug() << "adding tirangle"
//			 << glm::to_string(v0.pos).c_str()
//			 << glm::to_string(v1.pos).c_str()
//			 << glm::to_string(v2.pos).c_str();

	m_data.geom << v0 << v1 << v2;
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
			glm::vec2(0.16*0),
			glm::vec2(0.16*1),
			glm::vec2(0.16*2),
			glm::vec2(0.16*3),
			glm::vec2(0.16*4),
			glm::vec2(0.16*5),
	};

	//face front

	mesh->addTriangle(
			{pos[0], tex[0], norm[0]},
			{pos[1], tex[0], norm[0]},
			{pos[2], tex[0], norm[0]}
	);

	mesh->addTriangle(
			{pos[0], tex[0], norm[0]},
			{pos[2], tex[0], norm[0]},
			{pos[3], tex[0], norm[0]}
	);

	//face top

	mesh->addTriangle(
			{pos[2], tex[1], norm[1]},
			{pos[4], tex[1], norm[1]},
			{pos[3], tex[1], norm[1]}
	);

	mesh->addTriangle(
			{pos[2], tex[1], norm[1]},
			{pos[5], tex[1], norm[1]},
			{pos[4], tex[1], norm[1]}
	);

	//face right

	mesh->addTriangle(
			{pos[1], tex[2], norm[2]},
			{pos[5], tex[2], norm[2]},
			{pos[2], tex[2], norm[2]}
	);

	mesh->addTriangle(
			{pos[1], tex[2], norm[2]},
			{pos[6], tex[2], norm[2]},
			{pos[5], tex[2], norm[2]}
	);

	//face left

	mesh->addTriangle(
			{pos[0], tex[3], norm[3]},
			{pos[4], tex[3], norm[3]},
			{pos[7], tex[3], norm[3]}
	);

	mesh->addTriangle(
			{pos[0], tex[3], norm[3]},
			{pos[3], tex[3], norm[3]},
			{pos[4], tex[3], norm[3]}
	);

	//face back

	mesh->addTriangle(
			{pos[5], tex[4], norm[4]},
			{pos[7], tex[4], norm[4]},
			{pos[4], tex[4], norm[4]}
	);

	mesh->addTriangle(
			{pos[5], tex[4], norm[4]},
			{pos[6], tex[4], norm[4]},
			{pos[7], tex[4], norm[4]}
	);

	//face bottom

	mesh->addTriangle(
			{pos[0], tex[5], norm[5]},
			{pos[7], tex[5], norm[5]},
			{pos[6], tex[5], norm[5]}
	);

	mesh->addTriangle(
			{pos[0], tex[5], norm[5]},
			{pos[6], tex[5], norm[5]},
			{pos[1], tex[5], norm[5]}
	);


	return mesh;
}
