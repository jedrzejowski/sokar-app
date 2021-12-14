//
// Created by adam on 13.05.2021.
//

#include "BoundingMeshSimplification.hpp"

using namespace SokarAlg;

BoundingMeshSimplification::BoundingMeshSimplification() {

}

BoundingMeshSimplificationPtr BoundingMeshSimplification::New() {

    return SokarAlg::BoundingMeshSimplificationPtr(new BoundingMeshSimplification);
}

Sokar3D::IndexedMeshPtr BoundingMeshSimplification::exec() {

    auto bound_mesh = std::make_shared<boundingmesh::Mesh>();

    // region initialization of mesh

    auto my_vertex_2_bounding_vertex = QHash<Sokar3D::IndexedMesh::size_type, boundingmesh::Index>();

    for (auto iter = mesh->getVertices().constKeyValueBegin();
         iter != mesh->getVertices().constKeyValueEnd(); ++iter) {
        auto i = bound_mesh->addVertex(boundingmesh::Vector3{iter->second.x, iter->second.y, iter->second.z});
        my_vertex_2_bounding_vertex[iter->first] = i;
    }

    for (const auto &face: mesh->getFaces()) {
        bound_mesh->addTriangle(
                my_vertex_2_bounding_vertex[face.i0],
                my_vertex_2_bounding_vertex[face.i1],
                my_vertex_2_bounding_vertex[face.i2]
        );
    }

    //endregion

    boundingmesh::Decimator decimator(direction);
    decimator.setMaximumError(maximum_error);
    decimator.setTargetVertices(qFloor(float(bound_mesh->nVertices()) * vertex_reduction));
    decimator.setMesh(*bound_mesh);

    auto decimated_mesh = decimator.compute();
    decimated_mesh->writeObj("/tmp/sokar/w.obj");
    auto output_mesh = Sokar3D::IndexedMesh::New();

    std::vector<uint> decimeted2outputVertexMap;

    auto vert_num = decimated_mesh->nVertices();
    decimeted2outputVertexMap.resize(vert_num);

    for (uint i = 0; i < vert_num; ++i) {
        auto vert = decimated_mesh->vertex(i).position();

        auto new_i = output_mesh->addVertex({vert.x(), vert.y(), vert.z()}, false);

        decimeted2outputVertexMap[i] = new_i;
    }

    auto triangle_num = decimated_mesh->nTriangles();

    qDebug() << "triangle_num" << triangle_num;

    for (uint i = 0; i < triangle_num; ++i) {
        auto &triangle = decimated_mesh->triangle(i);

        output_mesh->addTriangle(
                decimeted2outputVertexMap[triangle.vertex(0)],
                decimeted2outputVertexMap[triangle.vertex(1)],
                decimeted2outputVertexMap[triangle.vertex(2)],
                false
        );
    }

    return output_mesh;
}


QString BoundingMeshSimplification::toDisplay() {

    return QString();
}

BoundingMeshSimplification::DecimationDirection BoundingMeshSimplification::getDirection() const {

    return direction;
}

void BoundingMeshSimplification::setDirection(BoundingMeshSimplification::DecimationDirection direction) {

    BoundingMeshSimplification::direction = direction;
}

float BoundingMeshSimplification::getMaximumError() const {

    return maximum_error;
}

void BoundingMeshSimplification::setMaximumError(float new_maximum_error) {

    Q_ASSERT(new_maximum_error > 0);

    maximum_error = new_maximum_error;
}

float BoundingMeshSimplification::getVertexReduction() const {

    return vertex_reduction;
}

void BoundingMeshSimplification::setVertexReduction(float new_vertex_reduction) {

    Q_ASSERT(new_vertex_reduction > 0);
    Q_ASSERT(new_vertex_reduction < 1);

    BoundingMeshSimplification::vertex_reduction = new_vertex_reduction;
}
