
#include "windowint.h"

#include "sokar/palette.h"

using namespace Sokar::Monochrome2;


void WindowInt::genMenu() {

	{
		auto submenu = toolbarMenu.addMenu(tr("Color Palettes"));

		for (auto palette : Palette::getAll()) {
			auto action = new QAction(palette->getDisplay(), submenu);

			connect(action, &QAction::triggered, [this, palette](bool) {


				emit forceRefreshSignal();
			});

			submenu->addAction(action);
		}
	}

	toolbarMenu.addSection(tr("Windowing"));

	for (auto &win : defaultWindows) {
		auto action = new QAction();

		action->setText(
				(win.name.isEmpty() ? "" : (win.name + ", ")) +
				"C" + QString::number((int) win.center) + " " +
				"W" + QString::number((int) win.width)
		);

		connect(action, &QAction::triggered, [this, win](bool) {
			activateDefWin(win);
		});

		toolbarMenu.addAction(action);
	}

	{
		auto submenu = toolbarMenu.addMenu(tr("Predefined"));

		static QVector<DefaultWindow> prefeined;

		if (prefeined.isEmpty())
			prefeined
					<< DefaultWindow{0, 0, tr("Head And Neck")}
					<< DefaultWindow{40, 80, tr("Brain")}
					<< DefaultWindow{50, 130, tr("Subdural 1")}
					<< DefaultWindow{100, 300, tr("Subdural 2")}
					<< DefaultWindow{32, 8, tr("Stroke 1")}
					<< DefaultWindow{40, 40, tr("Stroke 2")}
					<< DefaultWindow{600, 2800, tr("Temporal Bones")}
					<< DefaultWindow{20, 350, tr("Soft Tissues 1")}
					<< DefaultWindow{60, 400, tr("Soft Tissues 2")}
					//
					<< DefaultWindow{0, 0, tr("Chest")}
					<< DefaultWindow{-600, 1500, tr("Lungs")}
					<< DefaultWindow{50, 350, tr("Mediastinum")}
					//
					<< DefaultWindow{0, 0, tr("Abdomen")}
					<< DefaultWindow{50, 400, tr("Soft Tissues")}
					<< DefaultWindow{30, 150, tr("Liver")}
					//
					<< DefaultWindow{0, 0, tr("Spine")}
					<< DefaultWindow{50, 250, tr("Soft Tissues")}
					<< DefaultWindow{400, 1800, tr("Bone")};

		for (auto &win : prefeined) {
			if (win.center == 0 and win.width == 0) {
				submenu->addSection(win.name);
			} else {
				auto action = new QAction(
						win.name + ", " +
						"C" + QString::number((int) win.center) + " " +
						"W" + QString::number((int) win.width),
						submenu);

				connect(action, &QAction::triggered, [this, win](bool) {
					activateDefWin(win);
				});

				submenu->addAction(action);
			}
		}
	}
}