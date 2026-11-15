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

GLuint loadTexture(const char* path)
{
	int w, h, ch;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(path, &w, &h, &ch, 0);
	if (!data) {
		std::cout << "Falha ao carregar: " << path << "\n";
		return 0;
	}

	GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
	return id;
}

int main() {
	


	float Width_Window = 600.0f;
	float Height_Window = 800.0f;

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
	GLuint birdTex = loadTexture("assets/Sprite-0001.png");
	GLuint bgTex = loadTexture("assets/background.png");
	unsigned char* byte = stbi_load("assets/background.png", &widtex, &hegtex, &nrChannels, 0);
	if (byte == NULL){
		std::cout << "Failed to load texture \n";
		std::cout << "The ERROR could be: \n";
		std::cout << "	- Texture file not found or invalid. \n";
		std::cout << "	- Check whether the assets(/src/assets) folder is located where the build. \n";
		
		return 0;
	}

	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, widtex, hegtex, 0, format, GL_UNSIGNED_BYTE, byte);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_set_flip_vertically_on_load(true);

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
	const float gravidade = -1300.0f;   //-1800.0f 
	const float jump = 400.0f;  //600.0f
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
		//------------------------Pulo-e-Gravidade-------------------------//
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

		//------------------------------------------------------------------//

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 0);
		GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		VAO1.Bind();
		
		//---------------------------Background----------------------------//

		glm::mat4 bgModel = glm::mat4(1.0f);
		bgModel = glm::translate(bgModel, glm::vec3(300.0f, 400.0f, 0.0f));
		bgModel = glm::scale(bgModel, glm::vec3(10.0f,13.0f, 0.0f));    
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bgModel));
		glBindTexture(GL_TEXTURE_2D, bgTex);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//------------------------------Bird-------------------------------//

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(birdX, birdY, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, birdTex);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//------------------------------------------------------------------//

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
