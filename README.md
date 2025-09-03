
# COL106 Git File System

A simplified, in-memory version control system inspired by Git



## Features

- Implemented completely using C++
- Cross Platform
- Self implemented HashMap and Heap without std libraries


## Run Locally

Clone the project

```bash
  git clone https://link-to-project
```

Go to the project directory

```bash
  cd cgfs
```

On Linux

```bash
  chmod +x run.sh
  ./run.sh
```

On Windows

```bash
  .\run.bat
```

## Usage

```
<command> <filename> [parameters]
```


### Commands and Complexities
Let's define the following:
* `N`: Total number of files
* `V`: Total number of versions for a single file
* `D`: Depth of active_version node of a file
* `L`: Length of content

We are going to ignore complexity to take input/ print output.

**COMMANDS:**

* **CREATE `<filename>`** `O(N log(N))`  
  Creates a file with a root version (ID 0), empty content, and an initial snapshot message.

* **READ `<filename>`** `O(1)`  
  Displays the content of the file's currently active version.

* **INSERT `<filename>` `<content>`** `O(L + log(N))`  
  Appends content to the file.

    * If the active version is already a snapshot → creates a new version.
    * Otherwise → modifies the active version in place.

* **UPDATE `<filename>` `<content>`** `O(L + log(N))`  
  Replaces the file's content. Follows the same versioning logic as `INSERT`.

* **SNAPSHOT `<filename>` `<message>`** `O(1)`  
  Marks the active version as a snapshot, making its content immutable.
  Stores the provided message and the current time.

* **ROLLBACK `<filename>` `[versionID]`** `O(1)`  
  Sets the active version pointer to the specified `versionID`.

    * If no ID is provided → rolls back to the parent of the current active version.

* **HISTORY `<filename>`** `O(D)`  
  Lists all snapshotted versions of the file chronologically, which lie on the path from active node to the root in the file tree, showing:

    * Version ID
    * Timestamp
    * Message

* **RECENT FILES `[num]`** `O(num log(num))`  
  Lists files in descending order of their last modification time restricted to the first num entries. If no num is provided, it shows all files.

* **BIGGEST TREES `[num]`** `O(num log(num))`  
  Lists files in descending order of their total version count restricted to the first num entries. If no num is provided, it shows all files.

* **DETAILS `<filename>`** `O(1)`  
  Shows summary of file.

* **VERSIONS `<filename>`** `O(V)`  
  Shows details of all versions of file.

* **COMPARE `<filename>` `<versionID-1>` `[versionID-2]`** `O(N₁*N₂) Nᵢ is number of lines in Vᵢ`  
  Shows the diff between versions.

    * Default for `versionID-2` is the active version.

* **help**  
  Displays this message.

* **exit**  
  Quit.


### MultiLine Content and Message

To enter multiline content or message:

1. First type `<command> <filename>` and press **Enter**.
2. Then type your content/message.
3. To end MultiLine Input:

    * Press **Ctrl+Z + Enter** on **Windows**
    * Press **Ctrl+D** on **Linux**


### Notes

* Command case does not matter.

## Authors

- [@Seeker220](https://www.github.com/seeker220)

