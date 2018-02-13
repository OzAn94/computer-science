/**
 * Represents a red-black tree data structure, allowing I/O operations of insertion, searching,
 * deletion, successor, predecessor, minimum and maximum.
 *
 * The tree nodes are objects of sub-classes of RBTreeNode class.
 *
 * This data structure is based on "Introduction to Algorithms" book, by
 * Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford Stein.
 *
 * @author Oz Anani
 */
public class RBTree {

    private RBTreeNode _root;
    private RBTreeNode _nullNode;

    /**
     * Constructor for objects of class RBTree
     */
    /**
     * Constructor for red-black trees.
     * The null-node must be created by the calling function, for further comparisons.
     *
     * @param nullNode the tree's null node
     */
    public RBTree(RBTreeNode nullNode) {
        _nullNode = nullNode;
        _nullNode.turnBlack(); // null-node is always black
        _root = _nullNode;
    }

    /**
     * Red-black tree left rotation. Assuming that x.getRightSon() != null
     *
     * @param x left rotate around x
     */
    private void rotateLeft(RBTreeNode x) {
        RBTreeNode y = x.getRightSon();
        x.setRightSon(y.getLeftSon());

        if (y.getLeftSon() != _nullNode)
            y.getLeftSon().setParent(x);

        y.setParent(x.getParent());

        if (x.getParent() == _nullNode)
            _root = y;

        else if (x == x.getParent().getLeftSon())
            x.getParent().setLeftSon(y);

        else x.getParent().setRightSon(y);

        y.setLeftSon(x);
        x.setParent(y);
    }

    /**
     * Red-black tree right rotation. Assuming that x.getRightSon() != null
     *
     * @param x right rotate around x
     */
    private void rotateRight(RBTreeNode x) {
        RBTreeNode y = x.getLeftSon();
        x.setLeftSon(y.getRightSon());

        if (y.getRightSon() != _nullNode)
            y.getRightSon().setParent(x);

        y.setParent(x.getParent());

        if (x.getParent() == _nullNode)
            _root = y;

        else if (x == x.getParent().getRightSon())
            x.getParent().setRightSon(y);

        else x.getParent().setLeftSon(y);

        y.setRightSon(x);
        x.setParent(y);
    }

    /**
     * Red-black tree insertion.
     *
     * @param z the node to insert
     */
    public void insert(RBTreeNode z) {
        RBTreeNode y = _nullNode;
        RBTreeNode x = _root;

        while (x != _nullNode) {
            y = x;
            if (z.getKey() < x.getKey())
                x = x.getLeftSon();
            else
                x = x.getRightSon();
        }

        z.setParent(y);

        if (y == _nullNode)
            _root = z;
        else if (z.getKey() < y.getKey())
            y.setLeftSon(z);
        else
            y.setRightSon(z);

        z.setLeftSon(_nullNode);
        z.setRightSon(_nullNode);
        z.turnRed();

        insertFixup(z);
    }

    /**
     * Red-black tree insertion fixup method.
     *
     * @param z the inserted node
     */
    private void insertFixup(RBTreeNode z) {
        RBTreeNode y;

        while (z.getParent().isRed()) {
			
            if (z.getParent() == z.getParent().getParent().getLeftSon()) {
                y = z.getParent().getParent().getRightSon();

                if (y.isRed()) {
                    z.getParent().turnBlack();
                    y.turnBlack();
                    z.getParent().getParent().turnRed();
                    z = z.getParent().getParent();
					
                } else {
                    if (z == z.getParent().getRightSon()) {
                        z = z.getParent();
                        rotateLeft(z);
                    }

                    z.getParent().turnBlack();
                    z.getParent().getParent().turnRed();
                    rotateRight(z.getParent().getParent());
                }

            } else {
                y = z.getParent().getParent().getLeftSon();

                if (y.isRed()) {
                    z.getParent().turnBlack();
                    y.turnBlack();
                    z.getParent().getParent().turnRed();
                    z = z.getParent().getParent();

                } else {
                    if (z == z.getParent().getLeftSon()) {
                        z = z.getParent();
                        rotateRight(z);
                    }

                    z.getParent().turnBlack();
                    z.getParent().getParent().turnRed();
                    rotateLeft(z.getParent().getParent());
                }
            }
        }

        _root.turnBlack();
    }

    // assume that z is not _nullNode

    /**
     * Red-black tree deletion. Assuming that z != _nullNode
     *
     * @param z the node to delete
     * @return
     */
    public RBTreeNode delete(RBTreeNode z) {
        RBTreeNode y, x;

        if (z.getLeftSon() == _nullNode || z.getRightSon() == _nullNode)
            y = z;
        else
            y = successor(z);

        if (y.getLeftSon() != _nullNode)
            x = y.getLeftSon();
        else
            x = y.getRightSon();

        x.setParent(y.getParent());

        if (y.getParent() == _nullNode)
            _root = x;
        else if (y == y.getParent().getLeftSon())
            y.getParent().setLeftSon(x);
        else
            y.getParent().setRightSon(x);

        if (y != z) // copy satellite data
            z.overwriteNode(y);

        if (y.isBlack())
            deleteFixup(x);

        return y;
    }

    /**
     * Red-black tree deletion fixup.
     *
     * @param x the node to fix
     */
    private void deleteFixup(RBTreeNode x) {
        RBTreeNode w;

        while (x != _root && x.isBlack()) {
            if (x == x.getParent().getLeftSon()) {
                w = x.getParent().getRightSon();

                if (w.isRed()) {
                    w.turnBlack();
                    x.getParent().turnRed();
                    rotateLeft(x.getParent());
                    w = x.getParent().getRightSon();
                }

                if (w.getLeftSon().isBlack() && w.getRightSon().isBlack()) {
                    w.turnRed();
                    x = x.getParent();
					
                } else {
                    if (w.getRightSon().isBlack()) {
                        w.getLeftSon().turnBlack();
                        w.turnRed();
                        rotateRight(w);
                        w = x.getParent().getRightSon();
                    }

                    if (x.getParent().isRed())
                        w.turnRed();
                    else
                        w.turnBlack();

                    x.getParent().turnBlack();
                    w.getRightSon().turnBlack();
                    rotateLeft(x.getParent());
                    x = _root;
                }
				
            } else {
                w = x.getParent().getLeftSon();

                if (w.isRed()) {
                    w.turnBlack();
                    x.getParent().turnRed();
                    rotateRight(x.getParent());
                    w = x.getParent().getLeftSon();
                }

                if (w.getRightSon().isBlack() && w.getLeftSon().isBlack()) {
                    w.turnRed();
                    x = x.getParent();
					
                } else {
                    if (w.getLeftSon().isBlack()) {
                        w.getRightSon().turnBlack();
                        w.turnRed();
                        rotateLeft(w);
                        w = x.getParent().getLeftSon();
                    }

                    if (x.getParent().isRed())
                        w.turnRed();
                    else
                        w.turnBlack();

                    x.getParent().turnBlack();
                    w.getLeftSon().turnBlack();
                    rotateRight(x.getParent());
                    x = _root;
                }
            }
        }

        x.turnBlack();
    }

    /**
     * Perform a search in the whole tree.
     *
     * @param k the key to search
     * @return the matching node, or null if not found
     */
    public RBTreeNode search(int k) {
        return search(_root, k);
    }

    /**
     * Red-black tree searching.
     *
     * @param x the root of the sub-tree to search in
     * @param k the key to search
     * @return the matching node or null if not found
     */
    private RBTreeNode search(RBTreeNode x, int k) {
        while (x != _nullNode && k != x.getKey())
            if (k < x.getKey())
                x = x.getLeftSon();
            else
                x = x.getRightSon();

        return x;
    }

    /**
     * Get the minimum node in the sub-tree rooted in x.
     *
     * @param x the root of the sub-tree
     * @return the min node
     */
    public RBTreeNode min(RBTreeNode x) {
        if (x == _nullNode)
            return x;

        while (x.getLeftSon() != _nullNode)
            x = x.getLeftSon();

        return x;
    }

    /**
     * Get the maximum node in the sub-tree rooted in x.
     *
     * @param x the root of the sub-tree
     * @return the max node
     */
    public RBTreeNode max(RBTreeNode x) {
        if (x == _nullNode)
            return x;

        while (x.getRightSon() != _nullNode)
            x = x.getRightSon();

        return x;
    }

    /**
     * Get the successor of node x. Assuming that x is not _nullNode.
     *
     * @param x node
     * @return the successor of x, or null if x is maximum
     */
    public RBTreeNode successor(RBTreeNode x) {
        if (x.getRightSon() != _nullNode)
            return min(x.getRightSon());

        RBTreeNode y = x.getParent();

        while (y != _nullNode && x == y.getRightSon()) {
            x = y;
            y = y.getParent();
        }

        return y;
    }

    /**
     * Get the predecessor of node x. Assuming that x is not _nullNode.
     *
     * @param x node
     * @return the predecessor of x, or null if x is minimum
     */
    public RBTreeNode predecessor(RBTreeNode x) {
        if (x.getLeftSon() != _nullNode)
            return max(x.getLeftSon());

        RBTreeNode y = x.getParent();

        while (y != _nullNode && x == y.getLeftSon()) {
            x = y;
            y = y.getParent();
        }

        return y;
    }

    /*
    Helper functions for printing the tree
    */
    public void display() {
        final int height = 5, width = 64;

        int len = width * height * 2 + 2;
        StringBuilder sb = new StringBuilder(len);
        for (int i = 1; i <= len; i++)
            sb.append(i < len - 2 && i % width == 0 ? "\n" : ' ');

        displayR(sb, width / 2, 1, width / 4, width, _root, " ");
        System.out.println(sb);
    }

    /*
    Helper functions for printing the tree
    */
    private void displayR(StringBuilder sb, int c, int r, int d, int w, RBTreeNode n,
                          String edge) {
        if (n != _nullNode) {
            displayR(sb, c - d, r + 2, d / 2, w, n.getLeftSon(), " /");

            String color = "red";

            if (n.isBlack())
                color = "black";

            String s = color + "-" + String.valueOf(n.getKey());
            int idx1 = r * w + c - (s.length() + 1) / 2;
            int idx2 = idx1 + s.length();
            int idx3 = idx1 - w;
            if (idx2 < sb.length())
                sb.replace(idx1, idx2, s).replace(idx3, idx3 + 2, edge);

            displayR(sb, c + d, r + 2, d / 2, w, n.getRightSon(), "\\ ");
        }
    }
}
