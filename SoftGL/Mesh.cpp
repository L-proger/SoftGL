/*
#include "Mesh.h"

Mesh::Mesh(Renderer3D* renderer)
	:submeshCount(0)
{
	pRenderer = renderer;
}
Mesh::~Mesh()
{

}
void Mesh::AddSource(VertexBuffer* vb)
{
	DataSource src;
	src.vb = vb;
	sources.push_back(src);
}
void Mesh::AddSource(void* data, int count, int stride)
{

}
void Mesh::AddSection(int source, int faceStart, int faceCount)
{
	Section s;
	s.faceStart = faceStart;
	s.faceCount = faceCount;

	sources[source].sections.push_back(s);

	submeshCount++;
}
void Mesh::DrawSection(int num)
{

}*/