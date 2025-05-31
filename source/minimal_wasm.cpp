// #include <emscripten.h>
// #include <emscripten/html5.h>
// #include <stdio.h>
// #include <string.h>
// #include <vector>
// #include <cstdint>
// #include <cmath>
// #include <random>
// #include <chrono>

// // Canvas size
// const int CANVAS_WIDTH = 800;
// const int CANVAS_HEIGHT = 600;

// // Colors for our simple rendering
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

// // Star field for background
// struct Star {
//     float x, y;
//     float speed;
//     uint32_t color;
// };

// const int NUM_STARS = 100;

// // Sprite structure for better rendering
// struct Sprite {
//     int x, y;
//     int width, height;
//     uint32_t color;
//     float rotation;  // in radians
//     float scale;
// };

// // Bullet structure
// struct Bullet {
//     float x, y;
//     float vx, vy;
//     int width, height;
//     uint32_t color;
//     int damage;
//     bool active;
// };

// // Enemy structure
// struct Enemy {
//     float x, y;
//     float vx, vy;
//     int width, height;
//     uint32_t color;
//     int health;
//     bool active;
//     int type;
//     float angle;
//     float speed;
// };

// // Particle effect
// struct Particle {
//     float x, y;
//     float vx, vy;
//     float life;
//     float max_life;
//     uint32_t color;
//     float size;
// };

// // Game state
// struct GameState {
//     // Player properties
//     Sprite player;
//     int player_speed;
//     bool is_moving_left;
//     bool is_moving_right;
//     bool is_moving_up;
//     bool is_moving_down;
//     bool is_shooting;
//     int shoot_cooldown;
//     int shoot_cooldown_max;
//     int health;
//     int max_health;
    
//     // Game objects
//     std::vector<Bullet> bullets;
//     std::vector<Enemy> enemies;
//     std::vector<Particle> particles;
//     std::vector<Star> stars;
    
//     // Game state
//     int score;
//     bool game_over;
//     bool win;
//     int level;
//     int enemy_spawn_timer;
//     int enemy_spawn_max;
    
//     // Animation timers
//     float time;
//     float game_time;
// };

// // Global game state
// GameState g_game;

// // JavaScript drawing functions for better performance
// EM_JS(void, js_clear_screen, (int color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.fillRect(0, 0, Module.canvas.width, Module.canvas.height);
// });

// EM_JS(void, js_draw_rect, (int x, int y, int width, int height, int color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.fillRect(x, y, width, height);
// });

// EM_JS(void, js_draw_circle, (int x, int y, int radius, int color), {
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

// EM_JS(void, js_draw_triangle, (int x1, int y1, int x2, int y2, int x3, int y3, int color), {
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

// EM_JS(void, js_draw_text, (const char* text, int x, int y, int size, int color), {
//     const ctx = Module.canvas.getContext('2d');
//     ctx.font = size + 'px Arial';
//     ctx.fillStyle = 'rgba(' + 
//         ((color >> 16) & 0xFF) + ',' + 
//         ((color >> 8) & 0xFF) + ',' + 
//         (color & 0xFF) + ',' + 
//         ((color >> 24) & 0xFF) / 255.0 + ')';
//     ctx.fillText(UTF8ToString(text), x, y);
// });

// // Clear the screen
// void clear_screen(uint32_t color) {
//     js_clear_screen(color);
// }

// // Draw a filled rectangle
// void draw_rect(int x, int y, int width, int height, uint32_t color) {
//     js_draw_rect(x, y, width, height, color);
// }

// // Draw a filled circle
// void draw_circle(int x, int y, int radius, uint32_t color) {
//     js_draw_circle(x, y, radius, color);
// }

// // Draw a filled triangle
// void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
//     js_draw_triangle(x1, y1, x2, y2, x3, y3, color);
// }

// // Draw text
// void draw_text(const char* text, int x, int y, int size, uint32_t color) {
//     js_draw_text(text, x, y, size, color);
// }

// // Draw a spaceship
// void draw_spaceship(int x, int y, int size, float rotation, uint32_t color) {
//     // Convert the center point and size to triangle points
//     int x1 = x + sin(rotation) * size;
//     int y1 = y - cos(rotation) * size;
    
//     int x2 = x + sin(rotation + 2.5) * size * 0.7;
//     int y2 = y - cos(rotation + 2.5) * size * 0.7;
    
//     int x3 = x + sin(rotation - 2.5) * size * 0.7;
//     int y3 = y - cos(rotation - 2.5) * size * 0.7;
    
//     draw_triangle(x1, y1, x2, y2, x3, y3, color);
    
//     // Add engine glow
//     int x4 = x + sin(rotation + 3.14) * size * 0.5;
//     int y4 = y - cos(rotation + 3.14) * size * 0.5;
    
//     draw_circle(x4, y4, size / 4, COLOR_ORANGE);
// }

// // Random number generator
// std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// // Get a random float between min and max
// float random_float(float min, float max) {
//     std::uniform_real_distribution<float> dist(min, max);
//     return dist(rng);
// }

// // Initialize the game
// void init_game() {
//     // Initialize player
//     g_game.player.x = CANVAS_WIDTH / 2;
//     g_game.player.y = CANVAS_HEIGHT - 50;
//     g_game.player.width = 30;
//     g_game.player.height = 30;
//     g_game.player.color = COLOR_GREEN;
//     g_game.player.rotation = 0;
//     g_game.player.scale = 1.0f;
    
//     g_game.player_speed = 5;
//     g_game.is_moving_left = false;
//     g_game.is_moving_right = false;
//     g_game.is_moving_up = false;
//     g_game.is_moving_down = false;
//     g_game.is_shooting = false;
//     g_game.shoot_cooldown = 0;
//     g_game.shoot_cooldown_max = 10;
//     g_game.health = 100;
//     g_game.max_health = 100;
    
//     // Clear game objects
//     g_game.bullets.clear();
//     g_game.enemies.clear();
//     g_game.particles.clear();
    
//     // Initialize stars for parallax background
//     g_game.stars.clear();
//     for (int i = 0; i < NUM_STARS; i++) {
//         Star star;
//         star.x = random_float(0, CANVAS_WIDTH);
//         star.y = random_float(0, CANVAS_HEIGHT);
//         star.speed = random_float(0.5f, 3.0f);
        
//         // Randomize star colors with different shades of blue/white
//         float brightness = random_float(0.5f, 1.0f);
//         star.color = 0xFF000000 | 
//                     (int(200 * brightness) << 16) | 
//                     (int(220 * brightness) << 8) | 
//                     (int(255 * brightness));
        
//         g_game.stars.push_back(star);
//     }
    
//     // Initialize game state
//     g_game.score = 0;
//     g_game.game_over = false;
//     g_game.win = false;
//     g_game.level = 1;
//     g_game.enemy_spawn_timer = 0;
//     g_game.enemy_spawn_max = 60;
//     g_game.time = 0;
//     g_game.game_time = 0;
    
//     // Create some initial enemies
//     for (int i = 0; i < 5; i++) {
//         Enemy enemy;
//         enemy.x = 100 + i * 120;
//         enemy.y = 50;
//         enemy.width = 30;
//         enemy.height = 30;
//         enemy.color = COLOR_RED;
//         enemy.health = 10;
//         enemy.active = true;
//         enemy.type = i % 3;  // Different enemy types
//         enemy.vx = random_float(-1.0f, 1.0f);
//         enemy.vy = random_float(0.5f, 1.5f);
//         enemy.angle = 3.14159f;  // Facing down
//         enemy.speed = 1.0f + (i % 3);
        
//         g_game.enemies.push_back(enemy);
//     }
// }

// // Update game state
// void update_game() {
//     // Update game time
//     g_game.time += 1.0f / 60.0f;
//     if (!g_game.game_over) {
//         g_game.game_time += 1.0f / 60.0f;
//     }
    
//     // Move player
//     if (g_game.is_moving_left) {
//         g_game.player.x -= g_game.player_speed;
//         if (g_game.player.x < 0) {
//             g_game.player.x = 0;
//         }
//     }
//     if (g_game.is_moving_right) {
//         g_game.player.x += g_game.player_speed;
//         if (g_game.player.x > CANVAS_WIDTH) {
//             g_game.player.x = CANVAS_WIDTH;
//         }
//     }
//     if (g_game.is_moving_up) {
//         g_game.player.y -= g_game.player_speed;
//         if (g_game.player.y < 0) {
//             g_game.player.y = 0;
//         }
//     }
//     if (g_game.is_moving_down) {
//         g_game.player.y += g_game.player_speed;
//         if (g_game.player.y > CANVAS_HEIGHT) {
//             g_game.player.y = CANVAS_HEIGHT;
//         }
//     }

//     // Handle shooting
//     if (g_game.is_shooting && g_game.shoot_cooldown <= 0) {
//         // Create a new bullet
//         Bullet bullet;
//         bullet.x = g_game.player.x;
//         bullet.y = g_game.player.y - 20;
//         bullet.vx = 0;
//         bullet.vy = -10;
//         bullet.width = 5;
//         bullet.height = 10;
//         bullet.color = COLOR_YELLOW;
//         bullet.damage = 10;
//         bullet.active = true;
        
//         g_game.bullets.push_back(bullet);
        
//         // Add muzzle flash particles
//         for (int i = 0; i < 5; i++) {
//             Particle p;
//             p.x = g_game.player.x;
//             p.y = g_game.player.y - 15;
//             p.vx = random_float(-2.0f, 2.0f);
//             p.vy = random_float(-5.0f, -1.0f);
//             p.life = random_float(0.2f, 0.5f);
//             p.max_life = p.life;
//             p.color = COLOR_YELLOW;
//             p.size = random_float(2.0f, 5.0f);
            
//             g_game.particles.push_back(p);
//         }
        
//         g_game.shoot_cooldown = g_game.shoot_cooldown_max;
//     }
    
//     if (g_game.shoot_cooldown > 0) {
//         g_game.shoot_cooldown--;
//     }

//     // Update bullets
//     for (auto& bullet : g_game.bullets) {
//         bullet.x += bullet.vx;
//         bullet.y += bullet.vy;
        
//         // Check if bullet is off-screen
//         if (bullet.y < -bullet.height || bullet.y > CANVAS_HEIGHT + bullet.height ||
//             bullet.x < -bullet.width || bullet.x > CANVAS_WIDTH + bullet.width) {
//             bullet.active = false;
//         }
//     }

//     // Remove inactive bullets
//     g_game.bullets.erase(
//         std::remove_if(g_game.bullets.begin(), g_game.bullets.end(),
//             [](const Bullet& bullet) {
//                 return !bullet.active;
//             }),
//         g_game.bullets.end()
//     );

//     // Update enemies
//     for (auto& enemy : g_game.enemies) {
//         if (!enemy.active) continue;
        
//         // Different enemy movement patterns based on type
//         switch (enemy.type) {
//             case 0: // Straight down
//                 enemy.y += enemy.speed;
//                 break;
//             case 1: // Sine wave
//                 enemy.x += sin(g_game.time * 2.0f) * enemy.speed;
//                 enemy.y += enemy.speed * 0.8f;
//                 break;
//             case 2: // Circle pattern
//                 enemy.x = CANVAS_WIDTH/2 + cos(g_game.time * enemy.speed) * 150;
//                 enemy.y = CANVAS_HEIGHT/4 + sin(g_game.time * enemy.speed) * 100;
//                 break;
//             default:
//                 // Custom movement
//                 enemy.x += enemy.vx;
//                 enemy.y += enemy.vy;
                
//                 // Bounce off screen edges
//                 if (enemy.x < 0 || enemy.x > CANVAS_WIDTH) {
//                     enemy.vx *= -1;
//                 }
//                 if (enemy.y < 0) {
//                     enemy.vy *= -1;
//                 }
//         }
        
//         // Check if enemy is off the bottom of the screen
//         if (enemy.y > CANVAS_HEIGHT + enemy.height) {
//             enemy.active = false;
//         }
//     }

//     // Remove inactive enemies
//     g_game.enemies.erase(
//         std::remove_if(g_game.enemies.begin(), g_game.enemies.end(),
//             [](const Enemy& enemy) {
//                 return !enemy.active;
//             }),
//         g_game.enemies.end()
//     );

//     // Spawn new enemies
//     if (!g_game.game_over && g_game.enemy_spawn_timer <= 0 && g_game.enemies.size() < 10) {
//         Enemy enemy;
//         enemy.x = random_float(30, CANVAS_WIDTH - 30);
//         enemy.y = -30;
//         enemy.width = 30;
//         enemy.height = 30;
//         enemy.color = COLOR_RED;
//         enemy.health = 10 * g_game.level;
//         enemy.active = true;
//         enemy.type = rand() % 3;  // Random enemy type
//         enemy.vx = random_float(-2.0f, 2.0f);
//         enemy.vy = random_float(1.0f, 3.0f);
//         enemy.angle = 3.14159f;  // Facing down
//         enemy.speed = 1.0f + (g_game.level * 0.5f);
        
//         g_game.enemies.push_back(enemy);
        
//         // Reset spawn timer
//         g_game.enemy_spawn_timer = g_game.enemy_spawn_max;
//     }
    
//     if (g_game.enemy_spawn_timer > 0) {
//         g_game.enemy_spawn_timer--;
//     }

//     // Collision detection: bullets hitting enemies
//     for (auto& bullet : g_game.bullets) {
//         if (!bullet.active) continue;
        
//         for (auto& enemy : g_game.enemies) {
//             if (!enemy.active) continue;
            
//             // Simple collision detection (rectangle intersection)
//             if (bullet.x + bullet.width > enemy.x - enemy.width/2 &&
//                 bullet.x - bullet.width < enemy.x + enemy.width/2 &&
//                 bullet.y + bullet.height > enemy.y - enemy.height/2 &&
//                 bullet.y - bullet.height < enemy.y + enemy.height/2) {
                
//                 // Bullet hit enemy
//                 enemy.health -= bullet.damage;
//                 bullet.active = false;
                
//                 // Create explosion particles
//                 for (int i = 0; i < 10; i++) {
//                     Particle p;
//                     p.x = bullet.x;
//                     p.y = bullet.y;
//                     p.vx = random_float(-3.0f, 3.0f);
//                     p.vy = random_float(-3.0f, 3.0f);
//                     p.life = random_float(0.5f, 1.0f);
//                     p.max_life = p.life;
//                     p.color = enemy.health <= 0 ? COLOR_ORANGE : COLOR_YELLOW;
//                     p.size = random_float(2.0f, 6.0f);
                    
//                     g_game.particles.push_back(p);
//                 }
                
//                 // Check if enemy is destroyed
//                 if (enemy.health <= 0) {
//                     enemy.active = false;
//                     g_game.score += 100 * g_game.level;
                    
//                     // Create explosion particles
//                     for (int i = 0; i < 20; i++) {
//                         Particle p;
//                         p.x = enemy.x;
//                         p.y = enemy.y;
//                         p.vx = random_float(-5.0f, 5.0f);
//                         p.vy = random_float(-5.0f, 5.0f);
//                         p.life = random_float(0.5f, 1.5f);
//                         p.max_life = p.life;
//                         p.color = i % 2 == 0 ? COLOR_ORANGE : COLOR_RED;
//                         p.size = random_float(3.0f, 8.0f);
                        
//                         g_game.particles.push_back(p);
//                     }
//                 }
                
//                 break;
//             }
//         }
//     }
    
//     // Collision detection: enemies hitting player
//     if (!g_game.game_over) {
//         for (auto& enemy : g_game.enemies) {
//             if (!enemy.active) continue;
            
//             // Simple collision detection (rectangle intersection)
//             if (g_game.player.x + g_game.player.width/2 > enemy.x - enemy.width/2 &&
//                 g_game.player.x - g_game.player.width/2 < enemy.x + enemy.width/2 &&
//                 g_game.player.y + g_game.player.height/2 > enemy.y - enemy.height/2 &&
//                 g_game.player.y - g_game.player.height/2 < enemy.y + enemy.height/2) {
                
//                 // Enemy hit player
//                 g_game.health -= 10;
//                 enemy.active = false;
                
//                 // Create explosion particles
//                 for (int i = 0; i < 20; i++) {
//                     Particle p;
//                     p.x = g_game.player.x;
//                     p.y = g_game.player.y;
//                     p.vx = random_float(-5.0f, 5.0f);
//                     p.vy = random_float(-5.0f, 5.0f);
//                     p.life = random_float(0.5f, 1.5f);
//                     p.max_life = p.life;
//                     p.color = COLOR_ORANGE;
//                     p.size = random_float(3.0f, 8.0f);
                    
//                     g_game.particles.push_back(p);
//                 }
                
//                 // Check if player is destroyed
//                 if (g_game.health <= 0) {
//                     g_game.game_over = true;
//                     g_game.win = false;
                    
//                     // Create more explosion particles
//                     for (int i = 0; i < 50; i++) {
//                         Particle p;
//                         p.x = g_game.player.x;
//                         p.y = g_game.player.y;
//                         p.vx = random_float(-8.0f, 8.0f);
//                         p.vy = random_float(-8.0f, 8.0f);
//                         p.life = random_float(1.0f, 3.0f);
//                         p.max_life = p.life;
//                         p.color = i % 3 == 0 ? COLOR_RED : (i % 3 == 1 ? COLOR_ORANGE : COLOR_YELLOW);
//                         p.size = random_float(5.0f, 15.0f);
                        
//                         g_game.particles.push_back(p);
//                     }
//                 }
                
//                 break;
//             }
//         }
//     }
    
//     // Update particles
//     for (auto& p : g_game.particles) {
//         p.x += p.vx;
//         p.y += p.vy;
//         p.life -= 1.0f / 60.0f;
//     }
    
//     // Remove dead particles
//     g_game.particles.erase(
//         std::remove_if(g_game.particles.begin(), g_game.particles.end(),
//             [](const Particle& p) {
//                 return p.life <= 0;
//             }),
//         g_game.particles.end()
//     );
    
//     // Update stars (parallax background)
//     for (auto& star : g_game.stars) {
//         star.y += star.speed;
//         if (star.y > CANVAS_HEIGHT) {
//             star.y = 0;
//             star.x = random_float(0, CANVAS_WIDTH);
//         }
//     }
    
//     // Level up if all enemies are destroyed
//     if (g_game.enemies.empty() && !g_game.game_over) {
//         g_game.level++;
//         g_game.enemy_spawn_max = std::max(10, g_game.enemy_spawn_max - 5); // Spawn enemies faster
        
//         // Create level up effect
//         for (int i = 0; i < 50; i++) {
//             Particle p;
//             p.x = random_float(0, CANVAS_WIDTH);
//             p.y = random_float(0, CANVAS_HEIGHT);
//             p.vx = random_float(-3.0f, 3.0f);
//             p.vy = random_float(-3.0f, 3.0f);
//             p.life = random_float(1.0f, 2.0f);
//             p.max_life = p.life;
//             p.color = COLOR_GREEN;
//             p.size = random_float(3.0f, 8.0f);
            
//             g_game.particles.push_back(p);
//         }
//     }
// }

// // Render the game
// void render_game() {
//     // Clear screen with dark blue background
//     clear_screen(0xFF000020);
    
//     // Draw stars
//     for (const auto& star : g_game.stars) {
//         draw_circle(star.x, star.y, 1 + star.speed / 2, star.color);
//     }
//     // Draw particles
//     for (const auto& p : g_game.particles) {
//         uint32_t color = p.color;
//         // Fade out particles as they age
//         float alpha = p.life / p.max_life;
//         color = (color & 0x00FFFFFF) | (static_cast<uint32_t>(alpha * 255) << 24);
//         draw_circle(p.x, p.y, p.size, color);
//     }
    
//     // Draw bullets
//     for (const auto& bullet : g_game.bullets) {
//         if (!bullet.active) continue;
//         draw_rect(bullet.x - bullet.width/2, bullet.y - bullet.height/2, bullet.width, bullet.height, bullet.color);
//     }
    
//     // Draw enemies
//     for (const auto& enemy : g_game.enemies) {
//         if (!enemy.active) continue;
        
//         // Draw different enemy types
//         switch (enemy.type) {
//             case 0: // Basic triangle enemy
//                 draw_triangle(
//                     enemy.x, enemy.y - enemy.height/2,
//                     enemy.x - enemy.width/2, enemy.y + enemy.height/2,
//                     enemy.x + enemy.width/2, enemy.y + enemy.height/2,
//                     enemy.color
//                 );
//                 break;
//             case 1: // Circle enemy
//                 draw_circle(enemy.x, enemy.y, enemy.width/2, enemy.color);
//                 break;
//             case 2: // Diamond enemy
//                 draw_triangle(
//                     enemy.x, enemy.y - enemy.height/2,
//                     enemy.x - enemy.width/2, enemy.y,
//                     enemy.x, enemy.y + enemy.height/2,
//                     enemy.color
//                 );
//                 draw_triangle(
//                     enemy.x, enemy.y - enemy.height/2,
//                     enemy.x + enemy.width/2, enemy.y,
//                     enemy.x, enemy.y + enemy.height/2,
//                     enemy.color
//                 );
//                 break;
//             default:
//                 draw_rect(enemy.x - enemy.width/2, enemy.y - enemy.height/2, enemy.width, enemy.height, enemy.color);
//                 break;
//         }
//     }
    
//     // Draw player spaceship
//     draw_spaceship(g_game.player.x, g_game.player.y, g_game.player.width/2, 0, g_game.player.color);
    
//     // Draw health bar
//     draw_rect(10, 10, 200, 20, COLOR_GRAY);
//     draw_rect(10, 10, 200 * g_game.health / g_game.max_health, 20, COLOR_GREEN);
    
//     // Draw score and level
//     char score_text[64];
//     sprintf(score_text, "Score: %d", g_game.score);
//     draw_text(score_text, 10, 50, 24, COLOR_WHITE);
    
//     char level_text[64];
//     sprintf(level_text, "Level: %d", g_game.level);
//     draw_text(level_text, 10, 80, 24, COLOR_WHITE);
    
//     // Display game over message if applicable
//     if (g_game.game_over) {
//         draw_rect(CANVAS_WIDTH/2 - 150, CANVAS_HEIGHT/2 - 100, 300, 200, 0xCC000000);
//         draw_text("GAME OVER", CANVAS_WIDTH/2 - 100, CANVAS_HEIGHT/2 - 50, 32, COLOR_RED);
        
//         char final_score[64];
//         sprintf(final_score, "Final Score: %d", g_game.score);
//         draw_text(final_score, CANVAS_WIDTH/2 - 100, CANVAS_HEIGHT/2, 24, COLOR_WHITE);
        
//         draw_text("Press SPACE to restart", CANVAS_WIDTH/2 - 120, CANVAS_HEIGHT/2 + 50, 18, COLOR_WHITE);
//     }
// }

// // Main game loop (called by requestAnimationFrame)
// void game_loop() {
//     update_game();
//     render_game();
// }

// // Handle key down events
// extern "C" {
//     EMSCRIPTEN_KEEPALIVE
//     void handle_key_down(int key_code) {
//         switch (key_code) {
//             case 37: // Left arrow
//                 g_game.is_moving_left = true;
//                 break;
//             case 38: // Up arrow
//                 g_game.is_moving_up = true;
//                 break;
//             case 39: // Right arrow
//                 g_game.is_moving_right = true;
//                 break;
//             case 40: // Down arrow
//                 g_game.is_moving_down = true;
//                 break;
//             case 32: // Space
//                 if (g_game.game_over) {
//                     // Restart game if game over
//                     init_game();
//                 } else {
//                     g_game.is_shooting = true;
//                 }
//                 break;
//         }
//     }

//     // Handle key up events
//     EMSCRIPTEN_KEEPALIVE
//     void handle_key_up(int key_code) {
//         switch (key_code) {
//             case 37: // Left arrow
//                 g_game.is_moving_left = false;
//                 break;
//             case 38: // Up arrow
//                 g_game.is_moving_up = false;
//                 break;
//             case 39: // Right arrow
//                 g_game.is_moving_right = false;
//                 break;
//             case 40: // Down arrow
//                 g_game.is_moving_down = false;
//                 break;
//             case 32: // Space
//                 g_game.is_shooting = false;
//                 break;
//         }
//     }

//     // Reset the game
//     EMSCRIPTEN_KEEPALIVE
//     void reset_game() {
//         init_game();
//     }

//     // Get the current score
//     EMSCRIPTEN_KEEPALIVE
//     int get_score() {
//         return g_game.score;
//     }
    
//     // Get the current level
//     EMSCRIPTEN_KEEPALIVE
//     int get_level() {
//         return g_game.level;
//     }
    
//     // Get player health
//     EMSCRIPTEN_KEEPALIVE
//     int get_health() {
//         return g_game.health;
//     }
    
//     // Get game state
//     EMSCRIPTEN_KEEPALIVE
//     bool is_game_over() {
//         return g_game.game_over;
//     }
    
//     // Get number of enemies
//     EMSCRIPTEN_KEEPALIVE
//     int get_enemy_count() {
//         return g_game.enemies.size();
//     }
// }

// // Main entry point
// int main() {
//     printf("MilkySpace WASM Minimal Version\n");
    
//     // Initialize the game
//     init_game();
    
//     // Set up the main loop to run at 60 FPS
//     emscripten_set_main_loop(game_loop, 60, 1);
    
//     return 0;
// }