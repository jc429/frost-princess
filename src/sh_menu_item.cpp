#include "sh_menu_item.h"
#include "sh_menu.h"


namespace sh
{
	menu_item::menu_item(menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<16> text)
	{
		_position = pos;
		_action_id = action_id;
		_text = text;
		_menu = menu_owner;
		_above = NULL;
		_below = NULL;
	}
	
	menu_item::~menu_item()
	{

	}

	// void menu_item::set_menu_action(void (*func)())
	// {
	// 	_menu_action = func;
	// }
	
	void menu_item::link_above(menu_item *other)
	{
		_above = other;
		if(other != NULL)
			other->_below = this;
	}

	void menu_item::link_below(menu_item *other)
	{
		_below = other;
		if(other != NULL)
			other->_above = this;
	}

	void menu_item::select()
	{
		if(_menu == NULL)
		{
			return;
		}
		_menu->highlight_menu_item(this);
	}

	void menu_item::show_item()
	{
		
	}

	void menu_item::hide_item()
	{

	}

	void menu_item::interact_with_item(menu_item_interact_type interaction)
	{
		switch(interaction)
		{
		case menu_item_interact_type::UP_PRESS:
			if(_above != NULL)
			{
				_above->select();
			}
			break;
		case menu_item_interact_type::DOWN_PRESS:
			if(_below != NULL)
			{
				_below->select();
			}
			break;
		case menu_item_interact_type::A_PRESS:
			_menu->perform_action(_action_id);
			break;
		default:
			break;
		}
	}


	


}