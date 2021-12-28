#include "sh_game_settings.h"

namespace sh
{
	namespace game_settings
	{
		#define MUSIC_VOLUME_DEFAULT	7
		#define SFX_VOLUME_DEFAULT		7
		static int music_volume = MUSIC_VOLUME_DEFAULT;
		static int sfx_volume = SFX_VOLUME_DEFAULT;

		void set_music_volume(int vol)
		{
			music_volume = vol;
		}

		int get_music_volume()
		{
			return music_volume;
		}

		void set_sfx_volume(int vol)
		{
			sfx_volume = vol;
		}

		int get_sfx_volume()
		{
			return sfx_volume;
		}

	}
}