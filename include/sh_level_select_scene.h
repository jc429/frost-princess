#ifndef SH_LEVEL_SELECT
#define SH_LEVEL_SELECT

#include "sh_scene.h"
#include "sh_cursor.h"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	class level_select_scene : public scene
	{
	private:
		bn::regular_bg_ptr title_bg;
		bn::vector<bn::fixed_point, 9> panel_positions;
		bn::vector<bn::sprite_ptr, 9> portrait_frames;

		cursor level_select_cursor;
		int selected_panel;
	public:
		level_select_scene();
		~level_select_scene() = default;

		void update();
		void select_panel(int panel_id);
	};

}

#endif //SH_LEVEL_SELECT