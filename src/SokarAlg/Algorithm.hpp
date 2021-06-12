//
// Created by adam on 12.06.2021.
//

#pragma once

#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QSharedPointer>
#include "SokarAlg.hpp"

namespace SokarAlg {

	template<typename Output>
	class Algorithm : public QEnableSharedFromThis<Algorithm<Output>> {
	protected:

		virtual Output exec() = 0;

	public:

		Output execSync();

		QFuture<Output> execAsync();
	};

	template<typename Output>
	Output Algorithm<Output>::execSync() {
		return exec();
	}

	template<typename Output>
	QFuture<Output> Algorithm<Output>::execAsync() {
		auto self = this->sharedFromThis();

		return QtConcurrent::run([self]() {
			return self->execSync();
		});
	}
}



