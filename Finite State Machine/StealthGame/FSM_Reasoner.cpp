#include "FSM_Reasoner.h"

#include "AIActor_Guard.h"

FSM_Reasoner::~FSM_Reasoner() {}

bool FSM_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context) {
  reasonerID = _id;
  actorBlackboard = &_context;
  return true;
}

/*
Think is called on demand to choose from the possible Options that AI has.

The function should implement the logic of the AI method to choose one Option as
the selectedOption.
*/
void FSM_Reasoner::Think() {
  // note current state
  GuardState _lastState = currentState;

  bool canSeePlayer = actorBlackboard->GetValue<int>("CanSeePlayer") == 1;
  bool canHearNoise = actorBlackboard->GetValue<int>("CanHearPlayer") == 1;
  bool hasRaisedAlarm = actorBlackboard->GetValue<int>("HasRaisedAlarm") == 1;
  float energy = actorBlackboard->GetValue<float>("Energy");

  const float SPRINT_HIGH = 10.0f;
  const float SPRINT_LOW = 5.0f;

  /*
        if (currentState == GuardState::PATROLLING) {
        if (actorBlackboard->GetValue<float>("Energy") < 1) {
        currentState = GuardState::RESTING;
        selectedOption = GetOptionByName("OptionRest");
        }

        // TASK TODO - add in additional transitions from the Patrolling State
        } else if (currentState == GuardState::RESTING) {
        // TASK TODO - add in transitions from the Resting State
        // when accessing the blackboard, make sure to specifcy the <type> being
        // accessed e.g. GetValue<float>("Energy") or
     GetValue<int>("CanSeePlayer")
        // pseudocode 伪代码
        // if energy >= 20:
        //		current state = GuardState::PATROLLING
        //		selected option = OptionPatrol
        if (actorBlackboard->GetValue<float>("Energy") >= 20) {
        currentState = GuardState::PATROLLING;
        selectedOption = GetOptionByName("OptionPatrol");
        }
        }
  */

  // TASK TODO - add conditions additional GuardStates
  switch (currentState) {
    case GuardState::PATROLLING:
      selectedOption = GetOptionByName("OptionPatrol");
      if (canSeePlayer) {
        currentState = GuardState::CHASING;
        selectedOption = GetOptionByName("OptionChase");
      } else if (canHearNoise) {
        currentState = GuardState::INVESTIGATING;
        selectedOption = GetOptionByName("OptionInvestigate");
      } else if (energy < 1) {
        currentState = GuardState::RESTING;
        selectedOption = GetOptionByName("OptionRest");
      }
      break;

    case GuardState::CHASING:
      selectedOption = GetOptionByName("OptionChase");
      if (canSeePlayer && energy >= SPRINT_HIGH) {
        currentState = GuardState::SPRINTING;
        selectedOption = GetOptionByName("OptionSprint");
      } else if (!canSeePlayer && !hasRaisedAlarm) {
        currentState = GuardState::ALARM;
        selectedOption = GetOptionByName("OptionAlarm");
      } else if (!canSeePlayer) {
        currentState = GuardState::PATROLLING;
        selectedOption = GetOptionByName("OptionPatrol");
      } else if (energy < 1) {
        currentState = GuardState::RESTING;
        selectedOption = GetOptionByName("OptionRest");
      }
      break;

    case GuardState::SPRINTING:
      selectedOption = GetOptionByName("OptionSprint");
      if (!canSeePlayer && !hasRaisedAlarm) {
        currentState = GuardState::ALARM;
        selectedOption = GetOptionByName("OptionAlarm");
      } else if (!canSeePlayer) {
        currentState = GuardState::PATROLLING;
        selectedOption = GetOptionByName("OptionPatrol");
      } else if (energy < SPRINT_LOW) {
        currentState = GuardState::CHASING;
        selectedOption = GetOptionByName("OptionChase");
      } else if (energy < 1) {
        currentState = GuardState::RESTING;
        selectedOption = GetOptionByName("OptionRest");
      }
      break;

    case GuardState::INVESTIGATING:
      selectedOption = GetOptionByName("OptionInvestigate");
      if (canSeePlayer) {
        currentState = GuardState::CHASING;
        selectedOption = GetOptionByName("OptionChase");
      } else if (energy < 1) {
        currentState = GuardState::RESTING;
        selectedOption = GetOptionByName("OptionRest");
      }
      break;

    case GuardState::ALARM:
      selectedOption = GetOptionByName("OptionAlarm");
      if (hasRaisedAlarm) {
        currentState = GuardState::PATROLLING;
        selectedOption = GetOptionByName("OptionPatrol");
      }
      break;

    case GuardState::RESTING:
      selectedOption = GetOptionByName("OptionRest");
      if (energy >= 20) {
        currentState = GuardState::PATROLLING;
        selectedOption = GetOptionByName("OptionPatrol");
      }
      break;

    default:
      break;
  }

  // if state changed, call the Pre/Post Transition functions for
  // cleanup/initialisation

  if (_lastState != currentState) {
    actorBlackboard->GetActorContext()->PreTransition();
  }
}

void FSM_Reasoner::SetStartingState(GuardState _state, std::string _stateName) {
  currentState = _state;
  selectedOption = GetOptionByName(_stateName);
}

std::string FSM_Reasoner::GetStateName() {
  if (currentState == GuardState::RESTING)
    return "Resting";
  else if (currentState == GuardState::ALARM)
    return "Alarm";
  else if (currentState == GuardState::CHASING)
    return "Chase";
  else if (currentState == GuardState::INVESTIGATING)
    return "Investigate";
  else if (currentState == GuardState::PATROLLING)
    return "Patrol";
  else if (currentState == GuardState::SPRINTING)
    return "Sprint";
  else
    return "Unknown";
}
