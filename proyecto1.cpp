#include <iostream>
#include <fstream>
using namespace std;

struct ArcaneLine // Arista
{
    float manaPoint;
    int dest;
    ArcaneLine *Next;
    
    ArcaneLine(int dest, float value) : dest(dest), manaPoint(value), Next(nullptr) {}
    ~ArcaneLine() {}  
};

class SpellVert // Vértice
{
private:
    int position;
    ArcaneLine *listedArcane;
    SpellVert *nextVert;

public:
    SpellVert(int position) : position(position), listedArcane(nullptr), nextVert(nullptr) {}

    void addArcaneLine(int dest, float value) {
        ArcaneLine *newArcaneLine = new ArcaneLine(dest, value);
        newArcaneLine->Next = listedArcane;
        listedArcane = newArcaneLine;
    }

    void printArcaneLines() {
        ArcaneLine *temp = listedArcane;
        while (temp != nullptr) {
            cout << "(" << temp->dest << "," << temp->manaPoint << ") ";
            temp = temp->Next;
        }
        cout << endl;
    }

    void setNextVert(SpellVert *temp) {this->nextVert = temp;}
    SpellVert *getNextVert() {return nextVert;}
    int getPosition() {return position;}

    ~SpellVert() {
        ArcaneLine *temp;
        while (listedArcane != nullptr) {
            temp = listedArcane;
            listedArcane = listedArcane->Next;
            delete temp;
        }
    }
};

class Spell // Grafo
{
private:
    SpellVert *listedSpells;
    string name;
    string rune;
    Spell *nextSpell;

public:
    Spell() : listedSpells(nullptr), name(""), rune(""), nextSpell(nullptr) {}

    void addName(string name, string rune) {
        this->name = name;
        this->rune = rune;
    }

    void addSpellVert(int position) {
        SpellVert *newSpellVert = new SpellVert(position);
        newSpellVert->setNextVert(listedSpells);
        listedSpells = newSpellVert;
    }

    SpellVert *findVert(int src) {
        SpellVert *temp = listedSpells;
        while (temp != nullptr) {
            if (temp->getPosition() == src) {
                return temp;
            }
            temp = temp->getNextVert();
        }
        return nullptr;
    }

    void addArcaneLine(int src, int dest, float value) {
        SpellVert *originVert = findVert(src);
        if (originVert != nullptr) {
            originVert->addArcaneLine(dest, value);
        }
    }

    void printSpellVert() {
        cout << "Nombre del mago: " << name << endl;
        cout << "Runas utilizadas: " << rune << endl;

        SpellVert *temp = listedSpells;
        while (temp != nullptr) {
            cout << "Vertice " << temp->getPosition() << " -> ";
            temp->printArcaneLines();
            temp = temp->getNextVert();
        }
    }

    void setNextSpell(Spell *temp) { this->nextSpell = temp; }
    Spell *getNextSpell() { return nextSpell; }

    ~Spell() {
        SpellVert *temp;
        while (listedSpells != nullptr) {
            temp = listedSpells;
            listedSpells = listedSpells->getNextVert();
            delete temp;
        }
    }
};

class SpellDectector //Aca es donde modificaremos todo.
{
private:
    Spell *spells;
    int spellCount;
public:
    SpellDectector() : spells(nullptr), spellCount(0) {}

    void readSpell() {
        ifstream file("spellList.in");
        if (!file.is_open()) {
            return;
        }

        file >> spellCount;
        file.ignore();

        for (int i = 0; i < spellCount; i++) {
            string name, rune;
            int vertCount, arcaneLineCount;

            getline(file, name);
            file >> vertCount;
            file.ignore();
            getline(file, rune); 
            file >> arcaneLineCount; 

            Spell *newSpell = new Spell();
            newSpell->addName(name, rune);

            
            for (int j = 1; j <= vertCount; j++) {
                newSpell->addSpellVert(j);
            }

        
            for (int j = 0; j < arcaneLineCount; j++) {
                int src, dest;
                float value;
                file >> src >> dest >> value;
                newSpell->addArcaneLine(src, dest, value);
            }

    
            if (spells == nullptr) {
                spells = newSpell;
            } else {
                Spell *temp = spells;
                while (temp->getNextSpell() != nullptr) {
                    temp = temp->getNextSpell();
                }
                temp->setNextSpell(newSpell);
            }
        }

        file.close();
    }

    void printSpells() {
        Spell *temp = spells;
        while (temp != nullptr) {
            temp->printSpellVert();
            temp = temp->getNextSpell();
            cout << endl;
        }
    }

    ~SpellDectector() {
        Spell *temp;
        while (spells != nullptr) {
            temp = spells;
            spells = spells->getNextSpell();
            delete temp;
        }
    }
};

int main() {
    SpellDectector detector;
    detector.readSpell();
    detector.printSpells();
    return 0;
}
