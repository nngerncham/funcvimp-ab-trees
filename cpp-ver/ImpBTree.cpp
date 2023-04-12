#include <optional>
#include <stack>
#include <string>
#include <vector>

using K = int;
using V = std::string;

class ImpDictNode {
	public:
		std::vector<K> keys;
		std::vector<V> values;
		std::vector<ImpDictNode *> children;

	ImpDictNode(uint b) {
		this->keys.reserve(b + 1);
		this->values.reserve(b + 1);
		this->children.reserve(b + 1);
	}

	/**
	  Finds the right index to insert a key into node.
	  If key already exists, returns its index.
	  Otherwise, returns the index that the key should belong in.
	  */
	int find_insert_idx(K target_key) {
		if (this->keys.empty()) {
			return 0;
		}

		int idx = 0;
		for (K key: this->keys) {
			if (key >= target_key) {
				return idx;
			}
			idx++;
		}

		return idx;
	}

	/**
	  Inserts a key-value pair into a node.
	  If key already exists, replace the value.
	  Does not care about overflowing.
	  */
	void insert_node(K key, V value) {
		int insert_idx = this->find_insert_idx(key);
		if (insert_idx < this->keys.size() && this->keys[insert_idx] == key) {
			this->values[insert_idx] = value;
			return;
		}

		this->keys.insert(this->keys.begin() + insert_idx, key);
		this->values.insert(this->values.begin() + insert_idx, value);
	}

	bool is_overflowing(int b) {
		return this->keys.size() > b;
	}

	/**
	  Checks if the current node is a leaf or not.
	  */
	bool is_leaf() {
		return this->children.empty();
	}
};

class ImpDict {
	int a;
	int b;

	public:
		std::optional<ImpDictNode *> root;
		int size;

	ImpDict(uint a, uint b) {
		this->a = a;
		this->b = b;

		this->root = std::optional<ImpDictNode *>(std::nullopt);
		this->size = 0;
	}

	/**
	  Creates a stack of nodes visited in order to find that key.
	  Stack ends at a leaf node if key not found.
	  */
	std::stack<ImpDictNode *> search_key(K key) {
		ImpDictNode *current_node = this->root.value();
		std::stack<ImpDictNode *> path;
		path.push(current_node);

		while (!current_node->is_leaf()) {
			int target_idx = current_node->find_insert_idx(key);
			if (current_node->keys[target_idx] == key) {
				break;
			}

			current_node = current_node->children[target_idx];
			path.push(current_node);
		}

		return path;
	}

	/**
	  Inserts a key-value pair into the tree.
	  Grows the tree if necessary.
	  */
	void insert(K key, V value) {
		if (!this->root.has_value()) {
			this->root = new ImpDictNode(this->b);
			this->root.value()->insert_node(key, value);
			return;
		}

		std::stack<ImpDictNode *> path = this->search_key(key);
		ImpDictNode *current_node = path.top();
		current_node->insert_node(key, value);
		if (!current_node->is_overflowing(this->b)) {
			return;
		}

		// grow
	}
};
