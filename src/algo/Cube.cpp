//
// Created by adam on 03.05.2021.
//

#include "./Cube.h"


QDebug operator<<(QDebug dbg, const SokarAlg::Cube &cube) {

	QDebugStateSaver saver(dbg);

	dbg.nospace() << "SokarAlg::Cube(" << Qt::endl
				  << cube.position[0] << "=" << cube.value[0] << Qt::endl
				  << cube.position[1] << "=" << cube.value[1] << Qt::endl
				  << cube.position[2] << "=" << cube.value[2] << Qt::endl
				  << cube.position[3] << "=" << cube.value[3] << Qt::endl
				  << cube.position[4] << "=" << cube.value[4] << Qt::endl
				  << cube.position[5] << "=" << cube.value[5] << Qt::endl
				  << cube.position[6] << "=" << cube.value[6] << Qt::endl
				  << cube.position[7] << "=" << cube.value[7] << Qt::endl
				  << ")";

	return dbg;
}
