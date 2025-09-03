#ifndef FILE_HPP
#define FILE_HPP

#include "TreeNode.hpp"
#include "HashMap.hpp"
#include "Myers.hpp"
#include <string>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <iomanip>

inline std::string timeToString(time_t t) {
    // function to convert epoch time to DD/MM/YY HH:MM:SS
    std::tm *tm_ptr = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%d/%m/%y %H:%M:%S");
    return oss.str();
}

struct File {
    TreeNode *root;
    TreeNode *active_version;
    HashMap<int, TreeNode *> version_map;
    int total_versions;
    std::string name;
    time_t last_modification_time;

    File(const std::string &filename, const std::string &content) {
        name = filename;
        root = new TreeNode(content, 0);
        total_versions = 1;
        active_version = root;
        version_map.insert(0, root);
        snapshot("Initial Snapshot");
        last_modification_time = root->created_timestamp;
    }

    void snapshot(const std::string &message) {
        // takes snapshot of current version
        if (active_version->isSnapshot()) {
            throw std::invalid_argument("Current version is already a snapshot");
        }
        active_version->snapshot_timestamp = time(nullptr);
        active_version->message = message;
    }

    void newversion(const std::string &content, TreeNode *parent, int version_id) {
        // creates new version with given id and parent
        if (version_map.count(version_id)) {
            throw std::invalid_argument("Version ID must be unique.");
        }
        TreeNode *new_version = new TreeNode(content, version_id);
        total_versions++;
        new_version->parent = parent;
        parent->children.push_back(new_version);
        version_map.insert(version_id, new_version);
        active_version = new_version;
        last_modification_time = active_version->created_timestamp;
    }

    void compare(int v1, int v2) const {
        // prints diff between two versions
        if (!(version_map.count(v1)) || !(version_map.count(v2))) {
            throw std::out_of_range("File doesn't have given versionID.");
        }
        std::cout << "Comparing v" << v1 << " -> v" << v2 << "\n";
        printdiff(version_map.get(v1)->content, version_map.get(v2)->content);
    }

    void print_details() const {
        // prints details of file
        std::cout << "Active Version Size (in chars) : " << active_version->content.size() << "\n";
        std::cout << "Total Version : " << total_versions << "\n";
        std::cout << "Last Modified Time : " << timeToString(last_modification_time) << "\n";
        std::cout << "Active VersionID : " << active_version->version_id << "\n";
        std::cout << "Active Version Creation Time : " << timeToString(active_version->created_timestamp) << "\n";
        std::cout << "Active Version is " << (active_version->isSnapshot() ? "" : "not ") << "a snapshot.\n";
        (active_version->version_id == 0)
            ? std::cout << "Active Version is root.\n"
            : std::cout << "Parent of Active Version is v" << active_version->parent->version_id << "\n";
    }

    void print_versions() const {
        // prints all versions of file sorted by versionID
        for (int i = 0; i < total_versions; i++) {
            TreeNode *current_v = version_map.get(i);
            std::cout << "v" << i << (current_v->isSnapshot() ? " is a snapshot" : "") << (
                (i == active_version->version_id) ? " is active " : " ");
            (current_v->version_id == 0)
                ? std::cout << "is root.\n"
                : std::cout << "with parent v" << current_v->parent->version_id << "\n";
        }
    }
};

#endif
