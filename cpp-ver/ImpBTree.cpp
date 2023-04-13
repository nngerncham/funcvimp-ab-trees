#include <optional>
#include <ostream>
#include <stack>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>

using K = int;
using V = std::string;

class ImpDictNode {
	public:
		std::vector<K> keys;
		std::vector<V> values;
		std::vector<ImpDictNode *> children;

	ImpDictNode(int b) {
		this->keys.reserve(b + 1);
		this->values.reserve(b + 1);
		this->children.reserve(b + 1);
	}

	ImpDictNode(int b, std::vector<K> _keys, std::vector<V> _values, std::vector<ImpDictNode *> _children) {
		this->keys = _keys;
		this->keys.reserve(b);

		this->values = _values;
		this->values.reserve(b);

		this->children = _children;
		this->children.reserve(b);
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

	void insert_children(int idx, ImpDictNode *left, ImpDictNode *right) {
		if (!this->is_leaf()) {
			this->children.erase(this->children.begin() + idx, this->children.begin() + idx + 1);
		}
		this->children.insert(this->children.begin() + idx, right);
		this->children.insert(this->children.begin() + idx, left);
	}

	bool is_overflowing(int b) {
		return this->keys.size() >= b;
	}

	/**
	  Checks if the current node is a leaf or not.
	  */
	bool is_leaf() {
		return this->children.empty();
	}

	void print_node() {
		for (int i = 0; i < this->keys.size(); i++) {
			std::cout << "(" <<
				std::to_string(this->keys[i]) << ": " <<
				this->values[i] << "), ";
		}
		std::cout << std::endl;
	}
};

class ImpDict {
	int b;

	public:
		std::optional<ImpDictNode *> root;
		int size;

	ImpDict(int b) {
		this->b = b;

		this->root = std::optional<ImpDictNode *>(std::nullopt);
		this->size = 0;
	}

	static void print_tree(ImpDictNode *node) {
		if (node->is_leaf()) {
			node->print_node();
			return;
		}

		for (int i = 0; i < node->keys.size(); i++) {
			print_tree(node->children[i]);
			std::cout << "(" <<
				std::to_string(node->keys[i]) << ": " <<
				node->values[i] << "), ";
		}
		std::cout << std::endl;
		print_tree(node->children[node->children.size() - 1]);
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
	  Splits a node into two for propagation.
	  */
	using SplitNode = std::tuple<ImpDictNode *, K, V, ImpDictNode *>;
	SplitNode split_node(ImpDictNode *node) {
		int mid = node->keys.size() / 2;
		K mid_key = node->keys[mid];
		V mid_value = node->values[mid];

		std::vector<K> head_keys (node->keys.begin(), node->keys.begin() + mid);
		std::vector<K> tail_keys (node->keys.begin() + mid + 1, node->keys.end());

		std::vector<V> head_values (node->values.begin(), node->values.begin() + mid);
		std::vector<V> tail_values (node->values.begin() + mid + 1, node->values.end());

		std::vector<ImpDictNode *> head_children;
		std::vector<ImpDictNode *> tail_children;
		if (!node->children.empty()) {
			int c_mid = node->children.size() / 2;
			head_children = std::vector<ImpDictNode *> (node->children.begin(), node->children.begin() + c_mid);
			tail_children = std::vector<ImpDictNode *> (node->children.begin() + c_mid, node->children.end());
		}

		ImpDictNode *left = new ImpDictNode(this->b, head_keys, head_values, head_children);
		ImpDictNode *right = new ImpDictNode(this->b, tail_keys, tail_values, tail_children);

		return std::make_tuple(left, mid_key, mid_value, right);
	}

	/**
	  Inserts a key-value pair into the tree.
	  Grows the tree if necessary.
	  */
	void insert(K key, V value) {
		this->size++;
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

		path.pop();
		while (!path.empty()) {
			ImpDictNode *parent = path.top();

			auto [left, k, v, right] = split_node(current_node);
			int target_idx = parent->find_insert_idx(k);
			parent->insert_node(k, v);
			parent->insert_children(target_idx, left, right);

			current_node = parent;
			path.pop();
		}

		if (current_node->is_overflowing(this->b)) {
			auto [left, k, v, right] = split_node(current_node);
			ImpDictNode *new_root = new ImpDictNode(b);

			int insert_idx = new_root->find_insert_idx(k);
			new_root->insert_node(k, v);
			new_root->insert_children(insert_idx, left, right);

			this->root = new_root;
		}
	}
};
