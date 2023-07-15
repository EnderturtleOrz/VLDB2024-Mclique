/**
 * @file match.hpp
 * @author P. Foggia (pfoggia\@unisa.it)
 * @author V.Carletti (vcarletti\@unisa.it)
 * @date   December, 2014
 * @brief  Declaration of the match function.
 */

#ifndef MATCH_H
#define MATCH_H

#include <stack>
#include "argraph.hpp"
#include"../global-variable.h"

//#define MAX_SUBGRAPH_MATCHING_NUM 10000

using namespace std;

/**
 * @brief Definition of the match_visitor type.\n
 * A match visitor is a function that is invoked for
 * each match that has been found.
 * If the function returns FALSE, then the next match is
 * searched; else the seach process terminates.
 * @param [in] n Number of nodes.
 * @param [in] c1 Core Set of the first graph.
 * @param [in]c2 Core Set of the second graph.
 * @param [in/out] usr_data User defined parameter.
 * @return TRUE If the matching process must be stopped.
 * @return FALSE If the matching process must continue.
*/
typedef bool (*match_visitor)(int n, node_id c1[], node_id c2[], void *tree_s,
                              void *usr_data, vector<vector<int> > &matched_subgraphs);

//typedef bool (*match_visitor_new)(int n, node_id c1[], node_id c2[], void *tree_s,
//                              void *usr_data, vector<vector<int> > &);

template<typename State>
bool match(State &s, int *pn, node_id c1[], node_id c2[],
           match_visitor vis, void *usr_data, vector<vector<int> > &matched_subgraphs);

template<typename State>
bool match_it(State &s, int *pn, node_id c1[], node_id c2[],
              match_visitor vis, void *usr_data, vector<vector<int> > &matched_subgraphs);

template<typename State>
bool match(State &s, int *pn, node_id c1[], node_id c2[]);


/**
 * @brief  Finds a matching between two graph, if it exists, given the
 * initial state of the matching process.
 * @note  c1 and c2 will contain the ids of the corresponding nodes
 * in the two graphs.
 * @param [in] s Initial State.
 * @param [out] c1 Core Set of the first graph.
 * @param [out] c2 Core Set of the second graph.
 * @param [out] pn Number of matched nodes.
 * @return TRUE If the matching process finds a solution.
 * @return FALSE If the matching process doesn't find solutions.
*/
template<typename State>
bool match(State &s, int *pn, node_id c1[], node_id c2[]) {

    if (s.IsGoal()) {
        *pn = s.CoreLen();
        s.GetCoreSet(c1, c2);
        return true;
    }

    if (s.IsDead())
        return false;

    node_id n1 = NULL_NODE, n2 = NULL_NODE;
    bool found = false;

    while (!found && s.NextPair(&n1, &n2, n1, n2)) {
        if (s.IsFeasiblePair(n1, n2)) {
            State s1(s);
            s1.AddPair(n1, n2);
            found = match(s1, pn, c1, c2);
        }
    }

    return found;
}

/**
 * @brief Visits all the matchings between two graphs,  starting
 * from state s.
 * @note  c1 and c2 will contain the ids of the corresponding nodes
 * in the two graphs.
 * @param [in] s Initial State.
 * @param [out] c1 Core Set of the first graph.
 * @param [out] c2 Core Set of the second graph.
 * @param [out] pn Number of matched nodes.
 * @param [in] vis Matching visitor.
 * @param [in/out] usr_data User defined parameter for the visitor.
 * @return TRUE If if the caller must stop the visit.
 * @return FALSE If if the caller must continue the visit.
*/
template<typename State>
bool match(State &s, int *pn, node_id c1[], node_id c2[],
           match_visitor vis, void *usr_data, vector<vector<int> > &matched_subgraphs) {

    if (SUBGRAPH_LIMIT != -1 && matched_subgraphs.size() >= SUBGRAPH_LIMIT)
        return false;

    if (s.IsGoal()) {
        ++*pn;
        int n = s.CoreLen();
        s.GetCoreSet(c1, c2);
        return vis(n, c1, c2, &s, usr_data, matched_subgraphs);
    }

    if (s.IsDead())
        return false;

    node_id n1 = NULL_NODE, n2 = NULL_NODE;
    while (s.NextPair(&n1, &n2, n1, n2)) {
        if (SUBGRAPH_LIMIT != -1 && matched_subgraphs.size() >= SUBGRAPH_LIMIT)
            return false;
        if (s.IsFeasiblePair(n1, n2)) {
            State s1(s);
            s1.AddPair(n1, n2);
            if (match(s1, pn, c1, c2, vis, usr_data, matched_subgraphs)) {
                return true;
            }
        }
    }
    return false;
}

template<typename State>
bool match_it(State &s0, int *pn, node_id c1[], node_id c2[],
              match_visitor vis, void *usr_data, vector<vector<int> > &matched_subgraphs) {
    stack<State> stateStack;
    stateStack.push(s0);

    while (!stateStack.empty()) {
        State s = stateStack.top();

        if (s.IsGoal()) {
            ++*pn;
            int n = s.CoreLen();
            s.GetCoreSet(c1, c2);
            return vis(n, c1, c2, &s, usr_data, matched_subgraphs);
        }

        if (s.IsDead())
            return false;

        node_id n1 = NULL_NODE, n2 = NULL_NODE;
        while (s.NextPair(&n1, &n2, n1, n2)) {
            if (s.IsFeasiblePair(n1, n2)) {
                State child(s);
                child.AddPair(n1, n2);
                stateStack.push(child);
            }
        }

    }
    return false;
}

#endif
