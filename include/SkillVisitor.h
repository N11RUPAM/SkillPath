#pragma once

class TechnicalSkill;
class TheorySkill;
class RepertoireSkill;

class SkillVisitor {
public:
  virtual ~SkillVisitor() = default;

  virtual void visit(const TechnicalSkill &skill) = 0;
  virtual void visit(const TheorySkill &skill) = 0;
  virtual void visit(const RepertoireSkill &skill) = 0;
};
