#pragma once
#include "../AI_Framework/framework.h"
#include "AIActor_Guard.h"
#include "FSM_Reasoner.h"
/*
 * AIConstructor_FSM_Stealth is responsible for defining the AI components the
 * AI Reasoner will use when making decisions
 */
class AIConstructor_FSM_Stealth : public AIConstructorBase {
 public:
  bool Init() override;
  void DefineActions() override;
  void DefineConsiderations() override;
  void DefineOptions() override;
};
