#ifndef BlockRasterizer_h__
#define BlockRasterizer_h__

#include "Texture2D.h"
#include "LMath.h"
#include "Viewport.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <assert.h>
#include <math.h>
#include "Tools.h"
#include <vector>
#include "IRenderWindow.h"
#include "Inline.h"
#include "Plane.h"
#include "static_vector.h"

#define REG_COUNT 5

struct RegisterBlock
{
	Vector4D reg[REG_COUNT];

    INLINE static void Sub(const RegisterBlock& b1, const RegisterBlock& b2, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] - b2.reg[i];
		}
	}
    INLINE static void Add(const RegisterBlock& b1, const RegisterBlock& b2, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] + b2.reg[i];
		}
	}
    INLINE static void Mul(const RegisterBlock& b1, float val, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] * val;
		}
	}
    INLINE static void Div(const RegisterBlock& b1, float val, RegisterBlock& result)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			result.reg[i] = b1.reg[i] / val;
		}
	}
    INLINE static void Mul(RegisterBlock& b1, float val)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			b1.reg[i] *= val;
		}
	}
    INLINE static void Div(RegisterBlock& b1, float val)
	{
		for(int i = 0; i< REG_COUNT; i++)
		{
			b1.reg[i] /= val;
		}
	}
    INLINE static void Clone(const RegisterBlock& from, RegisterBlock& to)
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
struct FFPFog
{
	bool enabled;
	float start;
	float end;
	Vector3D color;
};
struct FFPState
{
	FFPFog fog;
};

#define RENDER_FFP

#ifdef RENDER_FFP
#include "FFP.h"
#endif

struct ClipFace
{
	RegisterBlock v0;
	RegisterBlock v1;
	RegisterBlock v2;
};

#define MAX_TEX_SLOTS 16

class BlockRasterizer
{
public:
    //typedef std::vector<ClipFace> ClipVector;
	typedef Static_vector<ClipFace, 2> ClipVector;

    BlockRasterizer();
    ~BlockRasterizer();
    void SetWorldMatrix(const Matrix4x4& m);
    void SetViewMatrix(const Matrix4x4& m);
    void SetProjectionMatrix(const Matrix4x4& m);
    int GetPointNDCZone(const Vector4D& point);
    void ClipToFrustumPlane(Plane plane, ClipVector* src, ClipVector* dst);
    void ClipToFrustum(ClipFace face, ClipVector* dst);
    void SetFog(const FFPFog& fog);
    Texture2D* GetBackBuffer();
    Texture2D* GetDepthBuffer();
    void SetPrimitiveType(int type);
    void Draw(int offset, int length);
    void FixupMapping();
    void SetVertexBuffer(VertexBuffer* vb, int slot);
    void SetInputLayout(InputLayout* layout);
    void SetPixelShader(PixelShader* shader);
    void SetVertexShader(VertexShader* shader);
    void SetBlendState(BlendState* state);
    void SetTexture(Texture2D* tex, uint8 slot);

    void SetAlphaTestEnable(bool enable);
    void SetAlphaTestRef(float ref);

    void SetAmbientEnable(bool enable);
    void SetAmbientColor(const Vector3D& color);

    float GetBlendAlpha(BLEND_SOURCE src, const Vector4D& srcColor, const Vector4D& dstColor);
private:
    Texture2D* tex_slots[MAX_TEX_SLOTS];

    Matrix4x4 mWorld;
    Matrix4x4 mView;
    Matrix4x4 mProj;
    Matrix4x4 mWVP;

    FFPState ffp_state;
    IRenderWindow* render_window;
    Plane NDCPlanes[7];

    uint32 ConvertColor(const Vector4D& color);
    Vector4D ConvertColor(uint32 color);
    Vector4D FFP_GetPixelColor(Vector4D* input, float z);
    void DrawTriangle(RegisterBlock r0_src, RegisterBlock r1_src, RegisterBlock r2_src);


	Vector4D* r0_in[REG_COUNT];
	Vector4D* r1_in[REG_COUNT];
	Vector4D* r2_in[REG_COUNT];

	RegisterBlock r0_out;
	RegisterBlock r1_out;
	RegisterBlock r2_out;
	RegisterBlock rA_out;
	RegisterBlock rB_out;
	RegisterBlock r_ps;

	BlendState blendState;

	InputLayout* geomLayout;
	int regMapping[REG_COUNT];

	VertexBuffer* vbSlots[8];
	VertexShader* vs;
	PixelShader* ps;
	int primitiveType;

    bool alpha_test_enable;
    float alpha_ref;

    bool ambient_enable;
    Vector3D ambient_color;


	Texture2D* backBuffer;
	Texture2D* depthBuffer;
};
#endif // BlockRasterizer_h__
