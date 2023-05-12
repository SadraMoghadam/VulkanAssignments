// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "TextMaker.hpp"


 std::vector<SingleText> demoText = {
	{1, {"Spheres", "", "", ""}, 0, 0},
	{1, {"City Scene", "", "", ""}, 0, 0}
};

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightPos;
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec2 UV;
	glm::vec3 norm;
};

class Assignment13;
void GameLogic(Assignment13 *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World);

// MAIN ! 
class Assignment13 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;

	// Pipelines [Shader couples]
	VertexDescriptor VD;
	Pipeline P1;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model<Vertex> M1, M2;
	Texture T1, T1em;
	DescriptorSet DS1;

	TextMaker txt;
	
	// Other application parameters
	int currScene = 0;
	float Ar;
	glm::mat4 ViewPrj;
	glm::vec3 Pos = glm::vec3(0,0,10);
	glm::vec3 cameraPos;
	float Yaw = glm::radians(0.0f);
	float Pitch = glm::radians(0.0f);
	float Roll = glm::radians(0.0f);

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment 13";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.015f, 0.03f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 7;
		texturesInPool = 4;
		setsInPool = 4;
		
		Ar = 4.0f / 3.0f;
	}
	
	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		std::cout << "Window resized to: " << w << " x " << h << "\n";
		Ar = (float)w / (float)h;
	}
	
	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSL1.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
				  });

		// Vertex descriptors
		VD.init(this, {
				  {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
				         sizeof(glm::vec3), NORMAL},
				  {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
				         sizeof(glm::vec2), UV}
				});

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, &VD, "shaders/BlinnVert.spv", "shaders/BlinnFrag.spv", {&DSL1});

		// Models, textures and Descriptors (values assigned to the uniforms)
		M1.init(this, &VD, "models/Sphere.obj", OBJ);
		M2.init(this, &VD, "models/City.obj", OBJ);
		
		T1.init(this, "textures/TexturesCity.png");
		T1em.init(this, "textures/TexturesCityEmission.png");

		txt.init(this, &demoText);
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();

		DS1.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{3, TEXTURE, 0, &T1em}
				});
		txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		
		DS1.cleanup();
		
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1.cleanup();
		T1em.cleanup();
		M1.cleanup();
		M2.cleanup();

		DSL1.cleanup();
		
		P1.destroy();		
		
		txt.localCleanup();
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		switch(currScene) {
		  case 0:
			P1.bind(commandBuffer);
			M1.bind(commandBuffer);
			DS1.bind(commandBuffer, P1, currentImage);
						
			vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);
			break;
		  case 1:
			P1.bind(commandBuffer);
			M2.bind(commandBuffer);
			DS1.bind(commandBuffer, P1, currentImage);
						
			vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
			break;
		}

		txt.populateCommandBuffer(commandBuffer, currentImage, currScene);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static bool debounce = false;
		static int curDebounce = 0;
//std::cout << xpos << " " << ypos << " " << m_dx << " " << m_dy << "\n";

		if(glfwGetKey(window, GLFW_KEY_SPACE)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_SPACE;
				currScene = (currScene+1) % 2;
				std::cout << "Scene : " << currScene << "\n";
				Pos = glm::vec3(0,0,currScene == 0 ? 10 : 8);
				RebuildPipeline();
			}
		} else {
			if((curDebounce == GLFW_KEY_SPACE) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		
		GameLogic();
		
//printMat4("ViewPrj", ViewPrj);
//printMat4("WM", WM);
		
		UniformBufferObject ubo{};								
		// Here is where you actually update your uniforms

		// updates global uniforms
		ubo.mMat = glm::mat4(1);
		ubo.mvpMat = ViewPrj;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		
		float dang = Pitch + glm::radians(15.0f);

		GlobalUniformBufferObject gubo{};
		switch(currScene) {
		  case 0:
			ubo.mMat = glm::scale(glm::mat4(1), glm::vec3(3));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));

			gubo.lightPos = glm::vec3(5, 5, 5);
			gubo.lightDir = glm::normalize(glm::vec3(1, 1, 1));
			gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			gubo.eyePos = cameraPos;

			DS1.map(currentImage, &ubo, sizeof(ubo), 0);
			DS1.map(currentImage, &gubo, sizeof(gubo), 2);
			break;
		  case 1:
			ubo.mMat = glm::mat4(1);
			ubo.mvpMat = ViewPrj;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));

			gubo.lightPos = glm::vec3(0.5f,4.92f,-4.12f);
			gubo.lightDir = glm::vec3(0, 1, 0);
			gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			gubo.eyePos = cameraPos;

			DS1.map(currentImage, &ubo, sizeof(ubo), 0);
			DS1.map(currentImage, &gubo, sizeof(gubo), 2);
			break;
		}
	}
	
	void GameLogic() {
		// Parameters
		// Camera FOV-y, Near Plane and Far Plane
		const float FOVy = glm::radians(45.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.f;
		// Camera target height and distance
		const float camHeight = 1.25;
		const float camDist = 1.5;
		// Camera Pitch limits
		const float minPitch = glm::radians(-60.0f);
		const float maxPitch = glm::radians(60.0f);
		// Rotation and motion speed
		const float ROT_SPEED = glm::radians(120.0f);
		const float MOVE_SPEED = 2.0f;

		// Integration with the timers and the controllers
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);

		// Game Logic implementation
		// Current Player Position - statc variable make sure its value remain unchanged in subsequent calls to the procedure

		// To be done in the assignment
		ViewPrj = glm::mat4(1);
		glm::mat4 World = glm::mat4(1);
		
		// World
		// Position
		glm::vec3 ux = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0,1,0)) * glm::vec4(1,0,0,1);
		glm::vec3 uz = glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0,1,0)) * glm::vec4(0,0,-1,1);
		Pos = Pos + MOVE_SPEED * m.x * ux * deltaT;
		Pos = Pos + MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
		Pos.y = Pos.y < 0.0f ? 0.0f : Pos.y;
		Pos = Pos + MOVE_SPEED * m.z * uz * deltaT;
		// Rotation
		Yaw = Yaw - ROT_SPEED * deltaT * r.y;
		Pitch = Pitch + ROT_SPEED * deltaT * r.x;
		Pitch  =  Pitch < minPitch ? minPitch :
				   (Pitch > maxPitch ? maxPitch : Pitch);
		Roll   = Roll   - ROT_SPEED * deltaT * r.z;
		Roll   = Roll < glm::radians(-175.0f) ? glm::radians(-175.0f) :
				   (Roll > glm::radians( 175.0f) ? glm::radians( 175.0f) : Roll);

//std::cout << Pos.x << ", " << Pos.y << ", " << Pos.z << ", " << Yaw << ", " << Pitch << ", " << Roll << "\n";

		// Final world matrix computaiton
		World = glm::translate(glm::mat4(1), Pos) * glm::rotate(glm::mat4(1.0f), Yaw, glm::vec3(0,1,0));
		
		// Projection
		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		// View
		// Target
		glm::vec3 target = Pos + glm::vec3(0.0f, camHeight, 0.0f);

		// Camera position, depending on Yaw parameter, but not character direction
		cameraPos = World * glm::vec4(0.0f, camHeight + camDist * sin(Pitch), camDist * cos(Pitch), 1.0);
		// Damping of camera
		glm::mat4 View = glm::rotate(glm::mat4(1.0f), -Roll, glm::vec3(0,0,1)) *
						 glm::lookAt(cameraPos, target, glm::vec3(0,1,0));

		ViewPrj = Prj * View;
	}
};


// This is the main: probably you do not need to touch this!
int main() {
    Assignment13 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}