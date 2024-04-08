#pragma once

#ifndef CCOMMANDLINEPARSER_OPTIONS_MANAGER_H
#define CCOMMANDLINEPARSER_OPTIONS_MANAGER_H

#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
#include <sec_api/string_s.h>

using namespace std;

namespace cclip
{
    struct option
    {
        /**
         * The short name of the option (e.g. -h) minus the dash (-).
         */
        const char *short_name;
        /**
         * The long name of the option (e.g. --help) minus the dashes (--).
         */
        const char *long_name;
        /**
         * The description of the option.
         */
        const char *description;
        /**
         * Whether or not the option is required.
         */
        bool is_required;
        /**
         * Whether or not the option has an argument.
         */
        bool has_argument;
        /**
         * The argument of the option.
         */
        char *argument;
    };

    class options_manager
    {
    private:
        const char *context;
        vector<option *> options;
        vector<option *> present_options;

        option *get_option_from_global_list(const std::string &name);

    public:
        /**
         * Create a new options manager.
         * @param context the application context (e.g. "myapp" or "My App")
         */
        explicit options_manager(const char *context);

        ~options_manager() = default;

        /**
         * Add an option to the options manager.
         * @param short_name the short name of the option (e.g. -h) minus the dash (-)
         * @param long_name the long name of the option (e.g. --help) minus the dashes (--)
         * @param description the description of the option
         * @param is_required whether or not the option is required
         * @param has_argument whether or not the option has an argument
         * @return the option that was added
         */
        option *add_option(const char *short_name, const char *long_name, const char *description, bool is_required, bool has_argument);

        /**
         * Parse the command line arguments.
         * @param argc the number of arguments
         * @param argv the arguments
         */
        void parse(int argc, char **argv);

        /**
         * Print the help message to stdout.
         */
        void print_help() const;

        /**
         * Get the help message. This is useful if you want to print the help message to a file or something.<br>
         * <b style="color: #f81c1c;"><i>Make sure to delete the returned string when you're done with it.</i></b>
         * @return the help message
         */
        [[nodiscard]] const char *get_help() const;

        /**
         * Check if the option is present in the arguments.
         * @param name the short or long name of the option
         * @return whether or not the option is present
         */
        [[nodiscard]] bool is_present(const std::string &name) const;

        /**
         * Get the option by name.
         * @param name the short or long name of the option
         * @return the option
         */
        option *get_option(const std::string &name);
    };

    inline options_manager::options_manager(const char *context)
    {
        this->context = context;
    }

    inline option *options_manager::add_option(const char *short_name, const char *long_name, const char *description, const bool is_required, const bool has_argument)
    {
        auto *opt = new option{short_name, long_name, description, is_required, has_argument};
        this->options.push_back(opt);
        return opt;
    }

    inline void options_manager::parse(const int argc, char **argv)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (const char *arg = argv[i]; arg[0] == '-')
            {
                if (arg[1] == '-')
                {
                    // long option
                    const char *name = arg + 2;
                    if (option *opt = this->get_option_from_global_list(name); opt == nullptr)
                    {
                        std::cerr << "Unknown option: --" << name << std::endl;
                        this->print_help();
                        exit(1);
                    } else
                    {
                        if (opt->has_argument)
                        {
                            if (i + 1 >= argc)
                            {
                                std::cerr << "Missing argument for option: " << name << std::endl;
                                this->print_help();
                                exit(1);
                            }
                            opt->argument = argv[i + 1];
                        }
                        this->present_options.push_back(opt);
                    }
                } else
                {
                    // short option
                    const char *name = arg + 1;
                    if (option *opt = this->get_option_from_global_list(name); opt == nullptr)
                    {
                        std::cerr << "Unknown option: -" << name << std::endl;
                        this->print_help();
                        exit(1);
                    } else
                    {
                        if (opt->has_argument)
                        {
                            if (i + 1 >= argc)
                            {
                                std::cerr << "Missing argument for option: " << name << std::endl;
                                this->print_help();
                                exit(1);
                            }
                            opt->argument = argv[i + 1];
                        }
                        this->present_options.push_back(opt);
                    }
                }
            }
        }
        bool missing = false;
        for (const auto &option: this->options)
        {
            if (option->is_required && !this->is_present(option->short_name))
            {
                std::cerr << "Missing required option: -" << option->short_name << " or --" << option->long_name << std::endl;
                missing = true;
            }
        }
        if (missing)
        {
            //            std::cerr << std::endl;
            this->print_help();
            exit(1);
        }
    }

    inline void options_manager::print_help() const
    {
        const char *help = this->get_help();
        std::cout << help << std::endl;
        delete[] help; // Deletes the memory allocated by get_help()
    }

    inline const char *options_manager::get_help() const
    {
        std::stringstream buf;
        buf << this->context << " Help:\n";

        for (auto &option: this->options)
        {
            buf << " -" << option->short_name << ", --" << option->long_name;
            if (option->has_argument)
            {
                buf << " <arg>";
            }
            if (option->is_required)
            {
                buf << " (required)";
            }


            buf << "\n\t" << option->description << "\n";
        }


        const size_t length = buf.str().length() + 1;
        const auto help_str = new char[length];
        strcpy_s(help_str, length, buf.str().c_str());

        return help_str;
    }

    inline bool options_manager::is_present(const std::string &name) const
    {
        return std::any_of(this->present_options.begin(), this->present_options.end(), [&name](const option *opt)
        {
            return opt->short_name == name || opt->long_name == name;
        });
    }

    inline option *options_manager::get_option(const std::string &name)
    {
        for (const auto &option: this->present_options)
        {
            if (option->short_name == name || option->long_name == name)
            {
                return option;
            }
        }
        return nullptr;
    }

    inline option *options_manager::get_option_from_global_list(const std::string &name)
    {
        for (const auto &option: this->options)
        {
            if (option->short_name == name || option->long_name == name)
            {
                return option;
            }
        }
        return nullptr;
    }
} // cclip
#endif //CCOMMANDLINEPARSER_OPTIONS_MANAGER_H
