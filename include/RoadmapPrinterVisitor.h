#pragma once
#include "SkillTypes.h"
#include "SkillVisitor.h"
#include <iostream>

class RoadmapPrinterVisitor : public SkillVisitor {
public:
  void visit(const TechnicalSkill &skill) override {
    std::cout << "  [Technical] " << skill.getName()
              << " | hours=" << skill.estimatedHours()
              << " | difficulty=" << skill.difficultyScore()
              << " | priority=" << skill.getPriority() << "\n";
  }

  void visit(const TheorySkill &skill) override {
    std::cout << "  [Theory]    " << skill.getName()
              << " | hours=" << skill.estimatedHours()
              << " | difficulty=" << skill.difficultyScore()
              << " | priority=" << skill.getPriority() << "\n";
  }

  void visit(const RepertoireSkill &skill) override {
    std::cout << "  [Repertoire]" << skill.getName()
              << " | hours=" << skill.estimatedHours()
              << " | priority=" << skill.getPriority()
              << (skill.isRelatedSkillMastered()
                      ? "  (discount active: related skill mastered)"
                      : "  (no discount yet)")
              << "\n";
  }
};
