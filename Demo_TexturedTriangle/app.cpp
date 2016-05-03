#include <stdio.h>

#include "LMath.h"
#include "LString.h"
#include "Buffer.h"
#include "IRenderWindow.h"
#include "Plane.h"
#include "Texture2D.h"
#include "BlockRasterizer.h"
#include "RenderWindow.h"
#include "Vertex.h"
#include "texture_utils.h"
#include "VSDefault.h"
#include "PSDefault.h"
#include "StaticBuffer.h"
#include <array>

#pragma comment(lib, R"(D:\github\softgl\Debug\softgl.lib)")

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

	Matrix4x4 mWorld;
	Matrix4x4 mView;
	Matrix4x4 mProj;

	Mat4x4Identity(&mWorld);
	Mat4x4LookAtLH(Vector3D(0.0f, 1.5f, -3.5f), Vector3D(0.0f, 0.5f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f), mView);
	Mat4x4PerspectiveFOV(3.1415f / 4.0f, (float)sx / (float)sy, 0.1f, 100.0f, mProj);

	static_buffer<Vertex, 9> vertex_buffer({
		/*Vertex(Vector4D(-1.0f, 0.0f, -1.0f, 1.0f), Vector2D(0.0f, 1.0f)),
		Vertex(Vector4D(-1.0f, 0.0f, 1.0f, 1.0f), Vector2D(0.0f, 0.0f)),
		Vertex(Vector4D(1.0f, 0.0f, 1.0f, 1.0f), Vector2D(1.0f, 0.0f)),
		Vertex(Vector4D(1.0f, 0.0f, -1.0f, 1.0f), Vector2D(1.0f, 1.0f)),
		Vertex(Vector4D(0.0f, 1.0f, 0.0f, 1.0f), Vector2D(0.0f, 0.0f)),*/


		Vertex(Vector4D(-1.0f, 0.0f, -1.0f, 1.0f), Vector2D(0.0f, 1.0f)),
		Vertex(Vector4D(-1.0f, 0.0f, 1.0f, 1.0f), Vector2D(0.0f, 0.0f)),
		Vertex(Vector4D(1.0f, 0.0f, 1.0f, 1.0f), Vector2D(1.0f, 0.0f)),

		Vertex(Vector4D(-1.0f, 0.0f, -1.0f, 1.0f), Vector2D(0.0f, 1.0f)),
		Vertex(Vector4D(1.0f, 0.0f, 1.0f, 1.0f), Vector2D(1.0f, 0.0f)),
		Vertex(Vector4D(1.0f, 0.0f, -1.0f, 1.0f), Vector2D(1.0f, 1.0f)),

		Vertex(Vector4D(-1.0f, 0.0f, -1.0f, 1.0f), Vector2D(0.0f, 1.0f)),
		Vertex(Vector4D(-1.0f, 0.0f, 1.0f, 1.0f), Vector2D(0.0f, 0.0f)),
		Vertex(Vector4D(0.0f, 1.0f, 0.0f, 1.0f), Vector2D(0.5f, 0.0f))
	});

	static_buffer<uint16_t, 9> index_buffer({0,1,2, 0,2,3, 0,1,4});

	InputElement elements[] = {
		InputElement("POSITION", 0, RT_FLOAT4, 0),
		InputElement("TEXCOORD", 16, RT_FLOAT2, 0)
	};

	InputLayout* layout = new InputLayout(elements, 2);

	rasterizer.SetInputLayout(layout);
	rasterizer.SetVertexBuffer(&vertex_buffer, 0, Vertex::stride());

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
		Mat4x4RotationY(angle, mWorld);

		vs->mView = mView;
		vs->mProj = mProj;
		vs->mWorld = mWorld;

		texture_utils::fill<uint32_t>(backBuffer, 0x000000ff);
		texture_utils::fill<float>(depthBuffer, 1.0f);

		rasterizer.Draw(0, 9);
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
