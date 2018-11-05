#pragma once

#include <QCore>
#include <qwidgets/dicomscene.h>

namespace Sokar{
	class Monochrome2DicomScene;
}

class Sokar::Monochrome2DicomScene : public Sokar::DicomScene{

protected:
	QPixmap* genQPixmap() override;

};

