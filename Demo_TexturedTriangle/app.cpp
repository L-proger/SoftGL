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

#if defined(_DEBUG)
#pragma comment(lib, R"(D:\github\softgl\Debug\softgl.lib)")
#else
#pragma comment(lib, R"(D:\github\softgl\Release\softgl.lib)")
#endif
#pragma comment(lib, "vld.lib")

#include "LMath_.h"

int main() {


	int sx = 640;
	int sy = 480;

	RenderWindow* wnd = new RenderWindow();
	wnd->SetSize(640, 480);
	wnd->SetCaption("LOL");
	wnd->CenterWindow();

	auto backBuffer = new Texture2D(sx, sy, 4);
	auto depthBuffer = new Texture2D(sx, sy, 4);

	std::string tex_path = R"(C:\Users\Sergey\Desktop\test.bmp)";
	Texture2D* tex = new Texture2D(tex_path.c_str());

	BlockRasterizer rasterizer;

	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;

	mWorld = float4x4::identity();
	mView = lm::matrix_lookat_lh(float3(0.0f, 1.5f, -3.5f), float3(0.0f, 0.5f, 0.0f), float3(0.0f, 1.0f, 0.0f));
	mProj = matrix_perspective(3.1415f / 4.0f, (float)sx / (float)sy, 0.1f, 100.0f);

	static_buffer<Vertex, 5> vertex_buffer({
		Vertex(float4(-1.0f, 0.0f, -1.0f, 1.0f), float2(0.0f, 1.0f)),
		Vertex(float4(-1.0f, 0.0f, 1.0f, 1.0f), float2(0.0f, 0.0f)),
		Vertex(float4(1.0f, 0.0f, 1.0f, 1.0f), float2(1.0f, 0.0f)),
		Vertex(float4(1.0f, 0.0f, -1.0f, 1.0f), float2(1.0f, 1.0f)),
		Vertex(float4(0.0f, 1.0f, 0.0f, 1.0f), float2(0.5f, 0.0f)),
	});

	static_buffer<indices_t, 9> index_buffer({0,1,2, 0,2,3, 0,1,4});

	StaticInputLayout<2> layout;
	layout.elements = {
		InputElement("POSITION", 0, RegType::float4, 0),
		InputElement("TEXCOORD", 16, RegType::float2, 0)
	};

	rasterizer.SetInputLayout(&layout);
	rasterizer.SetVertexBuffer(&vertex_buffer, 0, Vertex::stride());
	rasterizer.set_index_buffer(&index_buffer, 0);
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

	while (true) {
		//update world matrix rotation
		mWorld = matrix_rotation<float>(angle);

		vs->mView = mView;
		vs->mProj = mProj;
		vs->mWorld = mWorld;

		texture_utils::fill<uint32_t>(backBuffer, 0x000000ff);
		texture_utils::fill<float>(depthBuffer, 1.0f);

		rasterizer.DrawIndexed(9, 0);
		wnd->Present(backBuffer);

		frames++;
		angle += 0.1f;
		if (angle > 100000.0f)
			angle = 0.0f;
		wnd->Update();
	}

	getchar();
	return 0;
}
