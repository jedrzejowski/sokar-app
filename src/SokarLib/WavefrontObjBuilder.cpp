//
// Created by adam on 12.06.2021.
//

#include "WavefrontObjBuilder.hpp"

using namespace SokarLib;
using Size = WavefrontObjBuilder::size_type;

Size WavefrontObjBuilder::addVertex(const glm::vec3 &v) {

    vertices << "v" << space << v.x << space << v.y << space << v.z << endl;
    return ++verticesI;
}

Size WavefrontObjBuilder::addTextureCord(const glm::vec2 &vt) {

    texture << "t" << space << vt.x << space << vt.y << endl;
    return ++textureI;
}

Size WavefrontObjBuilder::addNormal(const glm::vec3 &vn) {

    normals << "vn" << space << vn.x << space << vn.y << space << vn.z << endl;
    return ++normalsI;
}

Size WavefrontObjBuilder::addFaceV(
        const size_type &v1,
        const size_type &v2,
        const size_type &v3
) {

    faces << "f" << space << v1 << space << v2 << space << v3 << endl;
    return ++facesI;
}

Size WavefrontObjBuilder::addFaceVT(
        const size_type &v1, const size_type &t1,
        const size_type &v2, const size_type &t2,
        const size_type &v3, const size_type &t3
) {

    faces << "f" << space
          << v1 << slash << t1 << space
          << v1 << slash << t2 << space
          << v1 << slash << t3 << endl;
    return ++facesI;
}

Size WavefrontObjBuilder::addFaceVTN(
        const size_type &v1, const size_type &t1, const size_type &n1,
        const size_type &v2, const size_type &t2, const size_type &n2,
        const size_type &v3, const size_type &t3, const size_type &n3
) {

    faces << "f" << space
          << v1 << slash << t1 << slash << n1 << space
          << v1 << slash << t2 << slash << n2 << space
          << v1 << slash << t3 << slash << n3 << endl;
    return ++facesI;
}

Size WavefrontObjBuilder::addFaceVN(
        const size_type &v1, const size_type &n1,
        const size_type &v2, const size_type &n2,
        const size_type &v3, const size_type &n3
) {

    faces << "f" << space
          << v1 << slash << n1 << space
          << v1 << slash << n2 << space
          << v1 << slash << n3 << endl;
    return ++facesI;
}

void WavefrontObjBuilder::dump2stream(QTextStream &stream) const {

    stream << vertices.str().c_str();
    stream << texture.str().c_str();
    stream << normals.str().c_str();
    stream << faces.str().c_str();
}

