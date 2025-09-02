#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <vector>
#include <string>
#include <ctime>

struct TreeNode {
    int version_id;
    std::string content; // Empty if not a snapshot
    std::string message;
    time_t created_timestamp;
    time_t snapshot_timestamp; // Null if not a snapshot
    TreeNode *parent;
    std::vector<TreeNode *> children;

    TreeNode(const std::string &content, int version_id) {
        this->version_id = version_id;
        this->content = content;
        message = "";
        created_timestamp = time(nullptr);
        snapshot_timestamp = static_cast<time_t>(-1);
        parent = nullptr;
    }

    bool isSnapshot() const {
        // if treenode is a snapshot
        return (message.length() != 0);
    }
};

#endif
