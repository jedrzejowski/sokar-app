//
// Created by adam on 03.05.2021.
//

#pragma once

#include "./_def.h"
#include "./Triangle.h"
#include "./Volume/VirtualVolume.h"
#include "../3d/StaticMesh.hpp"

namespace SokarAlg {

	class VolumeSegmentator : public QObject {
	Q_OBJECT
	protected:

		const VirtualVolume *virtualVolume = nullptr;
		IndexedMesh *mesh = nullptr;

		virtual QFuture<void> execAlg() = 0;

	public:
		QFuture<void> exec();

		[[nodiscard]]
		const VirtualVolume *getVirtualVolume() const;

		void setVirtualVolume(const VirtualVolume *virtualVolume);

		void addTriangle(const Triangle &t);

		[[nodiscard]]
		const IndexedMesh *getMesh() const;
	};
}



