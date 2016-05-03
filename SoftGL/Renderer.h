#ifndef Renderer_h__
#define Renderer_h__
#include <windows.h>
#include "Buffer.h"
#include "Texture2D.h"

#include "LMath.h"
#define REG_COUNT 8

struct Vertex
{
	Vector2D Position;
	Vector4D r0;
	Vector4D r1;
	Vector4D r2;

	Vertex()
	{

	}

	Vertex(Vector2D pos)
	{
		Position = pos;
		
	}

	Vertex(Vector2D pos, Vector4D _r0)
	{
		Position = pos;
		r0 = _r0;

	}
};



class Renderer
{


private:


	/*Vector4D r0[MAX_REG_COUNT];
	Vector4D r1[MAX_REG_COUNT];
	Vector4D r2[MAX_REG_COUNT];*/

	Vector4D interpStepLeft[REG_COUNT];
	Vector4D interpValLeft[REG_COUNT];

	Vector4D interpStepRight[REG_COUNT];
	Vector4D interpValRight[REG_COUNT];

	Vector4D interpValLeftM[REG_COUNT];
	Vector4D interpValRightM[REG_COUNT];

	Vector4D reg0[REG_COUNT];
	Vector4D reg1[REG_COUNT];
	Vector4D reg2[REG_COUNT];

	int regMapping[REG_COUNT];

	BITMAPINFO info;
	BITMAPINFOHEADER bitmapInfo;
	Buffer* backBuffer;
	HDC context;
	HWND hwnd;
	int scrW;
	int scrH;


public:
	bool alphaBlend;
	Texture2D* texture;
	Matrix3x3 transformationMatrix;
	
	Renderer(int width, int height, int handle)
	{

		alphaBlend = false;
		scrW = width;
		scrH = height;

		hwnd = (HWND)(handle);

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

		//create back buffer
		backBuffer = new Buffer(width * height * 4);
	}
	~Renderer()
	{
		if(backBuffer)
			delete backBuffer;
	}
	void clear()
	{
		memset(backBuffer->getDataPtr(), 0x11, backBuffer->Size());
	}

	void fillRectangle(Vertex tl, Vertex tr, Vertex bl, Vertex br, UINT color)
	{
		

		Vector2D offset = Vector2D((float)scrW / 2.0f, (float)scrH / 2.0f);

		UINT* ptr = static_cast<UINT*>(backBuffer->getDataPtr());

		

		Vec2TransformCoordinate(tl.Position, transformationMatrix);
		Vec2TransformCoordinate(tr.Position, transformationMatrix);

		Vec2TransformCoordinate(bl.Position, transformationMatrix);
		Vec2TransformCoordinate(br.Position, transformationMatrix);

	/*	tl.Position+=offset;
		tr.Position+=offset;
		bl.Position+=offset;
		br.Position+=offset;*/

		drawTriangle(tl, tr, bl, color);

		drawTriangle(bl, tr, br, color);



		
	}


	

	void drawTriangleBary(Vertex& _p1, Vertex& _p2, Vertex& _p3, UINT color)
	{
	
		UINT* ptr = static_cast<UINT*>(backBuffer->getDataPtr());
		const int x1 = fround(_p1.Position.X);
		const int y1 = fround(_p1.Position.Y);

		const int x2 = fround(_p2.Position.X);
		const int y2 = fround(_p2.Position.Y);

		const int x3 = fround(_p3.Position.X);
		const int y3 = fround(_p3.Position.Y);

		int maxX = x1;
		if(maxX < x2) maxX = x2;
		if(maxX < x3) maxX = x3;

		int maxY = y1;
		if(maxY < y2) maxY = y2;
		if(maxY < y3) maxY = y3;

		int minX = x1;
		if(minX > x2) minX = x2;
		if(minX > x3) minX = x3;

		int minY = y1;
		if(minY > y2) minY = y2;
		if(minY > y3) minY = y3;

		float p = 1.0f / ((float) (x1 * y2) - (x1 * y3) - (x2 * y1) + (x2 * y3) + (x3 * y1) - (x3 * y2));

		for(int x = minX; x <= maxX; x++)
		{
			for(int y = minY; y <= maxY; y++)
			{
				const float baryX = static_cast<float>(((x * y2) - (x * y3) - (x2 * y) + (x2 * y3) + (x3 * y) - (x3 * y2))) * p;
				const float baryY = static_cast<float>(((x1 * y) - (x1 * y3) - (x * y1) + (x * y3) + (x3 * y1) - (x3 * y))) * p;
				const float baryZ = static_cast<float>(((x1 * y2) - (x1 * y) - (x2 * y1) + (x2 * y) + (x * y1) - (x * y2))) * p;

				if (baryX >= 0.0f && baryY >= 0.0f && baryZ >= 0.0f)
				{

					const float tX = _p1.r0.X * baryX + _p2.r0.X * baryY + _p3.r0.X * baryZ;
					const float tY = _p1.r0.Y * baryX + _p2.r0.Y * baryY + _p3.r0.Y * baryZ;

					UINT colorTc = sampleTex(texture, tX, tY);
					//UINT colorTc = 0xaa00ffaa;

					int offset = y * scrW + x;

					if(alphaBlend)
					{
						UINT src = ptr[offset];
						ptr[offset] = blendrgb( colorTc, src,colorTc &0xff);

					}
					else
					{
						ptr[offset] = colorTc;
					}
					
				}
			}
		}

		
	}


	void Draw(int offset, int count)
	{
		
	}
	
	void drawTriangle2(Vertex& _p1, Vertex& _p2, Vertex& _p3, UINT color)
	{
	
		UINT* ptr = static_cast<UINT*>(backBuffer->getDataPtr());

		Vertex p1;
		Vertex p2;
		Vertex p3;

		p1 = _p1;
		p2 = _p2;
		p3 = _p3;

		Vertex swap;

		//sort by Y
		if(p1.Position.Y > p2.Position.Y)
		{
			swap = p1;
			p1 = p2;
			p2 = swap;
		}
		if(p2.Position.Y > p3.Position.Y)
		{
			swap = p2;
			p2 = p3;
			p3 = swap;
		}
		if(p1.Position.Y > p2.Position.Y)
		{
			swap = p1;
			p1 = p2;
			p2 = swap;
		}

		/*if(p2.Position.X > p3.Position.X)
		{
			swap = p2;
			p2 = p3;
			p3 = swap;
		}*/



		int x1 = (int)(p1.Position.X);
		int x2 = (int)(p2.Position.X);
		int x3 = (int)(p3.Position.X);
		int y1 = (int)(p1.Position.Y);
		int y2 = (int)(p2.Position.Y);
		int y3 = (int)(p3.Position.Y);

		float slopeLeft;
		float slopeRight;
		float offsetLeft;
		float offsetRight;


		float h1 = 1.0f / (float)(y1 - y2);
		float h2 = 1.0f / (float)(y2 - y3);
		float h3 = 1.0f / (float)(y1 - y3);


		slopeRight = (float)(x3 - x1) * -h3;

		

		//rasterize top triangle
		if(y1 < y2)
		{
			slopeLeft = (float)(x1 - x2) * h1;

			for(int i = 0; i<REG_COUNT; i++)
			{
				Vector4D rv1;
				Vector4D rv2;
				Vector4D rv3;

				rv1 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p1) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv2 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p2) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv3 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p3) + sizeof(Vector2D) + sizeof(Vector4D)*i);

				interpStepLeft[i] = (rv1 - rv2) * h1;
				interpStepRight[i] = (rv3 - rv1) * -h3;

				interpValLeft[i] = rv1;
				interpValRight[i] = rv1;

			}

			offsetLeft = p1.Position.X;
			offsetRight = p1.Position.X;

			int xL;
			int xR;
			int swap;

			for(int y = y1; y <= y2; y++)
			{
				if(y>=0 && y<scrH)
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

				Vector4D diff = (texcoordR - texcoordL) /(float)(xR - xL + 1);

				for(int x = xL; x<= xR; x++)
				{
					if(x>=0 && x<scrW)
					{

					
					//now we have X and Y coordinates - just put pixel here in back buffer ^________^
					Vector4D tc = texcoordL + diff * (float)(x - xL);
					UINT col = sampleTex(texture, tc.X,tc.Y);


					int offset = y * scrW + x;

					if(alphaBlend)
					{
						UINT src = ptr[offset];
						UINT col = sampleTex(texture, tc.X,tc.Y);
						ptr[offset] = blendrgb(col , src,(col & 0xff)>>0);

					}
					else
					{
						UINT col = sampleTex(texture, tc.X,tc.Y);
						ptr[offset] =col ;
					}
					}


				
					//ptr[y*scrW + x] = col;
				}

				for(int i = 0; i<REG_COUNT; i++)
				{
					interpValLeft[i] += interpStepLeft[i];
					interpValRight[i] += interpStepRight[i];
				}

				offsetLeft += slopeLeft;
				offsetRight += slopeRight;

			}
			}

		}


		//rasterize bottom triangle
		if(y2 < y3)
		{
			slopeLeft = (float)(x2 - x3) * -h2;
			slopeRight = -slopeRight;

			for(int i = 0; i<REG_COUNT; i++)
			{
				Vector4D rv1;
				Vector4D rv2;
				Vector4D rv3;

				rv1 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p1) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv2 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p2) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv3 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p3) + sizeof(Vector2D) + sizeof(Vector4D)*i);

				

				interpStepLeft[i] = (rv2 - rv3) * -h2;
				interpStepRight[i] = (rv1 - rv3) * -h3;

				interpValLeft[i] = rv3;
				interpValRight[i] = rv3;

			}


			offsetLeft = p3.Position.X;
			offsetRight = p3.Position.X;

			int xL;
			int xR;
			int swap;

			for(int y = y3; y > y2; y--)
			{
				if(y>=0 && y<scrH)
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

				Vector4D diff = (texcoordR - texcoordL) / (float)(xR - xL + 1);

				for(int x = xL; x<= xR; x++)
				{
					if(x>=0 && x<scrW)
					{
						//now we have X and Y coordinates - just put pixel here in back buffer ^________^
						Vector4D tc = texcoordL + diff * (float)(x - xL);
						UINT col = sampleTex(texture, tc.X,tc.Y);


						int offset = y * scrW + x;

						if(alphaBlend)
						{
							UINT src = ptr[offset];
							UINT col = sampleTex(texture, tc.X,tc.Y);
							ptr[offset] = blendrgb(col , src,(col & 0xff)>>0);

						}
						else
						{
							UINT col = sampleTex(texture, tc.X,tc.Y);
							ptr[offset] = col ;
						}
					}
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

			
		}
		
		
	}

	void drawTriangle(Vertex& _p1, Vertex& _p2, Vertex& _p3, UINT color)
	{
	
		UINT* ptr = static_cast<UINT*>(backBuffer->getDataPtr());

		Vertex p1;
		Vertex p2;
		Vertex p3;

		p1 = _p1;
		p2 = _p2;
		p3 = _p3;

		Vertex swap;

		//sort by Y
		if(p1.Position.Y > p2.Position.Y)
		{
			swap = p1;
			p1 = p2;
			p2 = swap;
		}
		if(p2.Position.Y > p3.Position.Y)
		{
			swap = p2;
			p2 = p3;
			p3 = swap;
		}
		if(p1.Position.Y > p2.Position.Y)
		{
			swap = p1;
			p1 = p2;
			p2 = swap;
		}

		/*if(p2.Position.X > p3.Position.X)
		{
			swap = p2;
			p2 = p3;
			p3 = swap;
		}*/



		int x1 = (int)(p1.Position.X);
		int x2 = (int)(p2.Position.X);
		int x3 = (int)(p3.Position.X);
		int y1 = (int)(p1.Position.Y);
		int y2 = (int)(p2.Position.Y);
		int y3 = (int)(p3.Position.Y);

		float slopeLeft;
		float slopeRight;
		float offsetLeft;
		float offsetRight;


		float h1 = 1.0f / (float)(y1 - y2);
		float h2 = 1.0f / (float)(y2 - y3);
		float h3 = 1.0f / (float)(y1 - y3);


		slopeRight = (float)(x3 - x1) * -h3;

		

		//rasterize top triangle
		if(y1 < y2)
		{
			slopeLeft = (float)(x1 - x2) * h1;

			for(int i = 0; i<REG_COUNT; i++)
			{
				Vector4D rv1;
				Vector4D rv2;
				Vector4D rv3;

				rv1 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p1) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv2 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p2) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv3 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p3) + sizeof(Vector2D) + sizeof(Vector4D)*i);

				interpStepLeft[i] = (rv1 - rv2) * h1;
				interpStepRight[i] = (rv3 - rv1) * -h3;

				interpValLeft[i] = rv1;
				interpValRight[i] = rv1;

			}

			offsetLeft = p1.Position.X;
			offsetRight = p1.Position.X;

			int xL;
			int xR;
			int swap;

			for(int y = y1; y <= y2; y++)
			{
				if(y>=0 && y<scrH)
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

				Vector4D diff = (texcoordR - texcoordL) /(float)(xR - xL + 1);

				for(int x = xL; x<= xR; x++)
				{
					if(x>=0 && x<scrW)
					{

					
					//now we have X and Y coordinates - just put pixel here in back buffer ^________^
					Vector4D tc = texcoordL + diff * (float)(x - xL);
					UINT col = sampleTex(texture, tc.X,tc.Y);


					int offset = y * scrW + x;

					if(alphaBlend)
					{
						UINT src = ptr[offset];
						UINT col = sampleTex(texture, tc.X,tc.Y);
						ptr[offset] = blendrgb(col , src,(col & 0xff)>>0);

					}
					else
					{
						UINT col = sampleTex(texture, tc.X,tc.Y);
						ptr[offset] =col ;
					}
					}


				
					//ptr[y*scrW + x] = col;
				}

				for(int i = 0; i<REG_COUNT; i++)
				{
					interpValLeft[i] += interpStepLeft[i];
					interpValRight[i] += interpStepRight[i];
				}

				offsetLeft += slopeLeft;
				offsetRight += slopeRight;

			}
			}

		}


		//rasterize bottom triangle
		if(y2 < y3)
		{
			slopeLeft = (float)(x2 - x3) * -h2;
			slopeRight = -slopeRight;

			for(int i = 0; i<REG_COUNT; i++)
			{
				Vector4D rv1;
				Vector4D rv2;
				Vector4D rv3;

				rv1 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p1) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv2 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p2) + sizeof(Vector2D) + sizeof(Vector4D)*i);
				rv3 = *reinterpret_cast<Vector4D*>(reinterpret_cast<char*>(&p3) + sizeof(Vector2D) + sizeof(Vector4D)*i);

				

				interpStepLeft[i] = (rv2 - rv3) * -h2;
				interpStepRight[i] = (rv1 - rv3) * -h3;

				interpValLeft[i] = rv3;
				interpValRight[i] = rv3;

			}


			offsetLeft = p3.Position.X;
			offsetRight = p3.Position.X;

			int xL;
			int xR;
			int swap;

			for(int y = y3; y > y2; y--)
			{
				if(y>=0 && y<scrH)
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

				Vector4D diff = (texcoordR - texcoordL) / (float)(xR - xL + 1);

				for(int x = xL; x<= xR; x++)
				{
					if(x>=0 && x<scrW)
					{
						//now we have X and Y coordinates - just put pixel here in back buffer ^________^
						Vector4D tc = texcoordL + diff * (float)(x - xL);
						UINT col = sampleTex(texture, tc.X,tc.Y);


						int offset = y * scrW + x;

						if(alphaBlend)
						{
							UINT src = ptr[offset];
							UINT col = sampleTex(texture, tc.X,tc.Y);
							ptr[offset] = blendrgb(col , src,(col & 0xff)>>0);

						}
						else
						{
							UINT col = sampleTex(texture, tc.X,tc.Y);
							ptr[offset] = col ;
						}
					}
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

			
		}
		
		
	}


	void sampleTex(Texture2D* tex, Vector4D* color, Vector4D* texcoord)
	{
		int x = (int)(texcoord->X * tex->fwidth);
		int y = (int)(texcoord->Y * tex->fheight);
		UCHAR* ptr = reinterpret_cast<UCHAR*>(tex->getBuffer()->getDataPtr());
		ptr+=(y*tex->width + x)* tex->bpp;
		color->Z = ((float)ptr[0]) / 255.0f;
		color->Y = ((float)ptr[1]) / 255.0f;
		color->X = ((float)ptr[2]) / 255.0f;
		color->W = 1.0f;
	}


	inline UINT sampleTex(Texture2D* tex, float tX, float tY)
	{
		tX = clampFloat(tX, 0.0f, 1.0f);
		tY = clampFloat(tY, 0.0f, 1.0f);

		tY = 1.0f - tY;

		int x = static_cast<int>((tX * tex->fwidth));
		int y = static_cast<int>((tY * tex->fheight));
		UCHAR* ptr = reinterpret_cast<UCHAR*>(tex->getBuffer()->getDataPtr());
		

		float pX = tX * tex->fwidth;
		float pY = tY * tex->fheight;

		UINT frX = (UINT)((pX - (float)((int)pX))* 255.0f);
		UINT frY = (UINT)((pY - (float)((int)pY))* 255.0f);

		UINT* ptr2 = reinterpret_cast<UINT*>(tex->getBuffer()->getDataPtr());

		UINT color;
		if(tex->bpp < 4)
		{
			UINT tl = 0xff000000;
			UINT tr = 0xff000000;
			UINT rl = 0xff000000;
			UINT rr = 0xff000000;

			memcpy(&tl, ptr+ (y*tex->width + x)* tex->bpp, 3);
			memcpy(&tr, ptr+ (y*tex->width + x + 1)* tex->bpp, 3);
			memcpy(&rl, ptr+ ((y + 1)*tex->width + x)* tex->bpp, 3);
			memcpy(&rr, ptr+ ((y + 1)*tex->width + x + 1)* tex->bpp, 3);

			color = blendrgb ( blendrgb ( tl, tr, frX), blendrgb ( rl, rr, frX), frY);
			 
		}
		else
		{
			
			int x1 = x;
			int x2 = x + 1;
			int x3 = x;
			int x4 = x + 1;

			int y1 = y;
			int y2 = y;
			int y3 = (y + 1);
			int y4 = (y + 1);

			x1 = clampInt(x1, 0, tex->width - 1);
			x2 = clampInt(x2, 0, tex->width - 1);
			x3 = clampInt(x3, 0, tex->width - 1);
			x4 = clampInt(x4, 0, tex->width - 1);


			y1 = clampInt(y1, 0, tex->height - 1);
			y2 = clampInt(y2, 0, tex->height - 1);
			y3 = clampInt(y3, 0, tex->height - 1);
			y4 = clampInt(y4, 0, tex->height - 1);




			UINT tl = ptr2[y1 * tex->width + x1];
			UINT tr = ptr2[y2 * tex->width + x2];
			UINT rl = ptr2[y3 * tex->width + x3];
			UINT rr = ptr2[y4 * tex->width + x4];

			color = blendrgb ( blendrgb ( tl, tr, frX), blendrgb ( rl, rr, frX), frY);
		}

		return color;
	}

	void present()
	{
		SetDIBitsToDevice(context, 0,0, scrW, scrH, 0, 0, 0, scrH, backBuffer->getDataPtr(), &info, 0);
	}
};
#endif // Renderer_h__