#pragma once


class CheckBox{
public:
	virtual ~CheckBox() = default;
	virtual void set_checkbox_icon() = 0;
	virtual void set_checkbox_size() = 0;
};