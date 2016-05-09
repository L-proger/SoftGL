#ifndef plane_generator_h__
#define plane_generator_h__

#include "imesh.h"
#include "lmath.h"

using namespace lm;

template<typename TVertex, typename TIndex>
struct PlaneGenerator {
	static constexpr size_t VertexCount = 4;
	static constexpr size_t IndexCount = 6;

	static constexpr float4 Corner(size_t id) {
		return float4(id % 2 == 0 ? -1.0f : 1.0f, 0, (id % 4) < 2 ? 1.0f : -1.0f, 1.0f);
	}

	static bool Generate(IMesh* m) {
		if (m == nullptr) {
			return false;
		}

		auto vb = m->GetVertexBuffer();
		if (vb == nullptr || (vb->Size() / vb->ItemSize()) < VertexCount) {
			return false;
		}

		auto ib = m->GetSubmeshBuffer(0);
		if (ib == nullptr || (ib->Size() / ib->ItemSize()) < IndexCount) {
			return false;
		}

		static constexpr std::array<float3, 6> normals = {
			float3(1,0,0),
			float3(-1,0,0),
			float3(0,1,0),
			float3(0,-1,0),
			float3(0,0,1),
			float3(0,0,-1)
		};

		static constexpr float2 uvs[4] = {
			float2(0,0),
			float2(1,0),
			float2(0,1),
			float2(1,1)
		};

		static constexpr size_t planeIndices[6] = {
			0,1,2,2,1,3
		};

		auto vertices = (TVertex*)vb->GetPointer();
		auto indices = (TIndex*)ib->GetPointer();

		//update vertices
		for (size_t j = 0; j < 4; ++j) {
			auto& vertex = vertices[j];
			vertex.Position = Corner(j);
			vertex.Normal = float3::up();
			vertex.UV0 = uvs[j];
		}

		for (size_t j = 0; j < 6; ++j) {
			indices[j] = planeIndices[j];
		}
		

		return true;
	}
};
#endif // plane_generator_h__
