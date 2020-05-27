#include <pangolin/pgl-gui/glfw-window.hpp>
#include <pangolin/pangolin/resource-manager.hpp>

#include <chess/game.hpp>

#include <iostream>

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1400;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 1000;

int main(int argc, char** argv) {
	pgl::gui::GLFWWindow window = pgl::gui::GLFWWindow::make_window(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");

  // initialize game
  // ---------------
  pgl::loader::set_root("/home/guillaume/dev/projects/chess");
	Game chess(SCREEN_WIDTH, SCREEN_HEIGHT);
  chess.init();

  // deltaTime variables
  // -------------------
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  // start game within menu state
  // ----------------------------
  /* Breakout.state = GAME_MENU; */

  while (window.is_open()) {
    // calculate delta time
    // --------------------
    /* float currentFrame = glfwGetTime(); */
    /* deltaTime = currentFrame - lastFrame; */
    /* lastFrame = currentFrame; */
		window.poll_events();

    // manage user input
    // -----------------
    chess.process_input(deltaTime);

    // update game state
    // -----------------
    chess.update(deltaTime);

    // render
    // ------
		window.clear();
    chess.render();

		window.swap_buffer();
  }

  // delete all resources as loaded using the resource manager
  // ---------------------------------------------------------
  pgl::loader::ResourceManager::clear();
  return 0;
}
