// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"


// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightDir;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

class Assignment06;
glm::mat4 MakeViewProjectionMatrix(float Ar, float Alpha, float Beta, float Gamma, glm::vec3 Pos);
glm::mat4 MakeWorldMatrix(glm::vec3 pos, glm::quat rQ, glm::vec3 size);

// MAIN ! 
class Assignment06 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;

	// Pipelines [Shader couples]
	Pipeline P1;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model M1, MG;
	Texture T;
	DescriptorSet DS1, DSG;
	
	// Other application parameters
	glm::vec3 CamPos = glm::vec3(0.440019, 0.5, 3.45706);;
	float CamAlpha = 0.0f;
	float CamBeta = 0.0f;
	float CamRho = 0.0f;
	float Ar;
	
	glm::vec3 KeyPos = glm::vec3(0.0f, 0.5f, 0.0f);
	glm::quat KeyRot = glm::quat(glm::vec3(0, glm::radians(32.7f), 0)) *
					   glm::quat(glm::vec3(glm::radians(25.2f), 0, 0)) *
					   glm::quat(glm::vec3(0, 0, glm::radians(-82.f)));
	glm::vec3 KeyScale = glm::vec3(0.95f);
	bool MoveCam = true;

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment 06";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.6f, 0.8f, 1.0f};
		
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
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
				  });

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, "shaders/PhongVert.spv", "shaders/PhongFrag.spv", {&DSL1});
		P1.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);

		// Models, textures and Descriptors (values assigned to the uniforms)
		M1.init(this, "models/Key.obj");
		MG.init(this, "models/Room.obj");
		
		T.init(this, "textures/Walls.png");
				
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();

		DS1.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		DSG.init(this, &DSL1, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T},
				{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		
		DS1.cleanup();
		DSG.cleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T.cleanup();
		M1.cleanup();
		MG.cleanup();

		DSL1.cleanup();
		
		P1.destroy();		
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

		P1.bind(commandBuffer);
		M1.bind(commandBuffer);
		DS1.bind(commandBuffer, P1, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);

		MG.bind(commandBuffer);
		DSG.bind(commandBuffer, P1, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MG.indices.size()), 1, 0, 0, 0);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		const float ROT_SPEED = glm::radians(120.0f);
		const float MOVE_SPEED = 2.0f;

		static float debounce = false;
		static int curDebounce = 0;

		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);

		if(MoveCam) {
			CamAlpha = CamAlpha - ROT_SPEED * deltaT * r.y;
			CamBeta  = CamBeta  - ROT_SPEED * deltaT * r.x;
			CamBeta  =  CamBeta < glm::radians(-90.0f) ? glm::radians(-90.0f) :
					   (CamBeta > glm::radians( 90.0f) ? glm::radians( 90.0f) : CamBeta);
			CamRho   = CamRho   - ROT_SPEED * deltaT * r.z;
			CamRho   =  CamRho < glm::radians(-180.0f) ? glm::radians(-180.0f) :
					   (CamRho > glm::radians( 180.0f) ? glm::radians( 180.0f) : CamRho);

			glm::mat3 CamDir = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) *
							   glm::rotate(glm::mat4(1.0f), CamBeta,  glm::vec3(1,0,0)) *
							   glm::rotate(glm::mat4(1.0f), CamRho,   glm::vec3(0,0,1));

			glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(1,0,0,1);
			glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(0,0,-1,1);
			CamPos = CamPos + MOVE_SPEED * m.x * ux * deltaT;
			CamPos = CamPos + MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
			CamPos = CamPos + MOVE_SPEED * m.z * uz * deltaT;
		} else {
			KeyPos.x += MOVE_SPEED * m.x * deltaT;
			KeyPos.y += MOVE_SPEED * m.y * deltaT;
			KeyPos.z += -MOVE_SPEED * m.z * deltaT;
			KeyRot = glm::quat(glm::vec3(0, -ROT_SPEED * deltaT * r.y, 0)) *
					 glm::quat(glm::vec3(-ROT_SPEED * deltaT * r.x, 0, 0)) *
					 glm::quat(glm::vec3(0, 0, ROT_SPEED * deltaT * r.z)) *
					 KeyRot;
		}
		
		if(glfwGetKey(window, GLFW_KEY_P)) {
			if(!debounce) {
				printVec3("CamPos", CamPos);
				
				debounce = true;
				curDebounce = GLFW_KEY_P;
			}
		} else {
			if((curDebounce == GLFW_KEY_P) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(fire) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_SPACE;
				MoveCam = ! MoveCam;
				std::cout << "Switch!  " << (MoveCam ? "Camera" : "Key") << "\n";
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


		glm::mat4 ViewPrj =  MakeViewProjectionMatrix(Ar, CamAlpha, CamBeta, CamRho, CamPos);
		UniformBufferObject ubo{};
		glm::mat4 baseTr = glm::mat4(1.0f);
								
		// Here is where you actually update your uniforms

		// updates global uniforms
		GlobalUniformBufferObject gubo{};
		gubo.lightDir = glm::vec3(cos(glm::radians(135.0f)), sin(glm::radians(135.0f)), 0.0f);
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = glm::vec3(100.0, 100.0, 100.0);

		ubo.mMat = MakeWorldMatrix(KeyPos, KeyRot, KeyScale) * baseTr;
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS1.map(currentImage, &ubo, sizeof(ubo), 0);
		DS1.map(currentImage, &gubo, sizeof(gubo), 2);

		ubo.mMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0,1,0)) * baseTr;
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DSG.map(currentImage, &ubo, sizeof(ubo), 0);
		DSG.map(currentImage, &gubo, sizeof(gubo), 2);
	}
	
};

#include "WorldView.hpp"

// This is the main: probably you do not need to touch this!
int main() {
    Assignment06 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}