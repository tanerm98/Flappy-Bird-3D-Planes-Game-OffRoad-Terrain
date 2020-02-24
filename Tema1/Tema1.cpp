#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;
typedef struct GLFWwindow GLFWwindow;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x = 0;
	logicSpace.y = 0;
	logicSpace.width = myWidth;
	logicSpace.height = myHeight;

	// background color
	bgr = 0.486f;
	bgg = 0.655f;
	bgb = 0.922f;

	// initial bird position
	posx = myWidth / 4;
	posy = 1.5f * myHeight / 2;

	// initialize tx and ty (the translation steps) for all objects
	translateX = 0;
	translateY = 0;
	translateX2 = 0;
	translateY2 = 0;
	translateX3 = 0;
	translateY3 = 0;
	translateX4 = 0;
	translateY4 = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularSteps
	angularStep = 1.5f;
	angularStep2 = 0.0f;

	// initialize movement
	direction_up = false;
	inertion = false;
	scale_positive = true;
	start = false;
	fly = true;

	//initialize game stats
	speed = 200;
	gameTime = 0;
	level = 1;
	scoreUpdate = 1;
	levelBonus = 0;
	minDif = 30;
	gameOver = false;
	announced = false;

	// initial bird dimensions
	bird_head_x = 0.66f * posx;
	bird_head_radius = 0.66f * 30;

	//Bird Body
	Mesh* bird_head = Object2D::CreateCircle("bird_head", glm::vec3(0.100f, 0.8314f, 0.3608f));
	AddMeshToList(bird_head);
	Mesh* bird_body = Object2D::CreateCircle("bird_body", glm::vec3(0.502f, 0.2745f, 0.1059f));
	AddMeshToList(bird_body);
	Mesh* bird_eye = Object2D::CreateCircle("bird_eye", glm::vec3(1, 1, 1));
	AddMeshToList(bird_eye);
	Mesh* bird_iris = Object2D::CreateCircle("bird_iris", glm::vec3(0, 0, 0));
	AddMeshToList(bird_iris);
	Mesh* bird_beak = Object2D::CreateTriangle("bird_beak", glm::vec3(0.55f, 0.27f, 0.0f));
	AddMeshToList(bird_beak);
	Mesh* bird_mouth = Object2D::CreateTriangle("bird_mouth", glm::vec3(0, 0, 0));
	AddMeshToList(bird_mouth);
	Mesh* bird_wing = Object2D::CreateParallelogram("bird_wing", glm::vec3(0.86f, 0.16f, 0.16f));
	AddMeshToList(bird_wing);

	//Pipes with random colors
	float r = (std::rand() % (101)) / 100.0f;
	float g = (std::rand() % (101)) / 100.0f;
	float b = (std::rand() % (101)) / 100.0f;
	Mesh* pipe = Object2D::CreateSquare("pipe", glm::vec3(r, g, b));
	AddMeshToList(pipe);

	r = (std::rand() % (101)) / 100.0f;
	g = (std::rand() % (101)) / 100.0f;
	b = (std::rand() % (101)) / 100.0f;
	Mesh* pipe2 = Object2D::CreateSquare("pipe2", glm::vec3(r, g, b));
	AddMeshToList(pipe2);

	//Background
	Mesh* sun = Object2D::CreateCircle("sun", glm::vec3(0.9765f, 0.8431f, 0.1098f));
	AddMeshToList(sun);
	Mesh* cloud = Object2D::CreateCircle("cloud", glm::vec3(0.573938f, 0.603827f, 0.657568f));
	AddMeshToList(cloud);

	// Initializing pipes with random lengths, random space (bigger than a minimum) between pipes and random sliding
	int u_p_l, d_p_l, dif;
	for (int i = 0; i < pipesNr; i++) {
		d_p_l = 5 + (std::rand() % (45 - 5 + 1));
		dif = 0;
		while (dif < minDif) {
			u_p_l = 5 + (std::rand() % (60 - 5 + 1));
			dif = 90 - d_p_l - u_p_l;
		}
		int crazy = std::rand() % 3;

		x_positions.push_back(i * 1600 / (pipesNr - 1));
		crazy_pipe.push_back(crazy);
		grow.push_back(0);
		down_pipe_length.push_back(d_p_l);
		up_pipe_length.push_back(u_p_l);

		down_pipe_length2.push_back(d_p_l);
		up_pipe_length2.push_back(u_p_l);
	}

	std::cout << endl << endl << "Welcome! Keep the bird alive by trying not to hit the obstacles!" << endl;
	std::cout << "You are playing with " << pipesNr << " pipes per screen!" << endl;
	std::cout << endl << "Press <SPACEBAR> to jump, <P> to pause the game and <R> to restart!" << endl;
	std::cout << "Press <Z> to lower the number of pipes per screen and <X> to increase it!" << endl;
	std::cout << endl << "Minimum space is now 30!" << endl;
	std::cout << endl << "Your current score is 0!" << endl;

}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void Tema1::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Tema1::Update(float deltaTimeSeconds)
{

	glm::ivec2 resolution = window->GetResolution();

	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(bgr, bgg, bgb), true);

	// Compute uniform 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix = visMatrix * VisualizationTransf2DUnif(logicSpace, viewSpace);
	DrawScene(deltaTimeSeconds, visMatrix);

	if (gameOver == true) {
		start = false;
		DrawGameOver();
		if (announced == false) {
			announced = true;
			std::cout << endl << "GAME OVER!" << endl;
			std::cout << endl << "Your final score is " << 5 * levelBonus << "!" << endl;
		}
	}

	if (start == true) {
		gameTime += deltaTimeSeconds;
		if (gameTime > level * 10) {
			// difficulty increases with each passed level
			// bird speed is increased
			level++;
			speed += 200;

			std::cout << endl << "You've reached level " << level << "!" << endl;

			// minimum possible distance between upper and lower pipe is decreased
			if (minDif > 10) {
				minDif -= 2;
				std::cout << endl << "Minimum space is now " << minDif << "!" << endl;
			}
			 
			// pipe color is different for each level
			float r = (std::rand() % (101)) / 100.0f;
			float g = (std::rand() % (101)) / 100.0f;
			float b = (std::rand() % (101)) / 100.0f;
			Mesh* pipe = Object2D::CreateSquare("pipe", glm::vec3(r, g, b));
			AddMeshToList(pipe);

			r = (std::rand() % (101)) / 100.0f;
			g = (std::rand() % (101)) / 100.0f;
			b = (std::rand() % (101)) / 100.0f;
			Mesh* pipe2 = Object2D::CreateSquare("pipe2", glm::vec3(r, g, b));
			AddMeshToList(pipe2);
		}

		// the score gains get bigger for each passed level
		if (gameTime > scoreUpdate) {
			scoreUpdate++;
			levelBonus += level;
			std::cout << "Your current score is " << 5 * levelBonus << "!" << endl;
		}
	}

}

void Tema1::DrawGameOver() {
	// this is the screen which appears when the player loses
	// the color of everything changes once every 15 frames
	flicker++;

	if (flicker % 30 == 0) {
		float r = (std::rand() % (101)) / 100.0f;
		float g = (std::rand() % (101)) / 100.0f;
		float b = (std::rand() % (101)) / 100.0f;
		Mesh* pipe = Object2D::CreateSquare("pipe", glm::vec3(r, g, b));
		AddMeshToList(pipe);

		r = (std::rand() % (101)) / 100.0f;
		g = (std::rand() % (101)) / 100.0f;
		b = (std::rand() % (101)) / 100.0f;
		Mesh* pipe2 = Object2D::CreateSquare("pipe2", glm::vec3(r, g, b));
		AddMeshToList(pipe2);

		Mesh* sun = Object2D::CreateCircle("sun", glm::vec3(1.0f - 0.9765f, 1.0f - 0.8431f, 1.0f - 0.1098f));
		AddMeshToList(sun);
		Mesh* cloud = Object2D::CreateCircle("cloud", glm::vec3(1.0f - 0.573938f, 1.0f - 0.603827f, 1.0f - 0.657568f));
		AddMeshToList(cloud);

		Mesh* bird_head = Object2D::CreateCircle("bird_head", glm::vec3(0.100f, 0.8314f, 0.3608f));
		AddMeshToList(bird_head);
		Mesh* bird_body = Object2D::CreateCircle("bird_body", glm::vec3(0.502f, 0.2745f, 0.1059f));
		AddMeshToList(bird_body);
		Mesh* bird_eye = Object2D::CreateCircle("bird_eye", glm::vec3(1, 1, 1));
		AddMeshToList(bird_eye);
		Mesh* bird_iris = Object2D::CreateCircle("bird_iris", glm::vec3(0, 0, 0));
		AddMeshToList(bird_iris);
		Mesh* bird_beak = Object2D::CreateTriangle("bird_beak", glm::vec3(0.55f, 0.27f, 0.0f));
		AddMeshToList(bird_beak);
		Mesh* bird_mouth = Object2D::CreateTriangle("bird_mouth", glm::vec3(0, 0, 0));
		AddMeshToList(bird_mouth);
		Mesh* bird_wing = Object2D::CreateParallelogram("bird_wing", glm::vec3(0.86f, 0.16f, 0.16f));
		AddMeshToList(bird_wing);

		bgr = 1 - bgr;
		bgg = 1 - bgg;
		bgb = 1 - bgb;
	}
	else if (flicker % 30 == 15) {
		float r = (std::rand() % (101)) / 100.0f;
		float g = (std::rand() % (101)) / 100.0f;
		float b = (std::rand() % (101)) / 100.0f;
		Mesh* pipe = Object2D::CreateSquare("pipe", glm::vec3(r, g, b));
		AddMeshToList(pipe);

		r = (std::rand() % (101)) / 100.0f;
		g = (std::rand() % (101)) / 100.0f;
		b = (std::rand() % (101)) / 100.0f;
		Mesh* pipe2 = Object2D::CreateSquare("pipe2", glm::vec3(r, g, b));
		AddMeshToList(pipe2);

		Mesh* sun = Object2D::CreateCircle("sun", glm::vec3(0.9765f, 0.8431f, 0.1098f));
		AddMeshToList(sun);
		Mesh* cloud = Object2D::CreateCircle("cloud", glm::vec3(0.573938f, 0.603827f, 0.657568f));
		AddMeshToList(cloud);

		Mesh* bird_head = Object2D::CreateCircle("bird_head", glm::vec3(1.0f - 0.100f, 1.0f - 0.8314f, 1.0f - 0.3608f));
		AddMeshToList(bird_head);
		Mesh* bird_body = Object2D::CreateCircle("bird_body", glm::vec3(1.0f - 0.502f, 1.0f - 0.2745f, 1.0f - 0.1059f));
		AddMeshToList(bird_body);
		Mesh* bird_eye = Object2D::CreateCircle("bird_eye", glm::vec3(0, 0, 0));
		AddMeshToList(bird_eye);
		Mesh* bird_iris = Object2D::CreateCircle("bird_iris", glm::vec3(1, 1, 1));
		AddMeshToList(bird_iris);
		Mesh* bird_beak = Object2D::CreateTriangle("bird_beak", glm::vec3(1.0f - 0.55f, 1.0f - 0.27f, 1.0f - 0.0f));
		AddMeshToList(bird_beak);
		Mesh* bird_mouth = Object2D::CreateTriangle("bird_mouth", glm::vec3(1, 1, 1));
		AddMeshToList(bird_mouth);
		Mesh* bird_wing = Object2D::CreateParallelogram("bird_wing", glm::vec3(1.0f - 0.86f, 1.0f - 0.16f, 1.0f - 0.16f));
		AddMeshToList(bird_wing);
	}
}

void Tema1::DrawScene(float deltaTimeSeconds, glm::mat3 visMatrix) {
	DrawBird(deltaTimeSeconds, visMatrix);
	DrawPipes(deltaTimeSeconds, visMatrix);
	DrawBackground(visMatrix);
}

void Tema1::DrawBackground(glm::mat3 visMatrix) {
	if (start == true) {
		translateX3 -= speed / 20 * 0.2f;
		translateX4 -= speed / 20 * 0.02f;
	}

	// the clouds ar constantly moving
	for (int i = 0; i < 3; i++) {
		int space = i * myWidth / 2;

		if ((translateX3 + myWidth / 4 + space < -200) && (start == true)) {
			translateX3 = myWidth / 8;
		}

		modelMatrix = visMatrix;
		modelMatrix *= Transform2D::Translate(translateX3 + myWidth / 6 + space, myHeight * 0.5f) * Transform2D::Scale(120, 100);
		RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix;
		modelMatrix *= Transform2D::Translate(translateX3 + myWidth / 8 + space, myHeight * 0.4f) * Transform2D::Scale(150, 100);
		RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix;
		modelMatrix *= Transform2D::Translate(translateX3 + myWidth / 4 + space, myHeight * 0.4f) * Transform2D::Scale(200, 100);
		RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
	}

	// the sun is also moving, but slower
	modelMatrix = visMatrix * Transform2D::Translate(translateX4, 0);
	modelMatrix *= Transform2D::Translate(myWidth / 2, myHeight * 0.66f) * Transform2D::Scale(300, 300);
	RenderMesh2D(meshes["sun"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawPipes(float deltaTimeSeconds, glm::mat3 visMatrix) {

	float pi = 3.14159265359f;

	glm::mat3 scale, scale1, scale2;
	int upPipeLength, downPipeLength, crazy;

	// if a pipe gets out of sight, a new one is generated with random distance, lengths etc...
	if (x_positions[0] < -80) {
		x_positions.erase(x_positions.begin());
		crazy_pipe.erase(crazy_pipe.begin());
		grow.erase(grow.begin());
		down_pipe_length.erase(down_pipe_length.begin());
		up_pipe_length.erase(up_pipe_length.begin());

		down_pipe_length2.erase(down_pipe_length2.begin());
		up_pipe_length2.erase(up_pipe_length2.begin());

		int u_p_l, d_p_l, dif;
		d_p_l = 5 + (std::rand() % (45 - 5 + 1));
		dif = 0;
		while (dif < minDif) {
			u_p_l = 5 + (std::rand() % (60 - 5 + 1));
			dif = 90 - d_p_l - u_p_l;
		}
		int crazy = std::rand() % 3;

		x_positions.push_back(x_positions.back() + 1600 / (pipesNr - 1) - 100 + (std::rand() % (201)));
		crazy_pipe.push_back(crazy);
		grow.push_back(0);
		down_pipe_length.push_back(d_p_l);
		up_pipe_length.push_back(u_p_l);

		down_pipe_length2.push_back(d_p_l);
		up_pipe_length2.push_back(u_p_l);
	}

	// rendering each pipe
	for (int i = 0; i < pipesNr; i++) {
		if (start != false) {
			x_positions[i] -= deltaTimeSeconds * speed;
			if (crazy_pipe[i] == 1) {
				// crazy pipe means a pair of pipes which scale continuosly
				if (grow[i] == 0) {
					down_pipe_length2[i] -= 50 * deltaTimeSeconds;
					up_pipe_length2[i] -= 50 * deltaTimeSeconds;
					if ((down_pipe_length2[i] <= 0) || (up_pipe_length2[i] <= 0)) {
						grow[i] = 1;
					}
				}
				else {
					down_pipe_length2[i] += 50 * deltaTimeSeconds;
					up_pipe_length2[i] += 50 * deltaTimeSeconds;
					if ((down_pipe_length2[i] >= down_pipe_length[i]) || (up_pipe_length2[i] >= up_pipe_length[i])) {
						grow[i] = 0;
					}
				}
			}
		}
		int pos = x_positions[i];
		upPipeLength = up_pipe_length2[i];
		downPipeLength = down_pipe_length2[i];

		// here is checked whether the bird hit a pipe or not
		if ((bird_head_x + bird_head_radius + 20 >= x_positions[i]) && (bird_head_x - bird_head_radius <= x_positions[i] + 80)) {
			if ((bird_head_y <= downPipeLength * 10 + 10) || (bird_head_y >= myHeight - upPipeLength * 10 - 10)) {
				gameOver = true;
			}
		}

		scale = Transform2D::Scale(8, downPipeLength);
		scale1 = Transform2D::Scale(8, upPipeLength);
		scale2 = Transform2D::Scale(10, 1);

		modelMatrix = visMatrix;
		modelMatrix = modelMatrix * Transform2D::Translate(pos, 0) * scale;
		RenderMesh2D(meshes["pipe"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix;
		modelMatrix = modelMatrix * Transform2D::Translate(pos - 10, downPipeLength * 10) * scale2;
		RenderMesh2D(meshes["pipe2"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix;
		modelMatrix = modelMatrix * Transform2D::Translate(pos + 80, myHeight) * scale1 * Transform2D::Rotate(pi);
		RenderMesh2D(meshes["pipe"], shaders["VertexColor"], modelMatrix);
		modelMatrix = visMatrix;
		modelMatrix = modelMatrix * Transform2D::Translate(pos - 10, myHeight - upPipeLength * 10 - 10) * scale2;
		RenderMesh2D(meshes["pipe2"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema1::DrawBird(float deltaTimeSeconds, glm::mat3 visMatrix) {
	// the bird wing makes flu-flu
	if (angularStep > 3.5f) {
		fly = false;
	}
	else if (angularStep < 1.5f) {
		fly = true;
	}
	if (fly == true) {
		angularStep += 10 * deltaTimeSeconds;
	}
	else {
		angularStep -= 10 * deltaTimeSeconds;
	}

	translateY = translateY2;

	// the bird goes up (with inertion) and down (with gravitation) => realistic physics
	if (start == true) {
		if (direction_up == true)
		{
			direction_up = false;
			inertion = true;
			elapsedTime3 = 0;
			initialSpeed3 = 1000;
		}
		else if ((inertion == true) && (initialSpeed3 >= 0)) {
			float dist = initialSpeed3 * deltaTimeSeconds - 32 * (acceleration * elapsedTime3 * elapsedTime3 / 2);

			if (translateY + posy <= myHeight * 1.5f) {
				if (dist >= 0) {
					translateY += 1.5f * dist;
					elapsedTime3 += deltaTimeSeconds;
					initialSpeed3 -= 32 * acceleration * elapsedTime3;
				}
				else {
					inertion = false;
					elapsedTime2 = 0;
				}
			}
			else {
				inertion = false;
			}
		}
		else if (translateY + posy >= 0)
		{
			translateY -= 16 * initialSpeed2 * elapsedTime2 + 32 * (acceleration * elapsedTime2 * elapsedTime2 / 2);
			elapsedTime2 += deltaTimeSeconds;
			initialSpeed2 = acceleration * elapsedTime2;
		}

		// the bird rotates depending on the direction it goes
		if (inertion == true) {
			if (angularStep2 < 0.05f) {
				angularStep2 += deltaTimeSeconds / 6;
			}
		}
		else {
			if (angularStep2 > -0.05f) {
				angularStep2 -= deltaTimeSeconds / 6;
			}
		}
	}

	resize = glm::mat3(1) * Transform2D::Scale(0.66f, 0.66f);
	glm::mat3 rotation = Transform2D::Rotate(angularStep2);

	modelMatrix = visMatrix * resize * rotation;
	modelMatrix *= Transform2D::Translate(posx - 40, posy + translateY - 40) * Transform2D::Scale(5, 3) * Transform2D::Rotate(angularStep);
	RenderMesh2D(meshes["bird_wing"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * resize * rotation;
	modelMatrix *= Transform2D::Translate(posx + 24, posy + 12 + translateY) * Transform2D::Scale(6, 6);
	RenderMesh2D(meshes["bird_iris"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * resize * rotation;
	modelMatrix *= Transform2D::Translate(posx + 20, posy + 10 + translateY) * Transform2D::Scale(10, 10);
	RenderMesh2D(meshes["bird_eye"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * resize * rotation;
	modelMatrix *= Transform2D::Translate(posx + 28, posy - 5 + translateY) * Transform2D::Scale(4, 1);
	RenderMesh2D(meshes["bird_mouth"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * resize * rotation;
	modelMatrix *= Transform2D::Translate(posx + 28, posy - 10 + translateY) * Transform2D::Scale(4, 2);
	RenderMesh2D(meshes["bird_beak"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * resize * rotation;
	modelMatrix *= Transform2D::Translate(posx, posy + translateY) * Transform2D::Scale(30, 30);
	RenderMesh2D(meshes["bird_head"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * resize * rotation;
	modelMatrix *= Transform2D::Translate(posx - 60, posy - 50 + translateY) * Transform2D::Scale(70, 40);
	RenderMesh2D(meshes["bird_body"], shaders["VertexColor"], modelMatrix);

	translateY2 = translateY;

	bird_head_y = 0.66f * (posy + translateY);
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// the bird jumps with SPACE and the game pauses with P
	if (gameOver == false) {
		if (key == GLFW_KEY_SPACE)
		{
			start = true;
			direction_up = true;
		}
		if (key == GLFW_KEY_P)
		{
			std::cout << endl << "<P>ause button pressed!" << endl;
			start = !start;
		}
	}
	// the game restarts with R
	if (key == GLFW_KEY_R)
	{
		std::cout << endl << "<R>estarting the game..." << endl;

		x_positions.erase(x_positions.begin(), x_positions.end());
		crazy_pipe.erase(crazy_pipe.begin(), crazy_pipe.end());
		grow.erase(grow.begin(), grow.end());
		down_pipe_length.erase(down_pipe_length.begin(), down_pipe_length.end());
		up_pipe_length.erase(up_pipe_length.begin(), up_pipe_length.end());

		down_pipe_length2.erase(down_pipe_length2.begin(), down_pipe_length2.end());
		up_pipe_length2.erase(up_pipe_length2.begin(), up_pipe_length2.end());

		Init();
	}

	// you can play the game with a bigger number of pipes per screen
	if (key == GLFW_KEY_X)
	{
		pipesNr++;
		std::cout << endl << "<R>estarting the game..." << endl;

		x_positions.erase(x_positions.begin(), x_positions.end());
		crazy_pipe.erase(crazy_pipe.begin(), crazy_pipe.end());
		grow.erase(grow.begin(), grow.end());
		down_pipe_length.erase(down_pipe_length.begin(), down_pipe_length.end());
		up_pipe_length.erase(up_pipe_length.begin(), up_pipe_length.end());

		down_pipe_length2.erase(down_pipe_length2.begin(), down_pipe_length2.end());
		up_pipe_length2.erase(up_pipe_length2.begin(), up_pipe_length2.end());

		Init();
	}

	// or you can play it with a smaller number of pipes per screen
	if (key == GLFW_KEY_Z)
	{
		if (pipesNr > 2) {
			pipesNr--;
		}
		std::cout << endl << "<R>estarting the game..." << endl;

		x_positions.erase(x_positions.begin(), x_positions.end());
		crazy_pipe.erase(crazy_pipe.begin(), crazy_pipe.end());
		grow.erase(grow.begin(), grow.end());
		down_pipe_length.erase(down_pipe_length.begin(), down_pipe_length.end());
		up_pipe_length.erase(up_pipe_length.begin(), up_pipe_length.end());

		down_pipe_length2.erase(down_pipe_length2.begin(), down_pipe_length2.end());
		up_pipe_length2.erase(up_pipe_length2.begin(), up_pipe_length2.end());

		Init();
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}