#ifndef SH_TILE_PATTERN_H
#define SH_TILE_PATTERN_H

namespace sh{

	
	enum class tile_pattern
	{
		EMPTY,			// no tiles

		SINGLE,			// a single tile
		LINE_2,			// 2 tiles in a straight line
		LINE_3,			// 3 tiles in a straight line
		LINE_4,			// 4 tiles in a straight line

		L_SMALL,		// 3 tiles in an L shape
		L_NORMAL,		// 4 tiles in an L shape
		L_REVERSE,		// 4 tiles in a mirrored L shape

		T_4,			// 4 tiles in a T shape
		S_4,			// 4 tiles in a S shape
		Z_4,			// 4 tiles in a Z shape
		SQUARE,			// 4 tiles in a 2x2 square

		/* UNSUPPORTED */
		// S_5,			// 5 tiles in a S shape
		// Z_5,			// 5 tiles in a Z shape
		// T_5,			// 5 tiles in a T pattern
		// CROSS,		// 5 tiles in a + pattern
		// X			// 5 tiles in an X pattern
		/***************/

		SPECIAL_SINGLE

	};

	namespace tile_patterns{
		tile_pattern next_tile_pattern(tile_pattern src);
		tile_pattern random_tile_pattern();
		int get_tile_index(tile_pattern p);
	}
}

#endif //SH_TILE_PATTERN_H