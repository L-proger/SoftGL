#define WIN32_LEAN_AND_MEAN
#include "vld.h"
#include "LString.h"
#include "Buffer.h"
#include "IRenderWindow.h"
#include "Texture.h"
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
#include "StaticTexture.h"
#include <cstdint>
#include "shaders/textured_no_lit.h"
#include <windows.h>
#include <WinBase.h>

#pragma comment(lib, "lframework.lib")
#pragma comment(lib, "softgl.lib")

using namespace LFramework;

void DrawMesh(BlockRasterizer* rasterizer, IMesh* mesh){
	rasterizer->SetVertexBuffer(mesh->GetVertexBuffer(), 0, mesh->GetVertexBuffer()->ItemSize());
	for (size_t i = 0; i < mesh->GetSubmeshCount(); ++i){
		auto ib = mesh->GetSubmeshBuffer(i);
		rasterizer->SetIndexBuffer(ib, 0);
		rasterizer->DrawIndexed(ib->Size() / ib->ItemSize(), 0);
	}
}

static constexpr size_t sx = 1600;
static constexpr size_t sy = 900;

alignas(32) MipChain<sizeof(uint32_t), sx, sy, 0> bb_data;
alignas(32) MipChain<sizeof(float), sx, sy, 0> db_data;

/*bool isAvxSupportedByWindows() {
	const DWORD64 avxFeatureMask = XSTATE_MASK_LEGACY_SSE | XSTATE_MASK_GSSE;
	return GetEnabledExtendedFeatures(avxFeatureMask) == avxFeatureMask;
}*/

int main()
{
	auto input = Input::Instance();
	auto keyboard = input->keyboards()[1];
	auto mouse = input->mice()[0];

	//auto tex_normal = texture_utils::LoadTexture(R"(D:\L\development\Resources\Floors\normal.bmp)");
	//auto tex_diffuse = texture_utils::LoadTexture(R"(D:\L\development\Resources\Floors\diffuse.bmp)");
	//auto tex_ao = texture_utils::LoadTexture(R"(D:\L\development\Resources\Floors\ao.bmp)");

	Game_object go;
	Camera camera(&go);
	camera.SetZFar(30);
	CameraController camController(&camera);

	go.transform.SetLocalPosition(float3(0, 1, -10.0f));


	RenderWindow* wnd = new RenderWindow();
	wnd->SetSize(sx, sy);
	wnd->SetCaption("SoftGL");
	wnd->CenterWindow();

	TextureDescription desc;
	desc.Width = sx;
	desc.Height = sy;
	desc.BytesPerPixel = sizeof(uint32_t);
	desc.MipCount = bb_data.MipsCount;

	StaticTexture<decltype(&bb_data)> backBuffer(&bb_data, desc);
	StaticTexture<decltype(&db_data)> depthBuffer(&db_data, desc);

	Mesh<1> plane;
	static_buffer<Vertex, PlaneGenerator<Vertex, indices_t>::VertexCount> quad_vb;
	static_buffer<indices_t, PlaneGenerator<Vertex, indices_t>::IndexCount> quad_ib;
	plane.vertexBuffer = &quad_vb;
	plane.submeshes[0] = &quad_ib;


	PlaneGenerator<Vertex, indices_t>::Generate(&plane);

	BlockRasterizer rasterizer;

	StaticInputLayout<3> layout;
	layout.elements = {
		InputElement("POSITION", 0, RegType::float4, 0),
		InputElement("NORMAL", 16, RegType::float3, 0),
		InputElement("TEXCOORD", 28, RegType::float2, 0)
	};

	rasterizer.SetInputLayout(&layout);
	rasterizer.SetPrimitiveType(PT_TRIANGLE_LIST);



	rasterizer.set_color_buffer(&backBuffer);
	rasterizer.set_depth_buffer(&depthBuffer);

	auto vs = VSDefault();
	auto ps = PSTexturedNoLit();
	//ps.diffuseMap = tex_diffuse;


	rasterizer.SetVertexShader(&vs);
	rasterizer.SetPixelShader(&ps);
	
	FpsCounter<40> fps;
	Stopwatch sw;
	float threadTime = 0;
	int frames = 0;
	float angle = 0.0f;
	while (true){
		frames++;
		fps.ComputeFPS();
		threadTime += fps.GetFrameTimeSeconds();
		if (threadTime >= 1.0f) {
			std::cout << "FPS: " << (int)(frames / threadTime) << std::endl;
			threadTime = 0;
			frames = 0;
		}
		wnd->Update();
		input->strobe();
		camController.Tick(fps.GetFrameTimeSeconds());

		//clear render targets
		texture_utils::fill<uint32_t>(&backBuffer, 0x00ff0000 );//0x00232327
		//texture_utils::fill<float>(&depthBuffer, 1.0f);

		//setup material
		vs.mWorld = lm::mul(matrix4x4_scale<float>(10, 10, 10), matrix4x4_rotation(Quaternion_f::angle_axis(-3.1415f/2*0, float3(1,0,0))));
		vs.mView = camera.world_to_camera_matrix();
		vs.mProj = camera.GetProjection();
		
		//DrawMesh(&rasterizer, &plane);

		//present
		wnd->Present(&backBuffer);

		
		angle += fps.GetFrameTimeSeconds();
	}

	getchar();
	return 0;
}
