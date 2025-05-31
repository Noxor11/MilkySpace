// #include <emscripten.h>
// #include <emscripten/html5.h>
// #include <stdio.h>
// #include <string.h>
// #include <vector>
// #include <cstdint>
// #include <cmath>
// #include <random>
// #include <chrono>
// #include <memory>
// #include <algorithm>

// // Canvas size
// const int CANVAS_WIDTH = 800;
// const int CANVAS_HEIGHT = 600;

// // Colors
// #define COLOR_BLACK   0xFF000000
// #define COLOR_RED     0xFFFF0000
// #define COLOR_GREEN   0xFF00FF00
// #define COLOR_BLUE    0xFF0000FF
// #define COLOR_WHITE   0xFFFFFFFF
// #define COLOR_YELLOW  0xFFFFFF00
// #define COLOR_MAGENTA 0xFFFF00FF
// #define COLOR_CYAN    0xFF00FFFF
// #define COLOR_GRAY    0xFF808080
// #define COLOR_PURPLE  0xFF800080
// #define COLOR_ORANGE  0xFFFF8000
// #define COLOR_NAVY    0xFF000080
// #define COLOR_PINK    0xFFFFC0CB
// #define COLOR_LIME    0xFF32CD32
// #define COLOR_TEAL    0xFF008080

// // MilkySpace game parameters
// const float PLAYER_SPEED = 5.0f;
// const float ENEMY_SPEED_BASE = 1.5f;
// const float BULLET_SPEED = 8.0f;
// const int PLAYER_SIZE = 30;
// const int ENEMY_SIZE = 25;
// const int BULLET_SIZE = 5;
// const int PLAYER_HEALTH = 100;
// const int ENEMY_HEALTH_BASE = 10;
// const int SCORE_PER_KILL = 100;
// const float PARTICLE_LIFE = 1.0f;
// const int MAX_PARTICLES = 200;
// const int MAX_STARS = 150;
// const int MAX_BULLETS = 50;
// const int BULLET_COOLDOWN = 10;
// const int ENEMY_SPAWN_RATE_BASE = 60;

// // Star background
// struct Star {
//     float x, y;
//     float speed;
//     uint32_t color;
// };

// // Game entity base
// struct GameObject {
//     float x, y;
//     float vx, vy;
//     int width, height;
//     uint32_t color;
//     float rotation;
//     bool active;
//     int health;
// };

// // Player ship
// struct Ship : GameObject {
//     int score;
//     int level;
//     int shootCooldown;
// };

// // Enemy ship
// struct Enemy : GameObject {
//     int type;
//     float angle;
//     float speed;
//     int shootCooldown;
//     int shootRate;
// };

// // Projectile/bullet
// struct Bullet : GameObject {
//     int damage;
//     bool playerOwned;
// };

// // Particle effect
// struct Particle : GameObject {
//     float life;
//     float maxLife;
//     float size;
// };

// // Game state
// struct GameState {
//     // Player
//     Ship player;
//     bool isMovingLeft;
//     bool isMovingRight;
//     bool isMovingUp;
//     bool isMovingDown;
//     bool isShooting;
//     int shootCooldown;
    
//     // Game objects
//     std::vector<Bullet> bullets;
//     std::vector<Enemy> enemies;
//     std::vector<Particle> particles;
//     std::vector<Star> stars;
    
//     // Game state
//     int score;
//     int highScore;
//     int level;
//     bool gameOver;
//     bool gamePaused;
//     bool gameStarted;
//     int enemySpawnTimer;
//     int enemySpawnRate;
    
//     // Timing
//     float deltaTime;
//     float gameTime;
//     float lastFrameTime;
// };

// // Global game state
// GameState g_game;

// // External JavaScript functions for rendering
// EM_JS(void, js_clear_screen, (uint32_t color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.fillRect(0, 0, Module.canvas.width, Module.canvas.height);
// });

// EM_JS(void, js_draw_rect, (int x, int y, int width, int height, uint32_t color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.fillRect(x, y, width, height);
// });

// EM_JS(void, js_draw_circle, (int x, int y, int radius, uint32_t color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.beginPath();
//     ctx.arc(x, y, radius, 0, Math.PI * 2, true);
//     ctx.fill();
// });

// EM_JS(void, js_draw_triangle, (int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.beginPath();
//     ctx.moveTo(x1, y1);
//     ctx.lineTo(x2, y2);
//     ctx.lineTo(x3, y3);
//     ctx.closePath();
//     ctx.fill();
// });

// EM_JS(void, js_draw_text, (const char* text, int x, int y, int size, uint32_t color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.font = size + 'px Arial';
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.fillText(UTF8ToString(text), x, y);
// });

// // Rendering functions
// void clear_screen(uint32_t color) {
//     js_clear_screen(color);
// }

// void draw_rect(int x, int y, int width, int height, uint32_t color) {
//     js_draw_rect(x, y, width, height, color);
// }

// void draw_circle(int x, int y, int radius, uint32_t color) {
//     js_draw_circle(x, y, radius, color);
// }

// void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
//     js_draw_triangle(x1, y1, x2, y2, x3, y3, color);
// }

// void draw_text(const char* text, int x, int y, int size, uint32_t color) {
//     js_draw_text(text, x, y, size, color);
// }

// // Draw a spaceship - player or enemy
// void draw_spaceship(int x, int y, int size, float rotation, uint32_t color, int type = 0) {
//     // Ship body
//     int x1 = x + sin(rotation) * size;
//     int y1 = y - cos(rotation) * size;
    
//     int x2 = x + sin(rotation + 2.5) * size * 0.7;
//     int y2 = y - cos(rotation + 2.5) * size * 0.7;
    
//     int x3 = x + sin(rotation - 2.5) * size * 0.7;
//     int y3 = y - cos(rotation - 2.5) * size * 0.7;
    
//     draw_triangle(x1, y1, x2, y2, x3, y3, color);
    
//     // Engine glow
//     int x4 = x + sin(rotation + 3.14) * size * 0.5;
//     int y4 = y - cos(rotation + 3.14) * size * 0.5;
    
//     // Different engine colors based on type
//     uint32_t engineColor = COLOR_ORANGE;
//     if (type == 1) engineColor = COLOR_CYAN;
//     if (type == 2) engineColor = COLOR_PURPLE;
    
//     draw_circle(x4, y4, size / 4, engineColor);
    
//     // Add details based on enemy type
//     if (type == 1) {
//         // Type 1: Add side guns
//         int x5 = x + sin(rotation + 1.57) * size * 0.5;
//         int y5 = y - cos(rotation + 1.57) * size * 0.5;
//         int x6 = x + sin(rotation - 1.57) * size * 0.5;
//         int y6 = y - cos(rotation - 1.57) * size * 0.5;
        
//         draw_circle(x5, y5, size / 6, COLOR_GRAY);
//         draw_circle(x6, y6, size / 6, COLOR_GRAY);
//     }
//     else if (type == 2) {
//         // Type 2: Add top shield
//         int x5 = x + sin(rotation) * size * 0.3;
//         int y5 = y - cos(rotation) * size * 0.3;
        
//         draw_circle(x5, y5, size / 3, COLOR_PURPLE);
//     }
// }

// // Random number generator
// std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// // Get a random float between min and max
// float random_float(float min, float max) {
//     std::uniform_real_distribution<float> dist(min, max);
//     return dist(rng);
// }

// // Create a particle explosion effect
// void create_explosion(float x, float y, int count, uint32_t color, float size = 3.0f) {
//     for (int i = 0; i < count; i++) {
//         if (g_game.particles.size() >= MAX_PARTICLES) {
//             // Replace oldest particle
//             g_game.particles[0] = g_game.particles.back();
//             g_game.particles.pop_back();
//         }
        
//         Particle particle;
//         particle.x = x;
//         particle.y = y;
//         particle.vx = random_float(-3.0f, 3.0f);
//         particle.vy = random_float(-3.0f, 3.0f);
//         particle.life = PARTICLE_LIFE;
//         particle.maxLife = PARTICLE_LIFE;
//         particle.color = color;
//         particle.size = random_float(size * 0.5f, size * 1.5f);
//         particle.active = true;
        
//         g_game.particles.push_back(particle);
//     }
// }

// // Create a star for the parallax background
// void create_star() {
//     Star star;
//     star.x = random_float(0, CANVAS_WIDTH);
//     star.y = random_float(0, CANVAS_HEIGHT);
//     star.speed = random_float(0.5f, 3.0f);
    
//     // Randomize star colors with different shades of blue/white
//     float brightness = random_float(0.5f, 1.0f);
//     star.color = 0xFF000000 | 
//                 (int(200 * brightness) << 16) | 
//                 (int(220 * brightness) << 8) | 
//                 (int(255 * brightness));
    
//     g_game.stars.push_back(star);
// }

// // Create a new enemy
// void spawn_enemy() {
//     if (g_game.gameOver) return;
    
//     Enemy enemy;
//     enemy.x = random_float(50, CANVAS_WIDTH - 50);
//     enemy.y = random_float(-50, 20);
//     enemy.vx = random_float(-1.0f, 1.0f);
//     enemy.vy = ENEMY_SPEED_BASE * (0.8f + 0.2f * g_game.level);
//     enemy.width = ENEMY_SIZE;
//     enemy.height = ENEMY_SIZE;
//     enemy.health = ENEMY_HEALTH_BASE + (g_game.level - 1) * 5;
//     enemy.active = true;
//     enemy.type = rand() % 3;  // Random enemy type (0, 1, or 2)
//     enemy.rotation = 3.14159f;  // Facing down
//     enemy.speed = ENEMY_SPEED_BASE * (0.8f + 0.2f * g_game.level);
//     enemy.shootCooldown = 0;
//     enemy.shootRate = 120 - (g_game.level * 5);  // Shoot more frequently at higher levels
    
//     // Different colors for different enemy types
//     switch (enemy.type) {
//         case 0:
//             enemy.color = COLOR_RED;
//             break;
//         case 1:
//             enemy.color = COLOR_CYAN;
//             break;
//         case 2:
//             enemy.color = COLOR_PURPLE;
//             break;
//     }
    
//     g_game.enemies.push_back(enemy);
// }

// // Fire a bullet
// void fire_bullet(float x, float y, float vx, float vy, bool playerOwned, uint32_t color = COLOR_YELLOW) {
//     if (g_game.bullets.size() >= MAX_BULLETS) {
//         // Replace oldest bullet
//         g_game.bullets[0] = g_game.bullets.back();
//         g_game.bullets.pop_back();
//     }
    
//     Bullet bullet;
//     bullet.x = x;
//     bullet.y = y;
//     bullet.vx = vx;
//     bullet.vy = vy;
//     bullet.width = BULLET_SIZE;
//     bullet.height = BULLET_SIZE;
//     bullet.color = color;
//     bullet.active = true;
//     bullet.damage = playerOwned ? 10 : 5;
//     bullet.playerOwned = playerOwned;
    
//     g_game.bullets.push_back(bullet);
// }

// // Check collision between two game objects
// bool check_collision(const GameObject& a, const GameObject& b) {
//     return (a.x + a.width/2 >= b.x - b.width/2 &&
//             a.x - a.width/2 <= b.x + b.width/2 &&
//             a.y + a.height/2 >= b.y - b.height/2 &&
//             a.y - a.height/2 <= b.y + b.height/2);
// }

// // Initialize the game
// void init_game() {
//     // Clear any existing game objects
//     g_game.bullets.clear();
//     g_game.enemies.clear();
//     g_game.particles.clear();
//     g_game.stars.clear();
    
//     // Initialize player
//     g_game.player.x = CANVAS_WIDTH / 2;
//     g_game.player.y = CANVAS_HEIGHT - 50;
//     g_game.player.vx = 0;
//     g_game.player.vy = 0;
//     g_game.player.width = PLAYER_SIZE;
//     g_game.player.height = PLAYER_SIZE;
//     g_game.player.color = COLOR_GREEN;
//     g_game.player.rotation = 0;  // Facing up
//     g_game.player.active = true;
//     g_game.player.health = PLAYER_HEALTH;
//     g_game.player.score = 0;
//     g_game.player.level = 1;
//     g_game.player.shootCooldown = 0;
    
//     // Initialize game state
//     g_game.isMovingLeft = false;
//     g_game.isMovingRight = false;
//     g_game.isMovingUp = false;
//     g_game.isMovingDown = false;
//     g_game.isShooting = false;
//     g_game.shootCooldown = 0;
//     g_game.score = 0;
//     g_game.level = 1;
//     g_game.gameOver = false;
//     g_game.gamePaused = false;
//     g_game.gameStarted = false;
//     g_game.enemySpawnTimer = 0;
//     g_game.enemySpawnRate = ENEMY_SPAWN_RATE_BASE;
//     g_game.deltaTime = 0.016f;  // Default to 60 FPS
//     g_game.gameTime = 0;
//     g_game.lastFrameTime = 0;
    
//     // Create initial star field
//     for (int i = 0; i < MAX_STARS; i++) {
//         create_star();
//     }
    
//     // Create a few initial enemies
//     for (int i = 0; i < 5; i++) {
//         spawn_enemy();
//     }
    
//     printf("Game initialized successfully\n");
// }

// // Reset the game
// void reset_game() {
//     // Save high score
//     if (g_game.score > g_game.highScore) {
//         g_game.highScore = g_game.score;
//     }
    
//     init_game();
// }

// // Update the game state
// void update_game() {
//     if (g_game.gamePaused) return;
    
//     if (!g_game.gameStarted) {
//         g_game.gameStarted = true;
//     }
    
//     // Update game time
//     g_game.gameTime += g_game.deltaTime;
    
//     // Update player based on input
//     if (g_game.isMovingLeft) {
//         g_game.player.vx = -PLAYER_SPEED;
//     } else if (g_game.isMovingRight) {
//         g_game.player.vx = PLAYER_SPEED;
//     } else {
//         g_game.player.vx = 0;
//     }
    
//     if (g_game.isMovingUp) {
//         g_game.player.vy = -PLAYER_SPEED;
//     } else if (g_game.isMovingDown) {
//         g_game.player.vy = PLAYER_SPEED;
//     } else {
//         g_game.player.vy = 0;
//     }
    
//     // Update player position
//     g_game.player.x += g_game.player.vx;
//     g_game.player.y += g_game.player.vy;
    
//     // Keep player in bounds
//     if (g_game.player.x < PLAYER_SIZE/2) g_game.player.x = PLAYER_SIZE/2;
//     if (g_game.player.x > CANVAS_WIDTH - PLAYER_SIZE/2) g_game.player.x = CANVAS_WIDTH - PLAYER_SIZE/2;
//     if (g_game.player.y < PLAYER_SIZE/2) g_game.player.y = PLAYER_SIZE/2;
//     if (g_game.player.y > CANVAS_HEIGHT - PLAYER_SIZE/2) g_game.player.y = CANVAS_HEIGHT - PLAYER_SIZE/2;
    
//     // Handle player shooting
//     if (g_game.isShooting && g_game.shootCooldown <= 0) {
//         fire_bullet(
//             g_game.player.x, 
//             g_game.player.y - PLAYER_SIZE/2, 
//             0, 
//             -BULLET_SPEED, 
//             true
//         );
//         g_game.shootCooldown = BULLET_COOLDOWN;
//     }
    
//     if (g_game.shootCooldown > 0) {
//         g_game.shootCooldown--;
//     }
    
//     // Update stars (parallax background)
//     for (auto& star : g_game.stars) {
//         star.y += star.speed;
        
//         // Wrap stars to top when they go off screen
//         if (star.y > CANVAS_HEIGHT) {
//             star.y = 0;
//             star.x = random_float(0, CANVAS_WIDTH);
//         }
//     }
    
//     // Update bullets
//     for (auto& bullet : g_game.bullets) {
//         if (!bullet.active) continue;
        
//         bullet.x += bullet.vx;
//         bullet.y += bullet.vy;
        
//         // Deactivate bullets that go off screen
//         if (bullet.y < -10 || bullet.y > CANVAS_HEIGHT + 10 || 
//             bullet.x < -10 || bullet.x > CANVAS_WIDTH + 10) {
//             bullet.active = false;
//         }
//     }
    
//     // Update enemies
//     for (auto& enemy : g_game.enemies) {
//         if (!enemy.active) continue;
        
//         // Move enemy
//         enemy.x += enemy.vx;
//         enemy.y += enemy.vy;
        
//         // Enemies that go off screen wrap around from the top
//         if (enemy.y > CANVAS_HEIGHT + 50) {
//             enemy.y = -30;
//             enemy.x = random_float(30, CANVAS_WIDTH - 30);
//         }
        
//         // Bounce off sides
//         if (enemy.x < ENEMY_SIZE/2 || enemy.x > CANVAS_WIDTH - ENEMY_SIZE/2) {
//             enemy.vx *= -1;
//         }
        
//         // Enemy shooting logic
//         enemy.shootCooldown--;
//         if (enemy.shootCooldown <= 0 && random_float(0, 100) < 2) {
//             // Fire a bullet toward player
//             float dx = g_game.player.x - enemy.x;
//             float dy = g_game.player.y - enemy.y;
//             float dist = sqrt(dx*dx + dy*dy);
            
//             if (dist > 0) {
//                 float vx = dx / dist * BULLET_SPEED * 0.5f;
//                 float vy = dy / dist * BULLET_SPEED * 0.5f;
                
//                 uint32_t bulletColor;
//                 switch (enemy.type) {
//                     case 0: bulletColor = COLOR_RED; break;
//                     case 1: bulletColor = COLOR_CYAN; break;
//                     case 2: bulletColor = COLOR_PURPLE; break;
//                     default: bulletColor = COLOR_RED;
//                 }
                
//                 fire_bullet(enemy.x, enemy.y + ENEMY_SIZE/2, vx, vy, false, bulletColor);
//             }
            
//             enemy.shootCooldown = enemy.shootRate;
//         }
//     }
    
//     // Update particles
//     for (auto& particle : g_game.particles) {
//         if (!particle.active) continue;
        
//         particle.x += particle.vx;
//         particle.y += particle.vy;
//         particle.life -= g_game.deltaTime;
        
//         if (particle.life <= 0) {
//             particle.active = false;
//         }
//     }
    
//     // Collision detection: player bullets vs enemies
//     for (auto& bullet : g_game.bullets) {
//         if (!bullet.active || !bullet.playerOwned) continue;
        
//         for (auto& enemy : g_game.enemies) {
//             if (!enemy.active) continue;
            
//             if (check_collision(bullet, enemy)) {
//                 // Hit enemy
//                 enemy.health -= bullet.damage;
//                 bullet.active = false;
                
//                 // Create hit effect
//                 create_explosion(bullet.x, bullet.y, 5, bullet.color, 2.0f);
                
//                 if (enemy.health <= 0) {
//                     // Enemy destroyed
//                     enemy.active = false;
//                     g_game.score += SCORE_PER_KILL * (enemy.type + 1);
                    
//                     // Create explosion
//                     create_explosion(enemy.x, enemy.y, 20, enemy.color, 4.0f);
//                 }
                
//                 break;
//             }
//         }
//     }
    
//     // Collision detection: enemy bullets vs player
//     for (auto& bullet : g_game.bullets) {
//         if (!bullet.active || bullet.playerOwned) continue;
        
//         if (check_collision(bullet, g_game.player)) {
//             // Player hit
//             g_game.player.health -= bullet.damage;
//             bullet.active = false;
            
//             // Create hit effect
//             create_explosion(bullet.x, bullet.y, 10, bullet.color, 3.0f);
            
//             if (g_game.player.health <= 0) {
//                 // Player destroyed - game over
//                 g_game.gameOver = true;
//                 create_explosion(g_game.player.x, g_game.player.y, 40, g_game.player.color, 5.0f);
//             }
//         }
//     }
    
//     // Collision detection: enemies vs player
//     for (auto& enemy : g_game.enemies) {
//         if (!enemy.active) continue;
        
//         if (check_collision(enemy, g_game.player)) {
//             // Player hit by enemy - major damage
//             g_game.player.health -= 20;
//             enemy.health = 0;
//             enemy.active = false;
            
//             // Create explosion
//             create_explosion(enemy.x, enemy.y, 25, enemy.color, 5.0f);
            
//             if (g_game.player.health <= 0) {
//                 // Player destroyed - game over
//                 g_game.gameOver = true;
//                 create_explosion(g_game.player.x, g_game.player.y, 40, g_game.player.color, 5.0f);
//             }
//         }
//     }
    
//     // Remove inactive entities
//     g_game.bullets.erase(
//         std::remove_if(g_game.bullets.begin(), g_game.bullets.end(), 
//             [](const Bullet& b) { return !b.active; }),
//         g_game.bullets.end()
//     );
    
//     g_game.enemies.erase(
//         std::remove_if(g_game.enemies.begin(), g_game.enemies.end(), 
//             [](const Enemy& e) { return !e.active; }),
//         g_game.enemies.end()
//     );
    
//     g_game.particles.erase(
//         std::remove_if(g_game.particles.begin(), g_game.particles.end(), 
//             [](const Particle& p) { return !p.active; }),
//         g_game.particles.end()
//     );
    
//     // Spawn new enemies
//     g_game.enemySpawnTimer++;
//     if (g_game.enemySpawnTimer >= g_game.enemySpawnRate && g_game.enemies.size() < 10 + g_game.level * 2) {
//         spawn_enemy();
//         g_game.enemySpawnTimer = 0;
//     }
    
//     // Level progression
//     if (g_game.score >= g_game.level * 1000) {
//         g_game.level++;
//         g_game.enemySpawnRate = std::max(20, ENEMY_SPAWN_RATE_BASE - (g_game.level * 5));
        
//         // Spawn some enemies for the new level
//         for (int i = 0; i < 3; i++) {
//             spawn_enemy();
//         }
        
//         // Create level up effect
//         create_explosion(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 50, COLOR_GREEN, 6.0f);
//     }
// }

// // Render the game
// void render_game() {
//     // Clear screen
//     clear_screen(COLOR_BLACK);
    
//     // Draw stars (background)
//     for (const auto& star : g_game.stars) {
//         draw_circle(star.x, star.y, 1, star.color);
//     }
    
//     // Draw particles
//     for (const auto& particle : g_game.particles) {
//         if (!particle.active) continue;
        
//         // Fade particle based on remaining life
//         uint32_t color = particle.color;
//         float alpha = particle.life / particle.maxLife;
//         color = (color & 0x00FFFFFF) | ((uint32_t)(alpha * 255) << 24);
        
//         draw_circle(particle.x, particle.y, particle.size, color);
//     }
    
//     // Draw bullets
//     for (const auto& bullet : g_game.bullets) {
//         if (!bullet.active) continue;
        
//         draw_circle(bullet.x, bullet.y, bullet.width / 2, bullet.color);
//     }
    
//     // Draw enemies
//     for (const auto& enemy : g_game.enemies) {
//         if (!enemy.active) continue;
        
//         draw_spaceship(enemy.x, enemy.y, enemy.width / 2, enemy.rotation, enemy.color, enemy.type);
//     }
    
//     // Draw player if not game over
//     if (!g_game.gameOver) {
//         draw_spaceship(g_game.player.x, g_game.player.y, g_game.player.width / 2, g_game.player.rotation, g_game.player.color);
//     }
    
//     // Draw health bar
//     int healthBarWidth = 200;
//     int healthBarHeight = 20;
//     int healthX = 20;
//     int healthY = 20;
    
//     // Background
//     draw_rect(healthX, healthY, healthBarWidth, healthBarHeight, COLOR_GRAY);
    
//     // Health amount
//     float healthPercent = (float)g_game.player.health / PLAYER_HEALTH;
//     uint32_t healthColor = COLOR_GREEN;
    
//     if (healthPercent < 0.6) healthColor = COLOR_YELLOW;
//     if (healthPercent < 0.3) healthColor = COLOR_RED;
    
//     draw_rect(healthX, healthY, healthBarWidth * healthPercent, healthBarHeight, healthColor);
    
//     // Draw UI: score, level, etc.
//     char scoreText[100];
//     sprintf(scoreText, "Score: %d", g_game.score);
//     draw_text(scoreText, 20, 60, 20, COLOR_WHITE);
    
//     char levelText[100];
//     sprintf(levelText, "Level: %d", g_game.level);
//     draw_text(levelText, 20, 90, 20, COLOR_WHITE);
    
//     char highScoreText[100];
//     sprintf(highScoreText, "High Score: %d", g_game.highScore);
//     draw_text(highScoreText, CANVAS_WIDTH - 200, 30, 20, COLOR_WHITE);
    
//     // Draw enemy count
//     char enemyText[100];
//     sprintf(enemyText, "Enemies: %d", (int)g_game.enemies.size());
//     draw_text(enemyText, CANVAS_WIDTH - 200, 60, 20, COLOR_WHITE);
    
//     // Game over screen
//     if (g_game.gameOver) {
//         draw_rect(CANVAS_WIDTH/2 - 150, CANVAS_HEIGHT/2 - 100, 300, 200, 0xCC000000);
        
//         draw_text("GAME OVER", CANVAS_WIDTH/2 - 100, CANVAS_HEIGHT/2 - 50, 32, COLOR_RED);
        
//         char finalScoreText[100];
//         sprintf(finalScoreText, "Final Score: %d", g_game.score);
//     }
// }