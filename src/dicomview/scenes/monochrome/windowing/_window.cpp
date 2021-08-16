
#include "_window.h"

using namespace Sokar;
using namespace Sokar::Monochrome;

Window::Window(SokarDicom::DataConverter &dataConverter) : Indicator(dataConverter) {}

QMenu *Window::getMenu() {
	if (toolbarMenu.isEmpty()) genMenu();
	return &toolbarMenu;
}

void Window::setInversed(bool inversed) {
	if (Window::inversed == inversed)
		return;

	Window::inversed = inversed;
	shouldRegen = true;
}

void Window::setPalette(Sokar::Palette *palette) {
	if (Window::palette == palette)
		return;

	Window::palette = palette;
	shouldRegen = true;
}

bool Window::isAlive() {
	return true;
}

void Window::updateLastChange() {
	lastChange = std::chrono::high_resolution_clock::now();
}
