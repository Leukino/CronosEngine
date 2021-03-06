#ifndef _CNOCTREE_H_
#define _CNOCTREE_H_

#include "GameObject/GameObject.h"

namespace Cronos {

	enum class NodeType { NONE = -1, ROOT, PARENT, CHILD };

	class CnOT_Node
	{
	public:

		CnOT_Node() {}
		CnOT_Node(math::AABB partitionSpace, NodeType nodeType, int maxObjectsInside);
		~CnOT_Node() {}

		//Node methods
		void Draw();
		void Split();
		void CleanUp();
		void CleanNodes();

		bool Insert(GameObject* GObj);
		void TakeOut(GameObject* GObject);

		//Getters 
		const bool IsChild()  const { return m_IsChild; }
		const math::AABB& GetCubicSpace() const { return m_CubicSpace; }

		std::vector<GameObject*> GetObjectsContained(const math::AABB cubicSpace);
		std::vector<GameObject*> GetObjectsContained(const math::Frustum cameraFrustum);

	private:
		
		CnOT_Node* m_Nodes = nullptr;
		int m_ChildsQuantity = 0, m_MaxObjectsInside = 0;

		bool m_IsChild = true;
		math::AABB m_CubicSpace;
		NodeType m_NodeType = NodeType::NONE;
		
		std::vector<GameObject*> GObjectsContained_Vector;
	};

	class CnOctree
	{
	public:

		CnOctree() {}
		CnOctree(math::AABB space, int maxObjectsInside);
		~CnOctree() {}

		//Octree Methods
		void Draw();
		void CleanUp();

		bool Insert(GameObject* GObject);
		void TakeOut(GameObject* GObject);

		//Getters
		std::vector<GameObject*> GetObjectsContained(const math::AABB cubicSpace);
		std::vector<GameObject*> GetObjectsContained(const math::Frustum cameraFrustum);
		//std::vector<GameObject*> GetObjectsContained(math::LineSegment& ray, std::map<float, const GameObject*>& vector, bool nearest = true);

		const bool IsSplitted() const { return isSplitted; }
		const math::AABB& GetCubicSpace() const { return m_Root->GetCubicSpace(); }

	private:

		bool isSplitted = false;
		CnOT_Node* m_Root = nullptr;
	};
}
#endif
