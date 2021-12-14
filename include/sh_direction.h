#ifndef SH_DIRECTION_H
#define SH_DIRECTION_H

#include <bn_point.h>

namespace sh
{
	enum class direction{
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3
	};

	namespace directions
	{
		direction next_CW(direction dir);
		direction next_CCW(direction dir);
		direction opposite(direction dir);

		bn::point dir_to_vector(direction dir);
		int dir_to_int(direction dir);
		direction int_to_dir(int dir);

		direction random_direction();
	}

}

#endif //SH_DIRECTION_H