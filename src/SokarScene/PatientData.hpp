#include "SokarScene.hpp"
#include "Indicator.hpp"

#include <gdcmFile.h>

namespace SokarScene {


    class PatientData : public Indicator {
    private:
        QGraphicsTextItem *text;
        void initData();
    public:
        PatientData(SokarDicom::DataConverter &dataConverter);
        void reposition() override;
        bool isAlive() override;
    };
}
