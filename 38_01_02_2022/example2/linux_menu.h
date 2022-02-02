#pragma once

#include "menu.h"

class LinuxMenu : public Menu {
public:
	LinuxMenu();
	~LinuxMenu();
	virtual void set_menu_size() override;
	virtual void set_menu_text() override;
	virtual void set_menu_items() override;
};