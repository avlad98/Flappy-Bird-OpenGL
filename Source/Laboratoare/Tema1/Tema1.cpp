#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include "Tema1.h"

using namespace std;

string birdName = "birdUp";

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

// Functia care genereaza valori random intre 0 si 1 pentru scalarea peretilor
float random()
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(0, 1);
	return dis(e);
}

bool collide(float birdX, float birdY, float birdRadius, float rectDownX, float rectDownY, float rectDownLX, float rectDownLY,
	float rectUpX, float rectUpY, float rectUpLX, float rectUpLY) {

	if ((birdX + birdRadius) >= rectDownX && 
		(birdX + birdRadius) <= (rectDownX + rectDownLX) &&
		(birdY + birdRadius) <= rectDownLY) {
		return true;
	}

	if ((birdX + birdRadius) >= rectUpX &&
		(birdX + birdRadius) <= (rectUpX + rectUpLX) &&
		(birdY + birdRadius) >= rectUpY) {
		return true;
	}

	return false;
}

bool pastBy(float birdX, float birdY, float birdRadius, float rectDownX, float rectDownY, float rectDownLX, float rectDownLY,
	float rectUpX, float rectUpY, float rectUpLX, float rectUpLY) {
	
	return birdX > rectDownX + rectDownLX;
}

void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);


	// Calculez variabilele in functie de rezolutia ecranului (acolo unde este posibil si necesar)
	birdScale = (float)resolution.y / 250;
	birdX = (float) resolution.x / 6;
	birdY = (float) resolution.y / 1.5 ;
	g = (float) resolution.y / 1.7  ;

	walls[0].downWall.x = resolution.x;
	walls[0].downWall.y = 0;
	walls[0].downWall.scaleX = (float)resolution.x / 21;
	walls[0].downWall.scaleY = (float)resolution.y / 9;
	walls[0].upWall.x = resolution.x;
	walls[0].upWall.y = (float)resolution.y / 1.5f;
	walls[0].upWall.scaleX = (float)resolution.x / 21;
	walls[0].upWall.scaleY = (float)resolution.y / 5;
	  
	walls[1].downWall.x = (float)resolution.x * 3 / 2;
	walls[1].downWall.y = 0;
	walls[1].downWall.scaleX = (float)resolution.x / 21;
	walls[1].downWall.scaleY = (float)resolution.y / 9;
	walls[1].upWall.x = (float)resolution.x * 3 / 2;
	walls[1].upWall.y = (float)resolution.y / 1.5f;
	walls[1].upWall.scaleX = (float)resolution.x / 21;
	walls[1].upWall.scaleY = (float)resolution.y / 5;

	wallSpeed = (float)resolution.x / 6;

	polygonMode = GL_FILL;

	// Definirea unui perete
	{
		glm::vec3 pipeColor = glm::vec3(0, 0, 1);
		// L = 3, l = 2
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 0), pipeColor),	//	1
			VertexFormat(glm::vec3(2, 0, 0), pipeColor),	//	2
			VertexFormat(glm::vec3(2, 3, 0), pipeColor),	//	3
			VertexFormat(glm::vec3(0, 3, 0), pipeColor),	//	4
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,
			0, 3, 2
		};

		meshes["rectangle"] = new Mesh("rectangle");
		meshes["rectangle"]->InitFromData(vertices, indices);
		Mesh *rectangle = CreateMesh("rectangle", vertices, indices);
	}

	// Definirea pasarii cu aripa in sus
	{
		vector<VertexFormat> vertices
		{
			//						X,	Y,	Z,			R	G	B
			VertexFormat(glm::vec3(-9, -5, 0), glm::vec3(1, 0, 0)),	//	0
			VertexFormat(glm::vec3(-2.46, -1.23, 0), glm::vec3(1, 0, 0)),	//	1
			VertexFormat(glm::vec3(-5, -0.64, 0), glm::vec3(1, 0, 0)),	//	2
			VertexFormat(glm::vec3(-5, -0.64, 0), glm::vec3(1, 1, 0)),	//	3
			VertexFormat(glm::vec3(1, -2, 0), glm::vec3(1, 1, 0)),	//	4
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 1, 0)),	//	5
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 1, 0)),	//	6
			VertexFormat(glm::vec3(2.09, 4, 0), glm::vec3(1, 1, 0)),	//	7
			VertexFormat(glm::vec3(-1.47, 2.1, 0), glm::vec3(1, 1, 0)),	//	8
			VertexFormat(glm::vec3(-1.47, 2.1, 0), glm::vec3(1, 1, 0)),	//	9
			VertexFormat(glm::vec3(-5, -0.64, 0), glm::vec3(1, 1, 0)),	//	10
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 1, 0)),	//	11
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 0, 0)),	//	12
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	13
			VertexFormat(glm::vec3(2.09, 4.1, 0), glm::vec3(1, 0, 0)),	//	14
			VertexFormat(glm::vec3(2.09, 4.1, 0), glm::vec3(1, 0, 0)),	//	15
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	16
			VertexFormat(glm::vec3(4, 4, 0), glm::vec3(1, 0, 0)),	//	17
			VertexFormat(glm::vec3(4, 4, 0), glm::vec3(1, 0, 0)),	//	18
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	19
			VertexFormat(glm::vec3(4.66, 3.63, 0), glm::vec3(1, 0, 0)),	//	20
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	21
			VertexFormat(glm::vec3(5.44, 1.4, 0), glm::vec3(1, 0, 0)),	//	22
			VertexFormat(glm::vec3(4.66, 3.63, 0), glm::vec3(1, 0, 0)),	//	23

			// Aripa sus
			VertexFormat(glm::vec3(-4, 0.64, 0), glm::vec3(0, 1, 0)),	//	24
			VertexFormat(glm::vec3(1, 2, 0), glm::vec3(0, 1, 0)),	//	25
			VertexFormat(glm::vec3(-4.66, 4.24, 0), glm::vec3(0, 1, 0)),	//	26
			VertexFormat(glm::vec3(-4, 0.64, 0), glm::vec3(0, 1, 0)),	//	27
			VertexFormat(glm::vec3(-4.66, 4.24, 0), glm::vec3(0, 1, 0)),	//	28
			VertexFormat(glm::vec3(-8.67, 7.24, 0), glm::vec3(0, 1, 0)),	//	29
		};

		vector<unsigned short> indices =
		{
			0,1,2,
			3,4,5,
			6,7,8,
			9,10,11,
			12,13,14,
			15,16,17,
			18,19,20,
			21,22,23,
			24,25,26,
			27,28,29,
		};

		meshes["birdUp"] = new Mesh("birdUp");
		meshes["birdUp"]->InitFromData(vertices, indices);
		Mesh *bird = CreateMesh("birdUp", vertices, indices);
	}

	// Definirea pasarii cu aripa in jos
	{
		vector<VertexFormat> vertices
		{
			//						X,	Y,	Z,			R	G	B
			VertexFormat(glm::vec3(-9, -5, 0), glm::vec3(1, 0, 0)),	//	0
			VertexFormat(glm::vec3(-2.46, -1.23, 0), glm::vec3(1, 0, 0)),	//	1
			VertexFormat(glm::vec3(-5, -0.64, 0), glm::vec3(1, 0, 0)),	//	2
			VertexFormat(glm::vec3(-5, -0.64, 0), glm::vec3(1, 1, 0)),	//	3
			VertexFormat(glm::vec3(1, -2, 0), glm::vec3(1, 1, 0)),	//	4
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 1, 0)),	//	5
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 1, 0)),	//	6
			VertexFormat(glm::vec3(2.09, 4, 0), glm::vec3(1, 1, 0)),	//	7
			VertexFormat(glm::vec3(-1.47, 2.1, 0), glm::vec3(1, 1, 0)),	//	8
			VertexFormat(glm::vec3(-1.47, 2.1, 0), glm::vec3(1, 1, 0)),	//	9
			VertexFormat(glm::vec3(-5, -0.64, 0), glm::vec3(1, 1, 0)),	//	10
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 1, 0)),	//	11
			VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 0, 0)),	//	12
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	13
			VertexFormat(glm::vec3(2.09, 4.1, 0), glm::vec3(1, 0, 0)),	//	14
			VertexFormat(glm::vec3(2.09, 4.1, 0), glm::vec3(1, 0, 0)),	//	15
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	16
			VertexFormat(glm::vec3(4, 4, 0), glm::vec3(1, 0, 0)),	//	17
			VertexFormat(glm::vec3(4, 4, 0), glm::vec3(1, 0, 0)),	//	18
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	19
			VertexFormat(glm::vec3(4.66, 3.63, 0), glm::vec3(1, 0, 0)),	//	20
			VertexFormat(glm::vec3(4, 2.35, 0), glm::vec3(1, 0, 0)),	//	21
			VertexFormat(glm::vec3(5.44, 1.4, 0), glm::vec3(1, 0, 0)),	//	22
			VertexFormat(glm::vec3(4.66, 3.63, 0), glm::vec3(1, 0, 0)),	//	23

			//Aripa jos
			VertexFormat(glm::vec3(-6, 0.5, 0), glm::vec3(0, 1, 0)),	//	24
			VertexFormat(glm::vec3(1.5, 2, 0), glm::vec3(0, 1, 0)),	//	25
			VertexFormat(glm::vec3(-3, -1.97, 0), glm::vec3(0, 1, 0)),	//	26
			VertexFormat(glm::vec3(2.85, -9.16, 0), glm::vec3(0, 1, 0)),	//	27
		};

		vector<unsigned short> indices =
		{
			0,1,2,
			3,4,5,
			6,7,8,
			9,10,11,
			12,13,14,
			15,16,17,
			18,19,20,
			21,22,23,
			24,26,25,
			25,26,27
		};

		meshes["birdDown"] = new Mesh("birdDown");
		meshes["birdDown"]->InitFromData(vertices, indices);
		Mesh *bird = CreateMesh("birdDown", vertices, indices);
	}

	// Definirea ierbii
	{
		glm::vec3 grassColor = glm::vec3(0, 1, 0);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-3, 0, 0), grassColor),	//	0
			VertexFormat(glm::vec3(-2, 0, 0), grassColor),	//	1
			VertexFormat(glm::vec3(-3, 2, 0), grassColor),	//	2
			VertexFormat(glm::vec3(0, 1, 0), grassColor),	//	3
			VertexFormat(glm::vec3(-1, 2, 0), grassColor),	//	4
			VertexFormat(glm::vec3(2, 0, 0), grassColor),	//	5
			VertexFormat(glm::vec3(1, 2, 0), grassColor),	//	6
			VertexFormat(glm::vec3(3, 0, 0), grassColor),	//	7
			VertexFormat(glm::vec3(3, 2, 0), grassColor)	//	8
		};

		vector<unsigned short> indices =
		{
			0,1,2,
			1,3,4,
			1,5,3,
			3,5,6,
			5,7,8
		};

		meshes["grass"] = new Mesh("grass");
		meshes["grass"]->InitFromData(vertices, indices);
		Mesh *rectangle = CreateMesh("grass", vertices, indices);
	}
}

Mesh* Tema1::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindVertexArray(0);
	CheckOpenGLError();
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Tema1::FrameStart()
{
	glClearColor(0, 0.7, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	// Verific daca s-a terminat jocul
	if (gameover) {
		cout << "Game over" << endl;
		cout << "Your score: " << score << endl;
		exit(0);
	}
	else {
		glLineWidth(3);
		glPointSize(5);
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

		/* Bird */
		angle -= downAngle * deltaTimeSeconds;
		angle += upAngle * deltaTimeSeconds;
		if (angle < minTilt) angle = minTilt;
		if (angle > maxTilt) angle = maxTilt;
		upAngle -= upAngle > 0 ? downAngle / 8 : 0;

		// Schimba aripa sus / jos cand se ridica pasarea
		if (impulse  > 0)
			if(counter++ % 8 == 0)
				birdName = birdName == "birdUp" ? "birdDown" : "birdUp";
		else 
			birdName = "birdUp";

		// Randez pasarea
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(birdX, birdY);
		modelMatrix *= Transform2D::Scale(birdScale, birdScale);
		modelMatrix *= Transform2D::Rotate(angle);
		RenderMesh2D(meshes[birdName], shaders["VertexColor"], modelMatrix);
		birdRadius = 2 * birdScale;

		// Aplic gravitatia si impulsul de ridicare pe pasare
		acc += (impulse - g) * deltaTimeSeconds;
		impulse -= impulse > 0 ? g / 10 : 0;

		birdY = (float)resolution.y / 1.5;
		birdY += acc;


		/* Walls */

		//Down wall 0
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(walls[0].downWall.x, walls[0].downWall.y);
		modelMatrix *= Transform2D::Scale(walls[0].downWall.scaleX, walls[0].downWall.scaleY);
		RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

		//Up wall 0
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(walls[0].upWall.x, walls[0].upWall.y);
		modelMatrix *= Transform2D::Scale(walls[0].upWall.scaleX, walls[0].upWall.scaleY);
		RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

		// Verific daca peretii au iesit din ecran si ii repozitionez la capatul opus
		if (walls[0].downWall.x > -2 * walls[0].downWall.scaleX) {
			walls[0].downWall.x -= wallSpeed * deltaTimeSeconds;
			walls[0].upWall.x -= wallSpeed * deltaTimeSeconds;
		} 
		else {
			walls[0].downWall.x = resolution.x;
			walls[0].downWall.scaleY = (float)resolution.y / 60 + random() * resolution.y / 10;
			walls[0].upWall.x = resolution.x;
			walls[0].upWall.y = (float)resolution.y / 3.5 + walls[0].downWall.scaleY * 3;
			walls[0].upWall.scaleY = (float)resolution.y / 4 - walls[0].downWall.scaleY;
			walls[0].pastBy = false;
		}

		//Down wall 1
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(walls[1].downWall.x, walls[1].downWall.y);
		modelMatrix *= Transform2D::Scale(walls[1].downWall.scaleX, walls[1].downWall.scaleY);
		RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

		//Up wall 1
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(walls[1].upWall.x, walls[1].upWall.y);
		modelMatrix *= Transform2D::Scale(walls[1].upWall.scaleX, walls[1].upWall.scaleY);
		RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

		if (walls[1].downWall.x > -2 * walls[0].downWall.scaleX) {
			walls[1].downWall.x -= wallSpeed * deltaTimeSeconds;
			walls[1].upWall.x -= wallSpeed * deltaTimeSeconds;
		}
		else {
			walls[1].downWall.x = resolution.x;
			walls[1].downWall.scaleY = (float)resolution.y / 60 + random() * resolution.y / 10;
			walls[1].upWall.x = resolution.x;
			walls[1].upWall.y = (float)resolution.y / 3.5 + walls[1].downWall.scaleY * 3;
			walls[1].upWall.scaleY = (float)resolution.y / 4 - walls[1].downWall.scaleY;
			walls[1].pastBy = false;
		}

		// Verific daca pasarea a coborat mai jos de axa OX
		gameover = birdY <= 0;

		// Verific coliziunea dintre pasare si cei 4 pereti
		if (collide(birdX, birdY, birdScale * birdRadius,
			walls[0].downWall.x, walls[0].downWall.y, walls[0].downWall.scaleX * 2, walls[0].downWall.scaleY * 3,
			walls[0].upWall.x, walls[0].upWall.y, walls[0].upWall.scaleX * 2, walls[0].upWall.scaleY * 3)) {
			gameover = true;
		}

		if (collide(birdX, birdY, birdScale * birdRadius,
			walls[1].downWall.x, walls[1].downWall.y, walls[1].downWall.scaleX * 2, walls[1].downWall.scaleY * 3,
			walls[1].upWall.x, walls[1].upWall.y, walls[1].upWall.scaleX * 2, walls[1].upWall.scaleY * 3)) {
			gameover = true;
		}

		// Variabila cu care cresc viteza peretilor
		float levelSpeed = (float)resolution.x / 15;

		// Verific daca pasarea a trecut de o pereche de pereti si cresc scorul
		if (!walls[0].pastBy &&	pastBy(birdX, birdY, birdScale * birdRadius,
				walls[0].downWall.x, walls[0].downWall.y, walls[0].downWall.scaleX * 2, walls[0].downWall.scaleY * 3,
				walls[0].upWall.x, walls[0].upWall.y, walls[0].upWall.scaleX * 2, walls[0].upWall.scaleY * 3)) {

				walls[0].pastBy = true;
				score++;
				cout << "Score: " << score << endl;
				if (score % 3 == 0) wallSpeed += levelSpeed;
		}

		if (!walls[1].pastBy &&	pastBy(birdX, birdY, birdScale * birdRadius,
			walls[1].downWall.x, walls[1].downWall.y, walls[1].downWall.scaleX * 2, walls[1].downWall.scaleY * 3,
			walls[1].upWall.x, walls[1].upWall.y, walls[1].upWall.scaleX * 2, walls[1].upWall.scaleY * 3)) {

			walls[1].pastBy = true;
			score++;
			cout << "Score: " << score << endl;
			if (score % 3 == 0) wallSpeed += levelSpeed;
		}

		/* Grass */
		grassScaleX = (float)resolution.x / 20;
		grassScaleY = (float)resolution.y / 15;

		// Randez iarba pe tot ecranul folosind un singur mesh la pozitii diferite
		for (int i = 0; i < 9; i++) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(3 * i *  grassScaleX, 0);
			modelMatrix *= Transform2D::Scale(grassScaleX, grassScaleY);
			RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// Pasarea sare si se inclina in sus cand apas SPACE
	if (key == GLFW_KEY_SPACE) {
		impulse = 3 * g;
		upAngle = 4 * downAngle;
	}
}
