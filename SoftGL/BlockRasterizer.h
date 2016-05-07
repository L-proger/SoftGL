#ifndef BlockRasterizer_h__
#define BlockRasterizer_h__

#include "Texture2D.h"
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
#include "lmath.h"

using namespace lm;

#define REG_COUNT 5

struct RegisterBlock
{
	std::array<float4, REG_COUNT> reg;

    inline static void Sub(const RegisterBlock& b1, const RegisterBlock& b2, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] - b2.reg[i];
		}
	}
    inline static void Add(const RegisterBlock& b1, const RegisterBlock& b2, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] + b2.reg[i];
		}
	}
    inline static void Mul(const RegisterBlock& b1, float val, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] * val;
		}
	}
    inline static void Div(const RegisterBlock& b1, float val, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] / val;
		}
	}
    inline static void Mul(RegisterBlock& b1, float val)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			b1.reg[i] *= val;
		}
	}
    inline static void Div(RegisterBlock& b1, float val)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			b1.reg[i] /= val;
		}
	}
    inline static void Clone(const RegisterBlock& from, RegisterBlock& to)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			to.reg[i] = from.reg[i];
		}
	}
};
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
    int GetPointNDCZone(const float4& point);
    void ClipToFrustumPlane(RasterizerPlane plane, ClipVector& src, ClipVector& dst);
    void ClipToFrustum(ClipFace face, ClipVector& dst);
    Texture2D* GetBackBuffer();
    Texture2D* GetDepthBuffer();

	void set_color_buffer(Texture2D* buffer);
	void set_depth_buffer(Texture2D* buffer);

    void SetPrimitiveType(int type);
    void Draw(int offset, int length);
	void DrawIndexed(size_t index_count, size_t start_index_location);
    void FixupMapping();
    void SetVertexBuffer(buffer* vb, size_t slot, size_t stride);
	void SetIndexBuffer(buffer* ib, size_t slot);

    void SetInputLayout(IInputLayout* layout);
    void SetPixelShader(PixelShader* shader);
    void SetVertexShader(VertexShader* shader);
    void SetBlendState(BlendState* state);
    void SetTexture(Texture2D* tex, uint8_t slot);

    float GetBlendAlpha(BLEND_SOURCE src, const float4& srcColor, const float4& dstColor);
private:
	void draw_impl(void* v0, void* v1, void* v2);
	std::array<Texture2D*, MAX_TEX_SLOTS> tex_slots;

    IRenderWindow* render_window;
	std::array<RasterizerPlane, 7> NDCPlanes;

    uint32_t ConvertColor(const float4& color);
    float4 ConvertColor(uint32_t color);
    void DrawTriangle(RegisterBlock r0_src, RegisterBlock r1_src, RegisterBlock r2_src);

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
		buffer* buffer;
		size_t stride;
	};

	std::array<vertex_buffer_slot, 8> vbSlots;
	std::array<buffer*, 8> ibSlots;
	VertexShader* vs;
	PixelShader* ps;
	int primitiveType;

	Texture2D* backBuffer;
	Texture2D* depthBuffer;
};
#endif // BlockRasterizer_h__
