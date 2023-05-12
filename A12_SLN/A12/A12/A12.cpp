// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "TextMaker.hpp"


 std::vector<SingleText> demoText = {
	{1, {"Scene", "", "", ""}, 0, 0},
	{1, {"Sample spheres", "", "", ""}, 0, 0}
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


struct VertexClassic {
	glm::vec3 pos;
	glm::vec2 UV;
	glm::vec3 norm;
};

struct VertexNormMap {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec2 texCoord;
};

class Assignment12;
void GameLogic(Assignment12 *A, float Ar, glm::mat4 &ViewPrj, glm::mat4 &World);

// MAIN ! 
class Assignment12 : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1, DSL2, DSL3, DSL4;

	// Pipelines [Shader couples]
	VertexDescriptor VDClassic, VDNormMap;
	Pipeline P1, P2, P3, P4;

	// Models, textures and Descriptors (values assigned to the uniforms)
	Model<VertexClassic> M1, M2, M3, SphereC;
	Model<VertexNormMap> M4, M5, M6, M7, SphereNM;
	Texture T1_2, T3, T4, T4_NM, T4_Mat, T5, T5_NM, T5_Mat, T6, T6_NM, T6_Mat, T7, T7_NM, T7_Mat;
	DescriptorSet DS1, DS2, DS3, DS4, DS5, DS6, DS7;

	TextMaker txt;
	
	// Other application parameters
	float Ar;
	glm::mat4 ViewPrj;
	glm::vec3 Pos[2] = {glm::vec3(0.9,0,3), glm::vec3(0,0,6)};
	glm::vec3 cameraPos;
	float Yaw[2] = {glm::radians(0.0f), 0.0f};
	float Pitch[2] = {glm::radians(0.0f), 0.0f};
	float Roll[2] = {glm::radians(0.0f), 0.0f};
	
	int currScene = 1;

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Assignment 12";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 0.6f, 0.8f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 15;
		texturesInPool = 16;
		setsInPool = 8;
		
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
		DSL2.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
				  });
		DSL3.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
				  });
		DSL4.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
					{4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
				  });
				  
		// Vertex descriptors
		VDClassic.init(this, {
				  {0, sizeof(VertexClassic), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexClassic, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexClassic, norm),
				         sizeof(glm::vec3), NORMAL},
				  {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexClassic, UV),
				         sizeof(glm::vec2), UV}
				});

		VDNormMap.init(this, {
				  {0, sizeof(VertexNormMap), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexNormMap, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexNormMap, normal),
				         sizeof(glm::vec3), NORMAL},
				  {0, 2, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(VertexNormMap, tangent),
				         sizeof(glm::vec4), TANGENT},
				  {0, 3, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexNormMap, texCoord),
				         sizeof(glm::vec2), UV}
				});

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, &VDClassic, "shaders/ClassicVert.spv", "shaders/PhongFrag.spv", {&DSL1});
		P2.init(this, &VDClassic, "shaders/ClassicVert.spv", "shaders/OrenNayarFrag.spv", {&DSL2});
		P3.init(this, &VDNormMap, "shaders/NormMapVert.spv", "shaders/BlinnNormMapFrag.spv", {&DSL3});
		P4.init(this, &VDNormMap, "shaders/NormMapVert.spv", "shaders/GGXNormMapFrag.spv", {&DSL4});

		// Models, textures and Descriptors (values assigned to the uniforms)
		M1.init(this, &VDClassic, "models/Floor.obj", OBJ);
		M2.init(this, &VDClassic, "models/PoolTableObj.obj", OBJ);
		M3.init(this, &VDClassic, "models/PoolTableFloor.obj", OBJ);
		SphereC.init(this, &VDClassic, "models/Sphere.obj", OBJ);
		M4.init(this, &VDNormMap, "models/PoolTable.gltf", GLTF);
		M5.init(this, &VDNormMap, "models/StatuePedestal.gltf", GLTF);
		M6.init(this, &VDNormMap, "models/StatueLightBody.gltf", GLTF);
		M7.init(this, &VDNormMap, "models/StatueDarkBody.gltf", GLTF);
		SphereNM.init(this, &VDNormMap, "models/Sphere.gltf", GLTF);
/*		
		{
			for(int i = 0; i < SphereNM.vertices.size(); i++) {
				std::cout << "Vert: " << i << "\n";
				printVec3("Pos", SphereNM.vertices[i].pos);
				printVec3("Norm", SphereNM.vertices[i].normal);
				printVec4("Tan", SphereNM.vertices[i].tangent);
				printVec2("UV", SphereNM.vertices[i].texCoord);
			}
		}
*/
/*		{
			for(int i = 0; i < M5.vertices.size(); i++) {
				std::cout << "Vert: " << i << "\n";
				printVec3("Pos", M5.vertices[i].pos);
				printVec3("Norm", M5.vertices[i].normal);
				printVec4("Tan", M5.vertices[i].tangent);
				printVec2("UV", M5.vertices[i].texCoord);
			}
		}
*/
		
		T1_2.init(this, "textures/TexturesInterior.png");
		T3.init(this, "textures/Grass_07_basecolor.png");
		T4.init(this, "textures/Wood_01_basecolor.png");
		T4_NM.init(this, "textures/Wood_01_normal.png", VK_FORMAT_R8G8B8A8_UNORM);
		T4_Mat.init(this, "textures/Wood_01_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
		T5.init(this, "textures/Marble_08_basecolor.png");
		T5_NM.init(this, "textures/Marble_08_normal.png", VK_FORMAT_R8G8B8A8_UNORM);
		T5_Mat.init(this, "textures/Marble_08_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
		T6.init(this, "textures/Metals_09_basecolor.png");
		T6_NM.init(this, "textures/Metals_09_normal.png", VK_FORMAT_R8G8B8A8_UNORM);
		T6_Mat.init(this, "textures/Metals_09_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
		T7.init(this, "textures/Metals_10_basecolor.png");
		T7_NM.init(this, "textures/Metals_10_normal.png", VK_FORMAT_R8G8B8A8_UNORM);
		T7_Mat.init(this, "textures/Metals_10_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);

		txt.init(this, &demoText);
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P1.create();
		P2.create();
		P3.create();
		P4.create();

		DS1.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1_2},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		DS2.init(this, &DSL1, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1_2},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		DS3.init(this, &DSL2, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T3},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
				});
		DS4.init(this, &DSL3, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T4},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{3, TEXTURE, 0, &T4_NM},
					{4, TEXTURE, 0, &T4_Mat}
				});
		DS5.init(this, &DSL3, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T5},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{3, TEXTURE, 0, &T5_NM},
					{4, TEXTURE, 0, &T5_Mat}
				});
		DS6.init(this, &DSL4, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T6},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{3, TEXTURE, 0, &T6_NM},
					{4, TEXTURE, 0, &T6_Mat}
				});
		DS7.init(this, &DSL4, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T7},
					{2, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{3, TEXTURE, 0, &T7_NM},
					{4, TEXTURE, 0, &T7_Mat}
				});

		txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P1.cleanup();
		P2.cleanup();
		P3.cleanup();
		P4.cleanup();
		
		DS1.cleanup();
		DS2.cleanup();
		DS3.cleanup();
		DS4.cleanup();
		DS5.cleanup();
		DS6.cleanup();
		DS7.cleanup();
		
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		T1_2.cleanup();
		T3.cleanup();
		T4.cleanup();
		T4_NM.cleanup();
		T4_Mat.cleanup();
		T5.cleanup();
		T5_NM.cleanup();
		T5_Mat.cleanup();
		T6.cleanup();
		T6_NM.cleanup();
		T6_Mat.cleanup();
		T7.cleanup();
		T7_NM.cleanup();
		T7_Mat.cleanup();
		
		M1.cleanup();
		M2.cleanup();
		M3.cleanup();
		SphereC.cleanup();
		M4.cleanup();
		M5.cleanup();
		M6.cleanup();
		M7.cleanup();
		SphereNM.cleanup();

		DSL1.cleanup();
		DSL2.cleanup();
		DSL3.cleanup();
		DSL4.cleanup();
		
		P1.destroy();		
		P2.destroy();		
		P3.destroy();		
		P4.destroy();		
		
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
		M2.bind(commandBuffer);
		DS2.bind(commandBuffer, P1, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);


		P2.bind(commandBuffer);
		M3.bind(commandBuffer);
		DS3.bind(commandBuffer, P2, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M3.indices.size()), 1, 0, 0, 0);


		P3.bind(commandBuffer);

		M4.bind(commandBuffer);
		DS4.bind(commandBuffer, P3, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M4.indices.size()), 1, 0, 0, 0);
		M5.bind(commandBuffer);
		DS5.bind(commandBuffer, P3, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M5.indices.size()), 1, 0, 0, 0);


		P4.bind(commandBuffer);

		M6.bind(commandBuffer);
		DS6.bind(commandBuffer, P4, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M6.indices.size()), 1, 0, 0, 0);
		M7.bind(commandBuffer);
		DS7.bind(commandBuffer, P4, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M7.indices.size()), 1, 0, 0, 0);
		
		break;
	  case 1:
		P1.bind(commandBuffer);
		SphereC.bind(commandBuffer);
		DS1.bind(commandBuffer, P1, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(SphereC.indices.size()), 1, 0, 0, 0);

		P2.bind(commandBuffer);
		DS3.bind(commandBuffer, P2, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(SphereC.indices.size()), 1, 0, 0, 0);

		P3.bind(commandBuffer);
		SphereNM.bind(commandBuffer);
		DS4.bind(commandBuffer, P3, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(SphereNM.indices.size()), 1, 0, 0, 0);
		DS5.bind(commandBuffer, P3, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(SphereNM.indices.size()), 1, 0, 0, 0);

		P4.bind(commandBuffer);
		DS6.bind(commandBuffer, P4, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(SphereNM.indices.size()), 1, 0, 0, 0);
		DS7.bind(commandBuffer, P4, currentImage);	
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(SphereNM.indices.size()), 1, 0, 0, 0);
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
		
		GlobalUniformBufferObject gubo{};
		gubo.lightDir = glm::vec3(cos(glm::radians(135.0f)) * cos(glm::radians(210.0f)), sin(glm::radians(135.0f)), cos(glm::radians(135.0f)) * sin(glm::radians(210.0f)));
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = cameraPos;

		switch(currScene) {
		  case 0:
			DS1.map(currentImage, &ubo, sizeof(ubo), 0);
			DS1.map(currentImage, &gubo, sizeof(gubo), 2);

			DS2.map(currentImage, &ubo, sizeof(ubo), 0);
			DS2.map(currentImage, &gubo, sizeof(gubo), 2);

			DS3.map(currentImage, &ubo, sizeof(ubo), 0);
			DS3.map(currentImage, &gubo, sizeof(gubo), 2);

			ubo.mMat = glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(1,0,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS4.map(currentImage, &ubo, sizeof(ubo), 0);
			DS4.map(currentImage, &gubo, sizeof(gubo), 2);

			ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(1.8,0,0)) *
					   glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(1,0,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS5.map(currentImage, &ubo, sizeof(ubo), 0);
			DS5.map(currentImage, &gubo, sizeof(gubo), 2);

			DS6.map(currentImage, &ubo, sizeof(ubo), 0);
			DS6.map(currentImage, &gubo, sizeof(gubo), 2);

			DS7.map(currentImage, &ubo, sizeof(ubo), 0);
			DS7.map(currentImage, &gubo, sizeof(gubo), 2);
			break;

		  case 1:
			ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(-3,2.25,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			DS1.map(currentImage, &ubo, sizeof(ubo), 0);
			DS1.map(currentImage, &gubo, sizeof(gubo), 2);

			ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(0,2.25,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			DS3.map(currentImage, &ubo, sizeof(ubo), 0);
			DS3.map(currentImage, &gubo, sizeof(gubo), 2);

			ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(3,2.25,0)) *
					   glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(1,0,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS4.map(currentImage, &ubo, sizeof(ubo), 0);
			DS4.map(currentImage, &gubo, sizeof(gubo), 2);

			ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(-3,-.25,0)) *
					   glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(1,0,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS5.map(currentImage, &ubo, sizeof(ubo), 0);
			DS5.map(currentImage, &gubo, sizeof(gubo), 2);

			ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(0,-.25,0)) *
					   glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(1,0,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS6.map(currentImage, &ubo, sizeof(ubo), 0);
			DS6.map(currentImage, &gubo, sizeof(gubo), 2);

			ubo.mMat = glm::translate(glm::mat4(1), glm::vec3(3,-.25,0)) *
					   glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(1,0,0));
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS7.map(currentImage, &ubo, sizeof(ubo), 0);
			DS7.map(currentImage, &gubo, sizeof(gubo), 2);
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
		glm::vec3 ux = glm::rotate(glm::mat4(1.0f), Yaw[currScene], glm::vec3(0,1,0)) * glm::vec4(1,0,0,1);
		glm::vec3 uz = glm::rotate(glm::mat4(1.0f), Yaw[currScene], glm::vec3(0,1,0)) * glm::vec4(0,0,-1,1);
		Pos[currScene] = Pos[currScene] + MOVE_SPEED * m.x * ux * deltaT;
		Pos[currScene] = Pos[currScene] + MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
		Pos[currScene].y = Pos[currScene].y < 0.0f ? 0.0f : Pos[currScene].y;
		Pos[currScene] = Pos[currScene] + MOVE_SPEED * m.z * uz * deltaT;
		// Rotation
		Yaw[currScene] = Yaw[currScene] - ROT_SPEED * deltaT * r.y;
		Pitch[currScene] = Pitch[currScene] + ROT_SPEED * deltaT * r.x;
		Pitch[currScene] = Pitch[currScene] < minPitch ? minPitch :
				   (Pitch[currScene] > maxPitch ? maxPitch : Pitch[currScene]);
		Roll[currScene]   = Roll[currScene]   - ROT_SPEED * deltaT * r.z;
		Roll[currScene]   = Roll[currScene] < glm::radians(-175.0f) ? glm::radians(-175.0f) :
				   (Roll[currScene] > glm::radians( 175.0f) ? glm::radians( 175.0f) : Roll[currScene]);

//std::cout << Pos.x << ", " << Pos.y << ", " << Pos.z << ", " << Yaw << ", " << Pitch << ", " << Roll << "\n";

		// Final world matrix computaiton
		World = glm::translate(glm::mat4(1), Pos[currScene]) * glm::rotate(glm::mat4(1.0f), Yaw[currScene], glm::vec3(0,1,0));
		
		// Projection
		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		// View
		// Target
		glm::vec3 target = Pos[currScene] + glm::vec3(0.0f, camHeight, 0.0f);

		// Camera position, depending on Yaw parameter, but not character direction
		cameraPos = World * glm::vec4(0.0f, camHeight + camDist * sin(Pitch[currScene]), camDist * cos(Pitch[currScene]), 1.0);
		// Damping of camera
		glm::mat4 View = glm::rotate(glm::mat4(1.0f), -Roll[currScene], glm::vec3(0,0,1)) *
						 glm::lookAt(cameraPos, target, glm::vec3(0,1,0));

		ViewPrj = Prj * View;
	}
};


// This is the main: probably you do not need to touch this!
int main() {
    Assignment12 app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}