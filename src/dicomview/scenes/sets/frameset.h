#pragma once

#include "_sceneset.h"
#include "sokar/dicombundle.h"

namespace Sokar {
	class DicomFrameSet : public DicomSceneSet {
	Q_OBJECT
	protected:

		const DicomBundle* bundle;

		std::vector<char> imageBuffer;

		QString title;

		int numberOfFrames = 1;
		SceneSequence *sceneSequence = nullptr;

	public:

		explicit DicomFrameSet(const DicomBundle *bundle, QObject *parent = nullptr);
		~DicomFrameSet() override;

		inline const gdcm::File &getGdcmFile() const { return imageReader->GetFile(); }

		SceneSequence *getSceneSequence() override;

		const QString &getTitle() override;

	private:

		void initScenes();
	};
}