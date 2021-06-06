
#pragma once
#include <stdio.h>
#include <memory>
#include <queue>
using namespace std;

namespace ariel
{

    template <typename T>
    class BinaryTree
    {

    public:
        BinaryTree()
        {
            root = nullptr;
        }
        ~BinaryTree()
        {
            delete_Tree();
        }
        BinaryTree(const BinaryTree &otherTree)
        {
            delete_Tree();
            copy_Tree(otherTree);
        }

        BinaryTree(BinaryTree &&otherTree) noexcept
        {
            root = otherTree.root;
            otherTree.root = nullptr;
        }

        BinaryTree &operator=( BinaryTree otherTree)
        {
            if (this != &otherTree)
            {
                delete_Tree();
                copy_Tree(otherTree);
            } 
            return *this;
        }

        BinaryTree &operator=(BinaryTree &&otherTree) noexcept
        {
            root = otherTree.root;
            otherTree.root = nullptr;
            return *this;
        }

        BinaryTree<T>& add_root(T val)
        {
            if (root == nullptr)
            {
                root = new Node(val);
            }
            else
            {
                root->value = val;
            }
            return *this;
        }

        BinaryTree<T>& add_left(T parent, T left_child)
        {
            Node *add_new_child = find_parent(parent);
            if (add_new_child == nullptr)
            {
                delete_Tree();
                throw std::invalid_argument("Parent not found");
            }
            if (add_new_child->left == nullptr)
            {
                add_new_child->left = new Node(left_child);
            }
            else
            {
                (add_new_child->left)->value = left_child;
            }
            return *this;
        }

        BinaryTree<T>& add_right(T parent, const T right_child)
        {
            Node *add_new_child = find_parent(parent);
            if (add_new_child == nullptr)
            {
                delete_Tree();
                throw std::invalid_argument("Parent not found");
            }
            if (add_new_child->right == nullptr)
            {
                add_new_child->right = new Node(right_child);
            }
            else
            {
                (add_new_child->right)->value = right_child;
            }
            return *this;
        }

        friend ostream &operator<<(ostream &out, const BinaryTree &tree)
        {
            return out;
        }

        struct Node
        {
            T value;
            Node *left;
            Node *right;

            Node(const T &v) : value(v), left(nullptr), right(nullptr)
            {
            }
        };
        Node *root;
        Node *find_parent(T value)
        {
            if (root)
            {
                for (auto itr = begin_inorder(); itr != end_inorder(); ++itr)
                {
                    if (*itr == value)
                    {
                        return itr.get_Node();
                    }
                }
            }
            return nullptr;
        }

        void delete_Tree()
        {
            if (!root)
            {
                for (auto it = begin_inorder(); it != end_inorder(); ++it)
                {
                    Node *to_delete = it.get_Node();
                    delete to_delete;
                }
            }
        }

        void copy_Tree(const BinaryTree &otherTree)
        {
            if (otherTree.root)
            {
                root = new Node(otherTree.root->value);
                copy(root, otherTree.root);
            }
        }

        //copy with recurtion
        void copy(Node *node, Node *otherNode)
        {
            if (otherNode->right)
            {
                node->right = new Node(otherNode->right->value);
                copy(node->right, otherNode->right);
            }
            if (otherNode->left)
            {
                node->left = new Node(otherNode->left->value);
                copy(node->left, otherNode->left);
            }
        }

        //-------------------------------------------------------------------
        // iterator related code:
        // inner class and methods that return instances of it)
        //-------------------------------------------------------------------

        class preorder_iterator
        {
        private:
            Node *current_node_ptr;
            queue<Node *> q;

        public:
            void init_preorder(Node *root, queue<Node *> &q)
            {
                if (!(root))
                {
                    return;
                }
                q.push(root);
                init_preorder(root->left, q);
                init_preorder(root->right, q);
            }
            preorder_iterator(Node *curr) : current_node_ptr(curr)
            {
                init_preorder(current_node_ptr, q);
                q.push(nullptr);
                current_node_ptr = q.front();
                q.pop();
            }

            preorder_iterator &operator++()
            {
                current_node_ptr = q.front();
                q.pop();
                return *this;
            }

            preorder_iterator operator++(int)
            {
                preorder_iterator itr = *this;
                current_node_ptr = q.front();
                q.pop();
                return itr;
            }

            T &operator*() const
            {
                return current_node_ptr->value;
            }

            T *operator->() const
            {
                return &(current_node_ptr->value);
            }

            bool operator==(const preorder_iterator &other) const
            {
                return current_node_ptr == other.current_node_ptr;
            }

            bool operator!=(const preorder_iterator &rhs) const
            {
                return current_node_ptr != rhs.current_node_ptr;
            }
        };

        class inorder_iterator
        {
        public:
            Node *current_node_ptr;
            queue<Node *> q;

            void init_inorder(Node *root, queue<Node *> &q)
            {
                if (!(root))
                {
                    return;
                }
                init_inorder(root->left, q);
                q.push(root);
                init_inorder(root->right, q);
            }

            inorder_iterator(Node *curr) : current_node_ptr(curr)
            {
                init_inorder(current_node_ptr, q);
                q.push(nullptr);
                current_node_ptr = q.front();
                q.pop();
            }

            inorder_iterator &operator++()
            {
                current_node_ptr = q.front();
                q.pop();
                return *this;
            }

            inorder_iterator operator++(int)
            {
                inorder_iterator itr = *this;
                current_node_ptr = q.front();
                q.pop();
                return itr;
            }
            T &operator*() const
            {
                return current_node_ptr->value;
            }

            T *operator->() const
            {
                return &(current_node_ptr->value);
            }

            bool operator==(const inorder_iterator &other) const
            {
                return current_node_ptr == other.current_node_ptr;
            }

            bool operator!=(const inorder_iterator &rhs) const
            {
                return current_node_ptr != rhs.current_node_ptr;
            }

            Node *get_Node()
            {
                return current_node_ptr;
            }
        };

        class postorder_iterator
        {
        public:
            Node *current_node_ptr;
            queue<Node *> q;

            void init_postorder(Node *root, queue<Node *> &q)
            {
                if (!(root))
                {
                    return;
                }
                init_postorder(root->left, q);
                init_postorder(root->right, q);
                q.push(root);
            }

            postorder_iterator(Node *curr) : current_node_ptr(curr)
            {
                init_postorder(current_node_ptr, q);
                q.push(nullptr);
                current_node_ptr = q.front();
                q.pop();
            }

            postorder_iterator &operator++()
            {
                current_node_ptr = q.front();
                q.pop();
                return *this;
            }

            postorder_iterator operator++(int)
            {
                postorder_iterator itr = *this;
                current_node_ptr = q.front();
                q.pop();
                return itr;
            }
            T &operator*() const
            {
                return current_node_ptr->value;
            }

            T *operator->() const
            {
                return &(current_node_ptr->value);
            }

            bool operator==(const postorder_iterator &other) const
            {
                return current_node_ptr == other.current_node_ptr;
            }

            bool operator!=(const postorder_iterator &rhs) const
            {
                return current_node_ptr != rhs.current_node_ptr;
            }
        };

        preorder_iterator begin_preorder() const
        {
            return preorder_iterator{root};
        }
        preorder_iterator end_preorder() const
        {
            return preorder_iterator{nullptr};
        }
        inorder_iterator begin_inorder() const
        {
            return inorder_iterator{root};
        }
        inorder_iterator end_inorder() const
        {
            return inorder_iterator{nullptr};
        }
        inorder_iterator begin() const
        {
            return inorder_iterator{root};
        }
        inorder_iterator end() const
        {
            return inorder_iterator{nullptr};
        }
        postorder_iterator begin_postorder() const
        {
            return postorder_iterator{root};
        }
        postorder_iterator end_postorder() const
        {
            return postorder_iterator{nullptr};
        }
    };

}
