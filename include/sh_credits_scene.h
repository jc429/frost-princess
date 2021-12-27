#ifndef SH_CREDITS_SCENE_H
#define SH_CREDITS_SCENE_H

#include "sh_scene.h"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>

namespace sh
{

	class credits_scene : public scene
	{
	private:
		bn::vector<bn::regular_bg_ptr, 2> backgrounds;

		bn::sprite_text_generator text_generator;
		bn::vector<bn::sprite_ptr, 32> text_sprites;


	public:
		credits_scene();
		~credits_scene() = default;
		void update() override;

	};

}

#endif //SH_CREDITS_SCENE_H