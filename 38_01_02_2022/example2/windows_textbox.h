#pragma once

#include "textbox.h"

class WindowsTextBox : public TextBox {
public:
	WindowsTextBox();
	~WindowsTextBox();
	virtual void set_textbox_font() override;
	virtual void set_textbox_size() override;
};