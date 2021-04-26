//
// Created by adam on 29.03.2021.
//

#include "Mesh.h"
#include "./MeshVertex.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>

using namespace Sokar3D;

MeshData *Mesh::data() {
	if (m_maybeRunning && !m_data.isValid())
		m_data = m_future.result();

	return &m_data;
}

void Mesh::reset() {
	*data() = MeshData();
	m_maybeRunning = false;
}

void Mesh::addTriangle(MeshVertex v0, MeshVertex v1, MeshVertex v2) {
	qDebug() << "adding tirangle"
			 << glm::to_string(v0.pos).c_str()
			 << glm::to_string(v0.pos).c_str()
			 << glm::to_string(v0.pos).c_str();

	m_data.geom << v0 << v1 << v2;
}

Mesh *Mesh::createCubeMesh() {
	auto mesh = new Sokar3D::Mesh();

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
			glm::vec3{0, 0, 0}, // front
			glm::vec3{1, 0, 0}, // top
			glm::vec3{1, 1, 0}, // right
			glm::vec3{0, 1, 0}, // left
			glm::vec3{0, 1, 1}, // back
			glm::vec3{1, 1, 1}, // bottom
	};

	static auto tex = glm::vec2(0);

	//face front

	mesh->addTriangle(
			{pos[0], tex, norm[0]},
			{pos[2], tex, norm[0]},
			{pos[1], tex, norm[0]}
	);

	mesh->addTriangle(
			{pos[0], tex, norm[0]},
			{pos[3], tex, norm[0]},
			{pos[2], tex, norm[0]}
	);

	//face top

	mesh->addTriangle(
			{pos[2], tex, norm[0]},
			{pos[3], tex, norm[0]},
			{pos[4], tex, norm[0]}
	);

	mesh->addTriangle(
			{pos[2], tex, norm[0]},
			{pos[4], tex, norm[0]},
			{pos[5], tex, norm[0]}
	);

	//face right

	mesh->addTriangle(
			{pos[1], tex, norm[0]},
			{pos[2], tex, norm[0]},
			{pos[5], tex, norm[0]}
	);

	mesh->addTriangle(
			{pos[1], tex, norm[0]},
			{pos[5], tex, norm[0]},
			{pos[6], tex, norm[0]}
	);

	//face left

	mesh->addTriangle(
			{pos[0], tex, norm[0]},
			{pos[7], tex, norm[0]},
			{pos[4], tex, norm[0]}
	);

	mesh->addTriangle(
			{pos[0], tex, norm[0]},
			{pos[4], tex, norm[0]},
			{pos[3], tex, norm[0]}
	);

	//face back

	mesh->addTriangle(
			{pos[5], tex, norm[0]},
			{pos[4], tex, norm[0]},
			{pos[7], tex, norm[0]}
	);

	mesh->addTriangle(
			{pos[5], tex, norm[0]},
			{pos[7], tex, norm[0]},
			{pos[6], tex, norm[0]}
	);

	//face bottom

	mesh->addTriangle(
			{pos[0], tex, norm[0]},
			{pos[6], tex, norm[0]},
			{pos[7], tex, norm[0]}
	);

	mesh->addTriangle(
			{pos[0], tex, norm[0]},
			{pos[1], tex, norm[0]},
			{pos[6], tex, norm[0]}
	);


	return mesh;
}
