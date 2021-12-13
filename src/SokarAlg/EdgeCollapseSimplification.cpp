//
// Created by adam on 13.12.2021.
//

#include "EdgeCollapseSimplification.hpp"
#include "libigl_mesh.hpp"

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

    libigl_mesh qq;

    Eigen::MatrixXd &V = qq.verts;
    Eigen::MatrixXi &F = qq.faces;

    Eigen::VectorXi EMAP;
    Eigen::MatrixXi E, EF, EI;
    igl::min_heap<std::tuple<double, int, int> > Q;
    Eigen::VectorXi EQ;
    // If an edge were collapsed, we'd collapse it to these points:
    Eigen::MatrixXd C;
    int num_collapsed;

    igl::edge_flaps(F, E, EMAP, EF, EI);
    C.resize(E.rows(), V.cols());
    Eigen::VectorXd costs(E.rows());
    // https://stackoverflow.com/questions/2852140/priority-queue-clear-method
    // Q.clear();
    Q = {};
    EQ = Eigen::VectorXi::Zero(E.rows());
    {
        Eigen::VectorXd costs(E.rows());
        igl::parallel_for(E.rows(), [&](const int e) {
            double cost = e;
            Eigen::RowVectorXd p(1, 3);
            igl::shortest_edge_and_midpoint(e, V, F, E, EMAP, EF, EI, cost, p);
            C.row(e) = p;
            costs(e) = cost;
        }, 10000);
        for (int e = 0; e < E.rows(); e++) {
            Q.emplace(costs(e), e, 0);
        }
    }

    num_collapsed = 0;

    bool something_collapsed = false;
    // collapse edge
    const int max_iter = std::ceil(0.01 * Q.size());
    for (int j = 0; j < max_iter; j++) {
        if (!igl::collapse_edge(igl::shortest_edge_and_midpoint, V, F, E, EMAP, EF, EI, Q, EQ, C)) {
            break;
        }
        something_collapsed = true;
        num_collapsed++;
    }

    return Sokar3D::IndexedMeshPtr();
}
