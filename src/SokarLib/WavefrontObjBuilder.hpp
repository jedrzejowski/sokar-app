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
        using size_type = qint32;
    private:
        std::string endl = "\n";
        std::string space = " ";
        std::string slash = "/";

        size_type verticesI = 0;
        std::stringstream vertices;

        size_type textureI = 0;
        std::stringstream texture;

        size_type normalsI = 0;
        std::stringstream normals;

        size_type facesI = 0;
        std::stringstream faces;

    public:
        size_type addVertex(const glm::vec3 &v);
        size_type addTextureCord(const glm::vec2 &vt);
        size_type addNormal(const glm::vec3 &vn);

        size_type addFaceV(
                const size_type &v1,
                const size_type &v2,
                const size_type &v3
        );

        size_type addFaceVT(
                const size_type &v1, const size_type &t1,
                const size_type &v2, const size_type &t2,
                const size_type &v3, const size_type &t3
        );

        size_type addFaceVTN(
                const size_type &v1, const size_type &t1, const size_type &n1,
                const size_type &v2, const size_type &t2, const size_type &n2,
                const size_type &v3, const size_type &t3, const size_type &n3
        );

        size_type addFaceVN(
                const size_type &v1, const size_type &n1,
                const size_type &v2, const size_type &n2,
                const size_type &v3, const size_type &n3
        );

        void dump2stream(QTextStream &stream) const override;
    };
}



