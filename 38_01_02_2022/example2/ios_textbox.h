#pragma once

#include "textbox.h"

class IosTextBox : public TextBox {
public:
	IosTextBox();
	~IosTextBox();
	virtual void set_textbox_font() override;
	virtual void set_textbox_size() override;
};