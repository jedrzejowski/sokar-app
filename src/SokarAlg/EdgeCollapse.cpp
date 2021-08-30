//
// Created by adam on 13.05.2021.
//

#include "EdgeCollapse.hpp"

using namespace SokarAlg;

EdgeCollapse::EdgeCollapse() {

}

EdgeCollapsePtr EdgeCollapse::New() {

    return SokarAlg::EdgeCollapsePtr(new EdgeCollapse);
}

Sokar3D::IndexedMeshPtr EdgeCollapse::exec() {

    auto bound_mesh = mesh->toBoundingMesh();

    boundingmesh::Decimator decimator(direction);
    decimator.setMaximumError(maximum_error);
    decimator.setTargetVertices(qFloor(float(bound_mesh->nVertices()) * vertex_reduction));
    decimator.setMesh(*bound_mesh);

    auto decimated_mesh = decimator.compute();
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


QString EdgeCollapse::toDisplay() {

    return QString();
}

EdgeCollapse::DecimationDirection EdgeCollapse::getDirection() const {

    return direction;
}

void EdgeCollapse::setDirection(EdgeCollapse::DecimationDirection direction) {

    EdgeCollapse::direction = direction;
}

float EdgeCollapse::getMaximumError() const {

    return maximum_error;
}

void EdgeCollapse::setMaximumError(float new_maximum_error) {

    Q_ASSERT(new_maximum_error > 0);

    maximum_error = new_maximum_error;
}

float EdgeCollapse::getVertexReduction() const {

    return vertex_reduction;
}

void EdgeCollapse::setVertexReduction(float new_vertex_reduction) {

    Q_ASSERT(new_vertex_reduction > 0);
    Q_ASSERT(new_vertex_reduction < 1);

    EdgeCollapse::vertex_reduction = new_vertex_reduction;
}
