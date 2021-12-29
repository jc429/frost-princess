#include "sh_cursor.h"

#include <bn_fixed.h>
#include <bn_fixed_point.h>
#include <bn_point.h>
#include <bn_sprite_builder.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "bn_sprite_items_cursor_corner.h"

namespace sh
{

	cursor::cursor()
	{
		{
			bn::fixed_point pos(0,0);
			bn::sprite_builder builder(bn::sprite_items::cursor_corner);
			// top left
			builder.set_position(pos);
			builder.set_bg_priority(1);
			builder.set_z_order(-100);
			_sprites.push_back(builder.build());
			_animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites.back(), 16, bn::sprite_items::cursor_corner.tiles_item(), 0, 1));
			// top right
			builder.set_horizontal_flip(true);
			_sprites.push_back(builder.build());
			_animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites.back(), 16, bn::sprite_items::cursor_corner.tiles_item(), 0, 1));
			// bottom left
			builder.set_horizontal_flip(false);
			builder.set_vertical_flip(true);
			_sprites.push_back(builder.build());
			_animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites.back(), 16, bn::sprite_items::cursor_corner.tiles_item(), 0, 1));
			// bottom right
			builder.set_horizontal_flip(true);
			_sprites.push_back(builder.release_build());
			_animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites.back(), 16, bn::sprite_items::cursor_corner.tiles_item(), 0, 1));
		}
		_position = bn::point(0,0);
		_halfsize = bn::point(8,8);
		update_sprite();
	}

	cursor::~cursor()
	{
		_sprites.clear();
		_animate_actions.clear();
	}

	void cursor::update_sprite()
	{
		bn::fixed_point pos;
		pos.set_x(_position.x() - _halfsize.x());
		pos.set_y(_position.y() - _halfsize.y());
		_sprites.at(0).set_position(pos);
		pos.set_x(_position.x() + _halfsize.x());
		_sprites.at(1).set_position(pos);
		pos.set_x(_position.x() - _halfsize.x());
		pos.set_y(_position.y() + _halfsize.y());
		_sprites.at(2).set_position(pos);
		pos.set_x(_position.x() + _halfsize.x());
		_sprites.at(3).set_position(pos);
	}

	void cursor::update()
	{
		for(auto it = _animate_actions.begin(), end = _animate_actions.end(); it != end; ++it)
		{
			it->update();
		}
	}

	void cursor::set_position(bn::fixed_point pos)
	{
		_position = pos;
		update_sprite();
	}

	void cursor::set_size(bn::point size)
	{
		_halfsize.set_x(size.x() * 0.5);
		_halfsize.set_y(size.y() * 0.5);
		update_sprite();
	}

	void cursor::set_visible(bool visible)
	{
		for(auto it = _sprites.begin(), end = _sprites.end(); it != end; ++it)
		{
			it->set_visible(visible);
		}
	}
}