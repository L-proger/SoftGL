#include <stdio.h>

#include "../SoftGL/LMath.h"
#include "../SoftGL/LString.h"
#include "../SoftGL/Buffer.h"
#include "../SoftGL/FFP.h"
#include "../SoftGL/IRenderWindow.h"
#include "../SoftGL/Plane.h"
#include "../SoftGL/Texture2D.h"
#include "../SoftGL/BlockRasterizer.h"
#include "../SoftGL/RenderWindow.h"
#include "../SoftGL/Vertex.h"
#include "../SoftGL/texture_utils.h"

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
	Matrix4x4 mWVP;

	Mat4x4Identity(&mWorld);
	Mat4x4LookAtLH(Vector3D(0.0f, 1.5f, -3.5f), Vector3D(0.0f, 0.5f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f), mView);
	Mat4x4PerspectiveFOV(3.1415f / 4.0f, (float)sx / (float)sy, 0.1f, 100.0f, mProj);

	mWVP = mWorld * mView * mProj;

	Vertex geom[3 * 3];

	geom[0] = Vertex(Vector4D(1.0f, 0.0f, -1.0f, 1.0f), Vector2D(1.0f, 1.0f));
	geom[1] = Vertex(Vector4D(0.0f, 1.0f, 0.0f, 1.0f), Vector2D(0.0f, 0.0f));
	geom[2] = Vertex(Vector4D(1.0f, 0.0f, 1.0f, 1.0f), Vector2D(1.0f, 0.0f));

	geom[3] = Vertex(Vector4D(-1.0f, 0.0f, -1.0f, 1.0f), Vector2D(0.0f, 1.0f));
	geom[4] = Vertex(Vector4D(-1.0f, 0.0f, 1.0f, 1.0f), Vector2D(0.0f, 0.0f));
	geom[5] = Vertex(Vector4D(1.0f, 0.0f, -1.0f, 1.0f), Vector2D(1.0f, 1.0f));

	geom[6] = Vertex(Vector4D(1.0f, 0.0f, -1.0f, 1.0f), Vector2D(1.0f, 1.0f));
	geom[7] = Vertex(Vector4D(-1.0f, 0.0f, 1.0f, 1.0f), Vector2D(0.0f, 0.0f));
	geom[8] = Vertex(Vector4D(1.0f, 0.0f, 1.0f, 1.0f), Vector2D(1.0f, 0.0f));

	VertexBuffer* vb = new VertexBuffer(sizeof(geom), Vertex::stride());

	vb->Write(&geom[0], 0, sizeof(geom)); //FIX IT!  Check for overflow!!

	InputElement elements[] = {
		InputElement(FFP_VERTEXELEMENT_POSITION, 0, RT_FLOAT4, 0),
		InputElement(FFP_VERTEXELEMENT_TEXCOORD, 16, RT_FLOAT2, 0)
	};


	InputLayout* layout = new InputLayout(elements, 2);

	rasterizer.SetInputLayout(layout);
	rasterizer.SetVertexBuffer(vb, 0);

	rasterizer.SetPrimitiveType(PT_TRIANGLE_LIST);

	rasterizer.SetWorldMatrix(mWorld);
	rasterizer.SetViewMatrix(mView);
	rasterizer.SetProjectionMatrix(mProj);


	int frames = 0;
	float angle = 0.0f;

	rasterizer.SetTexture(tex, 0);

	rasterizer.set_color_buffer(backBuffer);
	rasterizer.set_depth_buffer(depthBuffer);

	while (true) {
		//update world matrix rotation
		Mat4x4RotationY(angle, mWorld);
		rasterizer.SetWorldMatrix(mWorld);
		mWVP = mWorld * mView * mProj;

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
