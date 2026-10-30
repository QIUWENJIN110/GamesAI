#include "AIActor_Guard.h"
#include "AIBrain_FSM.h"
#include <cmath>
#include <iostream>

#include "AIBrain_FSM.h"
#include "FSM_Reasoner.h"

AIActor_Guard::AIActor_Guard()
{
	posX = 0;
	posY = 0;
	rotation = 0;
	guardState = GuardState::Stationary;
	isActive = false;
	hasMoveTarget = false;
	dt = 0;
	movePoint = 0;
	moveTime = 0;

	_random = AIRandom_Global::getInstance();
	_math = AIMath_Global::getInstance();
}

AIActor_Guard::~AIActor_Guard()
{
	
}


// **  AIActorBase overrides **


bool AIActor_Guard::Init(std::string _id, AIConstructorBase& _constructor)
{
	actorID = _id;

	brain = std::make_unique<AIBrain_FSM>();

	brain->Init(_constructor, *this);


	return true;
}

/*
* Update is called each frame and should only contain code desined to be called this often
*/
void AIActor_Guard::Update(float _dt)
{

	dt = _dt; // for convenience, store the dt

	// call the brain to think and perform action
	brain->Update();


	// if the guard is moving, update movement status
	if (guardState == GuardState::Moving)
	{
		// if reached current destination
		if (movePoint >= 1)
		{
			// update brain and tracking
			EditBBValue("ReachedDesintation", 1);
			hasMoveTarget = false;
			guardState = GuardState::Stationary;

			// if raising alarm, set raise alarm data
			if (isRaisingAlarm)
			{
				EditBBValue("HasRaisedAlarm", 1);
				isRaisingAlarm = false;
			}

		}
	}

	UpdateEnergyLevel(_dt);

}



// ** Movement functions **


void AIActor_Guard::Spawn(float _x, float _y)
{
	posX = _x;
	posY = _y;

}

void AIActor_Guard::SetPatrolBounds(int _x, int _y)
{
	patrolMaxX = _x;
	patrolMaxY = _y;
}

/*
* GetRandomPatrolPoint will return a random patrol point from the map
*/
sf::Vector2f AIActor_Guard::GetRandomPatrolPoint()
{

	int randX = _random->GetRandomValue(patrolMaxX);
	int randY = _random->GetRandomValue(patrolMaxY);

	// as random position is base on the grid, convert to world point
	sf::Vector2f _movePos(_math->ConvertGridPosToSreenPos(randX), _math->ConvertGridPosToSreenPos(randY));

	return (_movePos);
}

/*
* MoveToPoint will trigger the movement of the Actor to the specficed point
*/
void AIActor_Guard::MoveToPoint(sf::Vector2f _targetPoint, bool _sprint)
{
	moveTarget = _targetPoint;
	moveOrigin.x = posX;
	moveOrigin.y = posY;


	float _moveDist = _math->CalcDistance(AIMath_Global::Vector2f(moveOrigin.x, moveOrigin.y), AIMath_Global::Vector2f(moveTarget.x, moveTarget.y));
		

	if (_sprint)
		moveTime = _moveDist / (moveSpeed * sprintMultiplier);
	else
		moveTime = _moveDist / moveSpeed;

	movePoint = 0;

	sf::Vector2f vec = moveTarget - moveOrigin;

	rotation = _math->CalcRotation(AIMath_Global::Vector2f(vec.x, vec.y) );

	EditBBValue("ReachedDesintation", 0);
}


/*
* Move will progress the Agent along the path to the designated target
*/
void AIActor_Guard::Move(float _dt)
{
	posX = std::lerp(moveOrigin.x, moveTarget.x, movePoint);
	posY = std::lerp(moveOrigin.y, moveTarget.y, movePoint);

	movePoint += _dt / moveTime;
}



void AIActor_Guard::UpdateEnergyLevel(float _dt)
{


	float _curEnergy = GetBBValue<float>("Energy");
	float _newEnergy = _curEnergy;
	if (guardState == GuardState::Stationary)
	{
		_newEnergy += restEnergyGain * _dt;
	}
	else if (guardState == GuardState::Moving)
	{
		_newEnergy -= walkEneryUse * _dt;
	}
	else if (guardState == GuardState::Running)
	{
		_newEnergy -= runEnergyUse * _dt;
	}

	if (_newEnergy < 0)
		_newEnergy = 0;
	if (_newEnergy > energyCap)
		_newEnergy = energyCap;

	EditBBValue("Energy", _newEnergy);
}


// **  Behaviour Functions - should be called each frame to progress the behaviour **


void AIActor_Guard::Rest()
{
	guardState = GuardState::Stationary;
}

void AIActor_Guard::Patrol()
{
	if (!hasMoveTarget)
	{
		// if Agent does not have target, get random destination
		sf::Vector2f _randPoint = GetRandomPatrolPoint();
		MoveToPoint(_randPoint, false);
		hasMoveTarget = true;
	}

	guardState = GuardState::Moving;
	Move(dt);

}

void AIActor_Guard::Investigate()
{

	if (!hasMoveTarget)
	{
		// if Agent does not have target, set destination as the point player last heard

		sf::Vector2f _noisePoint = sf::Vector2f(GetBBValue<AIMath_Global::Vector2f>("PlayerHeardAt").x, GetBBValue<AIMath_Global::Vector2f>("PlayerHeardAt").y);
		EditBBValue("CanHearPlayer", 0); // reset so that once finished investigating, don't try to investigate the same point

		MoveToPoint(_noisePoint, false);
		hasMoveTarget = true;
	}


	guardState = GuardState::Moving;
	Move(dt);
}


void AIActor_Guard::RaiseAlarm()
{

	if (!hasMoveTarget)
	{
		// if Agent does not have target, set destination as the alarm (centre of map)

		MoveToPoint(alarmPoint, false);
		hasMoveTarget = true;
		isRaisingAlarm = true;
	}


	guardState = GuardState::Moving;
	Move(dt);

}
void AIActor_Guard::Chase()
{
	//Agent should always move to player's location, but use a timer to ensure not recalculating every frame
	if (chaseCounter <= 0)
	{
		chaseCounter = chaseRefresh;

		sf::Vector2f _playerPoint = sf::Vector2f(GetBBValue<AIMath_Global::Vector2f>("PlayerSeenAt").x, GetBBValue<AIMath_Global::Vector2f>("PlayerSeenAt").y);

		MoveToPoint(_playerPoint, false);
		hasMoveTarget = true;
	}

	chaseCounter -= dt;
	guardState = GuardState::Moving;
	Move(dt);
}

void AIActor_Guard::Sprint()
{
	//Agent should always move to player's location, but use a timer to ensure not recalculating every frame
	if (chaseCounter <= 0)
	{
		chaseCounter = chaseRefresh;
		sf::Vector2f _playerPoint = sf::Vector2f(GetBBValue<AIMath_Global::Vector2f>("PlayerSeenAt").x, GetBBValue<AIMath_Global::Vector2f>("PlayerSeenAt").y);
		MoveToPoint(_playerPoint, true);
		hasMoveTarget = true;
	}

	chaseCounter -= dt;
	guardState = GuardState::Running;
	Move(dt);
}


// ** Status Functions **


bool AIActor_Guard::IsPlayerSeen()
{

	if (GetBBValue<int>("CanSeePlayer") == 1)
		return true;
	else
		return false;
}

bool AIActor_Guard::IsPlayerHeard()
{

	if (GetBBValue<int>("CanHearPlayer") == 1)
		return true;
	else
		return false;
}

void AIActor_Guard::UpdateCanSeePlayer(float _x, float _y)
{
	EditBBValue("CanSeePlayer", 1);
	
	AddBBValue("PlayerSeenAt", AIMath_Global::Vector2f(_x, _y));


}
void AIActor_Guard::UpdateCannotSeePlayer()
{
	EditBBValue("CanSeePlayer", 0);

	if (HasBBValue<AIMath_Global::Vector2f>("PlayerSeenAt"))
	{
		DeleteBBValue<AIMath_Global::Vector2f>("PlayerSeenAt");
	}


}

void AIActor_Guard::UpdateCanHearPlayer(float _x, float _y)
{
	EditBBValue("CanHearPlayer", 1);

	AddBBValue("PlayerHeardAt", AIMath_Global::Vector2f(_x, _y));

}

void AIActor_Guard::UpdateCannotHearPlayer()
{
	EditBBValue("CanHearPlayer", 0);

	if (HasBBValue<AIMath_Global::Vector2f>("PlayerHeardAt"))
	{

		DeleteBBValue<AIMath_Global::Vector2f>("PlayerHeardAt");

	}


}



//State Transition Functions


/*
* PreTransition is called before state change - use to clean up and movement or status tracking
*/
void AIActor_Guard::PreTransition()
{
	hasMoveTarget = false;
	chaseCounter = 0;
}



bool AIActor_Guard::CanSeePoint(sf::Vector2f _point)
{

	//LOS Triange, from guard to two points ahead
	AIMath_Global::Vector2f p1(0,0);
	AIMath_Global::Vector2f p2(-128, 256);
	AIMath_Global::Vector2f p3(128, 256);

	AIMath_Global::Vector2f p1_rot = _math->RotatePoint(p1, rotation);
	AIMath_Global::Vector2f p2_rot = _math->RotatePoint(p2, rotation);
	AIMath_Global::Vector2f p3_rot = _math->RotatePoint(p3, rotation);

	//offset to Actor point
	p1_rot.x += posX;
	p2_rot.x += posX;
	p3_rot.x += posX;

	p1_rot.y += posY;
	p2_rot.y += posY;
	p3_rot.y += posY;

	bool inLOS = _math->isPointInTriange(p1_rot, p2_rot, p3_rot, AIMath_Global::Vector2f(_point.x, _point.y));

	return inLOS;
}


bool AIActor_Guard::CanHearPoint(sf::Vector2f _point)
{
	// hearing is uniform radius from guard point
	float _playerDist = _math->CalcDistance(AIMath_Global::Vector2f(_point.x, _point.y), AIMath_Global::Vector2f(posX, posY));

	if (_playerDist <= hearRange)
	{
		return true;
	}

	return false;
}


std::string AIActor_Guard::GetCurrentState()
{
	FSM_Reasoner* reasoner = static_cast<FSM_Reasoner*>(brain->GetReasoner());

	return reasoner->GetStateName();
}