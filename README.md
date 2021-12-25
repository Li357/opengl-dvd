### A nostalgic bouncing logo

![demo](demo.gif)

This was built on macOS. Requirements:
- OpenGL 3 (using `-framework OpenGL` on macOS 15)
- glfw3 (installed with Homebrew with headers in `/opt/homebrew/include` and libraries in `/opt/homebrew/lib`)

You can also just compile and run directly, specifying where you've installed `glfw`:

    g++ -std=c++20 -o main.o -framework OpenGL -I include/ -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw main.cpp
    ./main.o