#include "Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <stb/stb_image.h>
#include <Core/Engine.h>

using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	materialShininess = 69;
	materialKd = 0.69;
	materialKs = 0.69;

	const string textureLoc = "Source/Laboratoare/Tema3/Textures/";
	height2 = stbi_load((textureLoc + "base.png").c_str(), &width, &height, &channels, STBI_grey);

	Texture2D* sky = new Texture2D();
	sky->Load2D((textureLoc + "sky.jpg").c_str(), GL_REPEAT);
	mapTextures["sky"] = sky;

	Texture2D* ground = new Texture2D();
	ground->Load2D((textureLoc + "ground.jpg").c_str(), GL_REPEAT);
	mapTextures["ground"] = ground;

	mapTextures["base"] = CreateHeightMap(200, 200);
		
	Mesh* ground2 = new Mesh("plane");
	ground2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
	meshes[ground2->GetMeshID()] = ground2;

	Mesh* sky2 = new Mesh("sphere");
	sky2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[sky2->GetMeshID()] = sky2;

	Shader* shader1 = new Shader("GroundShader");
	shader1->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader1->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader1->CreateAndLink();
	shaders[shader1->GetName()] = shader1;

	Shader* shader2 = new Shader("SkyShader");
	shader2->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
	shader2->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader2.glsl", GL_FRAGMENT_SHADER);
	shader2->CreateAndLink();
	shaders[shader2->GetName()] = shader2;
}

void Tema3::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
	if (remodelValue <= 0) {
		triggerDown = 0;
	} else if (remodelValue >= 200) {
		triggerDown = 1;
	}
	

	if (triggerDown == 0) {
		remodelValue += deltaTimeSeconds / 2.0f;
	}
	else {
		remodelValue -= deltaTimeSeconds / 2.0f;
	}

	Remodel(remodelValue);

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0, -3));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 50, 50));
	RenderSimpleMesh(meshes["sphere"], shaders["SkyShader"], modelMatrix, mapTextures["sky"]);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, -3, -3));
	RenderSimpleMesh(meshes["plane"], shaders["GroundShader"], modelMatrix, mapTextures["base"], mapTextures["ground"]);
}

void Tema3::Remodel(float value)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int index = i * width + j;
			char newValue = height2[index] + value / 8.0f;

			if (newValue < 0) 
			{
				height2[index] = std::rand()%10;
			}
			else 
			{
				height2[index] = newValue;
			}

		}
	}

	CreateHeightMap(width, height);
}

void Tema3::FrameEnd()
{
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int loc_mix = glGetUniformLocation(shader->program, "mixer");
	glUniform1i(loc_mix, 0);

	float clock = Engine::GetElapsedTime();
	int clock_location = glGetUniformLocation(shader->program, "Clock");
	glUniform1f(clock_location, clock);


	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_1"), 0);
	}

	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_2"), 1);
	}

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

Texture2D* Tema3::CreateHeightMap(unsigned int width, unsigned int height)
{
	GLuint textureID = 0;
	unsigned int size = width * height * channels;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, (void*)height2);
	glGenerateMipmap(GL_TEXTURE_2D);
	CheckOpenGLError();
	Texture2D* map = new Texture2D();
	map->Init(0, width, height, channels);
	return map;
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema3::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE)
	{
		int value = std::rand() % 50;
		Remodel(value);
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}