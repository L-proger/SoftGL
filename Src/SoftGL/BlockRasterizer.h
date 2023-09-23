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
	void draw_impl(void* v0, void* v1, void* v2);

    IRenderWindow* render_window;
	std::array<RasterizerPlane, 6> NDCPlanes;

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
