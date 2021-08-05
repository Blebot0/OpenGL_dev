#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

class Model {

	public:
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};