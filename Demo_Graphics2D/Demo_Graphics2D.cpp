#include "lmath.h"
#include "RenderWindow.h"
#include "Texture.h"
#include "StaticTexture.h"
#include "Graphics2D.h"

#pragma comment(lib, "softgl.lib")

using namespace lm;

static constexpr size_t sx = 640;
static constexpr size_t sy = 480;
alignas(32) MipChain<sizeof(uint32_t), sx, sy, 0> bb_data;

int main()
{
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

	TextureWriter writer;
	writer.texture = &backBuffer;


	 
	typedef lm::Vector<uint8_t, 4> Color;
	float time = 0.0f;
	while (true)
	{
		time += 0.001f;
		wnd->Update();


		auto ms = matrix4x4Scale<float>(1, 1, 1);
		auto mr = matrix4x4RotationY(Quaternion_f::angle_axis(time, float3(0.0f, 1.0f, 0.0f)));

		auto world = lm::mul(ms, mr);

		auto view = lm::matrix4x4LookatLh(float3(0.0f, 1.5f, -2.0f), float3(0.0f), float3(0.0f, 1.0f, 0.0f));
		auto projection = lm::matrix4x4Perspective<float>(3.1415f / 4.0f, (float)sx / (float)sy, 0.1f, 100.0f);

		float4x4 vw = lm::mul(view, world);
		float4x4 pvw = lm::mul(projection, vw);


		clear(writer, Color(0));

		float cubeSize = 1.0f;
		float cubeExtent = cubeSize * 0.5f;

		float3 points[] = {
			float3(-cubeExtent, cubeExtent, cubeExtent),
			float3(-cubeExtent, cubeExtent, -cubeExtent),
			float3(-cubeExtent, -cubeExtent, -cubeExtent),
			float3(-cubeExtent, -cubeExtent, cubeExtent),

			float3(cubeExtent, cubeExtent, cubeExtent),
			float3(cubeExtent, cubeExtent, -cubeExtent),
			float3(cubeExtent, -cubeExtent, -cubeExtent),
			float3(cubeExtent, -cubeExtent, cubeExtent) };

		int indices[] = {0,1,1,2,2,3,3,0,  4,5,5,6,6,7,7,4, 0,4, 1,5, 2,6, 3,7};


		for (int i = 0; i < sizeof(points) / sizeof(points[i]); ++i) {
			float4 position(points[i], 1.0f);
			float4 projected = lm::mul(pvw, position);
			projected /= projected[3];
			points[i] = projected.slice<0,3>();
			points[i][1] = -points[i][1];
			points[i] = ((points[i] * 0.5f) + 0.5f);

			points[i][0] *= sx;
			points[i][1] *= sy;
		}

		for (int i = 0; i < sizeof(indices) / sizeof(indices[i]); i += 2) {
			float3 p0 = points[indices[i]];
			float3 p1 = points[indices[i + 1]];
			drawLine((int32_t)p0[0], (int32_t)p0[1], (int32_t)p1[0], (int32_t)p1[1], writer, Color((uint8_t)255U, (uint8_t)255U, (uint8_t)255U, (uint8_t)255U));
		}

		drawCircle(320, 240, 100, writer, Color((uint8_t)255, (uint8_t)255, (uint8_t)255, (uint8_t)255));

		

		wnd->Present(&backBuffer);
		Sleep(1);
	}
    return 0;
}

