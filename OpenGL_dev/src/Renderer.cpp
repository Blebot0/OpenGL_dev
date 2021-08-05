#include <iostream>
#include "Renderer.h"


/***************** Function to Catch errors in OpenGL ***************/
void GLClearError() {

    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {

    if (GLenum error = glGetError()) {

        std::cout << "[OpenGL Error] Error Code: ( " << error << " ) " << function << " " << file << " " << line << std::endl;
        return false;
    }

    return true;
}

//void Renderer::Draw(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const
//{
//    shader.Bind();
//    va.Bind();
//    ib.Bind();
//
//    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
//
//}
