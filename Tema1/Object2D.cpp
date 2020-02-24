#include "Object2D.h"

#include <Core/Engine.h>
#include <iostream>
#include <math.h>

using namespace std;

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 color) {
	glm::vec3 corner = glm::vec3(0, 0, 0);
	float length = 10;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	indices.push_back(0);
	indices.push_back(2);

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateParallelogram(std::string name, glm::vec3 color) {
	glm::vec3 corner = glm::vec3(0, 0, 0);
	float length = 10;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(2 * length, 0, 0), color),
		VertexFormat(corner + glm::vec3(3 * length, length, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color)
	};

	Mesh* parallelogram = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	indices.push_back(0);
	indices.push_back(2);

	parallelogram->InitFromData(vertices, indices);
	return parallelogram;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 color) {
	glm::vec3 corner = glm::vec3(0, 0, 0);
	float length = 10;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, length / 2, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	indices.push_back(0);
	indices.push_back(2);

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateCircle(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float nrPuncte = 999;
	float step = 1.0f / nrPuncte;
	float x, y;

	int indicesNr = 0;

	for (x = 1; x > 0; x -= step) {
		indicesNr++;
		y = sqrt(1 - x * x);
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}
	for (x = 0; x > -1; x -= step) {
		indicesNr++;
		y = sqrt(1 - x * x);
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}
	for (x = -1; x < 0; x += step) {
		indicesNr++;
		y = -sqrt(1 - x * x);
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}
	for (x = 0; x < 1; x += step) {
		indicesNr++;
		y = -sqrt(1 - x * x);
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}

	for (int i = 1; i < indicesNr; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	Mesh* circle = new Mesh(name);
	circle->InitFromData(vertices, indices);
	return circle;
}