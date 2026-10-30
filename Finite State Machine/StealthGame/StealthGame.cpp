// StealthGame.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameController.h"

/*
HandleEvents will capture SFML keyboard and mouse input and route to Game
Controller to handle
*/
void HandleEvents(sf::RenderWindow& window, GameController* gameController) {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        window.close();
      else {
        gameController->HandleKeyPress(keyPressed);
      }
    } else if (const auto* mouseButtonPressed =
                   event->getIf<sf::Event::MouseButtonPressed>()) {
      gameController->HandleMousePress(mouseButtonPressed);
    }
  }
}

int main() {
  // define window
  sf::RenderWindow window(sf::VideoMode({1000, 600}), "Stealth Game");
  window.setVerticalSyncEnabled(true);

  // define the views
  sf::View gameView(sf::FloatRect({0, 0}, {1200, 960.f}));
  sf::View uiView(sf::FloatRect({1200, 0}, {1600, 960.f}));

  gameView.setViewport(sf::FloatRect({0.f, 0.f}, {0.75f, 1.f}));
  uiView.setViewport(sf::FloatRect({0.75f, 0.f}, {0.75f, 1.f}));

  gameView.setCenter({600, 480});

  sf::Clock clock;

  // define the game controller, init and start the game
  GameController gameController;
  gameController.Init();
  gameController.StartGame();

  // display and game loop
  while (window.isOpen()) {
    // handle all events
    HandleEvents(window, &gameController);

    // display the game section of the window
    window.clear();
    window.setView(gameView);
    gameController.Render(window);

    // distaplay the UI section of the windro
    window.setView(uiView);
    gameController.DisplayUI(window);

    // call the function to display
    window.display();

    // tick the game controller
    sf::Time elapsed = clock.restart();
    float dt = elapsed.asSeconds();
    gameController.Update(dt);
  }
}
