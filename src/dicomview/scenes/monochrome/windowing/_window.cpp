
#include "_window.h"

using namespace Sokar::Monochrome;

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
