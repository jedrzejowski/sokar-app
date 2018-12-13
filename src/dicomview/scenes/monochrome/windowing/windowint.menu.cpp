
#include "windowint.h"

#include "sokar/palette.h"

using namespace Sokar::Monochrome;


void WindowInt::genMenu() {

	{
		auto submenu = toolbarMenu.addMenu(tr("Color Palettes"));

		auto actionGroup = new QActionGroup(this);

		for (auto palette : Palette::getAll()) {
			auto action = submenu->addAction(palette->getDisplay());

			action->setActionGroup(actionGroup);
			action->setCheckable(true);

			if (palette == Window::palette)
				action->setChecked(true);

			connect(action, &QAction::toggled, this, [&](bool checked) {
				if (!checked) return;
				setPalette(palette);
				emit forceRefreshSignal();
			});
		}
	}


	{
		toolbarMenu.addSection(tr("Windowing"));

		if (defaultWindows.isEmpty()) {

			auto action = toolbarMenu.addAction(tr("No Windows In File"));
			action->setDisabled(true);

		} else {

			for (auto &win : defaultWindows) {
				auto action = toolbarMenu.addAction(
						(win.name.isEmpty() ? "" : (win.name + ", ")) +
						"C" + QString::number((int) win.center) + " " +
						"W" + QString::number((int) win.width)
				);

				connect(action, &QAction::triggered, this, [&](bool) {
					activateDefWin(win);
				});
			}
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
					auto action = submenu->addAction(
							win.name + ", " +
							"C" + QString::number((int) win.center) + " " +
							"W" + QString::number((int) win.width)
					);

					connect(action, &QAction::triggered, this, [&](bool) {
						activateDefWin(win);
					});
				}
			}
		}
	}

	toolbarMenu.addSeparator();

	{
		auto action = toolbarMenu.addAction(tr("Inverse"));
		connect(action, &QAction::triggered, this, [&](bool) {
			setInversed(!isInversed());
			emit forceRefreshSignal();
		});
	}
}