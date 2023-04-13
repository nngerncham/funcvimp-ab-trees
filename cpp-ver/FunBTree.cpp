#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <tuple>

using K = int;
using V = std::string;

class FunDictNode {
	public:
		std::vector<K> keys;
		std::vector<V> values;
		std::vector<FunDictNode *> children;

		using SplitNode = std::tuple<FunDictNode *, K, V, FunDictNode *>;

	FunDictNode(K key, V value) {
		this->keys = std::vector<K>{ key };
		this->values = std::vector<V>{ value };
	}

	FunDictNode(std::vector<K> _keys, std::vector<V> _values) {
		this->keys = _keys;
		this->values = _values;
	}

	/**
	  Clones the node `from` and replaces the child with a new child.
	  */
	FunDictNode(FunDictNode *child, int c_idx, FunDictNode *from) {
		this->keys = std::vector<K>(from->keys);
		this->values = std::vector<V>(from->values);

		this->children = std::vector<FunDictNode *>(from->children);
		this->children[c_idx] = child;
	}

	/**
	  Clones the node `from` and replaces the child with a new child.
	  */
	FunDictNode(K k, V v, FunDictNode *left, FunDictNode *right, FunDictNode *from) {
		int insert_idx = from->find_insert_idx(k);

		this->keys = std::vector<K>(from->keys);
		this->keys.insert(this->keys.begin() + insert_idx, k);

		this->values = std::vector<V>(from->values);
		this->values.insert(this->values.begin() + insert_idx, v);

		this->children = std::vector<FunDictNode *>(from->children);
		this->children[insert_idx] = right;
		this->children.insert(this->children.begin() + insert_idx, left);
	}

	/**
	  Clones the node `from` and inserts key and value into the new node.
	  */
	FunDictNode(K key, V value, FunDictNode *from) {
		int insert_idx = from->find_insert_idx(key);

		this->keys = std::vector<K>(from->keys);
		this->values = std::vector<V>(from->values);

		if (this->keys[insert_idx] == key) {
			this->values[insert_idx] = value;
		} else {
			this->keys.insert(this->keys.begin() + insert_idx, key);
			this->values.insert(this->values.begin() + insert_idx, value);
		}
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
	  Splits node into two nodes for propagation.
	  */
	SplitNode split() {
		int n_keys = this->keys.size();
		std::vector<K> head_keys = std::vector<K>(this->keys.begin(),
												  this->keys.begin() + n_keys / 2);
		std::vector<K> tail_keys = std::vector<K>(this->keys.begin() + n_keys / 2 + 1,
												  this->keys.end());

		std::vector<V> head_values = std::vector<V>(this->values.begin(),
												  this->values.begin() + n_keys / 2);
		std::vector<V> tail_values = std::vector<V>(this->values.begin() + n_keys / 2 + 1,
												  this->values.end());

		K k = this->keys[n_keys / 2];
		V v = this->values[n_keys / 2];

		int n_child = this->children.size();
		std::vector<FunDictNode *> head_children = std::vector<FunDictNode *>(this->children.begin(), this->children.begin() + n_child / 2);
		std::vector<FunDictNode *> tail_children = std::vector<FunDictNode *>(this->children.begin() + n_child / 2, this->children.end());

		return std::make_tuple(new FunDictNode(head_keys, head_values),
							   k, v,
							   new FunDictNode(tail_keys, tail_values));
	}

	bool is_overflowing(int b) {
		return this->keys.size() >= b;
	}

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

class FunDict {
	int b;

	public:
		std::optional<FunDictNode *> root;
		int size;

	FunDict(int _b) {
		this->b = _b;
	}

	/**
	  Although FunDict is supposed to be as functional as it can be, it is still
	  just an interface so the internal mutability is functional but to the
	  programmer, it is imperative.
	  */
	void insert(K key, V value) {
		if (this->root.has_value()) {
			this->root = insert_helper(this->root.value(), key, value);
		} else {
			this->root = new FunDictNode(key, value);
		}
	}

	FunDictNode *insert_helper(FunDictNode *node, K key, V value) {
		if (node->is_leaf()) {
			return new FunDictNode(key, value, node);
		}

		int insert_idx = node->find_insert_idx(key);
		FunDictNode *new_child = insert_helper(node->children[insert_idx], key, value);

		if (!new_child->is_overflowing(this->b)) {
			return new FunDictNode(new_child, insert_idx, node);
		}

		auto [left, k, v, right] = new_child->split();
		return new FunDictNode(k, v, left, right, node);
	}
};
