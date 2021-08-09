#include <glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
int main(void)
{
    GLFWwindow* window;

    /***************** Initialize the library *********************************/
    if (!glfwInit())
        return -1;

    /***************** Create a windowed mode window and its OpenGL context **************************/
    window = glfwCreateWindow(640, 480, "Purple Square", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /************************* Make the window's context current *******************************/
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW isn't imported correctly" << std::endl;
    }
    {
        // Increase the Interval between frames
        glfwSwapInterval(1);

        /******************** Vertex definition *****************************/
        float positions[] = {
            // Each line is a vertex 
            // Here we are just using vertex position as the only attribute
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2, // indices of Vertex of First Triangle
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        
        Model model;
        
        layout.Push<float>(2);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);


        IndexBuffer ib(indices, 6);

        Shader shader("resources/shaders/basic.shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color", 0.8f, 0.0f, 0.0f, 1.0f);


        Texture texture("resources/textures/youtube.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        float r = 0.00f, increment = 0.05f;
        

        /***************** Loop until the user closes the window ****************/
        while (!glfwWindowShouldClose(window))
        {
            model.Clear();

            /************************ Changning Colours in Runtime using Uniform ****************/
            //shader.Bind();
            //shader.SetUniform4f("u_Color", r, 0.0f, 0.0f, 1.0f);
            //shader.SetUniform1i("u_Texture", 0);

            model.Draw(va, ib, shader);


            ////if (r > 1.0f)
            //    increment -= 0.05f;
            //else if (r < 0.0f) {
            //    increment += 0.05f;
            //}

            //r += increment;


            /********************* Swap front and back buffers *********************/
            glfwSwapBuffers(window);

            /********************** Poll for and process events ********************/
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}