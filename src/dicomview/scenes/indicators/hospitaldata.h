#include "_base.h"

#include <gdcmFile.h>

namespace Sokar {


	class HospitalDataIndicator : public SceneIndicator {
	private:
		QGraphicsTextItem* text;
	public:
		HospitalDataIndicator();
		void reposition() override;
		void loadData(const gdcm::File& file);
	};
}
