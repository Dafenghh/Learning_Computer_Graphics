# 计算机图形学 作业5


徐达烽

16340260

## Problem 1
> 使用OpenGL(3.3及以上)+GLFW或freeglut画一个简单的三角形。

### 实验结果


### 实现思路
参照教程，写好两个shader，连接好. 我把shader的代码存放在txt文件中，程序运行时先读取进来。


然后定义顶点数组，VAO和VBO即可。

```c++
	float vertices[] = {
		-0.5f, -0.3f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.3f, 0.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);//unbind
```

渲染循环中，这样实现画三角形：
```c++
glUseProgram(shaderProgram);
glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
glDrawArrays(GL_TRIANGLES, 0, 3);
```


## Problem 2
> 对三角形的三个顶点分别改为红绿蓝，像下面这样。并解释为什么会出现这样的结果。

### 实验结果

### 实现思路

修改vertex_shader和fragment_shader, 使其能够传进颜色作为属性。

```c++
// vertex_shader.txt
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor; 
void main()
{
    gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
}

// fragment_shader.txt
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
void main()
{
    FragColor = vec4(ourColor, 1.0f);
} 
```

然后修改顶点数组，加入颜色坐标，并且在VAO中添加属性：
```c++
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```
这样就做完了。为什么会出现这样的效果呢？因为在片段着色器中，会进行片段插值。

## Problem 3

### 实验结果
见Problem_3_Result.gif 动图

### 实现思路
导入imGUI后，参照imGUI的API 文档（在imGUI.h的注释里），就可以完成了。

这样生成一个取色器：

```c++
ImGui::Begin("Hello, world!");                          
ImGui::ColorEdit3("color picker", (float*)&color); // Edit 3 floats representing a color
ImGui::End();
```

并且在fragment_shader中，设置ourColor为uniform变量，主函数中这样更新它：
```c++
glUniform4f(vertexColorLocation, color.x, color.y, color.z, 1.0f);
```

## Problem 4

### 实验结果

### 实验思路

使用GL_LINES即可画出直线。
```c++
glDrawArrays(GL_LINES, 0, 4);
```

## Problem 5

### 实验结果


### 实现思路

依照教程设置好EBO, 并且渲染时用`glDrawElements`即可画出两个三角形。