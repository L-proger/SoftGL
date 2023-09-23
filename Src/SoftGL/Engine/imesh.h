#ifndef imesh_h__
#define imesh_h__

#include "Buffer.h"
#include "static_vector.h"

class IMesh {
public:
	virtual Buffer* GetVertexBuffer() = 0;
	virtual size_t GetSubmeshCount() const = 0;
	virtual Buffer* GetSubmeshBuffer(size_t id) const = 0;
};
#endif // imesh_h__
