#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
# define PI 3.14159265358979323846

typedef struct {
	float x, y;
	float scaleX, scaleY;
}Wall;

typedef struct {
	Wall upWall;
	Wall downWall;
	bool pastBy;
}WallPair;

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void OnKeyPress(int key, int mods) override;
	Mesh* CreateMesh(const char *name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);


protected:
	glm::ivec2 resolution;
	glm::mat3 modelMatrix = glm::mat3(1);;

	float translateX, translateY = 0;
	float scaleX, scaleY = 1;
	float angle;
	float birdScale;
	float birdX;
	float birdY;
	float birdRadius = 5.0f;
	const float maxTilt = PI / 5;
	const float minTilt = -maxTilt;
	float upAngle;
	float downAngle = (float)PI;


	float g;
	float impulse; 
	float acc;
	
	WallPair walls[2];
	float wallSpeed;

	float grassScaleX, grassScaleY;

	bool gameover;
	unsigned int score;
	unsigned int counter;

	GLenum polygonMode;
};
