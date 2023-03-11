// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "TextMaker.hpp"

 std::vector<SingleText> demoText = {
	{1, {"Isometric", "", "", ""}, 0, 0},
	{1, {"Dimetric", "", "", ""}, 0, 0},
	{1, {"Trimetric", "", "", ""}, 0, 0},
	{1, {"Cabinet", "", "", ""}, 0, 0},
	};



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

class Assignment04;
void SetupProjectionMatrices(Assignment04 *A, float Ar);

// MAIN ! 
class Assignment04 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;

	// Pipelines [Shader couples]
	Pipeline P1;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model M1, MG;
	Texture T1, TG;
	DescriptorSet DS1, DSG;
	glm::mat4 Prj[4];
	
	TextMaker txt;
	
	// Other application parameters
	int currPrj;
	

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment 02";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 5;
		texturesInPool = 3;
		setsInPool = 3;
	}
	
	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		std::cout << "Window resized to: " << w << " x " << h << "\n";
		SetupProjectionMatrices(this, (float)w / (float)h);
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
		M1.init(this, "models/QBert.obj");
		MG.init(this, "models/LargePlane.obj");
		
		T1.init(this, "textures/QBertColors.png");
		TG.init(this, "textures/3DLABbg_UV_Map_Checker_03_2048x2048.jpg");
				
		txt.init(this, &demoText);
		
		SetupProjectionMatrices(this, 4.0/3.0);
		
		currPrj = 0;
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();

		DS1.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		DSG.init(this, &DSL1, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &TG},
				{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
			});
		txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		
		DS1.cleanup();
		DSG.cleanup();
		
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1.cleanup();
		TG.cleanup();
		M1.cleanup();
		MG.cleanup();

		DSL1.cleanup();
		
		P1.destroy();
		
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
		M1.bind(commandBuffer);
		DS1.bind(commandBuffer, P1, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);
		MG.bind(commandBuffer);
		DSG.bind(commandBuffer, P1, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MG.indices.size()), 1, 0, 0, 0);


		txt.populateCommandBuffer(commandBuffer, currentImage, currPrj);
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

		if(glfwGetKey(window, GLFW_KEY_SPACE)) {
			if(!debounce) {
				debounce = true;
				curDebounce = GLFW_KEY_SPACE;
				currPrj = (currPrj+1) % 4;
				std::cout << "Projection : " << currPrj << "\n";
				RebuildPipeline();
			}
		} else {
			if((curDebounce == GLFW_KEY_SPACE) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}

		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
//			glfwDestroyWindow(window);
			glfwSetWindowShouldClose(window, GL_TRUE);
		}


		glm::mat4 CamMat = glm::mat4(1); 				
		UniformBufferObject ubo{};
		glm::mat4 baseTr = glm::mat4(1.0f);
								
		// Here is where you actually update your uniforms

		// updates global uniforms
		GlobalUniformBufferObject gubo{};
		gubo.lightDir = glm::vec3(cos(glm::radians(135.0f)), sin(glm::radians(135.0f)), 0.0f);
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = glm::vec3(100.0, 100.0, 100.0);

		ubo.mMat = baseTr * glm::translate(glm::mat4(1), glm::vec3(-3.5, +0.5, -0.5)) *
							glm::scale(glm::mat4(1), glm::vec3(0.5,0.5,0.5));
		ubo.mvpMat = Prj[currPrj] * CamMat * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS1.map(currentImage, &ubo, sizeof(ubo), 0);
		DS1.map(currentImage, &gubo, sizeof(gubo), 2);

		ubo.mMat = baseTr * glm::translate(glm::mat4(1), glm::vec3(0, -3.0, 0));
		ubo.mvpMat = Prj[currPrj] * CamMat * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DSG.map(currentImage, &ubo, sizeof(ubo), 0);
		DSG.map(currentImage, &gubo, sizeof(gubo), 2);
		


	}
	
	public:
	void Matrix(int i, glm::mat4 M) {
		if((i >= 1) && (i <= 4)) {
			Prj[i-1] = M;
		}
	}
};

#include "projection.hpp"

// This is the main: probably you do not need to touch this!
int main() {
    Assignment04 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}