#include "game.h"



Game::Game()            
		: 
		background_top(&spriteSheet, BACKGROUND_TOP, 0, 0),
		mv_bg_top1(&spriteSheet, MOVING_BG_TOP_1, 0, 0),
		mv_bg_top1_copy(mv_bg_top1),
		mv_bg_top2(&spriteSheet, MOVING_BG_TOP_2, 0, 0),
		mv_bg_top2_copy(mv_bg_top2),
		
		background_bot(&spriteSheet, BACKGROUND_BOT, 0, 0),
		mv_bg_bot1(&spriteSheet, MOVING_BG_BOT_1, 0, 0),
		mv_bg_bot1_copy(mv_bg_bot1),
		mv_bg_bot2(&spriteSheet, MOVING_BG_TOP_2, 0, 0),
		mv_bg_bot2_copy(mv_bg_bot2),

		player(&spriteSheet, PLAYER, 100, 10),
		
		bar(&spriteSheet, BOTTOM_BAR)
		{

	initOptions();

	// Create screens
	top     = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bottom 	= C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	

	Mix_PlayMusic(songs[2], 0);
	Mix_HookMusicFinished(playNextSong);


	mv_bg_top1_copy.setX(TOP_SCREEN_WIDTH);
	mv_bg_top2_copy.setX(TOP_SCREEN_WIDTH);

	mv_bg_bot1_copy.setX(BOTTOM_SCREEN_WIDTH);
	mv_bg_bot2_copy.setX(BOTTOM_SCREEN_WIDTH);

	
	bar.setY(BOTTOM_SCREEN_HEIGHT - bar.height());

	selector = std::make_shared<Object>(&spriteSheet, SELECTOR, 60, BOTTOM_HEIGHT_CENTER - 25);

	projectiles.reserve(10);
	enemy_projectiles.reserve(20);	
	enemies.reserve(20);

	move = 0;
	health_low = false;

	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);

	// The priority of these child threads must be higher (aka the value is lower) than that
	// of the main thread, otherwise there is thread starvation due to stdio being locked.
	threads[0] = threadCreate(threadShoot, 		(void*)(&sleepTime), STACKSIZE, prio-1, -2, false);
	threads[1] = threadCreate(threadFadeIn, 	(void*)(&fade), 	STACKSIZE, prio-1, -2, false);
	threads[3] = threadCreate(threadLowHealth, 	(void*)(&health_low), 	STACKSIZE, prio-1, -2, false);

	// look it up now so we don't have to do it later
	getHighscore();
}

void Game::initOptions() {
	menu_options[0] = {SETUP_GAME, 	BOTTOM_HEIGHT_CENTER - 25};
	menu_options[1] = {SETTINGS, 	BOTTOM_HEIGHT_CENTER +  5};
	
	menu_options[2].state.option 	=  	NEXT_SONG;
	menu_options[2].yPos		 	=	BOTTOM_HEIGHT_CENTER + 35;

						
	pause_options[0] = {GAME, 		BOTTOM_HEIGHT_CENTER - 20};			
	pause_options[1] = {SETTINGS, 	BOTTOM_HEIGHT_CENTER + 10};					
	pause_options[2] = {MENU, 		BOTTOM_HEIGHT_CENTER + 40};		
}

void Game::setupGame(Ship& player, std::vector<Enemy>& enemies,
				std::vector<std::shared_ptr<Projectile>>& projectiles,
				std::vector<std::shared_ptr<Projectile>>& enemy_projectiles) {
	player.setLife(10);
	player.setPos(BOTTOM_WIDTH_CENTER - player.width() / 2, BOTTOM_HEIGHT_CENTER - player.height() / 2);

	enemies.clear();
	projectiles.clear();
	enemy_projectiles.clear();
	
	game_round = 0;
	last_game_time = svcGetSystemTick() / SYSCLOCK_ARM11;
	score = 0;
	score_str = "";
}

void Game::getHighscore() {	
	std::filesystem::create_directory("/3ds");
	std::filesystem::create_directory(SCORE_FOLDER);

	highscore_file.open(SCORE_FILE, std::fstream::in);
	
	if (!highscore_file.is_open()) {
		FILE* file = fopen(SCORE_FILE, "w");
		fclose(file);
		highscore_file.open(SCORE_FILE);

		highscore_file << "0\n";
		highscore_file << "0\n";
	}
		
	highscore_file >> highscore;
	highscore_file >> best_time;
	
	highscore_file.close();

}

u64 Game::gameTime() {
	return  svcGetSystemTick() / SYSCLOCK_ARM11 - last_game_time;
}

void Game::drawBackgroundTop() {
	gameScene::draw(&background_top);

	gameScene::draw(&mv_bg_top1);
	gameScene::draw(&mv_bg_top1_copy);

	gameScene::draw(&mv_bg_top2);
	gameScene::draw(&mv_bg_top2_copy);
}

void Game::drawBackgroundBottom() {
	gameScene::draw(&background_bot);

	gameScene::draw(&mv_bg_bot1);
	gameScene::draw(&mv_bg_bot1_copy);

	gameScene::draw(&mv_bg_bot2);
	gameScene::draw(&mv_bg_bot2_copy);
}


void Game::startNewRound(std::vector<Enemy>& enemies) {
	for (int i = 1; i <= game_round; i++) {
		enemies.emplace_back(&spriteSheet, ENEMY_1, 40, 	i * 30);
		enemies.emplace_back(&spriteSheet, ENEMY_2, 80, 	i * 30);
		enemies.emplace_back(&spriteSheet, ENEMY_3, 120, 	i * 30);

		enemies.emplace_back(&spriteSheet, ENEMY_1, 160, 	i * 30);
		enemies.emplace_back(&spriteSheet, ENEMY_2, 200, 	i * 30);
		enemies.emplace_back(&spriteSheet, ENEMY_3, 240, 	i * 30);
	}
}

void Game::moveBackground(Object* bg, Object* bg_copy, float velocity, int screen_width) {
	bg->moveXBy(velocity);
	bg_copy->moveXBy(velocity);

	if (bg->px() < 0)
		bg->setX(screen_width);

	else if (bg_copy->px() < 0)
		bg_copy->setX(screen_width);
	
}

/**
 * @brief Checks for collision with a single bullet against the enemies
 * 
 * @param bullet one of your bullets
 * @param enemies the enemies
 * @return int the index holding the enemy hit, -1 if no enemy was hit.
 */
int Game::checkBulletColission(Projectile* bullet, std::vector<Enemy>& enemies) {
	for (int i = 0; i < (int)enemies.size() ; i++) {
		bool collision_Y = bullet->real_py() + bullet->height() > enemies[i].real_py() &&
						   bullet->real_py() < enemies[i].real_py() + enemies[i].height();

		bool collision_X = bullet->px() > enemies[i].px() - enemies[i].width()&&
						   bullet->px() < enemies[i].px() + enemies[i].width() / 2;


		if (collision_Y && collision_X) {
			enemies[i].changeLifeBy(-bullet->getDamage());
			return i;
		}
	}
	return -1;
}

/**
 * @brief Checks for collision with a single bullet against the player
 * 
 * @param bullet an enemy bullet
 * @param player the player
 * @return true when hit
 * @return false when not hit
 */
bool Game::checkBulletColission(Projectile* bullet, Ship& player) {
	bool collision_Y = bullet->real_py() + bullet->height() > player.real_py() &&
						bullet->real_py() < player.real_py() + player.height() / 2;

	bool collision_X = bullet->px() > player.px() - player.width()&&
						bullet->px() < player.px() + player.width() / 3;


	if (collision_Y && collision_X) {
		Mix_PlayChannel(-1, player_hit, 0);
		player.changeLifeBy(-bullet->getDamage());
		return true;
	}
	
	return false;
}

void Game::checkEnemyCollision(std::vector<Ship>& enemies) {
	for (int i = 0; i < (int)enemies.size() - 1 ; i++) {
		int xspace = enemies[i].real_px() + enemies[i].width() - enemies[i+1].real_px();
		int yspace = enemies[i].real_py() + enemies[i].height() - enemies[i+1].real_py();

		if (xspace <= abs(60) && yspace <= abs(60)) {
			if (xspace > 0) {
		   		enemies[i].moveXBy(-5);
				enemies[i+1].moveXBy(5);
			}
			else {
				enemies[i].moveXBy(5);
				enemies[i+1].moveXBy(-5);
			}

			if (yspace > 0) {
		   		enemies[i].moveYBy(-5);
				enemies[i+1].moveYBy(5);
			}
			else {
				enemies[i].moveYBy(5);
				enemies[i+1].moveYBy(-5);
			}
		}
	}
}

void Game::changeGameStateTo(GameState state) {
	// 	MENU, SETTINGS, SETUP_GAME, SETUP_NEW_ROUND, GAME, PAUSE, DEAD, WON

	switch (state) {
	
		case MENU:
			gameOnPlay = false;
			selector->setPos(60, BOTTOM_HEIGHT_CENTER - 25);
			selected_option_index = 0;
			selected_game_option = menu_options[selected_option_index].state.state;

		break;
		
		case SETTINGS:
			settings.selected = INVERT_X;
		break;
		
		case SETUP_GAME:
			gameOnPlay = true;
		break;

		case SETUP_NEW_ROUND:
			game_round++;
			threads[2] = threadCreate(threadWait, (void*)&round_ready, 	STACKSIZE, prio-1, -2, false);

		break;

		case GAME:
			is_new_highscore = false;
			is_new_best_time = false;
		break;

		case PAUSE:
			selected_option_index = 0;
			selected_game_option = pause_options[selected_option_index].state.state;
			selector->setPos(60, pause_options[selected_option_index].yPos);
		break;

		case DEAD:
			health_low = false;
			timeOfDeath = gameTime();
			score += (long int)timeOfDeath;
			score_str = std::string(std::to_string(score));

			selector->setPos(15, BOTTOM_HEIGHT_CENTER - 25);
			selected_game_option = SETUP_GAME;

						
			if (highscore < score) {
				highscore_file.open(SCORE_FILE, std::fstream::in);

				long int tmp;
				highscore_file >> tmp;

				u64 tmp_time;
				highscore_file >> tmp_time;
				highscore_file.close();

				highscore_file.open(SCORE_FILE, std::fstream::out);
				highscore_file <<  score << '\n';
				highscore_file <<  tmp_time  << '\n';
				highscore_file.close();
				is_new_highscore = true;

				highscore = score;
			}

			if (best_time < timeOfDeath) {
				highscore_file.open(SCORE_FILE, std::fstream::in);
				long int tmp_score;
				highscore_file >> tmp_score;
				highscore_file.close();

				highscore_file.open(SCORE_FILE, std::fstream::out);
				highscore_file <<  tmp_score 	<< '\n';
				highscore_file <<  timeOfDeath  << '\n';
				highscore_file.close();
				is_new_best_time = true;

				best_time = timeOfDeath;
			}

			gameOnPlay = false;
		break;


		
		default:
			std::exception();
		break;
	}
	
	game_state = state;
}

void Game::applySettings() {

	if (settings.options[INVERT_X]) {
		RIGHT	= KEY_LEFT;
		LEFT	= KEY_RIGHT;
	} else {
		RIGHT	= KEY_RIGHT;
		LEFT	= KEY_LEFT;
	}
	
	if (settings.options[INVERT_Y]) {
		UP 		= KEY_DOWN;
		DOWN 	= KEY_UP;
	} else {
		UP		= KEY_UP;
		DOWN	= KEY_DOWN;
	}

}

void Game::moveAllBackgroundsWithVelocity(float vFirst, float vSecond) {
	moveBackground(&mv_bg_top1, &mv_bg_top1_copy, vFirst, 	TOP_SCREEN_WIDTH);
	moveBackground(&mv_bg_top2, &mv_bg_top2_copy, vSecond, 	TOP_SCREEN_WIDTH);

	moveBackground(&mv_bg_bot1, &mv_bg_bot1_copy, vFirst, 	BOTTOM_SCREEN_WIDTH);
	moveBackground(&mv_bg_bot2, &mv_bg_bot2_copy, vSecond, 	BOTTOM_SCREEN_WIDTH);
}

void Game::updateScoreOnHit() {
	if (gameTime() < 60)
		score += 300;
	else if (gameTime() < 180)
		score += 250;
	else if (gameTime() < 60 * 5)
		score += 200;
	else
		score += 500;
}

void Game::checkInGamePlayerInput() {
	if (kDown & KEY_START) {
		Mix_PauseMusic();
		selected_game_option = GAME;
		game_state = PAUSE;
	}
	
	if (kDown & KEY_A) {
		if (canShoot) {
			std::shared_ptr<Projectile> p = player.shoot();
			p->setDisplayOnBottom(true);
			p->setPos(&player);
			projectiles.push_back(p);
			puts("Fire!");
			canShoot = false;
		}
	}

	
	if (kDown & KEY_B) {
		//player.shootBomb()
	}

	if (kDown & KEY_R) {
		// Mix_PlayChannel(-1, pong, 0);

		player.changeGun();
		canShoot = true;
	}

	if (kHeld & RIGHT) {
		move = player.px() + playerVelocityX;
		if (move < BOTTOM_SCREEN_WIDTH)
			player.moveXBy(playerVelocityX);
	}

	if (kHeld & LEFT) {
		move = player.px() - playerVelocityX;
		if (move > 0 + player.width())
			player.moveXBy(-playerVelocityX);
	}

	if (kHeld & UP) {
		move = player.py() - playerVelocityY;
		if (move > 0 + player.height() )
			player.moveYBy(playerVelocityY);
	} 
	
	if (kHeld & DOWN) {
		move = player.py() + playerVelocityY;
		if (move < BOTTOM_SCREEN_HEIGHT)
			player.moveYBy(-playerVelocityY);
	}
}

void Game::drawHealthBar() {

	u32 color;
	float life = player.getLife();

	if (life > 6)
		color = GREEN;
	else if (life > 3)
		color = YELLOW;
	else
		color = RED;

	for (int i = 0, x = 10; i < (int)life; ++i, x += 15)
		C2D_DrawRectangle(x, BOTTOM_SCREEN_HEIGHT - 10, 0, 15, 10, color, color, color, color);
}

void Game::handle_MENU() {
	moveAllBackgroundsWithVelocity(-0.2, -0.04);	

	gameScene::renderScene(top);
		drawBackgroundTop();

		C2D_DrawRectangle(50,50,0,TOP_SCREEN_WIDTH - 50*2, TOP_SCREEN_HEIGHT - 50*2, C2D_Color32(255,0,0,255), C2D_Color32(255,0,0,255), C2D_Color32(255,0,0,255), C2D_Color32(255,0,0,255));
		textScene::drawTopMenuText();

	gameScene::renderScene(bottom);
		drawBackgroundBottom();

		textScene::drawBottomMenuText();
		gameScene::draw(selector.get());

	
	if (kDown & KEY_A) {
		Mix_PlayChannel(-1, confirm_select, 0);

		if (menu_options[selected_option_index].state.option == NEXT_SONG) {
			Mix_HookMusicFinished(playNextSong);
			Mix_FadeOutMusic(100);
		}
		else
			changeGameStateTo(selected_game_option);					

	}

	if (kDown & KEY_UP) {
		Mix_PlayChannel(-1, move_up_menu, 0);
		
		selected_option_index = (selected_option_index - 1) % menu_options.size();
		selected_game_option = menu_options[selected_option_index].state.state;
		selector->setPos(60, menu_options[selected_option_index].yPos);
	}

	if (kDown & KEY_DOWN) {
		Mix_PlayChannel(-1, move_down_menu, 0);
		selected_option_index = (selected_option_index + 1) % menu_options.size();
		selected_game_option = menu_options[selected_option_index].state.state;
		selector->setPos(60, menu_options[selected_option_index].yPos);
	}
}

void Game::handle_GAME() {
	gameOnPlay = true;


	checkInGamePlayerInput();


	for (auto& enemy : enemies) {
		enemy.moveRandomly();

		//	Adjust shoot frecuency by number of enemies
		if (random() % (4 * enemies.size()) == 0) {
			std::shared_ptr<Projectile> p = enemy.shoot();
			p->setPos(&enemy);
			enemy_projectiles.push_back(p);
		}
	}

	moveAllBackgroundsWithVelocity(-0.2, -0.04);	

	if (player.getLife() < 4)
		health_low = true;

	gameScene::renderScene(top);
		drawBackgroundTop();

		for (Ship& enemy : enemies)
			gameScene::draw(&enemy);
		
		
		// render and move player projectiles on the top screen
		for (int i = 0; i < (int)projectiles.size(); i++) {

			if (projectiles[i]->shouldDisplayOnTop()) {
				if (projectiles[i]->py() < LOWEST_SCREEN_POSITION) {
					projectiles[i]->setDisplayOnTop(false);
					projectiles.erase(projectiles.begin() + i);
				} else {
					projectiles[i]->moveYBy(projectiles[i]->getVelocity());
					int ship_collided = checkBulletColission(projectiles[i].get(), enemies);
					if (ship_collided != -1) {
						updateScoreOnHit();

						Mix_PlayChannel(-1, hit, 0);
						projectiles.erase(projectiles.begin() + i);
						printf("Colided: %d\tLife: %.1f\n", ship_collided, enemies[ship_collided].getLife());
						
						if (enemies[ship_collided].getLife() <= 0) {
							enemies.erase(enemies.begin() + ship_collided);
							score += 1000;
						}
					}
					else
						gameScene::draw(projectiles[i].get());
				}
			}
		}

		// render and move enemy projectiles on the top screen
		for (int i = 0; i < (int)enemy_projectiles.size(); i++) {

			if (enemy_projectiles[i]->shouldDisplayOnTop()) {
				enemy_projectiles[i]->moveYBy(enemy_projectiles[i]->getVelocity());
				printf("%f", enemy_projectiles[i]->getVelocity());
				gameScene::draw(enemy_projectiles[i].get());
				if (enemy_projectiles[i]->py() > TOP_SCREEN_HEIGHT) {
					enemy_projectiles[i]->setY(-5);
					enemy_projectiles[i]->moveXBy(-40);
					enemy_projectiles[i]->setDisplayOnBottom(true);
					enemy_projectiles[i]->setDisplayOnTop(false);
				}
			} 
		}
		

	gameScene::renderScene(bottom);
		drawBackgroundBottom();


		// render and move player projectiles on the bottom screen
		for (int i = 0; i < (int)projectiles.size(); i++) {
			
			if (projectiles[i]->shouldDisplayOnBottom()) {
				projectiles[i]->moveYBy(projectiles[i]->getVelocity());
				gameScene::draw(projectiles[i].get());
				if (projectiles[i]->py() < LOWEST_SCREEN_POSITION) {
					projectiles[i]->setY(240);
					projectiles[i]->moveXBy(40);
					projectiles[i]->setDisplayOnBottom(false);
					projectiles[i]->setDisplayOnTop(true);
				}
			} 
		}

		// render and move enemy projectiles on the bottom screen
		for (int i = 0; i < (int)enemy_projectiles.size(); i++) {

			if (enemy_projectiles[i]->shouldDisplayOnBottom()) {
				if (enemy_projectiles[i]->py() > BOTTOM_SCREEN_HEIGHT) {
					enemy_projectiles[i]->setDisplayOnBottom(false);
				} else {

					enemy_projectiles[i]->moveYBy(enemy_projectiles[i]->getVelocity());
					bool player_hit = checkBulletColission(enemy_projectiles[i].get(), player);
					if (player_hit) {
						enemy_projectiles.erase(enemy_projectiles.begin() + i);							
						
						if (player.getLife() <= 0) {
							Mix_PlayChannel(-1, game_over, 0);
							sleep(1);
							changeGameStateTo(DEAD);
						}	
					}
					else
						gameScene::draw(enemy_projectiles[i].get());
				}
			}
		}

		if (enemies.size() == 0)	
			changeGameStateTo(SETUP_NEW_ROUND);


		gameScene::draw(&player);
		gameScene::draw(&bar);

		drawHealthBar();
}

void Game::handle_PAUSE() {
	gameScene::renderScene(top);
		drawBackgroundTop();

		for (Ship& enemy : enemies)
			gameScene::draw(&enemy);


		for (int i = 0; i < (int)projectiles.size(); i++)
			if (projectiles[i]->shouldDisplayOnTop())
				gameScene::draw(projectiles[i].get());
		

		for (int i = 0; i < (int)enemy_projectiles.size(); i++)
			if (enemy_projectiles[i]->shouldDisplayOnTop())
				gameScene::draw(enemy_projectiles[i].get());	
		
		
		C2D_DrawRectSolid(0,0,0,TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT, C2D_Color32(0,0,0,150));
		textScene::drawPauseMenuTop();


	gameScene::renderScene(bottom);
		drawBackgroundBottom();


		for (int i = 0; i < (int)projectiles.size(); i++)
			if (projectiles[i]->shouldDisplayOnBottom())
				gameScene::draw(projectiles[i].get());
		

		for (int i = 0; i < (int)enemy_projectiles.size(); i++)
			if (enemy_projectiles[i]->shouldDisplayOnBottom())
				gameScene::draw(enemy_projectiles[i].get());	
		
		
		gameScene::draw(&player);

		C2D_DrawRectSolid(0,0,0,TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT, C2D_Color32(0,0,0,150));

		textScene::drawPauseMenuBottom();
		gameScene::draw(selector.get());



	if (kDown & KEY_SELECT) {
		Mix_ResumeMusic();
		changeGameStateTo(GAME);
	}



	if (kDown & KEY_A) {
		Mix_PlayChannel(-1, confirm_select, 0);
		
		if (selected_game_option == GAME || selected_game_option == MENU)
			Mix_ResumeMusic();

		changeGameStateTo(selected_game_option);
	} 

	if (kDown & KEY_UP) {
		Mix_PlayChannel(-1, move_up_menu, 0);

		selected_option_index = (selected_option_index - 1) % pause_options.size();
		selected_game_option = pause_options[selected_option_index].state.state;
		selector->setPos(60, pause_options[selected_option_index].yPos);

	}

	if (kDown & KEY_DOWN) {
		Mix_PlayChannel(-1, move_down_menu, 0);

		selected_option_index = (selected_option_index + 1) % pause_options.size();
		selected_game_option = pause_options[selected_option_index].state.state;
		selector->setPos(60, pause_options[selected_option_index].yPos);

	}

}

void Game::handle_DEAD() {
	if (kDown & KEY_UP) {
		Mix_PlayChannel(-1, move_up_menu, 0);
		selected_game_option = SETUP_GAME;
		selector->setPos(15, BOTTOM_HEIGHT_CENTER - 25);

	}

	if (kDown & KEY_DOWN) {
		Mix_PlayChannel(-1, move_down_menu, 0);
		selected_game_option = MENU;
		selector->setPos(15, BOTTOM_HEIGHT_CENTER + 5);
	}

	if (kDown & KEY_A) {
		Mix_PlayChannel(-1, confirm_select, 0);
		selector->setPos(60, BOTTOM_HEIGHT_CENTER - 25);
		changeGameStateTo(selected_game_option); 
	}

	
	gameScene::renderScene(top);
		textScene::drawDeathMenuTop(score_str, timeOfDeath, game_round);
		
		if (is_new_highscore)
			textScene::drawNewHighscore();

		if (is_new_best_time)
			textScene::drawNewBestTime();			

	gameScene::renderScene(bottom);
		textScene::drawDeathMenuBottom();
		gameScene::draw(selector.get());
}

void Game::handle_SETTINGS() {
	if ( kDown & KEY_UP) {
		Mix_PlayChannel(-1, move_up_menu, 0);
		settings.selected = INVERT_X;
	}

	if ( kDown & KEY_DOWN) {
		Mix_PlayChannel(-1, move_down_menu, 0);
		settings.selected = INVERT_Y;
	}

	if ( kDown & KEY_LEFT) {
		Mix_PlayChannel(-1, click, 0);
		settings.options[settings.selected] = NO;
	}

	if ( kDown & KEY_RIGHT) {
		Mix_PlayChannel(-1, click, 0);
		settings.options[settings.selected] = YES;
	}


	if ( kDown & KEY_B) {
		Mix_PlayChannel(-1, confirm_select, 0);

		applySettings();

		if (gameOnPlay)
			changeGameStateTo(PAUSE);
		else
			changeGameStateTo(MENU);
	}

	moveAllBackgroundsWithVelocity(-0.2, -0.04);	

	gameScene::renderScene(top);
		drawBackgroundTop();

		C2D_DrawRectangle(50,50,0,TOP_SCREEN_WIDTH - 50*2, TOP_SCREEN_HEIGHT - 50*2, C2D_Color32(255,0,0,255), C2D_Color32(255,0,0,255), C2D_Color32(255,0,0,255), C2D_Color32(255,0,0,255));
		textScene::drawTopMenuText();
		

	gameScene::renderScene(bottom);
		textScene::drawSettingsMenu(&settings);
}

void Game::handle_SETUP_GAME() {
	setupGame(player, enemies, projectiles, enemy_projectiles);
	
	threads[2] = threadCreate(threadWait, (void*)&round_ready, 	STACKSIZE, prio-1, -2, false);
	changeGameStateTo(SETUP_NEW_ROUND);
}

void Game::handle_SETUP_NEW_ROUND() {

	moveAllBackgroundsWithVelocity(-5, -1);	

	if (!round_ready) {
		gameScene::renderScene(top);
			drawBackgroundTop();


			for (Ship& enemy : enemies) {
				gameScene::draw(&enemy);
			}

			textScene::drawRoundCenter(game_round);


		gameScene::renderScene(bottom);
			drawBackgroundBottom();
			
			gameScene::draw(&player);

	} else {

		player.setLife(10);
		enemies.clear();
		projectiles.clear();
		enemy_projectiles.clear();
		startNewRound(enemies);

		changeGameStateTo(GAME);
		round_ready = false;
	}
}

void Game::start() {
	while (aptMainLoop()) {
		hidScanInput();

		kDown = hidKeysDown();
		kHeld = hidKeysHeld();

		C2D_Fade(C2D_Color32(0,0,0,fade));

		switch (game_state) {
			case MENU:  			handle_MENU(); 				break;
			case GAME:  			handle_GAME(); 				break;
			case PAUSE: 			handle_PAUSE(); 			break;
			case DEAD:  			handle_DEAD(); 				break;
			case SETTINGS: 			handle_SETTINGS(); 			break;
			case SETUP_GAME: 		handle_SETUP_GAME(); 		break;
			case SETUP_NEW_ROUND: 	handle_SETUP_NEW_ROUND(); 	break;
		}

			
		C3D_FrameRate(60);
		C3D_FrameEnd(0);
		gspWaitForVBlank();
	}
}
