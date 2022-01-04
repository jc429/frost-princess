#ifndef SH_MENU_ITEM_H
#define SH_MENU_ITEM_H

#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_vector.h>

namespace sh
{

	enum class menu_action_id
	{
		NONE,			// pressing A on this menu item does nothing
		CLOSE_MENU,		// closes the menu and resumes the scene
		EXIT_SCENE,		// closes the menu and ends the current scene
		GO_TO_TITLE,
		GO_TO_BATTLE,
		GO_TO_OPTIONS,
		GO_TO_CREDITS,
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
		#define MENU_STRING_MAX_LEN 32

	public:
		class menu *_menu;
		menu_action_id _action_id;
		bn::fixed_point _position;
		bn::string<MENU_STRING_MAX_LEN> _text;
		// void (*_menu_action)();
		menu_item *_above;
		menu_item *_below;
		void (*update_menu_item_event)(int);

		menu_item(class menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<MENU_STRING_MAX_LEN> text);
		virtual ~menu_item();
		// void set_menu_action(void (*func)());

		void link_above(menu_item *other);
		void link_below(menu_item *other);
		void select();
		virtual void show_item();
		virtual void hide_item();
		virtual void interact_with_item(menu_item_interact_type interaction);
	};
	
	struct menu_slider : public menu_item
	{
		bn::fixed_point _slider_offset;
		bn::vector<bn::sprite_ptr, 2> _slider_bar;
		bn::sprite_ptr _slider_cursor;
		const int x_inc = 5;
		int _min_value;
		int _max_value;
		int _cur_value;

		menu_slider(class menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<MENU_STRING_MAX_LEN> text, bn::fixed_point spr_offset, int min_val, int max_val);
		~menu_slider();
		
		virtual void show_item() override;
		virtual void hide_item() override;
		void interact_with_item(menu_item_interact_type interaction) override;

		void set_range(int min, int max);
		int set_value(int val);
		int get_value();
		int increment();
		int decrement();
		void update_sprite();
	};

	struct menu_checkbox : public menu_item
	{
	protected:
		bn::fixed_point _checkbox_offset;
		bn::vector<bn::sprite_ptr, 2> _checkbox_sprites;
		bool _is_checked;
	public:
		menu_checkbox(class menu *menu_owner, bn::fixed_point pos, menu_action_id action_id, bn::string<MENU_STRING_MAX_LEN> text, bn::fixed_point spr_offset, bool default_checked);
		~menu_checkbox();

		virtual void show_item() override;
		virtual void hide_item() override;
		void interact_with_item(menu_item_interact_type interaction) override;
		
		void set_checked(bool checked);
		bool is_checked();
		void toggle();
		void update_sprite();
	};


}

#endif //SH_MENU_ITEM_H