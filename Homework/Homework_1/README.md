# Homework 1 – Learning about CG

徐达烽
16340260



## Problem 1

> 结合上述参考链接（或其他参考资料），谈谈自己对计算机图形学的理解。 

维基百科上，对计算机图形学的正式解释为：

> 计算机图形学（英语：computer graphics，缩写为CG）是研究计算机在硬件和软件的帮助下创建计算机图形的科学学科，是计算机科学的一个分支领域，主要关注数字合成与操作视觉的图形内容。虽然这个词通常被认为是指三维图形，事实上同时包括了二维图形以及影像处理。

由此我们可以狭义地理解，CG就是指用计算机来生成图形。而我们日常生活中，也经常接触到世界上顶尖的CG技术。比如好莱坞超逼真的电影特效，又或者现在越来越细致的3D游戏，都是计算机图形学的典型应用场景。

除此之外，计算机图形学也有非常广阔的应用。例如，计算机辅助设计软件就需要这门学科的支撑。


以数字图像处理和计算机视觉做对比：

学科|输入|输出
:-:|:---:|:---:
数字图像处理|图像|图像
计算机视觉|图像|Knowledge
计算机图形学|Knowledge|图形

这是我对这三门学科的理解。


## Problem 2

> 结合上述参考链接（或其他参考资料），回答什么是OpenGL? OpenGL ES? Web GL? Vulkan? DirectX? 

OpenGL是一个由Khronos组织制定并维护的规范(Specification), 用于渲染2D、3D矢量图形.

OpenGL ES(OpenGL for Embedded Systems)是OpenGL的子集，针对手机、PDA和游戏主机等嵌入式设备而设计。

WebGL是基于OpenGL ES的JavaScript API，用于在不使用插件的情况下在任何兼容的网页浏览器中呈现交互式2D和3D图形。

Vulkan是一个低开销、跨平台的二维、三维图形与计算的应用程序接口（API），最早由科纳斯组织在2015年游戏开发者大会（GDC）上发表。与OpenGL类似，Vulkan针对全平台即时3D图形程序（如电子游戏和交互媒体）而设计，并提供高性能与更均衡的CPU与GPU占用。

DirectX（Direct eXtension，缩写：DX）是由微软公司创建的一系列专为多媒体以及游戏开发的应用程序接口。旗下包含Direct3D、Direct2D、DirectCompute等等多个不同用途的子部分。

## Problem 3
> gl.h glu.h glew.h的作用分别是什么? 

gl.h是OpenGL版本1.1的基本头文件。如果要使用新于1.1版本的功能，就要添加扩展库。

glu.h是实用库，包含有43个函数，函数名的前缀为glu。不过这个库已经很久没有更新过了，所以现在很少使用这个头文件。

glew.h 全程是OpenGL Extension Wrangler Library，这是OpenGL扩展功能的跨平台库。


4.	使用GLFW和freeglut的目的是什么？ 

由于OpenGL的抽象性和跨平台性，如果直接使用OpenGL的接口，那么我们需要自己处理创建窗口，定义OpenGL上下文以及处理用户输入。这样会比较麻烦。

GLFW和freeglut可以节省我们书写操作系统相关代码的时间，提供一个窗口和上下文用来渲染。


5.	结合上述参考链接（或其他参考资料），选择一个SIGGRAPH 2017/2018上 你最喜欢的专题，介绍该专题是做 什么的，使用了什么CG技术？（不少于100字）


我最喜欢(SIGGRAPH 2017)[http://kesen.realtimerendering.com/sig2017.html]的"Get More Out of Your Photo"专题。

第一篇文章(VNect: Real-time 3D Human Pose Estimation with a Single RGB Camera)[http://gvv.mpi-inf.mpg.de/projects/VNect/]首次提出了使用单个RGB相机实时捕获人类全部3D骨骼姿势的方法。它使用了新的全卷积姿势公式，实时联合回归2D和3D关节位置，不需要紧密裁剪的输入框架。这是第一种可用于3D角色控制等实时应用的单目RGB方法。

在我看来，这种新的捕获方法使动作捕捉能变得更加亲民化。因为他们的结果在质量上与单目RGB-D方法（如Kinect）的结果相当，甚至一些场景下更好。而RGB-D相机显然比RGB相机成本高许多。相信在不远的将来，在更多优秀算法的帮助下，动作捕捉将会完全普及，成为人民群众的日常娱乐项目。

这个专题使用了CG中的动作捕捉技术。