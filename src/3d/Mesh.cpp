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
	qDebug() << "adding tirangle";
	m_data.geom << v0 << v1 << v2;
}
