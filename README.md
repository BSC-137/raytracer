# Ray Tracer in C++

A CPU-based ray tracer written from scratch in modern C++.  
This is a personal learning project to deeply understand computer graphics and C++ through hands-on implementation — no external rendering libraries, just math and pixels.

My goal is to build the fundamentals from scratch and evolve toward more advanced rendering (BVH, lights, textures) and eventually a full software renderer or game engine.

---

## 🚀 Features

- 📐 Custom `Vec3` vector math class
- 🎯 Ray–sphere intersection
- 🌤️ Sky gradient background
- 🔁 Anti-aliasing with stochastic supersampling
- 🧱 Abstract `Hittable` interface + `HittableList`
- 🟣 Multiple spheres, including a ground sphere
- 📸 Simple pinhole `Camera`
- 🧪 **Material system + recursive path tracing**
  - **Lambertian** (diffuse)
  - **Metal** with **fuzz** (rough reflections)
  - **Dielectric** (glass, refraction + Schlick)
- 🧭 Correct **normal orientation** with `front_face` and a small `t_min` epsilon to avoid self-intersections
- 🎨 PPM output (view in GIMP/IrfanView/ImageMagick)
- 🌈 Gamma correction (γ ≈ 2)

---

## 📁 Project Structure

| File                  | Description |
|-----------------------|-------------|
| `vec3.hpp`            | 3D vector math + RNG/sampling + reflect/refract |
| `ray.hpp`             | Ray class (`origin`, `direction`) |
| `hittable.hpp`        | `HitRecord`, base `Hittable`, normal orientation |
| `hittable_list.hpp`   | Container of hittables (smart pointers) |
| `material.hpp`        | Material interface (`scatter`) |
| `lambertian.hpp`      | Diffuse material |
| `metal.hpp`           | Reflective metal with fuzz |
| `dielectric.hpp`      | Glass/refraction with Schlick approximation |
| `sphere.hpp`          | Sphere implementing `Hittable` with a `Material` |
| `camera.hpp`          | Simple pinhole camera |
| `raytracer.cpp`       | Main render loop, scene setup |

---

## 🖼️ Output

Renders a PPM image file named `image.ppm` in the working directory.  
Open it with:
- GIMP / IrfanView
- ImageMagick: `convert image.ppm image.png`
- Any online PPM viewer

---

## 🔧 Build & Run

Make sure you have `g++` installed.

```bash
g++ -std=c++17 raytracer.cpp -o raytracer
./raytracer
