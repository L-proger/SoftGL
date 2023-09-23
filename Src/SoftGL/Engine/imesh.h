#pragma once

#include <SoftGL/Buffer.h>
#include <SoftGL/static_vector.h>

class IMesh {
public:
	virtual Buffer* GetVertexBuffer() = 0;
	virtual size_t GetSubmeshCount() const = 0;
	virtual Buffer* GetSubmeshBuffer(size_t id) const = 0;
};
