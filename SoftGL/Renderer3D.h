/*
#ifndef Renderer3D_h__
#define Renderer3D_h__

#include "LMath.h"
#include "Texture2D.h"




struct Vertex3D
{
	Vector4D Position;

	Vertex3D()
	{

	}

	Vertex3D(Vector4D pos)
	{
		Position = pos;

	}

	Vertex3D(Vector3D pos)
	{
		Position.X = pos.X;
		Position.Y = pos.Y;
		Position.Z = pos.Z;
		Position.W;

	}
};


struct Vertex3DColor
{
	Vector4D Position;
	Vector4D Color;

	Vertex3DColor()
	{

	}

	Vertex3DColor(Vector4D pos)
	{
		Position = pos;
		Color = Vector4D(1.0f, 1.0f, 1.0f, 1.0f);

	}
	Vertex3DColor(Vector4D pos, Vector4D col)
	{
		Position = pos;
		Color = col;
	}

	Vertex3DColor(Vector3D pos)
	{
		Position.X = pos.X;
		Position.Y = pos.Y;
		Position.Z = pos.Z;
		Position.W = 1.0f;
		Color = Vector4D(1.0f, 1.0f, 1.0f, 1.0f);

	}
};



class Renderer3D
{
private:
	Vector4D outColor;

	BlendState blendState;
	PRIMITIVE_TYPE primitiveType;

	

	

	int scrW;
	int scrH;
	float scrW_h;
	float scrH_h;
	HWND hwnd;
	BITMAPINFOHEADER bitmapInfo;
	BITMAPINFO info;
	HDC context;

	Texture2D* backBuffer;
	Texture2D* depthBuffer;
	Viewport* viewport;

	

	Vector4D interpStepLeft[REG_COUNT];
	Vector4D interpValLeft[REG_COUNT];

	Vector4D interpStepRight[REG_COUNT];
	Vector4D interpValRight[REG_COUNT];

	Vector4D interpValHor[REG_COUNT];
	Vector4D interpStepHor[REG_COUNT];



	Vector4D interpValLeftM[REG_COUNT];
	Vector4D interpValRightM[REG_COUNT];

	

	int usedInterpolators;

	

public:
	
	void SetPrimitiveType(PRIMITIVE_TYPE pt)
	{
		primitiveType = pt;
	}
	
	void SetBlendState(BlendState* state)
	{
		blendState = *state;
	}

	bool ZTestEnable;
	bool ZWriteEnbale;
	Matrix4x4 transform;
	Renderer3D(int width, int height, HWND output)
		:primitiveType(PT_TRIANGLE_LIST), , ZTestEnable(true), ZWriteEnbale(true)
	{
		blendState.AlphaOperation = BLEND_OP_ADD;
		blendState.SrcAlpha = BLEND_SRC_ALPHA;
		blendState.DstAlpha = BLEND_INV_SRC_ALPHA;
		blendState.BlendEnable = true;

		scrW = width;
		scrH = height;

		scrW_h = (float)(scrW / 2);
		scrH_h = (float)(scrH / 2);

		hwnd = output;

		memset(&vbSlots[0], 0, sizeof(VertexBuffer*) * 8);

		::ZeroMemory(&bitmapInfo,sizeof(BITMAPINFOHEADER));
		bitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.biWidth = width;
		bitmapInfo.biHeight = -height;
		bitmapInfo.biPlanes = 1;
		bitmapInfo.biBitCount = 32;
		bitmapInfo.biCompression = 0;
		bitmapInfo.biSizeImage = width * height * 4;

		info.bmiHeader = bitmapInfo;

		context = ::GetDC(hwnd);

		//default back buffer
		backBuffer = new Texture2D(width, height, 4);
		//default depth buffer
		depthBuffer = new Texture2D(width, height, 4);

		viewport = new Viewport(width, height, 0.0f, 1.0f);
	}
	~Renderer3D()
	{

	}
	

	UINT ConvertColor(const Vector4D& color)
	{
		UINT r = (UINT)(color.X * 255.0f);
		UINT g = (UINT)(color.Y * 255.0f);
		UINT b = (UINT)(color.Z * 255.0f);
		UINT a = (UINT)(color.W * 255.0f);

		return (a << 24) | (r << 16) | (g << 8) | (b);
	}

	Vector4D ConvertColor(UINT color)
	{
		float d = 1.0f / 255.0f;

		float a = (float)((color >> 24) & 0xff) * d;
		float r = (float)((color >> 16) & 0xff) * d;
		float g = (float)((color >> 8) & 0xff) * d;
		float b = (float)((color >> 0) & 0xff) * d;

		return Vector4D(r,g,b,a);
	}

	float GetBlendAlpha(BLEND_SOURCE src, const Vector4D& srcColor, const Vector4D& dstColor)
	{
		switch(src)
		{
		case BLEND_ZERO:
			return 0.0f;
		case BLEND_ONE:
			return 1.0f;
		case BLEND_SRC_ALPHA:
			return srcColor.W;
		case BLEND_INV_SRC_ALPHA:
			return 1.0f - srcColor.W;
		}
	}

	inline int ftol( float f )
	{

#ifdef __amigaos4__
		static hexdouble hd;
		__asm__ ( "fctiw %0, %1" : "=f" (hd.d) : "f" (f) );
		return hd.i.lo;
#else

		static int tmp;

#if _MSC_VER > 1000

		__asm
		{
			fld f
				fistp tmp
		}
#else
		asm volatile( "fistpl %0" : "=m" ( tmp ) : "t" ( f ) : "st" );
#endif
		return tmp;
#endif
	}

	__forceinline void DrawScanline(int y, const RegisterBlock& start, const RegisterBlock& end)
	{
		static RegisterBlock valStep;
		static RegisterBlock cVal;
		RegisterBlock::Sub(end, start, valStep);
		RegisterBlock::Div(valStep, end.reg[0].X - start.reg[0].X);
		RegisterBlock::Clone(start, cVal);


		UINT* ptr = static_cast<UINT*>(backBuffer->getBuffer()->getDataPtr());
		float* dbPtr = static_cast<float*>(depthBuffer->getBuffer()->getDataPtr());

		int sX = ftol(ceil(start.reg[0].X));
		int eX = ftol(ceil(end.reg[0].X));

		if(eX < 0) return;
		if(sX > scrW) return;
		if(sX < 0) sX = 0;
		if(eX >= (scrW -1)) eX = scrW - 1;

		for(int i = sX; i < eX; ++i, RegisterBlock::Add(cVal, valStep, cVal))
		{
			
			int offset = y * scrW + i;



			//depth test
			if(!ZTestEnable || dbPtr[offset] > cVal.reg[0].Z)
			{
				if(ZWriteEnbale)
				dbPtr[offset] = cVal.reg[0].Z;

			    Vector4D result = ps->Execute(&cVal.reg[0]);


				if(blendState.BlendEnable)
				{
					UINT dc = ptr[offset];

					Vector4D dstColor = ConvertColor(dc);

					float srcAlpha;
					float dstAlpha;

					//get source alpha
					srcAlpha = GetBlendAlpha(blendState.SrcAlpha, result, dstColor);
					dstAlpha = GetBlendAlpha(blendState.DstAlpha, result, dstColor);

					Vector4D finalColor = result * srcAlpha + dstColor * dstAlpha;

					ptr[offset] = ConvertColor(finalColor);

				}
				else
				{
					ptr[offset] = ConvertColor(result);
				}
				//ptr[offset] = ConvertColor(result);
			}

			
		}
	}

	void DrawTriangle()
	{
		

		//sort vertices
		RegisterBlock* swap;
		if(p0->reg[0].Y > p1->reg[0].Y)
		{
			swap = p0;
			p0 = p1;
			p1 = swap;
		}
		if(p1->reg[0].Y > p2->reg[0].Y)
		{
			swap = p1;
			p1 = p2;
			p2 = swap;
		}
		if(p0->reg[0].Y > p1->reg[0].Y)
		{
			swap = p0;
			p0 = p1;
			p1 = swap;
		}

		



		//compute gradient steps
		static RegisterBlock d10;
		static RegisterBlock d20;
		static RegisterBlock d21;

		RegisterBlock::Sub(*p1, *p0, d10);
		RegisterBlock::Sub(*p2, *p0, d20);
		RegisterBlock::Sub(*p2, *p1, d21);

		RegisterBlock::Div(d10, p1->reg[0].Y - p0->reg[0].Y);
		RegisterBlock::Div(d20, p2->reg[0].Y - p0->reg[0].Y);
		RegisterBlock::Div(d21, p2->reg[0].Y - p1->reg[0].Y);

		const int y1 = ftol(ceil(p1->reg[0].Y));
		const int y2 = ftol(ceil(p2->reg[0].Y));

		const float frac1 = (p0->reg[0].Y) - p0->reg[0].Y;
		const float frac2 = (p1->reg[0].Y) - p1->reg[0].Y;
		const float frac3 = (p0->reg[0].Y) - p2->reg[0].Y;

		static RegisterBlock startBlock;
		static RegisterBlock endBlock;

		//normal X
		if(d10.reg[0].X < d20.reg[0].X)
		{
			//compute left register block
			RegisterBlock::Mul(d10, frac1, startBlock);
			RegisterBlock::Add(startBlock, *p0, startBlock);

			//compute right register block
			RegisterBlock::Mul(d20, frac3, endBlock);
			RegisterBlock::Add(endBlock, *p2, endBlock);

			int startY = ftol(ceil(p0->reg[0].Y));

			//fill top triangle scan lines
			for(int cY = startY; cY < y1; ++cY, RegisterBlock::Add(startBlock, d10, startBlock),  RegisterBlock::Add(endBlock, d20, endBlock))
			{
				if(cY >=0 && cY < scrH)
				DrawScanline(cY, startBlock, endBlock);
			}


			//render bottom triangle
			RegisterBlock::Mul(d21, frac2, startBlock);
			RegisterBlock::Add(startBlock, *p1, startBlock);

			startY = ftol(ceil(p1->reg[0].Y));

			for(int cY = startY; cY < y2; ++cY, RegisterBlock::Add(startBlock, d21, startBlock),  RegisterBlock::Add(endBlock, d20, endBlock))
			{
				if(cY >=0 && cY < scrH)
				DrawScanline(cY, startBlock, endBlock);
			}
		}
		else //swapped X
		{
			//compute left register block
			RegisterBlock::Mul(d10, frac1, endBlock);
			RegisterBlock::Add(endBlock, *p0, endBlock);

			//compute right register block
			RegisterBlock::Mul(d20, frac3, startBlock);
			RegisterBlock::Add(startBlock, *p2, startBlock);

			int startY = ftol(ceil(p0->reg[0].Y));

			//fill top triangle scan lines
			for(int cY = startY; cY < y1; ++cY, RegisterBlock::Add(startBlock, d20, startBlock),  RegisterBlock::Add(endBlock, d10, endBlock))
			{
				if(cY >=0 && cY < scrH)
					DrawScanline(cY, startBlock, endBlock);
			}


			//render bottom triangle
			RegisterBlock::Mul(d21, frac2, endBlock);
			RegisterBlock::Add(endBlock, *p1, endBlock);

			startY = ftol(ceil(p1->reg[0].Y));

			for(int cY = startY; cY < y2; ++cY, RegisterBlock::Add(startBlock, d20, startBlock),  RegisterBlock::Add(endBlock, d21, endBlock))
			{
				if(cY >=0 && cY < scrH)
					DrawScanline(cY, startBlock, endBlock);
			}
		}
	}
	/ *void DrawTriangle2()
	{
		Vector4D* p0 = &r0_out[0];
		Vector4D* p1 = &r1_out[0];
		Vector4D* p2 = &r2_out[0];

		float iw0 = 1.0f / p0->W;
		float iw1 = 1.0f / p1->W;
		float iw2 = 1.0f / p2->W;

		//transform vertices to viewport space
		p0->X = (p0->X * iw0) * scrW_h + scrW_h;
		p0->Y = (p0->Y * iw0) * scrH_h + scrH_h;
		p0->Z = (p0->Z * iw0);

		p1->X = (p1->X * iw1) * scrW_h + scrW_h;
		p1->Y = (p1->Y * iw1) * scrH_h + scrH_h;
		p1->Z = (p1->Z * iw1);

		p2->X = (p2->X * iw2) * scrW_h + scrW_h;
		p2->Y = (p2->Y * iw2) * scrH_h + scrH_h;
		p2->Z = (p2->Z * iw2);

		//sort vertices
		Vector4D* swap;
		if(p0->Y > p1->Y)
		{
			swap = p0;
			p0 = p1;
			p1 = swap;
		}
		if(p1->Y > p2->Y)
		{
			swap = p1;
			p1 = p2;
			p2 = swap;
		}
		if(p0->Y > p1->Y)
		{
			swap = p0;
			p0 = p1;
			p1 = swap;
		}






UINT* ptr = static_cast<UINT*>(backBuffer->getBuffer()->getDataPtr());





		int x1 = (int)(p0->X);
		int x2 = (int)(p1->X);
		int x3 = (int)(p2->X);
		int y1 = (int)(p0->Y);
		int y2 = (int)(p1->Y);
		int y3 = (int)(p2->Y);

		float slopeLeft;
		float slopeRight;
		float offsetLeft;
		float offsetRight;


		float h1 = 1.0f / (float)(y1 - y2);
		float h2 = 1.0f / (float)(y2 - y3);
		float h3 = 1.0f / (float)(y1 - y3);


		slopeRight = (float)(x3 - x1) * -h3;



		//rasterize BOTTOM triangle
		if(y1 < y2)
		{
			slopeLeft = (float)(x1 - x2) * h1;

			for(int i = 0; i<usedInterpolators; i++)
			{

				interpStepLeft[i] = (p0[i] - p1[i]) * h1;
				interpStepRight[i] = (p0[i] - p2[i]) * h3;

				interpValLeft[i] = p0[i];
				interpValRight[i] = p0[i];

			}

			offsetLeft = p0->X;
			offsetRight = p0->X;

			int xL;
			int xR;
			int swap;


			float* dbPtr = static_cast<float*>(depthBuffer->getBuffer()->getDataPtr());

			for(int y = y1; y <= y2; y++)
			{
				xL = fround(offsetLeft);
				xR = fround(offsetRight);

				if(xR < xL)
				{

					for(int i = 0; i<REG_COUNT; i++)
					{
						interpValLeftM[i] = interpValRight[i];
						interpValRightM[i] = interpValLeft[i];
					}
					swap = xR;
					xR = xL;
					xL = swap;
				}
				else
				{
					for(int i = 0; i<REG_COUNT; i++)
					{
						interpValRightM[i] = interpValRight[i];
						interpValLeftM[i] = interpValLeft[i];
					}



				}



				Vector4D texcoordL = interpValLeftM[0];
				Vector4D texcoordR = interpValRightM[0];

float numStep = (float)(xR - xL + 1);

				for(int i = 0; i<usedInterpolators; i++)
				{
					interpValHor[i] = interpValLeftM[i];
					interpStepHor[i] = (interpValRightM[i] - interpValLeftM[i])  / numStep;
				}


				




				for(int x = xL; x<= xR; x++)
				{
					//now we have X and Y coordinates - just put pixel here in back buffer ^________^
				
					//	UINT col = sampleTex(texture, tc.X,tc.Y);



					int offset = y * scrW + x;

					float cd = dbPtr[offset];

					//z - test
					if(cd>interpValHor[0].Z)
					{
						outColor = ps->Execute(&interpValHor[0]);
				
						

						//alpha blending
						if(blendState.BlendEnable)
						{
							UINT dc = ptr[offset];

							Vector4D dstColor = ConvertColor(dc);

							float srcAlpha;
							float dstAlpha;

							//get source alpha
							srcAlpha = GetBlendAlpha(blendState.SrcAlpha, outColor, dstColor);
							dstAlpha = GetBlendAlpha(blendState.DstAlpha, outColor, dstColor);

							Vector4D finalColor = outColor * srcAlpha + dstColor * dstAlpha;

							ptr[offset] = ConvertColor(finalColor);

						}
						else
						{
							ptr[offset] = ConvertColor(outColor);
						}


						

						dbPtr[offset] = interpValHor[0].Z;
					}



					//if(alphaBlend)
					//{
					//	UINT src = ptr[offset];
					//	UINT col = sampleTex(texture, tc.X,tc.Y);
					//	ptr[offset] = blendrgb(col , src,(col & 0xff0000)>>16);

					//}
					//else
					//	{
					//	UINT col = sampleTex(texture, tc.X,tc.Y);

					//}



					//ptr[y*scrW + x] = col;
					for(int i = 0; i<usedInterpolators; i++)
					{
						interpValHor[i] += interpStepHor[i];
					}
				}

				for(int i = 0; i<usedInterpolators; i++)
				{
					interpValLeft[i] += interpStepLeft[i];
					interpValRight[i] += interpStepRight[i];
				}

				offsetLeft += slopeLeft;
				offsetRight += slopeRight;

			}

		}


		//rasterize bottom triangle
		if(y2 < y3)
		{
			slopeLeft = (float)(x2 - x3) * -h2;
			slopeRight = -slopeRight;

			for(int i = 0; i<usedInterpolators; i++)
			{


				interpStepLeft[i] = (p1[i] - p2[i]) * -h2;
				interpStepRight[i] = (p0[i] - p2[i]) * -h3;

				interpValLeft[i] = p2[i];
				interpValRight[i] = p2[i];

			}


			offsetLeft = p2->X;
			offsetRight = p2->X;

			int xL;
			int xR;
			int swap;
			float* dbPtr = static_cast<float*>(depthBuffer->getBuffer()->getDataPtr());
			for(int y = y3; y > y2; y--)
			{
				xL = fround(offsetLeft);
				xR = fround(offsetRight);

				if(xR < xL)
				{
					for(int i = 0; i<REG_COUNT; i++)
					{
						interpValLeftM[i] = interpValRight[i];
						interpValRightM[i] = interpValLeft[i];
					}

					swap = xR;
					xR = xL;
					xL = swap;
				}
				else
				{
					for(int i = 0; i<REG_COUNT; i++)
					{
						interpValRightM[i] = interpValRight[i];
						interpValLeftM[i] = interpValLeft[i];
					}



				}

				Vector4D texcoordL = interpValLeftM[0];
				Vector4D texcoordR = interpValRightM[0];

	

				float numStep = (float)(xR - xL + 1);

				for(int i = 0; i<usedInterpolators; i++)
				{
					interpValHor[i] = interpValLeftM[i];
					interpStepHor[i] = (interpValRightM[i] - interpValLeftM[i])  / numStep;
				}

				for(int x = xL; x<= xR; x++)
				{
					//now we have X and Y coordinates - just put pixel here in back buffer ^________^
			
					//	UINT col = sampleTex(texture, tc.X,tc.Y);


					int offset = y * scrW + x;

					//if(alphaBlend)
					//	{
					//		UINT src = ptr[offset];
					//		UINT col = sampleTex(texture, tc.X,tc.Y);
					//		ptr[offset] = blendrgb(col , src,(col & 0xff0000)>>16);

					//	}
					//	else
					//	{
					//	UINT col = sampleTex(texture, tc.X,tc.Y);
					float cd = dbPtr[offset];

					if(cd>interpValHor[0].Z)
					{

						outColor = ps->Execute(&interpValHor[0]);



						//alpha blending
						if(blendState.BlendEnable)
						{
							UINT dc = ptr[offset];

							Vector4D dstColor = ConvertColor(dc);

							float srcAlpha;
							float dstAlpha;

							//get source alpha
							srcAlpha = GetBlendAlpha(blendState.SrcAlpha, outColor, dstColor);
							dstAlpha = GetBlendAlpha(blendState.DstAlpha, outColor, dstColor);

							Vector4D finalColor = outColor * srcAlpha + dstColor * dstAlpha;

							ptr[offset] = ConvertColor(finalColor);

						}
						else
						{
							ptr[offset] = ConvertColor(outColor);
						}




						dbPtr[offset] = interpValHor[0].Z;
					}


					for(int i = 0; i<usedInterpolators; i++)
					{
						interpValHor[i] += interpStepHor[i];
					}
					//	}
				}


				offsetLeft += slopeLeft;
				offsetRight += slopeRight;

				for(int i = 0; i<REG_COUNT; i++)
				{
					interpValLeft[i] += interpStepLeft[i];
					interpValRight[i] += interpStepRight[i];
				}

			}


		}

	}* /


	struct int128
	{
		int r0;
		int r1;
		int r2;
		int r3;
	};
	struct int256
	{
		int r0;
		int r1;
		int r2;
		int r3;
		int r4;
		int r5;
		int r6;
		int r7;
	};
	void clear()
	{
		int clearColor = 0xff000000;
		int bbPixCnt = backBuffer->width * backBuffer->height;

		if(!(bbPixCnt % 16))
		{
			int256 clearVal = {clearColor,clearColor,clearColor,clearColor,clearColor,clearColor,clearColor,clearColor};
			int256* bbPtr = reinterpret_cast<int256*>(backBuffer->getBuffer()->getDataPtr());

			int numLoops = bbPixCnt >> 3;

			bbPtr[0] = clearVal;
			for(int i = 1; i < numLoops; i++)
				bbPtr[i] = bbPtr[i - 1];
		}
		else if(IsEvenNum(bbPixCnt))
		{
			__int64* bbPtr = static_cast<__int64*>(backBuffer->getBuffer()->getDataPtr());
			__int64 clearVal = static_cast<__int64>(clearColor) & 0xffffffff;
			clearVal = clearVal | (clearVal << 32);
			
			int numLoops = bbPixCnt / 2;

			bbPtr[0] = clearVal;
			for(int i = 1; i < numLoops; i++)
				bbPtr[i] = bbPtr[i - 1];
		}
		else
		{


			int* bbPtr = reinterpret_cast<int*>(backBuffer->getBuffer()->getDataPtr());

			for(int i = 0; i < bbPixCnt; i++)
				bbPtr[i] = 0;
		}

		int dbPixCnt = depthBuffer->width * depthBuffer->height;

			float* dbPtr = reinterpret_cast<float*>(depthBuffer->getBuffer()->getDataPtr());

			for(int i = 0; i < dbPixCnt; i++)
				dbPtr[i] = 1.0f;
		
		

		//memset(backBuffer->getBuffer()->getDataPtr(), 0x00, backBuffer->getBuffer()->getDataSize());
		//memset(depthBuffer->getBuffer()->getDataPtr(), 0xff, depthBuffer->getBuffer()->getDataSize());
	}

	void present()
	{
		SetDIBitsToDevice(context, 0,0, scrW, scrH, 0, 0, 0, scrH, backBuffer->getBuffer()->getDataPtr(), &info, 0);
	}
};
#endif // Renderer3D_h__*/