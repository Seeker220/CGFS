
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


### Commands

* **CREATE `<filename>`**
  Creates a file with a root version (ID 0), empty content, and an initial snapshot message.

* **READ `<filename>`**
  Displays the content of the file's currently active version.

* **INSERT `<filename>` `<content>`**
  Appends content to the file.

    * If the active version is already a snapshot → creates a new version.
    * Otherwise → modifies the active version in place.

* **UPDATE `<filename>` `<content>`**
  Replaces the file's content. Follows the same versioning logic as `INSERT`.

* **SNAPSHOT `<filename>` `<message>`**
  Marks the active version as a snapshot, making its content immutable.
  Stores the provided message and the current time.

* **ROLLBACK `<filename>` `[versionID]`**
  Sets the active version pointer to the specified `versionID`.

    * If no ID is provided → rolls back to the parent of the current active version.

* **HISTORY `<filename>`**
  Lists all snapshotted versions of the file chronologically, showing:

    * Version ID
    * Timestamp
    * Message

* **RECENT FILES**
  Lists files in descending order of their last modification time.

* **BIGGEST TREES**
  Lists files in descending order of their total version count.

* **DETAILS `<filename>`**
  Shows summary of file.

* **VERSIONS `<filename>`**
  Shows details of all versions of file.

* **COMPARE `<filename>` `<versionID-1>` `[versionID-2]`**
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

