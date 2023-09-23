#pragma once

#include "imesh.h"
#include <LMath/lmath.h>

#include <iostream>

using namespace lm;

template<typename TVertex, typename TIndex>
struct CubeGenerator {
	static constexpr size_t VertexCount = 24;
	static constexpr size_t IndexCount = 36;

	static constexpr float4 Corner(size_t id) {
		return float4(id % 2 == 0 ? -1.0f : 1.0f, id < 4 ? 1.0f : -1.0f, (id % 4) < 2 ? 1.0f : -1.0f, 1.0f);
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
			float3(1.0f,0.0f,0.0f),
			float3(-1.0f,0.0f,0.0f),
			float3(0.0f,1.0f,0.0f),
			float3(0.0f,-1.0f,0.0f),
			float3(0.0f,0.0f,1.0f),
			float3(0.0f,0.0f,-1.0f)
		};

		static constexpr float2 uvs[4] = {
			float2(0.0f,0.0f),
			float2(1.0f,0.0f),
			float2(1.0f,1.0f),
			float2(0.0f,1.0f)
		};

		static constexpr size_t indexMap[6] = {
			0,1,3,3,1,2
		};

		auto vertices = (TVertex*)vb->GetPointer();
		auto indices = (TIndex*)ib->GetPointer();

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
