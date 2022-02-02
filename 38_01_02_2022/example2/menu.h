#pragma once

class Menu{
public:
	virtual ~Menu() = default;
	virtual void set_menu_size() = 0;
	virtual void set_menu_text() = 0;
	virtual void set_menu_items() = 0;
};