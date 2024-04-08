//
// Created by drew_ on 4/7/2024.
//

#ifndef GIT_MANAGER_H
#define GIT_MANAGER_H
#include <string>

namespace ScheduledGitCommit
{
    class GitManager
    {
    private:
        int run_command(const std::string& command, std::string& output);
    public:
        bool has_changes();
        void add_all();
        void commit(const std::string& message);
    };
} // ScheduledGitCommit

#endif //GIT_MANAGER_H
