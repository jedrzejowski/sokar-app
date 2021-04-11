//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./_def.h"
#include "./Triangle.h"
#include "./VirtualVolume.h"

namespace SokarAlg {

	class MarchingCubes {
		VirtualVolume *virtualVolume;
		QVector<Triangle> triangles;
		float isoLevel;
	public:

		[[nodiscard]]
		VirtualVolume *getVirtualVolume() const;
		void setVirtualVolume(VirtualVolume *virtualVolume);

		const QVector<Triangle> &getTriangles() const;

		float getIsoLevel() const;
		void setIsoLevel(float isoLevel);

		QFuture<void> marchingCubes();

	private:
		quint32 marchCube(Cube cube);
	};
}

