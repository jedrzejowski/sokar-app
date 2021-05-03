//
// Created by adam on 03.05.2021.
//

#include "./Triangle.h"

QDebug operator<<(QDebug dbg, const SokarAlg::Triangle &tri) {

	QDebugStateSaver saver(dbg);

	dbg.nospace() << "Triangle(" << Qt::endl
				  << tri.vertex0 << Qt::endl
				  << tri.vertex1 << Qt::endl
				  << tri.vertex2 << Qt::endl
				  << ")";

	return dbg;
}
