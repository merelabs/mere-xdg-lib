#ifndef AUTOSTARTDIRECTORYTRAVERSER_H
#define AUTOSTARTDIRECTORYTRAVERSER_H

#include "desktopentry.h"
#include <QObject>

namespace Mere
{
namespace XDG
{

class AutostartDirectoryTraverser : public QObject
{
    Q_OBJECT
public:
    explicit AutostartDirectoryTraverser(QObject *parent = nullptr);
    std::vector<Mere::XDG::DesktopEntry> traverse() const;
    std::vector<Mere::XDG::DesktopEntry> traverse(DesktopEntry::TypeId type) const;

    std::vector<Mere::XDG::DesktopEntry> traverse(const std::string &path) const;
    std::vector<Mere::XDG::DesktopEntry> traverse(const std::string &path, DesktopEntry::TypeId type) const;

private:
    std::vector<Mere::XDG::DesktopEntry> m_entries;

};

}
}
#endif // AUTOSTARTDIRECTORYTRAVERSER_H
