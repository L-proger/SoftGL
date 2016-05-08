#ifndef cube_generator_h__
#define cube_generator_h__

#include "imesh.h"
#include "lmath.h"

#include <iostream>

using namespace lm;

template<typename TVertex, typename TIndex>
struct CubeGenerator {
	static constexpr size_t VertexCount = 24;
	static constexpr size_t IndexCount = 36;

	static constexpr float4 Corner(size_t id) {
		return float4(id % 2 == 0 ? -1 : 1, id < 4 ? 1 : -1, (id % 4) < 2 ? 1 : -1, 1.0f);
	}

	static bool Generate(IMesh* m) {
		if (m == nullptr) {
			return false;
		}

		auto vb = m->GetVertexBuffer();
		if (vb == nullptr || (vb->size() / vb->item_size()) < VertexCount) {
			return false;
		}

		auto ib = m->GetSubmeshBuffer(0);
		if (ib == nullptr || (ib->size() / ib->item_size()) < IndexCount) {
			return false;
		}

		static constexpr float extent = 0.5f;

		
		// 0-1
		//2-3
		//
		// 4-5
		//6-7

		static constexpr std::array<size_t, 4> sides[6] = {
			{ 3,1,5,7 },
			{ 0,2,6,4 },
			{ 0,1,3,2 },
			{ 6,7,5,4 },
			{ 1,0,4,5 },
			{ 2,3,7,6 }
		};

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
			float2(1,1),
			float2(0,1)
		};

		static constexpr size_t indexMap[6] = {
			0,1,3,3,1,2
		};

		auto vertices = (TVertex*)vb->get_pointer();
		auto indices = (TIndex*)ib->get_pointer();

		for(size_t i = 0; i < 6; ++i){
			//update vertices
			for (size_t j = 0; j < 4; ++j) {
				auto& vertex = vertices[i * 4 + j];
				vertex.Position = Corner(sides[i][j]);
				vertex.Normal = normals[i];
				vertex.UV0 = uvs[j];
			}
			auto indexBase = i * 4;
			for (size_t j = 0; j < 6; ++j) {
				indices[i * 6 + j] = indexBase + indexMap[j];
			}
		}

		return true;
	}
};
#endif // cube_generator_h__
