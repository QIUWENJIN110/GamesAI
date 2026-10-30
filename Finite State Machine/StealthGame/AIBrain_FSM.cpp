#include "AIBrain_FSM.h"

#include "AI_Framework/BrainComponents/AIBrainBlackboard.h"
#include "FSM_Reasoner.h"


bool AIBrain_FSM::Init(AIConstructorBase& _constructor,
                       AIActorBase& _actorContext) {
  brainBlackboard = std::make_unique<AIBrainBlackboard>();
  brainBlackboard->Init(_actorContext);

  brainReasoner = std::make_unique<FSM_Reasoner>();

  brainReasoner->SetOptions(_constructor);

  FSM_Reasoner* _reasoner = static_cast<FSM_Reasoner*>(GetReasoner());

  _reasoner->SetStartingState(FSM_Reasoner::GuardState::PATROLLING,
                              "OptionPatrol");

  _reasoner->Init("FMS Reasoner", *brainBlackboard);

  return true;
}

void AIBrain_FSM::Update() {
  // call the reasoner
  brainReasoner->Update();
}
