#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


int VertexAttribute(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Triangle with Vertex Attribute and Vertex Buffer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW isn't imported correctly" << std::endl;
    }

    /******************** Vertex definition *****************************/
    float positions[6] = {
        // Each line is a vertex 
        // Here we are just using vertex position as the only attribute
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.25f, 0.5f
    };
    unsigned int buffer;

    /************************* Allocating Buffer Space in GPU VRAM *****************/
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    /************************** Array Buffer with Float type Vertex Position *******************/
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);


    /********************* This is the function used to determine the vertex Attribute *****************************/
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, 3);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}