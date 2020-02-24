#pragma once
#pragma once

#include <string>
#include <iostream>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object3D {
	Mesh* CreateSquare(std::string name, glm::vec3 color);
	Mesh* CreateParallelogram(std::string name, glm::vec3 color);
	Mesh* CreateTriangle(std::string name, glm::vec3 color);
	Mesh* CreateCircle(std::string name, glm::vec3 color);
	Mesh* CreateCylinder(std::string name, glm::vec3 color);
	Mesh* CreateSphere(std::string name, glm::vec3 color);
	Mesh* CreateCube(std::string name, glm::vec3 color);
	

}

