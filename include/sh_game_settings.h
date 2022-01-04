#ifndef SH_GAME_SETTINGS_H
#define SH_GAME_SETTINGS_H

namespace sh
{
	namespace game_settings
	{
		void set_music_volume(int vol);
		int get_music_volume();

		void set_sfx_volume(int vol);
		int get_sfx_volume();

		void set_colorblind_mode_enabled(int enabled);
		bool colorblind_mode_enabled();

		void reset_settings_to_default(int _);
	}
}

#endif //SH_GAME_SETTINGS_H