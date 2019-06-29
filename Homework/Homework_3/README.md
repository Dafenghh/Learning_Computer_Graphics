# 计算机图形学 作业3


徐达烽

16340260

## Problem 1
> 使用Bresenham算法(只使用integer arithmetic)画一个三角形边框：input为三个2D点；output三条直线（要求图元只能用GL_POINTS，不能使用其他，比如GL_LINES等）。

画三角形即画三条线段。我们先完成generateLine函数，从两个端点生成线段上的所有点，然后对这些点用GL_LINES图元来画即可。


## Problem 2
> 使用Bresenham算法(只使用integer arithmetic)画一个圆：input为一个2D点(圆心)、一个integer半径； output为一个圆。

将圆分为8份，只需要画出1/8圆弧，然后用对称性质生成其他的点。


## Problem 3
> 在GUI在添加菜单栏，可以选择是三角形边框还是圆，以及能调整圆的大小(圆心固定即可)。

本次作业实现用，将GLFW、GLAD、ImGUI的初始化操作封装在一个类CG_tools里面，这样主函数就看起来清晰简洁了不少。



实现效果见演示视频。