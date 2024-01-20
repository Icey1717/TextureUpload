#ifndef ACTOR_GRAVITY_AWARE_H
#define ACTOR_GRAVITY_AWARE_H

#include "Types.h"
#include "Actor.h"

class CActorGravityAware : public CActorAutonomous {
public:
	CActorGravityAware(){
		IMPLEMENTATION_GUARD_LOG()
	}

	virtual void Create(ByteCode* pByteCode);
};

#endif //ACTOR_GRAVITY_AWARE_H