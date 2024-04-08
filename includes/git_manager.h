//
// Created by drew_ on 4/7/2024.
//

#ifndef GIT_MANAGER_H
#define GIT_MANAGER_H
#include <string>
#include <utility>

namespace ScheduledGitCommit
{
    class GitManager
    {
        /**
         * @brief Checks if there are any changes in the Git repository.
         *
         * This method checks if there are any changes in the Git repository by running the "git status" command and checking the output. If the output is empty, it means there are no changes.
         *
         * @return True if there are changes in the Git repository, false otherwise.
         */
        [[nodiscard]] bool has_changes() const;

        /**
         * @brief Adds all changes to the Git repository.
         *
         * This method adds all changes in the Git repository by executing the "git add ." command.
         *
         * @return None.
         */
        void add_all() const;

        /**
         * @brief Commit the changes to the Git repository.
         *
         * This method commits the changes to the Git repository using the specified commit message.
         *
         * @return None.
         */
        void commit() const;

        /**
         * @brief Creates a tag for the current commit.
         *
         * This method creates a tag with the commit message using the `git tag` command.
         *
         * @return None.
         */
        void create_tag() const;

        /**
         * @brief Pushes the commit and changes to the remote repository.
         *
         * This method pushes the commit and changes to the remote repository specified in the GitManager object.
         *
         * @return None.
         */
        void push() const;

        /**
         * @brief Execute a command and capture the output.
         *
         * This method executes the given command in the specified directory and captures the output.
         *
         * @param command The command to execute.
         * @param output The variable to store the output of the command.
         *
         * @return The exit code of the command execution. Returns 0 if the command executed successfully and -1 if there was an error.
         */
        int run_command(const std::string &command, std::string &output) const;

        /**
         * @brief Parses and modifies the message string.
         *
         * This method parses the message and replaces placeholders with dynamic values. The placeholders are:
         *   - "%t": Replaced with the current date and time in the format "YYYY-MM-DD HH:MM:SS".
         *   - "%d": Replaced with the current date in the format "YYYY-MM-DD".
         *
         * @return The modified message string.
         */
        [[nodiscard]] std::string parse_message() const;

        /**
         * @brief The directory path where the Git repository is located.
         *
         * This variable stores the directory path where the Git repository is located. It is used by the `GitManager` class to execute Git commands in the specified directory.
         */
        std::string directory;
        /**
         * @brief The unix timestamp representing the time of the commit.
         */
        unsigned long long time;
        /**
         * @brief The commit message.
         *
         * This variable stores the commit message that will be used when committing changes to the Git repository.
         */
        std::string message;
        /**
         * @brief Indicates whether a tag should be created for the current commit.
         *
         * This variable is used to determine whether a tag should be created for the current commit. If should_create_tag is set to true, a tag will be created after committing the changes; otherwise, no tag will be created.
         */
        bool should_create_tag = false;
        /**
         * @brief Indicates whether the changes should be pushed to the remote repository.
         *
         * This variable determines whether the changes made in the Git repository should be pushed to the remote repository. If the value is set to true, the changes will be pushed; otherwise, they will not be pushed.
         */
        bool should_push = false;

    public:
        /**
         * @brief GitManager constructor.
         *
         * This constructor initializes a GitManager object with the specified time, message, and directory.
         *
         * @param time The unix timestamp representing the time of the commit.
         * @param message The commit message.
         * @param directory The directory path where the Git repository is located.
         * @param create_tag Whether or not to create a tag with the commit message.
         * @param push Whether or not to push the commit to the remote repository.
         *
         * @return None.
         */
        GitManager(const unsigned long long time, std::string message, std::string directory, const bool create_tag, const bool push): directory(std::move(directory)), time(time), message(std::move(message)), should_create_tag(create_tag), should_push(push) {}

        /**
         * @brief Executes the GitManager functionality.
         *
         * This method is responsible for running the GitManager functionality in a new thread. It creates a new thread that sleeps for the specified duration and then checks if there are any changes in the Git repository using the `has_changes()` method. If there are changes, it performs the following actions:
         * - Calls the `add_all()` method to add all changed files to the Git repository.
         * - Calls the `commit()` method to commit the changes.
         * - If the `should_create_tag` flag is set to `true`, calls the `create_tag()` method to create a new tag.
         * - If the `should_push` flag is set to `true`, calls the `push()` method to push the changes to the remote repository.
         *
         * @note This method runs asynchronously in a new thread.
         */
        void run() const;
    };
} // ScheduledGitCommit

#endif //GIT_MANAGER_H
