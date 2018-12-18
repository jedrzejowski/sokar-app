#pragma once

#include "_sceneset.h"

namespace Sokar {
	class DicomFrameSet : public DicomSceneSet {
	Q_OBJECT
	protected:

		const gdcm::ImageReader *imageReader;
		const gdcm::File &gdcmFile;
		const gdcm::DataSet &gdcmDataSet;
		const gdcm::Image &gdcmImage;
		DataConverter dataConverter;

		std::vector<char> imageBuffer;

		QString title;

		int numberOfFrames = 1;
		SceneSequence *sceneSequence = nullptr;

	public:

		explicit DicomFrameSet(const gdcm::ImageReader *reader, QObject *parent = nullptr);
		~DicomFrameSet() override;

		inline const gdcm::File &getGdcmFile() const { return imageReader->GetFile(); }

		SceneSequence *getSceneSequence() override;

		const QString &getTitle() override;

	private:

		void initScenes();
	};
}