#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <stack>
#include <string>
#include <vector>

template <class K, class V>
class ImpDictNode {
	public:
		std::vector<K> keys;
		std::vector<V> values;
		std::vector<ImpDictNode<K, V> *> childtren;

	ImpDictNode<K, V>(uint b) {
		this->keys.reserve(b + 1);
		this->values.reserve(b + 1);
		this->children.reserve(b + 1);
	}
};

template <class K, class V>
class ImpDict {
	uint a;
	uint b;

	public:
		std::optional<ImpDictNode<K, V> *> root;
		uint size;

	ImpDict<K, V>(uint a, uint b) {
		this->a = a;
		this->b = b;

		this->root = std::optional<ImpDictNode<K, V>>(std::nullopt);
		this->size = 0;
	}
};
