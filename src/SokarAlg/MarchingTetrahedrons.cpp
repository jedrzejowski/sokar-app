//
// Created by adam on 14.03.2021.
//

#include "./MarchingTetrahedrons.hpp"
#include "SokarException.hpp"

// https://github.com/Calvin-L/MarchingTetrahedrons/blob/master/Decimate.cpp

using namespace SokarAlg;

MarchingTetrahedronsPtr MarchingTetrahedrons::New() {

    return MarchingTetrahedronsPtr(new MarchingTetrahedrons);
}

/*
 Tetrahedron layout:
       0
       *
      /|\
     / | \
  3 *-----* 1
     \ | /
      \|/
       *
       2
 */


Sokar3D::MeshPtr MarchingTetrahedrons::exec() {

    auto size = volume->getSize() - cubeSize;

    forI32space({0, 0, 0}, size, [this](const auto &pos) {

        /*
         Coordinates:
              z
              |
              |___ y
              /
             /
            x
         Cube layout:
            4-------7
           /|      /|
          / |     / |
         5-------6  |
         |  0----|--3
         | /     | /
         |/      |/
         1-------2
         Tetrahedrons are:
             0, 7, 3, 2
             0, 7, 2, 6
             0, 4, 6, 7
             0, 6, 1, 2
             0, 6, 1, 4
             5, 6, 1, 4
         */

        const Volume::Point p[8] = {
                getPoint({pos.x, pos.y, pos.z}),
                getPoint({pos.x + 1, pos.y, pos.z}),
                getPoint({pos.x + 1, pos.y + 1, pos.z}),
                getPoint({pos.x, pos.y + 1, pos.z}),
                getPoint({pos.x, pos.y, pos.z + 1}),
                getPoint({pos.x + 1, pos.y, pos.z + 1}),
                getPoint({pos.x + 1, pos.y + 1, pos.z + 1}),
                getPoint({pos.x, pos.y + 1, pos.z + 1})
        };

        marchTetrahedron({p[0], p[7], p[3], p[2]});
        marchTetrahedron({p[0], p[7], p[2], p[6]});
        marchTetrahedron({p[0], p[4], p[7], p[6]});
        marchTetrahedron({p[0], p[1], p[6], p[2]});
        marchTetrahedron({p[0], p[4], p[6], p[1]});
        marchTetrahedron({p[5], p[1], p[6], p[4]});

    });

    return getMesh();
}

glm::vec3 vertexInterp(
        float isoLevel,
        const Volume::Point &p1,
        const Volume::Point &p2
) {

    float mu;
    glm::vec3 p;

    if (areSame(isoLevel, p1.value)) {
        return p1.position;
    }
    if (areSame(isoLevel, p2.value)) {
        return p2.position;
    }
    if (areSame(p1.value, p2.value)) {
        return p1.position;
    }

    mu = (isoLevel - p1.value) / (p2.value - p1.value);
    p.x = float(p1.position.x) + mu * (float(p2.position.x) - float(p1.position.x));
    p.y = float(p1.position.y) + mu * (float(p2.position.y) - float(p1.position.y));
    p.z = float(p1.position.z) + mu * (float(p2.position.z) - float(p1.position.z));

    return p;
}


void MarchingTetrahedrons::marchTetrahedron(MarchingTetrahedrons::Tetrahedron tetra) {

    quint8 tetraIndex = 0;

    if (tetra[0].value < iso_level) tetraIndex |= 1;
    if (tetra[1].value < iso_level) tetraIndex |= 2;
    if (tetra[2].value < iso_level) tetraIndex |= 4;
    if (tetra[3].value < iso_level) tetraIndex |= 8;

    switch (tetraIndex) {

        // we don't do anything if everyone is inside or outside
        case 0x00:
        case 0x0F:
            break;

            // only vert 0 is inside
        case 0x01:
            addTriangle(
                    vertexInterp(0.0f, tetra[0], tetra[1]),
                    vertexInterp(0.0f, tetra[0], tetra[3]),
                    vertexInterp(0.0f, tetra[0], tetra[2])
            );
            break;

            // only vert 1 is inside
        case 0x02:
//            addTriangle(
//                    vertexInterp(0.0f, tetra[1], tetra[0]),
//                    vertexInterp(0.0f, tetra[1], tetra[2]),
//                    vertexInterp(0.0f, tetra[1], tetra[3])
//            );
            break;

            // only vert 2 is inside
        case 0x04:
//            addTriangle(
//                    vertexInterp(0.0f, tetra[2], tetra[0]),
//                    vertexInterp(0.0f, tetra[2], tetra[3]),
//                    vertexInterp(0.0f, tetra[2], tetra[1])
//            );
            break;

            // only vert 3 is inside
        case 0x08:
//            addTriangle(
//                    vertexInterp(0.0f, tetra[3], tetra[1]),
//                    vertexInterp(0.0f, tetra[3], tetra[2]),
//                    vertexInterp(0.0f, tetra[3], tetra[0])
//            );
            break;

            // verts 0, 1 are inside
        case 0x03: {

//            auto p13 = vertexInterp(0.0f, tetra[1], tetra[3]);
//            auto p20 = vertexInterp(0.0f, tetra[3], tetra[2]);
//
//            addTriangle(
//                    vertexInterp(0.0f, tetra[3], tetra[0]),
//                    p20,
//                    p13
//            );
//
//            addTriangle(
//                    p20,
//                    vertexInterp(0.0f, tetra[2], tetra[1]),
//                    p13
//            );
        }
            break;

            // verts 0, 2 are inside
        case 0x05: {
//            auto p30 = vertexInterp(0.0f, tetra[3], tetra[0]);
//            auto p12 = vertexInterp(0.0f, tetra[1], tetra[2]);
//
//            addTriangle(
//                    p30,
//                    p12,
//                    vertexInterp(0.0f, tetra[1], tetra[0])
//            );
//            addTriangle(
//                    p12,
//                    p30,
//                    vertexInterp(0.0f, tetra[2], tetra[3])
//            );
        }
            break;

            // verts 0, 3 are inside
        case 0x09: {
//            auto p02 = vertexInterp(0.0f, tetra[0], tetra[2]);
//            auto p13 = vertexInterp(0.0f, tetra[1], tetra[3]);
//
//            addTriangle(
//                    vertexInterp(0.0f, tetra[0], tetra[1]),
//                    p13,
//                    p02
//            );
//            addTriangle(
//                    p13,
//                    vertexInterp(0.0f, tetra[3], tetra[2]),
//                    p02
//            );
        }
            break;

            // verts 1, 2 are inside
        case 0x06: {
//            auto p13 = vertexInterp(0.0f, tetra[1], tetra[3]);
//            auto p02 = vertexInterp(0.0f, tetra[0], tetra[2]);
//
//            addTriangle(
//                    vertexInterp(0.0f, tetra[0], tetra[1]),
//                    p02,
//                    p13
//            );
//            addTriangle(
//                    p13,
//                    p02,
//                    vertexInterp(0.0f, tetra[1], tetra[3])
//            );
        }
            break;

            // verts 2, 3 are inside
        case 0x0C: {
//            auto p13 = vertexInterp(0.0f, tetra[1], tetra[3]);
//            auto p20 = vertexInterp(0.0f, tetra[2], tetra[0]);
//
//            addTriangle(
//                    p13,
//                    p20,
//                    vertexInterp(0.0f, tetra[3], tetra[0])
//            );
//            addTriangle(
//                    p20,
//                    p13,
//                    vertexInterp(0.0f, tetra[2], tetra[1])
//            );

        }
            break;

            // verts 1, 3 are inside
        case 0x0A: {

//            auto p30 = vertexInterp(0.0f, tetra[3], tetra[0]);
//            auto p12 = vertexInterp(0.0f, tetra[1], tetra[2]);
//
//            addTriangle(
//                    p30,
//                    vertexInterp(0.0f, tetra[1], tetra[0]),
//                    p12
//            );
//            addTriangle(
//                    p12,
//                    vertexInterp(0.0f, tetra[2], tetra[3]),
//                    p30
//            );
        }
            break;

            // verts 0, 1, 2 are inside
        case 0x07:
//            addTriangle(
//                    vertexInterp(0.0f, tetra[3], tetra[0]),
//                    vertexInterp(0.0f, tetra[3], tetra[2]),
//                    vertexInterp(0.0f, tetra[3], tetra[1])
//            );
            break;

            // verts 0, 1, 3 are inside
        case 0x0B:
//            addTriangle(
//                    vertexInterp(0.0f, tetra[2], tetra[1]),
//                    vertexInterp(0.0f, tetra[2], tetra[3]),
//                    vertexInterp(0.0f, tetra[2], tetra[0])
//            );
            break;

            // verts 0, 2, 3 are inside
        case 0x0D:
//            addTriangle(
//                    vertexInterp(0.0f, tetra[1], tetra[2]),
//                    vertexInterp(0.0f, tetra[1], tetra[3]),
//                    vertexInterp(0.0f, tetra[1], tetra[0])
//            );
            break;

            // verts 1, 2, 3 are inside
        case 0x0E:
//            addTriangle(
//                    vertexInterp(0.0f, tetra[0], tetra[1]),
//                    vertexInterp(0.0f, tetra[0], tetra[2]),
//                    vertexInterp(0.0f, tetra[0], tetra[3])
//            );
            break;

            // what is this I don't even
        default:
            assert(false);
    }
}


QString MarchingTetrahedrons::toDisplay() {

    return QString();
}

const glm::i32vec3 &MarchingTetrahedrons::getCubeSize() const {

    return cubeSize;
}

void MarchingTetrahedrons::setCubeSize(const glm::i32vec3 &cubeSize) {

    MarchingTetrahedrons::cubeSize = cubeSize;
}
