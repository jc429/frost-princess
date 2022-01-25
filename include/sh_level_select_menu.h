#ifndef SH_LEVEL_SELECT_MENU_H
#define SH_LEVEL_SELECT_MENU_H

#include "sh_cursor.h"
#include "sh_menu_item.h"
#include "sh_menu.h"

namespace sh
{
	#define CORNER_X 32
	#define CORNER_Y 40
	#define MIDDLE_X 64
	#define MIDDLE_Y 0
	class level_select_menu : public menu
	{
	protected:
		cursor _lev_sel_cursor;
		void build_level_select_menu();
		void set_cursor_visible(bool visible) override;

	public:
		level_select_menu();
		~level_select_menu();
		void update() override;

		void highlight_menu_item(menu_item *item) override;
	};


}


#endif //SH_LEVEL_SELECT_MENU_H