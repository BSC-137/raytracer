# Ray Tracer in C++

A simple CPU-based ray tracer written from scratch in modern C++.  
This is a personal project to teach myself C++ and graphics programming from the ground up, without relying on prebuilt engines or libraries.  
My goal is to eventually build more complex rendering systems and simulation environments.

---

## 🚀 Features

- 📐 Vector math using a custom `Vec3` class
- 🌤️ Sky gradient using ray direction for background
- 🎯 Ray-sphere intersection with shading
- 💡 Lambertian-style lighting using surface normals
- 🟣 Scene with multiple spheres (including a ground plane)
- 🧱 Abstract `Hittable` interface and `HittableList` support
- 🎨 Color output in PPM image format (viewable via GIMP, IrfanView, etc.)

---

## 📁 Project Structure

| File             | Description                                   |
|------------------|-----------------------------------------------|
| `vec3.hpp`       | 3D vector math library                        |
| `ray.hpp`        | Ray definition and direction logic            |
| `sphere.hpp`     | `Sphere` class implementing the `Hittable` interface |
| `hittable.hpp`   | Abstract class defining `hit()` for ray intersections |
| `hittable_list.hpp` | Aggregator for multiple `Hittable` objects   |
| `raytracer.cpp`  | Main rendering loop + camera setup + image output |

---

## 🧪 Build & Run

Make sure you have `g++` installed.

```bash
g++ -std=c++17 raytracer.cpp -o raytracer
./raytracer
