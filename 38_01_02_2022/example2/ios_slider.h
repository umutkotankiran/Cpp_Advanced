#pragma once

#include "slider.h"

class IosSlider : public Slider {
public:
	IosSlider();
	~IosSlider();
	virtual void set_slider_length() override;
	virtual void set_slider_color() override;
	virtual void set_slider_position() override;
};