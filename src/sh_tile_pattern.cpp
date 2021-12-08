#include "sh_tile_pattern.h"

#include "bn_random.h"

namespace sh
{
	namespace tile_patterns
	{
		tile_pattern next_tile_pattern(tile_pattern src)
		{
			switch (src)
			{
			case tile_pattern::EMPTY:
				return tile_pattern::EMPTY;
				break;

			case tile_pattern::SINGLE:
				return tile_pattern::LINE_2;
				break;
			case tile_pattern::LINE_2:
				return tile_pattern::LINE_3;
				break;
			case tile_pattern::LINE_3:
				return tile_pattern::LINE_4;
				break;
			case tile_pattern::LINE_4:
				return tile_pattern::L_SMALL;
				break;
			case tile_pattern::L_SMALL:
				return tile_pattern::L_NORMAL;
				break;
			case tile_pattern::L_NORMAL:
				return tile_pattern::L_REVERSE;
				break;
			case tile_pattern::L_REVERSE:
				return tile_pattern::T_4;
				break;
			case tile_pattern::T_4:
				return tile_pattern::S_4;
				break;
			case tile_pattern::S_4:
				return tile_pattern::Z_4;
				break;
			case tile_pattern::Z_4:
				return tile_pattern::SQUARE;
				break;
			case tile_pattern::SQUARE:
				return tile_pattern::SINGLE;
				break;
			
			default:
				return src;
				break;
			}
		}

		
		tile_pattern random_tile_pattern()
		{
			static bn::random random;
			int r = random.get_int(0, 11) + 1;
			switch(r)
			{
			case 1:
				return tile_pattern::SINGLE;
				break;
			case 2:
				return tile_pattern::LINE_2;
				break;
			case 3:
				return tile_pattern::LINE_3;
				break;
			case 4:
				return tile_pattern::LINE_4;
				break;
			case 5:
				return tile_pattern::L_SMALL;
				break;
			case 6:
				return tile_pattern::L_NORMAL;
				break;
			case 7:
				return tile_pattern::L_REVERSE;
				break;
			case 8:
				return tile_pattern::T_4;
				break;
			case 9:
				return tile_pattern::S_4;
				break;
			case 10:
				return tile_pattern::Z_4;
				break;
			case 11:
				return tile_pattern::SQUARE;
				break;
			default:
				return tile_pattern::SINGLE;
				break;
			}
		}

		
		int get_tile_index(tile_pattern p)
		{
			switch(p)
			{
			case tile_pattern::SINGLE:
				return 1;
				break;
			case tile_pattern::LINE_2:
				return 2;
				break;
			case tile_pattern::LINE_3:
				return 3;
				break;
			case tile_pattern::LINE_4:
				return 4;
				break;
			case tile_pattern::L_SMALL:
				return 5;
				break;
			case tile_pattern::L_NORMAL:
				return 6;
				break;
			case tile_pattern::L_REVERSE:
				return 7;
				break;
			case tile_pattern::T_4:
				return 8;
				break;
			case tile_pattern::S_4:
				return 9;
				break;
			case tile_pattern::Z_4:
				return 10;
				break;
			case tile_pattern::SQUARE:
				return 11;
				break;
			case tile_pattern::EMPTY:
			default:
				return 0;
				break;
			}
		}
	}

}