# Scheduled Git Commit

This is a simple command-line tool that allows you to create git commits on a timer.

# Setup

To use this tool, you will need the git cli installed on your system.
You can install it by following the instructions on the [git website](https://git-scm.com/downloads).
or install it using your favorite package manager.

## Create a new repository

To create a new repository, run the following commands in your terminal:

```bash
git init
```

## Set the remote repository (optional)

If you want to push your commits to a remote repository, you can set the remote repository by running the following command:

```bash
git remote add origin <remote-repository-url>
```

## Create a initial commit

To create an initial commit, run the following commands in your terminal:

```bash
git add .
git commit -m "Initial commit"
git push -u origin master
```

## Run the tool

To run the tool, run the following command in your terminal:

```bash
./scheduled-git-commit
```

# Usage

The tool will prompt you to enter the commit message and the time interval between commits.
You can enter the commit message and the time interval in seconds.
The tool will create a new commit with the specified message every time interval.
To stop the tool, press `Ctrl + C`.

| Short (-) | Long (--)       | Description                                                                                                                                                                                          |
|-----------|-----------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| h         | help            | Displays this help message.                                                                                                                                                                          |
| t         | time <arg>      | The time to schedule the commit, time is specified in milliseconds (`ms`), seconds (`s`), minutes (`m`), hours (`h`) or days (`d`). If no time specifier is used, the default is `ms`. Default: `1h` |
| m         | message <arg>   | The commit message template, check the docs for more information about message formatting. Default: `Scheduled commit at %t`                                                                         |
| d         | directory <arg> | The directory to commit, if not specified, the current directory is used. Default: `.`                                                                                                               |
| c         | tag             | If included, the commit will also create a tag using message template.                                                                                                                               |
| p         | push            | If included, the commit will also push to the remote repository.                                                                                                                                     |

## Message Formatting

| Placeholder | Description                                          |
|-------------|------------------------------------------------------|
| %t          | The current time in the format `YYYY-MM-DD HH:MM:SS` |
| %d          | The current date in the format `YYYY-MM-DD`          |

## Example

```bash
./scheduled-git-commit --time 1m --message "Commit %t" --directory /path/to/directory --tag --push
```