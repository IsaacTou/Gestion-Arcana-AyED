#include <iostream>
#include <fstream>
using namespace std;



template <typename T>
class Node {
    public:
        T payload;
        Node<T>* prev;
        Node<T>* next;

        Node(T v) {
            payload = v;
            prev = nullptr;
            next = nullptr;
        }
        Node(T *v) {
            payload = *v;
            prev = nullptr;
            next = nullptr;
        }
};

template <typename T>
class List
{
    private:
        Node<T>* _first;
        Node<T>* _last;

    public:
        List() {
            _first = _last = nullptr;
        }

        /*~List() {
            while (!isEmpty()) {
            remove(first());
            }
        }*/

        bool isEmpty() {
            return _first == nullptr;
        }

        T* get(Node<T>* p) {
            if (p == nullptr) return nullptr;
            return &(p->payload);
        }

        void next(Node<T>* &p) {
            if (p == nullptr) return;
            p = p->next;
        }
        void prev(Node<T>* &p) {
            if (p == nullptr) return;
            p = p->prev;
        }
        
        Node<T>* first() {
            return _first;
        }
        
        Node<T>* last() {
            return _last;
        }
        
        void preInsert(Node<T> *p, T v) {
            Node<T>* newnode = new Node<T>(v);

            if (_first == p) _first = newnode;
            if (_last == nullptr) _last = newnode;

            newnode->next = p;
            if (p != nullptr) {
                newnode->prev = p->prev;
                if (p->prev != nullptr) p->prev->next = newnode;
                p->prev = newnode;
            }
        }

        void postInsert(Node<T>* p, T v) {
            Node<T>* newnode = new Node<T>(v);

            if (_last == p) _last = newnode;
        
            if (_first == nullptr) _first = newnode;
            
            newnode->prev = p;
            if (p != nullptr) {
                newnode->next = p->next;
                if (p->next != nullptr) p->next->prev = newnode;
                p->next = newnode;
            }
        }

        void remove(Node<T>* p) {
            if (p == nullptr) return;

            if (p->prev != nullptr) p->prev->next = p->next;
            if (p->next != nullptr) p->next->prev = p->prev;

            if (p == _first) _first = p->next;
            if (p == _last) _last = p->prev;
        
            delete p;
        }

        void printNodos(){
            Node<T> *p = _first;
            while (p != nullptr){
                cout << p->payload << " ";
                p = p->next;
            }
            cout << endl;
        }
};



class MagicPoint;

class MagicLine //Arista
{
public:
    float Power;
    MagicPoint*point1;
    MagicPoint*point2;
    MagicLine(float power, MagicPoint *p1, MagicPoint *p2): Power(power), point1(p1), point2(p2){};
    MagicLine() : Power(0), point1(nullptr), point2(nullptr) {}
};


class MagicPoint //Vertice
{
public:
    int pointID; 
    char magicRune;
    List <MagicLine> lines;
                                  
    MagicPoint(int pointID, char magicRune): pointID(pointID), magicRune(magicRune){};

    void addMagicLine(float power, MagicPoint* point2){
        MagicLine* newLine = new MagicLine (power, this, point2);
        lines.postInsert(lines.last(),*newLine);
    }

    void castMagicLine(){
        Node <MagicLine>* iterator = lines.first();
        while (iterator != nullptr){
            cout << "Power " << iterator->payload.Power << " Entre " << iterator->payload.point1->pointID <<
            " and " << iterator->payload.point2->pointID << endl;
            lines.next(iterator);
        }
    }

    ~MagicPoint() {
        Node<MagicLine>* iterator = lines.first();
        while (iterator != nullptr) {
            Node<MagicLine>* toDelete = iterator;
            lines.next(iterator);
            delete toDelete; 
        }
    }
};


class Spell  //Grafo
{
public:
    int magicPointCount;
    string name;
    string runes;
    List<MagicPoint*> MagicPoints;                           

    Spell(): name(""), runes(""){};
    Spell(string name, string runes, int magicPointCount): name(name), runes(runes),magicPointCount(magicPointCount){};

    bool isSpecialRune(char rune){
        return rune == 'I' || rune == 'Q' || rune == 'T' || rune == 'V' || rune == 'L'||
               rune == 'O' || rune == 'A' || rune == 'D' || rune == 'F';
    }


    void addMagicPoint(int pointID, char magicRune){
        MagicPoint* v = new MagicPoint(pointID, magicRune);
        MagicPoints.postInsert(MagicPoints.last(), v);
    }

    void addMagicLine(float power, MagicPoint *v1, MagicPoint *v2){
        v1->addMagicLine(power, v2);
        v2->addMagicLine(power, v1);
    }

    void mostrarGrafos() {
        Node<MagicPoint*>* iterator = MagicPoints.first();
        while (iterator != nullptr){
            iterator->payload->castMagicLine();
            MagicPoints.next(iterator);
        }
    }

    MagicPoint* getMagicPoint(int index) {
        Node<MagicPoint*> *iterator = MagicPoints.first();
        for (int i = 1; i < index; ++i) {
            if (iterator == nullptr) {
                return nullptr;
            };
            MagicPoints.next(iterator);
        }
        return iterator != nullptr ? iterator->payload : nullptr;
    }


    void findPathPerPower(int index, MagicPoint* iterator, bool visited[], int& totalMagicPoints, int& totalLine, float& totalPower, int visitedOrder[]) {
        if (index == magicPointCount-1){
            cout << "Camino encontrado." << endl;
            cout << "Total de Vertices visitados: " << totalMagicPoints << endl;
            cout << "Total de aristas visitados: " << totalLine << endl; 
            cout << "Total Poder utilizado: " << totalPower <<  endl;
            cout << "Orden de visitados: ";
            for (int i = 0; i < magicPointCount; i++){
                cout << visitedOrder[i] << " ";
            }
            cout << endl;
            return;
        }

        visited[iterator->pointID -1] = true;
        totalMagicPoints++;
        visitedOrder[index] = iterator->pointID;

        Node<MagicLine>* lineIterator = iterator->lines.first();
        while (lineIterator != nullptr){
            MagicPoint* nextPoint = lineIterator->payload.point2;
            if (!visited[nextPoint->pointID-1]){
                totalLine++;
                totalPower += lineIterator->payload.Power;
                findPathPerPower(index+1,nextPoint,visited,totalMagicPoints,totalLine,totalPower,visitedOrder);
                if(index == magicPointCount-1){
                    return;
                }
                totalLine--;
                totalPower -= lineIterator->payload.Power;
            }
            iterator->lines.next(lineIterator);
        }

        visited[iterator->pointID-1] = false;
        totalMagicPoints--;   
    }

    void findPathPerPower() {
        int visitedOrder[magicPointCount] = {0};
        bool visited[magicPointCount] = {false};
        int totalVertices = 0;
        int totalEdges = 0;
        float totalPower = 0.0;
    
        MagicPoint* startPoint = getMagicPoint(1);
        if (startPoint != nullptr) {
            findPathPerPower(0, startPoint, visited, totalVertices, totalEdges, totalPower, visitedOrder);
        }
    }


    void findLongestCycleUtil(MagicPoint* start, MagicPoint* current, bool visited[], int& maxEdges, int currentEdges, MagicLine* parentEdge, int currentOrder[], int bestOrder[]) {
        visited[current->pointID - 1] = true;
        currentOrder[currentEdges] = current->pointID;

        Node<MagicLine>* lineIterator = current->lines.first();
        while (lineIterator != nullptr) {
            MagicPoint* nextPoint = lineIterator->payload.point2;
            if (nextPoint == start && currentEdges > maxEdges) {
                maxEdges = currentEdges;
                for (int i = 0; i <= maxEdges; ++i) {
                    bestOrder[i] = currentOrder[i];
                }
            } else if (!visited[nextPoint->pointID - 1]) {
                findLongestCycleUtil(start, nextPoint, visited, maxEdges, currentEdges + 1, &lineIterator->payload, currentOrder, bestOrder);
            }
            current->lines.next(lineIterator);
        }

        visited[current->pointID - 1] = false;
    }

    int findLongestCycle(int bestOrder[]) {
        bool visited[magicPointCount] = {false};
        int maxEdges = 0;
        int currentOrder[magicPointCount] = {0};

        Node<MagicPoint*>* iterator = MagicPoints.first();
        while (iterator != nullptr) {
            findLongestCycleUtil(iterator->payload, iterator->payload, visited, maxEdges, 0, nullptr, currentOrder, bestOrder);
            MagicPoints.next(iterator);
        }

        return maxEdges;
    }

    void findLongestPathUtil(MagicPoint* iterator, bool visited[], float &maxPower, float currentPower, int currentOrder[], int bestOrder[], int &maxLines, int currentLines) {
        visited[iterator->pointID - 1] = true;                                                              //Marcamos True con la ayuda del pointID (numero del vertice) y le restamos 1 para que concuerde con las posiciones del arreglo
        currentOrder[currentLines] = iterator->pointID;                                                     //Al arreglo de orden le asignamos el numero del vertice que estamos trabajando la primera iteracion "currentLines=0"              

        if (isSpecialRune(iterator->magicRune)) {                                                           //Aca buscamos a ver si el la runa de Iterador es una de las runas especial(Elemental,Estabilidad,catalitica) 
            if (currentPower > maxPower) {                                                                  //Preguntamos si el poder q llevamos actualmente es mayor a MaxPower 
                maxPower = currentPower;                                                                    //Si esto es correcto actualizamos el MaxPower ya que hemos conseguido una nueva ruta con mas poder
                maxLines = currentLines;
                for (int i = 0; i <= maxLines; ++i) {
                    bestOrder[i] = currentOrder[i];
                }
            }
        }

        Node<MagicLine>* lineIterator = iterator->lines.first();                                            //Creamos un Nodo de tipo MagicLine (arista) y le asignamos la lista de lineas que tiene nuesto Vertice actual                
        while (lineIterator != nullptr) {                                                                   //Mientras nuestro Nodo de MagicLine no sea Vacio iteramos
            MagicPoint* nextPoint = lineIterator->payload.point2;                                           //Creamos un puntero de tipo MagicPoint (Vertice) y le vamos a asignar el puntero de Point2 que esto no es mas que otro Vertice al cual nos queremos mover.
            if (!visited[nextPoint->pointID - 1]) {                                                         //Revisamos el ID del NextPint el cual nos dara un numero y este numero sera el indice de Visited y preguntamos si No lo hemos visitado entonces entramos en el if
                findLongestPathUtil(nextPoint, visited, maxPower, currentPower + lineIterator->payload.Power,
                                    currentOrder, bestOrder, maxLines, currentLines + 1);
            }
            iterator->lines.next(lineIterator);
        }
        visited[iterator->pointID - 1] = false;
    }

    float findLongestPath(int startID, int bestOrder[], int &maxEdges) {
        bool visited[magicPointCount] = {false};                                                            //Arreglo de boleanos para saber si el Vertice fue visitado
        float maxPower = 0.0;                                                                               //Total del poder que vamos a acumular
        int currentOrder[magicPointCount] = {0};                                                            //Orden en el cual visitamos el Grafo (Efecto Practico)

        MagicPoint* startPoint = getMagicPoint(startID);                                                    //Creamos un Vertice y lo igualamos al Vertice #1 de nuestra lista
        if (startPoint != nullptr) {                                                                        //Si el Vertice q acabamos se crear no se le asigno un punto nulo entramos
            findLongestPathUtil(startPoint, visited, maxPower, 0.0, currentOrder, bestOrder, maxEdges, 0);  //Pasamos nuestro Vertice junto con otras variables que nos ayudaran,
        }

        return maxPower;                                                                                    //retornamos el resultado obtenido.
    }

    ~Spell() {
        Node<MagicPoint*>* iterator = MagicPoints.first();
        while (iterator != nullptr) {
            delete iterator->payload; 
            MagicPoints.next(iterator);
        }
    }
};



class SpellDetector
{
private:
    Spell **spells;
    int spellCount;
public:
    SpellDetector(): spellCount(0), spells(nullptr){};

    void readSpell(){
        ifstream papyrus ("spellList.in");
        if (!papyrus.is_open()){
            return;
        }
        string line, name, rune;
        int spellCount, vertexCount, edgeCount;
        
        papyrus >> spellCount;
        spells = new Spell *[spellCount];

        for (int i = 0; i<spellCount; i++){
            papyrus.ignore();
            getline(papyrus,name);

            papyrus >> vertexCount;
            papyrus >> rune;
            papyrus >> edgeCount;
 
            spells[i] = new Spell(name, rune, vertexCount);

            for (int j = 1; j <= vertexCount; j++){
                spells[i]->addMagicPoint(j, rune[j-1]); 
            }

            for (int j = 0; j < edgeCount; j++){
                int v1, v2;
                float power;
                papyrus >> v1 >> v2 >> power;
                MagicPoint* point1 = spells[i]->getMagicPoint(v1);
                MagicPoint* point2 = spells[i]->getMagicPoint(v2);

                spells[i]->addMagicLine(power,point1,point2);
            }
            this->spellCount++;
        } 
        papyrus.close();
 
    }

    void printLongestCycle() {
        int bestOrder[spells[0]->magicPointCount] = {0};
        int longestCycle = spells[0]->findLongestCycle(bestOrder);
        cout << "El ciclo simple mas largo tiene " << longestCycle << " aristas." << endl;
        cout << "Orden de visitados: ";
        for (int i = 0; i <= longestCycle; ++i) {
            cout << bestOrder[i] << " ";
        }
        cout << endl;
    }
    void printLongestPath() {
        int bestOrder[spells[0]->magicPointCount] = {0};
        int maxEdges = 0;
        float longestPath = spells[0]->findLongestPath(1, bestOrder, maxEdges); 
        cout << "El camino mas largo tiene una ponderacion de " << longestPath << " y " << maxEdges << " aristas." << endl;
        cout << "Orden de visitados: ";
        for (int i = 0; i < maxEdges; ++i) {
            cout << bestOrder[i] << " ";
        }
        cout << endl;
    }

    void printMostLarge(){
        spells[0]->findPathPerPower();
    }    

    void printGraf(){
        for (int i = 0; i < spellCount; i++){
            spells[i]->mostrarGrafos();
            cout << "Siguiente " << endl;
        }
    }


    ~SpellDetector() {
        for (int i = 0; i < spellCount; i++) {
            delete spells[i];  
        }
        delete[] spells;
    }
};




int main (){
    SpellDetector a1;    
    a1.readSpell();
    a1.printLongestCycle();
    a1.printLongestPath();



    return 0;
}