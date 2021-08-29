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
    decimator.setTargetVertices(qFloor(vertex_reduction * float(bound_mesh->nVertices())));
    decimator.setMesh(*bound_mesh);

    auto decimated_mesh = decimator.compute();
    auto output_mesh = Sokar3D::IndexedMesh::New();

    auto vert_num = decimated_mesh->nVertices();


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
