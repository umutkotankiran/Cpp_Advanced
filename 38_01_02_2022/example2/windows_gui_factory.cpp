#include "windows_gui_factory.h"

#include "windows_menu.h"
#include "windows_button.h"
#include "windows_slider.h"
#include "windows_textbox.h"
#include "windows_checkbox.h"
#include <iostream>


WindowsGuiFactory::WindowsGuiFactory()
{
    std::cout << "WindowsGUiFactory created!\n";
}

MenuPtr WindowsGuiFactory::create_menu()
{
    return std::make_unique<WindowsMenu>();
}

ButtonPtr WindowsGuiFactory::create_button()
{
    return std::make_unique<WindowsButton>();
}

SliderPtr WindowsGuiFactory::create_slider()
{
    return std::make_unique<WindowsSlider>();
}

TextBoxPtr WindowsGuiFactory::create_textbox()
{
    return std::make_unique<WindowsTextBox>();
}

CheckBoxPtr WindowsGuiFactory::create_checkbox()
{
    return std::make_unique<WindowsCheckBox>();
}
