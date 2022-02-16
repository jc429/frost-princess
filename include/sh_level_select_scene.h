#ifndef SH_LEVEL_SELECT
#define SH_LEVEL_SELECT

#include "sh_scene.h"
#include "sh_cursor.h"
#include "sh_level_select_menu.h"
#include "sh_character_sprite.h"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{
	#define NUM_CHARACTERS 6

	class level_select_scene : public scene
	{
	private:
		bn::regular_bg_ptr _lev_sel_bg;
		bn::regular_bg_ptr _lev_sel_fg;
		bn::vector<bn::fixed_point, NUM_CHARACTERS> panel_positions;
		bn::vector<bn::sprite_ptr, NUM_CHARACTERS> portrait_frames;
		bn::vector<bn::sprite_ptr, NUM_CHARACTERS> portrait_sprites;

		character_sprite _char_sprite;
		bn::vector<bn::sprite_ptr, 3> _nametag_sprites;

		int selected_panel;
		
		level_select_menu _lev_sel_menu;


	public:
		level_select_scene();
		~level_select_scene();
		void update() override;

		void set_char_sprite(character_id char_id);
		level_select_menu *get_level_select_menu();
		// void select_panel(int panel_id);
	};

}

#endif //SH_LEVEL_SELECT