//////////////////////////////////////////////////////////////////////////////////
// 
// gameUI.cpp
// Jay-Mark Pascua
// 02/18/2022
// 
// Holds display-to-screen functions
//
//////////////////////////////////////////////////////////////////////////////////

#include "game.h"

void Game::displayPokemon(std::string id) {

	std::string filepath;
	SDL_Rect pokemon;

	// Display both pokemon at the same time if they are the same
	if (_playerPokemon.id == id && _opponentPokemon.id == id) {

		filepath = "assets/pokemon/sprites/" + id + ".png";
		pokemon = _opponentPokemonRect;
		int gap = _interface.images["MessageBox"].rect.y - (pokemon.y + pokemon.h);	// Ensures bottom of player sprite lines up with top of text bar (no floating sprites)
		pokemon.y += gap;

		_interface.render(NULL, &pokemon, filepath);

		filepath = "assets/pokemon/sprites/back/" + id + ".png";
		pokemon = _playerPokemonRect;
	}
	// Else if it's the player's pokemon, display the back of sprite
	else if (_playerPokemon.id == id) {

		filepath = "assets/pokemon/sprites/back/" + id + ".png";
		pokemon = _playerPokemonRect;
	}
	// Else display the opponent's pokemon
	else if (_opponentPokemon.id == id) {

		filepath = "assets/pokemon/sprites/" + id + ".png";
		pokemon = _opponentPokemonRect;
	}


	_interface.render(NULL, &pokemon, filepath);
}

void Game::displayAttackInfo(Attack& atk) {

	_interface.displayText(atk.elementType, &elementTypeTxt, white);
	_interface.displayText(atk.damageType, &damageTypeTxt, white);
	_interface.displayText(std::to_string(atk.tempPP) + "/" + std::to_string(atk.pp), &ppTxt, white);
}

void Game::displayUI() {

	// Render UI menus
	switch (_menuState)
	{
	case menuState::START_MENU:
		_interface.displayImage("Background");
		_interface.displayImage("MessageBox");

		// If player's Pokemon hasn't been defined yet
		if (_playerPokemon.name == "") {
			// Display player's choices
			_interface.render(NULL, &selection1, "assets/pokemon/sprites/" + p1->id + ".png");
			_interface.render(NULL, &selection2, "assets/pokemon/sprites/" + p2->id + ".png");
			_interface.render(NULL, &selection3, "assets/pokemon/sprites/" + p3->id + ".png");
		}
		// Else display what the player chose to the center of screen
		else {
			_interface.render(NULL, &selection2, "assets/pokemon/sprites/" + _playerPokemon.id + ".png");
			_interface.displayText("You selected " + _playerPokemon.name + "!", &atkTopLeftTxt, white);
		}
		break;
	case menuState::MAIN:
		// Render battlefield background
		_interface.displayImage("Battlefield");

		// Render Pokemon if they haven't fainted
		displayPokemon(_playerPokemon.id);

		if (_playerPokemon.id != _opponentPokemon.id) // Don't display opponent's pokemon if they have the same id, displayPokemon function will
			displayPokemon(_opponentPokemon.id);	  // print them both at the same time if they're the same

		// Render Pokemon HP Boxes
		displayPokemonHPBoxes();


		// Render Menu Text/Buttons
		if (_playerTurn) {
			_interface.displayImage("Menu");
			_interface.displayText("What will you do?", &atkTopLeftTxt, white);

			_interface.displayButton("Fight");
			_interface.displayButton("Pokemon");
			_interface.displayButton("Bag");
			_interface.displayButton("Run");
		}
		else {
			_interface.displayImage("MessageBox");
			_interface.displayText("A wild " + _opponentPokemon.name + " appeared!", &atkTopLeftTxt, white);
		}
		break;

	case menuState::FIGHT:
		// Render battlefield background
		_interface.displayImage("Battlefield");

		// Render Pokemon if they haven't fainted
		displayPokemon(_playerPokemon.id);

		if (_playerPokemon.id != _opponentPokemon.id) // Don't display opponent's pokemon if they have the same id, displayPokemon function will
			displayPokemon(_opponentPokemon.id);	  // print them both at the same time if they're the same

		// Render Pokemon HP Boxes
		displayPokemonHPBoxes();

		_interface.displayImage("FightMenu");

		// Render Attack Buttons
		_interface.displayButton("Attack 1");	// These button names are found in init.cpp
		_interface.displayButton("Attack 2");						
		_interface.displayButton("Attack 3");						
		_interface.displayButton("Attack 4");						

		_interface.displayText(_playerPokemon.attacks[0].name, &atkTopLeftTxt, white);
		_interface.displayText(_playerPokemon.attacks[1].name, &atkTopRightTxt, white);
		_interface.displayText(_playerPokemon.attacks[2].name, &atkBottomLeftTxt, white);
		_interface.displayText(_playerPokemon.attacks[3].name, &atkBottomRightTxt, white);

		break;
	case menuState::POKEMON:
		break;
	case menuState::STATS:
		break;
	case menuState::BAG:
		break;
	case menuState::RUN:
		break;
	default:
		break;
	}
}

void  Game::displayPokemonHPBoxes() {

	double hpBarWidth = 364;	// Rect width of the hp bars
	double scale;				// Used to resize hp bars accordingly

	// Adjust hp bars to reflect damage taken
	scale = (double)_playerPokemon.tempHp / (double)_playerPokemon.maxHp;
	hpBarWidth *= scale;
	playerHp.w = (int)hpBarWidth;

	hpBarWidth = 364;
	scale = (double)_opponentPokemon.tempHp / (double)_opponentPokemon.maxHp;
	hpBarWidth *= scale;
	opponentHp.w = (int)hpBarWidth;

	_interface.displayImage("PlayerPokemonHPBox");	// Display the box template	
	_interface.displayText(_playerPokemon.name, &playerPokemonNameTxt, white);	// Display pokemon name
	_interface.displayText("Lvl. 100", &playerPokemonLevelTxt, white);	// Display pokemon level
	_interface.displayText(std::to_string(_playerPokemon.tempHp) + "/" + std::to_string(_playerPokemon.maxHp), &playerPokemonHpTxt, black);	// Display pokemon HP
	_interface.displayRect(&playerHp, green);	// Display player pokemon's HP bar

	_interface.displayImage("OpponentPokemonHPBox");
	_interface.displayText(_opponentPokemon.name, &opponentPokemonNameTxt, white);
	_interface.displayText("Lvl. 100", &opponentPokemonLevelTxt, white);
	_interface.displayText(std::to_string(_opponentPokemon.tempHp) + "/" + std::to_string(_opponentPokemon.maxHp), &opponentPokemonHPTxt, black);
	_interface.displayRect(&opponentHp, green);
}

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// ANIMATION /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void Game::animate(std::string attackMsg) {

	_interface.clear();
	displayUI();
	_interface.displayImage("MessageBox");
	_interface.update();
}