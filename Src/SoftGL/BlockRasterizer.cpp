#include "BlockRasterizer.h"
#include "TextureSampler.h"
#include <assert.h>
#include <iostream>
#include "ImageDataAccessorR5G6B5.h"
#include "ImageDataAccessorR8G8B8.h"

BlockRasterizer::BlockRasterizer()
	:primitiveType(PT_TRIANGLE_LIST), vs(0), ps(0), geomLayout(0){

	blendState.AlphaOperation = BLEND_OP_ADD;
	blendState.SrcAlpha = BLEND_SRC_ALPHA;
	blendState.DstAlpha = BLEND_INV_SRC_ALPHA;
	blendState.BlendEnable = false;

	
	NDCPlanes[0] = RasterizerPlane(1, 0, 0, 1);//left
	NDCPlanes[1] = RasterizerPlane(-1, 0, 0, 1);//right
	NDCPlanes[2] = RasterizerPlane(0, 1, 0, 1);//top
	NDCPlanes[3] = RasterizerPlane(0, -1, 0, 1);//bottom
	NDCPlanes[4] = RasterizerPlane(0, 0, 1, 0);//near plane (D3D NDC, NeraClip Z == 0)
	NDCPlanes[5] = RasterizerPlane(0, 0, -1, 1);//far

	//NDCPlanes[6] = RasterizerPlane(0, 0, 0, 1);//WTF?
}

BlockRasterizer::~BlockRasterizer() {

}





bool BlockRasterizer::ClipToFrustumPlane(RasterizerPlane plane, ClipVector& src, ClipVector& dst, size_t regCount) {
	auto face_count = src.size();
	bool clipped = false;
	for (size_t i = 0; i < face_count; i++) {
		//read face from input
		ClipFace face = src[i];
		int v0out, v1out, v2out, vout_cnt;
		v0out = v1out = v2out = vout_cnt = 0;
		if (lm::dot(plane, face.v0.reg[0]) <= 0) { v0out++; vout_cnt++; }
		if (lm::dot(plane, face.v1.reg[0]) <= 0) { v1out++; vout_cnt++; }
		if (lm::dot(plane, face.v2.reg[0]) <= 0) { v2out++; vout_cnt++; }


		switch (vout_cnt) {
		case 3://all 3 vertices behind plane! Clip entire triangle
			clipped = true;
			continue;
		case 2:
		{
			//printf("2 vertices behind plane!\n");
			clipped = true;
			RegisterBlock* vert[3];
			RegisterBlock vA, vB;
			if (v0out && v1out) { vert[0] = &face.v0; vert[1] = &face.v1; vert[2] = &face.v2; }
			if (v1out && v2out) { vert[0] = &face.v1; vert[1] = &face.v2; vert[2] = &face.v0; }
			if (v2out && v0out) { vert[0] = &face.v2; vert[1] = &face.v0; vert[2] = &face.v1; }

			float alpha_a = lm::dot(plane, vert[2]->reg[0]) / (lm::dot(plane, vert[2]->reg[0]) - lm::dot(plane, vert[1]->reg[0]));
			float alpha_b = lm::dot(plane, vert[2]->reg[0]) / (lm::dot(plane, vert[2]->reg[0]) - lm::dot(plane, vert[0]->reg[0]));

			for (size_t k = 0; k < regCount; k++) {
				vA.reg[k] = vert[2]->reg[k] + (vert[1]->reg[k] - vert[2]->reg[k]) * alpha_a;
				vB.reg[k] = vert[2]->reg[k] + (vert[0]->reg[k] - vert[2]->reg[k]) * alpha_b;
			}

			ClipFace fa;
			fa.v0 = vA;
			fa.v1 = *vert[2];
			fa.v2 = vB;
			dst.push_back(fa);

		}
		break;
		case 1:
		{
			clipped = true;
			RegisterBlock* vert[3];
			RegisterBlock vA, vB;
			if (v0out) { vert[0] = &face.v0; vert[1] = &face.v1; vert[2] = &face.v2; }
			if (v1out) { vert[0] = &face.v1; vert[1] = &face.v2; vert[2] = &face.v0; }
			if (v2out) { vert[0] = &face.v2; vert[1] = &face.v0; vert[2] = &face.v1; }

			float alpha_a = lm::dot(plane, vert[2]->reg[0]) / (lm::dot(plane, vert[2]->reg[0]) - lm::dot(plane, vert[0]->reg[0]));
			float alpha_b = lm::dot(plane, vert[1]->reg[0]) / (lm::dot(plane, vert[1]->reg[0]) - lm::dot(plane, vert[0]->reg[0]));

			for (size_t k = 0; k < regCount; k++) {
				vA.reg[k] = vert[2]->reg[k] + (vert[0]->reg[k] - vert[2]->reg[k]) * alpha_a;
				vB.reg[k] = vert[1]->reg[k] + (vert[0]->reg[k] - vert[1]->reg[k]) * alpha_b;
			}

			ClipFace fa;
			fa.v0 = vB;
			fa.v1 = *vert[1];
			fa.v2 = vA;
			dst.push_back(fa);

			ClipFace fb;
			fb.v0 = vA;
			fb.v1 = *vert[1];
			fb.v2 = *vert[2];
			dst.push_back(fb);

			//DrawTriangle(rA_out, *vert[1], *vert[2]);
		}
		break;
		case 0:
			dst.push_back(face); //nothing to crop! Just push it
			break;
		}
	}
	return clipped;
}

void BlockRasterizer::ClipToFrustum(ClipFace face, ClipVector& dst, size_t regCount) {
	
	cv1.clear();
	cv2.clear();

	dst.clear();

	cv1.push_back(face);

	if(ClipToFrustumPlane(NDCPlanes[0], cv1, cv2, regCount)){
		std::cout << "Clipped with plane 0" << std::endl;
	}
	cv1.clear();

	if (ClipToFrustumPlane(NDCPlanes[1], cv2, cv1, regCount)) {
		std::cout << "Clipped with plane 1" << std::endl;
	}
	cv2.clear();

	if (ClipToFrustumPlane(NDCPlanes[2], cv1, cv2, regCount)) {
		std::cout << "Clipped with plane 2" << std::endl;
	}
	cv1.clear();

	if (ClipToFrustumPlane(NDCPlanes[3], cv2, cv1, regCount)) {
		std::cout << "Clipped with plane 3" << std::endl;
	}
	cv2.clear();

	if (ClipToFrustumPlane(NDCPlanes[4], cv1, cv2, regCount)) {
		std::cout << "Clipped with plane 4" << std::endl;
	}
	//cv1.clear();

	if (ClipToFrustumPlane(NDCPlanes[5], cv2, dst, regCount)) {
		std::cout << "Clipped with plane 5" << std::endl;
	}
	/*if (ClipToFrustumPlane(NDCPlanes[6], cv1, dst)) {
		std::cout << "Clipped with plane 6" << std::endl;
	}*/
}

Texture* BlockRasterizer::GetBackBuffer() {
	return backBuffer;
}

Texture* BlockRasterizer::GetDepthBuffer() {
	return depthBuffer;
}

void BlockRasterizer::setColorBuffer(Texture* buffer) {
	backBuffer = buffer;
}

void BlockRasterizer::setDepthBuffer(Texture* buffer) {
	depthBuffer = buffer;
}

void BlockRasterizer::SetPrimitiveType(int type) {
	primitiveType = type;
}

void BlockRasterizer::Draw(size_t offset, size_t length) {
	if (!length) return;
	switch (primitiveType) {
	case PT_TRIANGLE_LIST:
	{
		auto faceCount = length / 3;
		Buffer* geom = vbSlots[0].buffer;

		if (!faceCount){
			DebugInfo("No faces to draw");
			return;
		}
		if (!geom){
			DebugError("Vertex Buffer is not set");
			return;
		}

		auto stride = vbSlots[0].stride;

		//Buffer is too small for this draw call
		if (geom->Size() / stride < faceCount * 3) {
			DebugError("[Draw] Vertex Buffer is too small!");
			return;
		}
		uint8_t* data = (uint8_t*)geom->GetPointer();
		for (size_t i = 0; i < faceCount; i++) {
			//setup vertex data pointers
			uint8_t* v0 = data + ((offset + 0 + i * 3) * stride);
			uint8_t* v1 = data + ((offset + 1 + i * 3) * stride);
			uint8_t* v2 = data + ((offset + 2 + i * 3) * stride);
			draw_impl(v0, v1, v2);
		}
	}
	break;
	}
}

void BlockRasterizer::DrawIndexed(size_t index_count, size_t start_index_location)
{
	if (index_count == 0) return;
	switch (primitiveType) {
	case PT_TRIANGLE_LIST:
	{
		int faceCount = index_count / 3;
		Buffer* geom = vbSlots[0].buffer;
		Buffer* indices = ibSlots[0];

		if (!faceCount) {
			DebugInfo("No faces to draw");
			return;
		}
		if (!geom) {
			DebugError("Vertex Buffer is not set");
			return;
		}
		if (!indices) {
			DebugError("Index Buffer is not set");
			return;
		}
		int stride = vbSlots[0].stride;

		uint8_t* vertex_data = (uint8_t*)geom->GetPointer();
		auto vertices_count = geom->Size() / vbSlots[0].stride;
		indices_t* index_data = (indices_t*)indices->GetPointer();

		for (size_t i = 0; i < index_count; i+=3) {
			//setup vertex data pointers
			auto id0 = index_data[i + 0 + start_index_location];
			auto id1 = index_data[i + 1 + start_index_location];
			auto id2 = index_data[i + 2 + start_index_location];

			uint8_t* v0 = vertex_data + id0 * stride;
			uint8_t* v1 = vertex_data + id1 * stride;
			uint8_t* v2 = vertex_data + id2 * stride;
			draw_impl(v0, v1, v2);
		}
	}
	break;
	}
}

void BlockRasterizer::draw_impl(void* v0, void* v1, void* v2)
{
	int elements = geomLayout->Size();
	for (int i = 0; i < elements; i++) { //TODO: OMG WTF! Fix memory corruption!!!
		InputElement* e = geomLayout->GetElement(i);
		int rd = regMapping[i];
		if(rd >= 0){
			r0_in[rd] = reinterpret_cast<float4*>((uint8_t*)v0 + e->Offset);
			r1_in[rd] = reinterpret_cast<float4*>((uint8_t*)v1 + e->Offset);
			r2_in[rd] = reinterpret_cast<float4*>((uint8_t*)v2 + e->Offset);
		}
	}
	vs->Execute(&r0_in[0], &r0_out.reg[0]);
	vs->Execute(&r1_in[0], &r1_out.reg[0]);
	int numInterpolators = vs->Execute(&r2_in[0], &r2_out.reg[0]);


	//r0_out.reg[0].y() = -r0_out.reg[0].y();
	//r1_out.reg[0].y() = -r1_out.reg[0].y();
	//r2_out.reg[0].y() = -r2_out.reg[0].y();

	ClipVector cv;
	ClipFace cf;
	cf.v0 = r0_out;
	cf.v1 = r1_out;
	cf.v2 = r2_out;

	ClipToFrustum(cf, cv, numInterpolators);


	auto bbDesc = backBuffer->Desc();


	if (bbDesc.BytesPerPixel == 4) {
		for (size_t l = 0; l < cv.size(); l++) {
			ClipFace cf_render = cv[l];
			DrawTriangle<ImageDataAccessorR8G8B8>(cf_render.v0, cf_render.v1, cf_render.v2);
		}
	}
	else if (bbDesc.BytesPerPixel == 2) {
		for (size_t l = 0; l < cv.size(); l++) {
			ClipFace cf_render = cv[l];
			DrawTriangle<ImageDataAccessorR5G6B5>(cf_render.v0, cf_render.v1, cf_render.v2);
		}
	}
	else {
		//Not supported
	}

	
}

void BlockRasterizer::FixupMapping() {
	if (geomLayout != 0) {
		if (vs != 0) {
			auto elements = geomLayout->Size();
			for (size_t i = 0; i < elements; i++) {
				regMapping[i] = vs->FindRegister(geomLayout->GetElement(i)->name);
			}
		}
	}
}

void BlockRasterizer::SetVertexBuffer(Buffer* vb, size_t slot, size_t stride) {
	vbSlots[slot].buffer = vb;
	vbSlots[slot].stride = stride;
}

void BlockRasterizer::SetIndexBuffer(Buffer* ib, size_t slot){
	ibSlots[slot] = ib;
}

void BlockRasterizer::SetInputLayout(IInputLayout* layout) {
	geomLayout = layout;
	FixupMapping();
}

void BlockRasterizer::SetPixelShader(PixelShader* shader) {
	ps = shader;
}

void BlockRasterizer::SetVertexShader(VertexShader* shader) {
	vs = shader;
	FixupMapping();
}

void BlockRasterizer::SetBlendState(BlendState* state) {
	blendState = *state;
}

float BlockRasterizer::GetBlendAlpha(BLEND_SOURCE src, const float4& srcColor, const float4& dstColor) {
	switch (src) {
	case BLEND_ZERO:
		return 0.0f;
	case BLEND_ONE:
		return 1.0f;
	case BLEND_SRC_ALPHA:
		return srcColor.w();
	case BLEND_INV_SRC_ALPHA:
		return 1.0f - srcColor.w();
	}
	return 0;
}

uint32_t BlockRasterizer::ConvertColor(const float4& color) {
	uint32_t r = (uint32_t)(color.x() * 255.0f);
	uint32_t g = (uint32_t)(color.y() * 255.0f);
	uint32_t b = (uint32_t)(color.z() * 255.0f);
	uint32_t a = (uint32_t)(color.w() * 255.0f);

	return (a << 24) | (r << 16) | (g << 8) | (b);
}

float4 BlockRasterizer::ConvertColor(uint32_t color) {
	float d = 1.0f / 255.0f;

	float a = (float)((color >> 24) & 0xff) * d;
	float r = (float)((color >> 16) & 0xff) * d;
	float g = (float)((color >> 8) & 0xff) * d;
	float b = (float)((color >> 0) & 0xff) * d;

	return float4(r, g, b, a);
}

