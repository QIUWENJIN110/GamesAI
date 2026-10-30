#pragma once
#include "../AI_Framework/framework.h"
// #include "../AI_Framework//AIBase/AIReasonerBase.h"
class FSM_Reasoner : public AIReasonerBase {
 public:
  ~FSM_Reasoner();
  // States that the FSM can be in
  enum class GuardState {
    RESTING,
    PATROLLING,
    INVESTIGATING,
    CHASING,
    SPRINTING,
    ALARM
  };

  // AIReasonerBase overrides
  bool Init(std::string _id, AIBrainBlackboardBase& _context) override;
  void Think() override;
  std::string GetStateName();
  // Initialisation
  void SetStartingState(GuardState _state, std::string _stateName);

  // Tracking
  GuardState currentState;
};
