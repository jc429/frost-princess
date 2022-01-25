#ifndef SH_LEVEL_SELECT
#define SH_LEVEL_SELECT

#include "sh_scene.h"
#include "sh_cursor.h"
#include "sh_level_select_menu.h"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	#define CORNER_X 32
	#define CORNER_Y 40
	#define MIDDLE_X 64
	#define MIDDLE_Y 0
	class level_select_scene : public scene
	{
	private:
		bn::regular_bg_ptr title_bg;
		bn::vector<bn::fixed_point, 7> panel_positions;
		bn::vector<bn::sprite_ptr, 7> portrait_frames;
		bn::vector<bn::sprite_ptr, 7> portrait_sprites;

		// cursor level_select_cursor;
		int selected_panel;
	public:
		level_select_scene();
		~level_select_scene();
		void update() override;

		// void select_panel(int panel_id);
	};

}

#endif //SH_LEVEL_SELECT