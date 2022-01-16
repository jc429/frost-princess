#ifndef SH_LEVEL_SELECT_MENU_H
#define SH_LEVEL_SELECT_MENU_H

#include "sh_cursor.h"
#include "sh_menu_item.h"
#include "sh_menu.h"

namespace sh
{

	class level_select_menu : public menu
	{
	protected:
		cursor _cursor;
		void build_level_select_menu();

	public:
		level_select_menu();
		~level_select_menu();
		void update() override;

		void highlight_menu_item(menu_item *item) override;
	};


}


#endif //SH_LEVEL_SELECT_MENU_H