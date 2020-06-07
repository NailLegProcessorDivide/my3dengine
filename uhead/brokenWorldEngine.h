#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
//#include "inputHelper.h"

class Logger {
public:
	FILE* errorLog;
	FILE* warningLog;
	FILE* infoLog;
	int loggingVerboseness;

	Logger();
	void const logErr(int, const char*, ...);
	void setErr(FILE*);
	void const logWarn(int, const char*, ...);
	void setWarn(FILE*);
	void const logInfo(int, const char*, ...);
	void setInfo(FILE*);
	void setLoggingVerboseness(int);
};
extern Logger logger;

class BrokenWorldEngine {
private:

public:
	virtual ~BrokenWorldEngine();
	BrokenWorldEngine();
	BrokenWorldEngine(int width, int height, const char* title);
	BrokenWorldEngine(int width, int height);
	BrokenWorldEngine(const char* title);
	static void init();
	static void terminate();
	static BrokenWorldEngine* createBWE();
	static BrokenWorldEngine* createBWE(int width, int height, const char* title);
	static BrokenWorldEngine* createBWE(int width, int height);
	static BrokenWorldEngine* createBWE(const char* title);
	static void deleteBWE(BrokenWorldEngine*);

	virtual void destroyWindow();
	virtual void setWindowName(const char* title);

	virtual void filli(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	virtual void fillf(float r, float g, float b, float a);
	virtual void filli(uint8_t r, uint8_t g, uint8_t b);
	virtual void fillf(float r, float g, float b);
	virtual void filli(uint8_t r);
	virtual void fillf(float r);

	virtual int createModel(const std::vector<glm::vec3>&, int);
	virtual int createModel(float*, int);
	virtual int createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, int);
	virtual int createModel(float*, float*, int, bool normals);
	virtual int createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int);
	virtual int createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int);
	virtual int createModel(float*, float*, float*, int);
	virtual bool deleteModel(int model);

	virtual int bindModel(int modelID);
	virtual bool deleteModelBindingData(int bindingID);
	virtual int CreateModelInstance(int bindingInfoID);
	virtual int CreateModelInstance(int bindingInfoID, bool draw);
	//virtual int CreateModelInstance();

	virtual void draw();

	virtual int genShader(const char* vs, const char* fs);
	virtual int genShader(std::string& vs, std::string& fs);
	virtual void bindShader(int shaderID);

	virtual int getCam();
	virtual void setCam(int);

	virtual void setCamPos(float, float, float);
	virtual void setCamPos(glm::vec3);
	virtual glm::vec3 getCamPos();

	virtual void setCamRot(float, float, float);
	virtual void setCamRot(glm::vec3);
	virtual glm::vec3 getCamRot();

	virtual void setCamFov(float);
	virtual float getCamFov();

	virtual void setCamAspect(float);
	virtual float getCamAspect();


	virtual void setCamFarPlane(float);
	virtual float getCamFarPlane();
	virtual void setCamNearPlane(float);
	virtual float getCamNearPlane();

	virtual bool windowCloseRequested();
	virtual void pollEvents();
};