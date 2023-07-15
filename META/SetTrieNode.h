/* 
 * File:   SetTrieNode.h
 * Author: hjf
 *
 * Created on June 17, 2017, 3:06 PM
 */

#ifndef SETTRIENODE_H
#define    SETTRIENODE_H

#include<set>
#include<map>
#include<cstddef>

using namespace std;

class SetTrieNode {
public:

    SetTrieNode() {
    }

    SetTrieNode(int id, bool is_end_point, int height, int hash_id) : id_(id),
                                                                      is_end_point_(is_end_point), height_(height),
                                                                      hash_id_(hash_id) {
        child_map_ = new map<int, SetTrieNode *>();
    }

    ~SetTrieNode() {
    }

    int GetHeight() const { return height_; }

    int GetParent() const { return parent_; }

    int GetId() const { return id_; }

    int GetHashId() const { return hash_id_; }

    bool operator<(const SetTrieNode &stn) const {
        return height_ > stn.GetHeight() || (height_ == stn.GetHeight() && hash_id_ < stn.GetHashId());
    }

    void AddChild(int id, int &hash_id, bool is_end_point = false);

    SetTrieNode *GetChild(int id) const;

    bool ExistChild(int id) const;

    bool IsEndPoint() const;

    void UpdateEndPoint(bool is_end_point);

    void AddSet(const set<int> &s, int &hash_id);

    void release();

private:
    int id_;
    int height_;
    bool is_end_point_;
    int parent_;
    int hash_id_;
    map<int, SetTrieNode *> *child_map_;
};

inline bool SetTrieNode::IsEndPoint() const {
    return is_end_point_;
}

inline void SetTrieNode::UpdateEndPoint(bool is_end_point) {
    is_end_point_ |= is_end_point;
}

inline SetTrieNode *SetTrieNode::GetChild(int id) const {
    return child_map_->find(id) != child_map_->end() ? (*child_map_)[id] : NULL;
}

inline bool SetTrieNode::ExistChild(int id) const {
    return child_map_->find(id) != child_map_->end() ? true : false;
}

#endif    /* SETTRIENODE_H */

