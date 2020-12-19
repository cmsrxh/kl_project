#ifndef DIR_TREE_H
#define DIR_TREE_H

#include <list>
#include <string>

class DirTree
{
public:
    class TreeNode
    {
    public:
        TreeNode()
            : parent(0), children(0)
        {}

        bool operator < (const TreeNode& d);

        //! 指向父节点
        TreeNode *parent;

        //! 如果指针为空，表示这个节点是一个文件节点(即叶子节点)，不空表示当前是一个目录(枝干节点)
        std::list<TreeNode> *children;

        //! 目录或者文件名字
        std::string name;
    };

    DirTree();
    ~DirTree();

    void setRootName(const char *name);

    //! 压栈一个文件路径+名字，路径必须是包含根节点的路径，以便拆分
    void pushFile(const char *filePath);

    //! 路径和文件名，分开插入
    void pushDirPath(const char *path, const char *name);

    //! 遍历所有的文件节点
    void profile();

    //! 清空所有的数据
    void clear();

    //! 根据当前节点，生成完整的绝对路径
    std::string genPathByNode(TreeNode *node);

    TreeNode *getRootNode()
    {
        return &mRoot;
    }

    bool isRoot(TreeNode *node)
    {
        return node == &mRoot;
    }
private:
    //! 嵌套方式输入节点
    void _pushFile(const char *child, TreeNode *parent);

    //! 增加一个目录节点
    TreeNode *_incDir(std::string &name, TreeNode *parent);
    //! 增加一个文件节点
    void _incFile(std::string &name, TreeNode *parent);

    //! 嵌套方式打印所有的信息
    void _travel(TreeNode *node);

    //! 嵌套方式清空所有的数据, 需要首先清理，叶子节点，然后才能是，枝干节点
    void _clear(TreeNode *node);

    //! 根据输入的目录，生成对应node
    TreeNode *genTreeByDirPath(const char *path, TreeNode *parent);

    TreeNode mRoot;
};

#endif // DIR_TREE_H
