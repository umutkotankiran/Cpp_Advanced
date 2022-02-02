#pragma once

#include "button.h"


class IosButton : public Button {
public:
	IosButton();
	~IosButton();
	virtual void set_button_color() override;
	virtual void set_button_size() override;
	virtual void make_button_visible() override;
	virtual void make_button_invisible() override;
};
