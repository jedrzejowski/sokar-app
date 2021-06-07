//
// Created by adam on 03.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "VolumeSegmentator.hpp"
#include "./IndexedMesh.hpp"

using namespace SokarAlg;

void VolumeSegmentator::execBefore() {

	if (mesh != nullptr) {
		mesh.clear();
	}

	mesh = QSharedPointer<MeshType>::create();
}

QFuture<void> VolumeSegmentator::execAsync() {

	execBefore();
	return QtConcurrent::run([&]() {
		execAlg();
	});
}

void VolumeSegmentator::execSync() {
	execBefore();
	execAlg();
}

const QSharedPointer<const Volume> &VolumeSegmentator::getVolume() const {
	return volume;
}

void VolumeSegmentator::setVolume(const QSharedPointer<const Volume> &vv) {
	volume = vv;
}

void VolumeSegmentator::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {
	mesh->addTriangle(v0, v1, v2);
}

const QSharedPointer<VolumeSegmentator::MeshType> &VolumeSegmentator::getMesh() const {
	return mesh;
}

float VolumeSegmentator::getIsoLevel() const {
	return isoLevel;
}

void VolumeSegmentator::setIsoLevel(float isoLevel) {
	VolumeSegmentator::isoLevel = isoLevel;
}

VolumeSegmentator::Compare VolumeSegmentator::getIsoCompare() const {
	return isoCompare;
}

void VolumeSegmentator::setIsoCompare(VolumeSegmentator::Compare isoCompare) {
	VolumeSegmentator::isoCompare = isoCompare;
}