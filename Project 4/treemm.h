#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <iostream> // How does including work in h file
#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator(std::vector<ValueType> *vals)
        {
            m_vals = vals;
            index = 0;
        }
        Iterator() {
            m_vals = nullptr;
            index = 0;
        }
        ValueType& get_value() const
        {
            return (*m_vals)[index];  // Replace this line with correct code.
        }

        bool is_valid() const
        {
            return m_vals && index < m_vals->size()?true:false;
        }

        void advance()
        {
            index++;
        }

      private:
        std::vector<ValueType> *m_vals;
        int index;
        
    };

    TreeMultimap()
    {
        // Replace this line with correct code.
        m_head = nullptr;
    }

    ~TreeMultimap()
    {
        deleteHelper(m_head);
        m_head = nullptr;
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        // If empty, insert at beginning and exit
        if (m_head==nullptr) {
            m_head = new Node(key, value);
            return;
        }
        Node* n = m_head;
        // If empty, just create here
        
        while(true) {
            
            // HERE
            // If value equal, add and leave
            if (n->key == key) {
                n->vals.push_back(value);
                break;
            }
            
            // LEFT
            // If value less
            if(key < n->key) {
                // If left go left
                if(n->left != nullptr) {
                    n = n->left;
                    continue;
                }
                // If no left make left
                n->left = new Node(key, value);
                break;
            }
            
            // RIGHT
            // If value more, go right or create right and leave
            if(key > n->key) {
                // If right go right
                if(n->right != nullptr) {
                    n = n->right;
                    continue;
                }
                // If no right make right
                n->right = new Node(key, value);
                break;
            }
        }
        
    }

    Iterator find(const KeyType& key) const
    {
        if (m_head==nullptr) {
            return Iterator(nullptr);
        }
        Node* n = m_head;
        
        while(true) {
            if(n==nullptr) {
                return Iterator(nullptr);
            }
            
            if (n->key == key) {
                return Iterator(&(n->vals));
            }
            
            // LEFT
            // If left go left
            if(key < n->key) {
                    n = n->left;
                    continue;
            }
            
            // Right
            // If left go left
            if(key > n->key) {
                    n = n->right;
                    continue;
            }
            
            // HERE
            
            
            
        }
        return Iterator(nullptr);
    }

  private:
    struct Node {
        Node(KeyType k, ValueType v) {
            key = k;
            vals.push_back(v);
            left = nullptr;
            right = nullptr;
        }
        KeyType key;
        std::vector<ValueType> vals;
        Node* left;
        Node* right;
    };
    Node* m_head;
    
    // Recursively delete all nodes
    void deleteHelper(Node* n) {
        if(!n) {return;}
        deleteHelper(n->left);
        deleteHelper(n->right);
        delete n;
    }
};

#endif // TREEMULTIMAP_INCLUDED
