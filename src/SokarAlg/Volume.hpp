//
// Created by adam on 14.03.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {


	class Volume {
	public:

		Volume();
		~Volume();

		[[nodiscard]]
		virtual glm::i32vec3 getSize() const = 0;

		[[nodiscard]]
		virtual float getValue(const glm::i32vec3 &position) const = 0;

		[[nodiscard]]
		float getValueSafe(const glm::i32vec3 &position) const;

		[[nodiscard]]
		glm::i32vec3 clamp(const glm::i32vec3 &size) const;

		[[nodiscard]]
		bool isInVolume(const glm::i32vec3 &position) const;
	};

	class VolumeDecorator : public Volume {

	protected:

		QSharedPointer<const Volume> volume = nullptr;

		virtual void volumeChanged();

	public:

		explicit VolumeDecorator();
		explicit VolumeDecorator(const QSharedPointer<const Volume>& volume);
		virtual ~VolumeDecorator();

		[[nodiscard]]
		inline const QSharedPointer<const Volume> &getVolume() const {
			return volume;
		}

		void setVolume(const QSharedPointer<const Volume> &volume);

		glm::i32vec3 getSize() const override;
		float getValue(const glm::i32vec3 &position) const override;
	};
}



