#ifndef SH_AUDIO_H
#define SH_AUDIO_H

#include <bn_audio.h>
#include <bn_music.h>
#include <bn_music_items.h>
#include <bn_sound_items.h>
#include <bn_fixed.h>

namespace sh
{
	enum class track_id
	{
		TITLE,
		BATTLE_BGM,
		BATTLE_WIN,
		BATTLE_LOSE,
	};

	enum class sound_id
	{
		BLIP_HIGH,
		BLIP_LOW,
		BLIP_SOFT,
		UNSELECTABLE,
		WEW,
		POP_1,
		POP_2,
		POP_3,
		POP_HIGH
	};

	namespace audio
	{
		void set_music_volume(bn::fixed volume);
		void set_sfx_volume(bn::fixed volume);
		void play_track(track_id track);
		void stop_current_track();
		void play_sound(sound_id sound);

	}
}


#endif //SH_AUDIO_H