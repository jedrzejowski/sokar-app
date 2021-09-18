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

void MarchingTetrahedrons::marchTetrahedron(MarchingTetrahedrons::Tetrahedron tetra) {

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

    quint8 tetraIndex = 0;

    if (tetra[0].value < iso_level) tetraIndex |= 1 << 0;
    if (tetra[1].value < iso_level) tetraIndex |= 1 << 1;
    if (tetra[2].value < iso_level) tetraIndex |= 1 << 2;
    if (tetra[3].value < iso_level) tetraIndex |= 1 << 3;

    // DEBUG: narysowanie całego czwroroścanu
    if (false) {
        switch (tetraIndex) {

            // we don't do anything if everyone is inside or outside
            case 0x00:
            case 0x0F:
                break;

            default:
                addTriangle(tetra[0].position, tetra[1].position, tetra[3].position);
                addTriangle(tetra[0].position, tetra[3].position, tetra[2].position);
                addTriangle(tetra[1].position, tetra[3].position, tetra[2].position);
                addTriangle(tetra[0].position, tetra[1].position, tetra[2].position);
                return;
        }
    }

    switch (tetraIndex) {

        // we don't do anything if everyone is inside or outside
        case 0x00:
        case 0x0F:
            break;

            // only vert 0 is inside
        case 0x01:
            addTriangle(
                    interpolatePoint(tetra[0], tetra[1]),
                    interpolatePoint(tetra[0], tetra[3]),
                    interpolatePoint(tetra[0], tetra[2])
            );
            break;

            // only vert 1 is inside
        case 0x02:
            addTriangle(
                    interpolatePoint(tetra[1], tetra[0]),
                    interpolatePoint(tetra[1], tetra[2]),
                    interpolatePoint(tetra[1], tetra[3])
            );
            break;

            // only vert 2 is inside
        case 0x04:
            addTriangle(
                    interpolatePoint(tetra[2], tetra[0]),
                    interpolatePoint(tetra[2], tetra[3]),
                    interpolatePoint(tetra[2], tetra[1])
            );
            break;

            // only vert 3 is inside
        case 0x08:
            addTriangle(
                    interpolatePoint(tetra[3], tetra[1]),
                    interpolatePoint(tetra[3], tetra[2]),
                    interpolatePoint(tetra[3], tetra[0])
            );
            break;

            // verts 0, 1 are inside
        case 0x03: {

            auto p13 = interpolatePoint(tetra[1], tetra[3]);
            auto p20 = interpolatePoint(tetra[2], tetra[0]);

            addTriangle(
                    interpolatePoint(tetra[3], tetra[0]),
                    p20,
                    p13
            );

            addTriangle(
                    p20,
                    interpolatePoint(tetra[2], tetra[1]),
                    p13
            );
        }
            break;

            // verts 0, 2 are inside
        case 0x05: {
            auto p30 = interpolatePoint(tetra[3], tetra[0]);
            auto p12 = interpolatePoint(tetra[1], tetra[2]);

            addTriangle(
                    p30,
                    p12,
                    interpolatePoint(tetra[1], tetra[0])
            );
            addTriangle(
                    p12,
                    p30,
                    interpolatePoint(tetra[2], tetra[3])
            );
        }
            break;

            // verts 0, 3 are inside
        case 0x09: {
            auto p02 = interpolatePoint(tetra[0], tetra[2]);
            auto p13 = interpolatePoint(tetra[1], tetra[3]);

            addTriangle(
                    interpolatePoint(tetra[0], tetra[1]),
                    p13,
                    p02
            );
            addTriangle(
                    p13,
                    interpolatePoint(tetra[3], tetra[2]),
                    p02
            );
        }
            break;

            // verts 1, 2 are inside
        case 0x06: {
            auto p13 = interpolatePoint(tetra[1], tetra[3]);
            auto p02 = interpolatePoint(tetra[0], tetra[2]);

            addTriangle(
                    interpolatePoint(tetra[0], tetra[1]),
                    p02,
                    p13
            );
            addTriangle(
                    p13,
                    p02,
                    interpolatePoint(tetra[3], tetra[2])
            );
        }
            break;

            // verts 2, 3 are inside
        case 0x0C: {
            auto p13 = interpolatePoint(tetra[1], tetra[3]);
            auto p20 = interpolatePoint(tetra[2], tetra[0]);

            addTriangle(
                    p13,
                    p20,
                    interpolatePoint(tetra[3], tetra[0])
            );
            addTriangle(
                    p20,
                    p13,
                    interpolatePoint(tetra[2], tetra[1])
            );

        }
            break;

            // verts 1, 3 are inside
        case 0x0A: {

            auto p30 = interpolatePoint(tetra[3], tetra[0]);
            auto p12 = interpolatePoint(tetra[1], tetra[2]);

            addTriangle(
                    p30,
                    interpolatePoint(tetra[1], tetra[0]),
                    p12
            );
            addTriangle(
                    p12,
                    interpolatePoint(tetra[2], tetra[3]),
                    p30
            );
        }
            break;

            // verts 0, 1, 2 are inside
        case 0x07:
            addTriangle(
                    interpolatePoint(tetra[3], tetra[0]),
                    interpolatePoint(tetra[3], tetra[2]),
                    interpolatePoint(tetra[3], tetra[1])
            );
            break;

            // verts 0, 1, 3 are inside
        case 0x0B:
            addTriangle(
                    interpolatePoint(tetra[2], tetra[1]),
                    interpolatePoint(tetra[2], tetra[3]),
                    interpolatePoint(tetra[2], tetra[0])
            );
            break;

            // verts 0, 2, 3 are inside
        case 0x0D:
            addTriangle(
                    interpolatePoint(tetra[1], tetra[0]),
                    interpolatePoint(tetra[1], tetra[3]),
                    interpolatePoint(tetra[1], tetra[2])
            );
            break;

            // verts 1, 2, 3 are inside
        case 0x0E:
            addTriangle(
                    interpolatePoint(tetra[0], tetra[1]),
                    interpolatePoint(tetra[0], tetra[2]),
                    interpolatePoint(tetra[0], tetra[3])
            );
            break;

            // what is this I don't even
        default:
            Q_ASSERT(false);
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
