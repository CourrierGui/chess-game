#include <chess/board.hpp>
#include <iostream>

#include <pangolin/pangolin/resource-manager.hpp>

constexpr double board_size = 13.4;
constexpr double x_offset_ratio = double(2.01)/board_size;
constexpr double y_offset_ratio = double(1.82)/board_size;
constexpr double square_size_ratio = 1.0/8; //double(1.18)/board_size;

Board::Board(int size)
	: size(size), square_size(square_size_ratio*size),
		offset({size*x_offset_ratio, size*y_offset_ratio})
{
	for (auto& p: pieces) p = nullptr;
	pgl::loader::ResourceManager::load_texture(
		"../resources/textures/board2.png", true, "board"
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
		pos.x * square_size,
		size - square_size - pos.y * square_size
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
		(int) x / square_size,
		7 - (int) y / square_size
	};
}

const Piece* Board::get_piece(const Position& pos) const {
	if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7)
		return nullptr;
	return pieces[pos.x + 8*pos.y];
}

Piece* Board::get_piece(const Position& pos) {
	return pieces[pos.x + 8*pos.y];
}

void Board::set_piece(Piece* p, const Position& pos) {
	pieces[pos.x + 8*pos.y] = p;
}

void Board::move_piece(Piece* p, const Position& pos) {
	Position previous = p->get_pos();
	pieces[pos.x + 8*pos.y] = p;
	pieces[previous.x + 8*previous.y] = nullptr;
	p->set_pos(pos);
}
