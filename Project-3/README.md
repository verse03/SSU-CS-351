# Project 3 — WebGL Shaders

A series of simple WebGL applications, each building on the previous one, exploring procedurally generated shapes with vertex and fragment shaders.

## Versions

1. [Wireframe Triangle](triangle.html) — An equilateral triangle drawn with `gl.LINE_LOOP` using three procedurally generated vertices.
https://blue.cs.sonoma.edu/~bdang/triangle.html
2. [10-Sided Filled Disk](disk.html) — A filled convex polygon drawn with `gl.TRIANGLE_FAN`, using a `uniform` variable `N` to control the number of vertices.
https://blue.cs.sonoma.edu/~bdang/disk.html
3. [Five-Pointed Star](star.html) — A static five-pointed star created by alternating vertex radii (tips vs. valleys) in the vertex shader.
https://blue.cs.sonoma.edu/~bdang/star.html
4. [Spinning Star](spinning-star.html) — The same five-pointed star, but rotating over time using a `uniform` variable `t` updated each frame.
https://blue.cs.sonoma.edu/~bdang/spinning-star.html