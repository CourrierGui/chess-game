#pragma once

#include <pangolin/pangolin/sprite-renderer.hpp>

#include <chess/piece.hpp>
#include <array>

class Board {
	private:
		int size, square_size;
		glm::vec2 offset;
		std::array<Piece*, 64> pieces;

	public:
		Board(int size);
		void draw(pgl::render2D::SpriteRenderer& renderer);
		glm::vec2 get_pos(const Position& pos);
		glm::vec2 get_piece_size();
		bool in_board(int x, int y);
		Position get_pos(int x, int y);
		Piece* get_piece(const Position& pos);
		const Piece* get_piece(const Position& pos) const;
		void set_piece(Piece* p, const Position& pos);
		void move_piece(Piece* p, const Position& pos);
};
