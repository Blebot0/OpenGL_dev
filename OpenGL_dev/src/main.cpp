#include <glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

/***************** Structure to define string of Vertex and Fragment Shaders ***************/
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

/***************** Function to Parse values from a filepath to extract Vertex and Fragment Shaders ***************/
static ShaderProgramSource ParseShader(const std::string& filepath) {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;


    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss[2];
    while (std::getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }

        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}
/********************* Function to Compile Shaders ******************************/
static unsigned int CompileShader(unsigned int type, const std::string& source) {

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {

        std::cout << "FAILED TO COMPILE SHADER\n";

        glDeleteShader(id);

        return 0;
    }
    return id;
}

/********************* Function to Create Shaders ******************************/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}

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
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f,
        };

        unsigned int indices[] = {
            0, 1, 2, // indices of Vertex of First Triangle
            2, 3, 0
        };


        VertexArray va;
        VertexBuffer vb(positions, 8 * sizeof(float));
        VertexBufferLayout layout;

        layout.Push<float>(2);
        va.AddBuffer(vb, layout);


        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("resources/shaders/basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);

        int location = glGetUniformLocation(shader, "u_Color");
        ASSERT(location != -1);

        float r = 0.00f, g = 0.00f, b = 0.00f, increment = 0.05f;

        /***************** Loop until the user closes the window ****************/
        while (!glfwWindowShouldClose(window))
        {
            /**************** Render here *********************/
            glClear(GL_COLOR_BUFFER_BIT);

            /************************ Changning Colours in Runtime using Uniform ****************/
            GLCall(glUniform4f(location, r, 0.0f, 0.0f, 1.0f));

            if (r > 1.0f)
                increment -= 0.05f;
            else if (r < 0.0f) {
                increment += 0.05f;
            }

            r += increment;

            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /********************* Swap front and back buffers *********************/
            glfwSwapBuffers(window);

            /********************** Poll for and process events ********************/
            glfwPollEvents();
        }

        /********************** Deleting Shaders ********************/
        glDeleteProgram(shader);

    }
    glfwTerminate();
    return 0;
}