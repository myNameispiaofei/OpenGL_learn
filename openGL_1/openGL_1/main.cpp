//
//  main.cpp
//  openGL_1
//
//  Created by 母斌 on 2019/9/29.
//  Copyright © 2019 母斌. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y ,aPos.z, 1.0);\n"
    "}\n";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f ,0.2f, 1.0);\n"
    "}\n";

using namespace std;
int main(int argc, const char * argv[]) {
    glfwInit();
    //    主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //    次版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //    告诉glfw 我们使用的是core_profile 核心模块
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //    向前兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //    创建一个GLFW 窗口   宽 高  窗口名字  后边两个暂时不用管
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" <<endl;
        glfwTerminate();
        return -1;
    }
    //    通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);
    
    //    GLAD 是用来管理openGL 的函数指针的。所以在调用任何openGL函数之前我们都需要初始化GLAD。
    //    我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    
    // 一个顶点  缓冲对象
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };
   
    float testRectangle[] = {
        0.5f,  0.5f, 0.0f, //右上
        0.5f, -0.5f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, //左上
    };
    
    float test6Tran[] = {
        -0.5f,  -0.5f, 0.0f,
         0.5f,  -0.5f, 0.0f,
         0.0f,   0.3f, 0.0f,
        -0.5f,   0.0f, 0.0f,
         0.5f,   0.0f, 0.0f,
         0.0f,  -0.8f, 0.0f,
    };
    
    unsigned int indices[] = {
      0, 1, 2,
      3, 4, 5
    };
    
    
    unsigned int VAO, VBO ,EBO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1 ,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(test6Tran), &test6Tran, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
    
    
    
    // 链接顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), ((void *)0));
    glEnableVertexAttribArray(0);
    
    // 一个顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char shaderInfoLog[512];
    glad_glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, shaderInfoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << shaderInfoLog << endl;
    }
    
    //一个片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    //创建着色程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glad_glAttachShader(shaderProgram, vertexShader);
    glad_glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderInfoLog);
    };
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    

    //    创建渲染的视口: 我们必须要告诉OpenGl 需要渲染的尺寸大小，即为视口 viewport(),这样openGL 才能知道根据窗口大小显示数据和坐标。
    //    glViewport 前两个参数控制视口左下角位置，后两个参数控制视口的宽和高
    //    openGL 幕后使用的是glViewport 定义的 位置和宽高进行2D转换
    glViewport(0, 0, 800, 600);
    //    窗口调整的时候 视口应该也被调整  对窗口注册一个回调函数每次窗口大小被调整的时候会被调用
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    
    
    //   通过 glfwSetFramebufferSizeCallback glfw 函数 当窗口frame 变化时会调用。
    //    对于视网膜屏 Retain 屏   宽度和高度明显比原输入值更高一点。
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    void processInput(GLFWwindow *window);
    
    //   为了防止 渲染的图像一出现就退出 我们使用while 循环 。我们可以称之为Render Loop
    //    glfwWindowShouldClose 每次循环开始前检查一次GLFW 是否被要求退出 是true 的话渲染便结束了。
    while(!glfwWindowShouldClose(window))
    {
        //输出控制
        processInput(window);
        //        glfwSwapBuffers 会交换颜色缓冲（他是存储着GLFW 窗口每一个像素色值的大缓冲），将会作为输出显示在屏幕上
        //        当程序退出的时候 使用一个自定义的颜色清空屏幕  在每个新的渲染迭代可是的时候我们总希望清屏否则总是看到上次渲染的结果。
        //        我们可以使用glClear   GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。 我们清空颜色。
        glClearColor(0.20f, 0.30f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//        glDrawArrays(GL_TRIANGLES, 1, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
        //        glfwPollEvents 检查函数有没有触发什么事件 键盘输入 鼠标移动 并调用对应函数
        
    }
    glfwTerminate();
    
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    //
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

