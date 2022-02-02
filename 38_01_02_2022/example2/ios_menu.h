#pragma once

#include "menu.h"

class IosMenu : public Menu {
public:
	IosMenu();
	~IosMenu();
	virtual void set_menu_size() override;
	virtual void set_menu_text() override;
	virtual void set_menu_items() override;
};