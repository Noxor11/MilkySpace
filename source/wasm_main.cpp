// #ifdef __WASM__

// #include "game.h"
// #include <emscripten.h>
// #include <emscripten/html5.h>
// #include <SDL2/SDL.h>
// #include <chrono>
// #include <ctime>


// // Global game instance
// static Game* gameInstance = nullptr;
// static bool gameInitialized = false;
// static int frameCount = 0;
// static float deltaTime = 0.0f;
// static std::chrono::high_resolution_clock::time_point lastFrameTime;

// // Main game loop to be called by Emscripten
// void gameLoop() {
//     if (!gameInstance || !gameInitialized) return;
    
//     // Calculate delta time
//     auto currentTime = std::chrono::high_resolution_clock::now();
//     deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
//     lastFrameTime = currentTime;
    
//     // Process input
//     ScanInput();
    
//     // Update game state
//     gameInstance->update();
    
//     // Render the frame
//     FrameBegin(0);
//     gameInstance->render();
//     EndFrame(0);
    
//     // Count frames for debugging
//     frameCount++;
// }

// // Create and initialize the game
// extern "C" {
//     EMSCRIPTEN_KEEPALIVE
//     int initGame() {
//         // Initialize random number generator
//         srand(static_cast<unsigned int>(time(NULL)));
        
//         // Initialize systems
//         Result res;
        
//         printf("Initializing WebAssembly game...\n");
        
//         res = InitROMFS();
//         if (res != 0) {
//             HandleBadResult(res);
//             return res;
//         }
        
//         res = InitGraphics();
//         if (res != 0) {
//             HandleBadResult(res);
//             return res;
//         }
        
//         res = InitAudio();
//         if (res != 0) {
//             HandleBadResult(res);
//             return res;
//         }
        
//         res = InitTextScene();
//         if (res != 0) {
//             HandleBadResult(res);
//             return res;
//         }
        
//         // Create game instance
//         gameInstance = new Game();
        
//         // Initialize timing
//         lastFrameTime = std::chrono::high_resolution_clock::now();
        
//         // Set up the main loop with 60 FPS target
//         emscripten_set_main_loop(gameLoop, 60, 1);
        
//         gameInitialized = true;
//         printf("Game initialized successfully\n");
        
//         return 0;
//     }
    
//     EMSCRIPTEN_KEEPALIVE
//     void shutdownGame() {
//         if (gameInstance) {
//             delete gameInstance;
//             gameInstance = nullptr;
//         }
        
//         gameInitialized = false;
        
//         ExitTextScene();
//         ExitGraphics();
        
//         printf("Game shutdown complete\n");
//     }
    
//     EMSCRIPTEN_KEEPALIVE
//     int getFrameCount() {
//         return frameCount;
//     }
    
//     EMSCRIPTEN_KEEPALIVE
//     float getDeltaTime() {
//         return deltaTime;
//     }
    
//     EMSCRIPTEN_KEEPALIVE
//     bool isGameInitialized() {
//         return gameInitialized;
//     }
// }

// #endif // __WASM__