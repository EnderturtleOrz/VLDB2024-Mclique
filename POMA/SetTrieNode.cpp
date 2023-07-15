#include"SetTrieNode.h"

void SetTrieNode::AddChild(int id, int &hash_id, bool is_end_point) {
    if (!ExistChild(id)) {
        child_map_->insert(pair<int, SetTrieNode *>(id,
                                                    new SetTrieNode(id, is_end_point, height_ + 1, hash_id++)));
    }
}

void SetTrieNode::AddSet(const set<int> &s, int &hash_id) {
    SetTrieNode *current = this;
    for (const auto &e : s) {
        current->AddChild(e, hash_id);
        current = current->GetChild(e);
    }
    current->UpdateEndPoint(true);
}

void SetTrieNode::release() {
    for (auto e : *child_map_) {
        e.second->release();
    }
    delete child_map_;
}