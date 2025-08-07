# Ray Tracer in C++

A CPU-based ray tracer written from scratch in modern C++.  
This is a personal learning project to deeply understand computer graphics and C++ through hands-on implementation — no external rendering libraries, just math and pixels.

My goal is to build the fundamentals from scratch and evolve toward more complex rendering systems, simulations, and eventually a full software renderer or game engine.

---

## 🚀 Features So Far

- 📐 Custom `Vec3` vector math class
- 🎯 Ray-sphere intersection logic
- 🌤️ Sky gradient background based on ray direction
- 💡 Lambertian-style shading with light direction + surface normals
- 🔁 Anti-aliasing with stochastic supersampling
- 🟣 Scene support for multiple spheres, including a ground plane
- 🧱 Abstract `Hittable` interface with polymorphism
- 🗂️ `HittableList` to contain and manage many objects
- 📸 Camera abstraction for organized ray generation
- 🎨 RGB output in [PPM image format](https://en.wikipedia.org/wiki/Netpbm), viewable in GIMP, IrfanView, etc.

---

## 📁 Project Structure

| File                  | Description |
|-----------------------|-------------|
| `vec3.hpp`            | 3D vector library (arithmetic, normalization, dot product, etc.) |
| `ray.hpp`             | `Ray` class with origin and direction |
| `sphere.hpp`          | Sphere object that implements the `Hittable` interface |
| `hittable.hpp`        | Abstract base class for all scene objects (`hit()` method) |
| `hittable_list.hpp`   | List of `Hittable` objects forming the full scene |
| `camera.hpp`          | Simple camera class to generate rays based on screen coords |
| `raytracer.cpp`       | Main render loop and scene configuration (image is written here) |

---

## 🖼️ Sample Output

After running the program, a file `image.ppm` will be created in your current directory.

You can open it using:
- 🖼️ GIMP
- 🖼️ IrfanView
- 📸 ImageMagick (`convert image.ppm image.png`)
- 🧪 Online PPM viewers

---

## 🔧 Build & Run

Make sure you have `g++` installed.

```bash
g++ -std=c++17 raytracer.cpp -o raytracer
./raytracer
