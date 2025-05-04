# 🌊 WaterShader

A real-time OpenGL demo rendering an animated ocean surface using custom GLSL shaders, raymarching, and atmospheric effects.

## ✨ Features

- Dynamic ocean surface using procedurally generated waves
- Simple raymarching for wave-surface intersection
- Fast atmosphere approximation for realistic sky color
- Written in modern C++17 with GLFW, GLAD, and GLSL

###
Original shader on [Shadertoy](https://www.shadertoy.com/view/MdXyzX)

## 🛠️ Building

### Requirements
- [CMake ≥ 3.10](https://cmake.org/)
- [GLFW](https://www.glfw.org/) headers and binaries
- Glad
- GLFW & Glad is assumed to be in the root directory
- C++17-compatible compiler (e.g., MSVC, clang, g++)

### 💻 Build (Visual Studio)

Open a terminal in the root directory and run:

```bash
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug    # or Release