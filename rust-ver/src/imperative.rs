use crate::constants::B;
use std::{
    borrow::BorrowMut,
    cell::{RefCell, RefMut},
};

struct ImpDictNode<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    keys: Vec<K>,
    values: Vec<V>,
    children: Vec<Box<ImpDictNode<K, V>>>,
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

    fn search_node(&self, key: &K) -> Result<usize, usize> {
        self.keys.binary_search(key)
    }

    fn get_node(&self, key: &K) -> Option<V> {
        match self.keys.binary_search(key) {
            Result::Ok(idx) => Some(self.values[idx]),
            Result::Err(_) => None,
        }
    }

    fn is_leaf(&self) -> bool {
        self.children.is_empty()
    }

    fn min_max_keys(&self) -> Option<(K, K)> {
        if self.keys.is_empty() {
            None
        } else {
            Some((self.keys[0], self.keys[self.keys.len() - 1]))
        }
    }

    fn is_overflowing(&self) -> bool {
        self.keys.len() > B
    }
}

pub struct ImpDict<K, V>
where
    K: Copy + Clone + Ord,
    V: Copy + Clone,
{
    root: Option<Box<ImpDictNode<K, V>>>,
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

    fn insert(&mut self, key: K, value: V) {
        if self.root.is_none() {
            let mut new_root = ImpDictNode::new();
            new_root.insert_node(key, value);
            self.root = Some(Box::new(new_root));
            return;
        }

        let mut stack: Vec<RefMut<Box<ImpDictNode<K, V>>>> = Vec::new();
        let mut node = RefCell::new(self.root.unwrap());
        stack.push(node.borrow_mut());

        // Keeps going until it finds a suitable leaf.
        // The loop is read-only since it's only finding the right node so it
        // slaps the mutable reference into the stack while being immutable itself.
        while !node.borrow().is_leaf() {
            let idx_result = node.borrow().search_node(&key);
            match idx_result {
                Err(idx) => {
                    let target_child = node.borrow().children[idx];
                    stack.push(node.borrow_mut());
                    node = RefCell::new(target_child);
                }
                Ok(_) => {
                    break;
                }
            }
        }
    }
}
