#include "vld.h"
#include "LString.h"
#include "Buffer.h"
#include "IRenderWindow.h"
#include "Texture2D.h"
#include "BlockRasterizer.h"
#include "RenderWindow.h"
#include "Vertex.h"
#include "texture_utils.h"
#include "VSDefault.h"
#include "PSDefault.h"
#include "StaticBuffer.h"
#include <array>
#include "lmath.h"
#include "FpsCounter.h"
#include "transform.h"
#include "gameobject.h"
#include "Camera.h"
#include "Input.h"
#include "camera_controller.h"
#include "stopwatch.h"
#include "mesh.h"
#include "cube_generator.h"
#include "plane_generator.h"
#include "PsNormalMap.h"
#include <iostream>
#include "mip_buffer.h"

#pragma comment(lib, "lframework.lib")
#pragma comment(lib, "softgl.lib")
#pragma comment(lib, "vld.lib")

using namespace LFramework;

void DrawMesh(BlockRasterizer* rasterizer, IMesh* mesh){
	rasterizer->SetVertexBuffer(mesh->GetVertexBuffer(), 0, mesh->GetVertexBuffer()->ItemSize());
	for (size_t i = 0; i < mesh->GetSubmeshCount(); ++i){
		auto ib = mesh->GetSubmeshBuffer(i);
		rasterizer->SetIndexBuffer(ib, 0);
		rasterizer->DrawIndexed(ib->Size() / ib->ItemSize(), 0);
	}
}

void BlitDepthBuffer(Texture2D* depthBuffer, Texture2D* colorBuffer)
{
	auto src_ptr = (float*)depthBuffer->getBuffer()->GetPointer();
	auto dst_ptr = (uint32_t*)colorBuffer->getBuffer()->GetPointer();

	for(size_t y = 0; y < depthBuffer->height; ++y){
		for (size_t x = 0; x < depthBuffer->width; ++x) {
			auto id = y * depthBuffer->width + x;
			auto src = src_ptr[id];
			uint32_t value = (uint32_t)(std::pow((src - 0.75f) * 4, 2) * 255.0f);
			uint32_t color = value | (value << 8) | (value << 16);
			dst_ptr[id] = color;
		}
	}
}


template<typename _PixelType, size_t _Width, size_t _Height>
struct MipStaticBuffer : public static_buffer<_PixelType, _Width * _Height>, public MipBuffer {
public:
	static constexpr size_t Width = _Width;
	static constexpr size_t Height = _Height;
	virtual size_t GetWigth() override { return Width; }

	virtual size_t GetHeight() override { return Height; }

	virtual MipBuffer* GetChild() override { return &childMip; }

	virtual Buffer* GetData() override { return this; }

private:
	static constexpr uint32_t ChildWidth = Width / 2 > 0 ? Width / 2 : 1;
	static constexpr uint32_t ChildHeight = Height / 2 > 0 ? Height / 2 : 1;
	MipStaticBuffer<_PixelType, ChildWidth, ChildHeight> childMip;
};


template<typename _PixelType>
struct MipStaticBuffer<_PixelType,1,1> : public static_buffer<_PixelType, 1>, public MipBuffer {
public:
	size_t GetWigth() override { return 1; }

	size_t GetHeight() override { return 1; }

	MipBuffer* GetChild() override { return nullptr; }

	virtual Buffer* GetData() override { return this; }
};

MipStaticBuffer<uint8_t, 16, 32> mipData;

static void PrintMips(MipBuffer* mip){
	if(mip != nullptr){
		std::cout << "Mip: " << mip->GetWigth() << "x" << mip->GetHeight() << std::endl;
		PrintMips(mip->GetChild());
	}
}

int main()
{
	auto input = Input::Instance();
	auto keyboard = input->keyboards()[1];
	auto mouse = input->mice()[0];

	PrintMips(&mipData);

	Texture2D* tex_normal = new Texture2D(R"(C:\Users\Sergey\Desktop\normal.bmp)");
	Texture2D* tex_diffuse = new Texture2D(R"(C:\Users\Sergey\Desktop\diffuse.bmp)");
	Texture2D* tex_ao = new Texture2D(R"(C:\Users\Sergey\Desktop\ao.bmp)");

	Game_object go;
	Camera camera(&go);
	camera.SetZFar(30);
	CameraController camController(&camera);

	go.transform.SetLocalPosition(float3(0, 1, -10.0f));

	int sx = 640;
	int sy = 480;

	RenderWindow* wnd = new RenderWindow();
	wnd->SetSize(sx, sy);
	wnd->SetCaption("SoftGL");
	wnd->CenterWindow();

	auto backBuffer = new Texture2D(sx, sy, 4);
	auto depthBuffer = new Texture2D(sx, sy, 4);

	Mesh<1> plane;
	static_buffer<Vertex, PlaneGenerator<Vertex, indices_t>::VertexCount> quad_vb;
	static_buffer<indices_t, PlaneGenerator<Vertex, indices_t>::IndexCount> quad_ib;
	plane.vertexBuffer = &quad_vb;
	plane.submeshes[0] = &quad_ib;


	PlaneGenerator<Vertex, indices_t>::Generate(&plane);

	BlockRasterizer rasterizer;


	static_buffer<Vertex, CubeGenerator<Vertex, indices_t>::VertexCount> vertex_buffer;
	static_buffer<indices_t, CubeGenerator<Vertex, indices_t>::IndexCount> index_buffer;

	Mesh<1> mesh;
	mesh.vertexBuffer = &vertex_buffer;
	mesh.submeshes[0] = &index_buffer;

	CubeGenerator<Vertex, indices_t>::Generate(&mesh);

	StaticInputLayout<3> layout;
	layout.elements = {
		InputElement("POSITION", 0, RegType::float4, 0),
		InputElement("NORMAL", 16, RegType::float3, 0),
		InputElement("TEXCOORD", 28, RegType::float2, 0)
	};

	rasterizer.SetInputLayout(&layout);
	rasterizer.SetPrimitiveType(PT_TRIANGLE_LIST);


	int frames = 0;
	float angle = 0.0f;

	rasterizer.set_color_buffer(backBuffer);
	rasterizer.set_depth_buffer(depthBuffer);

	auto vs = VSDefault();
	auto ps = PSDefault();

	auto psNormal = PsNormalMap();

	ps.diffuse_map = tex_diffuse;
	psNormal.diffuse_map = tex_diffuse;
	psNormal.normal_map = tex_normal;
	psNormal.ao_map = tex_ao;

	rasterizer.SetVertexShader(&vs);
	rasterizer.SetPixelShader(&ps);

	FpsCounter fps;
	Stopwatch sw;

	while (true){
		float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(sw.Check()).count() / 1000000.0f;
		sw.Reset();
		wnd->Update();
		input->strobe();
		camController.Tick(deltaTime);

		//clear render targets
		texture_utils::fill<uint32_t>(backBuffer, 0x00232327);
		texture_utils::fill<float>(depthBuffer, 1.0f);

		//setup material

		ps.camPosition = camera.GameObject()->transform.get_global_position();

		vs.mView = camera.world_to_camera_matrix();
		vs.mProj = camera.GetProjection();
		vs.mWorld = matrix4x4_rotation<float>(angle);
		rasterizer.SetPixelShader(&ps);
		//draw mesh
		DrawMesh(&rasterizer, &mesh);

		psNormal.camPosition = ps.camPosition;
		rasterizer.SetPixelShader(&psNormal);
		vs.mWorld = matrix4x4_scale<float>(10,10,10);
		DrawMesh(&rasterizer, &plane);

		//present
		wnd->Present(backBuffer);

		frames++;
		angle += fps.GetTimeElapsed();
		fps.ComputeFPS();
		std::cout << fps.PreciseFPS() << std::endl;
	}

	getchar();
	return 0;
}
