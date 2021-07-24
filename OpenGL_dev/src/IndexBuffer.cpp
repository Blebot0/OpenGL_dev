#include <glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// Debug Break is a function provided by MS VS
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/***************** Function to Catch errors in OpenGL ***************/

static void GLClearError() {
        
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {

    if (GLenum error = glGetError()) {

        std::cout << "[OpenGL Error] Error Code: ( " << error << " ) "<<function<<" "<<file<<" "<<line<<std::endl;
        return false;
    }

    return true;
}
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

int IndexBuffer(void)
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

    unsigned int buffer;

    /************************* Allocating Buffer Space in GPU VRAM *****************/
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /************************** Array Buffer with Float type Vertex Position *******************/
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);


    /********************* This is the function used to determine the vertex Attribute *****************************/
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    unsigned int ibo; // index buffer object


    /************************* Allocating Index Buffer Space in GPU VRAM *****************/
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    /************************** Array Buffer with Float type Indices Position *******************/
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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

        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /********************* Swap front and back buffers *********************/
        glfwSwapBuffers(window);

        /********************** Poll for and process events ********************/
        glfwPollEvents();
    }

    /********************** Deleting Shaders ********************/
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}