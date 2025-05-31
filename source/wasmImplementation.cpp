#include "gameSpecification.h"
#include <cstdio>
#include <iostream>

#ifdef __WASM__

#include "textScene.h"

#include <cstdint>
#include <emscripten/em_js.h>
#include <emscripten/em_macros.h>
#include <emscripten/bind.h>


    extern std::array<Text, TEXT_LINES> g_staticText;
    extern std::array<Text, 10> scoreNumberText;
    TextBuf g_staticBuf;

    RenderTarget* currentTarget;

    EMSCRIPTEN_BINDINGS(milky_space_module) {
        emscripten::enum_<Screen>("Screen")
            .value("GFX_TOP", Screen::GFX_TOP)
            .value("GFX_BOTTOM", Screen::GFX_BOTTOM);
    }

    EM_JS(void, js_draw_rect, (int x, int y, int width, int height, int color, Screen screen_param), {
        // GFX_TOP is 0, GFX_BOTTOM is 1.
        // If screen is 0 (GFX_TOP), use canvasTopScreen. Otherwise (GFX_BOTTOM), use canvasBottomScreen.
        const canvas = screen_param === 0 ? Module.canvasTopScreen : Module.canvasBottomScreen;
        console.log("[js_draw_rect] Drawing on:", screen_param === 0 ? "TOP" : "BOTTOM", "canvas:", canvas.id);
        const ctx = canvas.getContext('2d');
        if (!ctx) {
            console.error("[js_draw_rect] Canvas context not found for screen_param:", screen_param);
            return;
        }

        const r = (color & 0xFF);
        const g = ((color >> 8) & 0xFF);
        const b = ((color >> 16) & 0xFF);
        const a = ((color >> 24) & 0xFF) / 255.0;
        ctx.fillStyle = 'rgba('+ r + ',' + g + ',' + b + ',' + a + ')';
        ctx.fillRect(x, y, width, height);
    });

    EM_JS(void, js_draw_text, (const char* text_ptr, int x, int y, int size, int color, Screen screen_param), {
        // GFX_TOP is 0, GFX_BOTTOM is 1.
        // If screen is 0 (GFX_TOP), use canvasTopScreen. Otherwise (GFX_BOTTOM), use canvasBottomScreen.
        const canvas = screen_param === 0 ? Module.canvasTopScreen : Module.canvasBottomScreen;
        console.log("[js_draw_text] Drawing on:", screen_param === 0 ? "TOP" : "BOTTOM", "canvas:", canvas.id, "text:", UTF8ToString(text_ptr));
        const ctx = canvas.getContext('2d');
        if (!ctx) {
            console.error("[js_draw_text] Canvas context not found for screen_param:", screen_param);
            return;
        }

        ctx.font = size + 'px Arial';
        const r = (color & 0xFF);
        const g = ((color >> 8) & 0xFF);
        const b = ((color >> 16) & 0xFF);
        const a = ((color >> 24) & 0xFF) / 255.0;
        ctx.fillStyle = 'rgba('+ r + ',' + g + ',' + b + ',' + a + ')';
        ctx.fillText(UTF8ToString(text_ptr), x, y);
    });

    EM_JS(void, js_draw_sprite, (float x, float y, float w, float h, float angle_rad, float center_x, float center_y, const char* image_path, Screen screen_param), {
        // GFX_TOP is 0, GFX_BOTTOM is 1.
        // If screen is 0 (GFX_TOP), use canvasTopScreen. Otherwise (GFX_BOTTOM), use canvasBottomScreen.
        const canvas = screen_param === 0 ? Module.canvasTopScreen : Module.canvasBottomScreen;
        const ctx = canvas.getContext('2d');
        if (!ctx) {
            console.error("[js_draw_sprite] Canvas context not found for screen_param:", screen_param);
            return;
        }

        // Get or create image
        let img = Module.imageCache[UTF8ToString(image_path)];
        if (!img) {
            img = new Image();
            img.src = UTF8ToString(image_path);
            Module.imageCache[img.src] = img;
        }

        // Draw the image if it's loaded
        if (img.complete) {
            ctx.save();
            ctx.translate(x + center_x, y + center_y);
            ctx.rotate(angle_rad);
            ctx.drawImage(img, -center_x, -center_y, w, h);
            ctx.restore();
        } else {
            // Add load event listener for first load
            img.addEventListener('load', function() {
                ctx.save();
                ctx.translate(x + center_x, y + center_y);
                ctx.rotate(angle_rad);
                ctx.drawImage(img, -center_x, -center_y, w, h);
                ctx.restore();
            }, { once: true });
        }
    });


    /** @brief Clamps a value between bounds
    *  @param[in] x The value to clamp
    *  @param[in] min The lower bound
    *  @param[in] max The upper bound
    *  @returns The clamped value
    */
    constexpr float Clamp(float x, float min, float max)
    {
        return x <= min ? min : x >= max ? max : x;
    }

    /** @brief Converts a float to u8
    *  @param[in] x Input value (0.0~1.0)
    *  @returns Output value (0~255)
    */
    constexpr uint8_t FloatToU8(float x)
    {
        return (uint8_t)(255.0f*Clamp(x, 0.0f, 1.0f)+0.5f);
    }

    void Fade(unsigned int color) {}
    uint32_t Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return r | (g << (uint32_t)8) | (b << (uint32_t)16) | (a << (uint32_t)24);
    }

    uint32_t Color32f(float r, float g, float b, float a) {
        return Color32(FloatToU8(r),FloatToU8(g),FloatToU8(b),FloatToU8(a));
    }

    float SetFramerate(float fps) {return 0;} // unused
    void EndFrame(uint8_t flags) {}
    void waitForVBlank() {}

    RenderTarget* CreateScreenTarget(Screen screen, ScreenSide screenSide) {
        RenderTarget* target = new RenderTarget();
        target->screen = screen;
        target->screenSide = screenSide;
        std::cout << "Creating screen target - Screen: " << (screen == Screen::GFX_TOP ? "TOP" : "BOTTOM")
                  << ", Address: " << target
                  << ", Screen value: " << static_cast<int>(screen) << std::endl;
        return target;
    }

    EM_JS(void, js_clear_canvas, (Screen screen_param, int color_val), {
        // console.log("[js_clear_canvas] Received screen_param:", screen_param, "(type:", typeof screen_param, ")");
        const ctx = screen_param === 0 ? Module.canvasTopScreen.getContext('2d') : Module.canvasBottomScreen.getContext('2d');
        if (!ctx) {
            console.error('[js_clear_canvas] Canvas context not found for screen_param:', screen_param);
            return;
        }
        const r = (color_val & 0xFF);
        const g = ((color_val >> 8) & 0xFF);
        const b = ((color_val >> 16) & 0xFF);
        const a = ((color_val >> 24) & 0xFF) / 255.0;
        ctx.fillStyle = 'rgba('+ r + ',' + g + ',' + b + ',' + a + ')';
        ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
    });

    void TargetClear(RenderTarget* target, uint32_t color_val) {
        if (!target) {
            std::cerr << "TargetClear: target is null!" << std::endl;
            return;
        }
        std::cout << "TargetClear: Clearing C++ screen " << &target->screen << " with color " << color_val << std::endl;
        js_clear_canvas(target->screen, color_val);
    }

    bool FrameBegin(uint8_t flags) {return true;}

    void SceneBegin(RenderTarget* target) {
        if (!target) {
            std::cerr << "SceneBegin: target is null!" << std::endl;
            return;
        }
        std::cout << "SceneBegin: Setting currentTarget to "
                  << (target->screen == Screen::GFX_TOP ? "TOP" : "BOTTOM")
                  << " screen (value: " << static_cast<int>(target->screen)
                  << ", address: " << target << ")" << std::endl;
        currentTarget = target;
    }


    void SpriteFromSheet(Sprite* sprite, SpriteSheet sheet, size_t index) {}
    void SpriteSetCenter(Sprite* sprite, float x, float y) { sprite->params.center.x = x; sprite->params.center.y = y;    }
    void SpriteFromImage(Sprite* sprite, Image image) {}

    bool DrawSprite(const Sprite* sprite) {
        if (!sprite) return false;
        if (!currentTarget) {
            std::cerr << "DrawSprite: currentTarget is NULL!" << std::endl;
            return false;
        }

        // Cast the void* image pointer to char* to get the image path
        const char* image_path = static_cast<const char*>(sprite->image);
        if (!image_path) {
            std::cerr << "DrawSprite: image path is NULL!" << std::endl;
            return false;
        }

        js_draw_sprite(sprite->params.pos.x, sprite->params.pos.y,
                      sprite->params.pos.w, sprite->params.pos.h,
                      sprite->params.angle,
                      sprite->params.center.x, sprite->params.center.y,
                      image_path,
                      currentTarget->screen);
        return true;
    }

    void DrawText(const Text* text, uint32_t flags, float x, float y, float z, float scaleX, float scaleY, uint32_t color) {
        if (!currentTarget) {
            std::cerr << "DrawText: currentTarget is NULL!" << std::endl;
            return;
        }
        // std::cout << (const char*)text->buf << " " << x << " " << y << " " << scaleX << std::endl;
        std::cout << "DrawText: Using C++ currentTarget->screen " << (int)currentTarget->screen << std::endl;
        const char* text_content = text && text->buf ? (const char*)text->buf : "null_text";
        js_draw_text(text_content, x, y, scaleX * 15, color, currentTarget->screen);
    }

    // ------------------------------------------------------------------------------

    const char* TextFontParse(Text* text, Font font, TextBuf buf, const char* str) {
        text->buf = (void*)str;
        return "uwu";
    }
    TextBuf TextBufNew(size_t maxGlyphs) {return (void*)"uwuuuuuw";}
    Font FontLoad(const char* filename) {return nullptr;}
    void TextOptimize(const Text* text) {}
    void TextBufDelete(TextBuf buf) {}
    void FontFree(Font font) {}

    uint64_t GetSystemTick() {return 1;}
// }

// namespace Audio {
    void PlayChunkOnChannel(uint8_t channel, Mix_Chunk* chunk, int loops) {}
    void PlayMusic(Mix_Music* music, int loops) {}
    void HookMusicFinished(MusicFunction func) {}
    void PauseMusic() {}
    void ResumeMusic() {}
    void FadeOutMusic(int ms) {}
    void FadeInMusic(Mix_Music* music, int loops, int ms) {}
//}


// namespace Thread {

    Thread CreateThread(ThreadFunction entrypoint, void* arg, size_t stack_size, int prio, int core_id, bool detached) {return 0;}
    void SleepThread(int64_t ns) {}
    Result GetThreadPriority(int32_t *out, Handle handle) {return 0;}
    unsigned int getCurrentThreadHandle() {return 0;}

    Result JoinThread(Thread thread, uint64_t timeout_ns) {return 0;}
    void FreeThread(Thread thread) {}
// }

// namespace System {

    Result InitROMFS() {return 0;}
    Result InitAudio() {return 0;}


    SpriteSheet SpriteSheetLoad(const char* filename) {
        return 0;
    }

    Result InitGraphics() {
        spriteSheet;

        SpriteSheetLoad("romfs:/gfx/sprites.t3x");
        return 0;
    }

    Result InitTextScene() {
        TextFontParse(&g_staticText[PLAY], 			nullptr, g_staticBuf, "play");
		TextFontParse(&g_staticText[SETTINGS_TXT], 	nullptr, g_staticBuf, "settings");
		TextFontParse(&g_staticText[NEXT_SONG_TXT], nullptr, g_staticBuf, "next_song");
		TextFontParse(&g_staticText[SCORE], 		nullptr, g_staticBuf, "score");
		TextFontParse(&g_staticText[TITLE], 		nullptr, g_staticBuf, "title");
        TextFontParse(&g_staticText[COLON], 		nullptr, g_staticBuf, "colon");
        TextFontParse(&g_staticText[TIME], 			nullptr, g_staticBuf, "time_survived");
        TextFontParse(&g_staticText[TRY_AGAIN], 	nullptr, g_staticBuf, "try_again");
        TextFontParse(&g_staticText[GO_BACK], 		nullptr, g_staticBuf, "go_back_menu");
        TextFontParse(&g_staticText[YOU_WON], 		nullptr, g_staticBuf, "you_won");
		TextFontParse(&g_staticText[YOU_LOST], 		nullptr, g_staticBuf, "you_lost");
        TextFontParse(&g_staticText[ROUND], 		nullptr, g_staticBuf, "round");
        TextFontParse(&g_staticText[NEW_HIGHSCORE], nullptr, g_staticBuf, "new_highscore");
        TextFontParse(&g_staticText[NEW_BEST_TIME], nullptr, g_staticBuf, "new_best_time");

		// Pause

		TextFontParse(&g_staticText[PAUSED], 		nullptr, g_staticBuf, "paused");
		TextFontParse(&g_staticText[RESUME], 		nullptr, g_staticBuf, "resume");
		TextFontParse(&g_staticText[EXIT], 			nullptr, g_staticBuf, "exit");

		//	Settings

		TextFontParse(&g_staticText[YES], 			nullptr, g_staticBuf, "yes");
        TextFontParse(&g_staticText[NO], 			nullptr, g_staticBuf, "no");

        TextFontParse(&g_staticText[INVERT_X],		nullptr, g_staticBuf, "invert_x");
        TextFontParse(&g_staticText[INVERT_Y], 		nullptr, g_staticBuf, "invert_y");

		//	Numbers for score display
		for (int i = 0; i < (int) scoreNumberText.size(); i++) {
			TextFontParse(&scoreNumberText[i], nullptr, g_staticBuf, std::to_string(i).c_str());
		}
		//	---------------------------------------------------------------------------

		// --------------Optimize text strings-------------
		for (Text txt: g_staticText) {
			TextOptimize(&txt);
		}


        return 0;
    }

    Result GetSystemLanguage(uint8_t* language) {return 0;}

    void HandleBadResult(Result result) {
        puts("uhh there was an error or smth");
    }

    void ExitGraphics() {}
    void ExitTextScene() {}

    EMSCRIPTEN_KEEPALIVE
    bool ShouldNotCloseWindow() {return true;}
//}

// namespace Input {
    void ScanInput() {}
    int32_t GetKeysDown() {return 0;}
    int32_t GetKeysHeld() {return 0;}
//}

// Handle key down events


    EMSCRIPTEN_KEEPALIVE
    bool DrawRectangle(
        float x, float y, float z, float w, float h,
        uint32_t clr0, uint32_t clr1, uint32_t clr2, uint32_t clr3) {

        if (!currentTarget) {
            std::cerr << "DrawRectangle: currentTarget is NULL!" << std::endl;
            return false;
        }
        // std::cout << "DrawRectangle: Using C++ currentTarget->screen " << (int)currentTarget->screen << std::endl;
        js_draw_rect(x, y, w, h, clr0, currentTarget->screen);
        return true;
    }

    EMSCRIPTEN_KEEPALIVE
    bool DrawRectSolid(
        float x, float y, float z, float w, float h,
        uint32_t clr)
    {
        return DrawRectangle(x,y,z,w,h,clr,clr,clr,clr);
    }



#endif
