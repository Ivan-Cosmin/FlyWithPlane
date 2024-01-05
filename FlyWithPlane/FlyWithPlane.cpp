#include <Windows.h>
#include <locale>
#include <codecvt>
#include <GL/glew.h>
#include "Camera.h"

#include "TextureLoader.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1000;

float skylight;
glm::vec3 lightPos = glm::vec3(3000.0f, 1000.0f, 8000.0f);
void changeHour(Shader& shader1, Shader& shader2, Shader& shader3, Shader& shader4)
{
	static glm::vec3 value(0.6f);
	skylight = value.x;
	if (Darker == true)
	{
		if (value.x > 0.2f)
		{
			value -= glm::vec3(0.05f);
			shader1.Activate();
			shader1.SetVec3("lightColor", value);
			shader2.Activate();
			shader2.SetVec3("lightColor", value);
			shader3.Activate();
			shader3.SetVec3("lightColor", value);
			shader4.Activate();
			shader4.SetVec3("lightColor", value);

			if (lightPos.z >= 0)
				lightPos -= glm::vec3(0.0f, -1000.0f, 2800.0f);
			else
				lightPos -= glm::vec3(0.0f, 1000.0f, 1750.0f);
		}
		Darker = false;
	}

	if (Lighter == true)
	{
		if (value.x < 0.6f)
		{
			value += glm::vec3(0.05f);
			shader1.Activate();
			shader1.SetVec3("lightColor", value);
			shader2.Activate();
			shader2.SetVec3("lightColor", value);
			shader3.Activate();
			shader3.SetVec3("lightColor", value);
			shader4.Activate();
			shader4.SetVec3("lightColor", value);

			if (lightPos.z < 0)
				lightPos += glm::vec3(0.0f, +1000.0f, 2800.0f);
			else
				lightPos += glm::vec3(0.0f, -1000.0f, 1750.0f);
		}
		Lighter = false;
	}
}

std::vector<Mesh> Aeroport;
unsigned int GrassTex;
unsigned int RoadTex;
unsigned int RoofTex;
unsigned int GrindaTex;
unsigned int TileTex;
unsigned int AvionTex;
unsigned int TurnTex;
unsigned int CopacTex;

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

void AeroportInit(std::string path)
{
	Mesh AcoperisHangar(path + "AcoperisHangar.obj", path);
	Aeroport.push_back(AcoperisHangar);

	Mesh Iarba(path + "Iarba.obj", path);
	Aeroport.push_back(Iarba);

	Mesh InteriorHangar(path + "InteriorHangar.obj", path);
	Aeroport.push_back(InteriorHangar);

	Mesh MetalHangare(path + "MetalHangare.obj", path);
	Aeroport.push_back(MetalHangare);

	Mesh Road(path + "Road.obj", path);
	Aeroport.push_back(Road);

	Mesh TurnBaza(path + "TurnBaza1.obj", path);
	TurnBaza.setColor(0, glm::vec3(0.85f, 0.85f, 0.85f));
	Aeroport.push_back(TurnBaza);

	Mesh TurnBazaTexture(path + "TurnBazaTexture.obj", path);
	Aeroport.push_back(TurnBazaTexture);

	Mesh TurnVarfAlb(path + "TurnVarfAlb.obj", path);
	TurnVarfAlb.setColor(0, glm::vec3(0.85f, 0.85f, 0.85f));
	Aeroport.push_back(TurnVarfAlb);

	Mesh TurnVarfNegru(path + "TurnVarfNegru.obj", path);
	TurnVarfNegru.setColor(0, glm::vec3(0.0f, 0.0f, 0.0f));
	Aeroport.push_back(TurnVarfNegru);

	Mesh Fundatie(path + "Fundatie.obj", path);
	Aeroport.push_back(Fundatie);

	Mesh Copaci(path + "FrunzeCopaci.obj", path);
	Aeroport.push_back(Copaci);

	Mesh PlaneMetal(path + "PlaneMetal.obj", path);
	Aeroport.push_back(PlaneMetal);

	for (int i = 0; i < Aeroport.size(); i++)
	{
		if (i != 1 && i != 4 && i != 9)
			Aeroport[i].setPosition(glm::vec3(10.f, -7.f, 10.f));
		else
			Aeroport[i].setPosition(glm::vec3(10.f, 0.f, 10.f));
		Aeroport[i].setScale(glm::vec3(10.f));
		Aeroport[i].initVAO();
	}

	GrassTex = CreateTexture(path + "Resources\\Sol.jpg");
	RoadTex = CreateTexture(path + "Resources\\Drum.jpg");
	RoofTex = CreateTexture(path + "Resources\\Shelter_simple_greenpanel.jpg");
	TurnTex = CreateTexture(path + "Resources\\tower2.jpg");
	CopacTex = CreateTexture(path + "Resources\\copac.jpg");
	AvionTex = CreateTexture(path + "Resources\\avion.jpg");
	TileTex = CreateTexture(path + "Resources\\Shelter_simple_whitepanel.jpg");
	GrindaTex = CreateTexture(path + "Resources\\Shelter_simple_frame.bmp");
}

void AeroportRender(Shader& shaderA, Shader& shaderM, Shader& shaderT)
{
	shaderT.Activate();
	for (int i = 0; i < Aeroport.size(); i++)
	{
		if (i == 0)
		{
			glBindTexture(GL_TEXTURE_2D, RoofTex);
			Aeroport[i].render(&shaderT);
		}
		if (i == 6)
		{
			glBindTexture(GL_TEXTURE_2D, TurnTex);
			Aeroport[i].render(&shaderT);
		}
		if (i == 2)
		{
			glBindTexture(GL_TEXTURE_2D, TileTex);
			Aeroport[i].render(&shaderT);
		}
		if (i == 3)
		{
			glBindTexture(GL_TEXTURE_2D, GrindaTex);
			Aeroport[i].render(&shaderT);
		}
		if (i == 10)
		{
			glBindTexture(GL_TEXTURE_2D, CopacTex);
			Aeroport[i].render(&shaderT);
		}
		if (i == 11)
		{
			glBindTexture(GL_TEXTURE_2D, AvionTex);
			Aeroport[i].render(&shaderT);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	shaderM.Activate();
	for (int i = 0; i < Aeroport.size(); i++)
	{
		if (i != 1 && i != 4 && i != 0 && i != 6 && i != 2 && i != 3 && i != 9 && i != 10 && i != 11)
		{
			Aeroport[i].render(&shaderM);
		}
	}

	shaderA.Activate();
	for (int i = 0; i < Aeroport.size(); i++)
	{
		if (i == 1)
		{
			glBindTexture(GL_TEXTURE_2D, GrassTex);
			Aeroport[i].render(&shaderT);
		}
		if (i == 4)
		{
			glBindTexture(GL_TEXTURE_2D, RoadTex);
			Aeroport[i].render(&shaderT);
		}
		if (i == 9)
		{
			glBindTexture(GL_TEXTURE_2D, RoadTex);
			Aeroport[i].render(&shaderT);
		}
	}
}


Camera* pCamera = nullptr;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void Cleanup()
{
	delete pCamera;
}

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FlyWithPlane", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	// first, configure the cube's VAO (and VBO)
	unsigned int cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create camera
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 2.0));


	wchar_t buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, buffer);

	std::wstring executablePath(buffer);
	std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string currentPath = converter.to_bytes(wscurrentPath);

	//Paths
	std::string ShadersPath = currentPath + "\\Shaders\\";
	std::string SkyBoxPath = currentPath + "\\Models\\skybox\\";
	std::string PlanePath = currentPath + "\\Models\\Plane\\";
	std::string AirportPath = currentPath + "\\Models\\Airport\\";
	std::string MapPath = currentPath + "\\Models\\Map\\";


	//Shaders
	Shader programShader((ShadersPath + "default.vs").c_str(), (ShadersPath + "default.fs").c_str());
	Shader skyboxShader((ShadersPath + "skybox.vs").c_str(), (ShadersPath + "skybox.fs").c_str());
	Shader airportShader((ShadersPath + "airport.vs").c_str(), (ShadersPath + "airport.fs").c_str());
	Shader lampShader((ShadersPath + "Lamp.vs").c_str(), (ShadersPath + "Lamp.fs").c_str());
	Shader terrainShader((ShadersPath + "terrain.vs").c_str(), (ShadersPath + "terrain.fs").c_str());


	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);


	terrainShader.Activate();
	terrainShader.SetVec3("lightColor", lightColor);

	Mesh Avion(PlanePath + "Plane.obj", PlanePath);
	Avion.setPosition(glm::vec3(0.f));
	Avion.setColor(0, glm::vec3(0.45f, 0.0f, 0.0f));
	Avion.setColor(1, glm::vec3(0.1f, 0.1f, 0.1f));
	Avion.setColor(2, glm::vec3(0.5f, 0.5f, 0.5f));
	Avion.setRotation(glm::vec3(15.0f, 180.0f, 0.f));
	Avion.setPosition(glm::vec3(0.0f, 1.8f, -1.5f));
	Avion.initVAO();

	Mesh Map(MapPath + "Map.obj", MapPath);
	Map.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	Map.setPosition(glm::vec3(1000.0f, -165.0f, -80.0f));
	Map.initVAO();

	// All the faces of the cubemap (make sure they are in this exact order)
	std::vector<std::string> facesCubemap =
	{
		SkyBoxPath + "px.png",
		SkyBoxPath + "nx.png",
		SkyBoxPath + "py.png",
		SkyBoxPath + "ny.png",
		SkyBoxPath + "pz.png",
		SkyBoxPath + "nz.png"
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture = LoadSkybox(facesCubemap);
	int floorTexture = CreateTexture(MapPath + "Map.jpg");
	AeroportInit(AirportPath);

	float deltaTime = 0.f;
	float lastFrame = 0.f;
	float deltaAltitude = 0.f;
	bool desprindere = false;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		double FrameStart = glfwGetTime();
		deltaTime = FrameStart - lastFrame;
		lastFrame = FrameStart;

		float currentAltitude = Avion.getPosition().y;
		deltaAltitude = (pCamera->speed - 0.5f) * 2.f;
		float currentTilt = Avion.getRotation().y;
		//changeHour(shader, terrainShader);

		float clearR = 0.07f + skylight / 2.f - 0.1f;
		float clearG = 0.13f + skylight / 2.f - 0.1f;
		float clearB = 0.17 + skylight / 2.f - 0.1f;
		glClearColor(clearR, clearG, clearB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (Avion.getPosition().y < 0.0f)
		{
			Avion.setPosition(glm::vec3(Avion.getPosition().x, 0.0f, Avion.getPosition().z));
		}

		if (Avion.getPosition().y > 0.0f)
		{
			desprindere = false;
		}
		else
		{
			desprindere = true;
		}

		if (UpPressed == true)
		{
			if (pCamera->speed < .5f)
			{
				pCamera->speed += 0.0002f;
			}
		}
		else
			if (DownPressed == true)
			{
				if (pCamera->speed > 0.f)
				{
					pCamera->speed -= 0.00015f;
				}
			}

		if (pCamera->speed > 0.15f)
		{
			Avion.setPosition(glm::vec3(Avion.getPosition() + glm::vec3(0.0f, 0.2f, 0.0f)));
		}
		else
			if (pCamera->speed < 0.1f && !desprindere)
			{
				if (Avion.getPosition().y > 0.0f)
				{
					Avion.setPosition(glm::vec3(Avion.getPosition() + glm::vec3(0.0f, -0.2f, 0.0f)));
				}
			}

		float Xrot = 0.0f;
		float Zrot = 0.0f;
		if (Avion.getPosition().y > 0.f)
		{
			if (pCamera->speed > 0.15f)
			{
				Xrot = deltaAltitude * 5.f * glm::cos(glm::radians(Avion.getRotation().y));
				Zrot = deltaAltitude * 5.f * -glm::sin(glm::radians(Avion.getRotation().y));
			}
			else
			{
				Xrot = deltaAltitude * 5.f * glm::cos(glm::radians(Avion.getRotation().y));
				Zrot = deltaAltitude * 5.f * -glm::sin(glm::radians(Avion.getRotation().y));
			}
		}

		float angle = glm::abs(Xrot) + glm::abs(Zrot);
		if (Avion.getRotation().y > 0.f)
			angle *= 4;
		else
			angle /= 2.f;
		float momentum = glm::abs(glm::cos(glm::radians(angle)));
		Avion.setPosition(glm::vec3(Avion.getPosition() + glm::vec3(pCamera->speed * momentum * 15.f * glm::sin(glm::radians(Avion.getRotation().y)), 0.0f, pCamera->speed * momentum * 15.f * glm::cos(glm::radians(Avion.getRotation().y)))));
		Avion.setRotation(glm::vec3(0.f, Avion.getRotation().y, 0.f) + glm::vec3(Xrot, 0.0f, Zrot));

		processInput(window, pCamera, deltaTime, &Avion);
		pCamera->SetPosition(glm::vec3(Avion.getPosition() + glm::vec3(-sin(glm::radians(Avion.getRotation().y + pCamera->offset)) * 50.f, 15.0f, -cos(glm::radians(Avion.getRotation().y + pCamera->offset)) * 50.f)));
		pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(0.0f, glm::radians((pCamera->frontTilt - 13.f) / 1.5f) * 50.f, 0.0f));
		pCamera->pitch = -pCamera->frontTilt;
		pCamera->yaw = -((float)Avion.getRotation().y + (float)pCamera->offset - 90.f);
		changeHour(terrainShader, skyboxShader, programShader, airportShader);

		glm::mat4 projection = pCamera->GetProjectionMatrix();
		glm::mat4 view = pCamera->GetViewMatrix();
		pCamera->UpdateCameraVectors();

		programShader.Activate();
		programShader.SetVec3("lightPos", lightPos);
		programShader.SetVec3("lightColor", glm::vec3(0.6f));
		programShader.SetVec3("viewPos", pCamera->GetPosition());

		programShader.SetMat4("projection", projection);
		programShader.SetMat4("view", view);
		Avion.render(&programShader);

		lampShader.Activate();
		lampShader.SetMat4("projection", projection);
		lampShader.SetMat4("view", view);
		glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(300.f)); // a smaller cube
		lampShader.SetMat4("model", lightModel);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		airportShader.Activate();
		airportShader.SetVec3("lightPos", lightPos);
		airportShader.SetVec3("lightColor", glm::vec3(0.6f));
		airportShader.SetVec3("viewPos", pCamera->GetPosition());

		airportShader.SetMat4("projection", projection);
		airportShader.SetMat4("view", view);
		AeroportRender(airportShader, programShader, terrainShader);

		terrainShader.Activate();
		terrainShader.SetMat4("projection", projection);
		terrainShader.SetMat4("view", view);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		Map.render(&terrainShader);
		glBindTexture(GL_TEXTURE_2D, 0);

		//render skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Activate();
		projection = pCamera->GetProjectionMatrix();
		view = glm::mat4(glm::mat3(pCamera->GetViewMatrix()));
		skyboxShader.SetMat4("projection", projection);
		skyboxShader.SetMat4("view", view);

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Cleanup();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	programShader.Delete();
	skyboxShader.Delete();
	airportShader.Delete();
	lampShader.Delete();
	terrainShader.Delete();

	// glfw: terminate, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}