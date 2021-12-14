//
// Created by adam on 30.08.2021.
//

#include "BoundingConvexDecomposition.hpp"
#include "boundingmesh/Decimator.h"
#include "boundingmesh/Mesh.h"
#include "boundingmesh/SegmenterDownsampling.h"
#include "optionparser/optionparser.h"

using namespace SokarAlg;

BoundingConvexDecomposition::BoundingConvexDecomposition() {

}

BoundingConvexDecompositionPtr BoundingConvexDecomposition::New() {

    return BoundingConvexDecompositionPtr(new BoundingConvexDecomposition);
}



QString BoundingConvexDecomposition::toDisplay() {

    return QString();
}

Sokar3D::IndexedMeshPtr BoundingConvexDecomposition::exec() {

//    auto bound_mesh = mesh->toBoundingMesh();

//    boundingmesh::Real bounding_box_diagonal = bound_mesh->getBoundingBoxDiagonal();
//    std::cout << "Filling holes in geometry..." << std::endl;
//    bound_mesh->closeHoles();
//
//    boundingmesh::SegmenterDownsampling segmenter;
//    segmenter.setMaxPasses(10);
//    segmenter.setAlpha(alpha);
//    segmenter.setDelta(alpha / 2);
//
//    std::cout << "Voxelizing mesh..." << std::endl;
//    segmenter.setMesh(bound_mesh, voxels);
//    std::cout << "Calculating bounding convex decomposition..." << std::endl;
//    segmenter.compute();
//    std::vector<std::shared_ptr<boundingmesh::Mesh> > decomposition =
//            segmenter.getSegmentation();
//
//    std::cout << "Simplifying bounding convex decomposition with the bounding "
//                 "mesh algorithm..."
//              << std::endl;
//    std::vector<std::shared_ptr<boundingmesh::Mesh> > decomposition_decimated;
//    for (int i = 0; i < decomposition.size(); ++i) {
//        boundingmesh::Decimator decimator;
//        decimator.setMesh(*decomposition[i]);
//        decimator.setMaximumError(
//                std::pow(target_error * bounding_box_diagonal, 2));
//        decomposition_decimated.push_back(decimator.compute());
//    }

    return Sokar3D::IndexedMeshPtr();
}

//region getter & setters

float BoundingConvexDecomposition::getAlpha() const {

    return alpha;
}

void BoundingConvexDecomposition::setAlpha(float new_alpha) {

    alpha = new_alpha;
}

float BoundingConvexDecomposition::getDelta() const {

    return delta;
}

void BoundingConvexDecomposition::setDelta(float new_delta) {

    delta = new_delta;
}

float BoundingConvexDecomposition::getMaxPasses() const {

    return new_passes;
}

void BoundingConvexDecomposition::setMaxPasses(float new_max_passes) {

    new_passes = new_max_passes;
}

float BoundingConvexDecomposition::getVertexRatio() const {

    return vertex_ratio;
}

void BoundingConvexDecomposition::setVertexRatio(float vertexRatio) {

    vertex_ratio = vertexRatio;
}

float BoundingConvexDecomposition::getTargetError() const {

    return target_error;
}

void BoundingConvexDecomposition::setTargetError(float targetError) {

    target_error = targetError;
}

//endregion