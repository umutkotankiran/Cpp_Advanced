#include "linux_gui_factory.h"
#include "windows_gui_factory.h"
#include "ios_gui_factory.h"
#include "button.h"
#include "textbox.h"
#include "slider.h"
#include "menu.h"
#include "checkbox.h"


void func(GuiFactory *f)
{
	auto pbutton = f->create_button();
	pbutton->set_button_color();
	pbutton->set_button_size();
	pbutton->make_button_visible();
	pbutton->make_button_invisible();
	
	auto ptextbox = f->create_textbox();
	ptextbox->set_textbox_font();
	ptextbox->set_textbox_size();

	auto pslider = f->create_slider();
	pslider->set_slider_color();
	pslider->set_slider_length();
	pslider->set_slider_position();

	auto pmenu = f->create_menu();
	pmenu->set_menu_items();
	pmenu->set_menu_size();
	pmenu->set_menu_text();

	auto pcheckbox = f->create_checkbox();
	pcheckbox->set_checkbox_icon();
	pcheckbox->set_checkbox_size();
}


int main()
{
	//auto pf = new LinuxGuiFactory{};
	//auto pf = new WindowsGuiFactory{};
	auto pf = new IosGuiFactory{};
	func(pf);
	delete pf;
}