#pragma once
#include "gameObject.h"
#include "ship.h"
#include "enemy.h"

#define SCORE_FOLDER 	"/3ds/MilkySpace"
#define SCORE_FILE		SCORE_FOLDER "/score"



class Game{

    bool is_new_highscore = false;
    bool is_new_best_time = false;

    enum GameState{
        MENU, SETTINGS, SETUP_GAME, SETUP_NEW_ROUND, GAME, PAUSE, DEAD
    };

    enum GameOption{
	    NEXT_SONG = 10
    };

    union selecting_item{
        GameState state;
        GameOption option;

    };

    struct selection{
        selecting_item state;
        int yPos;
    };

    GameState game_state 			= MENU; 
    GameState selected_game_option 	= SETUP_GAME;
    bool gameOnPlay = false;
    int selected_option_index = 0;

    Settings settings = initSettings();

    bool round_ready = false;

    u64 timeOfDeath;


    std::array<selection, 3> menu_options;
    std::array<selection, 3> pause_options; 


    std::shared_ptr<Object> selector;

    s32 prio = 0;


    int game_round = 1;

    u64 last_game_time = svcGetSystemTick() / SYSCLOCK_ARM11;

    long long unsigned int score = 0;
    long long unsigned int highscore 	= 0;

    std::string score_str;


    std::fstream highscore_file;
    u64 best_time 		= 0;



    float playerVelocityX = 5;
    float playerVelocityY = 5;


    long long int UP 	= KEY_UP;
    long long int DOWN 	= KEY_DOWN;
    long long int RIGHT = KEY_RIGHT;
    long long int LEFT 	= KEY_LEFT;


    u32 kDown;
	u32 kHeld;


	float move = 0;


    // --------------------------- Game Objects -------------------------- //
    Object background_top;
    Object mv_bg_top1;
    Object mv_bg_top1_copy;

    Object mv_bg_top2;
    Object mv_bg_top2_copy;


    Object background_bot;
    Object mv_bg_bot1;
    Object mv_bg_bot1_copy;

    Object mv_bg_bot2;
    Object mv_bg_bot2_copy;

    Ship player;
    Object bar;

    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::vector<std::shared_ptr<Projectile>> enemy_projectiles;

    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;

    std::vector<Enemy> enemies;



public:
    Game();

    void start();

private:

    void initOptions();

    void setupGame(Ship& player, std::vector<Enemy>& enemies,
                    std::vector<std::shared_ptr<Projectile>>& projectiles,
                    std::vector<std::shared_ptr<Projectile>>& enemy_projectiles);

    void getHighscore();

    u64 gameTime();

    void startNewRound(std::vector<Enemy>& enemies);

    int  checkBulletColission(Projectile* bullet, std::vector<Enemy>& enemies);
    bool checkBulletColission(Projectile* bullet, Ship& player);
    void checkEnemyCollision(std::vector<Ship>& enemies);
    void changeGameStateTo(GameState state);

    void applySettings();

    void moveBackground(Object* bg, Object* bg_copy, float velocity, int screen_width);
    void moveAllBackgroundsWithVelocity(float vFirst, float vSecond);

    void drawBackgroundTop();
    void drawBackgroundBot();

    void handle_MENU();
    void handle_GAME();
    void handle_PAUSE();
    void handle_DEAD();
    void handle_SETTINGS();
    void handle_SETUP_GAME();
    void handle_SETUP_NEW_ROUND();
};