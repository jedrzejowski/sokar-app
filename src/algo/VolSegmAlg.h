//
// Created by adam on 03.05.2021.
//

#pragma once

#include "./_def.h"
#include "./Triangle.h"
#include "./Volume/VirtualVolume.h"
#include "../3d/StaticMesh.h"

namespace SokarAlg {

	class VolSegmAlg {
	protected:

		const VirtualVolume *virtualVolume;
		Sokar3D::StaticMesh *staticMesh = nullptr;

		virtual QFuture<void> execAlg() = 0;

	public:
		QFuture<void> exec();

		[[nodiscard]]
		const VirtualVolume *getVirtualVolume() const;

		void setVirtualVolume(const VirtualVolume *virtualVolume);

		void addTriangle(const Triangle &t);

		[[nodiscard]]
		Sokar3D::StaticMesh *dumpStaticMesh();
	};
}



