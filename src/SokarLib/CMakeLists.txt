
set(SRC_SOKAR_LIB
        concat_array.h
        Jsonable.cpp
        Jsonable.hpp
        ThreadCache.hpp
        WavefrontObjBuilder.cpp
        WavefrontObjBuilder.hpp
        SaveableAsText.hpp
        SaveableAsText.cpp
        Displayable.cpp
        Displayable.hpp
        HashCubeSpace.hpp
        HashCubeSpace.cpp
        )

list(TRANSFORM SRC_SOKAR_LIB PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
