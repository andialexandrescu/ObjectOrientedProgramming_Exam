/* Alexandrescu Andra 132
 * CLion MinGW 11.0
 * Bahrim Dragos */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Item{
private:
    static int id;
public:
    static int getId();

    virtual void read(istream &in) = 0;
    virtual void display(ostream &out) const = 0;
    virtual ~Item();
};

int Item::id = 0;

int Item::getId() {
    Item::id++;
    return Item::id;
}

Item::~Item() {
    Item::id--;
}

class Zid: public Item{
private:
    mutable int cod;
    float inaltime;
    float lungime;
    float grosime;
public:
    Zid();
    Zid(int cod, float inaltime, float lungime, float grosime);
    ~Zid() override = default;

    void setInaltime(const float &i);
    void setLungime(const float &l);
    void setGrosime(const float &g);

    void read(istream &in) override;
    void display(ostream &out) const override;
};

Zid::Zid() : Item(), inaltime(0), lungime(0), grosime(0)
{
    this->cod = this->getId();
}

Zid::Zid(int cod, float inaltime, float lungime, float grosime) : Item(), inaltime(inaltime), lungime(lungime), grosime(grosime)
{
    this->cod = this->getId();
}

void Zid::read(istream &in) {
    cout<<"Introduceti date despre un zid:";
    cout<<endl<<"\tinaltime "; in>>this->inaltime;
    cout<<endl<<"\tlungime "; in>>this->lungime;
    cout<<endl<<"\tgrosime "; in>>this->grosime;
    cout<<endl;
}

void Zid::display(ostream &out) const {
    out<<"Date despre un zid:";
    out<<"\tcod "<<this->cod;
    out<<endl<<"\tinaltime "<<this->inaltime;
    out<<endl<<"\tlungime "<<this->lungime;
    out<<endl<<"\tgrosime "<<this->grosime;
    out<<endl;
}

void Zid::setInaltime(const float &i) {
    this->inaltime = i;
}

void Zid::setLungime(const float &l) {
    this->lungime = l;
}

void Zid::setGrosime(const float &g) {
    this->grosime = g;
}

class Turn: public Item{
private:
    mutable int cod;
    float putere_laser;
public:
    Turn();
    Turn(int cod, int putere_laser);
    ~Turn() override = default;

    void setPutereLaser(const float &p);

    void read(istream &in) override;
    void display(ostream &out) const override;
};

Turn::Turn() : putere_laser(0){
    this->cod = this->getId();
}

Turn::Turn(int cod, int putere_laser) : putere_laser(putere_laser)
{
    this->cod = this->getId();
}

void Turn::read(istream &in) {
    cout<<"Introduceti date despre un zid:";
    cout<<endl<<"\tputere laser "; in>>this->putere_laser;
    cout<<endl;
}

void Turn::display(ostream &out) const {
    out<<"Date despre un turn:";
    out<<"\tcod "<<this->cod;
    out<<endl<<"\tputere laser "<<this->putere_laser;
    out<<endl;
}

void Turn::setPutereLaser(const float &p) {
    this->putere_laser = p;
}

class Robot: virtual public Item{
private:
    mutable int cod;
    float damage;
    int nivel;
    int viata;
public:
    Robot();
    Robot(int cod, float damage, int nivel, int viata);
    ~Robot() override = default;

    void setDamage(const float &d);
    void setNivel(const int &n);
    void setViata(const int &v);

    void read(istream &in) override
    {
        cout<<"Introduceti date despre un robot:";
        cout<<endl<<"\tdamage "; in>>this->damage;
        cout<<endl<<"\tnivel "; in>>this->nivel;
        cout<<endl<<"\tviata "; in>>this->viata;
        cout<<endl;
    }
    void display(ostream &out) const override
    {
        out<<"Date despre un robot:";
        out<<endl<<"\tdamage "<<this->damage;
        out<<endl<<"\tnivel "<<this->nivel;
        out<<endl<<"\tviata "<<this->viata;
        out<<endl;
    }
};

Robot::Robot() : damage(0), nivel(0), viata(0)
{
    this->cod = this->getId();
}

Robot::Robot(int cod, float damage, int nivel, int viata) : damage(damage), nivel(nivel), viata(viata)
{
    this->cod = this->getId();
}

void Robot::setDamage(const float &d) {
    this->damage = d;
}

void Robot::setNivel(const int &n) {
    this->nivel = n;
}

void Robot::setViata(const int &v) {
    this->viata = v;
}

class Inventar{
private:
    vector<Item *> v_items;
    int puncte;
public:
    Inventar();
    Inventar(int puncte, const vector<Item *> &v_itm);
    Inventar(const Inventar &other_obj);
    Inventar& operator=(const Inventar &other_obj);
    ~Inventar();

    friend istream& operator>>(istream &in, Inventar &obj)
    {
        obj.read(in);
        return in;
    }
    friend ostream& operator<<(ostream &out, const Inventar &obj)
    {
        obj.display(out);
        return out;
    }

    void read(istream &in);
    void display(ostream &out) const;

    void computePuncte(Item *ptr)
    {
        if(typeid(*ptr) == typeid(Zid))
        {
            this->puncte -= 300;
            auto ptr_temp = dynamic_cast<Zid *>(ptr);
            ptr_temp->setInaltime(2);
            ptr_temp->setLungime(1);
            ptr_temp->setGrosime(0.5);
        } else if(typeid(*ptr) == typeid(Turn))
        {
            this->puncte -= 500;
            auto ptr_temp = dynamic_cast<Turn *>(ptr);
            ptr_temp->setPutereLaser(1000);
        }/*else if(typeid(*ptr) == typeid(RobotAerian))
        {
            ...
        }else if(typeid(*ptr) == typeid(RobotTerestru))
        {
            ...
        }*/
    }

};

Inventar::Inventar() : puncte(50000) { this->v_items.clear();}

Inventar::Inventar(int puncte, const vector<Item *> &v_itm) : puncte(puncte)
{
    this->v_items.clear();
    for(const auto &i: v_itm)
    {
        this->v_items.push_back(i); // vector de pointeri
    }
}

Inventar::Inventar(const Inventar &other_obj) : puncte(other_obj.puncte)
{
    this->v_items.clear();
    for(const auto &i: other_obj.v_items)
    {
        this->v_items.push_back(i); // vector de pointeri
    }
}

Inventar &Inventar::operator=(const Inventar &other_obj) {
    if(this != &other_obj)
    {
        this->puncte = other_obj.puncte;
        this->v_items.clear();
        for(const auto &i: other_obj.v_items)
        {
            this->v_items.push_back(i); // vector de pointeri
        }
    }
    return *this;
}

Inventar::~Inventar() {
    if(!this->v_items.empty())
    {
        for(const auto &i: this->v_items)
        {
            delete i;
        }
    }
    this->v_items.clear();
}

void Inventar::read(istream &in) {
    int n;
    this->puncte = 50000;
    cout<<"Nr de elem ale vectorului de items din inventar: "; in>>n;
    this->v_items.resize(n);
    for(int i = 0; i < n; i++)
    {
        int choice;
        Item *ptr_item = nullptr;
        cout<<"1.Zid\n2.Turn\n3.Robot\n";
        cout<<"Adauga un element de tip item in inventarul curent:"<<endl; in>>choice;
        cout<<"Item ["<<i<<"]:"<<endl;
        switch(choice)
        {
            case 1:
                ptr_item = new Zid();
                break;
            case 2:
                ptr_item = new Turn();
                break;
            case 3:
                ptr_item = new Robot();
                break;
            default:
                cout<<"Alegere invalida"<<endl;
                break;
        }
        ptr_item->read(in);
        //this->computePuncte(ptr_item); ar trebui ca functia sa fie folosita in meniu nu aici
        this->v_items.push_back(ptr_item);
    }
}

void Inventar::display(ostream &out) const {
    Item* ptr_temp = nullptr;
    cout<<"Vectorul de items din inventar: ";
    for(const auto &ptr_i: this->v_items)
    {
        try {
            if(typeid(*ptr_i) == typeid(Zid))
            {
                auto ptr_temp = dynamic_cast<Zid *>(ptr_i);
            } else if(typeid(*ptr_i) == typeid(Turn))
            {
                auto ptr_temp = dynamic_cast<Turn *>(ptr_i);
            } else if(typeid(*ptr_i) == typeid(Robot))
            {
                auto ptr_temp = dynamic_cast<Robot *>(ptr_i);
            } else
            {
                throw runtime_error("Nu e un item de un tip valid");
            }
            // teoretic ar fi trebuit si robot teresetru si aerian (tocmai de aceea mostenirea din robot catre item este virtuala) sa mosteneasca robot
            ptr_temp->display(out);
            out<<endl;
        } catch(const runtime_error &)
        {
            cout<<"Vectorul contine un item ilegal"<<endl;
        }
    }
    out<<endl;
}

int main() {
    vector<Inventar> v_inventare;
    int n, m;
    cout<<"Introduceti nr de inventare: "; cin>>n;
    v_inventare.resize(n);
    for(int i = 0; i < n; i++)
    {
        Inventar inv;
        cin>>inv;
        v_inventare.push_back(inv);
    }
    cout<<endl<<"Vectorul de inventare este:"<<endl;
    for(const auto &inv: v_inventare)
    {
        cout<<inv;
    }
    return 0;
}