#pragma once

#include <optional>
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
		std::array<Piece*, 32> pieces;
		Piece* selected_piece;
		Piece selected, possible;
		pgl::render2D::SpriteRenderer renderer;
		std::optional<Team> check;
		Team turn = Team::White;

	public:
		Game(int width, int height);
		auto init() -> void;
		auto process_input(float dt) -> void;
		auto update(float dt) -> void;
		auto render() -> void;
		auto process_tile(const Position& pos) -> void;
		auto unselect_piece() -> void;
		auto process_mouse_click(int xpos, int ypos) -> void;
		auto on_move() -> void;
};
