#ifndef gameobject_h__
#define gameobject_h__

#include "transform.h"

class Game_object {
public:
	Transform transform;
	Game_object():transform(this)
	{
		
	}
};
#endif // gameobject_h__
