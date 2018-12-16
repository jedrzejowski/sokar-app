#pragma once

#include <QtCore>
#include <QtWidgets>

namespace Sokar {
	namespace About {

		static void Sokar(QWidget *window) {
			static QString title = QObject::tr("About Sokar - Sokar");

			static QString text = QObject::tr(
					"<b>Sokar</b> to jest moja przeglądarka, i huj"

			).arg(
					""
			);

			QMessageBox::about(window, title, text);
		}

		static void GDCM(QWidget *window) {

			static QString title = QObject::tr("About GDCM - Sokar");

			static QString text = QObject::tr(
					"<b>Grassroots DiCoM</b> is a <b>C++</b> library for DICOM medical files. It is accessible from Python, C#, Java and PHP. It supports RAW, JPEG, JPEG 2000, JPEG-LS, RLE and deflated transfer syntax.<br>"
					"It comes with a super fast scanner implementation to quickly scan hundreds of DICOM files.<br>"
					"It supports SCU network operations (C-ECHO, C-FIND, C-STORE, C-MOVE). PS 3.3 & 3.6 are distributed as XML files.<br>"
					"It also provides PS 3.15 certificates and password based mecanism to anonymize and de-identify DICOM datasets.<br>"
					"<br>"
					"Features:"
					"<ul>"
					"<li>RAW, JPEG, JPEG 2000, JPEG-LS, RLE and deflated</li>"
					"<li>Quickly scan DICOM for attributes</li>"
					"<li>SCU: C-ECHO, C-FIND, C-STORE, C-MOVE</li>"
					"<li>PS 3.3 & PS 3.6/3.7 as XML files</li>"
					"<li>PS 3.15 de-identify / re-identify (certificates+password based)</li>"
					"<li>Portable C++</li>"
					"<li>Bindings: Python, C#, Java, PHP and Perl</li>"
					"<li>VTK bridge (ImageData and RTSTRUCT)</li>"
					"<li>Nightly test suite</li>"
					"<li>Support well-known DICOM issues (PMSCT_RLE1, JAI JPEG-LS, Signed Short JPEG)</li>"
					"</ul>"
					"Official sire: <a href=\"%1\">%1/</a><br>"
					"SourceForge profile: <a href=\"%2\">%2/</a><br>"
					"License: <a href=\"%3\">BSD licenses</a>, <a href=\"%4\">Apache License V2.0/</a>"

			).arg(
					/* %1 */"http://gdcm.sourceforge.net/",
					/* %2 */"https://sourceforge.net/projects/gdcm/",
					/* %3 */"https://en.wikipedia.org/wiki/BSD_licenses",
					/* %4 */"https://pl.wikipedia.org/wiki/Apache_License"
			);

			QMessageBox::about(window, title, text);
		}

	}
}

