#include "sh_menu_item.h"
#include "sh_menu.h"


namespace sh
{
	menu_item::menu_item(menu *menu_owner, int id, bn::fixed_point pos, menu_action_id action_id)
	{
		_item_id = id;
		_position = pos;
		_action_id = action_id;
		_text = NULL;
		_menu = menu_owner;
		_above = NULL;
		_below = NULL;
		_left = NULL;
		_right = NULL;
		hover_menu_item_event = NULL;
		click_menu_item_event = NULL;
	}

	menu_item::menu_item(menu *menu_owner, int id, bn::fixed_point pos, menu_action_id action_id, bn::string<MENU_STRING_MAX_LEN> text)
	{
		_item_id = id;
		_position = pos;
		_action_id = action_id;
		_text = text;
		_menu = menu_owner;
		_above = NULL;
		_below = NULL;
		_left = NULL;
		_right = NULL;
		hover_menu_item_event = NULL;
		click_menu_item_event = NULL;
	}
	
	menu_item::~menu_item()
	{
		_text.clear();
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
	
	void menu_item::link_left(menu_item *other)
	{
		_left = other;
		if(other != NULL)
			other->_right = this;
	}

	void menu_item::link_right(menu_item *other)
	{
		_right = other;
		if(other != NULL)
			other->_left = this;
	}

	void menu_item::select()
	{
		if(_menu == NULL)
		{
			return;
		}
		_menu->highlight_menu_item(this);
		if(hover_menu_item_event != NULL)
		{
			hover_menu_item_event(_item_id);
		}
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
		case menu_item_interact_type::LEFT_PRESS:
			if(_left != NULL)
			{
				_left->select();
			}
			break;
		case menu_item_interact_type::RIGHT_PRESS:
			if(_right != NULL)
			{
				_right->select();
			}
			break;
		case menu_item_interact_type::A_PRESS:
			_menu->perform_action(_action_id);
			if(click_menu_item_event != NULL)
			{
				click_menu_item_event(_item_id);
			}
			break;
		default:
			break;
		}
	}

}