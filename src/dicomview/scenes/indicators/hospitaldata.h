#include "_base.h"

#include <gdcmFile.h>

namespace Sokar {


	class HospitalDataIndicator : public SceneIndicator {
	private:
		QGraphicsTextItem* text;
		void initData();
	public:
		HospitalDataIndicator(DataConverter &dataConverter);
		void reposition() override;
	};
}
