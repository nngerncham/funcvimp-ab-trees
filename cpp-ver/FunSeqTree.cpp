#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <tuple>

using K = int;

class FunSeqNode {
	public:
		std::vector<K> keys;
		std::vector<FunSeqNode *> children;

		using SplitNode = std::tuple<FunSeqNode *, K, FunSeqNode *>;

	FunSeqNode(K key) {
		this->keys = std::vector<K>{ key };
	}

	FunSeqNode(std::vector<K> _keys, std::vector<FunSeqNode *> _children) {
		this->keys = _keys;
		this->children = _children;
	}

	/**
	  Clones the node `from` and replaces the child with a new child.
	  Used when replacing a node a long the path.
	  */
	FunSeqNode(FunSeqNode *child, int c_idx, FunSeqNode *from) {
		this->keys = std::vector<K>(from->keys);
		this->children = std::vector<FunSeqNode *>(from->children);

		if (!this->is_leaf()) {
			this->children[c_idx] = child;
		} else {
			this->children.push_back(child);
		}
	}

	/**
	  Clones the node `from` and replaces the child with a new child.
	  Used when propagating an overflowing node up to parent.
	  */
	FunSeqNode(K k, FunSeqNode *left, FunSeqNode *right, FunSeqNode *from) {
		int insert_idx = from->find_insert_idx(k);

		this->keys = std::vector<K>(from->keys);
		this->keys.insert(this->keys.begin() + insert_idx, k);

		this->children = std::vector<FunSeqNode *>(from->children);
		if (!this->is_leaf()) {
			this->children[insert_idx] = right;
			this->children.insert(this->children.begin() + insert_idx, left);
		} else {
			this->children.push_back(left);
			this->children.push_back(right);
		}
	}

	/**
	  Clones the node `from` and replaces the child with a new child.
	  Used when propagating an overflowing node up to parent.
	  */
	FunSeqNode(K k, FunSeqNode *left, FunSeqNode *right) {
		this->keys = std::vector<K>{ k };

		this->children.push_back(left);
		this->children.push_back(right);
	}

	/**
	  Clones the node `from` and inserts key and value into the new node.
	  */
	FunSeqNode(K key, FunSeqNode *from) {
		int insert_idx = from->find_insert_idx(key);

		this->keys = std::vector<K>(from->keys);

		if (this->keys[insert_idx] != key) {
			this->keys.insert(this->keys.begin() + insert_idx, key);
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

		K k = this->keys[n_keys / 2];

		int n_child = this->children.size();
		std::vector<FunSeqNode *> head_children;
		std::vector<FunSeqNode *> tail_children;
		if (n_child > 0) {
			head_children = std::vector<FunSeqNode *>(this->children.begin(), this->children.begin() + n_child / 2);
			tail_children = std::vector<FunSeqNode *>(this->children.begin() + n_child / 2, this->children.end());
		}

		return std::make_tuple(new FunSeqNode(head_keys, head_children),
							   k,
							   new FunSeqNode(tail_keys, tail_children));
	}

	bool is_overflowing(int b) {
		return this->keys.size() >= b;
	}

	bool is_leaf() {
		return this->children.empty();
	}

	//void print_node() {
		//for (int i = 0; i < this->keys.size(); i++) {
			//std::cout << "(" <<
				//std::to_string(this->keys[i]) << ": " <<
				//this->values[i] << "), ";
		//}
		//std::cout << std::endl;
	//}
};

class FunSeq {
	int b;

	public:
		std::optional<FunSeqNode *> root;
		int size;

	FunSeq(int _b) {
		this->b = _b;
	}

	/**
	  Although FunSeq is supposed to be as functional as it can be, it is still
	  just an interface so the internal mutability is functional but to the
	  programmer, it is imperative.
	  */
	void insert(K key) {
		if (this->root.has_value()) {
			FunSeqNode *new_root = insert_helper(this->root.value(), key);
			if (!new_root->is_overflowing(this->b)) {
				this->root = new_root;
			} else {
				auto [left, k, right] = new_root->split();
				this->root = new FunSeqNode(k, left, right);
			}
		} else {
			this->root = new FunSeqNode(key);
		}
	}

	FunSeqNode *insert_helper(FunSeqNode *node, K key) {
		if (node->is_leaf()) {
			return new FunSeqNode(key, node);
		}

		int insert_idx = node->find_insert_idx(key);
		FunSeqNode *new_child = insert_helper(node->children[insert_idx], key);
		if (!new_child->is_overflowing(this->b)) {
			return new FunSeqNode(new_child, insert_idx, node);
		}

		auto [left, k, right] = new_child->split();
		return new FunSeqNode(k, left, right, node);
	}
};
