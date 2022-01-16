#include "sh_level_select_menu.h"



namespace sh
{
	static bn::vector<menu_item, 8> _menu_items_;

	level_select_menu::level_select_menu() :
		menu(menu_type::LEVEL_SELECT_MENU, 1)
	{
		_cursor_sprite.set_visible(false);	//unused cursor

		_cursor.set_size(bn::point(48,48));
		_cursor.set_position(bn::fixed_point(0,0));
		_cursor.set_visible(true);

		build_level_select_menu();

		highlight_menu_item(_item_list.front());
	}


	level_select_menu::~level_select_menu()
	{
		_menu_items_.clear();
	}

	void level_select_menu::update()
	{
		_cursor.update();
		menu::update();
	}

	void level_select_menu::highlight_menu_item(menu_item *item)
	{
		_current_item = item;
		if(item == NULL)
		{
			_cursor.set_visible(false);
		}
		else{
			_cursor.set_position(item->_position);
			_cursor.set_visible(true);
		}
	}


	void level_select_menu::build_level_select_menu()
	{
		#define CENTER		0
		#define TOP_LEFT	1
		#define TOP_RIGHT	2
		#define MID_LEFT	3
		#define MID_RIGHT	4
		#define BOT_LEFT	5
		#define BOT_RIGHT	6

		bn::fixed_point pos(0,0);
		{
			// _menu_items_.push_back(menu_item(this, pos, menu_action_id::GO_TO_BATTLE, "Continue"));
			_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, ""));
			_item_list.push_back(&_menu_items_.back());
		}

		// TOP_LEFT
		pos = bn::fixed_point(-32, -40);
		{
			_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, ""));
			_item_list.push_back(&_menu_items_.back());
		}
		// TOP_RIGHT
		pos = bn::fixed_point(32, -40);
		{
			_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, ""));
			_item_list.push_back(&_menu_items_.back());
		}

		// MID_LEFT
		pos = bn::fixed_point(-56, 0);
		{
			_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, ""));
			_item_list.push_back(&_menu_items_.back());
		}
		// MID_RIGHT
		pos = bn::fixed_point(56, 0);
		{
			_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, ""));
			_item_list.push_back(&_menu_items_.back());
		}

		// BOT_LEFT
		pos = bn::fixed_point(-32, 40);
		{
			_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, ""));
			_item_list.push_back(&_menu_items_.back());
		}
		// BOT_RIGHT
		pos = bn::fixed_point(32, 40);
		{
			_menu_items_.push_back(menu_item(this, pos, menu_action_id::NONE, ""));
			_item_list.push_back(&_menu_items_.back());
		}

		_item_list.at(CENTER)->link_above(_item_list.at(TOP_LEFT));
		_item_list.at(CENTER)->link_below(_item_list.at(BOT_RIGHT));
		_item_list.at(TOP_LEFT)->link_left(_item_list.at(MID_LEFT));
		_item_list.at(TOP_RIGHT)->link_right(_item_list.at(MID_RIGHT));
		_item_list.at(BOT_LEFT)->link_left(_item_list.at(MID_LEFT));
		_item_list.at(BOT_RIGHT)->link_right(_item_list.at(MID_RIGHT));
		_item_list.at(MID_RIGHT)->link_right(_item_list.at(MID_LEFT));

		_item_list.at(CENTER)->link_right(_item_list.at(MID_RIGHT));
		_item_list.at(CENTER)->link_left(_item_list.at(MID_LEFT));

		_item_list.at(TOP_LEFT)->link_right(_item_list.at(TOP_RIGHT));
		_item_list.at(TOP_LEFT)->link_below(_item_list.at(MID_LEFT));
		_item_list.at(TOP_RIGHT)->link_below(_item_list.at(MID_RIGHT));

		_item_list.at(BOT_LEFT)->link_right(_item_list.at(BOT_RIGHT));
		_item_list.at(BOT_LEFT)->link_above(_item_list.at(MID_LEFT));
		_item_list.at(BOT_RIGHT)->link_above(_item_list.at(MID_RIGHT));

		_item_list.at(BOT_LEFT)->link_below(_item_list.at(TOP_LEFT));
		_item_list.at(BOT_RIGHT)->link_below(_item_list.at(TOP_RIGHT));

	}

}