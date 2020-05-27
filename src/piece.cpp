#include <chess/piece.hpp>
#include <chess/board.hpp>

#include <pangolin/pangolin/resource-manager.hpp>

#include <iostream>

const double ratio = (1.0); //+0.2/3.7);

Piece::Piece() {

}

Piece::Piece(
	const std::string& type,
	const std::string& texture_file,
	int w, int h,
	Team team)
	: type(type), pos({.w = w, .h = h}), team(team), alive(true)
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

std::vector<Position> Piece::next_moves(const Board& board) {
	std::vector<Position> res;
	if (type == "pawn") {
		if (team == Team::White) {
			Position pp{.w=pos.w-1,.h=pos.h-1};
			const Piece* pi=board.get_piece(pp);
			if (pi != nullptr && !pi->same_team(Team::White))
				res.push_back(pp);
			pp = Position{.w=pos.w+1,.h=pos.h-1};
			const Piece* pii = board.get_piece(pp);
			if (pii != nullptr && !pii->same_team(Team::White))
				res.push_back(pp);

			if (Position p{.w=pos.w, .h=pos.h-1};
					board.valid_move(p, team) &&
					(board.get_piece(p) == nullptr
					 || board.get_piece(p)->same_team(Team::White)))
				res.push_back(p);
			else
				return res;
			if (pos.h == 6)
				if (Position p{.w=pos.w, .h=pos.h-2};
						board.valid_move(p, team) &&
						(board.get_piece(p) == nullptr
						 || board.get_piece(p)->same_team(Team::White)))
					res.push_back(p);
		} else if (team == Team::Black) {
			Position pp{.w=pos.w-1,.h=pos.h+1};
			const Piece* pi=board.get_piece(pp);
			if (pi != nullptr && !pi->same_team(Team::Black))
				res.push_back(pp);
			pp = Position{.w=pos.w+1,.h=pos.h+1};
			const Piece* pii = board.get_piece(pp);
			if (pii != nullptr && !pii->same_team(Team::Black))
				res.push_back(pp);

			if (Position p{.w=pos.w, .h=pos.h+1};
					board.valid_move(p, team) &&
					(board.get_piece(p) == nullptr
					 || board.get_piece(p)->same_team(Team::Black)))
				res.push_back(p);
			else
				return res;
			if (pos.h == 1)
				if (Position p{.w=pos.w, .h=pos.h+2};
						board.valid_move(p, team) &&
					(board.get_piece(p) == nullptr
					 || board.get_piece(p)->same_team(Team::Black)))
					res.push_back(p);
		}

	} else if (type == "knight") {
		if (Position p{.w=pos.w+1, .h=pos.h+2}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w+2, .h=pos.h+1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w+1, .h=pos.h-2}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w+2, .h=pos.h-1}; board.valid_move(p, team))
			res.push_back(p);

		if (Position p{.w=pos.w-1, .h=pos.h+2}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w-2, .h=pos.h+1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w-1, .h=pos.h-2}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w-2, .h=pos.h-1}; board.valid_move(p, team))
			res.push_back(p);

	} else if (type == "bishop") {
		int x=pos.w, y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			Position p{.w=++x, .h=++y};
			const Piece* o = board.get_piece(p);
			if (board.valid_move(p, team)) {
				res.push_back(p);
				if (o != nullptr && !o->same_team(this)) {
					break;
				}
			} else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=--x, .h=--y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=++x, .h=--y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=--x, .h=++y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}

	} else if (type == "rook") {
		int x=pos.w, y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=x, .h=++y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=x, .h=--y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=++x, .h=y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=--x, .h=y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}

	} else if (type == "king") {
		if (Position p{.w=pos.w,   .h=pos.h+1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w+1, .h=pos.h+1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w+1, .h=pos.h-1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w+1, .h=pos.h}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w,   .h=pos.h-1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w-1, .h=pos.h+1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w-1, .h=pos.h-1}; board.valid_move(p, team))
			res.push_back(p);
		if (Position p{.w=pos.w-1, .h=pos.h}; board.valid_move(p, team))
			res.push_back(p);

	} else if (type == "queen") {
		int x=pos.w, y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=x, .h=++y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=x, .h=--y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=++x, .h=y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=--x, .h=y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w, y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=++x, .h=++y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=--x, .h=--y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=++x, .h=--y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}
		x=pos.w; y=pos.h;
		while (0 <= x && x < 8 && 0 <= y && y < 8) {
			if (Position p{.w=--x, .h=++y}; board.valid_move(p, team))
				res.push_back(p);
			else
				break;
		}

	}
	return res;
}

bool Piece::same_team(Team team) const {
	return this->team == team;
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
