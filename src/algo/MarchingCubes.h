//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./_def.h"
#include "./Triangle.h"
#include "./VirtualVolume.h"
#include "./VolSegmAlg.h"
#include "../3d/StaticMesh.h"

namespace SokarAlg {

	class MarchingCubes : public VolSegmAlg {

		float isoLevel = 0.5;
		glm::vec3 cubeSize = glm::vec3(1.f);
		quint32 triangleIndex = 0;

	protected:
		QFuture<void> execAlg() override;

	public:

		[[nodiscard]]
		float getIsoLevel() const;
		void setIsoLevel(float isoLevel);

		const glm::vec3 &getCubeSize() const;
		void setCubeSize(const glm::vec3 &cubeSize);


	private:
		quint32 marchCube(Cube cube);
	};
}

