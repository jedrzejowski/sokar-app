//
// Created by adam on 12.06.2021.
//

#include "WavefrontObjBuilder.hpp"

using namespace SokarLib;
using Size = WavefrontObjBuilder::Size;

Size WavefrontObjBuilder::addVertex(const glm::vec3 &v) {
	vertices << "v" << space << v.x << space << v.y << space << v.z << endl;
	return ++verticesI;
}

Size WavefrontObjBuilder::addTextureCord(const glm::vec2 &vt) {
	texture << "t" << space << vt.x << space << vt.y << endl;
	return ++textureI;
}

Size WavefrontObjBuilder::addNormal(const glm::vec3 &vn) {
	normals << "vn" << space << vn.x << space << vn.y << space << vn.z << endl;
	return ++normalsI;
}

Size WavefrontObjBuilder::addFaceV(
		const Size &v1,
		const Size &v2,
		const Size &v3
) {
	faces << "f" << space << v1 << space << v2 << space << v3 << endl;
	return ++facesI;
}

Size WavefrontObjBuilder::addFaceVT(
		const Size &v1, const Size &t1,
		const Size &v2, const Size &t2,
		const Size &v3, const Size &t3
) {
	faces << "f" << space
		  << v1 << slash << t1 << space
		  << v1 << slash << t2 << space
		  << v1 << slash << t3 << endl;
	return ++facesI;
}

Size WavefrontObjBuilder::addFaceVTN(
		const Size &v1, const Size &t1, const Size &n1,
		const Size &v2, const Size &t2, const Size &n2,
		const Size &v3, const Size &t3, const Size &n3
) {
	faces << "f" << space
		  << v1 << slash << t1 << slash << n1 << space
		  << v1 << slash << t2 << slash << n2 << space
		  << v1 << slash << t3 << slash << n3 << endl;
	return ++facesI;
}

Size WavefrontObjBuilder::addFaceVN(
		const Size &v1, const Size &n1,
		const Size &v2, const Size &n2,
		const Size &v3, const Size &n3
) {
	faces << "f" << space
		  << v1 << slash << << n1 << space
		  << v1 << slash << << n2 << space
		  << v1 << slash << << n3 << endl;
	return ++facesI;
}

QByteArray WavefrontObjBuilder::dump2byteArray()const {
	auto array = QByteArray();

	array.push_back(vertices.str().c_str());
	array.push_back(texture.str().c_str());
	array.push_back(normals.str().c_str());
	array.push_back(faces.str().c_str());

	return array;
}

