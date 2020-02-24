#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	Shader* shader = new Shader("SeaShader");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	// Scene
	{
		// The sea
		{
			Mesh* sea = Object3D::CreateCylinder("sea", glm::vec3(0.0f, 0.467f, 0.745f));
			AddMeshToList(sea);
		}

		// Clouds
		{
			Mesh* cloud = Object3D::CreateCube("cloud", glm::vec3(0.85f, 0.93f, 0.95f));
			AddMeshToList(cloud);

			for (int i = 0; i < cloudsNr; i++) {
				float z = (float)(rand() % 51) - 15.0f;
				cloud_z_pos.push_back(z);
			}
		}

		// The plane
		{
			Mesh* mesh = new Mesh("plane");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "avion.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		// Plane rotor
		{
			Mesh* rotor = Object3D::CreateCube("rotor", glm::vec3(0.85f, 0.93f, 0.95f));
			AddMeshToList(rotor);
		}

		// The obstacles
		{
			Mesh* smoke = Object3D::CreateSphere("smoke", glm::vec3(0.961f, 0.961f, 0.961f));
			AddMeshToList(smoke);
		}

		// The obstacles
		{
			Mesh* obst = Object3D::CreateSphere("obst", glm::vec3(0.0f, 0.2f, 0.0f));
			AddMeshToList(obst);

			for (int i = 0; i < obstaclesNr; i++) {
				float x = -100.0f + i * (200.0f / obstaclesNr);
				float aux = (rand() % 101) / 100.0f * 5.0f;
				x += aux;
				obst_x_pos.push_back(x);

				float y = (rand() % 36) - 25.0f;
				obst_y_pos.push_back(y);

				float z = (rand() % 101) - 100.0f;
				obst_z_pos.push_back(z);
			}
		}

		// The fuel
		{
			Mesh* fill = Object3D::CreateCube("fill", glm::vec3(0.0f, 0.81f, 0.82f));
			AddMeshToList(fill);

			for (int i = 0; i < fillNr; i++) {
				float x = -100.0f + i * (200.0f / fillNr);
				float aux = (rand() % 101) / 100.0f * 50.0f;
				x += aux;
				fill_x_pos.push_back(x);

				float y = (rand() % 36) - 25.0f;
				fill_y_pos.push_back(y);

				float z = (rand() % 101) - 100.0f;
				fill_z_pos.push_back(z);
			}
		}
	}

	// User interface
	{
		// The lives left
		{
			Mesh* mesh = Object3D::CreateCircle("life", glm::vec3(1, 0, 0));
			AddMeshToList(mesh);
		}

		// The fuel left
		{
			Mesh* mesh = Object3D::CreateSquare("canister", glm::vec3(0, 0, 0));
			AddMeshToList(mesh);

			mesh = Object3D::CreateSquare("fuel", glm::vec3(1, 1, 1));
			AddMeshToList(mesh);
		}
	}
	

}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.529f, 0.808f, 0.922f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	// Score
	{
		if (livesLeft > 0) {
			if (elapsed_time == 0.0f) {
				cout << "Get a big score by avoiding the obstacles and collecting fuel!" << endl << endl;
				cout << "Press C if you want to change your POV: third person (side or rear) and first person!" << endl;
				cout << "While pressing the left mouse button, move the cursor app, down, left or right to move the plane!" << endl << endl;
				cout << "You are now at level: 1!" << endl;
			}
			elapsed_time += deltaTimeSeconds;
			if (elapsed_time / (10 * level) >= 1) {
				score = elapsed_time + level * increaseDif * speed;
				level++;
				speed += 0.05f;

				cout << endl << "You are now at level: " << level << "!" << endl;
				cout << "Your score is " << score << "!" << endl << endl;
				cout << "Speed has increased!" << endl;

				if (level / (5 * increaseDif) > 0) {
					increaseDif++;

					cout << endl << "More obstacles were added!" << endl;

					{
						obstaclesNr++;
						float x = -100.0f + (obstaclesNr - 1) * (200.0f / obstaclesNr);
						float aux = (rand() % 101) / 100.0f * 5.0f;
						x += aux;
						obst_x_pos.push_back(x);

						float y = (rand() % 36) - 25.0f;
						obst_y_pos.push_back(y);

						float z = (rand() % 101) - 100.0f;
						obst_z_pos.push_back(z);
					}

				}
			}
		}
	}

	// Sea
	{
		seaRotate += 25.0f * deltaTimeSeconds;
		if (seaRotate >= 360.0f) {
			seaRotate = 0;
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scale(100, 32, 64);
		modelMatrix *= Transform3D::Translate(0, -1.85f, -2.2f);
		modelMatrix *= Transform3D::RotateOZ(RADIANS(seaRotate));
		modelMatrix *= firstPerson;

		RenderSimpleMesh(meshes["sea"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.467f, 0.745f));
	}

	// Clouds
	{
		for (int i = 0; i < cloudsNr; i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Scale(100, 32, 64);
			modelMatrix *= Transform3D::Translate(0, -1.85f, -2.2f);
			modelMatrix *= Transform3D::Scale(10.0f / 100.0f, 3.2f / 32.0f, 6.4f / 64.0f);
			modelMatrix *= Transform3D::RotateOZ(RADIANS(seaRotate) + RADIANS(i * 360.0f / cloudsNr));
			modelMatrix *= Transform3D::Translate(8.5f, 25.0f, cloud_z_pos[i]);
			modelMatrix *= Transform3D::RotateOX(RADIANS(5 * seaRotate));
			modelMatrix *= Transform3D::RotateOY(RADIANS(2 * seaRotate));
			RenderSimpleMesh(meshes["cloud"], shaders["Simple"], modelMatrix, glm::vec3(0.85f, 0.93f, 0.95f));

			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Scale(100, 32, 64);
			modelMatrix *= Transform3D::Translate(0, -1.85f, -2.2f);
			modelMatrix *= Transform3D::Scale(10.0f / 100.0f, 3.2f / 32.0f, 6.4f / 64.0f);
			modelMatrix *= Transform3D::RotateOZ(0.05f + RADIANS(seaRotate) + RADIANS(i * 360.0f / cloudsNr));
			modelMatrix *= Transform3D::Translate(8.5f, 25.0f, cloud_z_pos[i]);
			modelMatrix *= Transform3D::RotateOZ(RADIANS(seaRotate));
			modelMatrix *= Transform3D::RotateOY(RADIANS(3 * seaRotate));
			RenderSimpleMesh(meshes["cloud"], shaders["Simple"], modelMatrix, glm::vec3(0.35f, 0.23f, 0.15f));

			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Scale(100, 32, 64);
			modelMatrix *= Transform3D::Translate(0, -1.85f, -2.2f);
			modelMatrix *= Transform3D::Scale(10.0f / 100.0f, 3.2f / 32.0f, 6.4f / 64.0f);
			modelMatrix *= Transform3D::RotateOZ(0.1f + RADIANS(seaRotate) + RADIANS(i * 360.0f / cloudsNr));
			modelMatrix *= Transform3D::Translate(8.5f, 25.0f, cloud_z_pos[i]);
			modelMatrix *= Transform3D::RotateOX(RADIANS(4 * seaRotate));
			modelMatrix *= Transform3D::RotateOZ(RADIANS(2 * seaRotate));
			RenderSimpleMesh(meshes["cloud"], shaders["Simple"], modelMatrix, glm::vec3(0.15f, 0.43f, 0.45f));
		}
	}

	// Plane
	{
		if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
			if (inclining > 0) {
				inclining--;
			}
			else if (inclining < 0) {
				inclining++;
			}
		}

		if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
			if (virating > 0) {
				virating--;
			}
			else if (virating < 0) {
				virating++;
			}
		}

		if (livesLeft < 1) {
			planeY -= 1.0f;
			inclining -= 10;
		}


		z_pos = -50.0f + planeZ;
		if (z_pos > 0) {
			z_pos = 0;
		}
		else if (z_pos < -100) {
			z_pos = -100;
		}

		if (hit == 1) {
			hitIncline = 10.0f;
			if (hitBack < 30.0f) {
				hitBack += 100.0 * deltaTimeSeconds;
				crash = hitBack;
			}
			else if (hitFront > 0.0f) {
				hitFront -= 20.0 * deltaTimeSeconds;
				crash = hitFront;
			}
			else {
				hit = 0;
				waterHit = 0;
			}

			if (waterHit != 1) {
				float explode = crash / 15.0f;

				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(-30.0f, -20.0f + planeY, z_pos + 2.0f);
				modelMatrix *= Transform3D::Scale(explode, explode, explode);
				RenderSimpleMesh(meshes["obst"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));

				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(-30.0f, -20.0f + planeY, z_pos - 2.0f);
				modelMatrix *= Transform3D::Scale(explode, explode, explode);
				RenderSimpleMesh(meshes["obst"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));

				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(-32.0f, -20.0f + planeY, z_pos);
				modelMatrix *= Transform3D::Scale(explode, explode, explode);
				RenderSimpleMesh(meshes["obst"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));

				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(-28.0f, -20.0f + planeY, z_pos);
				modelMatrix *= Transform3D::Scale(explode, explode, explode);
				RenderSimpleMesh(meshes["obst"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));

				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(-30.0f, -18.0f + planeY, z_pos);
				modelMatrix *= Transform3D::Scale(explode, explode, explode);
				RenderSimpleMesh(meshes["obst"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));

				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(-30.0f, -22.0f + planeY, z_pos);
				modelMatrix *= Transform3D::Scale(explode, explode, explode);
				RenderSimpleMesh(meshes["obst"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));
			}

		}
		else {
			hitBack = 0.0f;
			hitFront = 30.0f;
			crash = 0.0f;
			hitIncline = 0.0f;

			if (inclining >= -10.0f) {
				if (smokeSize <= 0.5f) {
					smokeSize = 2.0f;
				}
				else {
					smokeSize -= deltaTimeSeconds * 2.0f;
				}

				for (int i = 0; i < 6; i++) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix *= Transform3D::Translate(-45.0f - 4.0f * i - 10.0f * (2.0f - smokeSize), -30.0f + planeY - i * 0.5f, z_pos);
					modelMatrix *= Transform3D::Scale(smokeSize - i * 0.1f, smokeSize - i * 0.1f, smokeSize - i * 0.1f);
					RenderSimpleMesh(meshes["smoke"], shaders["SeaShader"], modelMatrix, glm::vec3(0.961f, 0.961f, 0.961f));
				}
			}
		}

		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix *= Transform3D::Translate(-40.0f - crash, -20.0f + planeY, z_pos);
		modelMatrix *= Transform3D::Scale(4, 4, 4);
		modelMatrix *= Transform3D::RotateOZ(RADIANS(inclining));
		modelMatrix *= Transform3D::RotateOY(RADIANS(virating));
		modelMatrix *= Transform3D::RotateOX(RADIANS(-virating));
		modelMatrix *= Transform3D::RotateOZ(RADIANS(hitIncline));
		modelMatrix *= Transform3D::RotateOY(RADIANS(90));

		RenderSimpleMesh(meshes["plane"], shaders["VertexNormal"], modelMatrix, glm::vec3(0.0f, 0.467f, 0.745f));

		modelMatrix *= Transform3D::Translate(1.8f, -1.2f, 3.3f);
		if ((rotorRotate % 6 >= 0) && (rotorRotate % 6 < 3)) {
			modelMatrix *= Transform3D::Scale(1.5f, 0.2f, 0.1f);
		}
		else {
			modelMatrix *= Transform3D::Scale(0.2f, 1.5f, 0.1f);
		}
		rotorRotate++;

		RenderSimpleMesh(meshes["rotor"], shaders["Simple"], modelMatrix, glm::vec3(0.15f, 0.43f, 0.45f));


		if (-20.0f + planeY <= -30.0f) {
			if (livesLeft > 0) {
				waterHit = 1;
				livesLeft--;

				cout << endl << "Try to stay above the water!" << endl << endl;

				planeY = 20.0f;

				hit = 1;
				hitBack = 0.0f;
				hitFront = 30.0f;
				crash = 0.0f;
				hitIncline = 0.0f;
			}
		}
	}

	// Obstacles
	{
		for (int i = 0; i < obstaclesNr; i++) {
			int colided = 0;

			obst_x_pos[i] -= speed * 2;
			if (obst_x_pos[i] <= -100.0f) {
				float aux = (rand() % 101) / 100.0f * 5.0f;
				obst_x_pos[i] = 100.0f + aux;

				float y = (rand() % 36) - 25.0f;
				obst_y_pos[i] = y;

				float z = (rand() % 101) - 100.0f;
				obst_z_pos[i] = y;
			}

			glm::mat4 modelMatrix = glm::mat4(1);
			if (typeOfPers != 0) {
				modelMatrix *= Transform3D::Translate(obst_x_pos[i], obst_y_pos[i], obst_z_pos[i]);
				modelMatrix *= Transform3D::Scale(2, 2, 2);
			}
			else {
				modelMatrix *= Transform3D::Translate(obst_x_pos[i], obst_y_pos[i], z_pos);
				modelMatrix *= Transform3D::Scale(2, 2, 2);
			}

			if (typeOfPers != 0) {
				if ((obst_x_pos[i] >= -40.0f - 3.0f) && (obst_x_pos[i] <= -40.0f + 3.0f)) {
					if ((obst_z_pos[i] >= z_pos - 10.0f) && (obst_z_pos[i] <= z_pos + 10.0f)) {
						if ((obst_y_pos[i] >= -20.0f + planeY - 10.0f) && (obst_y_pos[i] <= -20.0f + planeY + 10.0f)) {
							if (i != lastCol) {
								colided = 1;
								livesLeft--;
								cout << endl << "Easy there, you hit an obstacle!" << endl << endl;

								if ((livesLeft < 1) && (gameOver == 0)) {
									gameOver = 1;
									cout << endl << "You ran out of lives!" << endl;
									cout << "Game over! Your score was: " << score << endl << endl;
								}
							}
						}
					}
				}
			}
			else {
				if ((obst_x_pos[i] >= -40.0f - 3.0f) && (obst_x_pos[i] <= -40.0f + 3.0f)) {
					if ((obst_y_pos[i] >= -20.0f + planeY - 5.0f) && (obst_y_pos[i] <= -20.0f + planeY + 5.0f)) {
						if (i != lastCol) {
							colided = 1;
							livesLeft--;
							cout << endl << "Easy there, you hit an obstacle!" << endl << endl;

							if ((livesLeft < 1) && (gameOver == 0)) {
								gameOver = 1;
								cout << endl << "You ran out of lives!" << endl;
								cout << "Game over! Your score was: " << score << endl << endl;
							}
						}
					}
				}
			}

			if (colided == 0) {
				RenderSimpleMesh(meshes["obst"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));
			}
			else {
				hit = 1;
				hitBack = 0.0f;
				hitFront = 30.0f;
				crash = 0.0f;
				hitIncline = 0.0f;
				float aux = (float)(rand() % 201);
				obst_x_pos[i] = 100.0f + aux;
			}
		}
	}

	// Fillings
	{
		for (int i = 0; i < fillNr; i++) {

			fill_x_pos[i] -= speed * 4;
			if (fill_x_pos[i] <= -100.0f) {
				float aux = (rand() % 101) / 100.0f * 50.0f;
				fill_x_pos[i] = 100.0f + aux;

				float y = (rand() % 36) - 25.0f;
				fill_y_pos[i] = y;

				float z = (rand() % 101) - 100.0f;
				fill_z_pos[i] = z;
			}

			for (int j = 0; j < 3; j++) {
				int colided = 0;

				glm::mat4 modelMatrix = glm::mat4(1);
				if (typeOfPers != 0) {
					modelMatrix *= Transform3D::Translate(fill_x_pos[i] + j * 7.0f, fill_y_pos[i] + j % 2 * 0.75f, fill_z_pos[i]);
					modelMatrix *= Transform3D::Scale(2, 2, 2);
				}
				else {
					modelMatrix *= Transform3D::Translate(fill_x_pos[i] + j * 7.0f, fill_y_pos[i] + j % 2 * 0.75f, z_pos);
					modelMatrix *= Transform3D::Scale(2, 2, 2);
				}

				if (typeOfPers != 0) {
					if ((fill_x_pos[i] >= -40.0f - 5.0f) && (fill_x_pos[i] <= -40.0f + 5.0f)) {
						if ((fill_z_pos[i] >= z_pos - 10.0f) && (fill_z_pos[i] <= z_pos + 10.0f)) {
							if ((fill_y_pos[i] >= -20.0f + planeY - 10.0f) && (fill_y_pos[i] <= -20.0f + planeY + 10.0f)) {
								colided = 1;
							}
						}
					}
				}
				else {
					if ((fill_x_pos[i] >= -40.0f - 5.0f) && (fill_x_pos[i] <= -40.0f + 5.0f)) {
						if ((fill_y_pos[i] >= -20.0f + planeY - 10.0f) && (fill_y_pos[i] <= -20.0f + planeY + 10.0f)) {
							colided = 1;
						}
					}
				}

				if (colided == 0) {
					modelMatrix *= Transform3D::RotateOX(RADIANS(10 * seaRotate));
					modelMatrix *= Transform3D::RotateOY(RADIANS(20 * seaRotate));
					RenderSimpleMesh(meshes["fill"], shaders["SeaShader"], modelMatrix, glm::vec3(0.0f, 0.81f, 0.82f));
				}
				else {
					cout << endl << "You collected fuel, keep going!" << endl << endl;
					fuelLeftProc += 0.12f;
					float aux = (float)(rand() % 101);
					fill_x_pos[i] = 100.0f + aux;
				}
			}
		}
	}

	// Life
	{
		for (int i = 0; i < livesLeft; i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-2.25f + i * 0.11f, 2.2f, 0.0f);
			modelMatrix *= Transform3D::Scale(0.05f, 0.05f, 0);
			RenderMesh(meshes["life"], shaders["VertexColor"], modelMatrix);
		}
	}

	// Fuel
	{
		fuelLeftProc -= deltaTimeSeconds / 20;
		if (fuelLeftProc < 0) {
			fuelLeftProc = 0;
			livesLeft = 0;

			if ((livesLeft < 1) && (gameOver == 0)) {
				gameOver = 1;
				cout << endl << "You ran out of fuel!" << endl;
				cout << "Game over! Your score was: " << score << endl << endl;
			}
		}
		if (fuelLeftProc > 1) {
			fuelLeftProc = 1;
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(1.52f, 2.12f, 0.0f);
		modelMatrix *= Transform3D::Scale(fuelLeftProc * 0.076f, 0.011f, 0);
		RenderMesh(meshes["fuel"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(1.5f, 2.1f, 0.0f);
		modelMatrix *= Transform3D::Scale(0.08f, 0.015f, 0);
		RenderMesh(meshes["canister"], shaders["VertexColor"], modelMatrix);
	}

	projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	int model_location = glGetUniformLocation(shader->program, "Model");

	glm::mat4 matrix(1.0f);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int view_location = glGetUniformLocation(shader->program, "View");

	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	if (typeOfPers % 3 == 1) {
		viewMatrix = glm::lookAt(
			glm::vec3(-100.0f, 0.0f, -50.0f),
			glm::vec3(1.0f, -0.2f, -50.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (typeOfPers % 3 == 2) {
		viewMatrix = glm::lookAt(
			glm::vec3(-40.0f - crash, -20.0f + planeY + 4.0f, z_pos - 7.0f),
			glm::vec3(-10.0f - crash, -20.0f + planeY + 4.0f, z_pos - 7.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	int projection_location = glGetUniformLocation(shader->program, "Projection");

	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	float clock = Engine::GetElapsedTime();
	int clock_location = glGetUniformLocation(shader->program, "Clock");
	glUniform1f(clock_location, 2 * abs(sinf(clock)));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C)
	{
		typeOfPers = (typeOfPers + 1) % 3;
		if (typeOfPers % 3 != 0) {
			firstPerson = Transform3D::RotateOY(RADIANS(1));
		}
		else {
			firstPerson = Transform3D::RotateOY(RADIANS(-1));
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (deltaY < 0) {
			goUp = 1;
			if (inclining < 30) {
				if (inclining < 0) {
					inclining += 8;
				}
				else {
					inclining += 2;
				}
			}
		} else if (deltaY > 0) {
			goUp = -1;
			if (inclining > -30) {
				if (inclining > 0) {
					inclining -= 8;
				}
				else {
					inclining -= 2;
				}
			}
		}
		planeY -= (float)deltaY / 4;

		if (deltaX < 0) {
			goLeft = 1;
			if (virating < 30) {
				if (virating < 0) {
					virating += 8;
				}
				else {
					virating += 2;
				}
			}
		}
		else if (deltaX > 0) {
			goLeft = -1;
			if (virating > -30) {
				if (virating > 0) {
					virating -= 8;
				}
				else {
					virating -= 2;
				}
			}
		}
		planeZ += (float)deltaX / 4.0f;
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
