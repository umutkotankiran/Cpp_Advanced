#include "linux_gui_factory.h"

#include "linux_menu.h"
#include "linux_button.h"
#include "linux_slider.h"
#include "linux_textbox.h"
#include "linux_checkbox.h"
#include <iostream>

LinuxGuiFactory::LinuxGuiFactory()
{
    std::cout << "LinuxGUiFactory created!\n";
}

MenuPtr LinuxGuiFactory::create_menu()
{
    return std::make_unique<LinuxMenu>();
}

ButtonPtr LinuxGuiFactory::create_button()
{
    return std::make_unique<LinuxButton>();
}

SliderPtr LinuxGuiFactory::create_slider()
{
    return std::make_unique<LinuxSlider>();
}

TextBoxPtr LinuxGuiFactory::create_textbox()
{
    return std::make_unique<LinuxTextBox>();
}

CheckBoxPtr LinuxGuiFactory::create_checkbox()
{
    return std::make_unique<LinuxCheckBox>();
}
