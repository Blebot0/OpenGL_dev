#include "Model.h"

void Model::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

void Model::Clear() const
{
	/**************** Render here *********************/
	GLCall( glClear(GL_COLOR_BUFFER_BIT));
}
