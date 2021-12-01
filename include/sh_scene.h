#ifndef SH_SCENE_H
#define SH_SCENE_H

#include "bn_optional_fwd.h"

namespace sh
{
	enum class scene_type;

	class scene
	{
	public:
		virtual ~scene() = default;
		[[nodiscard]]
		virtual bn::optional<scene_type> update() = 0;

	protected:
		scene() = default;
	};
}


#endif //SH_SCENE_H