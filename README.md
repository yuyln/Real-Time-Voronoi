# Real Time Voronoi Diagram Renderer

Very simple program, made for studying purposes only.
Uses [GLFW](https://www.glfw.org) and [OpenCL](https://www.khronos.org/opencl/) to render and compute the [Voronoi Diagram](https://en.wikipedia.org/wiki/Voronoi_diagram).

It uses OpenCL to compute each pixel on the window, then uses a buffer to write this to the screen using GLFW.
