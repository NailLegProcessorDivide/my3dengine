#pragma once
#include "GL/glew.h"
#include "GLFW/GLFW3.h"

#include "inputHelperGLFW.h"
#include "brokenWorldEngine.h"
#include "bwestorage.h"

class WorldEngineGL;
class BweWindow;
class Model;
class BweTexture;
struct ModelBindingData;
class ModelInstance;
class Shader;
class Camera;

class BweWindow {
public:
	GLFWwindow* window;
	EnumContainer<ModelBindingData> modelbindings;
	EnumContainer<ModelInstance> modelInstances;
	EnumContainer<Shader> shaders;
	EnumContainer<Camera> cameras;
	int activeCamera;
	int activeShader;
public:
	BweWindow(int width, int height, const char* title);
};

class WorldEngineGL: public BrokenWorldEngine {
private:
	BweWindow window;
	EnumContainer<Model> models;
public:
	~WorldEngineGL();

	//window stuff
	WorldEngineGL();
	WorldEngineGL(int width, int height, const char* title);
	WorldEngineGL(int width, int height);
	WorldEngineGL(const char* title);

	void init();

	void destroyWindow();
	void setWindowName(const char* title);

	void filli(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void fillf(float r, float g, float b, float a);
	void filli(uint8_t r, uint8_t g, uint8_t b);
	void fillf(float r, float g, float b);
	void filli(uint8_t r);
	void fillf(float r);

	KeyState getKey(Key);

	int createModel(const std::vector<glm::vec3>&, int);
	int createModel(float*, int);
	int createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, int);
	int createModel(float*, float*, int, bool normals);
	int createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int);
	int createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int);
	int createModel(float*, float*, float*, int);
	bool deleteModel(int);

	int bindModel(int modelID);
	bool deleteModelBindingData(int modbinid);
	int CreateModelInstance(int bindingInfoIDD);
	int CreateModelInstance(int bindingInfoID, bool draw);

	int genShader(const char* vs, const char* fs);
	int genShader(std::string& vs, std::string& fs);
	void bindShader(int shaderID);

	void draw();

	int getCam();
	void setCam(int);

	void setCamPos(float, float, float);
	void setCamPos(glm::vec3);
	glm::vec3 getCamPos();

	void setCamRot(float, float, float);
	void setCamRot(glm::vec3);
	glm::vec3 getCamRot();

	void setCamFov(float);
	float getCamFov();

	void setCamAspect(float);
	float getCamAspect();


	void setCamFarPlane(float);
	float getCamFarPlane();
	void setCamNearPlane(float);
	float getCamNearPlane();

	bool windowCloseRequested();
	void pollEvents();
};


class Model {
public:
	Model(const std::vector<glm::vec3>&, int);
	Model(float*, int);
	Model(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, int);
	Model(float*, float*, int, bool normals);
	Model(const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int);
	Model(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int);
	Model(float*, float*, float*, int);
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> uv;
	std::vector<uint32_t> inds;
	int indexCount;
	int triangleCount;
	uint8_t modeltype;//na-na-na-na-inds-uv-norm-pos
};

class BweTexture {
public:

};

struct ModelBindingData
{
	GLuint atribArray;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	int triangleCount;
};

class ModelInstance {
public:
	ModelBindingData* bindingData;
	BweTexture* texture;
	bool toDraw;
	void bindData();
};

class Shader {
public:
	std::string vsfile;
	std::string fsfile;
	GLuint shaderID;
	GLuint uniformMSMID;
	void build(const char* vs, const char* fs);
	void build(std::string& vs, std::string& fs);
	void rebuild();
};

class Camera {
	glm::vec3 position;
	glm::vec3 rotation;
	float fov;
	float nearPlane;
	float farPlane;
	float aspectRatio;
	//glm::mat4 transformation;
public:
	Camera();
	Camera(glm::vec3 p, glm::vec3 r, float fov, float nearPlane, float farPlane, float aspectRatio);
	Camera(float p1, float p2, float p3, float r1, float r2, float r3, float fov, float nearPlane, float farPlane, float aspectRatio);
	glm::vec3 getPos();
	glm::vec3 getRot();
	float getFarPlane();
	float getNearPlane();
	float getFOV();
	float getAspectRatio();
	void setPos(glm::vec3);
	void setRot(glm::vec3);
	void setFarPlane(float);
	void setNearPlane(float);
	void setFOV(float);
	void setAspectRatio(float);
	glm::mat4& getTransform();
};