#pragma once

#include "button.h"


class LinuxButton : public Button {
public:
	LinuxButton();
	~LinuxButton();
	virtual void set_button_color() override;
	virtual void set_button_size() override;
	virtual void make_button_visible() override;
	virtual void make_button_invisible() override;
};

