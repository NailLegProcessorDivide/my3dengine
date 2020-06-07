#include "brokenWorldEngine.h"
#include "bwegl.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <vector>
#include <cstdarg>
#include <fstream>
#include <sstream>


uint32_t numberOfEngineInstances = 0;

Logger logger = Logger();
Logger::Logger():errorLog(stderr), infoLog(stdout), warningLog(stdout){
	loggingVerboseness = 0;
	errorLog = stderr;
	infoLog = stdout;
	warningLog = stdout;
}

BrokenWorldEngine* BrokenWorldEngine::createBWE() {
	return new WorldEngineGL();
}
BrokenWorldEngine* BrokenWorldEngine::createBWE(const char* title) {
	return new WorldEngineGL(title);
}
BrokenWorldEngine* BrokenWorldEngine::createBWE(int width, int height) {
	return new WorldEngineGL(width, height);
}
BrokenWorldEngine* BrokenWorldEngine::createBWE(int width, int height, const char* title) {
	return new WorldEngineGL(width, height, title);
}

void BrokenWorldEngine::deleteBWE(BrokenWorldEngine* bwe) {
	delete bwe;
}
void BrokenWorldEngine::init() {
	if (!glfwInit()) {
		logger.logErr(9, "failed to initialize GLFW\n");
	}
	else {
		logger.logInfo(2, "successfuly initialized GLFW\n");
	}
}
void BrokenWorldEngine::terminate() {
	logger.logInfo(2, "glfw terminated");
	glfwTerminate();
}

WorldEngineGL::WorldEngineGL() : window(256, 240, "My Broken Worlds Game!") {}
WorldEngineGL::WorldEngineGL(const char* title) : window(256, 240, title) {}
WorldEngineGL::WorldEngineGL(int width, int height) : window(width, height, "My Broken Worlds Game!") {}
WorldEngineGL::WorldEngineGL(int width, int height, const char* title) : window(width, height, title) {}

void WorldEngineGL::init() {
}

WorldEngineGL::~WorldEngineGL() {
	destroyWindow();
}


void WorldEngineGL::destroyWindow() {
	logger.logInfo(2, "window removed successfuly\n");
	glfwDestroyWindow(window.window);
	EnumContainer<ModelBindingData>* mbds = &window.modelbindings;
	bool stopLoop = false;
	int i = 0;
	while(!stopLoop) {
		int id = mbds->getCreated(i);
		if (id == -1) {
			stopLoop = true;
			break;
		}
		else {
			deleteModel(id);
		}
		i++;
	}
}

void WorldEngineGL::setWindowName(const char* title) {
	logger.logInfo(1, "window name changed to %s.\n", title);
	glfwSetWindowTitle(window.window, title);
}

void WorldEngineGL::filli(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void WorldEngineGL::fillf(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void WorldEngineGL::filli(uint8_t r, uint8_t g, uint8_t b) {
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 0);
}

void WorldEngineGL::fillf(float r, float g, float b) {
	glClearColor(r, g, b, 0);
}

void WorldEngineGL::filli(uint8_t r) {
	glClearColor(r, r, r, 0);
}

void WorldEngineGL::fillf(float r) {
	glClearColor(r, r, r, 0);
}


int WorldEngineGL::createModel(const std::vector<glm::vec3>& pos, int count) {
	return models.add(new Model(pos, count));
}
int WorldEngineGL::createModel(float* pos, int count) {
	return models.add(new Model(pos, count));
}
int WorldEngineGL::createModel(const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& normals, int count) {
	return models.add(new Model(pos, normals, count));
}
int WorldEngineGL::createModel(float* pos, float* uvnorm, int count, bool normals) {
	return models.add(new Model(pos, uvnorm, count, normals));
}
int WorldEngineGL::createModel(const std::vector<glm::vec3>& pos, const std::vector<glm::vec2>& uvs, int count) {
	return models.add(new Model(pos, uvs, count));
}
int WorldEngineGL::createModel(const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs, int count) {
	return models.add(new Model(pos, normals, uvs, count));
}
int WorldEngineGL::createModel(float* pos, float* normals, float* uvs, int count) {
	return models.add(new Model(pos, normals, uvs, count));
}
bool WorldEngineGL::deleteModel(int modelID) {

	delete models.get(modelID);
	return models.remove(modelID);
}

bool WorldEngineGL::deleteModelBindingData(int bindingID){
	return false;
}

int WorldEngineGL::CreateModelInstance(int bindingInfoID) {
	return CreateModelInstance(bindingInfoID, false);
}

int WorldEngineGL::CreateModelInstance(int bindingInfoID, bool draw) {
	ModelInstance* mi = new ModelInstance();
	mi->bindingData = window.modelbindings.get(bindingInfoID);
	mi->toDraw = draw;
	return window.modelInstances.add(mi);
}

int WorldEngineGL::bindModel(int modelID) {
	Model* model = models.get(modelID);
	ModelBindingData* mbd = new ModelBindingData();
	mbd->triangleCount = model->triangleCount;
	glGenVertexArrays(1, &mbd->atribArray);
	logger.logInfo(1, "glGenVertexArrays(1, &mbd->atribArray);\n");
	glBindVertexArray(mbd->atribArray);
	logger.logInfo(1, "glBindVertexArray(%i);\n", mbd->atribArray);
	glGenBuffers(1, &mbd->vertexBuffer);
	logger.logInfo(1, "glGenBuffers(1, &mbd->vertexBuffer);\n");
	glBindBuffer(GL_ARRAY_BUFFER, mbd->vertexBuffer);
	logger.logInfo(1, "glBindBuffer(GL_ARRAY_BUFFER, %i);\n", mbd->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mbd->triangleCount * 3, model->pos.data(), GL_STATIC_DRAW);
	logger.logInfo(1, "glBufferData(GL_ARRAY_BUFFER, %i, model->pos.data(), GL_STATIC_DRAW);\n", sizeof(glm::vec3) * mbd->triangleCount * 3);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	logger.logInfo(1, "glVertexAttribPointer\n");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	logger.logInfo(1, "glBindBuffer(GL_ARRAY_BUFFER, 0);\n");
	glBindVertexArray(0);
	logger.logInfo(1, "glBindVertexArray(0);\n");
	//logger.logInfo(1, "successfully bound model %i\n", modelID);
	return window.modelbindings.add(mbd);
}

void WorldEngineGL::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	logger.logInfo(1, "glclear\n");
	int i = 0;
	int instID = window.modelInstances.getCreated(i++);
	glm::mat4 cammat = window.cameras.get(window.activeCamera)->getTransform();
	while (instID != -1) {
		ModelInstance* modinst = window.modelInstances.get(instID);
		if (modinst->toDraw) {
			//glm::vec4 modmat = glm::translate(glm::mat4(1), modinst->

			glUniformMatrix4fv(window.shaders.get(window.activeShader)->uniformMSMID, 1, GL_FALSE, &cammat[0][0]);

			modinst->bindData();
			glEnableVertexAttribArray(0);
			logger.logInfo(1, "glEnableVertexAttribArray(0);\n");
			//logger.logInfo(2, "draw inst, %i, %i\n", instID, i);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3 * modinst->bindingData->triangleCount);
			logger.logInfo(1, "glDrawArrays(GL_TRIANGLES, 0, %i);\n", 3 * modinst->bindingData->triangleCount);
			glDisableVertexAttribArray(0);
			logger.logInfo(1, "glDisableVertexAttribArray(0);\n");
			glBindVertexArray(0);
			logger.logInfo(1, "glBindVertexArray(0);\n");
			instID = window.modelInstances.getCreated(i++);
		}
	}
	glfwSwapBuffers(window.window);
}

KeyState WorldEngineGL::getKey(Key key) {
	return (KeyState) glfwGetKey(window.window, (int)key);
}

bool WorldEngineGL::windowCloseRequested() {
	return glfwWindowShouldClose(window.window) != 0;
}

void WorldEngineGL::pollEvents() {
	glfwPollEvents();
}

int WorldEngineGL::genShader(const char* vs, const char* fs) {
	Shader* s = new Shader();
	s->build(vs, fs);
	return window.shaders.add(s);
}

int WorldEngineGL::genShader(std::string& vs, std::string& fs) {
	Shader* s = new Shader();
	s->build(vs, fs);
	return window.shaders.add(s);
}

void WorldEngineGL::bindShader(int shaderID) {
	Shader* shad = window.shaders.get(shaderID);
	logger.logInfo(1, "shader program %i bound\n", shad->shaderID);
	glUseProgram(shad->shaderID);
	window.activeShader = shaderID;
}

int WorldEngineGL::getCam() {
	return window.activeCamera;
}
void WorldEngineGL::setCam(int cam) {
	window.activeCamera = cam;
}

void WorldEngineGL::setCamPos(float x, float y, float z) {
	window.cameras.get(window.activeCamera)->setPos(glm::vec3(x, y, z));
}
void WorldEngineGL::setCamPos(glm::vec3 npos) {
	window.cameras.get(window.activeCamera)->setPos(npos);
}
glm::vec3 WorldEngineGL::getCamPos() {
	return window.cameras.get(window.activeCamera)->getPos();
}

void WorldEngineGL::setCamRot(float x, float y, float z) {
	window.cameras.get(window.activeCamera)->setRot(glm::vec3(x, y, z));
}
void WorldEngineGL::setCamRot(glm::vec3 nrot) {
	window.cameras.get(window.activeCamera)->setRot(nrot);
}
glm::vec3 WorldEngineGL::getCamRot() {
	return window.cameras.get(window.activeCamera)->getRot();
}

void WorldEngineGL::setCamFov(float nfov) {
	window.cameras.get(window.activeCamera)->setFOV(nfov);
}
float WorldEngineGL::getCamFov() {
	return window.cameras.get(window.activeCamera)->getFOV();
}

void WorldEngineGL::setCamAspect(float asp) {
	window.cameras.get(window.activeCamera)->setAspectRatio(asp);
}
float WorldEngineGL::getCamAspect() {
	return window.cameras.get(window.activeCamera)->getAspectRatio();
}

void WorldEngineGL::setCamFarPlane(float fp){
	window.cameras.get(window.activeCamera)->setFarPlane(fp);
}
float WorldEngineGL::getCamFarPlane(){
	return window.cameras.get(window.activeCamera)->getFarPlane();
}
void WorldEngineGL::setCamNearPlane(float np){
	window.cameras.get(window.activeCamera)->setNearPlane(np);
}
float WorldEngineGL::getCamNearPlane(){
	return window.cameras.get(window.activeCamera)->getNearPlane();
}

///////////////////LOGGER////////////////////////

void const Logger::logErr(int siverity, const char* error, ...) {
	if (errorLog == NULL || siverity < loggingVerboseness) {
		return;
	}
	fprintf(errorLog, "ERROR: ");
	va_list args;
	va_start(args, error);
	vfprintf(errorLog, error, args);
	va_end(args);
}

void const Logger::logWarn(int siverity, const char* error, ...) {
	if (warningLog == NULL || siverity < loggingVerboseness) {
		return;
	}
	fprintf(warningLog, "WARNING: ");
	va_list args;
	va_start(args, error);
	vfprintf(warningLog, error, args);
	va_end(args);
}

void const Logger::logInfo(int siverity, const char* error, ...) {
	if (infoLog == NULL || siverity < loggingVerboseness) {
		return;
	}
	fprintf(infoLog, "INFO: ");
	va_list args;
	va_start(args, error);
	vfprintf(infoLog, error, args);
	va_end(args);
}

void Logger::setErr(FILE* errFile) {
	errorLog = errFile;
}

void Logger::setWarn(FILE* warnFile) {
	warningLog = warnFile;
}

void Logger::setInfo(FILE* infoFile) {
	infoLog = infoFile;
}

void Logger::setLoggingVerboseness(int verboseness) {
	loggingVerboseness = verboseness;
}


////////////////////////////////MODEL//////////////////////////////////////////

Model::Model(const std::vector<glm::vec3>& _pos, int count):modeltype(0x01){
	int numberToUse = count;
	if (count != _pos.size()) {
		numberToUse = std::min((int)_pos.size(), count);
	}
	if (count != _pos.size()) {
		logger.logWarn(7, "Model given %i positions but given count of %i. Using the frist %i\n",
			_pos.size(), count, numberToUse);
	}
	for (int i = 0; i < numberToUse; i++) {
		pos.push_back(_pos[i]);
	}
	indexCount = numberToUse;
	triangleCount = numberToUse / 3;
}

Model::Model(const std::vector<glm::vec3>& _pos, const std::vector<glm::vec3>& _normals,
				int count): modeltype(0x03) {
	int numberToUse = count;
	if (count != _pos.size() || count != _normals.size()) {
		numberToUse = std::min((int)_pos.size(), count);
		numberToUse = std::min((int)_normals.size(), numberToUse);
		logger.logWarn(7, "Model given %i positions, %i normals but given count of %i. Using the frist %i\n",
			_pos.size(), _normals.size(), count, numberToUse);
	}
	for (int i = 0; i < numberToUse; i++) {
		pos.push_back(_pos[i]);
		norm.push_back(_normals[i]);
	}
	indexCount = numberToUse;
	triangleCount = numberToUse / 3;
}

Model::Model(const std::vector<glm::vec3>& _pos, const std::vector<glm::vec2>& _uvs,
				int count) : modeltype(0x05) {
	int numberToUse = count;
	if (count != _pos.size() || count != _uvs.size()) {
		numberToUse = std::min((int)_pos.size(), count);
		numberToUse = std::min((int)_uvs.size(), numberToUse);
		logger.logWarn(7, "Model given %i positions, %i uvs but given count of %i. Using the frist %i\n",
			_pos.size(), _uvs.size(), count, numberToUse);
	}
	for (int i = 0; i < numberToUse; i++) {
		pos.push_back(_pos[i]);
		uv.push_back(_uvs[i]);
	}
	indexCount = numberToUse;
	triangleCount = numberToUse / 3;
}

Model::Model(const std::vector<glm::vec3>& _pos, const std::vector<glm::vec3>& _normals,
				const std::vector<glm::vec2>& _uvs, int count) : modeltype(0x07) {
	int numberToUse = count;
	if (count != _pos.size() || count != _uvs.size() || count != _normals.size()) {
		numberToUse = std::min((int)_pos.size(), count);
		numberToUse = std::min((int)_uvs.size(), numberToUse);
		numberToUse = std::min((int)_normals.size(), numberToUse);
		logger.logWarn(7, "Model given %i positions, %i uvs, %i normals but given count of %i. Using the frist %i\n",
			_pos.size(), _uvs.size(), _normals.size(), count, numberToUse);
	}
	for (int i = 0; i < numberToUse; i++) {
		pos.push_back(_pos[i]);
		uv.push_back(_uvs[i]);
	}
	indexCount = numberToUse;
	triangleCount = numberToUse / 3;
}

Model::Model(float* _pos, int count) : modeltype(0x01) {
	int numberToUse = count;
	for (int i = 0; i < numberToUse; i++) {
		pos.push_back(glm::vec3(_pos[3 * i], _pos[3 * i + 1], _pos[3 * i + 2]));
	}
	indexCount = numberToUse;
	triangleCount = numberToUse/3;
}

Model::Model(float* _pos, float* uvnorms, int count, bool normal) : modeltype(normal?0x03:0x05) {
	int numberToUse = count;
	for (int i = 0; i < numberToUse; i++) {
		pos.push_back(glm::vec3(_pos[3 * i], _pos[3 * i + 1], _pos[3 * i + 2]));
		if (normal) {
			norm.push_back(glm::vec3(uvnorms[3 * i], uvnorms[3 * i + 1], uvnorms[3 * i + 2]));
		}
		else {
			uv.push_back(glm::vec2(uvnorms[2 * i], uvnorms[2 * i + 1]));
		}
	}
	indexCount = numberToUse;
	triangleCount = numberToUse / 3;
}

Model::Model(float* _pos, float* norms, float* uvs, int count) : modeltype(0x07) {
	int numberToUse = count;
	for (int i = 0; i < numberToUse; i++) {
		pos.push_back(glm::vec3(_pos[3 * i], _pos[3 * i + 1], _pos[3 * i + 2]));
		norm.push_back(glm::vec3(norms[3 * i], norms[3 * i + 1], norms[3 * i + 2]));
		uv.push_back(glm::vec2(uvs[2 * i], uvs[2 * i + 1]));
	}
	indexCount = numberToUse;
	triangleCount = numberToUse / 3;
}

BweWindow::BweWindow(int width, int height, const char* title) : activeCamera(cameras.add(new Camera())) {
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL) {
		//fprintf();
		logger.logErr(9, "failed to create window\n");
	}
	if (glewInit() != GLEW_OK) {
		logger.logErr(9, "Failed to initialize GLEW\n");
		glfwDestroyWindow(window);
	}
	else {
		logger.logInfo(1, "GLEW initialized\n");
	}

	//glfwSwapInterval(16);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void ModelInstance::bindData() {
	glBindVertexArray(bindingData->atribArray);
	logger.logInfo(1, "glBindVertexArray(%i);\n", bindingData->atribArray);
	/*
	//logger.logInfo(1, "bound buffer %i, %i verts", bindingData->vertexBuffer, bindingData->triangleCount * 3);
	glBindBuffer(GL_ARRAY_BUFFER, bindingData->vertexBuffer);
	logger.logInfo(1, "glBindBuffer(GL_ARRAY_BUFFER, %i);\n", bindingData->vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	logger.logInfo(1, "glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);\n");
	*/
}

void Shader::build(const char* vs, const char* fs) {
	vsfile = vs;
	fsfile = fs;
	rebuild();
}
void Shader::build(std::string& vs, std::string& fs) {
	vsfile = vs;
	fsfile = fs;
	rebuild();
}

void Shader::rebuild() {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vsfile.c_str(), std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		logger.logErr(7, "unable to open %s \n", vsfile.c_str());
		//getchar();
		return ;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	
	std::ifstream FragmentShaderStream(fsfile.c_str(), std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else {
		logger.logErr(7, "Unable to open %s\n", vsfile.c_str());
		//getchar();
		return;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vsfile.c_str());
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		logger.logErr(7, "%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", vsfile.c_str());
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		logger.logErr(7, "%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		logger.logErr(7, "%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	shaderID = ProgramID;
	uniformMSMID = glGetUniformLocation(shaderID, "MSM");
	logger.logInfo(2, "created shader program with ID, %i", ProgramID);
}

Camera::Camera() :position(0), rotation(0), fov(1.22173f), nearPlane(0.1f), farPlane(100),
	aspectRatio(16.0f/9) {

}

Camera::Camera(glm::vec3 pos, glm::vec3 rot, float _fov = 1.22173f, float _nearPlane = 0.1f,
	float _farPlane = 100, float _aspectRatio = 16.0f/9) : position(pos), rotation(rot), fov(1.22173),
	nearPlane(0.1f), farPlane(100), aspectRatio(_aspectRatio) {

}

Camera::Camera(float p1, float p2, float p3, float r1, float r2, float r3, float _fov = 1.22173f,
	float _nearPlane = 0.1f, float _farPlane = 100, float _aspectRatio = 16.0f/9) : position(p1, p2, p3),
	rotation(r1, r2, r3), fov(1.22173),
	nearPlane(0.1f), farPlane(100), aspectRatio(_aspectRatio) {

}

glm::vec3 Camera::getPos() {
	return position;
}

void Camera::setPos(glm::vec3 _pos) {
	position = _pos;
}

glm::vec3 Camera::getRot() {
	return rotation;
}

void Camera::setRot(glm::vec3 _rot) {
	rotation = _rot;
}

float Camera::getFarPlane() {
	return farPlane;
}

void Camera::setFarPlane(float fp) {
	farPlane = fp;
}

float Camera::getNearPlane() {
	return nearPlane;
}

void Camera::setNearPlane(float np) {
	nearPlane = np;
}

float Camera::getFOV() {
	return fov;
}
void Camera::setFOV(float _fov) {
	fov = _fov;
}

float Camera::getAspectRatio() {
	return aspectRatio;
}
void Camera::setAspectRatio(float asp) {
	aspectRatio = asp;
}
glm::mat4& Camera::getTransform() {
	glm::mat4 projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	glm::mat4 View = glm::lookAt(
		glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 mvp = projectionMatrix * View;
	glm::mat4 translation = glm::mat4(1);
	glm::translate(glm::mat4(1), -position);
	glm::rotate(translation, rotation.y, glm::vec3(0,1,0));
	glm::rotate(translation, rotation.z, glm::vec3(0,0,1));
	glm::rotate(translation, rotation.x, glm::vec3(1,0,0));
	glm::mat4 cammat = projectionMatrix * translation;
	return mvp;
}