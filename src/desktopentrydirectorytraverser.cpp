#include "desktopentrydirectorytraverser.h"
#include "desktopentrydirectory.h"
#include "desktopentryparser.h"

#include <sys/types.h>
#include <dirent.h>

#include <QDir>
Mere::XDG::DesktopEntryDirectoryTraverser::DesktopEntryDirectoryTraverser(QObject *parent)
    : QObject(parent)
{

}

std::vector<Mere::XDG::DesktopEntry> Mere::XDG::DesktopEntryDirectoryTraverser::traverse() const
{
    std::vector<Mere::XDG::DesktopEntry> entries;

    std::vector<std::string> directoris = Mere::XDG::DesktopEntryDirectory::directories();
    for(const std::string &directory : directoris)
    {
        std::vector<Mere::XDG::DesktopEntry> _entries = this->traverse(directory);
        if (_entries.empty()) continue;

        entries.insert(entries.end(), std::make_move_iterator(_entries.begin()), std::make_move_iterator(_entries.end()));
    }

    return entries;
}

std::vector<Mere::XDG::DesktopEntry> Mere::XDG::DesktopEntryDirectoryTraverser::traverse(DesktopEntry::Type type) const
{
    std::vector<Mere::XDG::DesktopEntry> entries;

    std::vector<std::string> directoris = Mere::XDG::DesktopEntryDirectory::directories();
    for(const std::string &directory : directoris)
    {
        std::vector<Mere::XDG::DesktopEntry> _entries = this->traverse(directory, type);
        if (_entries.empty()) continue;

        entries.insert(entries.end(), std::make_move_iterator(_entries.begin()), std::make_move_iterator(_entries.end()));
    }

    return entries;
}

std::vector<Mere::XDG::DesktopEntry> Mere::XDG::DesktopEntryDirectoryTraverser::traverse(const std::string &path) const
{
    std::vector<Mere::XDG::DesktopEntry> entries;

    std::string base(path);
    if (base.back() != '/')
        base.append("/");

    std::vector<std::string> files = this->files(path);
    for(const std::string &file : files)
    {
        std::string p(base);
        DesktopEntryParser parser(p.append(file));
        if(!parser.parse()) continue;

        DesktopEntry entry = parser.entry();
        if(!entry.valid()) continue;

        entries.push_back(std::move(entry));
    }

    emit traversed(path);

    return entries;
}

std::vector<Mere::XDG::DesktopEntry> Mere::XDG::DesktopEntryDirectoryTraverser::traverse(const std::string &path, DesktopEntry::Type type) const
{
    std::vector<Mere::XDG::DesktopEntry> entries;

    std::string base(path);
    if (base.back() != '/')
        base.append("/");

    std::vector<std::string> files = this->files(path);
    for(const std::string &file : files)
    {
        std::string p(base);
        DesktopEntryParser parser(p.append(file));
        if(!parser.parse()) continue;

        DesktopEntry entry = parser.entry();
        if(!entry.valid()) continue;
        if (entry.typeId() != type) continue;

        entries.push_back(std::move(entry));
    }

    //emit traversed(path);

    return entries;
}
std::map<std::string, std::vector<std::string>> Mere::XDG::DesktopEntryDirectoryTraverser::files() const
{
    std::map<std::string, std::vector<std::string>> files;

    std::vector<std::string> directoris = Mere::XDG::DesktopEntryDirectory::directories();
    for(const std::string &directory : directoris)
    {
        std::vector<std::string> _files = this->files(directory);
        if (_files.empty()) continue;

        files.insert({directory, std::move(_files)});
    }

    return files;
}

std::vector<std::string> Mere::XDG::DesktopEntryDirectoryTraverser::files(const std::string &path) const
{
    std::vector<std::string> files;

    if (path.empty()) return files;

    if (DIR *dir = opendir(path.c_str()))
    {
        const char *extension = ".desktop";

        struct dirent *dp;
        while ((dp = readdir(dir)))
        {
            //if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0 )
            //    continue;

            if (strlen(dp->d_name) <= strlen(extension))
                continue;

            if(strcmp(&dp->d_name[strlen(dp->d_name) - strlen(extension)], extension))
                continue;

            files.push_back(dp->d_name);
        }
        closedir(dir);
    }

    return files;
}
