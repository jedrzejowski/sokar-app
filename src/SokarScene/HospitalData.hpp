#include "SokarScene.hpp"
#include "SceneIndicator.hpp"

#include <gdcmFile.h>
#include <QGraphicsTextItem>

namespace SokarScene {


	class HospitalData : public SceneIndicator {
	private:
		QGraphicsTextItem* text;
		void initData();
	public:
	    explicit HospitalData(SokarDicom::DataConverter &dataConverter);
		void reposition() override;
		bool isAlive() override;
	};
}
