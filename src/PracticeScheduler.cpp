#include "PracticeScheduler.h"
#include <queue>
#include <unordered_map>

using namespace std;

namespace {
struct ReadySkillComparator {
  const SkillGraph *graph;

  bool operator()(int a, int b) const {
    const Skill &sa = graph->getNode(a);
    const Skill &sb = graph->getNode(b);
    if (sa.getPriority() != sb.getPriority()) {
      return sa.getPriority() < sb.getPriority();
    }
    return sa.estimatedHours() > sb.estimatedHours();
  }
};
} // namespace

PracticeScheduler::PracticeScheduler(const SkillGraph &graph) : graph_(graph) {}

vector<int> PracticeScheduler::generatePracticeOrder() const {
  if (graph_.hasCycle()) {
    throw CyclicDependencyException(
        "generatePracticeOrder: Circular dependency detected");
  }

  unordered_map<int, int> inDegree;
  for (int id : graph_.getAllNodeIds()) {
    inDegree[id] = graph_.getInDegree(id);
  }

  ReadySkillComparator cmp{&graph_};
  priority_queue<int, vector<int>, ReadySkillComparator> ready(cmp);

  for (const auto &[id, degree] : inDegree) {
    if (degree == 0)
      ready.push(id);
  }

  vector<int> order;
  order.reserve(graph_.nodeCount());

  while (!ready.empty()) {
    int current = ready.top();
    ready.pop();
    order.push_back(current);

    for (int dependent : graph_.getDependents(current)) {
      if (--inDegree[dependent] == 0) {
        ready.push(dependent);
      }
    }
  }

  if (order.size() != graph_.nodeCount()) {
    throw CyclicDependencyException(
        "generatePracticeOrder: cycle detected mid-schedule");
  }

  return order;
}
