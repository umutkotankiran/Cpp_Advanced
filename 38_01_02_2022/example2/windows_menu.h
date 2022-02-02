#pragma once

#include "menu.h"

class WindowsMenu : public Menu {
public:
	WindowsMenu();
	~WindowsMenu();
	virtual void set_menu_size() override;
	virtual void set_menu_text() override;
	virtual void set_menu_items() override;
};
