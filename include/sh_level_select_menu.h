#ifndef SH_LEVEL_SELECT_MENU_H
#define SH_LEVEL_SELECT_MENU_H

#include "sh_cursor.h"
#include "sh_menu_item.h"
#include "sh_menu.h"

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	#define LEV_SEL_ITEM_TOP_LEFT	0
	#define LEV_SEL_ITEM_TOP_RIGHT	1
	#define LEV_SEL_ITEM_MID_LEFT	2
	#define LEV_SEL_ITEM_MID_RIGHT	3
	#define LEV_SEL_ITEM_BOT_LEFT	4
	#define LEV_SEL_ITEM_BOT_RIGHT	5


	class level_select_menu : public menu
	{
		#define NUM_CURSORS 2
	protected:
		// cursor _lev_sel_cursor;
		bn::vector<bn::sprite_ptr, NUM_CURSORS> _cursors;
		bn::vector<menu_item *, NUM_CURSORS> _selected_items;
		int _active_cursor;


		void build_level_select_menu();
		void set_cursor_visible(bool visible) override;
		void set_active_cursor(int cursor_id);

	public:
		level_select_menu();
		~level_select_menu();
		void update() override;

		void highlight_menu_item(menu_item *item) override;
		menu_item *current_item() override;
		void move_all_cursors_to_item(menu_item *item);

		void increase_active_cursor();
		void decrease_active_cursor();
		int get_active_cursor_id();
		
		bn::vector<menu_item*,16> *item_list();
	};


	namespace lev_sel_pos
	{
		const bn::fixed_point top_left(-80,-30);
		const bn::fixed_point top_right(-16,-30);
		const bn::fixed_point mid_left(-80,10);
		const bn::fixed_point mid_right(-16,10);
		const bn::fixed_point bot_left(-80,50);
		const bn::fixed_point bot_right(-16,50);
	}
}


#endif //SH_LEVEL_SELECT_MENU_H