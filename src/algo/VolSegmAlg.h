//
// Created by adam on 03.05.2021.
//

#pragma once

#include "./_def.h"
#include "./Triangle.h"
#include "./VirtualVolume.h"
#include "../3d/StaticMesh.h"

namespace SokarAlg {

	class VolSegmAlg {
	protected:

		const VirtualVolume *virtualVolume;
		std::vector<Triangle> triangles;

		virtual QFuture<void> execAlg() = 0;

	public:
		QFuture<void> exec();

		[[nodiscard]]
		const VirtualVolume *getVirtualVolume() const;

		void setVirtualVolume(const VirtualVolume *virtualVolume);

		[[nodiscard]]
		const std::vector<Triangle> &getTriangles() const;

		[[nodiscard]]
		Sokar3D::StaticMesh *toStaticMesh() const;
	};
}



