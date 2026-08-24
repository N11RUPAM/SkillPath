#include "SkillGraph.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>

using namespace std;

void SkillGraph::addNode(SkillCategory category, int id, const string &name,
                         int priority, int baseHours) {
  nodes_[id] = SkillFactory::create(category, id, name, priority, baseHours);
  adjList_[id];
  if (inDegree_.find(id) == inDegree_.end()) {
    inDegree_[id] = 0;
  }
}

void SkillGraph::addDependency(int prereqId, int nextId) {
  if (nodes_.find(prereqId) == nodes_.end())
    throw UnknownSkillException("Unknown prerequisite id");
  if (nodes_.find(nextId) == nodes_.end())
    throw UnknownSkillException("Unknown dependent id");

  adjList_[prereqId].push_back(nextId);
  inDegree_[nextId]++;
}

int SkillGraph::calculateTotalRoadmapHours() const {
  int total = 0;
  for (const auto &[id, skillPtr] : nodes_) {
    total += skillPtr->estimatedHours();
  }
  return total;
}

bool SkillGraph::computeTopoOrder(vector<int> &order) const {
  unordered_map<int, int> inDegreeCopy = inDegree_;
  queue<int> ready;

  for (const auto &[id, degree] : inDegreeCopy) {
    if (degree == 0)
      ready.push(id);
  }

  order.clear();
  order.reserve(nodes_.size());

  while (!ready.empty()) {
    int current = ready.front();
    ready.pop();
    order.push_back(current);

    auto it = adjList_.find(current);
    if (it == adjList_.end())
      continue;

    for (int dependent : it->second) {
      if (--inDegreeCopy[dependent] == 0) {
        ready.push(dependent);
      }
    }
  }

  return order.size() == nodes_.size();
}

bool SkillGraph::hasCycle() const {
  vector<int> order;
  return !computeTopoOrder(order);
}

vector<int> SkillGraph::topologicalOrder() const {
  vector<int> order;
  if (!computeTopoOrder(order)) {
    throw CyclicDependencyException(
        "topologicalOrder: roadmap contains a circular dependency");
  }
  return order;
}

void SkillGraph::computeLongestDistances(
    unordered_map<int, int> &dist, unordered_map<int, int> &parent) const {
  vector<int> order;
  if (!computeTopoOrder(order)) {
    throw CyclicDependencyException(
        "criticalPath: cannot compute longest path on a cyclic roadmap");
  }

  dist.clear();
  parent.clear();

  for (const auto &[id, skillPtr] : nodes_) {
    dist[id] = skillPtr->estimatedHours();
    parent[id] = -1;
  }

  for (int u : order) {
    auto it = adjList_.find(u);
    if (it == adjList_.end())
      continue;

    for (int v : it->second) {
      int candidate = dist[u] + nodes_.at(v)->estimatedHours();
      if (candidate > dist[v]) {
        dist[v] = candidate;
        parent[v] = u;
      }
    }
  }
}

vector<int>
SkillGraph::reconstructPath(int endId,
                            const unordered_map<int, int> &parent) const {
  vector<int> path;
  int current = endId;
  while (current != -1) {
    path.push_back(current);
    auto it = parent.find(current);
    current = (it == parent.end()) ? -1 : it->second;
  }
  reverse(path.begin(), path.end());
  return path;
}

vector<int> SkillGraph::criticalPath(int &totalHours) const {
  unordered_map<int, int> dist, parent;
  computeLongestDistances(dist, parent);

  int bestId = -1;
  int bestDist = numeric_limits<int>::min();
  for (const auto &[id, d] : dist) {
    if (d > bestDist) {
      bestDist = d;
      bestId = id;
    }
  }

  totalHours = (bestId == -1) ? 0 : bestDist;
  return bestId == -1 ? vector<int>{} : reconstructPath(bestId, parent);
}

vector<int> SkillGraph::criticalPathTo(int goalId, int &totalHours) const {
  if (nodes_.find(goalId) == nodes_.end())
    throw UnknownSkillException("criticalPathTo: unknown goal id");

  unordered_map<int, int> dist, parent;
  computeLongestDistances(dist, parent);

  totalHours = dist.at(goalId);
  return reconstructPath(goalId, parent);
}

const Skill &SkillGraph::getNode(int id) const {
  auto it = nodes_.find(id);
  if (it == nodes_.end())
    throw UnknownSkillException("getNode: unknown id");
  return *(it->second);
}

Skill &SkillGraph::getNodeMutable(int id) {
  auto it = nodes_.find(id);
  if (it == nodes_.end())
    throw UnknownSkillException("getNodeMutable: unknown id");
  return *(it->second);
}

vector<int> SkillGraph::getAllNodeIds() const {
  vector<int> ids;
  ids.reserve(nodes_.size());
  for (const auto &[id, skillPtr] : nodes_)
    ids.push_back(id);
  return ids;
}

const vector<int> &SkillGraph::getDependents(int id) const {
  static const vector<int> empty;
  auto it = adjList_.find(id);
  return it == adjList_.end() ? empty : it->second;
}

int SkillGraph::getInDegree(int id) const {
  auto it = inDegree_.find(id);
  return it == inDegree_.end() ? 0 : it->second;
}

void SkillGraph::acceptForAll(SkillVisitor &visitor) const {
  for (const auto &[id, skillPtr] : nodes_) {
    skillPtr->accept(visitor);
  }
}

void SkillGraph::printGraph() const {
  for (const auto &[id, skillPtr] : nodes_) {
    cout << "[" << id << "] " << skillPtr->getName() << " ("
         << skillPtr->category() << ", hours=" << skillPtr->estimatedHours()
         << ", priority=" << skillPtr->getPriority() << ")\n";
    const auto &deps = getDependents(id);
    if (!deps.empty()) {
      cout << "    unlocks: ";
      for (size_t i = 0; i < deps.size(); ++i) {
        cout << nodes_.at(deps[i])->getName();
        if (i + 1 < deps.size())
          cout << ", ";
      }
      cout << "\n";
    }
  }
}
