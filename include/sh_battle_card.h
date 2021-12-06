#ifndef SH_BATTLE_CARD_H
#define SH_BATTLE_CARD_H

#include "sh_tile_pattern.h"

namespace sh
{


	class battle_card
	{
	private:
		virtual ~battle_card() = default;
		sh::tile_pattern tile_pattern;

	public:
		battle_card() = default;
		
	};

}

#endif //SH_BATTLE_CARD_H