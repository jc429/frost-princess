#ifndef SH_MENU_H
#define SH_MENU_H

#include "sh_direction.h"

#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_vector.h>

namespace sh
{

	enum class menu_type
	{
		PAUSE_MENU,
	};	
	
	enum class menu_action_id
	{
		CLOSE_MENU,
	};

	struct menu_item
	{
	public:
		menu_action_id _action_id;
		bn::fixed_point _position;
		bn::string<16> _text;

		menu_item(bn::fixed_point pos, menu_action_id action_id, bn::string<16> text);

		bool input_confirm_action();
		bool input_direction_action(direction dir);
	};

	class menu
	{
	private:
		menu_type _type;
		bn::vector<menu_item, 8> _menu_items;
		const int _item_offset_y = 16;
		bn::vector<bn::regular_bg_ptr, 1> _menu_bgs;
		bn::vector<bn::sprite_ptr, 12> _text_sprites;
		bn::sprite_ptr _cursor_sprite;
		const bn::fixed_point _cursor_offset = bn::fixed_point(-8,0);

		int _selection_id; 
		bool _menu_open;

	public:
		menu(menu_type type, int bg_layer, bn::sprite_text_generator &text_generator);
		~menu();
		void update();
		
		void move_cursor(direction dir);
		void select_item(int item_index);
		void close_menu();
	};
}

#endif //SH_MENU_H