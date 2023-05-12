/*
 * Author: Rahul M
 * Date: 07-02-2023
 *
 * Segment tree with lazy propagation for solving problems involving range queries and range updates.
 * The values in the nodes are updated lazily as an when needed which allows range updates in log(n) time.
 * This code can be easily modified for solving different types of problems.
 *
 * Time Complexity:-
 *      Range Query = O(log(n))
 *      Range Update = O(log(n))
 *
 * Space Complexity = O(n)
 */

template <typename T=int>
class SegmentTree {

public:
    vector<T> st;
    vector<T> arr;
    vector<T> lazy;
    int n;

    // modifiable
    T invalidValue = 0;

    int left(int ind) {
        return 2*ind + 1;
    }

    int right(int ind) {
        return 2*ind + 2;
    }

    // modifiable
    T operate(T a, T b) {
        return a + b;
    }

    // modifiable
    void pull(int ind) {
        st[ind] = operate(st[left(ind)], st[right(ind)]);
    }

    // modifiable 
    void push(int ss, int se, int si) {
        // for range sum query
        st[si] += lazy[si]*(se-ss+1);

        // for range max/min query
        // st[si] += lazy[si];

        if(ss != se) {
            lazy[left(si)] += lazy[si];
            lazy[right(si)] += lazy[si];
        }

        lazy[si] = 0;
    }

    void build(int ss, int se, int si) {
        // if we reached a leaf node
        if(ss == se) {
            st[si] = arr[ss];
            return;
        }

        int mid = (ss+se)/2;

        // build the left and right subtrees
        build(ss, mid, left(si));
        build(mid+1, se, right(si));

        // calculate the value for the current node
        pull(si);
    }

    T query(int qs, int qe, int ss, int se, int si) {
        // push the updates to the left and right subtrees
        push(ss, se, si);

        // if we are completely outside query range
        if(se < qs or ss > qe) return invalidValue;

        // if we are completely inside query range
        if(ss >= qs and se <= qe) return st[si];

        int mid = (ss+se)/2;

        // find the values for the left and right child
        T leftVal = query(qs, qe, ss, mid, left(si));
        T rightVal = query(qs, qe, mid+1, se, right(si));

        return operate(leftVal, rightVal);
    }

    void update(int qs, int qe, T val, int ss, int se, int si) {
        // push the updates to the left and right subtrees
        push(ss, se, si);

        // if we are completely outside query range
        if(se < qs or ss > qe) return;

        // if we are completely inside query range
        if(ss >= qs and se <= qe) {
            lazy[si] += val;
            push(ss, se, si);
            return;
        }

        int mid = (ss+se)/2;

        // update the left subtree and the right subtree
        update(qs, qe, val, ss, mid, left(si));
        update(qs, qe, val, mid+1, se, right(si));

        // calculate the value for the current node
        pull(si);
    }

    SegmentTree(vector<T>& arr) {
        n = arr.size();
        this->arr = arr;
        st = vector<T>(4*n);
        lazy = vector<T>(4*n, 0);

        // build the segment tree
        build(0, n-1, 0);
    }

    SegmentTree(int n) {
        this->n = n;
        arr = vector<T>(n, 0);
        st = vector<T>(4*n);
        lazy = vector<T>(4*n, 0);

        // build the segment tree
        build(0, n-1, 0);
    }

    T query(int qs, int qe) {
        return query(qs, qe, 0, n-1, 0);
    }

    void update(int qs, int qe, T val) {
        update(qs, qe, val, 0, n-1, 0);
    }
};
