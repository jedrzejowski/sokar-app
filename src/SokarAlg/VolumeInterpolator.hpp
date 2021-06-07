//
// Created by adam on 08.05.2021.
//

#pragma once

#include "./gte/Array3.h"
#include "./gte/IntpAkimaUniform3.h"
#include "./SokarAlg.hpp"

namespace SokarAlg {

	class VolumeInterpolator {
	protected:
		QSharedPointer<const Volume> vv = nullptr;

		virtual void dicomVolumeChanged();

	public:
		virtual ~VolumeInterpolator();

		[[nodiscard]]
		const QSharedPointer<const Volume> &getVolume() const;
		void setVolume(const QSharedPointer<const Volume> &vv);

		[[nodiscard]]
		virtual float interpolate(const glm::vec3 &position) const = 0;
	};

	class NearestVolumeInterpolator : public VolumeInterpolator {
	public:
		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	class LinearVolumeInterpolator : public VolumeInterpolator {
	public:
		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	class PolynomialVolumeInterpolator1 : public VolumeInterpolator {
	protected:
		glm::i32vec3 size = glm::i32vec3(2, 2, 2);
	public:

		[[nodiscard]]
		const glm::i32vec3 &getSize() const;
		void setSize(const glm::i32vec3 &size);

		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	class PolynomialVolumeInterpolator2 : public PolynomialVolumeInterpolator1 {
	public:

		[[nodiscard]]
		float interpolate(const glm::vec3 &position) const override;
	};

	class AkimaVolumeInterpolator : public VolumeInterpolator {
	private:

		std::vector<float> values;
		gte::IntpAkimaUniform3<float> *externInterpolator = nullptr;

	protected:
		void dicomVolumeChanged() override;
	public:
		float interpolate(const glm::vec3 &position) const override;
	};

	class CubicVolumeInterpolator : public VolumeInterpolator {
	private:
		std::array<std::array<float, 4>, 4> blend;
	public:
		explicit CubicVolumeInterpolator(bool catmullRom = false);
		float interpolate(const glm::vec3 &position) const override;
	};

	//https://www.mathworks.com/help/matlab/ref/interp3.html
}



