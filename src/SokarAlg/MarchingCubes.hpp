//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./SokarAlg.hpp"
#include "Volume.hpp"
#include "VolumeSegmentator.hpp"
#include "Sokar3D/StaticMesh.hpp"
#include "Range.hpp"

namespace SokarAlg {

	class MarchingCubes : public VolumeSegmentator {

		struct Cube {
			glm::i32vec3 position[8];
			float value[8];
		};

		glm::i32vec3 cubeSize = glm::i32vec3(1.f);

	protected:
		void execAlg() override;

	public:

		[[nodiscard]]
		const glm::i32vec3 &getCubeSize() const;
		void setCubeSize(const glm::i32vec3 &cubeSize);
		QString toDisplay() override;


	private:

		[[nodiscard]]
		Cube getCube(const glm::i32vec3 &position, const glm::i32vec3 &size = glm::i32vec3(1)) const;
		quint32 marchCube(Cube cube);
	};
}

