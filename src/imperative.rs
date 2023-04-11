const A: usize = 4;
const B: usize = 2 * A;

struct ImpDictNode<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    keys: Vec<K>,
    values: Vec<V>,
    children: Vec<ImpDictNode<K, V>>,
}

/**
Assume that all the restructuring will be handled by the tree so insertion
and deletion is just for each of the node
*/
impl<K, V> ImpDictNode<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    fn new() -> Self {
        ImpDictNode {
            keys: Vec::with_capacity(B + 1),
            values: Vec::with_capacity(B + 1),
            children: Vec::with_capacity(B + 1),
        }
    }

    fn insert_node(&mut self, key: K, value: V) {
        match self.keys.binary_search(&key) {
            Result::Err(idx) => {
                self.keys.insert(idx, key);
                self.values.insert(idx, value);
            }
            Result::Ok(idx) => {
                self.keys[idx] = key;
                self.values[idx] = value;
            }
        }
    }

    fn delete_node(&mut self, key: K) {
        match self.keys.binary_search(&key) {
            Result::Ok(idx) => {
                self.keys.remove(idx);
                self.values.remove(idx);
            }
            _ => {}
        }
    }

    fn get_node(&self, key: &K) -> Option<V> {
        match self.keys.binary_search(key) {
            Result::Ok(idx) => Some(self.values[idx]),
            Result::Err(_) => None,
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
}

impl<K, V> ImpDict<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    fn new() -> ImpDict<K, V> {
        ImpDict {
            root: None,
            size: 0,
        }
    }
}
