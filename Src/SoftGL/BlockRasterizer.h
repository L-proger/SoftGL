#ifndef BlockRasterizer_h__
#define BlockRasterizer_h__

#include "Texture.h"
#include "Viewport.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <assert.h>
#include <math.h>
#include "Tools.h"
#include <vector>
#include "IRenderWindow.h"
#include "static_vector.h"
#include "RasterizerSettings.h"
#include <array>
#include "Debug.h"
#include "RegisterBlock.h"
#include <LMath/lmath.h>

using namespace lm;

enum PRIMITIVE_TYPE
{
	PT_TRIANGLE_LIST = 0,
	PT_TRIANGLE_STRIP = 1,
	PT_LINE_LIST = 2,
	PT_LINE_STRIP = 3,
	PT_POINT_LIST = 4

};
enum BLEND_SOURCE
{
	BLEND_ZERO = 0,
	BLEND_ONE = 1,
	BLEND_SRC_ALPHA = 3,
	BLEND_INV_SRC_ALPHA = 4
};
enum BLEND_OP
{
	BLEND_OP_ADD = 0
};
struct BlendState
{
	BLEND_SOURCE SrcAlpha;
	BLEND_SOURCE DstAlpha;
	BLEND_OP AlphaOperation;
	bool BlendEnable;
};
enum NDCZone
{
    ZONE_ZERO = 0,
    ZONE_LEFT = 1,
    ZONE_RIGHT = 2,
    ZONE_BOTTOM = 3,
    ZONE_TOP = 4,
    ZONE_FAR = 5,
    ZONE_NEAR = 6
};

static constexpr const char* NDCZoneStr[7]{
	"ZONE_ZERO",
	"ZONE_LEFT",
	"ZONE_RIGHT",
	"ZONE_BOTTOM",
	"ZONE_TOP",
	"ZONE_FAR",
	"ZONE_NEAR"
};

struct ClipFace
{
	RegisterBlock v0;
	RegisterBlock v1;
	RegisterBlock v2;
};


class BlockRasterizer {
public:
	typedef Plane<float> RasterizerPlane;
	typedef Static_vector<ClipFace, 9> ClipVector;

    BlockRasterizer();
    ~BlockRasterizer();
	bool ClipToFrustumPlane(RasterizerPlane plane, ClipVector& src, ClipVector& dst, size_t regCount);
    void ClipToFrustum(ClipFace face, ClipVector& dst, size_t regCount);
	Texture* GetBackBuffer();
	Texture* GetDepthBuffer();

	void setColorBuffer(Texture* buffer);
	void setDepthBuffer(Texture* buffer);

    void SetPrimitiveType(int type);
    void Draw(size_t offset, size_t length);
	void DrawIndexed(size_t index_count, size_t start_index_location);
    void FixupMapping();
    void SetVertexBuffer(Buffer* vb, size_t slot, size_t stride);
	void SetIndexBuffer(Buffer* ib, size_t slot);

    void SetInputLayout(IInputLayout* layout);
    void SetPixelShader(PixelShader* shader);
    void SetVertexShader(VertexShader* shader);
    void SetBlendState(BlendState* state);

    float GetBlendAlpha(BLEND_SOURCE src, const float4& srcColor, const float4& dstColor);
private:
	ClipVector cv1, cv2;
	void draw_impl(void* v0, void* v1, void* v2);

    IRenderWindow* render_window;
	std::array<RasterizerPlane, 6> NDCPlanes;

    uint32_t ConvertColor(const float4& color);
    float4 ConvertColor(uint32_t color);


	template<size_t _BlockSize, typename _Var>
	inline void CalcDerivatives(
		int dy10, int dy20,
		int dx10, int dx20,
		_Var z0, _Var z1, _Var z2, _Var& ddx, _Var& ddy) {

		const auto dc10 = z1 - z0;
		const auto dc20 = z2 - z0;

		//(iy1 - iy0)
		auto A = dc20 * dy10 - dc10 * dy20;
		auto B = dc10 * dx20 - dc20 * dx10;
		int C = dy20 * dx10 - dx20 * dy10;

		ddx = A / (float)-C;
		ddy = B / (float)-C;
	}


	template<typename ColorDataAccessor>
	void DrawTriangle(RegisterBlock r0_src, RegisterBlock r1_src, RegisterBlock r2_src) {
		float scrW_h = (float)(backBuffer->Desc().Width / 2);
		float scrH_h = (float)(backBuffer->Desc().Height / 2);

		RegisterBlock* p0 = &r0_src;
		RegisterBlock* p1 = &r1_src;
		RegisterBlock* p2 = &r2_src;

		//p1->reg[0].W = abs(p1->reg[0].W);
		float iw0 = 1.0f / p0->reg[0].w();
		float iw1 = 1.0f / p1->reg[0].w();
		float iw2 = 1.0f / p2->reg[0].w();

		//transform vertices to viewport space
		p0->reg[0].x() = (p0->reg[0].x() * iw0) * scrW_h + scrW_h;
		p0->reg[0].y() = (p0->reg[0].y() * iw0) * scrH_h + scrH_h;
		p0->reg[0].z() = (p0->reg[0].z() * iw0);

		p1->reg[0].x() = (p1->reg[0].x() * iw1) * scrW_h + scrW_h;
		p1->reg[0].y() = (p1->reg[0].y() * iw1) * scrH_h + scrH_h;
		p1->reg[0].z() = (p1->reg[0].z() * iw1);

		p2->reg[0].x() = (p2->reg[0].x() * iw2) * scrW_h + scrW_h;
		p2->reg[0].y() = (p2->reg[0].y() * iw2) * scrH_h + scrH_h;
		p2->reg[0].z() = (p2->reg[0].z() * iw2);

		float3 camZ(p0->reg[0].z(), p1->reg[0].z(), p2->reg[0].z());

		bool disableCulling = false;
		if (disableCulling)
			if (((p1->reg[0].x() - p0->reg[0].x()) * (p2->reg[0].y() - p0->reg[0].y()) - (p1->reg[0].y() - p0->reg[0].y()) * (p2->reg[0].x() - p0->reg[0].x()) <= 0)) {
				RegisterBlock* pTmp = p0;
				p0 = p2;
				p2 = pTmp;
			}


		const int x0 = (int)std::round(16.0f * p0->reg[0].x());
		const int x1 = (int)std::round(16.0f * p1->reg[0].x());
		const int x2 = (int)std::round(16.0f * p2->reg[0].x());
		const int y0 = (int)std::round(16.0f * p0->reg[0].y());
		const int y1 = (int)std::round(16.0f * p1->reg[0].y());
		const int y2 = (int)std::round(16.0f * p2->reg[0].y());

		const int Dx01 = x0 - x1;
		const int Dx12 = x1 - x2;
		const int Dx20 = x2 - x0;

		const int Dy01 = y0 - y1;
		const int Dy12 = y1 - y2;
		const int Dy20 = y2 - y0;

		const int FDx01 = Dx01 << 4;
		const int FDx12 = Dx12 << 4;
		const int FDx20 = Dx20 << 4;

		const int FDy01 = Dy01 << 4;
		const int FDy12 = Dy12 << 4;
		const int FDy20 = Dy20 << 4;

		//bounding rectangle
		int minX = ((std::min)((std::min)(x0, x1), x2) + 0xF) >> 4;
		int maxX = ((std::max)((std::max)(x0, x1), x2) + 0xF) >> 4;
		int minY = ((std::min)((std::min)(y0, y1), y2) + 0xF) >> 4;
		int maxY = ((std::max)((std::max)(y0, y1), y2) + 0xF) >> 4;

		const int ix0 = (x0 + 0xf) >> 4;
		const int ix1 = (x1 + 0xf) >> 4;
		const int ix2 = (x2 + 0xf) >> 4;

		const int iy0 = (y0 + 0xf) >> 4;
		const int iy1 = (y1 + 0xf) >> 4;
		const int iy2 = (y2 + 0xf) >> 4;

		const int diy20 = iy2 - iy0;
		const int dix20 = ix2 - ix0;
		const int diy10 = iy1 - iy0;
		const int dix10 = ix1 - ix0;

		const int area = dix10 * diy20 - dix20 * diy10;

		if (area == 0) return;


		if (maxY < 0)
			return;
		if (minY >= (int)backBuffer->Desc().Height)
			return;
		if (maxX < 0)
			return;
		if (minX >= (int)backBuffer->Desc().Width)
			return;

		if (minY < 0)
			minY = 0;
		if (maxY >= (int)backBuffer->Desc().Height)
			maxY = (int)backBuffer->Desc().Height - 1;

		if (minX < 0)
			minX = 0;
		if (maxX >= (int)backBuffer->Desc().Width)
			maxX = (int)backBuffer->Desc().Width - 1;


		int blockSize = 8;

		minX &= ~(blockSize - 1);
		minY &= ~(blockSize - 1);

		auto bbPtr = reinterpret_cast<typename ColorDataAccessor::PixelDataType*>(backBuffer->LockWrite());

		auto dbPtr = reinterpret_cast<float*>(depthBuffer->LockWrite());

		bbPtr += minY * backBuffer->Desc().Width;
		dbPtr += minY * depthBuffer->Desc().Width;

		int C1 = Dy01 * x0 - Dx01 * y0;
		int C2 = Dy12 * x1 - Dx12 * y1;
		int C3 = Dy20 * x2 - Dx20 * y2;


		//top-left fill convention fix
		if (!(Dy01 < 0 || (Dy01 == 0 && Dx01 > 0))) C1--;
		if (!(Dy12 < 0 || (Dy12 == 0 && Dx12 > 0))) C2--;
		if (!(Dy20 < 0 || (Dy20 == 0 && Dx20 > 0))) C3--;

		RegisterBlock triDerivX;
		RegisterBlock triDerivY;

		RegisterBlock invRegs[3];
		for (int i = 1; i < REG_COUNT; i++) {
			invRegs[0].reg[i] = p0->reg[i] * iw0;
			invRegs[1].reg[i] = p1->reg[i] * iw1;
			invRegs[2].reg[i] = p2->reg[i] * iw2;
		}

		for (int i = 1; i < REG_COUNT; i++) {
			CalcDerivatives<8, float4>(diy10, diy20, dix10, dix20, invRegs[0].reg[i], invRegs[1].reg[i], invRegs[2].reg[i], triDerivX.reg[i], triDerivY.reg[i]);
		}
		float2 derivZW_X;
		float2 derivZW_Y;

		float2 p0zw = p0->reg[0].zw();
		p0zw.y() = 1.0f / p0zw.y();

		float2 p1zw = p1->reg[0].zw();
		p1zw.y() = 1.0f / p1zw.y();

		float2 p2zw = p2->reg[0].zw();
		p2zw.y() = 1.0f / p2zw.y();

		CalcDerivatives<8, float2>(diy10, diy20, dix10, dix20, p0zw, p1zw, p2zw, derivZW_X, derivZW_Y);

		//scan all blocks
		for (int y = minY; y < maxY; y += blockSize) {
			for (int x = minX; x < maxX; x += blockSize) {
				//find block corners
				int x0 = x << 4;
				int x1 = (x + blockSize - 1) << 4;
				int y0 = y << 4;
				int y1 = (y + blockSize - 1) << 4;

				//evaluate half space functions for each corner
				bool a00 = C1 + Dx01 * y0 - Dy01 * x0 < 0;
				bool a01 = C1 + Dx01 * y0 - Dy01 * x1 < 0;
				bool a10 = C1 + Dx01 * y1 - Dy01 * x0 < 0;
				bool a11 = C1 + Dx01 * y1 - Dy01 * x1 < 0;
				int a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);

				bool b00 = C2 + Dx12 * y0 - Dy12 * x0 < 0;
				bool b01 = C2 + Dx12 * y0 - Dy12 * x1 < 0;
				bool b10 = C2 + Dx12 * y1 - Dy12 * x0 < 0;
				bool b11 = C2 + Dx12 * y1 - Dy12 * x1 < 0;
				int b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);

				bool c00 = C3 + Dx20 * y0 - Dy20 * x0 < 0;
				bool c01 = C3 + Dx20 * y0 - Dy20 * x1 < 0;
				bool c10 = C3 + Dx20 * y1 - Dy20 * x0 < 0;
				bool c11 = C3 + Dx20 * y1 - Dy20 * x1 < 0;
				int c = ((uint32_t)c00 << 0) | ((uint32_t)c10 << 1) | ((uint32_t)c01 << 2) | ((uint32_t)c11 << 3);

				//Skip entire block if all it's 4 corners outside of triangle edges
				if (a == 0 || b == 0 || c == 0)
					continue;

				auto colorBuffer = bbPtr;
				auto zBuffer = dbPtr;

				RegisterBlock var00;
				//interpolate registers
				for (int r = 1; r < REG_COUNT; r++) {
					var00.reg[r] = invRegs[0].reg[r] + triDerivX.reg[r] * (x - ix0) + triDerivY.reg[r] * (y - iy0);
				}

				//Whole block is inside triangle!
				if (a == 0xf && b == 0xf && c == 0xf) {
					for (int by = 0; by < blockSize; by++) {
						for (int bx = x; bx < (x + blockSize); bx++) {
							//compute interpolation weights

							auto ddx0 = (bx - ix0);
							auto ddy0 = (y + by - iy0);
							float2 zw = p0zw + derivZW_X * ddx0 + derivZW_Y * ddy0;

							//interpolate Z
							float z_interp = zw.x();

							if (zBuffer[bx] > z_interp)
							{
								zBuffer[bx] = z_interp;
								//interpolate registers

								for (int r = 1; r < REG_COUNT; r++) {
									r_ps.reg[r] = (var00.reg[r] + triDerivX.reg[r] * (bx - x)) / zw.y();
								}

								/*for (int r = 1; r < REG_COUNT; r++) {
									r_ps.reg[r] = invRegs[0].reg[r] + triDerivX.reg[r] * ddx0 + triDerivY.reg[r] * ddy0;
								}*/

								float4 pixel_color = ps->Execute(&r_ps.reg[0]);

								if (blendState.BlendEnable) {
									uint32_t dc = colorBuffer[bx];

									float4 dstColor = ColorDataAccessor::ConvertColor(dc);

									//get source alpha
									float srcAlpha = GetBlendAlpha(blendState.SrcAlpha, pixel_color, dstColor);
									float dstAlpha = GetBlendAlpha(blendState.DstAlpha, pixel_color, dstColor);

									pixel_color = pixel_color * srcAlpha + dstColor * dstAlpha;
								}
								//colorBuffer[bx] += 0x00606060;
								colorBuffer[bx] = ColorDataAccessor::ConvertColor(pixel_color);
							}
						}
						colorBuffer += backBuffer->Desc().Width;
						zBuffer += depthBuffer->Desc().Width;

						//increment Y variables
						for (int r = 1; r < REG_COUNT; r++) {
							var00.reg[r] = var00.reg[r] + triDerivY.reg[r];
						}
					}
				}
				else //Partially covered block
				{
					int CY1 = C1 + Dx01 * y0 - Dy01 * x0;
					int CY2 = C2 + Dx12 * y0 - Dy12 * x0;
					int CY3 = C3 + Dx20 * y0 - Dy20 * x0;

					for (int by = y; by < (y + blockSize); by++) {
						int CX1 = CY1;
						int CX2 = CY2;
						int CX3 = CY3;

						for (int bx = x; bx < (x + blockSize); bx++) {
							if (CX1 < 0 && CX2 < 0 && CX3 < 0) {
								auto ddx0 = bx - ix0;
								auto ddy0 = (by - iy0);
								float2 zw = p0zw + derivZW_X * ddx0 + derivZW_Y * ddy0;

								float z_interp = zw.x();

								if (zBuffer[bx] > z_interp) {
									zBuffer[bx] = z_interp;
									//interpolate registers
									for (int r = 1; r < REG_COUNT; r++) {
										r_ps.reg[r] = invRegs[0].reg[r] + triDerivX.reg[r] * ddx0 + triDerivY.reg[r] * ddy0;
										r_ps.reg[r] /= zw.y();//add perspective correction
									}

									float4 rst;
									rst = ps->Execute(&r_ps.reg[0]);

									//UINT texcolor = ConvertColor(rst);

									if (blendState.BlendEnable) {
										uint32_t dc = colorBuffer[bx];

										float4 dstColor = ColorDataAccessor::ConvertColor(dc);

										float srcAlpha;
										float dstAlpha;

										//get source alpha
										srcAlpha = GetBlendAlpha(blendState.SrcAlpha, rst, dstColor);
										dstAlpha = GetBlendAlpha(blendState.DstAlpha, rst, dstColor);

										rst = rst * srcAlpha + dstColor * dstAlpha;
									}
									//colorBuffer[bx] += 0x00606060;
									colorBuffer[bx] = ColorDataAccessor::ConvertColor(rst);
								}
							}

							CX1 -= FDy01;
							CX2 -= FDy12;
							CX3 -= FDy20;
						}
						CY1 += FDx01;
						CY2 += FDx12;
						CY3 += FDx20;

						colorBuffer += backBuffer->Desc().Width;
						zBuffer += depthBuffer->Desc().Width;
					}
				}
			}
			bbPtr += backBuffer->Desc().Width * blockSize;
			dbPtr += depthBuffer->Desc().Width * blockSize;
		}
	}

	std::array<float4*, REG_COUNT> r0_in;
	std::array<float4*, REG_COUNT> r1_in;
	std::array<float4*, REG_COUNT> r2_in;

	RegisterBlock r0_out;
	RegisterBlock r1_out;
	RegisterBlock r2_out;
	RegisterBlock rA_out;
	RegisterBlock rB_out;
	RegisterBlock r_ps;

	BlendState blendState;

	IInputLayout* geomLayout;
	std::array<int, REG_COUNT> regMapping;

	struct vertex_buffer_slot{
		Buffer* buffer;
		size_t stride;
	};

	std::array<vertex_buffer_slot, 8> vbSlots;
	std::array<Buffer*, 8> ibSlots;
	VertexShader* vs;
	PixelShader* ps;
	int primitiveType;

	Texture* backBuffer;
	Texture* depthBuffer;
};
#endif // BlockRasterizer_h__
