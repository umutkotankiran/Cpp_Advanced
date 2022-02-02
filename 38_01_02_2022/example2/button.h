#pragma once


class Button {
public:
	virtual ~Button() = default;
	virtual void set_button_color() = 0;
	virtual void set_button_size() = 0;
	virtual void make_button_visible() = 0;
	virtual void make_button_invisible() = 0;
};