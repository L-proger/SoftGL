/*
#ifndef Mesh_h__
#define Mesh_h__
#include "VertexBuffer.h"
#include <vector>
#include "Renderer3D.h"

struct Section
{
	int faceStart;
	int faceCount;
};
struct DataSource
{
	VertexBuffer* vb;
	std::vector<Section> sections;
};
class Mesh
{
public:
	Mesh(Renderer3D* renderer);
	~Mesh();
	void AddSource(VertexBuffer* vb);
	void AddSource(void* data, int count, int stride);
	void AddSection(int source, int faceStart, int faceCount);
	void DrawSection(int num);
private:
	std::vector<DataSource> sources;
	Renderer3D* pRenderer;
	int submeshCount;
    InputLayout* layout;
};
#endif // Mesh_h__
*/
