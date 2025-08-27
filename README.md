# 🎨 3D Renderer in C

This is a **from-scratch 3D renderer** written in plain C.  
It casts rays into a scene of spheres, computes intersections, applies lighting & shadows, and outputs the final image as a PPM file.  

Built as part of a graphics programming assignment, but extended and cleaned up into a fun standalone project.  

---

## ✨ Features
- **Vector math library** (add, subtract, dot, normalize, etc.)
- **Dynamic world of spheres** with positions, radii, and colors
- **Ray–sphere intersection** using quadratic solutions
- **Point light source** with brightness and distance falloff
- **Lambertian shading** (diffuse lighting)
- **Hard shadows**
- **Anti-aliasing** with 3×3 grid sampling
- **Multi-file C project** with Makefile
- Outputs **PPM images** (portable pixmap) you can open with most image viewers

---

## 🖼️ Example Output
Example scene with multiple spheres, lighting, and shadows (with anti-aliasing enabled):
<img width="1278" height="962" alt="image" src="https://github.com/user-attachments/assets/b56f1fc5-3606-41f6-b767-19c7a75045ed" />  
<img width="1274" height="952" alt="image" src="https://github.com/user-attachments/assets/67011e32-e5f4-4546-8cfb-84031c82cd93" />  
<img width="1270" height="950" alt="image" src="https://github.com/user-attachments/assets/40a0080e-72d1-48be-a150-0fdf6011d755" />  

---

## 🚀 Getting Started

### Build
# Clone the repo
git clone https://github.com/yourusername/3D-Renderer.git
cd 3D-Renderer

# Build all versions
make

**This creates three executables:**  

**MS1_assg** → Vector Math + World Setup  
Demonstrates vector operations (add, subtract, dot, normalize, etc.) and dynamic world management.  
Outputs plain text results, not an image.  

**MS2_assg** → Grayscale Ray Tracing  
Implements the full ray tracing pipeline (camera, viewport, rays, ray–sphere intersection, lighting, shadows).  
Outputs a grayscale PPM image where spheres are white and the background is black.  

**FS_assg** → Final Submission (Full Color + Anti-Aliasing)  
Adds color parsing from HEX → RGB, sorts colors, assigns background/sphere colors properly, and smooths edges with 3×3 anti-aliasing.  
Outputs a full-color PPM image.  

## Run  
./FS_assg examples/input.txt output.ppm  
You can view the .ppm directly in many apps, or convert to PNG:  
convert output.ppm output.png   # (ImageMagick)  

---

## **📂 Input Format**  
Input files define the scene in plain text:  
image width image height  
viewport height  
focal length  
light position x y z light brightness  
num colors  
hex color 1 hex color 2 ...  
background color index  
num spheres  
sphere x y z radius color_index  

---

...
## **⚡ Example Input**  
640 480  
2.0  
1.0  
20.0 20.0 10.0 1000.0  
4  
0x1188EE 0xDD2266 0xDD7700 0x11CC66  
0  
3  
2.0 0.0 -5.0 2.0 2  
-2.0 0.0 -5.0 2.0 3  
0.0 -102.0 -5.0 100.0 1  

---

## **🛠️ Implementation Details**  
Written in C99  
Math: sqrt, dot products, vector normalization  
Shadows: secondary rays from intersection points  
Anti-aliasing: 9 rays per pixel, averaged  
Organized into src/ with headers for clarity  
Build targets controlled with preprocessor macros:  

**Compile with one of:**  
   -DMS1 → vector math + world setup (no rendering)  
   -DMS2 → grayscale ray tracing  
   -DFS  → full-color ray tracing with anti-aliasing  

---
