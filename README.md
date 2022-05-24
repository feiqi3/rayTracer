<!--
 * @Author: feiqi3
 * @Date: 2022-03-01 23:47:35
 * @LastEditTime: 2022-05-24 11:58:14
 * @LastEditors: feiqi3
 * @Description: |The readme File|
 * @FilePath: \rayTracer\README.md
 * ->blog: feiqi3.cn <-
-->
My work after reading "Ray Tracing in one weekend"
Here is the book [Ray Tracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
Lots of new features have been added on the origin program in the book.
Until now, the following features have been done:    
- Texture mapping base on Barycentric coordinate     
- Multithreading support  
- .obj model rendering
- Sky box  
- A simple( and also incorrect) Blinn-Phong.
    
TO DO:   
- Importance sampling  
- Accecleration structure  
- Post effects
NO plan for:  
The randome scene.    

how to run:  
mkdir build  
cd build   
cmake ..  
make   
./ray_tracer
Then run it and wait to be surprised.


![](https://s2.loli.net/2022/05/23/LPozr4nQcMu1ldN.jpg)
