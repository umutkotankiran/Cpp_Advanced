#pragma once

class Slider {
public:
	virtual ~Slider() = default;
	virtual void set_slider_length() = 0;
	virtual void set_slider_color() = 0;
	virtual void set_slider_position() = 0;
};