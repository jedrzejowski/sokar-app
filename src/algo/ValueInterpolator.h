//
// Created by adam on 08.05.2021.
//

#pragma once

#include "./_def.h"

namespace SokarAlg {

	class ValueInterpolator {
	protected:
		const DicomVolume *vv = nullptr;

	public:
		virtual ~ValueInterpolator();

		[[nodiscard]]
		const DicomVolume *getVolume() const;
		void setVolume(const DicomVolume *vv);

		[[nodiscard]]
		virtual float interpolate(const glm::vec3 &position) const = 0;
	};

	class NearestValueInterpolator : public ValueInterpolator {
	public:
		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	class LinearValueInterpolator : public ValueInterpolator {
	public:
		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	//https://www.mathworks.com/help/matlab/ref/interp3.html
}



