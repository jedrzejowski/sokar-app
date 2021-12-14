//
// Created by adam on 13.12.2021.
//

#include "EdgeCollapseSimplification.hpp"

#include <igl/circulation.h>
#include <igl/collapse_edge.h>
#include <igl/edge_flaps.h>
#include <igl/decimate.h>
#include <igl/shortest_edge_and_midpoint.h>
#include <igl/parallel_for.h>
#include <igl/read_triangle_mesh.h>
#include <Eigen/Core>
#include <set>


using namespace SokarAlg;

EdgeCollapseSimplification::EdgeCollapseSimplification() {

}

EdgeCollapseSimplificationPtr EdgeCollapseSimplification::New() {

    return SokarAlg::EdgeCollapseSimplificationPtr(new EdgeCollapseSimplification);
}

QString EdgeCollapseSimplification::toDisplay() {

    return QString();
}

Sokar3D::IndexedMeshPtr EdgeCollapseSimplification::exec() {

    qDebug() << "0";

    auto my_vertex_2_igl_mesh = QHash<Sokar3D::IndexedMesh::size_type, boundingmesh::Index>();

    auto &verts = mesh->getVertices();
    auto &faces = mesh->getFaces();

    Eigen::MatrixXd V(verts.size(), 3);
    Eigen::MatrixXi F(faces.size(), 3);

    int i = -1;
    for (auto iter = mesh->getVertices().constKeyValueBegin();
         iter != mesh->getVertices().constKeyValueEnd(); ++iter) {

        ++i;

        V(i, 0) = verts[i].x;
        V(i, 1) = verts[i].y;
        V(i, 2) = verts[i].z;

        my_vertex_2_igl_mesh[iter->first] = i;
    }

    for (const auto &face: mesh->getFaces()) {

        V(i, 0) = my_vertex_2_igl_mesh[face.i0];
        V(i, 1) = my_vertex_2_igl_mesh[face.i1];
        V(i, 2) = my_vertex_2_igl_mesh[face.i2];

    }

//    Eigen::MatrixXd U;
//    Eigen::MatrixXi G;
//    Eigen::VectorXi J;
//    Eigen::VectorXi I;
//
//    igl::decimate(V, F, 10000, U, G, J, I);

    auto output_mesh = Sokar3D::IndexedMesh::New();

    qDebug() << "V.cols()" << V.cols();
    qDebug() << "F.cols()" << F.cols();
    qDebug() << "V.rows()" << V.rows();
    qDebug() << "F.rows()" << F.rows();

    for (int i = 0; i < V.rows(); ++i) {
        auto col = V.row(i);

        qDebug() << glm::vec3(col(0), col(1), col(2));
        output_mesh->addVertex(glm::vec3(col(0), col(1), col(2)), false);
    }

    for (int i = 0; i < F.rows(); ++i) {
        auto col = F.row(i);

        output_mesh->addTriangle(
                col(0),
                col(1),
                col(2)
        );
    }

    return output_mesh;
}

float EdgeCollapseSimplification::getVertexReduction() const {

    return vertex_reduction;
}

void EdgeCollapseSimplification::setVertexReduction(float vertexReduction) {

    vertex_reduction = vertexReduction;
}
