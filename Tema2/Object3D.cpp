#include "Object3D.h"

#include <Core/Engine.h>
#include <iostream>
#include <math.h>

using namespace std;

Mesh* Object3D::CreateCylinder(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float nrPuncte = 15;
	float step = 1.0f / nrPuncte;
	float x, y, x2, y2, z, c;

	int indicesNr = 0;
	int start = 0;

	for (z = 0; z <= 3; z += 0.25f) {

		for (x = 1; x > 0; x -= step) {
			indicesNr++;
			y = sqrt(1 - x * x);

			x2 = x + (std::rand() % (81)) / 1000.0f - 0.04f;
			y2 = y + (std::rand() % (81)) / 1000.0f - 0.04f;
			c = (std::rand() % 5) / 10.0f - 0.2f;;
			vertices.push_back(VertexFormat(glm::vec3(x2, y2, z), color * (1.0f + c)));
		}
		for (x = 0; x > -1; x -= step) {
			indicesNr++;
			y = sqrt(1 - x * x);
			x2 = x + (std::rand() % (81)) / 1000.0f - 0.04f;
			y2 = y + (std::rand() % (81)) / 1000.0f - 0.04f;
			c = (std::rand() % 5) / 10.0f - 0.2f;;
			vertices.push_back(VertexFormat(glm::vec3(x2, y2, z), color * (1.0f + c)));
		}
		for (x = -1; x < 0; x += step) {
			indicesNr++;
			y = -sqrt(1 - x * x);
			x2 = x + (std::rand() % (81)) / 1000.0f - 0.04f;
			y2 = y + (std::rand() % (81)) / 1000.0f - 0.04f;
			c = (std::rand() % 5) / 10.0f - 0.2f;;
			vertices.push_back(VertexFormat(glm::vec3(x2, y2, z), color * (1.0f + c)));
		}
		for (x = 0; x < 1; x += step) {
			indicesNr++;
			y = -sqrt(1 - x * x);
			x2 = x + (std::rand() % (81)) / 1000.0f - 0.04f;
			y2 = y + (std::rand() % (81)) / 1000.0f - 0.04f;
			c = (std::rand() % 5) / 10.0f - 0.2f;;
			vertices.push_back(VertexFormat(glm::vec3(x2, y2, z), color * (1.0f + c)));
		}


		if (z > 0) {
			for (int i = start; i < start + indicesNr; i++) {
				indices.push_back(i);
				indices.push_back(i + 1);
				indices.push_back(i - indicesNr);

				indices.push_back(i + 1);
				indices.push_back(i - indicesNr + 1);
				indices.push_back(i - indicesNr);
			}
		}
		start = start + indicesNr;
		indicesNr = 0;
	}

	Mesh* cylinder = new Mesh(name);
	cylinder->InitFromData(vertices, indices);
	return cylinder;
}

Mesh* Object3D::CreateSphere(std::string name, glm::vec3 color) {
		int i, j;
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;
		int indicator = 0;

		float lats = 50;
		float longs = 50;

		for (i = 0; i <= lats; i++) {
			double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
			double z0 = sin(lat0);
			double zr0 = cos(lat0);

			double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
			double z1 = sin(lat1);
			double zr1 = cos(lat1);

			for (j = 0; j <= longs; j++) {
				double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
				double x = cos(lng);
				double y = sin(lng);

				vertices.push_back(VertexFormat(glm::vec3(x * zr0, y * zr0, z0), color));
				indices.push_back(indicator);
				if (j > 1) {
					indices.push_back(indicator - 1);
					indices.push_back(indicator - 2);
				}
				indicator++;

				vertices.push_back(VertexFormat(glm::vec3(x * zr1, y * zr1, z1), color));
				indices.push_back(indicator);
				if (j > 1) {
					indices.push_back(indicator - 1);
					indices.push_back(indicator - 2);
				}
				indicator++;
			}
			indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
		}

	Mesh* sphere = new Mesh(name);
	sphere->InitFromData(vertices, indices);
	return sphere;
}

Mesh* Object3D::CreateCube(std::string name, glm::vec3 color) {

	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(1, -1,  1), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(-1, 1,  1), glm::vec3(0, 0, 1)),
		VertexFormat(glm::vec3(1, 1,  1), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(-1, -1,  -1), glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(1, -1,  -1), glm::vec3(1, 0, 1)),
		VertexFormat(glm::vec3(-1, 1,  -1), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(1, 1,  -1), glm::vec3(1, 1, 1))

	};

	vector<unsigned short> indices =
	{
		0, 1, 2,
		1, 3, 2,
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4
	};

	Mesh* cube = new Mesh(name);
	cube->InitFromData(vertices, indices);
	return cube;
}

Mesh* Object3D::CreateSquare(std::string name, glm::vec3 color) {
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

Mesh* Object3D::CreateParallelogram(std::string name, glm::vec3 color) {
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

Mesh* Object3D::CreateTriangle(std::string name, glm::vec3 color) {
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

Mesh* Object3D::CreateCircle(std::string name, glm::vec3 color) {
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