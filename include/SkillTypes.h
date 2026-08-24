#pragma once
#include "Skill.h"

class TechnicalSkill : public Skill {
public:
  TechnicalSkill(int id, std::string name, int priority, int baseHours)
      : Skill(id, std::move(name), priority), baseHours_(baseHours) {}

  int estimatedHours() const override { return baseHours_; }
  double difficultyScore() const override { return baseHours_ * 1.2; }
  std::string category() const override { return "Technical"; }
  void accept(SkillVisitor &visitor) const override;

private:
  int baseHours_;
};

class TheorySkill : public Skill {
public:
  TheorySkill(int id, std::string name, int priority, int baseHours)
      : Skill(id, std::move(name), priority), baseHours_(baseHours) {}

  int estimatedHours() const override { return baseHours_; }
  double difficultyScore() const override { return baseHours_ * 0.8; }
  std::string category() const override { return "Theory"; }
  void accept(SkillVisitor &visitor) const override;

private:
  int baseHours_;
};

class RepertoireSkill : public Skill {
public:
  RepertoireSkill(int id, std::string name, int priority, int baseHours)
      : Skill(id, std::move(name), priority), baseHours_(baseHours),
        relatedSkillMastered_(false) {}

  void markRelatedSkillMastered() { relatedSkillMastered_ = true; }
  bool isRelatedSkillMastered() const { return relatedSkillMastered_; }

  int estimatedHours() const override {
    if (relatedSkillMastered_) {
      return static_cast<int>(baseHours_ * kDecayFactor);
    }
    return baseHours_;
  }

  double difficultyScore() const override {
    return static_cast<double>(estimatedHours());
  }

  std::string category() const override { return "Repertoire"; }
  void accept(SkillVisitor &visitor) const override;

private:
  static constexpr double kDecayFactor = 0.7;
  int baseHours_;
  bool relatedSkillMastered_;
};
