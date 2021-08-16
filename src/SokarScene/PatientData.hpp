#include "SokarScene.hpp"
#include "SceneIndicator.hpp"

#include <gdcmFile.h>

namespace SokarScene {


	class PatientData : public SceneIndicator {
	private:
		QGraphicsTextItem* text;
		void initData();
	public:
	    PatientData(SokarDicom::DataConverter& dataConverter);
		void reposition() override;
		bool isAlive() override;
	};
}
