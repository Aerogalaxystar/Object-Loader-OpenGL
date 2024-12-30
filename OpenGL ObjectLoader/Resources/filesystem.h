#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <cstdlib>

class FileSystem
{
private:
    static const char* const logl_root; // Declaration of logl_root constant

    typedef std::string(*Builder)(const std::string&);

public:
    static std::string getPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const&) = getPathBuilder();
        return (*pathBuilder)(path);
    }

private:
    static std::string const& getRoot()
    {
        static char* envRoot;
        size_t len;
        errno_t err = _dupenv_s(&envRoot, &len, "LOGL_ROOT_PATH");
        static char const* givenRoot = (err == 0 && envRoot != nullptr ? envRoot : logl_root);
        static std::string root = (givenRoot != nullptr ? givenRoot : "");
        free(static_cast<void*>(envRoot)); // Cast envRoot to void* before passing to free
        return root;
    }

    static Builder getPathBuilder()
    {
        if (!getRoot().empty())
            return &FileSystem::getPathRelativeRoot;
        else
            return &FileSystem::getPathRelativeBinary;
    }

    static std::string getPathRelativeRoot(const std::string& path)
    {
        return getRoot() + std::string("/") + path;
    }

    static std::string getPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }
};

// Definition of logl_root constant
const char* const FileSystem::logl_root = "${CMAKE_SOURCE_DIR}";

#endif // FILESYSTEM_H

