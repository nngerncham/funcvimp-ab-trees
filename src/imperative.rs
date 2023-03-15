pub struct ImpDictNode<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    keys: Vec<K>,
    values: Vec<V>,
    children: Vec<Box<ImpDictNode<K, V>>>,
}

impl<K, V> ImpDictNode<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    fn new(b: usize) -> Self {
        ImpDictNode {
            keys: Vec::with_capacity(b),
            values: Vec::with_capacity(b),
            children: Vec::with_capacity(b),
        }
    }

    fn with_kv(b: usize, key: K, value: V) -> Self {
        let mut node = ImpDictNode {
            keys: Vec::with_capacity(b),
            values: Vec::with_capacity(b),
            children: Vec::with_capacity(b),
        };

        node.keys.push(key);
        node.values.push(value);

        node
    }

    fn insert_node(root: &mut ImpDictNode<K, V>, key: K, value: V) {
        match root.keys.binary_search(&key) {
            Result::Err(idx) => {
                root.keys.insert(idx, key);
                root.values.insert(idx, value);
            }
            Result::Ok(idx) => {
                root.keys[idx] = key;
                root.values[idx] = value;
            }
        }
    }
}

pub struct ImpDict<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    root: Option<ImpDictNode<K, V>>,
    size: usize,
    a: usize,
    b: usize,
}

impl<K, V> ImpDict<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    fn new(a: usize, b: usize) -> Self {
        ImpDict {
            root: None,
            size: 0,
            a,
            b,
        }
    }

    fn search_node(root: &ImpDictNode<K, V>, key: &K) -> Box<ImpDictNode<K, V>> {
        Box::new(ImpDictNode::new(0))
    }

    fn grow(&mut self, key: &K, node: &mut ImpDictNode<K, V>) {
        // TODO: implement me
    }

    fn insert(&mut self, key: K, value: V) {
        match &self.root {
            None => {
                self.root = Some(ImpDictNode::with_kv(self.b, key, value));
            }
            Some(root) => {
                let mut target_node = *ImpDict::search_node(root, &key);
                ImpDictNode::insert_node(&mut target_node, key, value);
                if root.keys.len() >= self.b {
                    self.grow(&key, &mut target_node);
                }
            }
        }

        self.size = self.size + 1;
    }

    fn delete(&mut self, key: K) {
        self.size = self.size - 1;
        if self.size <= 0 {
            self.root = None;
        }
    }
}
