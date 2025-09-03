#include "FileSystem.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>

std::string rawinput() {
    // function to take multi-line input
    // Use Ctrl+G (ASCII 7) to terminate input instead of EOF
    std::string rawstring;
    char ch;
    bool newLine = true;

    while (std::cin.get(ch)) {
        if (ch == '\x07') {
            // Ctrl+G (ASCII 7 - BEL character)
            // Consume any immediately following newline to prevent it from
            // being interpreted as an empty command in the main loop
            if (std::cin.peek() == '\n') {
                std::cin.get(); // consume the newline
            }
            break;
        }
        if (ch == '\n') {
            rawstring += ch;
            newLine = true;
        } else {
            rawstring += ch;
            newLine = false;
        }
    }

    // Remove trailing newline if present to match original behavior
    if (!rawstring.empty() && rawstring.back() == '\n') {
        rawstring.pop_back();
    }

    return rawstring;
}

void toLower(std::string &s) {
    // function to convert command to lowercase
    for (char &c: s) {
        c = tolower(c);
    }
}

int main() {
    FileSystem fs;
    std::string line;
    std::cout << "Welcome to COL106 Git v1.0.0\n";
    std::cout << "Enter 'exit' to quit.\n";
    std::cout << "Enter 'help' to display help.\n";

    while (true) {
        // loops until exit
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            std::cout << "Exiting COL106 Git v1.0.0. Bye!\n";
            break;
        }
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        toLower(command);

        try {
            // tries to handle command
            if (command == "exit") {
                std::cout << "Exiting COL106 Git v1.0.0. Bye!\n";
                break;
            } else if (command == "help") {
                std::string help = R"(usage: <command> <filename> [parameters]
    commands:
        CREATE <filename>                               : Creates a file with a root version (ID 0), empty content,
                                                          and an initial snapshot message.
        READ <filename>                                 : Displays the content of the file's currently active version.
        INSERT <filename> <content>                     : Appends content to the file. This creates a new version if
                                                          the active version is already a snapshot; otherwise, it
                                                          modifies the active version in place.
        UPDATE <filename> <content>                     : Replaces the file's content. Follows the same versioning
                                                          logic as INSERT.
        SNAPSHOT <filename> <message>                   : Marks the active version as a snapshot, making its content
                                                          immutable. It stores the provided message and the current time.
        ROLLBACK <filename> [versionID]                 : Sets the active version pointer to the specified versionID.
                                                          If no ID is provided, it rolls back to the parent of the current
                                                          active version.
        HISTORY <filename>                              : Lists all snapshotted versions of the file chronologically,
                                                          which lie on the path from active node to the root in the file
                                                          tree, showing their ID, timestamp, and message.
        RECENT FILES [num]                              : Lists files in descending order of their last modification time
                                                          restricted to the first num entries. If no num is provided, it
                                                          shows all files.
        BIGGEST TREES [num]                             : Lists files in descending order of their total version count
                                                          restricted to the first num entries. If no num is provided, it
                                                          shows all files.
        DETAILS <filename>                              : Shows summary of file.
        VERSIONS <filename>                             : Shows details of all versions of file.
        COMPARE <filename> <versionID-1> [versionID-2]  : Shows the diff versionID-1 -> versionID-2. If not provided,
                                                          default versionID-2 is active-version.
        help                                            : Displays this message.
        exit                                            : Quit.
    MultiLine Content and Message:
        First type <command> <filename> and press enter to begin MultiLine Content or Message. Press 'Ctrl+G' to end MultiLine Input.
    Note: Command case does not matter.
)";
                std::cout << help;
            } else if (command == "create") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("CREATE requires a filename.");
                }
                fs.create(filename);
            } else if (command == "read") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("READ requires a filename.");
                }
                std::cout << fs.read(filename) << "\n";
            } else if (command == "insert") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("INSERT requires a filename.");
                }
                std::cout << "Enter content (end with 'Ctrl+G'):\n";
                std::string content = rawinput();
                fs.insert(filename, content);
            } else if (command == "update") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("UPDATE requires a filename.");
                }
                std::cout << "Enter content (end with 'Ctrl+G'):\n";
                std::string content = rawinput();
                fs.update(filename, content);
            } else if (command == "snapshot") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("SNAPSHOT requires a filename.");
                }
                std::cout << "Enter snapshot message (end with 'Ctrl+G'):\n";
                std::string message = rawinput();
                fs.snapshot(filename, message);
            } else if (command == "rollback") {
                std::string filename;
                int versionID;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("ROLLBACK requires a filename.");
                }
                if (ss >> versionID) {
                    fs.rollback(filename, versionID);
                } else {
                    fs.rollback(filename);
                }
            } else if (command == "history") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("HISTORY requires a filename.");
                }
                fs.history(filename);
            } else if (command == "details") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("DETAILS requires a filename.");
                }
                fs.details(filename);
            } else if (command == "versions") {
                std::string filename;
                if (!(ss >> filename)) {
                    throw std::invalid_argument("VERSIONS requires a filename.");
                }
                fs.versions(filename);
            } else if (command == "compare") {
                std::string filename;
                int v1, v2;
                if (!(ss >> filename >> v1)) {
                    throw std::invalid_argument("COMPARE requires a filename and at least one VersionID.");
                }
                if (ss >> v2) {
                    fs.compare(filename, v1, v2);
                } else {
                    fs.compare(filename, v1);
                }
            } else if (command == "recentfiles" || command == "recent" || command == "recent_files") {
                int num;
                if (ss >> num) {
                    fs.print_recent_files(num);
                } else {
                    fs.print_recent_files();
                }
            } else if (command == "biggesttrees" || command == "biggest" || command == "biggest_trees") {
                int num;
                if (ss >> num) {
                    fs.print_biggest_trees(num);
                } else {
                    fs.print_biggest_trees();
                }
            } else if (command.empty()) {
                std::cout << "Please enter a command.\n";
            } else {
                std::cerr << "Error: Unknown Command '" << command << "'.\n";
            }
        } catch (const std::exception &e) {
            // else shows exception and continues loop
            std::cerr << "Operation failed: " << e.what() << "\n";
        }
    }
    return 0;
}
