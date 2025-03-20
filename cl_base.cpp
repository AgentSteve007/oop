#include "cl_base.h"
#include <stack>

cl_base::cl_base(cl_base* parent, string name) : parent(parent), name(name) {
    if (parent != nullptr) { // если объект не имеет нулевой указатель
        parent->children.push_back(this); // добавление в контейнер адреса на текущий объект
    }
}

cl_base::~cl_base() {
    for (int i = 0; i < children.size(); i++) delete children[i];
}

string cl_base::get_name() const { // возврат значения наименования объекта
    return name;
}

cl_base* cl_base::get_parent() const { // возврат адреса указателя на головной объект
    return parent;
}

bool cl_base::setName(string name1) { // установка наименования объекта
    if (get_parent() != nullptr && get_parent()->get_child_by_name(name1) != nullptr) {
        return false;
    }
    name = name1;
    return true;
}

cl_base* cl_base::get_child_by_name(string name) {
    for (auto child : children) {
        if (child->name == name) {
            return child;
        }
    }
    return nullptr;
}

// Update
cl_base* cl_base::findObjOnBranch(string s_object_name) {
    cl_base* found = nullptr; // Инициализация пустого указателя found на объект класса cl_base
    queue<cl_base*> elementsQueue; // Объявление очереди elementsQueue, которая принимает указатели на объекты класса cl_base (принимает указатели подчинённых объектов)
    elementsQueue.push(this); // Добавление в конец очереди elementsQueue указателя на текущий объект
    while (!elementsQueue.empty()) { // Пока очередь elementsQueue содержит элементы
        cl_base* elem = elementsQueue.front();
        if (elem->name == s_object_name) {
            if (found != nullptr) {
                return nullptr;
            } else {
                found = elem;
            }
        }
        for (int i = 0; i < elem->children.size(); i++) {
            elementsQueue.push(elem->children[i]); // внесение адресов в очередь
        }
        elementsQueue.pop();
    }
    return found;
}

cl_base* cl_base::findObjOnTree(string s_object_name) {
    if (parent != nullptr) { // Если текущий объект не родительский, ищем объект в дереве
        return parent->findObjOnTree(s_object_name); // возврат адреса указателя на текущий объект дерева
    } else {
        return findObjOnBranch(s_object_name); // Если текущий объект не родительский, ищем объект в ветке
    }
}

void cl_base::Print(int level) {
    cout << endl;
    for (int i = 0; i < level; ++i) {
        cout << " ";
    }
    cout << this->get_name();
    for (int i = 0; i < children.size(); ++i) {
        children[i]->Print(level + 1);
    }
}

void cl_base::PrintWithState(int level) {
    cout << endl;
    for (int i = 0; i < level; ++i) {
        cout << " ";
    }
    if (this->state != 0) {
        cout << this->get_name() << " is ready";
    } else {
        cout << this->get_name() << " is not ready";
    }
    for (int i = 0; i < children.size(); ++i) {
        children[i]->PrintWithState(level + 1);
    }
}

void cl_base::setState(int state) {
    if (parent == nullptr || parent->state != 0) {
        this->state = state;
    }
    if (state == 0) {
        this->state = state;
        for (int i = 0; i < children.size(); i++) {
            children[i]->setState(state);
        }
    }
}

// 3 Часть
bool cl_base::set_parent(cl_base* new_parent) {
    /*
    Метод переопределения головного объекта для текущего в дереве иерархии
    */
    if (this->get_parent() == new_parent) { // головной объект совпадает с новым головным
        return true;
    }
    if (this->get_parent() == nullptr || new_parent == nullptr) {
        // значение указателя на новый головной объект равно значению нулевого указателя
        return false;
    }
    if (new_parent->get_child_by_name(this->get_name()) != nullptr) {
        // У нового головного объекта уже есть подчинённый с именем текущего объекта
        return false;
    }
    stack<cl_base*> st; // объявление стека st
    st.push(this);
    while (!st.empty()) { // пока стек имеет какие-то значения
        cl_base* current_node_ptr = st.top(); // инициализация указателя по верхнему элементу
        st.pop();
        if (current_node_ptr == new_parent) {
            return false;
        }
        for (int i = 0; i < current_node_ptr->children.size(); ++i) {
            st.push(current_node_ptr->children[i]);
        }
    }
    vector<cl_base*>& v = this->get_parent()->children; // будет производиться перемещение старого головного объекта в вектор подчинённых
    for (int i = 0; i < v.size(); ++i) {
        if (v[i]->get_name() == this->get_name()) {
            v.erase(v.begin() + i);
            new_parent->children.push_back(this); // добавление текущего объекта в вектор подчинённых объектов с учётом изменения головного объекта
            return true;
        }
    }
    return false;
}

void cl_base::remove_child_by_name(string child_name) {
    /*
    Метод удаления подчиненного объекта по наименованию
    */
    vector<cl_base*>& vec = this->children;
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i]->get_name() == child_name) {
            delete vec[i]; // вызов деструктора для объекта по i-ому адресу вектора vec
            vec.erase(vec.begin() + i); // удаление i-ого элемента
            return;
        }
    }
}

cl_base* cl_base::get_object_by_path(string path) {
    /*
    Метод получения указателя на любой объект в составе дерева иерархии объектов
    согласно пути (координаты)
    */
    if (path.empty()) {
        return nullptr;
    }
    if (path == ".") {
        return this;
    }
    if (path[0] == '.') {
        return findObjOnBranch(path.substr(1)); // вернуть результат вызова метода findObjOnBranch c параметром path, начиная со второго символа
    }
    if (path.substr(0, 2) == "//") { // первые два символа path равны "//"
        return this->findObjOnTree(path.substr(2)); // вернуть результат вызова метода findObjOnBranch, начиная с третьего символа
    }
    if (path[0] != '/') {
        size_t slash_index = path.find('/');
        cl_base* child_ptr = this->get_child_by_name(path.substr(0, slash_index));
        if (child_ptr == nullptr || slash_index == string::npos) {
            return child_ptr;
        }
        return child_ptr->get_object_by_path(path.substr(slash_index + 1));
    }
    cl_base* root_ptr = this;
    while (root_ptr->get_parent() != nullptr) {
        root_ptr = root_ptr->get_parent();
    }
    if (path == "/") {
        return root_ptr;
    }
    return root_ptr->get_object_by_path(path.substr(1));
}

// 4 Часть
string cl_base::get_absolute_path() {
    cl_base* p_obj = this; // указатель на текущий объект в прохождении иерархии вверх
    string s_path = "";
    while (p_obj->get_parent() != nullptr) {
        s_path = "/" + p_obj->get_name() + s_path;
        p_obj = p_obj->get_parent();
    }
    if (s_path == "") s_path = '/';
    return s_path;
}

int cl_base::get_class_number() {
    return i_class_number;
}

vector<cl_base*> cl_base::get_children() {
    return children;
}

void cl_base::set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {
    o_sh* p_value; // объявление новой связи
    for (int i = 0; i < p_connects.size(); i++) { // не существует ли уже связь с заданными параметрами
        if (p_connects[i]->p_signal == p_signal &&
            p_connects[i]->p_target == p_target &&
            p_connects[i]->p_handler == p_handler)
            return;
    }
    p_value = new o_sh(); // динамическое создание новой связи
    p_value->p_signal = p_signal; // присвоение свойств параметрам
    p_value->p_target = p_target;
    p_value->p_handler = p_handler;
    p_connects.push_back(p_value); // добавление в контейнер новой связи
}

void cl_base::delete_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler) {
    for (int i = 0; i < p_connects.size(); i++) { // находим связь с заданными параметрами
        if (p_connects[i]->p_signal == p_signal &&
            p_connects[i]->p_target == p_target &&
            p_connects[i]->p_handler == p_handler) {
            p_connects.erase(p_connects.begin() + i); // удаляем из контейнера связей найденную связь
            return;
        }
    }
}

void cl_base::emit_signal(TYPE_SIGNAL p_signal, string s_message) {
    TYPE_HANDLER p_handler;
    cl_base* p_object;
    if (state == 0) return; // если текущий объект отключен - выход
    (this->*p_signal)(s_message);
    for (int i = 0; i < p_connects.size(); i++) {
        if (p_connects[i]->p_signal == p_signal && p_connects[i]->p_target->state != 0) { // определение допустимого обработчика
            p_handler = p_connects[i]->p_handler;
            p_object = p_connects[i]->p_target;
            (p_object->*p_handler)(s_message); // отправка сигнала
        }
    }
}
