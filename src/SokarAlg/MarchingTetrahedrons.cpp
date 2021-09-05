//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./MarchingTetrahedrons.hpp"

using namespace SokarAlg;

const qint32 edgeTable[16] = {
        0x00, // 0x00
        0x07, // 0x01
        0x29, // 0x02
        0x2E, // 0x03
        0x0E, // 0x04
        0x1B, // 0x05
        0x37, // 0x06
        0x32, // 0x07
        0x32, // 0x08
        0x0, // 0x09
        0x0, // 0x0A
        0x0, // 0x0B
        0x0, // 0x0C
        0x0, // 0x0D
        0x0, // 0x0E
        0x0, // 0x0F
};

//https://github.com/Calvin-L/MarchingTetrahedrons/blob/master/Decimate.cpp

const qint32 triTable[16][8] = {
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x00
        {0, 1, 2, -1, -1, -1, -1, -1}, // 0x01
        {0, 3, 5, -1, -1, -1, -1, -1}, // 0x02
        {2, 5, 1, 5, 2, 3, -1, -1}, // 0x03
        {3, 2, 4, -1, -1, -1, -1, -1}, // 0x04
        {0, 1, 4, 0, 4, 3, -1, -1}, // 0x05
        {0, 2, 5, 5, 3, 0, -1, -1}, // 0x06
        {5, 1, 4, -1, -1, -1, -1, -1}, // 0x07
        {5, 4, 1, -1, -1, -1, -1, -1}, // 0x08
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x09
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x0A
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x0B
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x0C
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x0D
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x0E
        {-1, -1, -1, -1, -1, -1, -1, -1}, // 0x0F
};

void MarchingTetrahedrons::marchTetrahedron(MarchingTetrahedrons::Tetrahedron cube) {

}
Cube MarchingTetrahedrons::getTetrahedron(const glm::i32vec3 &position, ushort tet_num) const {

    return nullptr;
}
QString MarchingTetrahedrons::toDisplay() {

    return QString();
}
Sokar3D::MeshPtr MarchingTetrahedrons::exec() {

    return Sokar3D::MeshPtr();
}
