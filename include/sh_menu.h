#ifndef SH_MENU_H
#define SH_MENU_H

#include "sh_direction.h"
#include "sh_scene.h"

#include <bn_fixed_point.h>
#include <bn_optional.h>
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
		OPTIONS_MENU,
	};	
	
	enum class menu_action_id
	{
		NONE,		// pressing A on this menu item does nothing
		CLOSE_MENU,
		EXIT_SCENE,
		RESET_SETTINGS,
	};

	enum class menu_item_interact_type
	{
		NONE,
		A_PRESS,
		B_PRESS,
		UP_PRESS,
		DOWN_PRESS,
		LEFT_PRESS,
		RIGHT_PRESS
	};

	struct menu_item
	{
	public:
		class menu *_menu;
		menu_action_id _action_id;
		bn::fixed_point _position;
		bn::string<16> _text;
		// void (*_menu_action)();
		menu_item *_above;
		menu_item *_below;

		menu_item(class menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<16> text);
		// void set_menu_action(void (*func)());

		void link_above(menu_item *other);
		void link_below(menu_item *other);
		void select();
		void interact_with_item(menu_item_interact_type interaction);
		// bool input_confirm_action();
		// bool input_direction_action(direction dir);
	};
	
	struct menu_slider : public menu_item
	{
		bn::sprite_ptr _slider_cursor;
		const int x_inc = 5;
		int _min_value;
		int _max_value;
		int _cur_value;

		menu_slider(class menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<16> text, int min_val, int max_val);
		~menu_slider();
		
		void set_range(int min, int max);
		int set_value(int val);
		int get_value();
		int increment();
		int decrement();

		void update_sprite();
	};

	class menu
	{
	private:
		scene *_current_scene;
		menu_type _type;
		int _bg_layer;
		bn::fixed_point _position;
		bn::vector<bn::regular_bg_ptr, 1> _menu_bg;
		// bn::optional<bn::regular_bg_ptr> _menu_bg;
		bn::vector<menu_item, 8> _menu_items;
		menu_item *_current_item;

		const bn::fixed_point _cursor_offset = bn::fixed_point(-8,0);
		bn::sprite_ptr _cursor_sprite;
		bn::vector<bn::sprite_ptr, 12> _text_sprites;


		bool _menu_open;
		//distance to space menu elements vertically
		const int _item_offset_y = 16;

	public:
		menu(menu_type type, int layer, bn::sprite_text_generator &text_generator);
		~menu();
		void update();
		
		void highlight_menu_item(menu_item *item);
		// void select_item(int item_index);
		void perform_action(menu_action_id action_id);
		void close_menu();
		bool is_open();

		int get_layer();
	};
}

#endif //SH_MENU_H