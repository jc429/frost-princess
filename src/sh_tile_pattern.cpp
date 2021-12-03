#include "sh_tile_pattern.h"

namespace sh{


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



}