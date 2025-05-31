import React, { useEffect, useRef, useState } from "react";

interface WasmGameProps {
  width?: number;
  height?: number;
}

declare global {
  interface Window {
    MilkySpace: any;
    Module?: any;
  }
}

const WasmGame: React.FC<WasmGameProps> = ({ width = 800, height = 600 }) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [isLoaded, setIsLoaded] = useState(false);
  const [isInitializing, setIsInitializing] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [frameCount, setFrameCount] = useState(0);
  const [fps, setFps] = useState(0);
  const moduleRef = useRef<any>(null);
  const animationFrameRef = useRef<number | null>(null);
  const lastFrameTimeRef = useRef<number>(performance.now());
  const frameCounterRef = useRef<number>(0);
  const fpsIntervalRef = useRef<ReturnType<typeof setInterval> | null>(null);

  // Function to update debug info
  const updateDebugInfo = () => {
    if (!moduleRef.current || !isLoaded) return;

    try {
      const newFrameCount = moduleRef.current.ccall(
        "getFrameCount",
        "number",
        [],
        [],
      );
      const deltaTime = moduleRef.current.ccall(
        "getDeltaTime",
        "float",
        [],
        [],
      );
      const isGameInit = moduleRef.current.ccall(
        "isGameInitialized",
        "boolean",
        [],
        [],
      );

      setFrameCount(newFrameCount);

      // Calculate FPS
      const now = performance.now();
      frameCounterRef.current++;

      if (now - lastFrameTimeRef.current >= 1000) {
        setFps(
          Math.round(
            (frameCounterRef.current * 1000) / (now - lastFrameTimeRef.current),
          ),
        );
        lastFrameTimeRef.current = now;
        frameCounterRef.current = 0;
      }

      // Debug print
      if (newFrameCount % 300 === 0) {
        moduleRef.current.ccall("printDebugInfo", null, [], []);
        console.log(
          `Game state: initialized=${isGameInit}, frames=${newFrameCount}, delta=${deltaTime.toFixed(4)}`,
        );
      }

      animationFrameRef.current = requestAnimationFrame(updateDebugInfo);
    } catch (err) {
      console.error("Error updating debug info:", err);
    }
  };

  useEffect(() => {
    async function loadWasmModule() {
      try {
        if (!canvasRef.current) {
          throw new Error("Canvas element not available");
        }

        setIsInitializing(true);
        console.log("Canvas ready:", canvasRef.current);

        // Import the WASM module
        console.log("Loading WASM module...");
        try {
          // Try to load the module directly via script tag
          if (typeof window.MilkySpace === 'function') {
            console.log("Using global MilkySpace object");
            moduleRef.current = await window.MilkySpace({
              canvas: canvasRef.current,
              print: (text: string) => console.log("[WASM]", text),
              printErr: (text: string) => console.error("[WASM Error]", text),
          });
          } else {
            // Fallback to dynamic import
            console.log("Global MilkySpace not found, trying dynamic import");
            // Create a script element to load the module
            const script = document.createElement('script');
            script.src = "/src/MilkySpace.js";

            await new Promise<void>((resolve, reject) => {
              script.onload = () => {
                console.log("Script loaded successfully");
                resolve();
              };
              script.onerror = () => {
                reject(new Error("Failed to load MilkySpace.js"));
              };
              document.body.appendChild(script);
            });

            if (typeof window.MilkySpace !== 'function') {
              throw new Error("MilkySpace is not available after loading the script");
            }

            moduleRef.current = await window.MilkySpace({
              canvas: canvasRef.current,
              print: (text: string) => console.log("[WASM]", text),
              printErr: (text: string) => console.error("[WASM Error]", text),
            });
          }
          console.log("Module initialized:", moduleRef.current);
        } catch (loadError) {
          console.error("Error loading module:", loadError);
          setError(`Failed to load WASM module: ${loadError instanceof Error ? loadError.message : String(loadError)}`);
          setIsInitializing(false);
          return;
        }
          onRuntimeInitialized: () => {
            console.log("MilkySpace runtime initialized");

            // Initialize the game
            try {
              console.log("Available methods:", Object.keys(moduleRef.current).filter(key => typeof moduleRef.current[key] === 'function'));

              // Try to initialize the game with the correct function name
              // The module is already initialized by Emscripten's main function
              // We don't need to call initGame as the C++ init_game function is already called in main()
              // Just verify we have access to the necessary functions
              let result = 0; // Assume success
              
              // Check that the module exports the functions we need
              const requiredFunctions = [
                '_handle_key_down', 
                '_handle_key_up', 
                '_reset_game', 
                '_get_score', 
                '_get_level',
                '_get_health',
                '_is_game_over'
              ];
              
              const missingFunctions = requiredFunctions.filter(
                fn => typeof moduleRef.current[fn] !== 'function'
              );
              
              if (missingFunctions.length > 0) {
                console.error("Missing required functions:", missingFunctions);
                result = -1;
              } else {
                console.log("All required functions are available");
              }

              if (result === 0) {
                console.log("Game initialized successfully");
                setIsLoaded(true);
                setIsInitializing(false);

                // Start the debug info updater
                animationFrameRef.current =
                  requestAnimationFrame(updateDebugInfo);

                // Handle canvas resize
                const handleResize = () => {
                  if (canvasRef.current && moduleRef.current) {
                    const canvas = canvasRef.current;
                    const container = canvas.parentElement;

                    if (container) {
                      const parentWidth = container.clientWidth;
                      const parentHeight = container.clientHeight;

                      // Maintain aspect ratio (4:3)
                      const aspectRatio = 4 / 3;
                      let newWidth, newHeight;

                      if (parentWidth / parentHeight > aspectRatio) {
                        // Container is wider than needed
                        newHeight = parentHeight;
                        newWidth = newHeight * aspectRatio;
                      } else {
                        // Container is taller than needed
                        newWidth = parentWidth;
                        newHeight = newWidth / aspectRatio;
                      }

                      canvas.width = newWidth;
                      canvas.height = newHeight;

                      // Center the canvas
                      canvas.style.marginLeft = `${(parentWidth - newWidth) / 2}px`;
                    } else {
                      canvas.width = width;
                      canvas.height = height;
                    }

                    moduleRef.current.ccall(
                      "resizeCanvas",
                      null,
                      ["number", "number"],
                      [canvas.width, canvas.height],
                    );
                  }
                };

                window.addEventListener("resize", handleResize);
                handleResize(); // Initial resize
              } else {
                setError(`Game initialization failed with code: ${result}`);
                setIsInitializing(false);
              }
            } catch (initErr) {
              console.error("Error during game initialization:", initErr);
              setError(
                `Initialization error: ${initErr instanceof Error ? initErr.message : "Unknown error"}`,
              );
              setIsInitializing(false);
            }
              });
            } else {
              // Fallback to dynamic import
              console.log("Global MilkySpace not found, trying dynamic import");
              // Create a script element to load the module
              const script = document.createElement('script');
              script.src = "/src/MilkySpace.js";

              await new Promise<void>((resolve, reject) => {
                script.onload = () => {
                  console.log("Script loaded successfully");
                  resolve();
                };
                script.onerror = () => {
                  reject(new Error("Failed to load MilkySpace.js"));
                };
                document.body.appendChild(script);
              });

              if (typeof window.MilkySpace !== 'function') {
                throw new Error("MilkySpace is not available after loading the script");
              }

              moduleRef.current = await window.MilkySpace({
                canvas: canvasRef.current,
                print: (text: string) => console.log("[WASM]", text),
                printErr: (text: string) => console.error("[WASM Error]", text),
                onRuntimeInitialized: () => {
                  // Same initialization code as above
                  console.log("MilkySpace runtime initialized");
                
                  try {
                    console.log("Available methods:", Object.keys(moduleRef.current).filter(key => typeof moduleRef.current[key] === 'function'));
                  
                    // Verify functions
                    let result = 0;
                  
                    const requiredFunctions = [
                      '_handle_key_down', 
                      '_handle_key_up', 
                      '_reset_game', 
                      '_get_score', 
                      '_get_level',
                      '_get_health',
                      '_is_game_over'
                    ];
                  
                    const missingFunctions = requiredFunctions.filter(
                      fn => typeof moduleRef.current[fn] !== 'function'
                    );
                  
                    if (missingFunctions.length > 0) {
                      console.error("Missing required functions:", missingFunctions);
                      result = -1;
                    } else {
                      console.log("All required functions are available");
                    }
                  
                    if (result === 0) {
                      console.log("Game initialized successfully");
                      setIsLoaded(true);
                      setIsInitializing(false);
                    
                      // Start the debug info updater
                      animationFrameRef.current = requestAnimationFrame(updateDebugInfo);
                    
                      // Handle canvas resize
                      const handleResize = () => {
                        if (canvasRef.current && moduleRef.current) {
                          const canvas = canvasRef.current;
                          const container = canvas.parentElement;
                        
                          if (container) {
                            const parentWidth = container.clientWidth;
                            const parentHeight = container.clientHeight;
                          
                            // Maintain aspect ratio (4:3)
                            const aspectRatio = 4 / 3;
                            let newWidth, newHeight;
                          
                            if (parentWidth / parentHeight > aspectRatio) {
                              // Container is wider than needed
                              newHeight = parentHeight;
                              newWidth = newHeight * aspectRatio;
                            } else {
                              // Container is taller than needed
                              newWidth = parentWidth;
                              newHeight = newWidth / aspectRatio;
                            }
                          
                            canvas.width = newWidth;
                            canvas.height = newHeight;
                          
                            // Center the canvas
                            canvas.style.marginLeft = `${(parentWidth - newWidth) / 2}px`;
                          } else {
                            canvas.width = width;
                            canvas.height = height;
                          }
                        
                          moduleRef.current.ccall(
                            "resizeCanvas",
                            null,
                            ["number", "number"],
                            [canvas.width, canvas.height],
                          );
                        }
                      };
                    
                      window.addEventListener("resize", handleResize);
                      handleResize(); // Initial resize
                    } else {
                      setError(`Game initialization failed with code: ${result}`);
                      setIsInitializing(false);
                    }
                  } catch (initErr) {
                    console.error("Error during game initialization:", initErr);
                    setError(
                      `Initialization error: ${initErr instanceof Error ? initErr.message : "Unknown error"}`
                    );
                    setIsInitializing(false);
                  }
                }
              });
            }
          
            // Store in window for debugging
            window.MilkySpace = moduleRef.current;
          } catch (err) {
            console.error("Failed to load WASM module:", err);
            setError(
              err instanceof Error ? err.message : "Unknown error loading the game",
            );
            setIsInitializing(false);
          }
    }

    loadWasmModule();

    // Update FPS every second
    fpsIntervalRef.current = setInterval(() => {
      if (isLoaded && moduleRef.current) {
        try {
          const isReady = moduleRef.current.ccall(
            "isGameReady",
            "number",
            [],
            [],
          );
          if (!isReady) {
            console.warn("Game not ready yet");
          }
        } catch (err) {
          console.error("Error checking game readiness:", err);
        }
      }
    }, 1000);

    // Cleanup when component unmounts
    return () => {
      if (animationFrameRef.current) {
        cancelAnimationFrame(animationFrameRef.current);
      }

      if (fpsIntervalRef.current) {
        clearInterval(fpsIntervalRef.current);
      }

      if (moduleRef.current) {
        try {
          moduleRef.current.ccall("shutdownGame", null, [], []);
          console.log("Game shutdown complete");
        } catch (err) {
          console.error("Error during game shutdown:", err);
        }
      }
    };
  }, [width, height]);

  return (
    <div
      className="wasm-game-container"
      style={{ width: "100%", height: "100%", position: "relative" }}
    >
      {error && (
        <div
          className="error-message"
          style={{
            color: "red",
            padding: "1rem",
            position: "absolute",
            top: 0,
            left: 0,
            backgroundColor: "rgba(0,0,0,0.7)",
            zIndex: 10,
          }}
        >
          Error: {error}
        </div>
      )}

      {isInitializing && !error && (
        <div
          className="loading-message"
          style={{
            padding: "1rem",
            position: "absolute",
            top: "50%",
            left: "50%",
            transform: "translate(-50%, -50%)",
            backgroundColor: "rgba(0,0,0,0.7)",
            color: "white",
            borderRadius: "5px",
            zIndex: 10,
          }}
        >
          Initializing game engine...
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
            padding: "0.5rem",
            fontSize: "12px",
            zIndex: 10,
          }}
        >
          FPS: {fps} | Frames: {frameCount}
        </div>
      )}

      <canvas
        ref={canvasRef}
        style={{
          display: "block",
          backgroundColor: "#000",
        }}
      />
    </div>
  );
};

export default WasmGame;
