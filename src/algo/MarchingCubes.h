//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./_def.h"
#include "./Triangle.h"
#include "./Volume/VirtualVolume.h"
#include "./VolSegmAlg.h"
#include "../3d/StaticMesh.hpp"
#include "./Range.h"

namespace SokarAlg {

	class MarchingCubes : public VolSegmAlg {

		Range<float> isoLevel ;
		glm::i32vec3 cubeSize = glm::i32vec3(1.f);
		quint32 triangleIndex = 0;

	protected:
		QFuture<void> execAlg() override;

	public:

		[[nodiscard]]
		const Range<float>& getIsoLevel() const;
		void setIsoLevel(const Range<float>& range);

		[[nodiscard]]
		const glm::i32vec3 &getCubeSize() const;
		void setCubeSize(const glm::i32vec3 &cubeSize);


	private:
		quint32 marchCube(Cube cube);
	};
}

