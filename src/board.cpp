#include <chess/board.hpp>
#include <iostream>

#include <pangolin/pangolin/resource-manager.hpp>

constexpr double board_size = 13.4;
constexpr double x_offset_ratio = double(2.01)/board_size;
constexpr double y_offset_ratio = double(1.82)/board_size;
constexpr double square_size_ratio = double(1.18)/board_size;

Board::Board(int size)
	: size(size), square_size(square_size_ratio*size),
		offset({size*x_offset_ratio, size*y_offset_ratio})
{
	for (auto& p: pieces) p = nullptr;
	pgl::loader::ResourceManager::load_texture(
		"../resources/textures/board.png", true, "board"
	);
}

void Board::draw(pgl::render2D::SpriteRenderer& renderer) {
	renderer.draw(
		pgl::loader::ResourceManager::get_texture("board"),
		glm::vec2(0.0f, 0.0f), glm::vec2(size, size), 0.0f
	);
}

glm::vec2 Board::get_pos(const Position& pos) {
	return glm::vec2(
		offset.x + pos.w*square_size,
		offset.y + pos.h*square_size
	);
}

glm::vec2 Board::get_piece_size() {
	return glm::vec2(square_size, square_size);
}

bool Board::in_board(int x, int y) {
	if (x < offset.x) return false;
	if (y < offset.y) return false;
	if (x > size - offset.x) return false;
	if (y > size - offset.y) return false;
	return true;
}

Position Board::get_pos(int x, int y) {
	return {
		(x - offset.x) / square_size,
		(y - offset.y) / square_size
	};
}

const Piece* Board::get_piece(const Position& pos) const {
	if (pos.w < 0 || pos.w > 7 || pos.h < 0 || pos.h > 7)
		return nullptr;
	return pieces[pos.w + 8*pos.h];
}

Piece* Board::get_piece(const Position& pos) {
	return pieces[pos.w + 8*pos.h];
}

void Board::set_piece(Piece* p, const Position& pos) {
	pieces[pos.w + 8*pos.h] = p;
}

void Board::move_piece(Piece* p, const Position& pos) {
	Position previous = p->get_pos();
	pieces[pos.w + 8*pos.h] = p;
	pieces[previous.w + 8*previous.h] = nullptr;
}

bool Board::valid_move(const Position& pos, Team team) const {
	if (pos.w < 0 || pos.h < 0 || pos.w > 7 || pos.h > 7)
		return false;
	if (get_piece(pos) == nullptr)
		return true;
	if (get_piece(pos)->same_team(team))
		return false;

	return true;
}
