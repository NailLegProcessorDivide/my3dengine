#include "brokenWorldEngine.h"

#include "bwegl.h"

BrokenWorldEngine::BrokenWorldEngine(){}
BrokenWorldEngine::BrokenWorldEngine(int width, int height, const char* title) {}
BrokenWorldEngine::BrokenWorldEngine(int width, int height) {}
BrokenWorldEngine::BrokenWorldEngine(const char* title) {}
BrokenWorldEngine::~BrokenWorldEngine(){}

void BrokenWorldEngine::destroyWindow() {}
bool BrokenWorldEngine::windowCloseRequested() {return true;}
void BrokenWorldEngine::setWindowName(const char* title) {}

void BrokenWorldEngine::filli(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {}
void BrokenWorldEngine::fillf(float r, float g, float b, float a) {}
void BrokenWorldEngine::filli(uint8_t r, uint8_t g, uint8_t b) {}
void BrokenWorldEngine::fillf(float r, float g, float b) {}
void BrokenWorldEngine::filli(uint8_t r) {}
void BrokenWorldEngine::fillf(float r) {}

void BrokenWorldEngine::pollEvents() {}

void BrokenWorldEngine::draw() {}

int BrokenWorldEngine::createModel(const std::vector<glm::vec3>&, int) { return -1; }
int BrokenWorldEngine::createModel(float*, int) { return -1; }
int BrokenWorldEngine::createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, int) { return -1; }
int BrokenWorldEngine::createModel(float*, float*, int, bool normals) { return -1; }
int BrokenWorldEngine::createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int) { return -1; }
int BrokenWorldEngine::createModel(const std::vector<glm::vec3>&, const std::vector<glm::vec3>&, const std::vector<glm::vec2>&, int) { return -1; }
int BrokenWorldEngine::createModel(float*, float*, float*, int) { return -1; }
bool BrokenWorldEngine::deleteModel(int model) { return false; }

int BrokenWorldEngine::bindModel(int model) { return -1; }
bool BrokenWorldEngine::deleteModelBindingData(int bindingID) { return false; }
int BrokenWorldEngine::CreateModelInstance(int bindingInfoID) { return -1; }
int BrokenWorldEngine::CreateModelInstance(int bindingInfoID, bool draw) { return -1; }

int BrokenWorldEngine::genShader(const char* vs, const char* fs) { return -1; }
int BrokenWorldEngine::genShader(std::string& vs, std::string& fs) { return -1; }
void BrokenWorldEngine::bindShader(int sh) {}

int BrokenWorldEngine::getCam() {return -1;}
void BrokenWorldEngine::setCam(int cam) {}

void BrokenWorldEngine::setCamPos(float x, float y, float z) {}
void BrokenWorldEngine::setCamPos(glm::vec3 npos) {}
glm::vec3 BrokenWorldEngine::getCamPos() {return glm::vec3(0);}

void BrokenWorldEngine::setCamRot(float x, float y, float z) {}
void BrokenWorldEngine::setCamRot(glm::vec3 nrot) {}
glm::vec3 BrokenWorldEngine::getCamRot() {return glm::vec3(0);}

void BrokenWorldEngine::setCamFov(float nfov) {}
float BrokenWorldEngine::getCamFov() {return 0;}

void BrokenWorldEngine::setCamAspect(float asp) {}
float BrokenWorldEngine::getCamAspect() {return 0;}

void BrokenWorldEngine::setCamFarPlane(float fp){}
float BrokenWorldEngine::getCamFarPlane(){return 0;}
void BrokenWorldEngine::setCamNearPlane(float np){}
float BrokenWorldEngine::getCamNearPlane(){return 0;}