#pragma once

#include <string>
#include <vector>

#include <pangolin/pangolin/sprite-renderer.hpp>
#include <pangolin/pangolin/texture.hpp>

class Board;

struct Position {
	int w,h;
};

enum class Team {
	Black, White, None
};

class Piece {
	private:
		std::string type;
		pgl::loader::Texture2D texture;
		Position pos;
		Team team;
		bool alive;

	public:
		Piece();
		void set_pos(const Position& pos);
		Position get_pos() const;
		std::vector<Position> next_moves(const Board& board);
		bool same_team(Team team) const;
		bool same_team(const Piece* piece) const;
		void kill();
		std::string get_type() const;

		Piece(
			const std::string& type,
			const std::string& texture_file,
			int w, int h,
			Team team
		);

		void draw(
			pgl::render2D::SpriteRenderer& renderer,
			Board& board
		);
};
