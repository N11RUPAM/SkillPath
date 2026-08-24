/*
 * DEMO INPUT (menu choices, id, name, category, hours, priority, deps)
 */

#include "JsonExportVisitor.h"
#include "PracticeScheduler.h"
#include "RoadmapPrinterVisitor.h"
#include "SkillGraph.h"
#include <iostream>

using namespace std;

int main() {
  SkillGraph graph;

  while (true) {
    cout << "\n=== SkillPath v2 (Polymorphic Skill Hierarchy) ===\n";
    cout << "1. Add Skill\n";
    cout << "2. Add Dependency (prereq before next)\n";
    cout << "3. Generate Priority-Driven Practice Order (Kahn's + Max-Heap)\n";
    cout << "4. Critical Path To A Goal Skill (Longest-Path DP)\n";
    cout << "5. Print Roadmap (Visitor: RoadmapPrinterVisitor)\n";
    cout << "6. Export Roadmap As JSON (Visitor: JsonExportVisitor)\n";
    cout << "7. Mark a Repertoire Skill's Related Skill as Mastered (discount)\n";
    cout << "8. System Bottleneck Analysis (Sequential vs Critical Path)\n";
    cout << "9. Exit\n";
    cout << "Choice: ";

    int choice;
    if (!(cin >> choice))
      break;

    try {
      if (choice == 1) {
        int id, priority, baseHours, catChoice;
        string name;
        cout << "Enter Node ID: ";
        cin >> id;
        cout << "Enter Skill Name (Use_Underscores): ";
        cin >> name;
        cout << "Category (1=Technical, 2=Theory, 3=Repertoire): ";
        cin >> catChoice;
        cout << "Enter Mastery Hours: ";
        cin >> baseHours;
        cout << "Enter Priority (1-10): ";
        cin >> priority;

        SkillCategory category = catChoice == 1   ? SkillCategory::TECHNICAL
                                  : catChoice == 2 ? SkillCategory::THEORY
                                                   : SkillCategory::REPERTOIRE;

        graph.addNode(category, id, name, priority, baseHours);
        cout << "Skill added as " << skillCategoryToString(category) << ".\n";

      } else if (choice == 2) {
        int prereq, next;
        cout << "Prerequisite Skill ID: ";
        cin >> prereq;
        cout << "Dependent Skill ID: ";
        cin >> next;
        graph.addDependency(prereq, next);
        cout << "Dependency added.\n";

      } else if (choice == 3) {
        PracticeScheduler scheduler(graph);
        auto order = scheduler.generatePracticeOrder();
        cout << "Practice order: ";
        for (int id : order)
          cout << graph.getNode(id).getName() << " ";
        cout << "\n";

      } else if (choice == 4) {
        int goalId, totalHours;
        cout << "Goal Skill ID: ";
        cin >> goalId;
        auto path = graph.criticalPathTo(goalId, totalHours);
        cout << "Minimum hours to unlock this skill: " << totalHours << "\n";
        cout << "Critical path: ";
        for (int id : path)
          cout << graph.getNode(id).getName() << " -> ";
        cout << "DONE\n";

      } else if (choice == 5) {
        RoadmapPrinterVisitor printer;
        graph.acceptForAll(printer);

      } else if (choice == 6) {
        JsonExportVisitor exporter;
        graph.acceptForAll(exporter);
        cout << exporter.getJson();

      } else if (choice == 7) {
        int id;
        cout << "Repertoire Skill ID to mark discounted: ";
        cin >> id;
        Skill &skill = graph.getNodeMutable(id);
        // dynamic_cast, not a string check on category(): this asks the
        // runtime type system itself "is this object actually a
        // RepertoireSkill?" via RTTI, rather than trusting a label that
        // could drift out of sync with the real type. Returns nullptr on
        // failure instead of undefined behavior, so it's always safe to
        // check.
        if (auto *rep = dynamic_cast<RepertoireSkill *>(&skill)) {
          rep->markRelatedSkillMastered();
          cout << "Marked. New estimated hours: " << rep->estimatedHours() << "\n";
        } else {
          cout << "That skill is not a Repertoire skill — no discount applies.\n";
        }

      } else if (choice == 8) {
        int totalHours;
        graph.criticalPath(totalHours);
        cout << "Sequential (one learner, all skills): "
             << graph.calculateTotalRoadmapHours() << " hours\n";
        cout << "Critical path (infinite parallel learners): " << totalHours
             << " hours\n";

      } else if (choice == 9) {
        break;
      } else {
        cout << "Invalid choice.\n";
      }
    } catch (const exception &e) {
      cout << "Error: " << e.what() << "\n";
    }
  }

  return 0;
}
