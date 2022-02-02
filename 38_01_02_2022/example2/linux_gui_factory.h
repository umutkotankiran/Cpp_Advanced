#pragma once

#include "gui_factory.h"

class LinuxGuiFactory : public GuiFactory {
public:
	LinuxGuiFactory();
	virtual std::unique_ptr<Menu> create_menu() override;
	virtual std::unique_ptr<Button> create_button() override;
	virtual std::unique_ptr<Slider> create_slider() override;
	virtual std::unique_ptr<TextBox> create_textbox() override;
	virtual std::unique_ptr<CheckBox> create_checkbox() override;
};