#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "File.hpp"
#include "Heap.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <ctime>
#include <iostream>


class FileSystem {
private:
    HashMap<std::string, File *> files;
    IndexedHeap<std::string, time_t, greater<time_t> > recent_files;
    IndexedHeap<std::string, int, greater<int> > biggest_trees;

    void update_recent_files(const File *file) {
        // function to update recent_files heap given filename
        recent_files.update(file->name, file->last_modification_time);
    }

    void update_biggest_trees(const File *file) {
        // function to update biggest_trees heap given filename
        biggest_trees.update(file->name, file->total_versions);
    }

public:
    void create(const std::string &filename) {
        // creates new file with given name
        if (files.count(filename)) {
            throw std::invalid_argument("Duplicate Filename not allowed.");
        }
        File *newfile = new File(filename, "");
        files.insert(filename, newfile);
        recent_files.push(newfile->name, newfile->last_modification_time);
        biggest_trees.push(newfile->name, newfile->total_versions);
        std::cout << "File '" << filename << "' created.\n";
    }

    const std::string &read(const std::string &filename) const {
        // prints content of active version of file
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        return files.get(filename)->active_version->content;
    }

    void insert(const std::string &filename, const std::string &content) {
        // inserts into active version of file if not snapshot, else creates new version and inserts
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        if (files.get(filename)->active_version->isSnapshot()) {
            std::cout << "File '" << filename << "' current version v" << files.get(filename)->active_version->
                    version_id << " is a snapshot.\n";
            files.get(filename)->newversion(files.get(filename)->active_version->content + content,
                                            files.get(filename)->active_version, files.get(filename)->total_versions);
            update_biggest_trees(files.get(filename));
            std::cout << "New version v" << files.get(filename)->active_version->version_id <<
                    " created and content inserted into '" << filename << "' v" << files.get(filename)->active_version->
                    version_id << ".\n";
        } else {
            files.get(filename)->active_version->content.append(content);
            files.get(filename)->last_modification_time = time(nullptr);
            std::cout << "Content inserted into '" << filename << "' v" << files.get(filename)->active_version->
                    version_id << ".\n";
        }
        update_recent_files(files.get(filename));
    }

    void update(const std::string &filename, const std::string &content) {
        // updates active version of file if not snapshot, else creates new version and updates
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        if (files.get(filename)->active_version->isSnapshot()) {
            std::cout << "File '" << filename << "' current version v" << files.get(filename)->active_version->
                    version_id << " is a snapshot.\n";
            files.get(filename)->newversion(content, files.get(filename)->active_version,
                                            files.get(filename)->total_versions);
            update_biggest_trees(files.get(filename));
            std::cout << "New version v" << files.get(filename)->active_version->version_id <<
                    " created and content of '" << filename << "' v" << files.get(filename)->active_version->version_id
                    << " updated.\n";
        } else {
            files.get(filename)->active_version->content = content;
            files.get(filename)->last_modification_time = time(nullptr);
            std::cout << "Content of '" << filename << "' v" << files.get(filename)->active_version->version_id <<
                    " updated.\n";
        }
        update_recent_files(files.get(filename));
    }

    void snapshot(const std::string &filename, const std::string &message) const {
        // snapshots current version if not a snapshot
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        files.get(filename)->snapshot(message);
        std::cout << "Snapshot created for '" << filename << "' v" << files.get(filename)->active_version->version_id <<
                ".\n";
    }

    void rollback(const std::string &filename, int versionID) const {
        // changes active version of file to given version
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        if (!(files.get(filename)->version_map.count(versionID))) {
            throw std::out_of_range("File doesn't have given versionID.");
        }
        std::cout << "Rolled back '" << filename << "' from v" << files.get(filename)->active_version << " to v" <<
                versionID << ".\n";
        files.get(filename)->active_version = files.get(filename)->version_map.get(versionID);
    }

    void rollback(const std::string &filename) const {
        // changes active version of file to parent
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        File *file = files.get(filename);
        if (file->active_version->parent == nullptr) {
            throw std::invalid_argument("Cannot rollback from root version");
        }
        std::cout << "Rolled back '" << filename << "' from v" << files.get(filename)->active_version << " to parent v"
                << file->active_version->parent->version_id << ".\n";
        file->active_version = file->active_version->parent;
    }

    void history(const std::string &filename) const {
        // shows snapshot history sorted by timestamp
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        std::cout << "Snapshot History for File" << filename << "\n";
        for (TreeNode *&snap: files.get(filename)->snapshot_history) {
            std::cout << "VersionID : " << snap->version_id << " TimeStamp : " << snap->snapshot_timestamp <<
                    " Message : " << snap->message << "\n";
        }
    }

    void compare(const std::string &filename, int v1, int v2 = -1) const {
        // prints diff of 2 file versions
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        if (v2 == -1) {
            files.get(filename)->compare(v1, files.get(filename)->active_version->version_id);
        } else {
            files.get(filename)->compare(v1, v2);
        }
    }

    void print_recent_files() {
        // prints recent files sorted by last modified
        std::vector<Element<std::string, time_t> > tempv = {};
        while (!(recent_files.empty())) {
            Element<std::string, time_t> recent_file = recent_files.top();
            tempv.push_back({recent_file});
            recent_files.pop();
            std::cout << "Filename : " << recent_file.key << ", Last Modified at : " << timeToString(recent_file.value)
                    << "\n";
        }
        recent_files.build(tempv);
    }

    void print_biggest_trees() {
        // prints biggest trees sorted by total versions
        std::vector<Element<std::string, int> > tempv = {};
        while (!(biggest_trees.empty())) {
            Element<std::string, int> big_tree = biggest_trees.top();
            tempv.push_back({big_tree});
            biggest_trees.pop();
            std::cout << "Filename : " << big_tree.key << ", Total Version : " << big_tree.value << "\n";
        }
        biggest_trees.build(tempv);
    }

    void details(const std::string &filename) const {
        // prints details of file
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        files.get(filename)->print_details();
    }

    void versions(const std::string &filename) const {
        // prints all versions of file sorted by versionid
        if (!(files.count(filename))) {
            throw std::out_of_range("No file exists with given filename.");
        }
        files.get(filename)->print_versions();
    }
};

#endif
