#include <QtCore>
#include <QApplication>
#include <sokar/settings.h>

#include <QGuiApplication>
#include <win/mainwindow.h>
#include "./3d/VulkanRenderer.h"
#include "./3d/MeshPipeline.h"
#include "./3d/CenterCamera.h"

QSettings *Sokar::qSettings;

int main(int argc, char *argv[]) {

	QCoreApplication::setOrganizationName("Gauganian");
	QCoreApplication::setOrganizationDomain("sokar.gauganian.com");
	QCoreApplication::setApplicationName("Sokar");

	QApplication app(argc, argv);

	Sokar::qSettings = new QSettings();

//	Sokar::MainWindow w;
//	w.show();

	auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();

	auto mesh = new Sokar3D::Mesh();

	mesh->addTriangle(
			{
					glm::vec3{-1, -1, 0}
			},
			{
					glm::vec3{-1, 1, 0}
			},
			{
					glm::vec3{0, 0, 0}
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

	auto renderer = ret.renderer;
	renderer->addPipelineWrapper(meshpw);
	auto camera = new Sokar3D::CenterCamera(
			{0, 0, 0},
			10
	);
	renderer->setCamera(camera);

	return app.exec();
}
