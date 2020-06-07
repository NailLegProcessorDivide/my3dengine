#include "BrokenWorldEngine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(char** args, int iargs) {
	BrokenWorldEngine::init();
	BrokenWorldEngine* bwe = BrokenWorldEngine::createBWE(600, 600, "hello");
	//bwe.setInfo(NULL);
	logger.setLoggingVerboseness(2);
	static float g_vertex_buffer_data[] = {
		-1.0f, -0.8f, 0.5f,
		 1.0f, -1.0f, 0.5f,
		 0.0f,  1.0f, 0.5f,
		-1.0f,  1.0f, 0.5f,
		 1.0f,  1.0f, 0.5f,
		 0.0f, -1.0f, 0.5f,
	};
	int tri = bwe->createModel(g_vertex_buffer_data, 6);
	int biningData = bwe->bindModel(tri);
	int instance = bwe->CreateModelInstance(biningData, true);
	char title[20];
	int n = 0;
	printf("%p\n", bwe);
	int shader = bwe->genShader("shaders/default.vs", "shaders/default.fs");
	bwe->bindShader(shader);
	//bwe->setCamPos(0, 0, 0);
	while (!bwe->windowCloseRequested()) {
		bwe->bindShader(shader);
		_itoa_s(n, title, 10);
		//bwe->fillf(1, 0, 1);
		bwe->setWindowName(title);
		bwe->pollEvents();

		bwe->setCamPos(sinf(n/100.0f),0,1);
		//bwe->setLoggingVerboseness(n % 10);
		n++;
		bwe->draw();
	}
	bwe->deleteModel(tri);
	BrokenWorldEngine::deleteBWE(bwe);
	BrokenWorldEngine::terminate();
	//delete bwe;
	getc(stdin);
}