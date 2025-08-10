# Ray Tracer in C++

A CPU-based ray tracer written from scratch in modern C++.  
This is a personal learning project to gain a deep understanding of computer graphics and C++ through hands-on implementation — without relying on external rendering libraries.

The aim is to build a strong foundation in rendering algorithms, then progressively extend the system towards advanced features such as BVH acceleration, realistic lighting, and potentially a full software renderer or visualization tool.

---

## Features

- **Custom `Vec3` Math Library**
  - 3D vector operations
  - Random sampling utilities
  - Reflection and refraction support
- **Ray–Object Intersection**
  - Sphere intersection
  - Moving spheres for motion blur
- **Camera**
  - Configurable field of view
  - Depth of field via aperture sampling
- **Lighting & Background**
  - Gradient sky background
  - HDR-friendly sky colors
  - Area light sources
- **Material System with Recursive Path Tracing**
  - Lambertian (diffuse)
  - Metal with adjustable surface roughness (fuzz)
  - Dielectric (glass) with refraction and Fresnel reflection
- **Physically Based Exposure**
  - Real camera parameters (`F_NUMBER`, `SHUTTER`, `ISO`)
  - Exposure scaling for realistic light intensities
- **ACES Filmic Tone Mapping**
  - Prevents highlight clipping
  - Preserves midtone contrast
  - Produces natural, cinematic output
- **Multiple Importance Sampling (MIS) for Diffuse Surfaces**
  - Combines BRDF and light sampling to reduce noise in direct lighting
- **Anti-aliasing**
  - Stochastic supersampling per pixel
- **PPM Image Output**
  - Simple, portable output format
  - Viewable in GIMP, IrfanView, ImageMagick

---

## Project Structure

| File                  | Description |
|-----------------------|-------------|
| `vec3.hpp`            | 3D vector math, random sampling, reflect/refract |
| `ray.hpp`             | Ray representation (`origin`, `direction`) |
| `hittable.hpp`        | Base `Hittable` interface and `HitRecord` structure |
| `hittable_list.hpp`   | List of hittable objects |
| `sphere.hpp`          | Sphere implementation of `Hittable` |
| `moving_sphere.hpp`   | Sphere with animated position (motion blur) |
| `material.hpp`        | Base material interface |
| `lambertian.hpp`      | Diffuse material |
| `metal.hpp`           | Metallic reflection with fuzziness |
| `dielectric.hpp`      | Transparent refractive material |
| `camera.hpp`          | Camera with DOF and shutter time |
| `raytracer.cpp`       | Main rendering loop, scene setup |
| `onb.hpp`             | Orthonormal basis for local coordinate frames (sampling) |

---

## Example Output

Sample render at 100 samples per pixel:

*(Insert rendered image here)*

---

## Build & Run

Requires a C++17-compatible compiler.

```bash
g++ -std=c++17 -O2 raytracer.cpp -o raytracer
./raytracer
