#pragma once

#include "slider.h"

class LinuxSlider : public Slider {
public:
	LinuxSlider();
	~LinuxSlider();
	virtual void set_slider_length() override;
	virtual void set_slider_color() override;
	virtual void set_slider_position() override;
};