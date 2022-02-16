#include "sh_level_select_menu.h"

#include <bn_sprite_item.h>
#include <bn_sprite_builder.h>

#include "bn_sprite_items_level_select_cursor.h"

namespace sh
{
	static bn::vector<menu_item, 6> _menu_items_;

	level_select_menu::level_select_menu() :
		menu(menu_type::LEVEL_SELECT_MENU, 1)
	{
		_cursor_sprite.set_visible(false);	//unused cursor

		// _lev_sel_cursor.set_size(bn::point(48,24));
		// _lev_sel_cursor.set_position(bn::fixed_point(0,0));
		// _lev_sel_cursor.set_visible(true);
		{
			bn::sprite_builder builder(bn::sprite_items::level_select_cursor);
			builder.set_bg_priority(1);
			builder.set_z_order(0);

			_cursors.push_back(builder.build());
			_cursors.back().set_tiles(bn::sprite_items::level_select_cursor.tiles_item(), 0);
			_selected_items.push_back(NULL);

			_cursors.push_back(builder.release_build());
			_cursors.back().set_tiles(bn::sprite_items::level_select_cursor.tiles_item(), 1);
			_selected_items.push_back(NULL);
		}
		_active_cursor = 0;

		build_level_select_menu();


		highlight_menu_item(_item_list.front());
	}


	level_select_menu::~level_select_menu()
	{
		_menu_items_.clear();
	}

	void level_select_menu::update()
	{
		// _lev_sel_cursor.update();
		menu::update();
	}

	void level_select_menu::set_cursor_visible(bool visible)
	{
		// _lev_sel_cursor.set_visible(visible);
	}

	void level_select_menu::set_active_cursor(int cursor_id)
	{
		_active_cursor = cursor_id;
		for(int i = 0; i < _cursors.size(); i++)
		{
			int z_order = (cursor_id == i) ? -100 : -50;
			_cursors.at(i).set_z_order(z_order);
		}
	}

	void level_select_menu::highlight_menu_item(menu_item *item)
	{
		// _current_item = item;
		_selected_items.at(_active_cursor) = item;
		if(item == NULL)
		{
			// set_cursor_visible(false);
		}
		else{
			// _lev_sel_cursor.set_position(item->_position);
			_cursors.at(_active_cursor).set_position(item->_position);
			// set_cursor_visible(true);
		}
	}

	menu_item *level_select_menu::current_item()
	{
		return _selected_items.at(_active_cursor);
	}

	void level_select_menu::move_all_cursors_to_item(menu_item *item)
	{
		for(int i = 0; i < NUM_CURSORS; i++)
		{
			_selected_items.at(i) = item;
			_cursors.at(i).set_position(item->_position);
		}
	}

	void level_select_menu::increase_active_cursor()
	{
		if(_active_cursor < NUM_CURSORS-1)
			set_active_cursor(_active_cursor+1);
	}

	void level_select_menu::decrease_active_cursor()
	{
		if(_active_cursor > 0)
			set_active_cursor(_active_cursor-1);
	}

	int level_select_menu::get_active_cursor_id()
	{
		return _active_cursor;
	}

	bn::vector<menu_item*,16> *level_select_menu::item_list()
	{
		return &_item_list;
	}

	void level_select_menu::build_level_select_menu()
	{
		// TOP_LEFT
		// (-80, -30)
		bn::fixed_point pos = lev_sel_pos::top_left;
		{
			_menu_items_.push_back(menu_item(this, 0, pos, menu_action_id::NONE));
			_item_list.push_back(&_menu_items_.back());
		}
		// LEV_SEL_ITEM_TOP_RIGHT
		// (-16, -30)
		pos = lev_sel_pos::top_right;
		{
			_menu_items_.push_back(menu_item(this, 1, pos, menu_action_id::NONE));
			_item_list.push_back(&_menu_items_.back());
		}

		// MID_LEFT
		// (-80, 10)
		pos = lev_sel_pos::mid_left;
		{
			_menu_items_.push_back(menu_item(this, 2, pos, menu_action_id::NONE));
			_item_list.push_back(&_menu_items_.back());
		}
		// MID_RIGHT
		// (-16, 10)
		pos = lev_sel_pos::mid_right;
		{
			_menu_items_.push_back(menu_item(this, 3, pos, menu_action_id::NONE));
			_item_list.push_back(&_menu_items_.back());
		}

		// BOT_LEFT
		// (-80, 50)
		pos = lev_sel_pos::bot_left;
		{
			_menu_items_.push_back(menu_item(this, 4, pos, menu_action_id::NONE));
			_item_list.push_back(&_menu_items_.back());
		}
		// BOT_RIGHT
		// (-16, 50)
		pos = lev_sel_pos::bot_right;
		{
			_menu_items_.push_back(menu_item(this, 5, pos, menu_action_id::NONE));
			_item_list.push_back(&_menu_items_.back());
		}

		_item_list.at(LEV_SEL_ITEM_TOP_LEFT)->link_left(_item_list.at(LEV_SEL_ITEM_TOP_RIGHT));
		_item_list.at(LEV_SEL_ITEM_TOP_LEFT)->link_right(_item_list.at(LEV_SEL_ITEM_TOP_RIGHT));
		_item_list.at(LEV_SEL_ITEM_MID_LEFT)->link_left(_item_list.at(LEV_SEL_ITEM_MID_RIGHT));
		_item_list.at(LEV_SEL_ITEM_MID_LEFT)->link_right(_item_list.at(LEV_SEL_ITEM_MID_RIGHT));
		_item_list.at(LEV_SEL_ITEM_BOT_LEFT)->link_left(_item_list.at(LEV_SEL_ITEM_BOT_RIGHT));
		_item_list.at(LEV_SEL_ITEM_BOT_LEFT)->link_right(_item_list.at(LEV_SEL_ITEM_BOT_RIGHT));

		_item_list.at(LEV_SEL_ITEM_TOP_LEFT)->link_below(_item_list.at(LEV_SEL_ITEM_MID_LEFT));
		_item_list.at(LEV_SEL_ITEM_TOP_RIGHT)->link_below(_item_list.at(LEV_SEL_ITEM_MID_RIGHT));
		_item_list.at(LEV_SEL_ITEM_MID_LEFT)->link_below(_item_list.at(LEV_SEL_ITEM_BOT_LEFT));
		_item_list.at(LEV_SEL_ITEM_MID_RIGHT)->link_below(_item_list.at(LEV_SEL_ITEM_BOT_RIGHT));
		_item_list.at(LEV_SEL_ITEM_BOT_LEFT)->link_below(_item_list.at(LEV_SEL_ITEM_TOP_LEFT));
		_item_list.at(LEV_SEL_ITEM_BOT_RIGHT)->link_below(_item_list.at(LEV_SEL_ITEM_TOP_RIGHT));

		move_all_cursors_to_item(_item_list.front());
		set_active_cursor(0);
	}



	


}