#pragma once

#include "textbox.h"

class LinuxTextBox : public TextBox{
public:
	LinuxTextBox();
	~LinuxTextBox();
	virtual void set_textbox_font() override;
	virtual void set_textbox_size() override;
};