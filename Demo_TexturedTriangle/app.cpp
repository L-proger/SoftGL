#include "vld.h"
#include <stdio.h>
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
#include <iostream>
#include "transform.h"
#include "gameobject.h"
#include "Camera.h"
#include "Input.h"
#include "camera_controller.h"
#include "stopwatch.h"

#pragma comment(lib, "lframework.lib")

#if defined(_DEBUG)
#pragma comment(lib, R"(D:\github\softgl\Debug\softgl.lib)")
#else
#pragma comment(lib, R"(D:\github\softgl\Release\softgl.lib)")
#endif
#pragma comment(lib, "vld.lib")

#include <FpsCounter.h>

using namespace LFramework;

int main() {
	auto input = Input::Instance();
	auto keyboard = input->keyboards()[1];
	auto mouse = input->mice()[0];

	std::string tex_path = R"(C:\Users\Sergey\Desktop\tile2.bmp)";
	Texture2D* tex = new Texture2D(tex_path.c_str());

	Game_object go;
	Camera camera(&go);

	CameraController camController(&camera);

	go.transform.set_localPosition(float3(0.0f, 1.0f, -3.0f));
	//go.transform.set_local_rotation(Quaternion_f::angle_axis(3.1415f / 7.0f, float3(1, 0, 0)));

	int sx = 640;
	int sy = 480;

	RenderWindow* wnd = new RenderWindow();
	wnd->SetSize(sx, sy);
	wnd->SetCaption("SoftGL");
	wnd->CenterWindow();

	auto backBuffer = new Texture2D(sx, sy, 4);
	auto depthBuffer = new Texture2D(sx, sy, 4);



	BlockRasterizer rasterizer;

	float4x4 mWorld;
	float4x4 mProj;

	mWorld = float4x4::identity();
	mProj = matrix4x4_perspective(3.1415f / 4.0f, (float)sx / (float)sy, 0.1f, 100.0f);

	static_buffer<Vertex, 5> vertex_buffer({
		Vertex(float4(-1.0f, 0.0f, -1.0f, 1.0f), float2(0.0f, 1.0f)),
		Vertex(float4(-1.0f, 0.0f, 1.0f, 1.0f), float2(0.0f, 0.0f)),
		Vertex(float4(1.0f, 0.0f, 1.0f, 1.0f), float2(1.0f, 0.0f)),
		Vertex(float4(1.0f, 0.0f, -1.0f, 1.0f), float2(1.0f, 1.0f)),
		Vertex(float4(0.0f, 1.0f, 0.0f, 1.0f), float2(1.0f, 0.5f)),
	});

	static_buffer<indices_t, 9> index_buffer({0,1,2, 0,2,3, 0,1,4});

	/*static_buffer<Vertex, 4> vertex_buffer({
		Vertex(float4(-1.0f, -1.0f, 0, 1.0f), float2(0.0f, 1.0f)),
		Vertex(float4(-1.0f, 1.0f, 0, 1.0f), float2(0.0f, 0.0f)),
		Vertex(float4(1.0f, 1.0f, 0, 1.0f), float2(1.0f, 0.0f)),
		Vertex(float4(1.0f, -1.0f, 0, 1.0f), float2(1.0f, 1.0f)),
	});

	static_buffer<indices_t, 6> index_buffer({ 0,1,2, 0,2,3 });*/
	

	StaticInputLayout<2> layout;
	layout.elements = {
		InputElement("POSITION", 0, RegType::float4, 0),
		InputElement("TEXCOORD", 16, RegType::float2, 0)
	};

	rasterizer.SetInputLayout(&layout);
	rasterizer.SetVertexBuffer(&vertex_buffer, 0, Vertex::stride());
	rasterizer.SetIndexBuffer(&index_buffer, 0);
	rasterizer.SetPrimitiveType(PT_TRIANGLE_LIST);

	int frames = 0;
	float angle = 0.0f;

	rasterizer.SetTexture(tex, 0);

	rasterizer.set_color_buffer(backBuffer);
	rasterizer.set_depth_buffer(depthBuffer);

	auto vs = new VSDefault();
	auto ps = new PSDefault();

	ps->diffuse_map = tex;

	rasterizer.SetVertexShader(vs);
	rasterizer.SetPixelShader(ps);

	FpsCounter fps;

	Stopwatch sw;
	
	while (true) {
		auto deltaTime = sw.CheckMs().count() / 1000.0f;
		sw.Reset();

		input->strobe();
		
		camController.Tick(deltaTime);

		//update world matrix rotation
		fps.ComputeFPS();
		std::cout << fps.PreciseFPS() << std::endl;
		vs->mView = camera.world_to_camera_matrix();
		vs->mProj = mProj;
		vs->mWorld = matrix4x4_rotation<float>(angle);

		//texture_utils::fill<uint32_t>(backBuffer, 0x00232327);
		texture_utils::fill<uint32_t>(backBuffer, 0x00ff0000);

		texture_utils::fill<float>(depthBuffer, 1.0f);

		rasterizer.DrawIndexed(9, 0);
		wnd->Present(backBuffer);

		frames++;
		angle += fps.GetTimeElapsed();
		if (angle > 100000.0f)
			angle = 0.0f;
		wnd->Update();
	}

	getchar();
	return 0;
}
