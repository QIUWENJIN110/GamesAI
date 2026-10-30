#include "AIConstructor_FSM_Stealth.h"

#include <iostream>

#include "AIActor_Guard.h"
#include "FSM_Reasoner.h"

bool AIConstructor_FSM_Stealth::Init() { return true; }

/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_FSM_Stealth::DefineActions() {
  // ** Rest Action **

  // Define the function for Resting
  auto restFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
    AIActor_Guard* actor = static_cast<AIActor_Guard*>(bb.GetActorContext());

    // Call behaviour functions here
    actor->Rest();

    return ActionStatus::ACTION_SUCCESS;
  };

  // Link the function to an identifier
  AddActionByName("ActionRest", restFunction);

  // ** Patrol Action **

  // Define the function for Patrolling
  auto patrolFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
    AIActor_Guard* actor = static_cast<AIActor_Guard*>(bb.GetActorContext());

    // Call behaviour functions here
    actor->Patrol();

    return ActionStatus::ACTION_SUCCESS;
  };

  // Link the function to an identifier
  AddActionByName("ActionPatrol", patrolFunction);

  // TASK TODO - add additional actions below
  // - use the above Patrol and Rest actions as examples

  // ** Investigate Action **

  // Define the function for Investigating
  auto investigateFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
    AIActor_Guard* actor = static_cast<AIActor_Guard*>(bb.GetActorContext());

    // Call behaviour functions here
    actor->Investigate();

    return ActionStatus::ACTION_SUCCESS;
  };

  // Link the function to an identifier
  AddActionByName("ActionInvestigate", investigateFunction);

  //  ** Chase Action **

  // Define the function for Chasing
  auto chaseFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
    AIActor_Guard* actor = static_cast<AIActor_Guard*>(bb.GetActorContext());

    // Call behaviour functions here
    actor->Chase();

    return ActionStatus::ACTION_SUCCESS;
  };

  // Link the function to an identifier
  AddActionByName("ActionChase", chaseFunction);

  // Define the function for Sprinting
  auto sprintFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
    AIActor_Guard* actor = static_cast<AIActor_Guard*>(bb.GetActorContext());

    // Call behaviour functions here
    actor->Sprint();

    return ActionStatus::ACTION_SUCCESS;
  };

  AddActionByName("ActionSprint", sprintFunction);

  // Define the function for RaiseAlarm
  auto alarmFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
    AIActor_Guard* actor = static_cast<AIActor_Guard*>(bb.GetActorContext());

    // Call behaviour functions here
    actor->RaiseAlarm();

    return ActionStatus::ACTION_SUCCESS;
  };

  AddActionByName("ActionAlarm", alarmFunction);
}

void AIConstructor_FSM_Stealth::DefineConsiderations() {}

/*
 * DefineOptions links the Options with Considerations and Actions
 */
void AIConstructor_FSM_Stealth::DefineOptions() {
  AddOptionByName("OptionRest", "ActionRest");
  AddOptionByName("OptionPatrol", "ActionPatrol");
  AddOptionByName("OptionInvestigate", "ActionInvestigate");
  AddOptionByName("OptionChase", "ActionChase");
  AddOptionByName("OptionSprint", "ActionSprint");
  AddOptionByName("OptionAlarm", "ActionAlarm");

  // TASK TODO - add code to initialise new options, linking an action
}
