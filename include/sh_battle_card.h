#ifndef SH_CARD_H
#define SH_CARD_H

#include "sh_tile_pattern.h"

namespace sh
{


	class battle_card
	{
	private:
		virtual ~battle_card() = default;
		tile_pattern tile_pattern;

	public:
		battle_card() = default;
		
	};

}

#endif //SH_CARD_H