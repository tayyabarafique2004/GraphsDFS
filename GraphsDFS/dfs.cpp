#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <cstdlib> // For system("CLS")
using namespace std;

struct Vertex {
    int value;
    Vertex* next;
    struct Edge* edges;
    bool visited;
};

struct Edge {
    Vertex* connectedVertex;
    Edge* next;
};

class Graph {
    Vertex* head;
    vector<vector<int>> adjMatrix;
    int vertexCount;

public:
    Graph();
    void insertVertex(int value);
    void deleteVertex(int value);
    void insertEdge(int from, int to);
    void deleteEdge(int from, int to);
    void AdjacentVertex(int value);
    bool isEmpty();
    Vertex* findVertex(int value);
    void DFS(int startValue);
    void resetVisited();
    void displayGraph();
    void displayAdjMatrix();
    void clearScreen();

private:
    int findVertexIndex(int value);
    void updateAdjMatrixOnDeletion(int index);
};

Graph::Graph() : head(nullptr), vertexCount(0) {}

void Graph::insertVertex(int value) {
    if (findVertex(value)) {
        cout << "Vertex " << value << " already exists!\n";
        return;
    }

    Vertex* newVertex = new Vertex();
    newVertex->value = value;
    newVertex->next = nullptr;
    newVertex->edges = nullptr;
    newVertex->visited = false;

    if (!head) {
        head = newVertex;
    }
    else {
        Vertex* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newVertex;
    }

    adjMatrix.push_back(vector<int>(vertexCount, 0));
    for (auto& row : adjMatrix) {
        row.push_back(0);
    }
    vertexCount++;

    cout << "Vertex " << value << " added successfully.\n";
}

void Graph::deleteVertex(int value) {
    if (isEmpty()) {
        cout << "Graph is empty. No vertices to delete.\n";
        return;
    }

    int index = findVertexIndex(value);
    if (index == -1) {
        cout << "Vertex " << value << " not found.\n";
        return;
    }

    Vertex* current = head;
    Vertex* prev = nullptr;

    while (current && current->value != value) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        cout << "Vertex " << value << " not found.\n";
        return;
    }

    Vertex* temp = head;
    while (temp) {
        deleteEdge(temp->value, value);
        temp = temp->next;
    }

    if (prev) {
        prev->next = current->next;
    }
    else {
        head = current->next;
    }
    delete current;

    updateAdjMatrixOnDeletion(index);
    vertexCount--;

    cout << "Vertex " << value << " deleted successfully.\n";
}

void Graph::insertEdge(int source, int destination) {
    Vertex* sourceVertex = findVertex(source);
    Vertex* destinationVertex = findVertex(destination);

    if (!sourceVertex) {
        cout << "Source vertex " << source << " not found!\n";
        return;
    }
    if (!destinationVertex) {
        cout << "Destination vertex " << destination << " not found!\n";
        return;
    }

    Edge* newEdge = new Edge{ destinationVertex, nullptr };
    if (!sourceVertex->edges) {
        sourceVertex->edges = newEdge;
    }
    else {
        Edge* temp = sourceVertex->edges;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newEdge;
    }

    int sourceIndex = findVertexIndex(source);
    int destIndex = findVertexIndex(destination);
    adjMatrix[sourceIndex][destIndex] = 1;

    cout << "Edge added between " << source << " and " << destination << ".\n";
}

void Graph::deleteEdge(int source, int destination) {
    if (isEmpty()) {
        cout << "Graph is empty. No edges to delete.\n";
        return;
    }

    Vertex* sourceVertex = findVertex(source);

    if (!sourceVertex) {
        cout << "Source vertex " << source << " not found!\n";
        return;
    }

    Edge* current = sourceVertex->edges;
    Edge* prev = nullptr;

    while (current && current->connectedVertex->value != destination) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        cout << "Edge between " << source << " and " << destination << " not found.\n";
        return;
    }

    if (prev) {
        prev->next = current->next;
    }
    else {
        sourceVertex->edges = current->next;
    }
    delete current;

    int sourceIndex = findVertexIndex(source);
    int destIndex = findVertexIndex(destination);
    adjMatrix[sourceIndex][destIndex] = 0;

    cout << "Edge between " << source << " and " << destination << " deleted successfully.\n";
}

void Graph::AdjacentVertex(int value) {
    if (isEmpty()) {
        cout << "Graph is empty.\n";
        return;
    }

    Vertex* vertex = findVertex(value);

    if (!vertex) {
        cout << "Vertex " << value << " not found.\n";
        return;
    }

    cout << "Adjacent vertices of " << value << ": ";
    Edge* edge = vertex->edges;
    while (edge) {
        cout << edge->connectedVertex->value << " ";
        edge = edge->next;
    }
    cout << endl;
}

bool Graph::isEmpty() {
    return head == nullptr;
}

Vertex* Graph::findVertex(int value) {
    Vertex* temp = head;
    while (temp) {
        if (temp->value == value) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

void Graph::DFS(int startValue) {
    if (isEmpty()) {
        cout << "Graph is empty. No vertices to traverse.\n";
        return;
    }

    Vertex* startVertex = findVertex(startValue);
    if (!startVertex) {
        cout << "Starting vertex " << startValue << " not found!\n";
        return;
    }

    stack<Vertex*> s;
    s.push(startVertex);

    cout << "DFS Traversal: ";
    while (!s.empty()) {
        Vertex* current = s.top();
        s.pop();

        if (!current->visited) {
            cout << current->value << " ";
            current->visited = true;

            Edge* edgeTemp = current->edges;
            while (edgeTemp) {
                if (!edgeTemp->connectedVertex->visited) {
                    s.push(edgeTemp->connectedVertex);
                }
                edgeTemp = edgeTemp->next;
            }
        }
    }
    cout << endl;

    resetVisited();
}

void Graph::resetVisited() {
    Vertex* temp = head;
    while (temp) {
        temp->visited = false;
        temp = temp->next;
    }
}

void Graph::displayGraph() {
    if (isEmpty()) {
        cout << "Graph is empty.\n";
        return;
    }

    Vertex* temp = head;
    cout << "Graph adjacency list:\n";
    while (temp) {
        cout << temp->value << ": ";
        Edge* edgeTemp = temp->edges;
        while (edgeTemp) {
            cout << edgeTemp->connectedVertex->value << " ";
            edgeTemp = edgeTemp->next;
        }
        cout << endl;
        temp = temp->next;
    }
}

void Graph::displayAdjMatrix() {
    if (isEmpty()) {
        cout << "Graph is empty. No adjacency matrix to display.\n";
        return;
    }

    cout << "Adjacency Matrix:\n  ";
    Vertex* temp = head;
    while (temp) {
        cout << temp->value << " ";
        temp = temp->next;
    }
    cout << endl;

    int rowIndex = 0;
    temp = head;
    while (temp) {
        cout << temp->value << " ";
        for (int val : adjMatrix[rowIndex]) {
            cout << val << " ";
        }
        cout << endl;
        temp = temp->next;
        rowIndex++;
    }
}

int Graph::findVertexIndex(int value) {
    Vertex* temp = head;
    int index = 0;
    while (temp) {
        if (temp->value == value) {
            return index;
        }
        temp = temp->next;
        index++;
    }
    return -1;
}

void Graph::updateAdjMatrixOnDeletion(int index) {
    adjMatrix.erase(adjMatrix.begin() + index);
    for (auto& row : adjMatrix) {
        row.erase(row.begin() + index);
    }
}

void Graph::clearScreen() {
    system("CLS");
}

int getValidInt() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter an integer: ";
    }
    return value;
}

int main() {
    Graph g;
    int choice, value, source, destination, startValue;

    do {
        g.clearScreen();
        cout << "\nMenu:\n";
        cout << "1. Insert Vertex\n";
        cout << "2. Delete Vertex\n";
        cout << "3. Insert Edge\n";
        cout << "4. Delete Edge\n";
        cout << "5. Display Adjacent Vertices\n";
        cout << "6. Check if Graph is Empty\n";
        cout << "7. Display Graph\n";
        cout << "8. Display Adjacency Matrix\n";
        cout << "9. Perform DFS\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        choice = getValidInt();

        switch (choice) {
        case 1:
            cout << "Enter vertex value: ";
            value = getValidInt();
            g.insertVertex(value);
            break;
        case 2:
            if (g.isEmpty()) {
                cout << "Graph is empty. No vertices to delete.\n";
                break;
            }
            cout << "Enter vertex value to delete: ";
            value = getValidInt();
            g.deleteVertex(value);
            break;
        case 3:
            if (g.isEmpty()) {
                cout << "Graph is empty. Cannot insert edges.\n";
                break;
            }
            cout << "Enter source vertex of edge: \n";
            source = getValidInt();
            cout << "Enter destination vertex of edge: \n";
            destination = getValidInt();
            g.insertEdge(source, destination);
            break;
        case 4:
            if (g.isEmpty()) {
                cout << "Graph is empty. No edges to delete.\n";
                break;
            }
            cout << "Enter source vertex of edge you want to delete: ";
            source = getValidInt();
            cout << "Enter destination vertex of edge you want to delete: ";
            destination = getValidInt();
            g.deleteEdge(source, destination);
            break;
        case 5:
            if (g.isEmpty()) {
                cout << "Graph is empty. No adjacent vertices to display.\n";
                break;
            }
            cout << "Enter vertex to find adjacent vertices: ";
            value = getValidInt();
            g.AdjacentVertex(value);
            break;
        case 6:
            cout << (g.isEmpty() ? "Graph is empty." : "Graph is not empty.") << endl;
            break;
        case 7:
            g.displayGraph();
            break;
        case 8:
            g.displayAdjMatrix();
            break;
        case 9:
            if (g.isEmpty()) {
                cout << "Graph is empty. DFS cannot be performed.\n";
                break;
            }
            cout << "Enter starting vertex for DFS: ";
            startValue = getValidInt();
            g.DFS(startValue);
            break;
        case 10:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
        if (choice != 10) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 10);

    return 0;
}
