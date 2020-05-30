#pragma once

#include <string>
#include <vector>
#include <array>

#include <pangolin/pangolin/sprite-renderer.hpp>
#include <pangolin/pangolin/texture.hpp>

class Board;

struct Move {
	int dx, dy;
};

struct Position {
	int x, y;
	Position& operator+=(const Move& pos);
	bool operator==(const Position& pos) const noexcept = default;
	bool operator!=(const Position& pos) const noexcept = default;
};

enum class Team {
	Black, White, None
};

Position operator+(const Position& p, const Move& m);

class Piece {
	protected:
		std::string type;
		pgl::loader::Texture2D texture;
		Position pos;
		Team team;
		bool alive;

	public:
		Piece();
		void set_pos(const Position& pos);
		Position get_pos() const;
		Team get_team() const;
		bool is_white() const;
		bool same_team(const Piece* piece) const;
		void kill();
		std::string get_type() const;

		bool valid_move(
			const Position& pos,
			const Board& board
		) const noexcept;

		virtual std::vector<Position> next_moves(const Board& board) const noexcept;

		Piece(
			const std::string& type,
			const std::string& texture_file,
			int x, int y,
			Team team
		);

		void draw(
			pgl::render2D::SpriteRenderer& renderer,
			Board& board
		);
};

class Knight : public Piece {
	constexpr static std::array<Move, 8> moves = {{
		{ 1,  2}, { 2,  1},
		{ 2, -1}, { 1, -2},
		{-2, -1}, {-1, -2},
		{-2,  1}, {-1,  2},
	}};

	public:
		Knight(
			const std::string& texture_file,
			int x, int y,
			Team team
		);

		std::vector<Position> next_moves(const Board& board) const noexcept override;
};

class King : public Piece {
	private:
		bool checked = false;
		constexpr static std::array<Move, 8> moves = {{
			{ 1,  0}, { 1,  1},
			{ 0,  1}, {-1,  1},
			{-1,  0}, {-1, -1},
			{ 0, -1}, { 1, -1},
		}};

	public:
		King(
			const std::string& texture_file,
			int x, int y,
			Team team
		);
		std::vector<Position> next_moves(const Board& board) const noexcept override;
};

class Bishop : public Piece {
	public:
		Bishop(
			const std::string& texture_file,
			int x, int y,
			Team team
		);
		std::vector<Position> next_moves(const Board& board) const noexcept override;
};

class Rook : public Piece {
	public:
		Rook(
			const std::string& texture_file,
			int x, int y,
			Team team
		);
		std::vector<Position> next_moves(const Board& board) const noexcept override;
};

class Queen : public Piece {
	public:
		Queen(
			const std::string& texture_file,
			int x, int y,
			Team team
		);
		std::vector<Position> next_moves(const Board& board) const noexcept override;
};

class Pawn : public Piece {
	private:
		int forward;
	public:
		Pawn(
			const std::string& texture_file,
			int x, int y,
			Team team
		);
		std::vector<Position> next_moves(const Board& board) const noexcept override;
};
