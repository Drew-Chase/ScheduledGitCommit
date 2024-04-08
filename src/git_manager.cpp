//
// Created by drew_ on 4/7/2024.
//

#include "../includes/git_manager.h"

#include <memory>
#include <stdexcept>
#include <thread>

namespace ScheduledGitCommit
{
    bool GitManager::has_changes() const
    {
        if (std::string output; run_command("git ", output) == 0)
        {
            return !output.empty();
        }

        return false;
    }

    void GitManager::add_all() const
    {
        std::string output;
        run_command("git add .", output);
    }

    void GitManager::commit() const
    {
        std::string output;
        run_command("git commit -m \"" + message + "\"", output);
    }

    void GitManager::create_tag() const
    {
        std::string output;
        run_command("git tag -a \"" + message + "\" -m \"" + message + "\"", output);
    }

    void GitManager::push() const
    {
        std::string output;
        run_command("git push --force", output);
    }

    int GitManager::run_command(const std::string &command, std::string &output) const
    {
        char buffer[128];
        std::string result;

#if defined(_WIN32)
        std::string full_command = "cd /D \"" + directory + "\" && " + command;
#else
        std::string full_command = "cd \"" + directory + "\" && " + command;
#endif

        const std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(full_command.c_str(), "r"), _pclose);
        if (!pipe)
        {
            return -1;
        }
        while (fgets(buffer, sizeof buffer, pipe.get()) != nullptr)
        {
            result += buffer;
        }
        output = result;
        // get the return code
        return pipe != nullptr ? 0 : -1;
    }

    void GitManager::run() const
    {
        // create a new thread
        std::thread([this]()
        {
            const std::chrono::milliseconds duration(this->time);
            std::this_thread::sleep_for(duration);
            if (has_changes())
            {
                add_all();
                commit();
                if (this->should_create_tag)
                {
                    create_tag();
                }
                if (should_push)
                {
                    push();
                }
            }
        });
    }
} // ScheduledGitCommit
