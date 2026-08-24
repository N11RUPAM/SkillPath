#pragma once
#include <string>

class SkillVisitor;
class Skill {
public:
  Skill(int id, std::string name, int priority)
      : id_(id), name_(std::move(name)), priority_(priority) {}

  virtual ~Skill() = default;

  int getId() const { return id_; }
  const std::string &getName() const { return name_; }
  int getPriority() const { return priority_; }

  virtual int estimatedHours() const = 0;

  virtual double difficultyScore() const = 0;

  virtual std::string category() const = 0;

  virtual void accept(SkillVisitor &visitor) const = 0;

protected:
  int id_;
  std::string name_;
  int priority_;
};
