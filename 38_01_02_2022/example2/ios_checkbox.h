#pragma once

#include "checkbox.h"

class IosCheckBox : public CheckBox{
public:
	IosCheckBox();
	~IosCheckBox();
	virtual void set_checkbox_icon() override;
	virtual void set_checkbox_size() override;
};

