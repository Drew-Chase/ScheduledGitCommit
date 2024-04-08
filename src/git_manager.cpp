//
// Created by drew_ on 4/7/2024.
//

#include "../includes/git_manager.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

namespace ScheduledGitCommit
{
    bool GitManager::has_changes() const
    {
        if (std::string output; run_command("git ", output) == 0)
        {
            std::cout << output << std::endl;
            return !output.empty();
        }

        return false;
    }

    void GitManager::add_all() const
    {
        std::string output;
        run_command("git add .", output);
        std::cout << output << std::endl;
    }

    void GitManager::commit() const
    {
        std::string output;
        run_command("git commit -m \"" + message + "\"", output);
        std::cout << output << std::endl;
    }

    void GitManager::create_tag() const
    {
        std::string output;
        run_command("git tag -a \"" + message + "\" -m \"" + message + "\"", output);
        std::cout << output << std::endl;
    }

    void GitManager::push() const
    {
        std::string output;
        run_command("git push --force", output);
        std::cout << output << std::endl;
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

    std::string GitManager::parse_message() const
    {
        std::string msg = message;
        const auto now = std::chrono::system_clock::now();
        const auto time = std::chrono::system_clock::to_time_t(now);
        const auto [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst] = *std::localtime(&time);
        const std::string time_str = std::to_string(tm_hour) + ":" + std::to_string(tm_min) + ":" + std::to_string(tm_sec);
        const std::string date_str = std::to_string(tm_year + 1900) + "-" + std::to_string(tm_mon + 1) + "-" + std::to_string(tm_mday);
        const std::string datetime_str = date_str + " " + time_str;


        if (const auto pos = msg.find("%t"); pos != std::string::npos)
        {
            msg.replace(pos, 2, datetime_str);
        }
        if (const auto pos = msg.find("%d"); pos != std::string::npos)
        {
            msg.replace(pos, 2, date_str);
        }

        return msg;
    }

    void GitManager::run() const
    {
        // create a new thread
        std::thread([this]()
        {
            const std::chrono::milliseconds duration(this->time);
            while (true)
            {
                std::this_thread::sleep_for(duration);
                try
                {
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
                } catch (std::exception &e)
                {
                    std::cerr << "An error occurred: " << e.what() << std::endl;
                    break;
                }
            }
        });
    }
} // ScheduledGitCommit
