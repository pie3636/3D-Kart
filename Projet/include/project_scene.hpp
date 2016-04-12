#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include "Viewer.hpp"
#include "ShaderProgram.hpp"
#include "CubeRenderable.hpp"
#include "CylinderRenderable.hpp"
#include "MeshRenderable.hpp"
#include "texturing/TexturedLightedMeshRenderable.hpp"

#include "./dynamics/DynamicSystem.hpp"
#include "./dynamics/DynamicSystemRenderable.hpp"

class Scene {
	public:
  		Scene(Viewer* viewer);
  		~Scene();

  private:
  		static int kartCount;
  		Viewer *viewer;
  		ShaderProgramPtr flatShader;
  		ShaderProgramPtr texShader;
  		DynamicSystemPtr dynSystem;
  		DynamicSystemRenderablePtr systemRenderable;
  		
  		TexturedLightedMeshRenderablePtr createTexturedKartFromMesh      ();
		CubeRenderablePtr        		 createKartFromPrimitives        ();
		CylinderRenderablePtr    		 createCharacterFromPrimitives   ();
		
		void kart_game_light	();
		void kart_game_borders	();
  		
		glm::mat4 translate  (RenderablePtr obj, double x, double y, double z);
		glm::mat4 rotate     (RenderablePtr obj, float alpha, double x, double y, double z);
		glm::mat4 scale      (RenderablePtr obj, double x, double y, double z);

		glm::mat4 translate  (RenderablePtr obj, glm::vec3 vec);
		glm::mat4 rotate     (RenderablePtr obj, float alpha, glm::vec3 vec);
		glm::mat4 scale      (RenderablePtr obj, glm::vec3 vec);
};

#endif // PROJECT_SCENE_H
