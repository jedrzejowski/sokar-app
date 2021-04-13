#include <QtCore>
#include <QApplication>
#include <sokar/settings.h>

#include <QGuiApplication>
#include <win/mainwindow.h>
#include "./3d/Renderer.h"
#include "./3d/MeshPipeline.h"

QSettings *Sokar::qSettings;

int main(int argc, char *argv[]) {

	QCoreApplication::setOrganizationName("Gauganian");
	QCoreApplication::setOrganizationDomain("sokar.gauganian.com");
	QCoreApplication::setApplicationName("Sokar");

	QApplication app(argc, argv);

	Sokar::qSettings = new QSettings();

//	Sokar::MainWindow w;
//	w.show();

	auto ret = Sokar3D::VulkanWidget::New<Sokar3D::Renderer>();

	auto mesh = new Sokar3D::Mesh();

	mesh->addTriangle(
			{
					glm::vec3{0, 0, 0}
			},
			{
					glm::vec3{1, -1, 0}
			},
			{
					glm::vec3{1, 1, 0}
			}
	);
	mesh->addTriangle(
			{
					glm::vec3{-1, -1, 0}
			},
			{
					glm::vec3{0, 0, 0}
			},
			{
					glm::vec3{-1, 1, 0}
			}
	);

	auto meshpw = new Sokar3D::MeshPipeline(mesh);

	ret.renderer->addPipelineWrapper(meshpw);

	return app.exec();
}
