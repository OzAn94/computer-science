/**
 * A simple, non-generic and minimalist implementation of the heap sort algorithm
 * using d-ary maximum heap, to sort an array of integers.
 *
 * "A d-ary heap is like a binary heap, but (with one possible exception) non-leaf
 * nodes have d children instead of 2 children." (problem 4.2, "Introduction to Algorithms",
 * by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford Stein.
 *
 * The heap is represented by an array.
 *
 * The root of the heap is _heapArray[0].
 * 
 * @author OzAn94
 */
public class MaxDHeap {
    // instance variables
    private int[] _heapArray;
    private int _heapSize; // current heap size
    private int _d;

    // constants
    private final int MAX_HEAP_SIZE = 200;

    /**
     * Constructor for d-ary heaps.
     * Constructs a d-heap from a given array.
     *
     * @param arr array contains the values to create the heap from
     * @param d   d
     */
    public MaxDHeap(int[] arr, int d) {
        _d = d;
        _heapSize = arr.length;
        _heapArray = new int[MAX_HEAP_SIZE];

        // copy the array values
        for (int i = 0; i < arr.length; i++)
            _heapArray[i] = arr[i];

        buildMaxDHeap();
    }

    /**
     * Build max-d-heap from _heapArray. This method re-arranges _heapArray.
     */
    private void buildMaxDHeap() {
        // calculate the index of the first leaf of the heap
        int firstLeaf = (int) Math.ceil((double) (_heapSize - 1) / _d);

        // iterate through the last parent up to the root
        for (int i = firstLeaf - 1; i >= 0; i--)
            maxHeapify(i);
    }

    /**
     * Returns the index of the k'th child of _heapArray[i] (1'th..d'th)
     *
     * @param i the parent index in _heapArray
     * @param k the number of the child to get (from 1 to d)
     * @return the index of the k'th child of _heapArray[i]
     */
    private int child(int i, int k) {
        return i * _d + k;
    }

    /**
     * Returns the index of the parent of _heapArray[i].
     *
     * @param i the child index
     * @return the index of the parent of _heapArray[i]
     */
    private int parent(int i) {
        return (int) Math.ceil(((double) i / _d) - 1);
    }

    /**
     * Do max-heapify to the node in _heapArray[i].
     *
     * @param i the index of the node to max-heapify
     */
    private void maxHeapify(int i) {
        int k = 1; // indicates first child 
        int c = child(i, k);
        int largest = i;

        // iterate until the child index isn't in the heap or there
        // are no more children.
        while (c < _heapSize && k <= _d) {
            if (_heapArray[c] > _heapArray[largest])
                largest = c;

            k++;
            c = child(i, k);
        }

        // check if one of the children is greater than the parent
        if (largest != i) {
            swap(i, largest);
            maxHeapify(largest);
        }
    }

    /**
     * Swaps between 2 values of the array.
     *
     * @param i the first index
     * @param j the second index
     */
    private void swap(int i, int j) {
        int temp = _heapArray[i];
        _heapArray[i] = _heapArray[j];
        _heapArray[j] = temp;
    }

    /**
     * Sorts the array using HeapSort algorithm.
     */
    public void sort() {
        // iterate the non-root nodes
        for (int i = _heapSize - 1; i > 0; i--) {
            swap(0, i);
            _heapSize--;
            maxHeapify(0);
        }
    }
}