#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>


class Tema1 : public SimpleScene {
public:
	struct ViewportSpace {
		ViewportSpace() : x(0), y(0), width(1), height(1) {}
		ViewportSpace(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
		int x;
		int y;
		int width;
		int height;
	};

	struct LogicSpace {
		LogicSpace() : x(0), y(0), width(1), height(1) {}
		LogicSpace(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}
		float x;
		float y;
		float width;
		float height;
	};

public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void DrawScene(float deltaTimeSeconds, glm::mat3 visMatrix);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

	glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

	void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

	void DrawPipes(float deltaTimeSeconds, glm::mat3 visMatrix);
	void DrawBird(float deltaTimeSeconds, glm::mat3 visMatrix);
	void DrawBackground(glm::mat3 visMatrix);
	void DrawGameOver();


protected:
	float length;
	ViewportSpace viewSpace;
	LogicSpace logicSpace;
	glm::mat3 modelMatrix, visMatrix, resize;

	float translateX, translateY;
	float translateX2, translateY2;
	float translateX3, translateY3;
	float translateX4, translateY4;
	float scaleX, scaleY;
	float angularStep, angularStep2;

	float direction_up;
	float inertion;
	float scale_positive;
	float start;
	float fly;

	int posx, posy;
	float acceleration = 9.81f;
	float elapsedTime2 = 0, initialSpeed2 = 0;
	float elapsedTime3 = 0, initialSpeed3 = 0;

	int myWidth = 1600;
	int myHeight = 900;

	int pipesNr = 5;

	std::vector<int> x_positions;
	std::vector<int> crazy_pipe;
	std::vector<int> grow;
	std::vector<float> up_pipe_length;
	std::vector<float> down_pipe_length;
	std::vector<float> up_pipe_length2;
	std::vector<float> down_pipe_length2;

	int speed;
	float gameTime;
	int level;
	int scoreUpdate;
	int levelBonus;
	int minDif;

	int bird_head_x;
	int bird_head_y;
	int bird_head_radius;

	int gameOver, announced, flicker = 0;

	float bgr, bgg, bgb;

};
