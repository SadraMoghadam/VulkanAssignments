// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"


// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBufferObject {
	alignas(16) float time;
};

class Assignment10;

// MAIN ! 
class Assignment10 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1{};

	// Pipelines [Shader couples]
	Pipeline P1{};

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model M{};
	DescriptorSet DS;
	
	// Other application parameters
	glm::vec3 CamPos = glm::vec3(0.0, 1.5, 7.0);
	float CamAlpha = 0.0f;
	float CamBeta = 0.0f;
	float Ar;

	std::vector<float> vPos;
	std::vector<int> vIdx;
		
	
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment 10";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.3f, 0.3f, 0.3f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 2;
		texturesInPool = 1;
		setsInPool = 1;
		
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
//std::cout << "DSL\n";
		DSL1.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
				  });

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
//std::cout << "P1\n";
		P1.init(this, "shaders/PhongVert.spv", "shaders/PhongFrag.spv", {&DSL1});
//std::cout << "P1.set\n";
		P1.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL,
 								    VK_CULL_MODE_NONE, false);

		// Models, textures and Descriptors (values assigned to the uniforms)
		
		std::cout << "create Mesh\n";
		createMesh();
		
		M.BP = this;
		
		for(int i = 0; i < vPos.size(); i+=8) {
				Vertex vertex{};
				vertex.pos = {vPos[i], vPos[i+1], vPos[i+2]};				
				vertex.texCoord = {vPos[i+3], vPos[i+4]};
				vertex.norm = {vPos[i+5], vPos[i+6], vPos[i+7]};				
				M.vertices.push_back(vertex);
		}
		for(int i = 0; i < vIdx.size(); i++) {
			if((vIdx[i] < 0) || (vIdx[i] >= M.vertices.size())) {
				std::cout << "Error! Index: " << i << " is outside range (" << vIdx[i] << ")\n";
				M.indices.push_back(0);
			} else {
				M.indices.push_back(vIdx[i]);
			}
		}

		M.createVertexBuffer();
		M.createIndexBuffer();
		std::cout << "Created model: V=" << M.vertices.size() << ", I=" << M.indices.size() << "\n";
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();

		DS.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		
		DS.cleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		M.cleanup();

		DSL1.cleanup();
		
		P1.destroy();		
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

		P1.bind(commandBuffer);
		M.bind(commandBuffer);
		DS.bind(commandBuffer, P1, currentImage);
					
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M.indices.size()), 1, 0, 0, 0);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		const float ROT_SPEED = glm::radians(120.0f);
		const float MOVE_SPEED = 2.0f;

		static float debounce = false;
		static int curDebounce = 0;

		float deltaT;
		static float L_time = 0.0;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);
		L_time += deltaT;

		CamAlpha = CamAlpha - ROT_SPEED * deltaT * r.y;
		CamBeta  = CamBeta  - ROT_SPEED * deltaT * r.x;
		CamBeta  =  CamBeta < glm::radians(-90.0f) ? glm::radians(-90.0f) :
				   (CamBeta > glm::radians( 90.0f) ? glm::radians( 90.0f) : CamBeta);

		glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(1,0,0,1);
		glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(0,0,-1,1);
		CamPos = CamPos + MOVE_SPEED * m.x * ux * deltaT;
		CamPos = CamPos + MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
		CamPos = CamPos + MOVE_SPEED * m.z * uz * deltaT;
		
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		glm::mat4 M = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 50.0f);
		M[1][1] *= -1;

		glm::mat4 Mv =  glm::rotate(glm::mat4(1.0), -CamBeta, glm::vec3(1,0,0)) *
						glm::rotate(glm::mat4(1.0), -CamAlpha, glm::vec3(0,1,0)) *
						glm::translate(glm::mat4(1.0), -CamPos);

		glm::mat4 ViewPrj =  M * Mv;
		UniformBufferObject ubo{};
		glm::mat4 baseTr = glm::mat4(1.0f);
								
		// Here is where you actually update your uniforms

		// updates global uniforms
		GlobalUniformBufferObject gubo{};
		gubo.time = L_time;

		ubo.mMat = baseTr;
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS.map(currentImage, &ubo, sizeof(ubo), 0);
		DS.map(currentImage, &gubo, sizeof(gubo), 1);
	}
		
	void createMesh() {
		const int resX = 100;
		const int resZ = 100;
		const float halfSizeX = 2.5;
		const float halfSizeZ = 2.5;
		
		for(int i = 0; i <= resX; i++) {
			for(int j = 0; j <= resZ; j++) {
				float u = (float)i / float(resX);
				float v = (float)j / float(resZ);
				float x = (2 * u - 1.0) * halfSizeX;
				float z = (2 * v - 1.0) * halfSizeZ;
				
				vPos.push_back(x); vPos.push_back(0.0); vPos.push_back(z);	// vertex 0
				vPos.push_back(u); vPos.push_back(v);	//UV
				vPos.push_back(0.0); vPos.push_back(1.0); vPos.push_back(0.0);	// Norm
				
				if((i < resX) && (j < resZ)) {
					int r1 = j * (resX + 1);
					int r2 = (j + 1) * (resX + 1);
					vIdx.push_back(r1+i);   vIdx.push_back(r1+i+1); vIdx.push_back(r2+i);
					vIdx.push_back(r1+i+1); vIdx.push_back(r2+i);   vIdx.push_back(r2+i+1);
				}
			}
		}
		
	}
};

// This is the main: probably you do not need to touch this!
int main() {
    Assignment10 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}