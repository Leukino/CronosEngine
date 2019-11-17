#ifndef _MESHCOMPONENT_H_
#define _MESHCOMPONENT_H_

#include "glm/glm.hpp"

#include "Component.h"
#include "Renderer/VertexArray.h"
#include "MaterialComponent.h"

namespace Cronos {

	struct CronosVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class MeshComponent : public Component
	{
	public:

		MeshComponent(GameObject* attachedGO);
		~MeshComponent();

		virtual void Update(float dt) override;
		void Draw(MaterialComponent* material, bool bindMaterial);

		void SetupMesh(std::vector<CronosVertex>vertices, std::vector<uint>indices);
		void RecalculateNormals() { m_NormalsCalculated = false; }
		bool &setDebugDraw() { RecalculateNormals(); return m_DebugDraw; }

		//Getters
		const std::vector<CronosVertex> GetVertexVector() const { return m_VertexVector; }
		const std::vector<uint> GetIndexVector() const { return m_IndicesVector; }
		static ComponentType GetType() { return ComponentType::MESH; }

	private:

		void CalculateNormals(std::vector<glm::vec3>& normals, std::vector<glm::vec3>& positions);

		void DrawVerticesNormals();
		void DrawPlanesNormals();
		void DrawCentralAxis();

	private:

		std::vector<CronosVertex> m_VertexVector;
		std::vector<uint> m_IndicesVector;

		VertexArray* m_MeshVAO = nullptr;
		VertexBuffer* m_MeshVBO = nullptr;
		IndexBuffer* m_MeshIBO = nullptr;

		bool m_DebugDraw = false;
		bool m_NormalsCalculated = false;
	};
}

#endif