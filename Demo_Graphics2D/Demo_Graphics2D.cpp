#include "lmath.h"
#include "RenderWindow.h"
#include "Texture.h"
#include "StaticTexture.h"
#include "Graphics2D.h"

#pragma comment(lib, "softgl.lib")

using namespace lm;

static constexpr size_t sx = 128;
static constexpr size_t sy = 64;
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

		auto world = lm::mul(matrix4x4_scale<float>(1, 1, 1), matrix4x4_rotation(Quaternion_f::angle_axis(time, float3(0, 1, 0))));

		auto view = lm::matrix4x4_lookat_lh(float3(0, 1.5, -2), float3(0), float3(0, 1, 0));
		auto projection = lm::matrix4x4_perspective<float>(3.1415 / 4.0f, (float)sx / (float)sy, 0.1f, 100.0f);

		float4x4 wv = lm::mul(world, view);
		float4x4 wvp = lm::mul(wv, projection);


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
			float4 projected = lm::mul(wvp, position);
			projected /= projected.w;
			points[i] = projected.xyz;
			points[i].y = -points[i].y;
			points[i] = ((points[i] * 0.5f) + 0.5f);

			points[i].x *= sx;
			points[i].y *= sy;
		}

		for (int i = 0; i < sizeof(indices) / sizeof(indices[i]); i += 2) {
			float3 p0 = points[indices[i]];
			float3 p1 = points[indices[i + 1]];

	
			drawLine((int32_t)p0.x, (int32_t)p0.y, (int32_t)p1.x, (int32_t)p1.y, writer, Color(255, 255, 255, 255));
		}

		/*int rays = 30;

		for (int i = 0; i < rays; ++i) {
			float angle = ((3.14159265358979f * 2.0f) / (float)rays) * (float)i;
			float radius = 100;
			float x = sinf(angle + time) * radius + 200;
			float y = cosf(angle + time) * radius + 200;

			drawLine(200, 200, x, y, writer, Color(255, 255, 255, 255));
		}*/

		

		wnd->Present(&backBuffer);
		Sleep(1);
	}
    return 0;
}

