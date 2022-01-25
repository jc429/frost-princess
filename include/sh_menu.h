#ifndef SH_MENU_H
#define SH_MENU_H

#include "sh_direction.h"
#include "sh_menu_item.h"
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
		TITLE_MENU,
		LEVEL_SELECT_MENU
	};


	class menu
	{
	protected:
		scene *_current_scene;
		menu_type _type;
		int _bg_layer;
		bn::fixed_point _position;
		bn::vector<bn::regular_bg_ptr, 1> _menu_bg;
		// bn::optional<bn::regular_bg_ptr> _menu_bg;
		bn::string<16> _header_text;
		bn::vector<menu_item*,16> _item_list;

		const bn::fixed_point _cursor_offset = bn::fixed_point(-8,1);
		bn::sprite_ptr _cursor_sprite;
		bn::vector<bn::sprite_ptr, 24> _text_sprites;

		bool _menu_open;
		bool _start_to_close;
		//distance to space menu elements vertically
		const int _item_offset_y = 16;

		menu_item *_current_item;

		void clear_menu_item_pool();
		void generate_menu_items();
		void generate_menu_text();

		virtual void set_cursor_visible(bool visible);

	public:
		menu(menu_type type, int layer);
		virtual ~menu();
		virtual void update();
		
		virtual void highlight_menu_item(menu_item *item);
		// void select_item(int item_index);
		void perform_action(menu_action_id action_id);
		virtual void open_menu();
		virtual void close_menu();
		bool is_open();

		int get_layer();
	};
}

#endif //SH_MENU_H