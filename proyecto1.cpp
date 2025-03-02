#include <iostream>
#include <fstream>
using namespace std;

////init Estructura de las listas y pilas

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
template <typename T>
class Stack : List<T> {
    public:
        bool isEmpty() {
            return List<T>::isEmpty();
        }
        T* first() {
            if (isEmpty()) return nullptr;
            return &List<T>::first()->payload;
        }
        T pop() {
            T x = List<T>::first()->payload;
            List<T>::remove(List<T>::first());
            return x;
        }
        void stack_push(T v) {
            List<T>::preInsert(List<T>::first(), v);
        }
};

////End Estructura de listas y pilas


////Init Estructura de los magos y sus hechizos

struct MagicSpell
{
    string name;                                                         //Nombre del Hechizo
    int type;                                                            //Entero que dice q tipo de hechizo es
    bool isLegal = false;                                                //siempre se inicia como !legal y se cambiara luego
};

class Mage
{
private:
    string name;
    List<MagicSpell> spells;
    int id;
    int spellCount;
    int ilegalityCount;
    bool underInvestigation = false;    
public:
    
    ////Init Constructores

    Mage(string name, int id, bool ilegalyCount):name(name), spellCount(0), id(0){                                                         //Se usa para un mago que tenemos que evaluarle sus hechizos.
        if (ilegalityCount) this->ilegalityCount++;
        if (ilegalityCount >= 3){
            this->underInvestigation = true;
        }
    };    
    Mage(string name)
    :name(name), spellCount(0), id(0),ilegalityCount(0),underInvestigation(true){};  //Se usa para un mago que hayamos encontrado en la lista de underInvestigation.
          
    ////End Constructores 

    //// Init Funciones

    void addSpell(string name, int type, bool isLegal, int id){                      //metodo para agregar hechizos.
        MagicSpell* newSpell = new MagicSpell;                                      
        newSpell->name = name;
        newSpell->type = type;                                                       
        newSpell->isLegal = isLegal;
                                                                                    
        Node<MagicSpell>* iterator = spells.first();                                    

        while (iterator != nullptr && iterator->payload.type <= newSpell->type) {    //iteramos la lista de hechizos y comparamos el tipo de hechizo.
            spells.next(iterator);                                                   //cada vez q nos movamos iremos comparando el TypoRune el cual es un numero   
            iterator = iterator->next;                                               //cuando consigamos un numero mayor o igual al que estamos buscando                   
        }     
                                                                                     //sabremos que hemos llegaod a la posicion deseada
        spells.preInsert(iterator, *newSpell);                                       //Agregamos el hechizo a la izquierda de Iterador

        spellCount++;
    }

    ////End funciones

    ////Init Getters y Setters

    string getName(){
        return name;
    }
    int getPosition(){
        return id;
    }
    int getSpellCount(){
        return spellCount;
    }
    int getIlegalityCount(){
        return ilegalityCount;
    }
    bool getUnderInvestigation(){
        return underInvestigation;
    }
    List<MagicSpell> getSpells() {
        return spells;
    }

    ////End Getters y Setters

    ////Init Destructor

    ~Mage(){
        while (!spells.isEmpty()) {
            spells.remove(spells.first());
        }
    };

    ////Init Destructor
};

////End Estructura de los magos y sus hechizos


////Init Estructura del grafo

class MagicPoint;

class MagicLine { //Arista
public:
    float Power;
    MagicPoint*point1;
    MagicPoint*point2;
    MagicLine(float power, MagicPoint *p1, MagicPoint *p2): Power(power), point1(p1), point2(p2){};
    MagicLine() : Power(0), point1(nullptr), point2(nullptr) {}
};

class MagicPoint { //Vertice
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

class Spell  { //Grafo
public:
    int magicPointCount;
    int typeRune;   
    bool IsLegal;                                               
    string name;
    string spellname;
    string runes;
    List<MagicPoint*> MagicPoints;                           

    Spell(): name(""), runes(""), spellname(""), IsLegal(true), typeRune(typeRune){};
    
    Spell(string name, string runes, int magicPointCount, int typeRune)
    :name(name), runes(runes),magicPointCount(magicPointCount), IsLegal(true), spellname(""), typeRune(typeRune){};

    bool isSpecialRune(char rune){
        return rune == 'I' || rune == 'Q' || rune == 'T' || rune == 'V' || rune == 'L'||
               rune == 'O' || rune == 'A' || rune == 'D' || rune == 'F';
    }

    bool isElementalRune(char rune){
        return rune == 'I' || rune == 'Q' || rune == 'T' || rune == 'V' || rune == 'L'||
               rune == 'O';
    }

    bool isVocal(char letter) {
        return letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u';
    }

    string ElementalType (char rune) { 
        if (rune == 'I') return "Ignatum";                               
        if (rune == 'Q') return "Aquos";                             
        if (rune == 'T') return "Terraminium";                               
        if (rune == 'V') return "Ventus";                              
        if (rune == 'L') return "Lux";
        if (rune == 'O') return "Tenebrae";
    }

    string getLastname(int &StringSize) {
        string lastname = "";
        int i = 0;
    
        while (i < name.length()) {
            if (name[i] == ' ') {
                i++; 
                break;
            }
            i++;
        }    
        while (i < name.length()) {
            lastname += name[i++];
            StringSize++;
        }
        return lastname;
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
    
    void EnergySupport() {
        Node<MagicPoint*> *iterator = MagicPoints.first();
        while (iterator != nullptr) {
            if (iterator->payload->magicRune == 'A') {
                break; 
            } else {
                MagicPoints.next(iterator); 
            }
        }
        Node<MagicLine> *lineiterator = iterator->payload->lines.first();
        while (lineiterator != nullptr) {
            if(lineiterator->payload.point2->magicRune == 'B') {
                iterator->payload->lines.next(lineiterator);
            } else {
                this->IsLegal = false;
                break;
            }
        }
    }

    void CataliticAdyacency() {
        Node<MagicPoint*> *iterator = MagicPoints.first();
        while (iterator != nullptr) {
            if (iterator->payload->magicRune == 'D') {
                Node<MagicLine> *lineiterator = iterator->payload->lines.first();
                while (lineiterator != nullptr) {
                    if(isElementalRune(lineiterator->payload.point2->magicRune)) {
                        this->IsLegal = false;
                        break;
                    } else {
                        iterator->payload->lines.next(lineiterator);
                    }
                }
                MagicPoints.next(iterator); 
            } else {
                MagicPoints.next(iterator); 
            }
        }
    }

    void SpellName() {
        int StringSize = 0;
        Node<MagicPoint*>* iterator = MagicPoints.first();
        while (iterator != nullptr) {
            if (isElementalRune(iterator->payload->magicRune)) {
                spellname += ElementalType(iterator->payload->magicRune);
                break;
            } else {
                MagicPoints.next(iterator);
            }
        }
        
        string lastname = getLastname(StringSize);
        if (StringSize > 0 && isVocal(lastname[StringSize - 1])) {
            lastname[StringSize - 1] = 'i';
            lastname += "um";
        } else {
            lastname += "um";
        }

        spellname += lastname;

        int bestOrder[magicPointCount] = {0};
        int maxEdges = 0;
        float longestPath = findLongestPath(1, bestOrder, maxEdges);
        int longestCycle = findLongestCycle(bestOrder);
        if (longestPath >= longestCycle) {
            spellname += " modicum";
        } else if (longestPath <= longestCycle) {
            spellname += " maximus";
        } else if (longestPath == 0) {
            spellname += " Arcante";
        }
    }
  
    
    ////Init Seccion de buscadores

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
                currentEdges++;
                currentOrder[currentEdges] = nextPoint->pointID;
                maxEdges = currentEdges;
                currentEdges--;
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
        if(maxEdges % 2 != 0) this->IsLegal = false;
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
                visited[iterator->pointID - 1] = false;
                return;
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
 
    ////End Seccion de buscadores

    ////Init seccion Getters y Setters

    string getName(){
        return name;
    }
    string getNameSpell(){
        return spellname;
    }


    ////End seccion Getters y Setters



    ////init Destructor

    ~Spell() {
        Node<MagicPoint*>* iterator = MagicPoints.first();
        while (iterator != nullptr) {
            delete iterator->payload; 
            MagicPoints.next(iterator);
        }
    }

    ////End Destructor

};

////End Estructura del grafo


////Init Programa Principal

/*
class underInvestigation
{
private:
    Stack<Mage> mages;                                 //Por ahora esta clase esta en desicion si la usare o no.
public:

};
*/

class SpellDetector {
private:
    List<Mage*> Mages;
    List<string> UnderInvestigation;
    Spell **spells;
    int spellCount;
    int magesCount;
public:
    SpellDetector(): spellCount(0), magesCount(0), spells(nullptr){};

    ////Init Seccion para leer archivos

    void readUnderInvestigarion(){
        ifstream wanted ("underInvestigation.in");
        if (!wanted.is_open()){
            return;
        }
        string name;
        while (getline(wanted,name)){
            UnderInvestigation.postInsert(UnderInvestigation.last(),name);
        }

        wanted.close();
    }

    void readSpell(){
        ifstream papyrus ("spellList.in");
        if (!papyrus.is_open()){
            return;
        }
        string line, name, rune;
        int spellCount, vertexCount, edgeCount, typeRune;
        
        papyrus >> spellCount;
        spells = new Spell *[spellCount];

        for (int i = 0; i<spellCount; i++){
            papyrus.ignore();
            getline(papyrus,name);

            papyrus >> vertexCount;
            papyrus >> rune;
            papyrus >> edgeCount;
            
            typeRune = kindRune(rune);

            spells[i] = new Spell(name,rune,vertexCount,typeRune);     

            for (int j = 1; j <= vertexCount; j++){
                int countElementalRune = 0;
                int countConfluentPoint = 0;
                spells[i]->addMagicPoint(j, rune[j-1]); 
                if (spells[i]->isElementalRune(rune[j-1])) countElementalRune++;
                if (rune[j-1] == 'A') countConfluentPoint++;
                if (countConfluentPoint == 2) spells[i]->IsLegal = false;
                if (countElementalRune == 4) spells[i]->IsLegal = false;
            }

            for (int j = 0; j < edgeCount; j++){
                int v1, v2;
                float power;
                papyrus >> v1 >> v2 >> power;
                MagicPoint* point1 = spells[i]->getMagicPoint(v1);
                MagicPoint* point2 = spells[i]->getMagicPoint(v2);

                spells[i]->addMagicLine(power,point1,point2);
            }

            ////Init Seccion de ilegalidad de hechizo

            spells[i]->EnergySupport();
            spells[i]->CataliticAdyacency();
            spells[i]->SpellName();

            ////End seccion de ilegalidad de hechizo


            ////Init Seccion para agregar los hechizos a los magos

            int position = 0;

            if(mageIsInTheParty(name, position)){                                                       
                Node<Mage*>* iterator = Mages.first();                      // 1, 2, 3                            
                while (iterator->payload->getPosition() != position){
                    Mages.next(iterator);
                }
                iterator->payload->addSpell(spells[i]->getNameSpell(), typeRune, spells[i]->IsLegal, position);       ////Recordar cambiar el ilegali
            }else{
                Mage *newMage = new Mage(name, magesCount+1, spells[i]->IsLegal);
                newMage->addSpell(spells[i]->getNameSpell(), typeRune, spells[i]->IsLegal, magesCount+1);
                Mages.postInsert(Mages.last(), newMage);
                this->magesCount++;
            }

            ////End Seccion para agregar los hechizos a los magos

            this->spellCount++;
        } 

        papyrus.close();
    }

    ////End Seccion para leer archivos

    ////Init Seccion de escritura de archivos

    void processedSpells() {
        ofstream processedSpells("processedSpells.out");

        if (!processedSpells.is_open()) {
            return;
        }
        Node<Mage*>* iterator = Mages.first();
        processedSpells << "Hechizos Legales" << endl;                         // Procesamos los hechizos legales
        processedSpells << endl;
        while (iterator != nullptr){
            Node<MagicSpell> *spellIterator = iterator->payload->getSpells().first();
            if (spellIterator->payload.isLegal) {
                while (spellIterator != nullptr){
                    processedSpells << spellIterator->payload.name << endl;
                    spellIterator = spellIterator->next;
                }
                processedSpells << iterator->payload->getName() << endl;
            } else {
                spellIterator = spellIterator->next;
            }
            processedSpells << endl;
            Mages.next(iterator);
        }
        processedSpells << "Hechizos Ilegales" << endl;                       // Procesamos los hechizos Ilegales
        while (iterator != nullptr){
            Node<MagicSpell> *spellIterator = iterator->payload->getSpells().first();
            if (!spellIterator->payload.isLegal) {
                while (spellIterator != nullptr){
                    processedSpells << spellIterator->payload.name;
                    spellIterator = spellIterator->next;
                }
                processedSpells << iterator->payload->getName() << endl;
            } else {
                spellIterator = spellIterator->next;
            }
            Mages.next(iterator);
        }
        processedSpells.close();

    }

    void WantedUpdate() {
        bool updated = false;          //Verificamos con la funcion UpdateWanted, la cual verifica si se actualizo la lista de underInvestigation                                    
        UpdateWanted(updated);
        if (updated) {                 //Si se modifico la lista, entonces, procederemos a modificar el underInvestigation.in
            ofstream UnderInvestigationIn("underInvestigation.in");

            if (!UnderInvestigationIn.is_open()) {
            return;
            }
            Node<string> *iterator = UnderInvestigation.first();
            while (iterator != nullptr) {
                UnderInvestigationIn << iterator->payload << endl;
                UnderInvestigation.next(iterator);
            }
            UnderInvestigationIn.close();
        }
    }

    ////End Seccion Escritura de Archivos

    ////Init Seccion Funciones del programa

    bool mageIsInTheParty(string name, int &position){                          
        if (Mages.isEmpty()) return false;                               //Si la lista de magos esta vacia no nos interesa buscar
        Node<Mage*> *iterator = Mages.first();                           //Creamos el iterador
        while (iterator != nullptr){                            
            if (iterator->payload->getName() == name){                   //Si el nombre del mago es igual al nombre que estamos buscando retornamos true
                position = iterator->payload->getPosition();             //Si el nombre del mago es igual al nombre que estamos buscando retornamos true
                return true;
            };                  
            Mages.next(iterator);
        }   
        return false;                                                    //Si no conseguimos el nombre retornamos false
    }

    int kindRune(string rune){
        for (int i = 0; i < rune.length(); i++){                        //revisamos el arreglo de string, si conseguimos una runa 
            if (rune[i] == 'I') return 1;                               //retornamos un numero dependiendo del tipo de runa
            if (rune[i] == 'Q') return 2;                               //esto se asume que un hechizo siempre tiene el mismo
            if (rune[i] == 'T') return 3;                               //tipo de runa elemental con que nos de una ya sabemos
            if (rune[i] == 'V') return 4;                               //todas las demas
            if (rune[i] == 'L') return 5;
            if (rune[i] == 'O') return 6;
        }
        return 0;                                                       //si no conseguimos ningun tipo de runa elemental se considera 0 = Arcano
    }

    void UpdateWanted(bool &Updated) {
        Node<Mage*> *iterator = Mages.first();
        while (iterator != nullptr) {
            if(iterator->payload->getUnderInvestigation()) {
                UnderInvestigation.postInsert(UnderInvestigation.last(), iterator->payload->getName());
                Updated = true;
                Mages.next(iterator);
            } else {
                Mages.next(iterator);
            }
        }
    }

    ////End Seccion Funciones del programa

    ////Init Seccion para imprimir

    void printMostLarge(){
        spells[0]->findPathPerPower();
    }    

    void printGraf(){
        for (int i = 0; i < spellCount; i++){
            spells[i]->mostrarGrafos();
            cout << "Siguiente " << endl;
        }
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

    void PrintIlegallities() {
        spells[0]->EnergySupport();
        spells[0]->CataliticAdyacency();
        int ilegally = spells[0]->IsLegal;
        cout << "Se han encontrado " << ilegally << " ilegalidades en el hechizo" << endl;
        spells[0]->SpellName();
        cout << "El nombre del hechizo es " << spells[0]->spellname << endl;
    }

    void printLongestPath() {
        int bestOrder[spells[0]->magicPointCount] = {0};
        int maxEdges = 0;
        float longestPath = spells[0]->findLongestPath(1, bestOrder, maxEdges); 
        cout << "El camino mas largo tiene una ponderacion de " << longestPath << " y " << maxEdges << " aristas." << endl;
        cout << "Orden de visitados: ";
        for (int i = 0; i <= maxEdges; ++i) {
            cout << bestOrder[i] << " ";
        }
        cout << endl;
    }

    void printMageList(){
        Node<Mage*>* iterator = Mages.first();
        while (iterator != nullptr){
            cout << "Mago:" << iterator->payload->getName() << endl;
            Node<MagicSpell> *spellIterator = iterator->payload->getSpells().first();
            while (spellIterator != nullptr){
                cout << "Hechizo: " << spellIterator->payload.name << endl;
                if (spellIterator->payload.isLegal) {
                    cout << "El hechizo es legal" << endl;
                    } else {
                    cout << "El hechizo es ilegal" << endl;
                }
                spellIterator = spellIterator->next;
                
            }
            Mages.next(iterator);
            cout << endl;
        }

    }

    void printWantedList() {
        Node<string> *iterator = UnderInvestigation.first();
        while(iterator != nullptr) {
            cout << iterator->payload << endl;
            UnderInvestigation.next(iterator);
        }
    }

    ////End Seccion para imprimir


    ////init Seccion Destructor

    ~SpellDetector() {
        for (int i = 0; i < spellCount; i++) {
            delete spells[i];  
        }
        delete[] spells;
    }

    ////End Seccion Destructor

};

////End Programa Principal




int main (){
    SpellDetector a1;               //Creamos el objeto de tipo SpellDetector que manejara el programa
    a1.readSpell();                 //Leemos los hechizos
    a1.readUnderInvestigarion();    //Leemos la lista de bajo investigacion
    a1.printLongestCycle();         //Esto lo podemos quitar(Solo efecto de prueba)
    a1.printLongestPath();          //Esto lo podemos quitar(Solo efecto de prueba)
    a1.printMageList();             //Esto lo podemos quitar(Solo efecto de prueba)
    a1.printWantedList();           //Esto lo podemos quitar(Solo efecto de prueba)
    a1.processedSpells();           /*En esta funcion hacemos el procesado de todas las reviones
                                    Hechas anteriormente, en dado caso, en la lectura de cada grafo ya se 
                                    ejecutan cada una de las funciones requeridas, como recorridos, verificaciones
                                    de legalidad etc... Por tanto, aqui ya solo es pasar toda esa informacion recopilada
                                    a processedSpells*/
    a1.WantedUpdate();              /*Realizamos la actualizacion de la lista de bajo investigacion
                                    en caso de ser necesario*/

    return 0;
}
