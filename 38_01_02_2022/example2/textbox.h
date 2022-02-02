#pragma once

class TextBox{
public:
	virtual ~TextBox() = default;
	virtual void set_textbox_font() = 0;
	virtual void set_textbox_size() = 0;
};