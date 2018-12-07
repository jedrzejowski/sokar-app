#include "_base.h"

#include <gdcmFile.h>

namespace Sokar {


	class PatientDataIndicator : public SceneIndicator {
	private:
		QGraphicsTextItem* text;
	public:
		PatientDataIndicator();
		void reposition() override;
		void loadData(const gdcm::File& file);
	};
}
