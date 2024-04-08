#include <iostream>
#include <string>
#include <filesystem>

#include "includes/git_manager.h"
#include "includes/options_manager.hpp"

auto main(const int argc, char **argv) -> int
{
    auto manager = new cclip::options_manager("Scheduled Git Commit");
    manager->add_option("h", "help", "Displays this help message.", false, false);
    manager->add_option("t", "time", "The time to schedule the commit, time is specified by a ms,s,m,h or d for milliseconds, seconds, minutes, hours or days. If no time specifier is used, the default is ms. (Default: 1h)", false, true);
    manager->add_option("m", "message", "The commit message template, check the docs for more information about message formatting. (Default: Scheduled commit at %t)", false, true);
    manager->add_option("d", "directory", "The directory to commit, if not specified, the current directory is used. (Default: .)", false, true);
    manager->add_option("ct", "tag", "If included, the commit will also create a tag using message template.", false, false);
    manager->add_option("p", "push", "If included, the commit will also push to the remote repository.", false, false);

    unsigned long long time = 3600000; // 1 hour
    std::string message = "Scheduled commit at %t";
    std::string directory = ".";

    manager->parse(argc, argv);
    if (manager->is_present("h"))
    {
        manager->print_help();
        return 0;
    }

    if (manager->is_present("t"))
    {
        if (const std::string time_str = manager->get_option("t")->argument; time_str.back() == 's')
        {
            time = std::stoull(time_str.substr(0, time_str.size() - 1)) * 1000; // Convert seconds to milliseconds
        } else if (time_str.back() == 'm')
        {
            time = std::stoull(time_str.substr(0, time_str.size() - 1)) * 60000; // Convert minutes to milliseconds
        } else if (time_str.back() == 'h')
        {
            time = std::stoull(time_str.substr(0, time_str.size() - 1)) * 3600000; // Convert hours to milliseconds
        } else if (time_str.back() == 'd')
        {
            time = std::stoull(time_str.substr(0, time_str.size() - 1)) * 86400000; // Convert days to milliseconds
        } else
        {
            time = std::stoull(time_str); // Default to milliseconds
        }
    }

    if (manager->is_present("m"))
    {
        message = manager->get_option("m")->argument;
    }

    if (manager->is_present("d"))
    {
        directory = manager->get_option("d")->argument;
    }

    const bool create_tag = manager->is_present("ct");
    const bool push = manager->is_present("p");

    delete manager; // Free memory for the options manager

    directory = std::filesystem::absolute(directory).string(); // Converts directory to absolute path

    ScheduledGitCommit::GitManager git(time, message, directory, create_tag, push);
    git.run();
}
