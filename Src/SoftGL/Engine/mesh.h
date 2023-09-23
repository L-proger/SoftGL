#ifndef mesh_h__
#define mesh_h__

#include "imesh.h"
#include <array>

template<size_t SubmeshCount>
class Mesh : public IMesh {
public:
	Buffer* vertexBuffer;
	std::array<Buffer*, SubmeshCount> submeshes;

	virtual Buffer* GetVertexBuffer() override {
		return vertexBuffer;
	}

	virtual size_t GetSubmeshCount() const override {
		return SubmeshCount;
	}

	virtual Buffer* GetSubmeshBuffer(size_t id) const override {
		return submeshes[id];
	}

};



#endif // mesh_h__
