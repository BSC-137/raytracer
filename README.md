# Ray Tracer in C++

A simple CPU-based ray tracer written from scratch in modern C++.  
This project is part of my personal journey to learn graphics programming, deepen my C++ skills, and build a foundation for future work in simulation, rendering, and visual systems.

## 🚀 Features

- 3D vector math via `Vec3` class
- Rays cast through every pixel
- Background gradient using ray direction
- Sphere rendering via ray-sphere intersection
- RGB color output in the PPM image format

## 🔧 Build & Run

Make sure you have `g++` installed.

```bash
g++ -std=c++17 raytracer.cpp -o raytracer
./raytracer
