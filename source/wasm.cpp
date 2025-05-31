// #include "gameSpecification.h"
// #include "sound.h"

// #ifdef __WASM__

// #include <emscripten.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_mixer.h>
// #include <chrono>
// #include <unordered_map>

// // Forward declarations of external variables
// extern Mix_Music* songs[7];
// extern Mix_Chunk* laser;
// extern Mix_Chunk* heavy_bullet;
// extern Mix_Chunk* hit;
// extern Mix_Chunk* player_hit;
// extern Mix_Chunk* game_over;
// extern Mix_Chunk* low_health;
// extern Mix_Chunk* move_up_menu;
// extern Mix_Chunk* move_down_menu;
// extern Mix_Chunk* confirm_select;
// extern Mix_Chunk* click;
// extern SpriteSheet spriteSheet;

// // Global variables
// static SDL_Window* window = nullptr;
// static SDL_Renderer* renderer = nullptr;
// static SDL_Texture* texture = nullptr;
// static int screenWidth = 800;
// static int screenHeight = 600;
// static bool sdlInitialized = false;
// static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
// static Uint32 lastFrameTime = 0;
// static const int TARGET_FPS = 60;
// static const int FRAME_DELAY = 1000 / TARGET_FPS;

// // Key mapping
// static std::unordered_map<SDL_Scancode, int> keyMap = {
//     {SDL_SCANCODE_UP, static_cast<int>(Key::UP)},
//     {SDL_SCANCODE_DOWN, static_cast<int>(Key::DOWN)},
//     {SDL_SCANCODE_LEFT, static_cast<int>(Key::LEFT)},
//     {SDL_SCANCODE_RIGHT, static_cast<int>(Key::RIGHT)},
//     {SDL_SCANCODE_Z, static_cast<int>(Key::A)},
//     {SDL_SCANCODE_X, static_cast<int>(Key::B)},
//     {SDL_SCANCODE_RETURN, static_cast<int>(Key::START)},
//     {SDL_SCANCODE_RSHIFT, static_cast<int>(Key::SELECT)}
// };

// // Implementation of platform-specific functions
// extern "C" {

// uint64_t GetSystemTick() {
//     auto now = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count();
//     return duration;
// }

// uint32_t Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
//     return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), r, g, b, a);
// }

// uint32_t Color32f(float r, float g, float b, float a) {
//     return Color32(
//         static_cast<uint8_t>(r * 255.0f),
//         static_cast<uint8_t>(g * 255.0f),
//         static_cast<uint8_t>(b * 255.0f),
//         static_cast<uint8_t>(a * 255.0f)
//     );
// }

// float SetFramerate(float fps) {
//     // Use SDL_Delay to cap framerate in browser
//     Uint32 currentTime = SDL_GetTicks();
//     Uint32 frameTime = currentTime - lastFrameTime;
    
//     if (frameTime < FRAME_DELAY) {
//         SDL_Delay(FRAME_DELAY - frameTime);
//     }
    
//     lastFrameTime = SDL_GetTicks();
//     return fps;
// }

// void EndFrame(uint8_t flags) {
//     SDL_RenderPresent(renderer);
// }

// void waitForVBlank() {
//     // No-op in WASM
// }

// void Fade(unsigned int color) {
//     SDL_SetRenderDrawColor(
//         renderer,
//         (color >> 24) & 0xFF,
//         (color >> 16) & 0xFF,
//         (color >> 8) & 0xFF,
//         color & 0xFF
//     );
//     SDL_Rect fullScreen = {0, 0, screenWidth, screenHeight};
//     SDL_RenderFillRect(renderer, &fullScreen);
// }

// RenderTarget* CreateScreenTarget(Screen screen, ScreenSide side) {
//     // In WASM we use a single renderer
//     if (!renderer) {
//         printf("Warning: Renderer not initialized when creating screen target\n");
//     }
//     return renderer;
// }

// void TargetClear(RenderTarget* target, uint32_t color) {
//     SDL_SetRenderDrawColor(
//         renderer,
//         (color >> 24) & 0xFF,
//         (color >> 16) & 0xFF,
//         (color >> 8) & 0xFF,
//         color & 0xFF
//     );
//     SDL_RenderClear(renderer);
// }

// bool FrameBegin(uint8_t flags) {
//     SDL_RenderClear(renderer);
//     return true;
// }

// void SceneBegin(RenderTarget* target) {
//     // No-op in SDL, we just use the current renderer
// }

// void SpriteFromSheet(Sprite* sprite, SpriteSheet sheet, size_t index) {
//     sprite->image = sheet;
    
//     // Set default parameters
//     sprite->params.pos.x = 0;
//     sprite->params.pos.y = 0;
//     sprite->params.pos.w = 32;
//     sprite->params.pos.h = 32;
//     sprite->params.depth = 0;
//     sprite->params.angle = 0;
// }

// void SpriteSetCenter(Sprite* sprite, float x, float y) {
//     sprite->params.center.x = x;
//     sprite->params.center.y = y;
// }

// void SpriteFromImage(Sprite* sprite, Image image) {
//     sprite->image = image;
// }

// bool DrawRectangle(float x, float y, float z, float w, float h,
//                   uint32_t clr0, uint32_t clr1, uint32_t clr2, uint32_t clr3) {
//     // Simplified version with single color
//     SDL_SetRenderDrawColor(
//         renderer,
//         (clr0 >> 24) & 0xFF,
//         (clr0 >> 16) & 0xFF,
//         (clr0 >> 8) & 0xFF,
//         clr0 & 0xFF
//     );
//     SDL_Rect rect = {(int)x, (int)y, (int)w, (int)h};
//     SDL_RenderFillRect(renderer, &rect);
//     return true;
// }

// bool DrawRectSolid(float x, float y, float z, float w, float h, uint32_t clr) {
//     return DrawRectangle(x, y, z, w, h, clr, clr, clr, clr);
// }

// bool DrawSprite(const Sprite* sprite) {
//     if (sprite && sprite->image) {
//         SDL_Texture* tex = (SDL_Texture*)sprite->image;
//         SDL_Rect destRect = {
//             (int)sprite->params.pos.x,
//             (int)sprite->params.pos.y,
//             (int)sprite->params.pos.w,
//             (int)sprite->params.pos.h
//         };
        
//         // For simplicity, we use a default source rectangle
//         SDL_Rect srcRect = {0, 0, 32, 32};
        
//         SDL_RenderCopyEx(
//             renderer,
//             tex,
//             &srcRect,
//             &destRect,
//             sprite->params.angle,
//             NULL,
//             SDL_FLIP_NONE
//         );
//         return true;
//     }
//     return false;
// }

// void DrawText(const Text* text, uint32_t flags, float x, float y, float z, float scaleX, float scaleY) {
//     // We would need SDL_ttf for proper text rendering
//     // This is a simplified version
// }

// const char* TextFontParse(Text* text, Font font, TextBuf buf, const char* str) {
//     if (text) {
//         text->buf = buf;
//         text->font = font;
//     }
//     return str;
// }

// TextBuf TextBufNew(size_t maxGlyphs) {
//     return (TextBuf)1; // Dummy value
// }

// void TextBufDelete(TextBuf buf) {
//     // No-op
// }

// Font FontLoad(const char* filename) {
//     return (Font)1; // Dummy value
// }

// void TextOptimize(const Text* text) {
//     // No-op
// }

// void FontFree(Font font) {
//     // No-op
// }

// // Audio functions
// void PlayChunkOnChannel(uint8_t channel, Mix_Chunk* chunk, int loops) {
//     if (chunk) {
//         Mix_PlayChannel(channel, chunk, loops);
//     }
// }

// void PlayMusic(Mix_Music* music, int loops) {
//     if (music) {
//         Mix_PlayMusic(music, loops);
//     }
// }

// void HookMusicFinished(MusicFunction func) {
//     Mix_HookMusicFinished(func);
// }

// void PauseMusic() {
//     Mix_PauseMusic();
// }

// void ResumeMusic() {
//     Mix_ResumeMusic();
// }

// void FadeOutMusic(int ms) {
//     Mix_FadeOutMusic(ms);
// }

// void FadeInMusic(Mix_Music* music, int loops, int ms) {
//     if (music) {
//         Mix_FadeInMusic(music, loops, ms);
//     }
// }

// // Thread functions (simplified for WASM)
// Thread CreateThread(ThreadFunction entrypoint, void* arg, size_t stack_size, int prio, int core_id, bool detached) {
//     // Execute synchronously since WASM doesn't support true threading
//     entrypoint(arg);
//     return nullptr;
// }

// void SleepThread(int64_t ns) {
//     // No-op in WASM
// }

// Result GetThreadPriority(int32_t *out, Handle handle) {
//     if (out) {
//         *out = 0;
//     }
//     return 0;
// }

// unsigned int getCurrentThreadHandle() {
//     return 0;
// }

// Result JoinThread(Thread thread, uint64_t timeout_ns) {
//     return 0;
// }

// void FreeThread(Thread thread) {
//     // No-op
// }

// // Initialization functions
// Result InitROMFS() {
//     return 0; // No-op in WASM
// }

// Result InitAudio() {
//     if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
//         return -1;
//     }

//     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) == -1) {
//         return -2;
//     }

//     // Load audio files from preloaded assets
//     songs[0] = Mix_LoadMUS("/assets/sound/music/background_loop.mp3");
//     songs[1] = Mix_LoadMUS("/assets/sound/music/background_2.mp3");
//     songs[2] = Mix_LoadMUS("/assets/sound/music/background_3.mp3");
//     songs[3] = Mix_LoadMUS("/assets/sound/music/background_4.mp3");
//     songs[4] = Mix_LoadMUS("/assets/sound/music/background_5.mp3");
//     songs[5] = Mix_LoadMUS("/assets/sound/music/background_6.mp3");
//     songs[6] = Mix_LoadMUS("/assets/sound/music/background_7.mp3");

//     if (!songs[0]) {
//         printf("Music could not be loaded.\n");
//         return -5;
//     }

//     laser = Mix_LoadWAV("/assets/sound/laser_shot.wav");
//     heavy_bullet = Mix_LoadWAV("/assets/sound/heavy_bullet.wav");
//     hit = Mix_LoadWAV("/assets/sound/8-bit-hit-7_NWM.wav");
//     player_hit = Mix_LoadWAV("/assets/sound/player_hit.wav");
//     game_over = Mix_LoadWAV("/assets/sound/game_over.wav");
//     low_health = Mix_LoadWAV("/assets/sound/low_health.wav");

//     move_up_menu = Mix_LoadWAV("/assets/sound/move_up.wav");
//     move_down_menu = Mix_LoadWAV("/assets/sound/move_down.wav");
//     confirm_select = Mix_LoadWAV("/assets/sound/confirm_selection.wav");
//     click = Mix_LoadWAV("/assets/sound/click.wav");

//     return 0;
// }

// Result InitGraphics() {
//     if (sdlInitialized) {
//         return 0; // Already initialized
//     }

//     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
//         printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
//         return -1;
//     }

//     // In Emscripten, we use the canvas that's already set in the HTML
//     #ifdef __EMSCRIPTEN__
//         // Let Emscripten create the canvas for us
//         SDL_CreateWindowAndRenderer(screenWidth, screenHeight, SDL_WINDOW_SHOWN, &window, &renderer);
//     #else
//         // For native builds
//         window = SDL_CreateWindow("MilkySpace - WASM Port", 
//                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
//                                   screenWidth, screenHeight, 
//                                   SDL_WINDOW_SHOWN);
//         renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//     #endif

//     if (!window || !renderer) {
//         printf("Window or renderer could not be created! SDL Error: %s\n", SDL_GetError());
//         return -2;
//     }

//     SDL_SetWindowTitle(window, "MilkySpace - WASM Port");
//     SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

//     // Create a simple colored texture for sprite sheet
//     texture = SDL_CreateTexture(
//         renderer,
//         SDL_PIXELFORMAT_RGBA8888,
//         SDL_TEXTUREACCESS_TARGET,
//         128,
//         128
//     );

//     if (!texture) {
//         printf("Could not create texture! SDL Error: %s\n", SDL_GetError());
//         return -3;
//     }

//     // Fill the texture with a color to make it visible
//     SDL_SetRenderTarget(renderer, texture);
//     SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Magenta
//     SDL_RenderClear(renderer);
//     SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan
//     SDL_Rect rect = {32, 32, 64, 64};
//     SDL_RenderFillRect(renderer, &rect);
//     SDL_SetRenderTarget(renderer, NULL);

//     spriteSheet = texture;
    
//     startTime = std::chrono::high_resolution_clock::now();
//     lastFrameTime = SDL_GetTicks();
//     sdlInitialized = true;
    
//     return 0;
// }

// Result InitTextScene() {
//     // Simplified for WASM
//     return 0;
// }

// Result GetSystemLanguage(uint8_t *language) {
//     if (language) {
//         *language = 1; // English by default
//     }
//     return 0;
// }

// void HandleBadResult(Result result) {
//     printf("Scene initialization failed with error code: %d\n", result);
// }

// void ExitGraphics() {
//     if (texture) {
//         SDL_DestroyTexture(texture);
//         texture = nullptr;
//     }
    
//     if (renderer) {
//         SDL_DestroyRenderer(renderer);
//         renderer = nullptr;
//     }
    
//     if (window) {
//         SDL_DestroyWindow(window);
//         window = nullptr;
//     }
    
//     SDL_Quit();
// }

// void ExitTextScene() {
//     // No-op
// }

// bool ShouldNotCloseWindow() {
//     // Always true in browser context unless explicitly closed
//     return true;
// }

// void ScanInput() {
//     SDL_PumpEvents();
// }

// int32_t GetKeysDown() {
//     int32_t keys = 0;
//     const Uint8 *state = SDL_GetKeyboardState(NULL);
    
//     for (const auto &pair : keyMap) {
//         if (state[pair.first]) {
//             keys |= pair.second;
//         }
//     }
    
//     return keys;
// }

// int32_t GetKeysHeld() {
//     return GetKeysDown(); // Same implementation for simplicity
// }

// } // extern "C"

// // WASM-specific entry points
// extern "C" {
//     // Function to resize the canvas
//     EMSCRIPTEN_KEEPALIVE
//     void resizeCanvas(int width, int height) {
//         screenWidth = width;
//         screenHeight = height;
        
//         if (renderer) {
//             SDL_RenderSetLogicalSize(renderer, width, height);
//         }
//     }
    
//     // Function to check if game is ready
//     EMSCRIPTEN_KEEPALIVE
//     int isGameReady() {
//         return sdlInitialized ? 1 : 0;
//     }
    
//     // Debug function
//     EMSCRIPTEN_KEEPALIVE
//     void printDebugInfo() {
//         if (renderer) {
//             SDL_RendererInfo info;
//             SDL_GetRendererInfo(renderer, &info);
//             printf("Renderer name: %s\n", info.name);
//             printf("Renderer flags: %u\n", info.flags);
//             printf("Texture formats count: %u\n", info.num_texture_formats);
//             printf("Max texture width: %d\n", info.max_texture_width);
//             printf("Max texture height: %d\n", info.max_texture_height);
//         } else {
//             printf("Renderer not initialized\n");
//         }
        
//         printf("SDL initialized: %s\n", sdlInitialized ? "true" : "false");
//         printf("Screen dimensions: %dx%d\n", screenWidth, screenHeight);
//     }
// }

// #endif // __WASM__