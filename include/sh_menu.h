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
		bn::vector<menu_item*,16> _item_list;

		menu_item *_current_item;

		const bn::fixed_point _cursor_offset = bn::fixed_point(-8,0);
		bn::sprite_ptr _cursor_sprite;
		bn::vector<bn::sprite_ptr, 12> _text_sprites;


		bool _menu_open;
		//distance to space menu elements vertically
		const int _item_offset_y = 16;
	protected:
		void clear_menu_item_pool();
		void generate_menu_items();

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