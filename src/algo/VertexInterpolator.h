//
// Created by adam on 08.05.2021.
//

#pragma once

#include "./_def.h"

namespace SokarAlg {

	class VertexInterpolator {
	protected:
		const DicomVolume *vv = nullptr;

	public:
		virtual ~VertexInterpolator();

		[[nodiscard]]
		const DicomVolume *getVirtualVolume() const;
		void setVirtualVolume(const DicomVolume *vv);

		[[nodiscard]]
		virtual float interpolate(const glm::vec3 &position) const = 0;
	};

	class NearestVertexInterpolator : public VertexInterpolator {
	public:
		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	class LinearVertexInterpolator : public VertexInterpolator {
	public:
		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	//https://www.mathworks.com/help/matlab/ref/interp3.html
}



