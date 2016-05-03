/*
#include "BlockRasterizer.h"
#include "RenderWindow.h"
#include "Timer.h"
#include <stdio.h>
#include <iostream>
#include "FpsCounter.h"
#include "LMath.h"

#include "VSDefault.h"
#include "PSDefault.h"
#include "PSTextured.h"

#include "Vertex.h"

int main()
{
	FpsCounter fpsCounter;

	int sx = 800;
	int sy = 600;

	RenderWindow w;
	w.SetSize(sx, sy);
	w.SetCaption("Tile based software renderer by .L");

	BlockRasterizer rasterizer(sx, sy, (HWND)w.GetWindowHandle());


	Matrix4x4 mWorld;
	Matrix4x4 mView;
	Matrix4x4 mProj;
	Matrix4x4 mWVP;

	Mat4x4Identity(&mWorld);
	Mat4x4LookAtLH(Vector3D(0.0f, 1.5f, -3.5f), Vector3D(0.0f, 0.5f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f), mView);
	Mat4x4PerspectiveFOV(3.1415f / 4.0f, (float)sx / (float)sy, 0.1f, 100.0f, mProj);

	mWVP = mWorld * mView * mProj;


	VertexBuffer* vb = new VertexBuffer(Vertex::stride() * 6, Vertex::stride());
	Vertex geom[6];
	geom[0] = Vertex(Vector4D(-1.0f, 0.0f, 0.0f, 1.0f), Vector2D(0.0f, 1.0f));
	geom[2] = Vertex(Vector4D( 0.0f, 1.0f, 0.0f, 1.0f), Vector2D(0.5f, 0.0f));
	geom[1] = Vertex(Vector4D( 1.0f, 0.0f, 0.0f, 1.0f), Vector2D(1.0f, 1.0f));

	geom[3] = Vertex(Vector4D(-2.0f, 0.0f, 0.0f, 1.0f), Vector2D(0.0f, 1.0f));
	geom[4] = Vertex(Vector4D( 0.0f, 0.0f, 30.0f, 1.0f), Vector2D(0.5f, 0.0f));
	geom[5] = Vertex(Vector4D( 2.0f, 0.0f, 0.0f, 1.0f), Vector2D(1.0f, 1.0f));

	vb->Write(&geom[0], 0, Vertex::stride() * 6); //FIX IT!  Check for overflow!!

	InputElement elements[] = 
	{
		InputElement(FFP_VERTEXELEMENT_POSITION, 0, RT_FLOAT4, 0),
		InputElement(FFP_VERTEXELEMENT_TEXCOORD, 16, RT_FLOAT2, 0)
	};


	InputLayout* layout = new InputLayout(elements, 2);
	VSDefault* vs = new VSDefault();
	PSTextured* ps = new PSTextured();

	
	//rasterizer.SetVertexShader(vs);
	//rasterizer.SetPixelShader(ps);
	rasterizer.SetInputLayout(layout);
	rasterizer.SetVertexBuffer(vb, 0);
	
	rasterizer.SetPrimitiveType(PT_TRIANGLE_LIST);

	vs->mView = mView;
	vs->mProj = mProj;
	vs->mWorld = mWorld;

	rasterizer.SetWorldMatrix(mWorld);
	rasterizer.SetViewMatrix(mView);
	rasterizer.SetProjectionMatrix(mProj);

	FFPFog fog;
	fog.start = 5.0f;
	fog.end = 11.0f;
	fog.enabled = true;
	fog.color = Vector3D(0.0f, 1.0f, 0.0f);

	rasterizer.SetFog(fog);
	
	Texture2D* tex = new Texture2D("grass_height.bmp");

	ps->diffuseTex = tex;
	rasterizer.diffuse = tex;
	int frames = 0;
	float angle = 0.0f;
	while(true)
	{
		//update world matrix rotation
		Mat4x4RotationY(angle,mWorld);
		rasterizer.SetWorldMatrix(mWorld);
		mWVP = mWorld * mView * mProj;
		vs->mWorld = mWorld;
		rasterizer.clear(0x000000ff);
		rasterizer.Draw(3, 3);
		rasterizer.present();
		w.Update();
		ps->appTime= angle;

		fpsCounter.ComputeFPS();
		angle += fpsCounter.GetTimeElapsed();
		if(angle >100000.0f)
			angle = 0.0f;
		frames++;
		if(frames >= 1000)
		{
			printf("FPS: %10.2f\r", fpsCounter.AverageFPS());
			frames = 0;
		}
		
	}
	return 0;
}*/
