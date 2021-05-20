//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./_def.h"
#include "./Triangle.h"
#include "./Volume/VirtualVolume.h"
#include "./VolumeSegmentator.hpp"
#include "../3d/StaticMesh.hpp"
#include "./Range.h"

namespace SokarAlg {

	class MarchingCubes : public VolumeSegmentator {
	Q_OBJECT

		Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)


		Range<float> isoLevel;
		glm::i32vec3 cubeSize = glm::i32vec3(1.f);

	protected:
		QFuture<void> execAlg() override;

	public:

		[[nodiscard]]
		const Range<float> &getIsoLevel() const;
		void setIsoLevel(const Range<float> &range);

		[[nodiscard]]
		const glm::i32vec3 &getCubeSize() const;
		void setCubeSize(const glm::i32vec3 &cubeSize);


	private:
		quint32 marchCube(Cube cube);
	};
}

