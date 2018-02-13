/**
 * An abstract representation of red-black tree node.
 * Inherited classes have to implement overwriteNode method.
 *
 * @author Oz Anani
 */
public abstract class RBTreeNode {
	
    private int _key;
    private RBTreeNode _p;
    private RBTreeNode _left;
    private RBTreeNode _right;
    private boolean _color;

    private static final boolean RED = true;
    private static final boolean BLACK = false;

    /**
     * Constructor for objects of RBTreeNode.
     *
     * @param key the node's key
     */
    public RBTreeNode(int key) {
        _key = key;
        _p = null;
        _left = null;
        _right = null;
        _color = RED;
    }

    // used for Reb-Black Tree delete method

    /**
     * Overwrite this node key and data with the input node's key and data.
     * This method is used in the RB-Tree delete process.
     *
     * @param node the required key and data
     */
    public abstract void overwriteNode(RBTreeNode node);

    /**
     * Get the node's key.
     *
     * @return the node's key
     */
    public int getKey() {
        return _key;
    }

    /**
     * Set the node's key.
     *
     * @param key the new key
     */
    protected void setKey(int key) {
        _key = key;
    }

    /**
     * Get the node's parent node.
     *
     * @return the parent node
     */
    public RBTreeNode getParent() {
        return _p;
    }

    /**
     * Get the node's left son.
     *
     * @return the left son
     */
    public RBTreeNode getLeftSon() {
        return _left;
    }

    /**
     * Get the node's right son.
     *
     * @return the right son
     */
    public RBTreeNode getRightSon() {
        return _right;
    }

    /**
     * Check if the node is red.
     *
     * @return true if the node is red
     */
    public boolean isRed() {
        return _color == RED;
    }

    /**
     * Check if the node is black. (this method was added for readability in code, instead of !isRed())
     *
     * @return true if the node is black
     */
    public boolean isBlack() {
        return _color == BLACK;
    }

    /**
     * Set the node's parent.
     *
     * @param v the new parent
     */
    protected void setParent(RBTreeNode v) {
        _p = v;
    }

    /**
     * Set the node's left son.
     *
     * @param v the new left son
     */
    protected void setLeftSon(RBTreeNode v) {
        _left = v;
    }

    /**
     * Set the node's right son.
     *
     * @param v the new right son
     */
    protected void setRightSon(RBTreeNode v) {
        _right = v;
    }

    /**
     * Turn the node red.
     */
    public void turnRed() {
        _color = RED;
    }

    /**
     * Turn the node black.
     */
    public void turnBlack() {
        _color = BLACK;
    }
}