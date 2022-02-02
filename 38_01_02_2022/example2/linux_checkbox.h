#pragma once

#include "checkbox.h"

class LinuxCheckBox : public CheckBox{
public:
	LinuxCheckBox();
	~LinuxCheckBox();
	virtual void set_checkbox_icon() override;
	virtual void set_checkbox_size() override;
};