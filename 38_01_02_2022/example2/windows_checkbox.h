#pragma once
#include "checkbox.h"



class WindowsCheckBox : public CheckBox{
public:
	WindowsCheckBox();
	~WindowsCheckBox();
	virtual void set_checkbox_icon() override;
	virtual void set_checkbox_size() override;
};