#include "sh_direction.h"
#include "sh_random.h"

namespace sh
{
	namespace directions
	{
		
		direction next_CW(direction dir)
		{
			switch (dir)
			{
			case direction::NORTH:
				return direction::EAST;
				break;
			case direction::EAST:
				return direction::SOUTH;
				break;
			case direction::SOUTH:
				return direction::WEST;
				break;
			case direction::WEST:
				return direction::NORTH;
				break;
			default:
				return dir;
				break;
			}
		}

		direction next_CCW(direction dir)
		{
			switch (dir)
			{
			case direction::NORTH:
				return direction::WEST;
				break;
			case direction::EAST:
				return direction::NORTH;
				break;
			case direction::SOUTH:
				return direction::EAST;
				break;
			case direction::WEST:
				return direction::SOUTH;
				break;
			default:
				return dir;
				break;
			}
		}

		direction opposite(direction dir)
		{
			switch (dir)
			{
			case direction::NORTH:
				return direction::SOUTH;
				break;
			case direction::EAST:
				return direction::WEST;
				break;
			case direction::SOUTH:
				return direction::NORTH;
				break;
			case direction::WEST:
				return direction::EAST;
				break;
			default:
				return dir;
				break;
			}
		}

		bn::point dir_to_vector(direction dir)
		{
			switch (dir)
			{
			case direction::NORTH:
				return bn::point(0,-1);
				break;
			case direction::EAST:
				return bn::point(1,0);
				break;
			case direction::SOUTH:
				return bn::point(0,1);
				break;
			case direction::WEST:
				return bn::point(-1,0);
				break;
			default:
				return bn::point(0,0);
				break;
			}
		}

		int dir_to_int(direction dir)
		{
			return (int)dir;
		}

		direction int_to_dir(int dir)
		{
			dir = bn::clamp(dir, 0, 4);
			return (direction)dir;
		}

		direction random_direction()
		{
			int r = random.get_int(0,4);
			return int_to_dir(r);
		}
	}
}