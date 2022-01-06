#include "sh_game_settings.h"
#include "sh_audio.h"
#include <bn_fixed.h>
#include <bn_math.h>

namespace sh
{
	namespace game_settings
	{
		#define MUSIC_VOLUME_DEFAULT	7
		static int _music_volume_ = MUSIC_VOLUME_DEFAULT;
		#define SFX_VOLUME_DEFAULT		7
		static int _sfx_volume_ = SFX_VOLUME_DEFAULT;
		#define COLORBLIND_MODE_DEFAULT false
		static bool _colorblind_mode_enabled_ = COLORBLIND_MODE_DEFAULT;

		void set_music_volume(int vol)
		{
			_music_volume_ = bn::clamp(vol, 0, 10);
			bn::fixed v = (bn::fixed)vol / (bn::fixed)10;
			audio::set_music_volume(v);
		}

		int get_music_volume()
		{
			return _music_volume_;
		}

		void set_sfx_volume(int vol)
		{
			_sfx_volume_ = bn::clamp(vol, 0, 10);
			bn::fixed v = (bn::fixed)vol / (bn::fixed)10;
			audio::set_sfx_volume(v);
			audio::play_sound(sound_id::BLIP_HIGH);
		}

		int get_sfx_volume()
		{
			return _sfx_volume_;
		}

		
		void set_colorblind_mode_enabled(int enabled)
		{
			_colorblind_mode_enabled_ = (enabled != 0);
		}

		bool colorblind_mode_enabled()
		{
			return _colorblind_mode_enabled_;
		}


		void reset_settings_to_default(int _)
		{
			set_music_volume(MUSIC_VOLUME_DEFAULT);
			set_sfx_volume(SFX_VOLUME_DEFAULT);
			set_colorblind_mode_enabled(COLORBLIND_MODE_DEFAULT);
		}


	}
}