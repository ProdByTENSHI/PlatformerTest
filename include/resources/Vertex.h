#pragma once

#include <gl/glew.h>

#include "tenshiUtil/Types.h"

namespace tenshi
{
	struct Vertex
	{
		Vertex(GLfloat posX, GLfloat posY, GLfloat u, GLfloat v)
		{
			m_Position[0] = posX;
			m_Position[1] = posY;
			m_TexCoords[0] = u;
			m_TexCoords[1] = v;
		}

		GLfloat m_Position[2];
		GLfloat m_TexCoords[2];
	};

	inline std::vector<Vertex> GetVertices()
	{
		std::vector<Vertex> _vertices;

		_vertices.push_back(Vertex(0.0f, 1.0f, 0.0f, 1.0f));
		_vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 0.0f));
		_vertices.push_back(Vertex(0.0f, 0.0f, 0.0f, 0.0f));

		_vertices.push_back(Vertex(0.0f, 1.0f, 0.0f, 1.0f));
		_vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f));
		_vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, 0.0f));

		return _vertices;
	}
}