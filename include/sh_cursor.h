#ifndef SH_CURSOR_H
#define SH_CURSOR_H

#include <bn_fixed_point.h>
#include <bn_point.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sh
{

	class cursor
	{
	private:
		bn::fixed_point _position;
		bn::point _halfsize;
		bn::vector<bn::sprite_ptr, 4> _sprites;
		bn::vector<bn::sprite_animate_action<2>, 4> _animate_actions;
		void update_sprite();

	public:
		cursor();
		~cursor();
		void update();
		void set_position(bn::fixed_point pos);
		void set_size(bn::point size);
		void set_visible(bool visible);
	};
}


#endif //SH_CURSOR_H