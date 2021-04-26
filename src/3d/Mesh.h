//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include "./_def.h"
#include "./MeshVertex.h"

namespace Sokar3D {

	struct MeshData {
		inline bool isValid() const { return !geom.isEmpty(); }

		inline int sizeInBytes() const {
			return geom.size() * sizeof(MeshVertex);
		}

		QVector<MeshVertex> geom;
	};

	class Mesh {
	public:
		MeshData *data();
		bool isValid() { return data()->isValid(); }
		void reset();

		void addTriangle(MeshVertex v0, MeshVertex v1, MeshVertex v2);

		static Mesh* createCubeMesh();

	private:
		bool m_maybeRunning = false;
		QFuture<MeshData> m_future;
		MeshData m_data;
	};
}