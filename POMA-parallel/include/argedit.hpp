/**
 * @file   argedit.hpp
 * @author P. Foggia (pfoggia\@unisa.it)
 * @author V.Carletti (vcarletti\@unisa.it)
 * @date   December, 2014
 * @brief  Definition of a simple ARG loader which allows graph edit operations
 * @see argraph.hpp
 */

#ifndef ARGEDIT_H
#define ARGEDIT_H

#include "argraph.hpp"

/**
 * @class ARGEdit
 * @brief A simple ARGLoader providing graph edit operations. 
 *	Nodes are stored as a linked list and each node stores the list of its out going edges.
 * @note The ARGEdit does not make provisions for the
 *       deallocation of the attributes, which must be
 *       dealt with by the programmer.
 */
template<typename Node, typename Edge>
class ARGEdit : public ARGLoader<Node, Edge> {
public:

    ARGEdit();

    ARGEdit(ARGraph<Node, Edge> &g);

    ARGEdit(ARGLoader<Node, Edge> &g);

    ~ARGEdit();

    /* Redefined ARGLoader methods */
    virtual int NodeCount();

    virtual Node GetNodeAttr(node_id node);

    virtual int OutEdgeCount(node_id node);

    virtual node_id GetOutEdge(node_id node, int i, Edge *pattr);

    /* Graph edit operations */
    node_id InsertNode(Node &attr);

    void InsertEdge(node_id n1, node_id n2, Edge &attr);

    void DeleteNode(node_id n);

    void DeleteEdge(node_id n1, node_id n2);

protected:
    int count; /**<Number of nodes */

    /**
    * @brief Structure of an edge used from the ARGEdit.
    * @note Edges are stored as a linked list.
    */
    struct eNode {
        node_id from;    /**<Start node id */
        node_id to;    /**<End node id */
        int pos;        /**<Current position */
        Edge attr;    /**<Edge attribute */
        eNode *next;    /**<Next edge structure of the list */
    };

    /**
    * @brief Structure of a node used from the ARGEdit.
    * @note Node are stored as a linked list.
    */
    struct nNode {
        node_id id;    /**<Node id */
        int count;    /**<Number of edges out going from the node */
        Node attr;    /**<Attribute of the node */
        nNode *next;    /**<Next node structure of the list*/
        eNode *edges;    /**<List of out going edges of the node */
    };

    nNode *nodes;        /**<List of nodes */
    nNode *lastNode;    /**<Last inserted node */
    eNode *lastEdge;    /**<Last inserted edge */
};

/**
* @brief   Simple constructor for the ARGEdit
*/
template<typename Node, typename Edge>
ARGEdit<Node, Edge>::ARGEdit() {
    count = 0;
    nodes = NULL;
    lastNode = NULL;
    lastEdge = NULL;
}

/**
* @brief   Construct an ARGEdit from an existing ARGraph
*
* @param [in] g  ARGraph
*/
template<typename Node, typename Edge>
ARGEdit<Node, Edge>::ARGEdit(ARGraph<Node, Edge> &g) {
    count = 0;
    nodes = NULL;
    lastNode = NULL;
    lastEdge = NULL;

    node_id n;
    for (n = 0; n < g.NodeCount(); n++)
        InsertNode(g.GetNodeAttr(n));
    int i;
    for (n = 0; n < g.NodeCount(); n++) {
        for (i = 0; i < g.OutEdgeCount(n); i++) {
            void *attr;
            node_id n2 = g.GetOutEdge(n, i, &attr);
            InsertEdge(n, n2, attr);
        }
    }
}

/**
* @brief   Construct an ARGEdit from an existing ARGLoader
*
* @param [in] g  ARGLoader
*/
template<typename Node, typename Edge>
ARGEdit<Node, Edge>::ARGEdit(ARGLoader<Node, Edge> &g) {
    count = 0;
    nodes = NULL;
    lastNode = NULL;
    lastEdge = NULL;

    node_id n;
    for (n = 0; n < g.NodeCount(); n++)
        InsertNode(g.GetNodeAttr(n));
    int i;
    for (n = 0; n < g.NodeCount(); n++) {
        for (i = 0; i < g.OutEdgeCount(n); i++) {
            Edge *attr;
            node_id n2 = g.GetOutEdge(n, i, &attr);
            InsertEdge(n, n2, attr);
        }
    }
}


/**
* @brief Destructor
*/
template<typename Node, typename Edge>
ARGEdit<Node, Edge>::~ARGEdit() {
    nNode *pn, *pn1;
    eNode *pe, *pe1;

    pn = nodes;
    while (pn != NULL) {
        pe = pn->edges;
        while (pe != NULL) {
            pe1 = pe;
            pe = pe->next;
            delete pe1;
        }
        pn1 = pn;
        pn = pn->next;
        delete pn1;
    }
}

/**
* @brief Count the number of nodes
* @returns Number of loaded nodes 
*/
template<typename Node, typename Edge>
int ARGEdit<Node, Edge>::NodeCount() {
    return count;
}

/**
* @brief Returns the attribute of a node.
* @param [in] id Node Id
* @returns The attribute of a node. 
*/
template<typename Node, typename Edge>
Node ARGEdit<Node, Edge>::GetNodeAttr(node_id id) {
    nNode *n = nodes;

    if (lastNode != NULL && lastNode->id <= id)
        n = lastNode;

    while (n != NULL && n->id != id)
        n = n->next;
    if (n == NULL)
        error("Inconsistent data");
    return n->attr;
}

/**
* @brief Returns the number of edges coming out of a node.
* @param [in] id Node Id
* @returns Number of edges coming out from the node. 
*/
template<typename Node, typename Edge>
int ARGEdit<Node, Edge>::OutEdgeCount(node_id id) {
    nNode *n = nodes;

    if (lastNode != NULL && lastNode->id <= id)
        n = lastNode;

    while (n != NULL && n->id != id)
        n = n->next;
    if (n == NULL)
        error("Inconsistent data");
    return n->count;
}

/**
* @brief Returns the i-th edge coming out from a given node
* @param [in] id Node Id
* @param [in] i Index of the edge 
* @param [out] pattr Attribute of the edge
* @returns Node id of the other end point. 
*/
template<typename Node, typename Edge>
node_id ARGEdit<Node, Edge>::GetOutEdge(node_id id, int i, Edge *pattr) {
    nNode *n = nodes;

    if (lastNode != NULL && lastNode->id <= id)
        n = lastNode;

    while (n != NULL && n->id != id)
        n = n->next;
    if (n == NULL)
        error("Inconsistent data");

    eNode *e = n->edges;
    int pos = 0;

    if (lastEdge != NULL && lastEdge->from == id &&
        lastEdge->pos >= 0 && lastEdge->pos <= i) {
        e = lastEdge;
        pos = e->pos;
    }

    while (e != NULL && pos < i) {
        e->pos = pos;
        e = e->next;
        pos++;
    }

    if (e == NULL)
        error("Inconsistent data");

    if (pattr != NULL)
        *pattr = e->attr;
    lastNode = n;
    lastEdge = e;

    return e->to;
}


/**
* @brief Insert a new node
* @param [in] attr Attribute of the node.
* @returns Node id of the inserted node. 
*/
template<typename Node, typename Edge>
node_id ARGEdit<Node, Edge>::InsertNode(Node &attr) {
    nNode *n = new nNode;
    if (n == NULL)
        error("Out of memory");
    node_id id = n->id = count++;
    n->attr = attr;
    n->edges = NULL;
    n->count = 0;

    nNode *p = nodes, *p0 = NULL;
    if (lastNode != NULL && lastNode->id < id) {
        p0 = lastNode;
        p = lastNode->next;
    }
    while (p != NULL && p->id < id) {
        p0 = p;
        p = p->next;
    }


    if (p0 == NULL) {
        n->next = nodes;
        nodes = n;
    } else {
        n->next = p0->next;
        p0->next = n;
    }
    lastNode = n;

    return n->id;
}

/**
* @brief Insert a new edge
* @param [in] id1 Start node of the edge.
* @param [in] id2 End node of the edge.
* @param [in] attr Attribute of the edge.
*/
template<typename Node, typename Edge>
void ARGEdit<Node, Edge>::InsertEdge(node_id id1, node_id id2, Edge &attr) {
    eNode *e = new eNode;

    if (e == NULL)
        error("Out of memory");

    e->from = id1;
    e->to = id2;
    e->attr = attr;
    e->pos = -1;
    nNode *pn = nodes;
    if (lastNode != NULL && lastNode->id <= id1) {
        pn = lastNode;
    }
    while (pn != NULL && pn->id < id1) {
        pn = pn->next;
    }
    if (pn == NULL || pn->id != id1)
        error("Bad param 1 in ARGEdit::InsertEdge: %d", (int) id1);

    eNode *p = pn->edges, *p0 = NULL;
    if (lastEdge != NULL && lastEdge->from == id1 && lastEdge->to < id2) {
        p0 = lastEdge;
        p = lastEdge->next;
    }
    while (p != NULL && p->to < id2) {
        p0 = p;
        p = p->next;
    }

    if (p != NULL && p->to == id2)
        error("Bad param 2 in ARGEdit::InsertEdge: %d", (int) id2);

    if (p0 == NULL) {
        e->next = pn->edges;
        pn->edges = e;
    } else {
        e->next = p0->next;
        p0->next = e;
    }

    pn->count++;
    lastNode = pn;
    lastEdge = e;
}


/**
* @brief Delete a node.
* @param [in] id Node id to delete
*/
template<typename Node, typename Edge>
void ARGEdit<Node, Edge>::DeleteNode(node_id id) {
    nNode *p = nodes, *p0 = NULL;

    if (lastNode != NULL && lastNode->id < id) {
        p0 = lastNode;
        p = lastNode->next;
    }
    while (p != NULL && p->id < id) {
        p0 = p;
        p = p->next;
    }

    if (p == NULL || !p->id == id)
        error("Bad param in ARGEdit::DeleteNode");

    while (p->edges != NULL) {
        eNode *ep = p->edges;
        p->edges = ep->next;
        delete ep;
    }
    lastEdge = NULL;
    delete p->attr;
    if (p0 == NULL) {
        nodes = p->next;
    } else {
        p0->next = p->next;
    }
    delete p;
    count--;

    p = nodes;
    while (p != NULL) {
        if (p->id > id)
            p->id--;
        eNode *ep0 = NULL;
        eNode *ep = p->edges;
        while (ep != NULL) {
            if (ep->to == id) {
                if (ep0 == NULL)
                    p->edges = ep->next;
                else
                    ep0->next = ep->next;
                delete ep;
                p->count--;
                if (ep0 == NULL)
                    ep = p->edges;
                else
                    ep = ep0->next;
                continue;
            }
            if (ep->from > id)
                ep->from--;
            if (ep->to > id)
                ep->to--;
            ep0 = ep;
            ep = ep->next;
        }
        p = p->next;
    }

    lastNode = p0;
}

/**
* @brief Delete an edge.
* @param [in] id1 Start node id
* @param [in] id2 End node id
*/
template<typename Node, typename Edge>
void ARGEdit<Node, Edge>::DeleteEdge(node_id id1, node_id id2) {
    nNode *pn = nodes;

    if (lastNode != NULL && lastNode->id <= id1)
        pn = lastNode;

    while (pn != NULL && pn->id < id1)
        pn = pn->next;

    if (pn == NULL || pn->id != id1)
        error("Bad param in ARGEdit::DeleteEdge");

    eNode *pe = pn->edges, *pe0 = NULL;

    if (lastEdge != NULL && lastEdge->from == id1 && lastEdge->to < id2) {
        pe0 = lastEdge;
        pe = pe0->next;
    }

    while (pe != NULL && pe->to < id2) {
        pe0 = pe;
        pe = pe->next;
    }

    if (pe == NULL || pe->to != id2)
        error("Bad param in ARGEdit::DeleteEdge");

    if (pe0 == NULL)
        pn->edges = pe->next;
    else
        pe0->next = pe->next;
    delete pe;
    pn->count--;

    lastNode = pn;
    lastEdge = pe0;
}

#endif
