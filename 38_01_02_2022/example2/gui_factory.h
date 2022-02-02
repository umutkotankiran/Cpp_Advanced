#pragma once

#include <memory>

class Menu;
class Button;
class Slider;
class TextBox;
class CheckBox;

using MenuPtr = std::unique_ptr<Menu>;
using ButtonPtr = std::unique_ptr<Button>;
using SliderPtr = std::unique_ptr<Slider>;
using TextBoxPtr = std::unique_ptr<TextBox>;
using CheckBoxPtr = std::unique_ptr<CheckBox>;

class GuiFactory {
public:
	virtual ~GuiFactory() = default;
	virtual MenuPtr create_menu() = 0;
	virtual ButtonPtr create_button() = 0;
	virtual SliderPtr create_slider() = 0;
	virtual TextBoxPtr create_textbox() = 0;
	virtual CheckBoxPtr create_checkbox() = 0;
};
