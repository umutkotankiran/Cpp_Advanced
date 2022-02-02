#include "ios_gui_factory.h"
#include "ios_menu.h"
#include "ios_button.h"
#include "ios_slider.h"
#include "ios_textbox.h"
#include "ios_checkbox.h"
#include <iostream>

IosGuiFactory::IosGuiFactory()
{
    std::cout << "IosGUiFactory created!\n";
}

MenuPtr IosGuiFactory::create_menu()
{
    return std::make_unique<IosMenu>();
}

ButtonPtr IosGuiFactory::create_button()
{
    return std::make_unique<IosButton>();
}

SliderPtr IosGuiFactory::create_slider()
{
    return std::make_unique<IosSlider>();
}

TextBoxPtr IosGuiFactory::create_textbox()
{
    return std::make_unique<IosTextBox>();
}

CheckBoxPtr IosGuiFactory::create_checkbox()
{
    return std::make_unique<IosCheckBox>();
}
