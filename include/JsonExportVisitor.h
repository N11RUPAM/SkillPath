#pragma once
#include "SkillTypes.h"
#include "SkillVisitor.h"
#include <sstream>
#include <string>

class JsonExportVisitor : public SkillVisitor {
public:
  void visit(const TechnicalSkill &skill) override {
    out_ << "{ \"id\": " << skill.getId() << ", \"name\": \"" << skill.getName()
         << "\", \"category\": \"Technical\", \"hours\": "
         << skill.estimatedHours()
         << ", \"difficulty\": " << skill.difficultyScore() << " }\n";
  }

  void visit(const TheorySkill &skill) override {
    out_ << "{ \"id\": " << skill.getId() << ", \"name\": \"" << skill.getName()
         << "\", \"category\": \"Theory\", \"hours\": "
         << skill.estimatedHours()
         << ", \"difficulty\": " << skill.difficultyScore() << " }\n";
  }

  void visit(const RepertoireSkill &skill) override {
    out_ << "{ \"id\": " << skill.getId() << ", \"name\": \"" << skill.getName()
         << "\", \"category\": \"Repertoire\", \"hours\": "
         << skill.estimatedHours() << ", \"discountActive\": "
         << (skill.isRelatedSkillMastered() ? "true" : "false") << " }\n";
  }

  std::string getJson() const { return out_.str(); }

private:
  std::ostringstream out_;
};
