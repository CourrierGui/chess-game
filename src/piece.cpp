#include <chess/piece.hpp>
#include <chess/board.hpp>

#include <pangolin/pangolin/resource-manager.hpp>
#include <pangolin/pgl-tools/logger.hpp>

#include <iostream>
#include <algorithm>

const double ratio = (1.0);

Piece::Piece() = default;

std::vector<Position> Piece::next_moves(const Board& board) const noexcept {
	return {};
}

Position& Position::operator+=(const Move& pos) {
	x += pos.dx;
	y += pos.dy;
	return *this;
}

bool Piece::valid_move(
	const Position& pos,
	const Board& board
	) const noexcept
{
	const Piece* other = board.get_piece(pos);

	if (pos.x < 0 || pos.y < 0 || pos.x > 7 || pos.y > 7)
		return false;
	if (other != nullptr && same_team(other))
		return false;

	return true;
}

Position operator+(const Position& p, const Move& m) {
	return {
		p.x + m.dx,
		p.y + m.dy
	};
}

Piece::Piece(
	const std::string& type,
	const std::string& texture_file,
	int x, int y,
	Team team)
	: type(type), pos({.x = x, .y = y}), team(team), alive(true)
{
	std::string texture_name = "t";
	texture = pgl::loader::ResourceManager::load_texture(
		texture_file, true, texture_name
	);
}

void Piece::set_pos(const Position& pos) {
	this->pos = pos;
}

Position Piece::get_pos() const {
	return pos;
}

void Piece::draw(
			pgl::render2D::SpriteRenderer& renderer,
			Board& board)
{
	if (!alive)
		return;
	glm::vec2 piece_size = board.get_piece_size();
	renderer.draw(
		texture, board.get_pos(pos),
		{piece_size.x*ratio, piece_size.y*ratio}
	);
}

bool Piece::is_white() const {
	return this->team == Team::White;
}

bool Piece::same_team(const Piece* piece) const {
	return this->team == piece->team;
}

void Piece::kill() {
	alive = false;
}

std::string Piece::get_type() const {
	return type;
}

Team Piece::get_team() const {
	return team;
}

Knight::Knight(
	const std::string& texture_file,
	int x, int y,
	Team team)
: Piece("knight", texture_file, x, y, team) {

}

std::vector<Position> Knight::next_moves(const Board& board) const noexcept {
	std::vector<Position> res;
	for (auto m: moves) {
		Position p = pos + m;
		if (valid_move(p, board))
			res.push_back(p);
	}
	return res;
}

King::King(
	const std::string& texture_file,
	int x, int y,
	Team team)
: Piece("king", texture_file, x, y, team) {

}

std::vector<Position> King::next_moves(const Board& board) const noexcept {
	std::vector<Position> res;
	for (auto m: moves) {
		Position p = pos + m;
		if (valid_move(p, board))
			res.push_back(p);
	}
	return res;
}

void add_moves(
	const Piece& piece,
	const Board& board,
	std::back_insert_iterator<std::vector<Position>>& it,
	const Position& stop,
	const Move& move)
{
	Position p = piece.get_pos() + move;
	while (p != stop+move) {
		if (p.x < 0 || p.y < 0 || p.x > 7 || p.y > 7)
			break;
		const Piece* other = board.get_piece(p);
		if (other == nullptr) {
			*it = p;
			p += move;
			continue;
		}
		if (other->same_team(&piece))
			break;
		else {
			*it = p;
			p += move;
			break;
		}
	}
}

void lines(
	const Piece& piece,
	const Position& pos,
	const Board& board,
	std::back_insert_iterator<std::vector<Position>>& it)
{
	Position top    = { pos.x, 7 };
	Position bottom = { pos.x, 0 };
	Position right  = { 0, pos.y };
	Position left   = { 7, pos.y };

	add_moves(piece, board, it, top,    {0,  1});
	add_moves(piece, board, it, bottom, {0, -1});

	add_moves(piece, board, it, right,  {-1,  0});
	add_moves(piece, board, it, left,   { 1,  0});
}

void diagonales(
	const Piece& piece,
	const Position& pos,
	const Board& board,
	std::back_insert_iterator<std::vector<Position>>& it)
{
	Position top_left = {
		std::max(0, pos.x - (7 - pos.y)),
		std::min(7, pos.y + pos.x)
	};
	Position bottom_right = {
		std::min(7, pos.x + pos.y),
		std::max(0, pos.y - (7 - pos.x))
	};
	add_moves(piece, board, it, bottom_right, { 1, -1});
	add_moves(piece, board, it, top_left,     {-1,  1});

	Position top_right = {
		std::min(7, pos.x + (7 - pos.y)),
		std::min(7, pos.y + (7 - pos.x))
	};
	Position bottom_left = {
		std::max(0, pos.x - pos.y),
		std::max(0, pos.y - pos.x)
	};
	add_moves(piece, board, it, bottom_left, {-1, -1});
	add_moves(piece, board, it, top_right,   { 1,  1});
}

Bishop::Bishop(
	const std::string& texture_file,
	int x, int y,
	Team team)
: Piece("bishop", texture_file, x, y, team) {

}

std::vector<Position> Bishop::next_moves(const Board& board) const noexcept {
	std::vector<Position> res;
	auto it = std::back_inserter(res);
	diagonales(*this, pos, board, it);
	return res;
}

Rook::Rook(
	const std::string& texture_file,
	int x, int y,
	Team team)
: Piece("rook", texture_file, x, y, team) {

}

std::vector<Position> Rook::next_moves(const Board& board) const noexcept {
	std::vector<Position> res;
	auto it = std::back_inserter(res);
	lines(*this, pos, board, it);
	return res;
}

Queen::Queen(
	const std::string& texture_file,
	int x, int y,
	Team team)
: Piece("queen", texture_file, x, y, team) {

}

std::vector<Position> Queen::next_moves(const Board& board) const noexcept {
	std::vector<Position> res;
	auto it = std::back_inserter(res);
	lines(*this, pos, board, it);
	diagonales(*this, pos, board, it);
	return res;
}

Pawn::Pawn(
	const std::string& texture_file,
	int x, int y,
	Team team)
: Piece("pawn", texture_file, x, y, team) {
	if (team == Team::White)
		forward = -1;
	else
		forward = 1;
}

std::vector<Position> Pawn::next_moves(const Board& board) const noexcept {
	std::vector<Position> res;
	Position p = pos + Move{0, forward};
	if (valid_move(p, board)) {
		const Piece* other = board.get_piece(p);
		if (other == nullptr)
			res.push_back(p);
	}

	// first double step
	if (  (pos.y == 1 && team == Team::Black)
			||(pos.y == 6 && team == Team::White)) {
		p += Move{0, forward};
		if (valid_move(p, board)) {
			const Piece* other = board.get_piece(p);
			if (other == nullptr)
				res.push_back(p);
		}
	}

	// look for ennemies
	p = pos + Move{1, forward};
	if (valid_move(p, board)) {
		const Piece* other = board.get_piece(p);
		if (other != nullptr && !same_team(other))
			res.push_back(p);
	}
	p = pos + Move{-1, forward};
	if (valid_move(p, board)) {
		const Piece* other = board.get_piece(p);
		if (other != nullptr && !same_team(other))
			res.push_back(p);
	}

	return res;
}
