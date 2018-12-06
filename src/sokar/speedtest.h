#pragma once

#include <chrono>

#include <QtCore>

using namespace std::chrono;

namespace Sokar {
	class SpeedTest {

		high_resolution_clock::time_point start = high_resolution_clock::now();
		QString name;
		bool closed = false;

	public:


		SpeedTest(QString name) : name(name), start(high_resolution_clock::now()) {
		}

		~SpeedTest() {
			close();
		}

		void close() {
			if (closed) return;

			closed = true;

			auto finish = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(finish - start).count();

			qDebug() << "SpeedTest:" << name << "=" << duration;
		}
	};
}