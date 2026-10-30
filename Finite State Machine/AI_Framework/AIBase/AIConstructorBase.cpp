#include "AIConstructorBase.h"

#include <iostream>

#include "../Actions/AIAction.h"
#include "../Considerations/AIConsideration.h"
#include "../Options/AIOption.h"

void AIConstructorBase::DefineAI() {
  DefineActions();
  DefineConsiderations();
  DefineOptions();
}

std::vector<AIOptionDefinition> AIConstructorBase::GetOptionVector(
    std::vector<std::string> _selectedoptions) {
  std::vector<AIOptionDefinition> _optionVector;

  for (const auto& optionID : _selectedoptions) {
    if (!options.contains(optionID)) {
      std::cout << "ERROR Matching Options : returning empty vector"
                << std::endl;
      std::cout << "Option being added: " << optionID << std::endl;
      return _optionVector;
    }
  }

  // for all options in the request list, add from the unordered map
  for (const auto& optionID : _selectedoptions) {
    _optionVector.push_back(options[optionID]);
  }

  return _optionVector;
}

std::vector<AIOptionDefinition> AIConstructorBase::GetOptionVector() {
  std::vector<AIOptionDefinition> _optionVector;

  // for all options in the request list, add from the unordered map
  for (const auto& option : options) {
    _optionVector.push_back(option.second);
  }

  return _optionVector;
}

AIOptionDefinition AIConstructorBase::GetOptionByName(std::string _optionName) {
  return (options[_optionName]);
}

void AIConstructorBase::AddActionByName(
    std::string _actionName,
    std::function<ActionStatus(AIBrainBlackboardBase&)> _function) {
  // Define an action, and init with the function which will be called when the
  // action is selected
  auto _action = std::make_shared<AIAction>();
  _action->Init(_actionName, _function);
  actions.insert({_actionName, _action});
}

void AIConstructorBase::AddConsiderationByName(
    std::string _actionName,
    std::function<bool(AIBrainBlackboardBase&)> _function) {
  // Define an action, and init with the function which will be called when the
  // action is selected
  auto _consideration = std::make_shared<AIConsideration>();
  _consideration->Init(_actionName, _function);
  considerations.insert({_actionName, _consideration});
}

void AIConstructorBase::AddOptionByName(std::string _optionName,
                                        std::string _actionName,
                                        int _priority) {
  // create an option for action
  AIOptionDefinition _option = AIOptionDefinition();
  _option.SetOptionName(_optionName);
  _option.SetActionName(_actionName);
  _option.SetPriority(_priority);
  options.insert({_optionName, _option});
}

void AIConstructorBase::AddOptionConsideration(std::string _optionName,
                                               std::string _considerationName) {
  // create a consideration for an option
  options[_optionName].AddConsiderationName(_considerationName);
}