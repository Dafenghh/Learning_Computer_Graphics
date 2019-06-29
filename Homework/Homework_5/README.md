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

![标准参数](ortho.png)
![left,right:-10..10](ortho_left_right_10.png)
![bottom,up:-10..10](ortho_bottom_up_10.png)
![far:20](ortho_far_20.png)

(3)透视投影(perspective projection)：实现透视投影，使用多组参数，比较结果差异


## Bonus