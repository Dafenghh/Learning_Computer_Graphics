# 计算机图形学 作业5


徐达烽

16340260

效果见camera.mp4视频。

## Basic

### 1. 投影
(1)把上次作业绘制的cube放置在(-1.5, 0.5, -1.5)位置，要求6个面颜色不一致.

36个顶点，每个顶点用8个gloat描述，三个空间坐标，两个纹理坐标，三个颜色坐标。
为修改颜色方便，每一面的颜色用一个宏描述。

```c++
#define CUBE_COLOR1 0.8f, 0.1f, 0.2f
#define CUBE_COLOR2 0.0f, 0.7f, 0.1f
#define CUBE_COLOR3 0.0f, 0.8f, 0.6f
#define CUBE_COLOR4 0.4f, 0.3f, 0.9f
#define CUBE_COLOR5 0.9f, 0.8f, 0.9f
#define CUBE_COLOR6 0.9f, 0.1f, 0.9f

float vertices[] = {
    -2.0f, -2.0f, -2.0f,  0.0f, 0.0f, CUBE_COLOR1,
     2.0f, -2.0f, -2.0f,  1.0f, 0.0f, CUBE_COLOR1,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR1,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR1,
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR1,
    -2.0f, -2.0f, -2.0f,  0.0f, 0.0f, CUBE_COLOR1,

    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR2,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR2,
     2.0f,  2.0f,  2.0f,  1.0f, 1.0f, CUBE_COLOR2,
     2.0f,  2.0f,  2.0f,  1.0f, 1.0f, CUBE_COLOR2,
    -2.0f,  2.0f,  2.0f,  0.0f, 1.0f, CUBE_COLOR2,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR2,

    -2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR3,
    -2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR3,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR3,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR3,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR3,
    -2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR3,

     2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR4,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR4,
     2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR4,
     2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR4,
     2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR4,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR4,

    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR5,
     2.0f, -2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR5,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR5,
     2.0f, -2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR5,
    -2.0f, -2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR5,
    -2.0f, -2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR5,

    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR6,
     2.0f,  2.0f, -2.0f,  1.0f, 1.0f, CUBE_COLOR6,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR6,
     2.0f,  2.0f,  2.0f,  1.0f, 0.0f, CUBE_COLOR6,
    -2.0f,  2.0f,  2.0f,  0.0f, 0.0f, CUBE_COLOR6,
    -2.0f,  2.0f, -2.0f,  0.0f, 1.0f, CUBE_COLOR6
};

...
// position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// texture coord attribute
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
// color attribute
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
glEnableVertexAttribArray(2);
```

使用translate即可将cube放置在(-1.5, 0.5, -1.5)位置。

(2)正交投影(orthographic projection)：实现正交投影，使用多组(left, right, bottom, top, near, far)参数， 比较结果差异.

```c++
float ortho_param[6] = {-6.0f, 6.0f, -6.0f, 6.0f, 0.1f, 100.0f};
...
ImGui::InputFloat2("Left and Right For Ortho", ortho_param);
ImGui::InputFloat2("Bottom and Up  For Ortho", ortho_param + 2);
ImGui::InputFloat2("Near and Far   For Ortho", ortho_param + 4);
...
projection = glm::ortho(ortho_param[0], ortho_param[1], ortho_param[2], ortho_param[3], ortho_param[4], ortho_param[5]);
shader.setMat4("projection", projection);
```

不同参数的结果：
```
[left,right,bottom,up,near,far]=[-6.0f, 6.0f, -6.0f, 6.0f, 0.1f, 100.0f]: 
```

![标准参数](https://github.com/Dafenghh/Learning_Computer_Graphics/raw/master/Homework/Homework_5/img/ortho.png)

```
[left,right,bottom,up,near,far]=[-10.0f, 10.0f, -6.0f, 6.0f, 0.1f, 100.0f]: 
```
![left,right:-10..10](https://github.com/Dafenghh/Learning_Computer_Graphics/raw/master/Homework/Homework_5/img/ortho_left_right_10.png)

```
[left,right,bottom,up,near,far]=[-6.0f, 6.0f, -10.0f, 10.0f, 0.1f, 100.0f]: 
```
![bottom,up:-10..10](https://github.com/Dafenghh/Learning_Computer_Graphics/raw/master/Homework/Homework_5/img/ortho_bottom_up_10.png)

```
[left,right,bottom,up,near,far]=[-6.0f, 6.0f, -6.0f, 6.0f, 0.1f, 20.0f]: 
```

![far:20](https://github.com/Dafenghh/Learning_Computer_Graphics/raw/master/Homework/Homework_5/img/ortho_far_20.png)

(3)透视投影(perspective projection)：实现透视投影，使用多组参数，比较结果差异


```c++
float perspect_param[3] = {45.0f, 0.1f, 100.0f};
...
ImGui::InputFloat3("Param For Perspective", perspect_param);
...
projection = glm::perspective(perspect_param[0], (float)SCR_WIDTH / (float)SCR_HEIGHT, perspect_param[1], perspect_param[2]);
shader.setMat4("projection", projection);
```

不同参数以及结果：

```
[fov, near, far]=[45.0f, 0.1f, 100.0f]
```
![标准参数](https://github.com/Dafenghh/Learning_Computer_Graphics/raw/master/Homework/Homework_5/img/perspective.png)

```
[fov, near, far]=[45.0f, 18.0f, 100.0f]
```
![标准参数](https://github.com/Dafenghh/Learning_Computer_Graphics/raw/master/Homework/Homework_5/img/perspective_near_18.png)

### 2. 视角变换

```c++
float camera_radian = clock_camera.GetTime();
camX = sin(camera_radian) * radius;
camZ = cos(camera_radian) * radius;
glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
shader.setMat4("view", view);
```

说明：clock_camera是计时器，它的GetTime()方法会返回当前的时间，当用户暂停camera的旋转时，这个计时器也会暂停。

### 3. GUI里添加菜单栏，可以选择各种功能。

本次作业GUI的完整菜单栏是：
```c++
ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
ImGui::Checkbox("Rotate Cube", &rotate_cube);
ImGui::Checkbox("Move Vertical", &move_vertical);
ImGui::Checkbox("Move Horizontal", &move_horizontal);
ImGui::Checkbox("Scale", &can_scale);
ImGui::Checkbox("Rotate Camera", &rotate_camera);
ImGui::Checkbox("Orthographic Projection", &ortho);
ImGui::InputFloat2("Left and Right For Ortho", ortho_param);
ImGui::InputFloat2("Bottom and Up  For Ortho", ortho_param + 2);
ImGui::InputFloat2("Near and Far   For Ortho", ortho_param + 4);
ImGui::InputFloat3("Param For Perspective", perspect_param);
ImGui::InputFloat3("Cube Position", cube_position);
ImGui::Checkbox("User Control", &user_control);
ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
ImGui::End();
```

### 4. 简答题
> 在现实生活中，我们一般将摄像机摆放的空间View matrix和被拍摄的物体摆设的空间Model matrix分开，但 是在OpenGL中却将两个合二为一设为ModelView matrix，通过上面的作业启发，你认为是为什么呢？

在openGL中实际上是不存在照相机这个物体的，实际上我们通过转换物体的位置来达到照相机不同角度和位置的效果，通过一个矩阵直接变换至需要的位置，能够节约计算资源.


对单个物体来说，摄像机绕着物体旋转的效果，和摄像机不动，物体自转的显示效果是相同的，所以可以合并Model和View矩阵。


## Bonus

见camera.h