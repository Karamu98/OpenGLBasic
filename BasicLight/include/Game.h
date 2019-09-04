#ifndef __GAME_H__
#define __GAME_H__


#include "Application.h"

#include "Cube.h"
#include "Pyramid.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

class Game : public Application
{
public:
	Game();
	~Game();

	bool OnCreate() override;
	void Update(float a_deltaTime) override;
	void Draw() override;
	void Destroy() override;

private:
	void ImGuiDraw();
	void ResizeFBO(float a_width, float a_height);
	void Screenshot();
private:
	bool isWireframe;
	float specularAmount;
	float gammaCorrection;

	unsigned int defaultFBO;
	unsigned int defaultColourTex;
	unsigned int depthRender;
	float fboWidth, fboHeight;
	
	std::shared_ptr<Shader> simpleShader;
	std::shared_ptr<Shader> lightShader;
	std::shared_ptr<Shape> shape;
	std::shared_ptr<Cube> light;
	std::shared_ptr<Camera> cam;
	std::shared_ptr<Texture> newTexture;
	glm::vec3 lightColour;

	// Cache
	unsigned int shaderProgram, lightProgram;
	bool screenshot;
	glm::vec2 viewPortSize;
};

#endif