#pragma once

#include <glm/glm.hpp>
#include <QtGlobal>

namespace Sokar {
	typedef quint8 uint8;
	typedef qint8 int8;
	typedef quint32 uint32;
	typedef quint64 uint64;
	typedef float float32;
	typedef double float64;


	typedef glm::u32vec2 u32vec2;
	typedef glm::u32vec3 u32vec3;
	typedef glm::vec3 vec3;


	class DicomView;

	class FrameChooser;

	class DicomToolBar;

	class DicomGraphics;

	class DataSetViewer;

	class MovieBar;

	class Scene;

	class DicomScene;

	class DicomSceneSet;

	class SceneIndicator;

	class SceneAvatar;

	class MovieMode;

	struct SceneParams;
}