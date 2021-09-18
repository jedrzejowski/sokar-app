SET(SRC_QT_UI
        ../QtUi.hpp
        ComboBox.hpp
        ComboBox.cpp
        DoubleSpinBox.hpp
        DoubleSpinBox.cpp
        SpinBox.hpp
        SpinBox.cpp
        Vec3Input.hpp
        Vec3Input.cpp
        )

list(TRANSFORM SRC_QT_UI PREPEND ${PROJECT_SOURCE_DIR}/src/QtUi/)


