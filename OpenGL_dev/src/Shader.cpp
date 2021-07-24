#include <glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

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

        else{
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}
/********************* Function to Compile Shaders ******************************/
static unsigned int CompileShader( unsigned int type, const std::string& source) {

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

int Shader(void)
{
    GLFWwindow* window;

    /***************** Initialize the library *********************************/
    if (!glfwInit())
        return -1;

    /***************** Create a windowed mode window and its OpenGL context **************************/
    window = glfwCreateWindow(640, 480, "Purple Triangle", NULL, NULL);
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

    /******************** Vertex definition *****************************/
    float positions[6] = {
        // Each line is a vertex 
        // Here we are just using vertex position as the only attribute
        -0.5f, -0.5f,
         0.f, 0.5f,
         0.5f, -0.5f
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



    ShaderProgramSource source = ParseShader("resources/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader); 
    
    /***************** Loop until the user closes the window ****************/
    while (!glfwWindowShouldClose(window))
    {
        /**************** Render here *********************/
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, 3);
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