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

#pragma comment(lib, R"(D:\softgl\Debug\softgl.lib)")

int main() {
	int sx = 640;
	int sy = 480;

	printf("Test1\n");

	RenderWindow* wnd = new RenderWindow();
	wnd->SetSize(640, 480);
	wnd->SetCaption("LOL");
	wnd->CenterWindow();

	//default back buffer
	auto backBuffer = new Texture2D(sx, sy, 4);
	//default depth buffer
	depthBuffer = new Texture2D(width, height, 4);

	std::string tex_path = R"(C:\Users\Sergey\Desktop\test.bmp)";
	Texture2D* tex = new Texture2D(tex_path.c_str());

	BlockRasterizer rasterizer(sx, sy, wnd);

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

	InputElement elements[] =
	{
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

	FFPFog fog;
	fog.start = 4.0f;
	fog.end = 11.0f;
	fog.enabled = true;
	fog.color = Vector3D(0.0f, 1.0f, 0.0f);

	rasterizer.SetFog(fog);

	int frames = 0;
	float angle = 0.0f;

	rasterizer.SetTexture(tex, 0);

	while (true) {
		//update world matrix rotation
		Mat4x4RotationY(angle, mWorld);
		rasterizer.SetWorldMatrix(mWorld);
		mWVP = mWorld * mView * mProj;

		rasterizer.Clear(0x000000ff);

		/*auto bb = rasterizer.GetBackBuffer();
		uint32_t* pixels = (uint32_t*)bb->getBuffer()->getDataPtr();
		for (size_t y = 0; y < bb->height; ++y) {
		for (size_t x = 0; x < bb->width; ++x) {
		uint32_t xVal = (x * 0xff) / bb->width;
		uint32_t yVal = (y * 0xff) / bb->height;
		pixels[y * bb->width + x] = ((xVal & 0xff) << 16) | ((yVal & 0xff) << 8);
		}
		}*/

		rasterizer.Draw(0, 9);
		rasterizer.Present();

		frames++;
		//fpsCounter.ComputeFPS();
		angle += 0.1f;
		if (angle >100000.0f)
			angle = 0.0f;
		wnd->Update();
	}

	getchar();
	return 0;
}
