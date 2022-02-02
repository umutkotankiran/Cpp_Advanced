#pragma once

#include "slider.h"

class WindowsSlider : public Slider {
public:
	WindowsSlider();
	~WindowsSlider();
	virtual void set_slider_length() override;
	virtual void set_slider_color() override;
	virtual void set_slider_position() override;
};