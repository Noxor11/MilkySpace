#pragma once

#include <cstddef>
#include <cstdint>
#define BIT(n) (1U<<(n))

enum text_flags {
	AtBaseline       = BIT(0), ///< Matches the Y coordinate with the baseline of the font.
	WithColor        = BIT(1), ///< Draws text with color. Requires a u32 color value.
	AlignLeft        = 0 << 2, ///< Draws text aligned to the left. This is the default.
	AlignRight       = 1 << 2, ///< Draws text aligned to the right.
	AlignCenter      = 2 << 2, ///< Draws text centered.
	AlignJustified   = 3 << 2, ///< Draws text justified. When C2D_WordWrap is not specified, right edge is x + scaleX*text->width. Otherwise, right edge is x + the width specified for those values.
	AlignMask        = 3 << 2, ///< Bitmask for alignment values.
	WordWrap         = BIT(4), ///< Draws text with wrapping of full words before specified width. Requires a float value, passed after color if C2D_WithColor is specified.
};

enum frame_flags {
	FRAME_SYNCDRAW = BIT(0), // Perform C3D_FrameSync before checking the GPU status
	FRAME_NONBLOCK = BIT(1), // Return false instead of waiting if the GPU is busy
};

/// Screen IDs.
enum class Screen {
	GFX_TOP    = 0,    ///< Top screen
	GFX_BOTTOM = 1, ///< Bottom screen
};

enum class ScreenSide {
	GFX_LEFT  = 0, ///< Left eye framebuffer
	GFX_RIGHT = 1, ///< Right eye framebuffer
};


typedef int32_t Result;
typedef uint32_t Handle;

typedef void (*ThreadFunction)(void *); ///< Thread entrypoint function.
typedef void (*MusicFunction)(void); ///< Thread entrypoint function.

enum Key: unsigned int {
	A       = BIT(0),       ///< A
	B       = BIT(1),       ///< B
	SELECT  = BIT(2),       ///< Select
	START   = BIT(3),       ///< Start
	DRIGHT  = BIT(4),       ///< D-Pad Right
	DLEFT   = BIT(5),       ///< D-Pad Left
	DUP     = BIT(6),       ///< D-Pad Up
	DDOWN   = BIT(7),       ///< D-Pad Down
	R       = BIT(8),       ///< R
	L       = BIT(9),       ///< L
	X       = BIT(10),      ///< X
	Y       = BIT(11),      ///< Y
	ZL      = BIT(14),      ///< ZL (New 3DS only)
	ZR      = BIT(15),      ///< ZR (New 3DS only)
	TOUCH   = BIT(20),      ///< Touch (Not actually provided by HID)
	CSTICK_RIGHT = BIT(24), ///< C-Stick Right (New 3DS only)
	CSTICK_LEFT  = BIT(25), ///< C-Stick Left (New 3DS only)
	CSTICK_UP    = BIT(26), ///< C-Stick Up (New 3DS only)
	CSTICK_DOWN  = BIT(27), ///< C-Stick Down (New 3DS only)
	CPAD_RIGHT = BIT(28),   ///< Circle Pad Right
	CPAD_LEFT  = BIT(29),   ///< Circle Pad Left
	CPAD_UP    = BIT(30),   ///< Circle Pad Up
	CPAD_DOWN  = BIT(31),   ///< Circle Pad Down

	// Generic catch-all directions
	UP    = DUP    | CPAD_UP,    ///< D-Pad Up or Circle Pad Up
	DOWN  = DDOWN  | CPAD_DOWN,  ///< D-Pad Down or Circle Pad Down
	LEFT  = DLEFT  | CPAD_LEFT,  ///< D-Pad Left or Circle Pad Left
	RIGHT = DRIGHT | CPAD_RIGHT, ///< D-Pad Right or Circle Pad Right
};


#ifdef __3DS__

    #include <citro2d.h>
    #include <SDL_mixer.h>
    #include <SDL.h>
    #include <SDL_audio.h>

    typedef C2D_Sprite Sprite;
    typedef C2D_SpriteSheet SpriteSheet;
    typedef C2D_Image Image;
    typedef C3D_RenderTarget RenderTarget;

    typedef C2D_Text Text;
    typedef C2D_Font Font;
    typedef C2D_TextBuf TextBuf;

    typedef Thread Thread;
    typedef Mix_Chunk Mix_Chunk;

    const auto CLOCK_RATE = SYSCLOCK_ARM11;

    // input

#endif

#ifdef __WASM__

    struct DrawParams {
        struct
        {
            float x, y, w, h;
        } pos;

        struct
        {
            float x, y;
        } center;

        float depth;
        float angle;
    };

    struct Sprite {
        void* image;
        DrawParams params;
    };

    typedef void* SpriteSheet;
    typedef void* Image;

    struct alignas(8) RenderTarget {
        Screen screen;
        ScreenSide screenSide;
    };

    typedef void* Font;
    typedef void* TextBuf;

    struct Text {
        TextBuf buf;   ///< Buffer associated with the text.
        size_t      begin; ///< Reserved for internal use.
        size_t      end;   ///< Reserved for internal use.
        float       width; ///< Width of the text in pixels, according to 1x scale metrics.
        uint32_t    lines; ///< Number of lines in the text.
        uint32_t    words; ///< Number of words in the text.
        Font    font;  ///< Font used to draw the text, or NULL for system font
    };

    typedef void* Thread;
    typedef void* Mix_Chunk;
    typedef void* Mix_Music;

    const auto CLOCK_RATE = 1; // this is not really correct

#endif

#ifdef __WASM__
extern "C" {
#endif
    void Fade(unsigned int color);
    uint32_t Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    uint32_t Color32f(float r, float g, float b, float a);
    float SetFramerate(float fps);
    void EndFrame(uint8_t flags);
    void waitForVBlank();

    RenderTarget* CreateScreenTarget(Screen, ScreenSide);
    void TargetClear(RenderTarget* target, uint32_t color);

    bool FrameBegin(uint8_t flags);

    void SceneBegin(RenderTarget* target);

    void SpriteFromSheet(Sprite* sprite, SpriteSheet sheet, size_t index);
    void SpriteSetCenter(Sprite* sprite, float x, float y);
    void SpriteFromImage(Sprite* sprite, Image image);

    bool DrawRectangle(
        float x, float y, float z, float w, float h,
        uint32_t clr0, uint32_t clr1, uint32_t clr2, uint32_t clr3);

    bool DrawRectSolid(float x, float y, float z, float w, float h, uint32_t clr);
    bool DrawSprite(const Sprite* sprite);

    // void DrawText(const Text* text, uint32_t flags, float x, float y, float z, float scaleX, float scaleY, ...);
    void DrawText(const Text* text, uint32_t flags, float x, float y, float z, float scaleX, float scaleY, uint32_t color = 0x000000);

    // ------------------------------------------------------------------------------

    const char* TextFontParse(Text* text, Font font, TextBuf buf, const char* str);
    TextBuf TextBufNew(size_t maxGlyphs);
    Font FontLoad(const char* filename);
    void TextOptimize(const Text* text);
    void TextBufDelete(TextBuf buf);
    void FontFree(Font font);

    uint64_t GetSystemTick();
// }

// namespace Audio {
    void PlayChunkOnChannel(uint8_t channel, Mix_Chunk* chunk, int loops);
    void PlayMusic(Mix_Music* music, int loops);
    void HookMusicFinished(MusicFunction func);
    void PauseMusic();
    void ResumeMusic();
    void FadeOutMusic(int ms);
    void FadeInMusic(Mix_Music* music, int loops, int ms);
//}


// namespace Thread {

    Thread CreateThread(ThreadFunction entrypoint, void* arg, size_t stack_size, int prio, int core_id, bool detached);
    void SleepThread(int64_t ns);
    Result GetThreadPriority(int32_t *out, Handle handle);
    unsigned int getCurrentThreadHandle();

    Result JoinThread(Thread thread, uint64_t timeout_ns);
    void FreeThread(Thread thread);
// }

// namespace System {

    Result InitROMFS();
    Result InitAudio();
    Result InitGraphics();
    Result InitTextScene();

    Result GetSystemLanguage(uint8_t* language);

    void HandleBadResult(Result result);

    void ExitGraphics();
    void ExitTextScene();

    bool ShouldNotCloseWindow();
//}

// namespace Input {
    void ScanInput();
    int32_t GetKeysDown();
    int32_t GetKeysHeld();
//}

#ifdef __WASM__
} // extern "C"
#endif
