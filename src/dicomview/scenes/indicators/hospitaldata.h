#include "_base.h"

#include <gdcmFile.h>

namespace Sokar {


	class HospitalDataIndicator : public SceneIndicator {
	private:
		QGraphicsTextItem* text;
		void initData();
	public:
		explicit HospitalDataIndicator(SokarDicom::DataConverter &dataConverter);
		void reposition() override;
		bool isAlive() override;
	};
}
