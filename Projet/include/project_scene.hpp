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
  		const double kartScaleFactor = 0.5;
  		static int kartCount;
  		Viewer *viewer;
  		ShaderProgramPtr flatShader;
  		ShaderProgramPtr texShader;
  		DynamicSystemPtr dynSystem;
  		DynamicSystemRenderablePtr systemRenderable;
  		
  		TexturedLightedMeshRenderablePtr createTexturedKartFromMesh      ();
		CylinderRenderablePtr    		 createCharacterFromPrimitives   ();
		
		void kart_game_light	();
		void kart_game_borders	();
  		
		glm::mat4 translate  (HierarchicalRenderablePtr obj, double x, double y, double z);
		glm::mat4 rotate     (HierarchicalRenderablePtr obj, float alpha, double x, double y, double z);
		glm::mat4 scale      (HierarchicalRenderablePtr obj, double x, double y, double z);

		glm::mat4 translate  (HierarchicalRenderablePtr obj, glm::vec3 vec);
		glm::mat4 rotate     (HierarchicalRenderablePtr obj, float alpha, glm::vec3 vec);
		glm::mat4 scale      (HierarchicalRenderablePtr obj, glm::vec3 vec);
		
		glm::mat4 translateParent    (HierarchicalRenderablePtr obj, double x, double y, double z);
		glm::mat4 rotateParent       (HierarchicalRenderablePtr obj, float alpha, double x, double y, double z);
		glm::mat4 scaleParent        (HierarchicalRenderablePtr obj, double x, double y, double z);

		glm::mat4 translateParent    (HierarchicalRenderablePtr obj, glm::vec3 vec);
		glm::mat4 rotateParent       (HierarchicalRenderablePtr obj, float alpha, glm::vec3 vec);
		glm::mat4 scaleParent        (HierarchicalRenderablePtr obj, glm::vec3 vec);
};

#endif // PROJECT_SCENE_H
