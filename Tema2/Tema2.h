#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "LabCamera.h"
#include "Transform3D.h"
#include "Object3D.h"

using namespace std;

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		float fov;
		float ortoSize = 1;
		int typeOfPers = 0;
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;

		float seaRotate = 0;

		float planeY = 0;
		float planeZ = 0;
		float z_pos = 0;
		int goUp = 0;
		int goLeft;
		float inclining = 0;
		float virating = 0;
		int hit = 0;
		float hitBack = 0;
		float hitFront = 30.0f;
		float crash = 0;
		float hitIncline = 0.0f;
		int waterHit = 0;

		int rotorRotate = 1;

		float smokeSize = 2.0f;
		float smokePos = 0.0f;

		int livesLeft = 3;
		float fuelLeftProc = 1.0f;

		glm::mat4 firstPerson;

		int cloudsNr = 40;
		std::vector<int> cloud_z_pos;

		int fillNr = 2;
		int lastFill = -1;
		std::vector<int> fill_x_pos;
		std::vector<int> fill_y_pos;
		std::vector<int> fill_z_pos;

		int obstaclesNr = 2;
		int lastCol = -1;
		std::vector<int> obst_x_pos;
		std::vector<int> obst_y_pos;
		std::vector<int> obst_z_pos;

		float elapsed_time = 0.0f;
		float score = 0.0f;
		int level = 1;
		int increaseDif = 1;
		int gameOver = 0;

		float speed = 0.5f;
};
