#include "main.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLfloat vertice[] =
{

	// X        Y		     u		V
	  32.0f,	-32.0f,		0.0f, 1.0f,
	 -32.0f,	-32.0f,		0.0f, 0.0f,
	 -32.0f,	 32.0f,		1.0f, 0.0f,
	  32.0f,	 32.0f,		1.0f, 1.0f,
};

GLuint indices[] =
{
		0, 1, 2,
		2, 3, 0,
};

int main() {
	


	float Width_Window = 800.0f;
	float Height_Window = 600.0f;

	system("color 5");
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Width_Window, Height_Window, "OpenGL Window", 0, 0);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window \n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	////////////////////////////////////////////////////////////////////////////

	Shader shaderProgram("Shaders/shader.vert", "Shaders/shader.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertice, sizeof(vertice));
	EBO EBO1(indices, sizeof(indices));

	unsigned int Texture;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int widtex, hegtex, nrChannels;
	unsigned char* byte = stbi_load("assets/Sprite-0001.png", &widtex, &hegtex, &nrChannels, 0);
	/*if (byte == NULL)
	{
		std::cout << "Failed to load texture \n";
		std::cout << "stbi_load returned NULL \n";
		return -1;
	}*/

	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, widtex, hegtex, 0, format, GL_UNSIGNED_BYTE, byte);
	glGenerateMipmap(GL_TEXTURE_2D);


	VAO1.LinkAttrib(VBO1, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	glm::mat4 model = glm::mat4(1.0f);


	glm::mat4 proj = glm::ortho(0.0f, Width_Window, 0.0f, Height_Window, -1.0f, 1.0f);

	const float birdX = 200.0f;
	float birdY = 300.0f;
	float velocidadeY = 0.0f;
	const float gravidade = -1800.0f;   //-1800.0f 
	const float jump = 600.0f;  //600.0f
	const float tamanho = 32.0f;
	bool spacefoiPressed = false;


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	///////////////////////////////////////////////////////////////////////////

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currTime = glfwGetTime();
		float delta = currTime - lastTime;
		lastTime = currTime;
		bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		if (spacePressed && !spacefoiPressed)
		{

			velocidadeY = jump;
		}
		spacefoiPressed = spacePressed;

		velocidadeY += gravidade * delta;
		birdY += velocidadeY * delta;

		if (birdY < tamanho)
		{
			birdY = tamanho;
			velocidadeY = 0.0f;
		}
		else if (birdY > Height_Window - tamanho)
		{
			birdY = Height_Window - tamanho;
			velocidadeY = 0.0f;
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(birdX, birdY, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		VAO1.Bind();
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		VAO1.Unbind();
		glfwPollEvents();
	}

	////////////////////////////////////////////////////////////////////////////

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
