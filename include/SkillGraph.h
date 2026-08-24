#pragma once
#include "Skill.h"
#include "SkillFactory.h"
#include "SkillVisitor.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class CyclicDependencyException : public std::runtime_error {
public:
  explicit CyclicDependencyException(const std::string &msg)
      : std::runtime_error(msg) {}
};

class UnknownSkillException : public std::runtime_error {
public:
  explicit UnknownSkillException(const std::string &msg)
      : std::runtime_error(msg) {}
};

class SkillGraph {
public:
  void addNode(SkillCategory category, int id, const std::string &name,
               int priority, int baseHours);

  void addDependency(int prereqId, int nextId);

  bool hasCycle() const;

  std::vector<int> topologicalOrder() const;

  std::vector<int> criticalPath(int &totalHours) const;

  std::vector<int> criticalPathTo(int goalId, int &totalHours) const;

  const Skill &getNode(int id) const;

  Skill &getNodeMutable(int id);

  std::vector<int> getAllNodeIds() const;
  const std::vector<int> &getDependents(int id) const;
  int getInDegree(int id) const;
  size_t nodeCount() const { return nodes_.size(); }

  int calculateTotalRoadmapHours() const;

  void acceptForAll(SkillVisitor &visitor) const;

  void printGraph() const;

private:
  std::unordered_map<int, std::shared_ptr<Skill>> nodes_;
  std::unordered_map<int, std::vector<int>> adjList_;
  std::unordered_map<int, int> inDegree_;

  bool computeTopoOrder(std::vector<int> &order) const;

  void computeLongestDistances(std::unordered_map<int, int> &dist,
                               std::unordered_map<int, int> &parent) const;

  std::vector<int>
  reconstructPath(int endId, const std::unordered_map<int, int> &parent) const;
};
