#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static char* filetobuf(char *fn)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		printf("reading file\n");
		fp = fopen(fn,"rt");

		if (fp != NULL) {
			printf("go to file end\n");
			fseek(fp, 0, SEEK_END);
			printf("get filesize\n");
			count = ftell(fp);
			printf("move back to start\n");
			rewind(fp);

			if (count > 0) {
				printf("allocating: %d \n", count + 1);
				content = (char *)malloc(sizeof(char) * (count+1));
				printf("reading file into buffer\n");
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

static GLuint create_shader_program() 
{
	int IsCompiled_VS, IsCompiled_FS;
	int IsLinked;
	int maxLength;
	char *vertexInfoLog;
	char *fragmentInfoLog;
	char *shaderProgramInfoLog;

	GLchar *vertexsource, *fragmentsource;
	GLuint vertexshader, fragmentshader;

	GLuint shaderprogram;
	
	printf("reading files\n");

	vertexsource = filetobuf("passthrough.vert");
	fragmentsource = filetobuf("passthrough.frag");

	printf("Building vertex shader");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	glCompileShader(vertexshader);

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if(!IsCompiled_VS)
	{
		glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
		vertexInfoLog = (char *)malloc(maxLength);
		glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);
		free(vertexInfoLog);
		return 0;
	}

	printf("Building fragment shader");

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	glCompileShader(fragmentshader);

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if(!IsCompiled_FS)
	{
		glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
		fragmentInfoLog = (char *)malloc(maxLength);
		glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);
		free(fragmentInfoLog);
		return 0;
	}

	printf("Creating shader program");

	shaderprogram = glCreateProgram();

	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);

	glBindAttribLocation(shaderprogram, 0, "in_Position");
	glBindAttribLocation(shaderprogram, 1, "in_Color");

	glLinkProgram(shaderprogram);

	glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
	if(!IsLinked)
	{
		glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);
		shaderProgramInfoLog = (char *)malloc(maxLength);
		glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog); 
		free(shaderProgramInfoLog);
		return 0;
	}

	return shaderprogram;
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	printf("Building shader\n");

	GLuint program;
	program = create_shader_program();

	printf("Still working\n");

	while (!glfwWindowShouldClose(window))
	{
//		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
//		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

