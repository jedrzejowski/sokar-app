//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"
#include "Array3.hpp"

namespace SokarAlg {

	class CachedVolume : public Volume {
		QSharedPointer<const Volume> volume = nullptr;
		glm::i32vec3 size = glm::i32vec3(0);
		Array3<float> cache;
	public:
		explicit CachedVolume();
		virtual ~CachedVolume();

		void rebuildCache();

		[[nodiscard]]
		const QSharedPointer<const Volume> &getVolume() const;
		void setVolume(const QSharedPointer<const Volume> &volume, bool rebuild = true);

		[[nodiscard]]
		glm::i32vec3 getSize() const override;
		[[nodiscard]]
		float getValue(const glm::i32vec3 &position) const override;
	};
}

