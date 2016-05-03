#ifndef Scene_h__
#define Scene_h__

#include "Tracer.h"
#include "Camera.h"
#include <vector>
#include "Primitive.h"
using namespace std;

class Scene
{
private:
	Camera* cam;
	typedef std::vector<Primitive*> PrimitiveList;
	PrimitiveList primitives;
public:
	Scene()
	{

	}
	void AddPrimitive(Primitive* p)
	{
		primitives.push_back(p);
	}
	bool RayCast(const Ray& r, IntersectionInfo* info)
	{
		IntersectionInfo tmp1;
		IntersectionInfo tmp2;

		tmp2.Distance = -1.0f;

		for(PrimitiveList::iterator it = primitives.begin(); it != primitives.end(); it++)
		{
			Primitive* p = (*it);
			if(p->Intersect(r, &tmp1))
			{
				if(tmp2.Distance == -1.0f)
				{
					tmp2.Distance = tmp1.Distance;
					tmp2.Normal = tmp1.Normal;
					tmp2.Position = tmp1.Position;
					tmp2.Color = tmp1.Color;
					tmp2.material = tmp1.material;
				}
				else
				{
					if(tmp2.Distance > tmp1.Distance)
					{
						tmp2.Distance = tmp1.Distance;
						tmp2.Normal = tmp1.Normal;
						tmp2.Position = tmp1.Position;
						tmp2.Color = tmp1.Color;
						tmp2.material = tmp1.material;
					}
				}
			}

		}
		

		if(tmp2.Distance == -1.0f)
			return false;
		else
		{
			info->Distance = tmp2.Distance;
			info->Normal = tmp2.Normal;
			info->Position = tmp2.Position;
			info->Color = tmp2.Color;
			info->material = tmp2.material;
			return true;
		}
	}
	void SetCamera(Camera* _cam)
	{
		cam = _cam;
	}
	Camera* GetCamera()
	{
		return cam;
	}
};

#endif // Scene_h__