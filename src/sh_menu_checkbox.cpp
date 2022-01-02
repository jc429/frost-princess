#include "sh_menu.h"
#include <bn_sprite_tiles_ptr.h>
#include "bn_sprite_items_menu_checkbox.h"

namespace sh
{
	
	menu_checkbox::menu_checkbox(menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<16> text, bn::fixed_point spr_offset, bool default_checked) :
		menu_item(menu_owner, pos, action_id,text)
	{
		_checkbox_sprites.push_back(bn::sprite_items::menu_checkbox.create_sprite(pos + spr_offset, 1));
		_checkbox_sprites.back().set_position(pos + spr_offset);
		_checkbox_sprites.back().set_bg_priority(menu_owner->get_layer());
		_checkbox_sprites.back().set_z_order(-10);

		set_checked(default_checked);
	}

	menu_checkbox::~menu_checkbox()
	{
		_checkbox_sprites.clear();
	}

	void menu_checkbox::interact_with_item(menu_item_interact_type interaction)
	{
		switch(interaction)
		{
		case menu_item_interact_type::A_PRESS:
			toggle();
			break;
		default:
			return menu_item::interact_with_item(interaction);
			break;
		}
	}

	void menu_checkbox::set_checked(bool checked)
	{
		_is_checked = checked;
		update_sprite();
	}


	void menu_checkbox::toggle()
	{
		set_checked(!_is_checked);
	}

	void menu_checkbox::update_sprite()
	{
		int tile = _is_checked ? 1 : 0;
		_checkbox_sprites.back().set_tiles(bn::sprite_items::menu_checkbox.tiles_item().create_tiles(tile));
	}

}