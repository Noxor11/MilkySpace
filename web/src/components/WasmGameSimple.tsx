import React, { useEffect, useRef, useState } from "react";

class Screen {
  width?: number;
  height?: number;
};

interface WasmGameProps {
  topScreen: Screen;
  bottomScreen: Screen;
}

declare global {
  interface Window {
    MilkySpace: any;
  }
  interface EmscriptenModule {
    imageCache: { [key: string]: HTMLImageElement };
  }
}

const WasmGameSimple: React.FC<WasmGameProps> = ({
  topScreen: { width: topScreenWidth = 400, height: topScreenHeight = 240 },
  bottomScreen: { width: bottomScreenWidth = 320, height: bottomScreenHeight = 240 }
}) => {
  const canvasTopScreenRef = useRef<HTMLCanvasElement>(null);
  const canvasBottomScreenRef = useRef<HTMLCanvasElement>(null);
  const [isLoaded, setIsLoaded] = useState(false);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [fps, setFps] = useState(0);
  const [frameCount, setFrameCount] = useState(0);
  const moduleRef = useRef<any>(null);
  const frameCounterRef = useRef<number>(0);
  const lastTimeRef = useRef<number>(performance.now());
  const animationFrameRef = useRef<number | null>(null);

  // Load and initialize the WebAssembly module
  useEffect(() => {
    if (!canvasTopScreenRef.current) return;
    if (!canvasBottomScreenRef.current) return;

    // Create module configuration
    const moduleConfig = {
      canvasTopScreen: canvasTopScreenRef.current,
      canvasBottomScreen: canvasBottomScreenRef.current,
      print: (text: string) => console.log("[WASM]", text),
      printErr: (text: string) => console.error("[WASM Error]", text),
      imageCache: {} as { [key: string]: HTMLImageElement },
      onRuntimeInitialized: () => {
        console.log("WASM runtime initialized");
        setIsLoaded(true);
        setIsLoading(false);

        // Set up event handlers
        document.addEventListener("keydown", handleKeyDown);
        document.addEventListener("keyup", handleKeyUp);

        // Start animation
        startGameLoop();
      },
    };

    // Function to handle script loading
    const loadScript = (url: string): Promise<void> => {
      return new Promise((resolve, reject) => {
        const script = document.createElement("script");
        script.src = url;
        script.async = true;
        script.onload = () => resolve();
        script.onerror = () =>
          reject(new Error(`Failed to load script: ${url}`));
        document.body.appendChild(script);
      });
    };

    // Load and initialize the WebAssembly module
    async function initWasm() {
      try {
        setIsLoading(true);

        // Load the script first
        await loadScript("../../src/MilkySpace.js");

        if (typeof window.MilkySpace !== "function") {
          throw new Error("MilkySpace function not available");
        }

        // Initialize the module
        moduleRef.current = await window.MilkySpace(moduleConfig);
        console.log("Module initialized:", moduleRef.current);
      } catch (err) {
        console.error("Failed to load WASM module:", err);
        setError(err instanceof Error ? err.message : "Unknown error");
        setIsLoading(false);
      }
    }

    initWasm();

    // Cleanup
    return () => {
      document.removeEventListener("keydown", handleKeyDown);
      document.removeEventListener("keyup", handleKeyUp);
      if (animationFrameRef.current) {
        cancelAnimationFrame(animationFrameRef.current);
      }
    };
  }, []);

  // Handle keyboard input
  const handleKeyDown = (event: KeyboardEvent) => {
    if (moduleRef.current && moduleRef.current._handle_key_down) {
      moduleRef.current._handle_key_down(event.keyCode);
    }
  };

  const handleKeyUp = (event: KeyboardEvent) => {
    if (moduleRef.current && moduleRef.current._handle_key_up) {
      moduleRef.current._handle_key_up(event.keyCode);
    }
  };

  // Game loop
  const startGameLoop = () => {
    const updateFrame = () => {
      // Calculate FPS
      const now = performance.now();
      frameCounterRef.current++;

      if (now - lastTimeRef.current >= 1000) {
        setFps(
          Math.round(
            (frameCounterRef.current * 1000) / (now - lastTimeRef.current),
          ),
        );
        frameCounterRef.current = 0;
        lastTimeRef.current = now;

        // Update debug info once per second
        if (moduleRef.current && moduleRef.current._get_score) {
          try {
            const score = moduleRef.current._get_score();
            setFrameCount(score); // Using score as a proxy for frame count
          } catch (e) {
            console.error("Error getting score:", e);
          }
        }
      }

      animationFrameRef.current = requestAnimationFrame(updateFrame);
    };

    animationFrameRef.current = requestAnimationFrame(updateFrame);
  };

  // Render component
  return (
    <div className="wasm-game-container" style={{ position: "relative", display: 'block' }}>
      {error && (
        <div
          className="error-message"
          style={{
            color: "red",
            padding: "10px",
            position: "absolute",
            top: 0,
            left: 0,
            backgroundColor: "rgba(0,0,0,0.7)",
          }}
        >
          Error: {error}
        </div>
      )}

      {isLoading && !error && (
        <div
          className="loading-message"
          style={{
            position: "absolute",
            top: "50%",
            left: "50%",
            transform: "translate(-50%, -50%)",
            padding: "10px",
            backgroundColor: "rgba(0,0,0,0.7)",
            color: "white",
          }}
        >
          Loading WebAssembly module...
        </div>
      )}

      {isLoaded && (
        <div
          className="debug-info"
          style={{
            position: "absolute",
            top: 0,
            right: 0,
            backgroundColor: "rgba(0,0,0,0.5)",
            color: "white",
            padding: "5px",
            fontSize: "12px",
          }}
        >
          FPS: {fps} | Score: {frameCount}
        </div>
      )}

      <canvas
        ref={canvasTopScreenRef}
        width={topScreenWidth}
        height={topScreenHeight}
        style={{ display: "block", backgroundColor: "#000", margin: "0 auto" }}
        id="canvasTopScreen"
      />

      <canvas
        ref={canvasBottomScreenRef}
        width={bottomScreenWidth}
        height={bottomScreenHeight}
        style={{ display: "block", backgroundColor: "#000", margin: "0 auto" }}
        id="canvasBottomScreen"
      />

      {isLoaded && (
        <div
          className="controls"
          style={{ marginTop: "10px", textAlign: "center" }}
        >
          <button onClick={() => moduleRef.current?._reset_game?.()}>
            Reset Game
          </button>
        </div>
      )}
    </div>
  );
};

export default WasmGameSimple;
