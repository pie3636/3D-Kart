#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include "CubeRenderable.hpp"
#include "HierarchicalTorusRenderable.hpp"
#include "CylinderRenderable.hpp"
#include "MeshRenderable.hpp"
#include "FrameRenderable.hpp"
#include "KeyframedKartRenderable.hpp"
#include "QuadRenderable.hpp"
#include "SphereRenderable.hpp"
#include "IndexedCubeRenderable.hpp"

#include "Viewer.hpp"
#include "ShaderProgram.hpp"
#include "Utils.hpp"


#include "lighting/DirectionalLightRenderable.hpp"
#include "lighting/SpotLightRenderable.hpp"

#include "texturing/TexturedLightedMeshRenderable.hpp"
#include "texturing/TexturedPlaneRenderable.hpp"
#include "texturing/TexturedLightedMeshRenderable.hpp"

#include "dynamics/ConstantForceField.hpp"
#include "dynamics/EulerExplicitSolver.hpp"
#include "dynamics/KartRenderable.hpp"
#include "dynamics/DampingForceField.hpp"
#include "dynamics/ConstantForceFieldRenderable.hpp"
#include "dynamics/ControlledForceFieldRenderable.hpp"
#include "dynamics/DynamicSystem.hpp"
#include "dynamics/DynamicSystemRenderable.hpp"

#include "log.hpp"
#include <fstream>

class Scene {
	public:
  		Scene(Viewer* viewer);
  		~Scene();
		static float times[16];

  private:
  		KartPtr mobile;
  		static const double kartScaleFactor;
		static int kartCount;
  		Viewer *viewer;
  		ShaderProgramPtr flatShader;
  		ShaderProgramPtr texShader;
  		DynamicSystemPtr dynSystem;
  		DynamicSystemRenderablePtr systemRenderable;

		KeyframedKartRenderablePtr createTexturedMovingKartFromMesh		 ();
		CylinderRenderablePtr    		 createCharacterFromPrimitives   ();
		KartRenderablePtr				 createControllableKart			 ();

		void kart_game_light	();
		void kart_game_borders	();
		void kart_game_road();

		KeyframedKartRenderablePtr moving_kart(KeyframedKartRenderablePtr root,
											   ShaderProgramPtr program,
											   const std::string& mesh_filename,
											   const std::string& texture_filename );


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
