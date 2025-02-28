// Mateo Guzman 
// 411 Fall Semester 
// Final Project 

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <sstream>
#include <set>
using namespace std;

const int ALLOW = 1;
const int BLOCK = 0;

struct Rule {
    int id;       
    string source;
    string destination;
    string protocol;
    int action;   // 1 = allow, 0 = block

    Rule(int id, string s, string d, string p, int a)
        : id(id), source(s), destination(d), protocol(p), action(a) {}
};

struct Node {
    Rule rule;
    double g;    //Cost from start (we'll keep this simple)
    double h;    //Heuristic cost (set to 0)
    double f;    //f = g + h
    vector<int> path;

    Node(Rule r, double g_cost, double h_cost, vector<int> p)
        : rule(r), g(g_cost), h(h_cost), f(g_cost + h_cost), path(p) {}
};

struct CompareNode {
    bool operator()(Node const& n1, Node const& n2) {
        if (n1.f == n2.f) {
            // Tie-break by rule ID to ensure stable order
            return n1.rule.id > n2.rule.id;
        }
        return n1.f > n2.f;
    }
};

//function prototypes
vector<Rule> readRules(int& num_rules, string& start_node, string& end_node);
double heuristic(const Rule& current, const string& end_node);
bool isGoal(const Rule& rule, const string& end_node);
vector<Rule> getNeighbors(const Rule& current, const vector<Rule>& rules);
bool match(const string& rule_field, const string& ip);
void optimizeFirewallRules(const vector<Rule>& rules, const string& start_node, const string& end_node);

//read rules from standard input
vector<Rule> readRules(int& num_rules, string& start_node, string& end_node) {
    vector<Rule> rules;
    string line;
    getline(cin, line);
    while (line.empty() || line[0] == '#') {
        if(!getline(cin, line)) break;
    }
    num_rules = stoi(line);
    for (int i = 0; i < num_rules; ++i) {
        do {
            if(!getline(cin, line)) break;
        } while (line.empty() || line[0] == '#');
        istringstream iss(line);
        string s, d, p;
        int a;
        iss >> s >> d >> p >> a;
        rules.push_back(Rule(i + 1, s, d, p, a));
    }
    while (getline(cin, line)) {
        if (!line.empty() && line[0] != '#') {
            break;
        }
    }
    istringstream iss(line);
    iss >> start_node >> end_node;
    return rules;
}

//heuristic function - set to 0 for simplicity
double heuristic(const Rule& current, const string& end_node) {
    return 0.0;
}

bool isGoal(const Rule& rule, const string& end_node) {
    //a goal if we have reached the end_node exactly
    return (rule.destination == end_node);
}

vector<Rule> getNeighbors(const Rule& current, const vector<Rule>& rules) {
    vector<Rule> neighbors;
    for (auto &rule : rules) {
        if (rule.id != current.id && rule.action == ALLOW) {
            //chain: current.rule.destination must match rule.source
            if (match(rule.source, current.destination)) {
                neighbors.push_back(rule);
            }
        }
    }
    return neighbors;
}

//odified match function: If the rule field is ANY, it matches any IP.
//otherwise, it must match exactly.
bool match(const string& rule_field, const string& ip) {
    if (rule_field == "ANY") return true;
    return (rule_field == ip);
}

void optimizeFirewallRules(const vector<Rule>& rules, const string& start_node, const string& end_node) {
    priority_queue<Node, vector<Node>, CompareNode> open_set;
    set<int> closed_set;

    //check if there is any exact match for the start_node
    bool exact_match_found = false;
    for (auto &rule : rules) {
        if (rule.action == ALLOW && rule.source == start_node) {
            exact_match_found = true;
            break;
        }
    }

    //initialize the open_set
    for (auto &rule : rules) {
        if (rule.action == ALLOW) {
            if ((exact_match_found && rule.source == start_node) ||
                (!exact_match_found && match(rule.source, start_node))) {
                double h = heuristic(rule, end_node);
                vector<int> path = {rule.id};
                open_set.push(Node(rule, 0.0, h, path));
            }
        }
    }

    while (!open_set.empty()) {
        Node current = open_set.top();
        open_set.pop();

        if (isGoal(current.rule, end_node)) {
            cout << "# Optimized Rule Path:" << endl;
            for (int id : current.path) {
                const Rule& r = rules[id - 1];
                cout << id << ". " << (r.action == ALLOW ? "Allow" : "Block")
                     << " traffic from " << r.source << " to " << r.destination
                     << " over " << r.protocol << endl;
            }
            return;
        }

        if (closed_set.find(current.rule.id) != closed_set.end()) {
            continue;
        }
        closed_set.insert(current.rule.id);

        vector<Rule> neighbors = getNeighbors(current.rule, rules);
        for (const auto& neighbor : neighbors) {
            //already checked action == ALLOW in getNeighbors
            double g_cost = current.g + 1.0; 
            double h_cost = heuristic(neighbor, end_node);
            vector<int> new_path = current.path;
            new_path.push_back(neighbor.id);

            open_set.push(Node(neighbor, g_cost, h_cost, new_path));
        }
    }

    cout << "No valid path found." << endl;
}

int main() {
    int num_rules;
    string start_node, end_node;
    vector<Rule> rules = readRules(num_rules, start_node, end_node);
    optimizeFirewallRules(rules, start_node, end_node);
    return 0;
}

