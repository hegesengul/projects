#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include <cmath>

static GLFWwindow* win = NULL;
int widthWindow = 1000, heightWindow = 1000;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
GLuint idMVPMatrix;

// Buffers
GLuint idVertexBuffer;
GLuint idIndexBuffer;

int textureWidth, textureHeight;
float heightFactor = 10;

int translation = 0;

float speed = 0;

glm::vec3 cameraPos ;

//glm::vec3 cameraDirection = glm::vec3(0,0,1);
glm::vec3 cameraDirection ;

//glm::vec3 up = glm::vec3(0,1,0);
glm::vec3 up ;

glm::vec3 cameraRight ;

glm::vec3 cameraUp ;

glm::mat4 view  ;

glm::mat4 projection ;

glm::mat4 MVP ;

glm::mat4 MV ;

// glm::vec4 lightPos = glm::vec4(textureWidth / 2.0 , 100 , textureHeight / 2.0 , 0);

glm::vec3 lightPos ;


static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_R){
		heightFactor += 0.5;
	}
	else if (key == GLFW_KEY_F){
		heightFactor -= 0.5;
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS){
		speed = 0;
	}
	else if (key == GLFW_KEY_Y){
		speed += 0.01;
	}
	else if (key == GLFW_KEY_H){
		speed -= 0.01;
	}
	else if (key == GLFW_KEY_W){
		auto x1 = glm::rotate(glm::mat4(1.0f) , 0.05f , cameraRight) * glm::vec4(cameraUp        , 0);
		auto x2 = glm::rotate(glm::mat4(1.0f) , 0.05f , cameraRight) * glm::vec4(cameraDirection , 0);

		cameraUp        = glm::vec3(x1.x , x1.y , x1.z) ;
		cameraDirection = glm::vec3(x2.x , x2.y , x2.z) ;

		cameraUp        = glm::normalize(cameraUp       );
		cameraDirection = glm::normalize(cameraDirection);
	}
	else if (key == GLFW_KEY_S){
		auto x1 = glm::rotate(glm::mat4(1.0f) , -0.05f , cameraRight) * glm::vec4(cameraUp        , 0);
		auto x2 = glm::rotate(glm::mat4(1.0f) , -0.05f , cameraRight) * glm::vec4(cameraDirection , 0);

		cameraUp        = glm::vec3(x1.x , x1.y , x1.z);
		cameraDirection = glm::vec3(x2.x , x2.y , x2.z);

		cameraUp        = glm::normalize(cameraUp       );
		cameraDirection = glm::normalize(cameraDirection);
	}
	else if (key == GLFW_KEY_A){
		auto x1 = glm::rotate(glm::mat4(1.0f) , 0.05f , cameraUp) * glm::vec4(cameraRight     , 0);
		auto x2 = glm::rotate(glm::mat4(1.0f) , 0.05f , cameraUp) * glm::vec4(cameraDirection , 0);

		cameraRight     = glm::vec3(x1.x , x1.y , x1.z);
		cameraDirection = glm::vec3(x2.x , x2.y , x2.z);
		
		cameraRight     = glm::normalize(cameraRight    );
		cameraDirection = glm::normalize(cameraDirection);
	}
	else if (key == GLFW_KEY_D){
		auto x1 = glm::rotate(glm::mat4(1.0f) , -0.05f , cameraUp) * glm::vec4(cameraRight     , 0);
		auto x2 = glm::rotate(glm::mat4(1.0f) , -0.05f , cameraUp) * glm::vec4(cameraDirection , 0);

		cameraRight     = glm::vec3(x1.x , x1.y , x1.z);
		cameraDirection = glm::vec3(x2.x , x2.y , x2.z);

		cameraRight     = glm::normalize(cameraRight    );
		cameraDirection = glm::normalize(cameraDirection);
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS){
		cameraPos = glm::vec3(textureWidth/2 , textureWidth/10 , - textureWidth / 4);
		//cameraPos = glm::vec3(495, 380, -631);

		cameraDirection = glm::vec3(0,0,1);
		//cameraDirection = glm::vec3(0, -0.34, 0.94);

		up = glm::vec3(0,1,0);
		//up = glm::vec3(-0.017, 0.94, 0.34);

		cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		cameraUp = glm::cross(cameraDirection, cameraRight);

		speed = 0;
	}
	else if (key == GLFW_KEY_T){
		lightPos.y += 5;
	}
	else if (key == GLFW_KEY_G){
		lightPos.y -= 5;
	}
	else if (key == GLFW_KEY_RIGHT){
		lightPos.x -= 5;
	}
	else if (key == GLFW_KEY_LEFT){
		lightPos.x += 5;
	}
	else if (key == GLFW_KEY_UP){
		lightPos.z += 5;
	}
	else if (key == GLFW_KEY_DOWN){
		lightPos.z -= 5;
	}
	else if (key == GLFW_KEY_Q) translation = (textureWidth + translation - 1) % textureWidth;
	else if (key == GLFW_KEY_E) translation = (textureWidth + translation + 1) % textureWidth;
	else if (key == GLFW_KEY_P && action == GLFW_PRESS){
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
}

void resize(GLFWwindow* window, int new_width, int new_height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
	//if(new_width == 0 ) new_width = 1 ;
	//if(new_height ==0 ) new_height  = 1;
    glViewport(0, 0, new_width, new_height);
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
	printf("Please provide height and texture image files!\n");
	exit(-1);
	}

	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) {
	exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This is required for remote
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This might be used for local

	win = glfwCreateWindow(widthWindow, heightWindow, "CENG477 - HW4", NULL, NULL);

	if (!win) {
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(win);

    glfwSetFramebufferSizeCallback(win, resize);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

		glfwTerminate();
		exit(-1);
	}

	glEnable (GL_DEPTH_TEST);

	GLuint idProgramShader;

	std::string vertex_shader_name = "shader.vert";
	std::string fragment_shader_name = "shader.frag";

	initShaders(idProgramShader , vertex_shader_name , fragment_shader_name);


	glUseProgram(idProgramShader);

	glfwSetKeyCallback(win, keyCallback);

	initTexture(argv[1], argv[2], &textureWidth, &textureHeight);

    glUniform1i(glGetUniformLocation(idProgramShader , "rgbTexture"), 0);
    glUniform1i(glGetUniformLocation(idProgramShader , "heiTexture"), 1);

	glUniform1f(glGetUniformLocation(idProgramShader , "heightFactor"), heightFactor);

	glUniform1i(glGetUniformLocation(idProgramShader , "widthTexture"), textureWidth);
	glUniform1i(glGetUniformLocation(idProgramShader , "heightTexture"), textureHeight);

	lightPos = glm::vec3(textureWidth / 2 , 100 , textureHeight / 2 );

	glUniform3fv(glGetUniformLocation(idProgramShader , "lightPosition") , 1 , glm::value_ptr(lightPos));


	GLfloat *coord = (GLfloat *) malloc(sizeof(GLfloat) * textureWidth * textureHeight * 18);

	for(int i = 0 , writeind = 0 ; i < textureWidth - 1 ; ++i){
		for(int j = 0 ; j < textureHeight - 1 ; ++j , writeind += 18){
			// 1st triangle
			coord[writeind    ] = i;
			coord[writeind + 1] = 0;
			coord[writeind + 2] = j;

			coord[writeind + 3] = i + 1 ;
			coord[writeind + 4] = 0     ;
			coord[writeind + 5] = j     ;

			coord[writeind + 6] = i    ;
			coord[writeind + 7] = 0    ;
			coord[writeind + 8] = j + 1;

			// 2nd triangle
			coord[writeind + 9 ] = i + 1;
			coord[writeind + 10] = 0    ;
			coord[writeind + 11] = j    ;

			coord[writeind + 12] = i + 1;
			coord[writeind + 13] = 0    ;
			coord[writeind + 14] = j + 1;

			coord[writeind + 15] = i    ;
			coord[writeind + 16] = 0    ;
			coord[writeind + 17] = j + 1;
		}
	}

	unsigned int VAO,VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * textureWidth * textureHeight * 18 , coord, GL_STATIC_DRAW);

	auto positionLoc =  glGetAttribLocation(idProgramShader,"pos");
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*) 0);
	//glEnableVertexAttribArray(positionLoc);
	free(coord);

	cameraPos = glm::vec3(textureWidth / 2 , textureWidth / 10 , - textureWidth / 4);
	//cameraPos = glm::vec3(500.0 , 72.600418 , 10.714212);

	cameraDirection = glm::vec3(0 , 0 , 1);
	//cameraDirection = glm::vec3(0, -0.34, 0.94);

	up = glm::vec3(0 , 1 , 0);
	//up = glm::vec3(-0.017, 0.94, 0.34);

	cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	cameraUp = glm::cross(cameraDirection, cameraRight);

	view = glm::lookAt(cameraPos , cameraPos + cameraDirection , cameraUp);

	projection = glm::perspective(glm::radians(45.0) , 1.0 , 0.1, 1000.0);


	MVP = projection * view;

	MV = view;

	glUniform3fv(glGetUniformLocation(idProgramShader , "cameraPosition"), 1 , glm::value_ptr(cameraPos));

	while(!glfwWindowShouldClose(win)) {
		glfwSwapBuffers(win);

		glClearDepth(1.0f);
		glClearColor(0 , 0 , 0 , 0);
		glClearStencil(0);

		//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(idProgramShader);

		cameraPos += glm::vec3(cameraDirection.x * speed , cameraDirection.y * speed , cameraDirection.z * speed);

		view = glm::lookAt(cameraPos, cameraPos + cameraDirection , cameraUp);

		MVP = projection * view;

		MV = view;

		/*std::cout << "right:     " << glm::to_string(cameraRight) << std::endl;
		std::cout << "up:        " << glm::to_string(cameraUp) << std::endl;
		std::cout << "direction: " <<  glm::to_string(cameraDirection) << std::endl;*/

		glUniform3fv(glGetUniformLocation(idProgramShader , "lightPosition") , 1 , glm::value_ptr(lightPos));
		glUniform3fv(glGetUniformLocation(idProgramShader , "cameraPosition"), 1 , glm::value_ptr(cameraPos));

		glUniform1i(glGetUniformLocation(idProgramShader , "text_delay"), translation);

		glUniform1f(glGetUniformLocation(idProgramShader , "heightFactor"), heightFactor);

		glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MV" ), 1, GL_FALSE, glm::value_ptr(MV));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES , 0 , (textureWidth - 1) * (textureHeight - 1) * 6);
		//glDrawArrays(GL_TRIANGLES, 0 , (textureWidth )* (textureHeight ) * 6);

		glfwPollEvents();
	}


	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
