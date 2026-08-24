#pragma once
#include "SkillGraph.h"
#include <vector>

class PracticeScheduler {
public:
  explicit PracticeScheduler(const SkillGraph &graph);
  std::vector<int> generatePracticeOrder() const;

private:
  const SkillGraph &graph_;
};
