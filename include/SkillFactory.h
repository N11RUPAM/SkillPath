#pragma once
#include "SkillTypes.h"
#include <memory>
#include <stdexcept>
#include <string>

enum class SkillCategory { TECHNICAL, THEORY, REPERTOIRE };

inline const char *skillCategoryToString(SkillCategory c) {
  switch (c) {
  case SkillCategory::TECHNICAL:
    return "TECHNICAL";
  case SkillCategory::THEORY:
    return "THEORY";
  case SkillCategory::REPERTOIRE:
    return "REPERTOIRE";
  }
  return "UNKNOWN";
}

class SkillFactory {
public:
  static std::shared_ptr<Skill> create(SkillCategory category, int id,
                                       const std::string &name, int priority,
                                       int baseHours) {
    switch (category) {
    case SkillCategory::TECHNICAL:
      return std::make_shared<TechnicalSkill>(id, name, priority, baseHours);
    case SkillCategory::THEORY:
      return std::make_shared<TheorySkill>(id, name, priority, baseHours);
    case SkillCategory::REPERTOIRE:
      return std::make_shared<RepertoireSkill>(id, name, priority, baseHours);
    }
    throw std::invalid_argument("SkillFactory: unknown SkillCategory");
  }
};
