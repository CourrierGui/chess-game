#include <chess/game.hpp>

#include <pangolin/pangolin/resource-manager.hpp>
#include <pangolin/pangolin/shader.hpp>

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

void Game::init() {
	pgl::gui::GLFWWindow::set_user_pointer((void*)this);

	pgl::gui::GLFWWindow::set_key_cb(
		[](GLFWwindow* window, int key, int scancode, int action, int mode) {
			auto* game_ptr = static_cast<Game*>(glfwGetWindowUserPointer(window));

			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				game_ptr->unselect_piece();
			} else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}
		}
	);

	pgl::gui::GLFWWindow::set_cursor_cb(
		[](GLFWwindow* window, double xpos, double ypos) {
		}
	);

	pgl::gui::GLFWWindow::set_mouse_button_cb(
		[](GLFWwindow* window, int button, int action, int mode) {
			auto* game_ptr = static_cast<Game*>(glfwGetWindowUserPointer(window));
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				if (game_ptr->in_board(xpos, ypos)) {
					Position pos = game_ptr->get_board_pos(xpos, ypos);
					game_ptr->process_square(pos);
				}
			}
		}
	);

	selected = Piece("selected", "../resources/textures/selected.png", 0, 0, Team::None);
	possible = Piece("selected", "../resources/textures/possible.png", 0, 0, Team::None);

	pieces[0] = Piece("rook",   "../resources/textures/golden/rook.png",   0, 0, Team::Black);
	pieces[1] = Piece("knight", "../resources/textures/golden/knight.png", 1, 0, Team::Black);
	pieces[2] = Piece("bishop", "../resources/textures/golden/bishop.png", 2, 0, Team::Black);
	pieces[3] = Piece("queen",  "../resources/textures/golden/queen.png",  3, 0, Team::Black);
	pieces[4] = Piece("king",   "../resources/textures/golden/king.png",   4, 0, Team::Black);
	pieces[5] = Piece("bishop", "../resources/textures/golden/bishop.png", 5, 0, Team::Black);
	pieces[6] = Piece("knight", "../resources/textures/golden/knight.png", 6, 0, Team::Black);
	pieces[7] = Piece("rook",   "../resources/textures/golden/rook.png",   7, 0, Team::Black);

	pieces[8]  = Piece("pawn", "../resources/textures/golden/pawn.png", 0, 1, Team::Black);
	pieces[9]  = Piece("pawn", "../resources/textures/golden/pawn.png", 1, 1, Team::Black);
	pieces[10] = Piece("pawn", "../resources/textures/golden/pawn.png", 2, 1, Team::Black);
	pieces[11] = Piece("pawn", "../resources/textures/golden/pawn.png", 3, 1, Team::Black);
	pieces[12] = Piece("pawn", "../resources/textures/golden/pawn.png", 4, 1, Team::Black);
	pieces[13] = Piece("pawn", "../resources/textures/golden/pawn.png", 5, 1, Team::Black);
	pieces[14] = Piece("pawn", "../resources/textures/golden/pawn.png", 6, 1, Team::Black);
	pieces[15] = Piece("pawn", "../resources/textures/golden/pawn.png", 7, 1, Team::Black);

	pieces[16] = Piece("rook",   "../resources/textures/green/rook.png",   0, 7, Team::White);
	pieces[17] = Piece("knight", "../resources/textures/green/knight.png", 1, 7, Team::White);
	pieces[18] = Piece("bishop", "../resources/textures/green/bishop.png", 2, 7, Team::White);
	pieces[19] = Piece("queen",  "../resources/textures/green/queen.png",  3, 7, Team::White);
	pieces[20] = Piece("king",   "../resources/textures/green/king.png",   4, 7, Team::White);
	pieces[21] = Piece("bishop", "../resources/textures/green/bishop.png", 5, 7, Team::White);
	pieces[22] = Piece("knight", "../resources/textures/green/knight.png", 6, 7, Team::White);
	pieces[23] = Piece("rook",   "../resources/textures/green/rook.png",   7, 7, Team::White);

	pieces[24] = Piece("pawn", "../resources/textures/green/pawn.png", 0, 6, Team::White);
	pieces[25] = Piece("pawn", "../resources/textures/green/pawn.png", 1, 6, Team::White);
	pieces[26] = Piece("pawn", "../resources/textures/green/pawn.png", 2, 6, Team::White);
	pieces[27] = Piece("pawn", "../resources/textures/green/pawn.png", 3, 6, Team::White);
	pieces[28] = Piece("pawn", "../resources/textures/green/pawn.png", 4, 6, Team::White);
	pieces[29] = Piece("pawn", "../resources/textures/green/pawn.png", 5, 6, Team::White);
	pieces[30] = Piece("pawn", "../resources/textures/green/pawn.png", 6, 6, Team::White);
	pieces[31] = Piece("pawn", "../resources/textures/green/pawn.png", 7, 6, Team::White);

	for (auto& p: pieces) {
		board.set_piece(&p, p.get_pos());
	}

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width),
																		static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	pgl::loader::ResourceManager::get_shader("sprite").use().setInteger("image", 0);
	pgl::loader::ResourceManager::get_shader("sprite").setMatrix4("projection", projection);
}

void Game::process_input(float dt) {

}

void Game::update(float dt) {

}

void Game::render() {
	board.draw(renderer);

	if (selected_piece != nullptr) {
		selected.set_pos(selected_piece->get_pos());
		selected.draw(renderer, board);
		for (auto& p: selected_piece->next_moves(board)) {
			possible.set_pos(p);
			possible.draw(renderer, board);
		}
	}

	for (auto& piece: pieces) {
		piece.draw(renderer, board);
	}
}

bool Game::in_board(int x, int y) {
	return board.in_board(x, y);
}

Position Game::get_board_pos(int x, int y) {
	return board.get_pos(x, y);
}

void Game::process_square(const Position& pos) {
	if (Piece* p = board.get_piece(pos); p != nullptr && p == selected_piece) {
		selected_piece = nullptr;
		return;
	}

	if (selected_piece != nullptr) {

		std::vector<Position> poses = selected_piece->next_moves(board);
		auto unary = [&pos](const Position& p) {
			return (p.w == pos.w && p.h == pos.h);
		};
		bool valid_pos = (std::find_if(poses.begin(), poses.end(), unary) != poses.end());

		if (Piece* p=board.get_piece(pos); (p == nullptr || !p->same_team(selected_piece)) && valid_pos) {
			if (p != nullptr)
				p->kill();
			board.move_piece(selected_piece, pos);
			selected_piece->set_pos(pos);
			selected_piece = nullptr;
		} else if (p != nullptr && p->same_team(selected_piece)) {
			selected_piece = p;
		} else {
			std::clog << "Their is a piece here.\n";
		}
	} else {
		selected_piece = board.get_piece(pos);
	}
}

void Game::unselect_piece() {
	selected_piece = nullptr;
}
