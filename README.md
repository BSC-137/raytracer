# Ray Tracer in C++

A CPU-based ray tracer written from scratch in modern C++.  
This is a personal learning project to gain a deep understanding of computer graphics and C++ through hands-on implementation — without relying on external rendering libraries.

The aim is to build a strong foundation in rendering algorithms, then progressively extend the system towards advanced features such as **BVH acceleration**, **realistic lighting**, and **physically based rendering**.

---

## ✨ Features Implemented

### 🧮 Math & Core
- **Custom `Vec3` math library**
  - 3D vector operations
  - Random sampling utilities
  - Reflection and refraction helpers

- **Ray–Object Intersection**
  - Sphere intersection
  - Axis-aligned rectangle primitives (`XYRect`, `YZRect`, `XZRect`)
  - Moving spheres for motion blur

---

### 📷 Camera System
- Configurable **field of view**
- **Depth of field** via aperture sampling
- **Motion blur** via shutter open/close times

---

### 🎨 Material System & Path Tracing
- Lambertian (diffuse)
- Metal with adjustable fuzziness
- Dielectric (glass) with refraction and Fresnel reflection
- Diffuse light emitters

---

### 💡 Lighting
- **Rectangular area light sources** with white light and intensity control
- **Multiple Importance Sampling (MIS)** combining BRDF and light sampling to reduce noise
- Direct + indirect lighting for realistic illumination

---

### ⚡ Performance
- **BVHNode acceleration structure**
  - Axis-aligned bounding box hierarchy
  - Significant performance boost on complex scenes

---

### 📸 Rendering & Output
- **Physically Based Exposure**
  - Real camera parameters (`F_NUMBER`, `SHUTTER`, `ISO`)
  - Exposure scaling for realistic brightness

- **ACES Filmic Tone Mapping**
  - Preserves midtones and prevents highlight clipping

- **Anti-Aliasing**
  - Stochastic supersampling per pixel

- **Image Output**
  - PPM format (simple and portable)

---

## 📂 Project Structure

| File                  | Description |
|-----------------------|-------------|
| `vec3.hpp`            | 3D vector math, random sampling, reflect/refract |
| `ray.hpp`             | Ray representation |
| `hittable.hpp`        | Base hittable interface and hit record |
| `hittable_list.hpp`   | List of hittable objects |
| `sphere.hpp`          | Sphere primitive |
| `moving_sphere.hpp`   | Moving sphere for motion blur |
| `xy_rect.hpp`         | Axis-aligned XY rectangle |
| `yz_rect.hpp`         | Axis-aligned YZ rectangle |
| `xz_rect.hpp`         | Axis-aligned XZ rectangle |
| `aabb.hpp`            | Axis-aligned bounding box for BVH |
| `bvh.hpp`             | Bounding Volume Hierarchy node |
| `material.hpp`        | Base material class |
| `lambertian.hpp`      | Diffuse material |
| `metal.hpp`           | Metallic reflection |
| `dielectric.hpp`      | Glass/refraction |
| `diffuse_light.hpp`   | Light-emitting material |
| `camera.hpp`          | Camera class |
| `onb.hpp`             | Orthonormal basis for sampling |
| `raytracer.cpp`       | Main rendering code and scene setup |

---

## 📸 Example Output

100 samples per pixel, Cornell-box-like scene with:
- White area light
- Glass and metal spheres
- Red and green walls
- BVH acceleration enabled

*(Insert render here — e.g., latest white-light output)*

---

## ⚡ Build & Run

Requires a **C++17** compiler.

```bash
g++ -std=c++17 -O2 raytracer.cpp -o raytracer
./raytracer
