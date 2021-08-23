//
// Created by adam on 12.06.2021.
//

#pragma once

#include <string>
#include <ostream>
#include <sstream>
#include <QFile>
#include "SokarGlm.hpp"
#include "SaveableAsText.hpp"

namespace SokarLib {
    class WavefrontObjBuilder : public SokarLib::SaveableAsText {
    public:
        using Size = qint32;
    private:
        std::string endl = "\n";
        std::string space = " ";
        std::string slash = "/";

        Size verticesI = 0;
        std::stringstream vertices;

        Size textureI = 0;
        std::stringstream texture;

        Size normalsI = 0;
        std::stringstream normals;

        Size facesI = 0;
        std::stringstream faces;

    public:
        Size addVertex(const glm::vec3 &v);
        Size addTextureCord(const glm::vec2 &vt);
        Size addNormal(const glm::vec3 &vn);

        Size addFaceV(
                const Size &v1,
                const Size &v2,
                const Size &v3
        );

        Size addFaceVT(
                const Size &v1, const Size &t1,
                const Size &v2, const Size &t2,
                const Size &v3, const Size &t3
        );

        Size addFaceVTN(
                const Size &v1, const Size &t1, const Size &n1,
                const Size &v2, const Size &t2, const Size &n2,
                const Size &v3, const Size &t3, const Size &n3
        );

        Size addFaceVN(
                const Size &v1, const Size &n1,
                const Size &v2, const Size &n2,
                const Size &v3, const Size &n3
        );

        void dump2stream(QTextStream &stream) const override;
    };
}



