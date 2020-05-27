#pragma once

#include <array>

#include <pangolin/pgl-gui/glfw-window.hpp>
#include <pangolin/pangolin/texture.hpp>
#include <pangolin/pangolin/sprite-renderer.hpp>

#include <chess/board.hpp>
#include <chess/piece.hpp>

class Game {
	private:
		int width, height;
		Board board;
		std::array<Piece, 32> pieces;
		Piece* selected_piece;
		Piece selected, possible;
		pgl::render2D::SpriteRenderer renderer;

	public:
		Game(int width, int height);
		void init();
		void process_input(float dt);
		void update(float dt);
		void render();
		bool in_board(int x, int y);
		Position get_board_pos(int x, int y);
		void process_square(const Position& pos);
		void unselect_piece();
};
