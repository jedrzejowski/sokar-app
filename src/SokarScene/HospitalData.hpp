#include "SokarScene.hpp"
#include "Indicator.hpp"

#include <gdcmFile.h>
#include <QGraphicsTextItem>

namespace SokarScene {


	class HospitalData : public Indicator {
	private:
		QGraphicsTextItem* text;
		void initData();
	public:
	    explicit HospitalData(SokarDicom::DataConverter &dataConverter);
		void reposition() override;
		bool isAlive() override;
	};
}
