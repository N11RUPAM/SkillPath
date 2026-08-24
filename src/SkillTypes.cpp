#include "SkillTypes.h"
#include "SkillVisitor.h"

void TechnicalSkill::accept(SkillVisitor &visitor) const {
  visitor.visit(*this);
}

void TheorySkill::accept(SkillVisitor &visitor) const { visitor.visit(*this); }

void RepertoireSkill::accept(SkillVisitor &visitor) const {
  visitor.visit(*this);
}
