#include <chess/game.hpp>

#include <pangolin/pangolin/resource-manager.hpp>
#include <pangolin/pangolin/shader.hpp>
#include <pangolin/pgl-tools/logger.hpp>

#include <algorithm>
#include <iostream>

Game::Game(int width, int height)
	: width(width), height(height),
	board(std::min(width, height)),
	pieces(), selected_piece(nullptr),
	renderer(pgl::loader::ResourceManager::load_shader(
		"../resources/shaders/sprite.vs",
		"../resources/shaders/sprite.fs",
		"", "sprite")
	)
{
}

auto Game::init() -> void{
	pgl::gui::GLFWWindow::set_user_pointer((void*)this);

	pgl::gui::GLFWWindow::set_key_cb(
		[](GLFWwindow* window, int key, int, int action, int) {
			auto* game_ptr = static_cast<Game*>(glfwGetWindowUserPointer(window));

			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				game_ptr->unselect_piece();
			} else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}
		}
	);

	pgl::gui::GLFWWindow::set_cursor_cb(
		[](GLFWwindow*, double, double) { }
	);

	pgl::gui::GLFWWindow::set_mouse_button_cb(
		[](GLFWwindow* window, int button, int action, int) {
			auto* game_ptr = static_cast<Game*>(glfwGetWindowUserPointer(window));
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				game_ptr->process_mouse_click(xpos, ypos);
			}
		}
	);

	selected = Piece("selected", "../resources/textures/selected.png", 0, 0, Team::None);
	possible = Piece("possible", "../resources/textures/possible.png", 0, 0, Team::None);

	pieces[0] = new Rook  ("../resources/textures/golden/rook.png",   0, 0, Team::Black);
	pieces[1] = new Knight("../resources/textures/golden/knight.png", 1, 0, Team::Black);
	pieces[2] = new Bishop("../resources/textures/golden/bishop.png", 2, 0, Team::Black);
	pieces[3] = new Queen ("../resources/textures/golden/queen.png",  3, 0, Team::Black);
	pieces[4] = new King  ("../resources/textures/golden/king.png",   4, 0, Team::Black);
	pieces[5] = new Bishop("../resources/textures/golden/bishop.png", 5, 0, Team::Black);
	pieces[6] = new Knight("../resources/textures/golden/knight.png", 6, 0, Team::Black);
	pieces[7] = new Rook  ("../resources/textures/golden/rook.png",   7, 0, Team::Black);

	for (int i=0; i<8; ++i) {
		pieces[8+i]  = new Pawn("../resources/textures/golden/pawn.png", i, 1, Team::Black);
	}

	pieces[16] = new Rook  ("../resources/textures/green/rook.png",   0, 7, Team::White);
	pieces[17] = new Knight("../resources/textures/green/knight.png", 1, 7, Team::White);
	pieces[18] = new Bishop("../resources/textures/green/bishop.png", 2, 7, Team::White);
	pieces[19] = new Queen ("../resources/textures/green/queen.png",  3, 7, Team::White);
	pieces[20] = new King  ("../resources/textures/green/king.png",   4, 7, Team::White);
	pieces[21] = new Bishop("../resources/textures/green/bishop.png", 5, 7, Team::White);
	pieces[22] = new Knight("../resources/textures/green/knight.png", 6, 7, Team::White);
	pieces[23] = new Rook  ("../resources/textures/green/rook.png",   7, 7, Team::White);

	for (int i=0; i<8; ++i)
		pieces[24+i] = new Pawn("../resources/textures/green/pawn.png", i, 6, Team::White);

	for (auto& p: pieces) {
		board.set_piece(p, p->get_pos());
	}

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width),
																		static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	pgl::loader::ResourceManager::get_shader("sprite").use().setInteger("image", 0);
	pgl::loader::ResourceManager::get_shader("sprite").setMatrix4("projection", projection);
}

auto Game::process_input(float) -> void {

}

auto Game::update(float) -> void {

}

auto Game::render() -> void {
	board.draw(renderer);

	if (selected_piece != nullptr) {
		// move selected object
		selected.set_pos(selected_piece->get_pos());
		// draw selected object
		selected.draw(renderer, board);

		for (auto& p: selected_piece->next_moves(board)) {
			// move possible object
			possible.set_pos(p);
			// draw possible object
			possible.draw(renderer, board);
		}
	}

	for (auto& piece: pieces) {
		piece->draw(renderer, board);
	}
}

auto Game::on_move() -> void {
	bool is_checked = false;
	for (Piece* p: pieces) {
		for (auto pos: p->next_moves(board)) {
			const Piece* piece = board.get_piece(pos);
			if (piece != nullptr
					&& piece->get_type() == "king")
			{
				is_checked = true;
				check = piece->get_team();
				PGL_INFO_ONCE("Checked !");
				break;
			}
		}
	}

	if (check.has_value() && !is_checked) {
		check.reset();
		PGL_INFO_ONCE("Uncheck.");
	}

}

auto Game::process_tile(const Position& pos) -> void {
	Piece* p = board.get_piece(pos);

	// if we click on the same piece, assumes that we want to unselect it
	if (p == selected_piece) {
		selected_piece = nullptr;
		return;
	}

	// if their is a piece selected
	if (selected_piece != nullptr) {
		// get the possible moves
		std::vector<Position> poses = selected_piece->next_moves(board);

		// checks if the tile clicked is in the list of possible moves
		auto unary = [&pos](const Position& p) {
			return p == pos;
		};
		bool valid_pos = (std::find_if(poses.begin(), poses.end(), unary) != poses.end());

		if (valid_pos) {
			// if the piece in the tile is on the opposit team
			if (p != nullptr && !p->same_team(selected_piece))
				p->kill(); // kill the piece

			// move the selected piece
			board.move_piece(selected_piece, pos);
			selected_piece = nullptr;
			on_move();

		// if another piece is clicked on the same team
		} else if (p != nullptr && p->same_team(selected_piece)) {
			// select that piece
			selected_piece = p;
		} else {
			PGL_INFO("Invalid position !");
		}
	} else {
		Piece* piece = board.get_piece(pos);
		selected_piece = piece;
		if (piece != nullptr && piece->get_team() == turn) {
			selected_piece = nullptr;
			PGL_INFO("It's not your turn !");
		}
		if (selected_piece != nullptr) {
			turn = selected_piece->get_team();
		}
	}
}

auto Game::unselect_piece() -> void {
	selected_piece = nullptr;
}

auto Game::process_mouse_click(int xpos, int ypos) -> void{
	Position pos = board.get_pos(xpos, ypos);
	process_tile(pos);
}
