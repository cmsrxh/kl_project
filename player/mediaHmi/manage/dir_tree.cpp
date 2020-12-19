#include "events/common_log.h"
#include "dir_tree.h"

DirTree::DirTree()
{    
    // 表示当前是一个根节点，其他节点的parent必然不为空
    mRoot.parent = 0;

    mRoot.children = new std::list<TreeNode>;
}

DirTree::~DirTree()
{
    clear();
    delete mRoot.children;
}

void DirTree::setRootName(const char *name)
{
    mRoot.name = name;
}

void DirTree::pushFile(const char *filePath)
{
    if (strncmp(filePath, mRoot.name.c_str(), mRoot.name.size()) == 0)
    {
        const char *p = filePath + mRoot.name.size();
        // 如果是斜杠就跳过它
        while (*p == '/') ++p;
        // 没有到末尾，就push进去
        if (*p != '\0')
        {
            _pushFile(p, &mRoot);
        }
    } else
    {
        GEN_Printf(LOG_WARN, "filepath: %s is invalid !", filePath);
    }
}

void DirTree::pushDirPath(const char *path, const char *name)
{
    if (strncmp(path, mRoot.name.c_str(), mRoot.name.size()) == 0)
    {
        TreeNode *node = &mRoot;
        const char *p  = path + mRoot.name.size();
        // 如果是斜杠就跳过它
        while (*p == '/') ++p;
        // 没有到末尾，就push进去
        if (*p != '\0')
        {
            node = genTreeByDirPath(p, node);
        }
        while (*name == '/') ++name;

        _pushFile(name, node);
    } else
    {
        GEN_Printf(LOG_WARN, "filepath: %s is invalid !", path);
    }
}

void DirTree::profile()
{
    _travel(&mRoot);
}

void DirTree::clear()
{
    for (std::list<TreeNode>::iterator it = mRoot.children->begin()
         ; it != mRoot.children->end(); ++it)
    {
        TreeNode *node = &(*it);

        _clear(node);
    }
    mRoot.children->clear();
}

std::string DirTree::genPathByNode(DirTree::TreeNode *node)
{
    if (!node) return "";
    std::list<TreeNode *> tmp;

    while(node)
    {
        tmp.push_back(node);
        node = node->parent;
    }

    tmp.reverse();
    std::string filePath;

    for (std::list<TreeNode *>::iterator it = tmp.begin()
         ; it != tmp.end(); ++it)
    {
        TreeNode *node = (*it);

        filePath.append(node->name);
        filePath.push_back('/');
    }

    filePath.resize(filePath.size()-1);

    return filePath;
}

void DirTree::_pushFile(const char *child, TreeNode *parent)
{
    //找到下一个‘/’斜杠标志，如果没有表示当前push的字串是文件，不是目录
    const char *p = strchr(child, '/');

    if (p) // is dir
    {
        std::string tmp(child, p - child);
        // GEN_Printf (LOG_DEBUG, "DIR: %s", tmp.c_str());

        // 如果是斜杠就跳过它
        while (*p == '/') ++p;
        // 没有到末尾，就push进去
        if (*p != '\0')
        {
            _pushFile(p, _incDir(tmp, parent));
        }
    } else // is file
    {
        std::string tmp(child);
        //GEN_Printf (LOG_DEBUG, "FILE: %s", tmp.c_str());
        _incFile(tmp, parent);
    }
}

DirTree::TreeNode * DirTree::_incDir(std::string &name, DirTree::TreeNode *parent)
{
    for (std::list<TreeNode >::iterator it = parent->children->begin()
         ; it != parent->children->end(); ++it)
    {
        TreeNode *node = &(*it);
        // 当前目录节点已经存在了，返回这个目录节点
        if (name == node->name)
        {
            return node;
        }
    }

    TreeNode node;

    node.name.swap(name);
    node.parent = parent;
    node.children = new std::list<TreeNode>;

    std::list<TreeNode>::iterator it = parent->children->insert(parent->children->end(), node);
    return &(*it);
}

void DirTree::_incFile(std::string &name, DirTree::TreeNode *parent)
{
    TreeNode node;

    node.name.swap(name);
    node.parent = parent;
    node.children = 0;

    parent->children->push_back(node);
}

void DirTree::_travel(DirTree::TreeNode *node)
{
    if (node->children)
    {
        for (std::list<TreeNode >::iterator it = node->children->begin()
             ; it != node->children->end(); ++it)
        {
            TreeNode *node = &(*it);
            _travel(node);
        }
    } else
    {
        GEN_Printf(LOG_DEBUG, "File: %s", node->name.c_str());
        GEN_Printf(LOG_DEBUG, "File: %s", genPathByNode(node).c_str());
        GEN_Printf(LOG_DEBUG, "File: %s", genPathByNode(node->parent).c_str());
    }
}

void DirTree::_clear(DirTree::TreeNode *node)
{
    if (node->children)
    {
        for (std::list<TreeNode>::iterator it = node->children->begin()
             ; it != node->children->end(); ++it)
        {
            TreeNode *node = &(*it);

            _clear(node);
        }
        delete node->children;
        node->children = 0;
    } else
    {
        // GEN_Printf(LOG_DEBUG, "DEL: %s", node->name.c_str());

    }
}

DirTree::TreeNode *DirTree::genTreeByDirPath(const char *path, TreeNode *parent)
{
    //找到下一个‘/’斜杠标志，如果没有表示当前push的字串是文件，不是目录
    const char *p = strchr(path, '/');

    if (p)
    {
        std::string tmp(path, p - path);

        // 如果是斜杠就跳过它
        while (*p == '/') ++p;
        // 没有到末尾，就push进去
        if (*p != '\0')
        {
            return genTreeByDirPath(p, _incDir(tmp, parent));
        } else
        {
            return _incDir(tmp, parent);
        }
    } else
    {
        std::string tmp(path);

        return _incDir(tmp, parent);
    }
}


bool DirTree::TreeNode::operator <(const DirTree::TreeNode &d)
{
    if (children) //all is dir
    {
        if (d.children)
        {
            return (0 < name.compare(d.name)) ? false : true;
        } else
        {
            return true;
        }
    } else
    {
        if (d.children)
        {
            return false;
        } else
        {
            return (0 < name.compare(d.name)) ? false : true;
        }
    }
}
