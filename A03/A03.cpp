// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "TextMaker.hpp"

 std::vector<SingleText> demoText = {
	{1, {"Solve the puzzle!", "", "", ""}, 0, 0}};



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

struct SkyboxUniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

class Assignment03;
void SetupMatrices(Assignment03 *A);

// MAIN ! 
class Assignment03 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;
	DescriptorSetLayout DSLskyBox;

	// Pipelines [Shader couples]
	Pipeline P1;
	Pipeline PskyBox;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model M1[6], MG;
	Texture T1, TG;
	DescriptorSet DS1[6], DSG;
	glm::mat4 AsTr[6];
	glm::mat4 Tpre[6];
	glm::mat4 Tpost[6];

	Model MskyBox;
	Texture TskyBox;
	DescriptorSet DSskyBox;
	
	TextMaker txt;
	
	// Other application parameters
//	glm::mat3 CamDir = glm::mat3(1.0f);
//	glm::vec3 CamPos = glm::vec3(0.0f, 5.2f, 12.0f);
	glm::vec3 CamPos = glm::vec3(0, 9.79878, 3.9552);
	glm::mat3 CamDir = glm::mat3(1, 0, 0, 0, 0.40035, -0.916363, 0, 0.916363, 0.40035);

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment 02";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 16;
		texturesInPool = 9;
		setsInPool = 9;
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
		DSLskyBox.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
				  });

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, "shaders/PhongVert.spv", "shaders/PhongFrag.spv", {&DSL1});
		P1.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);
		PskyBox.init(this, "shaders/SkyBoxVert.spv", "shaders/SkyBoxFrag.spv", {&DSLskyBox});
		PskyBox.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_BACK_BIT, false);

		// Models, textures and Descriptors (values assigned to the uniforms)
		M1[0].init(this, "models/TL.obj");
		M1[1].init(this, "models/TC.obj");
		M1[2].init(this, "models/TR.obj");
		M1[3].init(this, "models/BL.obj");
		M1[4].init(this, "models/BC.obj");
		M1[5].init(this, "models/BR.obj");
		MG.init(this, "models/LargePlane.obj");
		
		T1.init(this, "textures/Gufo_di_Vanzago.jpeg");
		TG.init(this, "textures/3DLABbg_UV_Map_Checker_03_2048x2048.jpg");
		
		MskyBox.init(this, "models/SkyBoxCube.obj");
		const char *T2fn[] = {"textures/sky/bkg1_right.png", "textures/sky/bkg1_left.png",
							  "textures/sky/bkg1_top.png",   "textures/sky/bkg1_bot.png",
							  "textures/sky/bkg1_front.png", "textures/sky/bkg1_back.png"};
		TskyBox.initCubic(this, T2fn);
		
		txt.init(this, &demoText);
		
		SetupMatrices(this);
//		#include "secrets.hpp"
		Tpre[0] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Tpre[1] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Tpre[2] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Tpre[3] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Tpre[4] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Tpre[5] = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		Tpost[0] = glm::mat4(-2.18557e-08, 0, 0.5, 0, 0, 0.5, 0, 0, -0.5, 0, -2.18557e-08, 0, 1, -1, -1.5, 1);
		Tpost[1] = glm::mat4(0.333333, 0, 0.666667, 0, 0, 1, 0, 0, -0.666667, 0, -0.333333, 0, 0.333333, -2, -0.333333, 1);
		Tpost[2] = glm::mat4(-0.707107, 0, 0.707107, 0, 0, 1, 0, 0, -0.707107, 0, -0.707107, 0, 2.70711, -2, 0.87868, 1);
		Tpost[3] = glm::mat4(-0.866025, 0, 0.5, 0, 0, 1, 0, 0, -0.5, 0, -0.866025, 0, -2.23205, -2, -0.133975, 1);
		Tpost[4] = glm::mat4(0.5, -0.707107, 0.5, 0, -0.707107, -5.96046e-08, 0.707107, 0, -0.5, -0.707107, -0.5, 0, 0.914213, 0.707107, -0.914213, 1);
		Tpost[5] = glm::mat4(-8.74228e-08, 0, 1, 0, 0, 1, 0, 0, -2, 0, -4.37114e-08, 0, -1, -2, 0, 1);	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();
		PskyBox.create();

		for(int i = 0; i < 6; i++) {
			DS1[i].init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		}
		DSG.init(this, &DSL1, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &TG},
				{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});
		DSskyBox.init(this, &DSLskyBox, {
					{0, UNIFORM, sizeof(SkyboxUniformBufferObject), nullptr},
					{1, TEXTURE, 0, &TskyBox}
				});
		txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		PskyBox.cleanup();
		
		for(int i = 0; i < 6; i++) {
			DS1[i].cleanup();
		}
		DSG.cleanup();
		DSskyBox.cleanup();
		
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1.cleanup();
		TG.cleanup();
		for(int i = 0; i < 6; i++) {
			M1[i].cleanup();
		}		
		MG.cleanup();

		DSL1.cleanup();
		
		TskyBox.cleanup();
		MskyBox.cleanup();
		DSLskyBox.cleanup();
		
		P1.destroy();
		PskyBox.destroy();
		
		txt.localCleanup();
		
		// for(int i = 0; i < 6; i++) {
			// delete AsTr[i];
		// }
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

		P1.bind(commandBuffer);
		for(int i = 0; i < 6; i++) {
			M1[i].bind(commandBuffer);
			DS1[i].bind(commandBuffer, P1, currentImage);
						
			vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M1[i].indices.size()), 1, 0, 0, 0);
		}
		MG.bind(commandBuffer);
		DSG.bind(commandBuffer, P1, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MG.indices.size()), 1, 0, 0, 0);

		PskyBox.bind(commandBuffer);
		MskyBox.bind(commandBuffer);
		DSskyBox.bind(commandBuffer, PskyBox, currentImage);
						
		// property .indices.size() of models, contains the number of triangles * 3 of the mesh.
		vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(MskyBox.indices.size()), 1, 0, 0, 0);

		txt.populateCommandBuffer(commandBuffer, currentImage);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();
		static float lastTime = 0.0f;
		
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>
					(currentTime - startTime).count();
		float deltaT = time - lastTime;
		lastTime = time;

		const float ROT_SPEED = glm::radians(60.0f);
		const float MOVE_SPEED = 5.0f;

		const float MOUSE_RES = 10.0f;
		
		static double old_xpos = 0, old_ypos = 0;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		double m_dx = xpos - old_xpos;
		double m_dy = ypos - old_ypos;
		old_xpos = xpos; old_ypos = ypos;
		
		static float debounce = false;
		static int curDebounce = 0;
//std::cout << xpos << " " << ypos << " " << m_dx << " " << m_dy << "\n";

		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   deltaT * (float)m_dx * ROT_SPEED / MOUSE_RES,
							   glm::vec3(CamDir[1])) * glm::mat4(CamDir));
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   deltaT * (float)m_dy * ROT_SPEED / MOUSE_RES,
							   glm::vec3(CamDir[0])) * glm::mat4(CamDir));
		}

		if(glfwGetKey(window, GLFW_KEY_LEFT)) {
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   deltaT * ROT_SPEED,
							   glm::vec3(CamDir[1])) * glm::mat4(CamDir));
		}
		if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   -deltaT * ROT_SPEED,
							   glm::vec3(CamDir[1])) * glm::mat4(CamDir));
		}
		if(glfwGetKey(window, GLFW_KEY_UP)) {
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   deltaT * ROT_SPEED,
							   glm::vec3(CamDir[0])) * glm::mat4(CamDir));
		}
		if(glfwGetKey(window, GLFW_KEY_DOWN)) {
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   -deltaT * ROT_SPEED,
							   glm::vec3(CamDir[0])) * glm::mat4(CamDir));
		}
		if(glfwGetKey(window, GLFW_KEY_Q)) {
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   deltaT * ROT_SPEED,
							   glm::vec3(CamDir[2])) * glm::mat4(CamDir));
		}
		if(glfwGetKey(window, GLFW_KEY_E)) {
			CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f),
							   -deltaT * ROT_SPEED,
							   glm::vec3(CamDir[2])) * glm::mat4(CamDir));
		}
		
		if(glfwGetKey(window, GLFW_KEY_A)) {
			CamPos -= MOVE_SPEED * glm::vec3(CamDir[0]) * deltaT;
		}
		if(glfwGetKey(window, GLFW_KEY_D)) {
			CamPos += MOVE_SPEED * glm::vec3(CamDir[0]) * deltaT;
		}
		if(glfwGetKey(window, GLFW_KEY_S)) {
			CamPos += MOVE_SPEED * glm::vec3(CamDir[2]) * deltaT;
		}
		if(glfwGetKey(window, GLFW_KEY_W)) {
			CamPos -= MOVE_SPEED * glm::vec3(CamDir[2]) * deltaT;
		}
		if(glfwGetKey(window, GLFW_KEY_F)) {
			CamPos -= MOVE_SPEED * glm::vec3(CamDir[1]) * deltaT;
		}
		if(glfwGetKey(window, GLFW_KEY_R)) {
			CamPos += MOVE_SPEED * glm::vec3(CamDir[1]) * deltaT;
		}
		
		if(glfwGetKey(window, GLFW_KEY_P)) {
			if(!debounce) {
				printVec3("CamPos", CamPos);
				printMat3("CamDir", CamDir);
				
				debounce = true;
				curDebounce = GLFW_KEY_P;
			}
		} else {
			if((curDebounce == GLFW_KEY_P) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
//			glfwDestroyWindow(window);
			glfwSetWindowShouldClose(window, GL_TRUE);
		}


// std::cout << "Cam Pos: " << CamPos[0] << " " << CamPos[1] << " " << CamPos[2] << "\n";

		glm::mat4 CamMat = glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos);
					
		glm::mat4 Prj = glm::perspective(glm::radians(45.0f),
						swapChainExtent.width / (float) swapChainExtent.height,
						0.1f, 50.0f);
		Prj[1][1] *= -1;
					
					
		UniformBufferObject ubo{};
		glm::mat4 baseTr = glm::rotate(glm::mat4(1.0f),
								cos(time / 3.0f) * glm::radians(30.0f),
								glm::vec3(0.0f, 1.0f, 0.0f));
								
		// Here is where you actually update your uniforms

		// updates global uniforms
		GlobalUniformBufferObject gubo{};
		gubo.lightDir = glm::vec3(cos(glm::radians(135.0f)), sin(glm::radians(135.0f)), 0.0f);
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = CamPos;
		for(int i = 0; i < 6; i++) {
			ubo.mMat = baseTr * Tpre[i] * AsTr[i] * Tpost[i];
			ubo.mvpMat = Prj * CamMat * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS1[i].map(currentImage, &ubo, sizeof(ubo), 0);
			DS1[i].map(currentImage, &gubo, sizeof(gubo), 2);
		}
		ubo.mMat = baseTr;
		ubo.mvpMat = Prj * CamMat * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DSG.map(currentImage, &ubo, sizeof(ubo), 0);
		DSG.map(currentImage, &gubo, sizeof(gubo), 2);
		
		// update Skybox uniforms
		SkyboxUniformBufferObject sbubo{};
		sbubo.mMat = glm::mat4(1.0f);
		sbubo.nMat = glm::mat4(1.0f);
		sbubo.mvpMat = Prj * glm::transpose(glm::mat4(CamDir));
		DSskyBox.map(currentImage, &sbubo, sizeof(sbubo), 0);


	}
	
	public:
	void Matrix(int i, glm::mat4 M) {
		if((i >= 1) && (i <= 6)) {
			AsTr[i-1] = M;
		}
	}
};

#include "transforms.hpp"

// This is the main: probably you do not need to touch this!
int main() {
    Assignment03 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}