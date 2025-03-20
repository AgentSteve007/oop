#include "application.h"

application::application(cl_base* parent): cl_base(parent) {
    p_signals.push_back(SIGNAL_D(cl_1::signal_f));
    // добавление в контейнер сигналов для каждого класса
    p_signals.push_back(SIGNAL_D(cl_2::signal_f));
    p_signals.push_back(SIGNAL_D(cl_3::signal_f));
    p_signals.push_back(SIGNAL_D(cl_4::signal_f));
    p_signals.push_back(SIGNAL_D(cl_5::signal_f));
    p_signals.push_back(SIGNAL_D(cl_6::signal_f));

    p_handlers.push_back(HANDLER_D(cl_1::handler_f));
    // добавление в контейнер обработчиков для каждого класса
    p_handlers.push_back(HANDLER_D(cl_2::handler_f));
    p_handlers.push_back(HANDLER_D(cl_3::handler_f));
    p_handlers.push_back(HANDLER_D(cl_4::handler_f));
    p_handlers.push_back(HANDLER_D(cl_5::handler_f));
    p_handlers.push_back(HANDLER_D(cl_6::handler_f));
}

void application::build_tree_objects() {
    // Построение дерева иерархии
    string s_path; // путь головного объекта
    string s_signal; // путь объекта, выдающего сигнал
    string s_handler; // путь целевого объекта
    string s_child_name; // имя нового дочернего объекта
    int i_class_number; // номер класса
    cl_base* p_current_parent; // указатель на текущий головной объект
    cl_base* p_signal; // указатель на объект, выдающий сигнал
    cl_base* p_handler; // указатель на целевой объект

    cin >> s_path; // ввод имени головного объекта дерева иерархии
    p_root_obj = new cl_1(nullptr, s_path);

    while (true) {
        cin >> s_path;
        if (s_path == "endtree") { // если путь равен endtree, то заканчиваем создание объекта
            break;
        }
        cin >> s_child_name;
        cin >> i_class_number;

        p_current_parent = p_root_obj->get_object_by_path(s_path); // получаем адрес головного объекта
        if (p_current_parent->get_child_by_name(s_child_name) != nullptr) { // если происходит дублирование имен
            continue;
        }

        if (i_class_number == 1) {
            // в зависимости от номера класса создаем новый объект
            new cl_1(p_current_parent, s_child_name);
        } else if (i_class_number == 2) {
            new cl_2(p_current_parent, s_child_name);
        } else if (i_class_number == 3) {
            new cl_3(p_current_parent, s_child_name);
        } else if (i_class_number == 4) {
            new cl_4(p_current_parent, s_child_name);
        } else if (i_class_number == 5) {
            new cl_5(p_current_parent, s_child_name);
        } else if (i_class_number == 6) {
            new cl_6(p_current_parent, s_child_name);
        }
    }

    cin >> s_signal;
    while (s_signal != "end_of_connections") {
        cin >> s_handler;
        p_signal = p_root_obj->get_object_by_path(s_signal); // получаем адрес объекта, выдающего сигнал
        p_handler = p_root_obj->get_object_by_path(s_handler); // получаем адрес целевого объекта
        p_signal->set_connection(p_signals[p_signal->get_class_number() - 1], p_handler, p_handlers[p_handler->get_class_number() - 1]);
        cin >> s_signal;
    }
}

int application::exec_app() {
    queue<cl_base*> q; // очередь объектов для прохода дерева в ширину
    q.push(p_root_obj); // добавление корневого объекта

    while (!q.empty()) {
        // проход дерева в ширину для активации всех объектов
        q.front()->setState(1);
        for (int i = 0; i < q.front()->get_children().size(); i++) {
            // добавление в очередь следующих объектов
            q.push(q.front()->get_children()[i]);
        }
        q.pop();
    }

    string s_command; // команда
    string s_path; // путь объекта
    string s_handler; // путь целевого объекта
    string s_message; // сообщение
    int i_state; // состояние объекта
    cl_base* p_obj; // указатель на объект для выполнения заданной команды
    cl_base* p_signal; // указатель на объект, выдающий сигнал
    cl_base* p_handler; // указатель на целевой объект

    cout << "Object tree";
    p_root_obj->Print(0); // вывод дерева иерархии объектов
    cout << endl;

    cin >> s_command;
    while (s_command != "END") {
        cin >> s_path;

        if (s_command == "EMIT") { // Если команда - EMIT, то выдаём сигнал от заданного по координате объекта
            getline(cin, s_message);
            p_obj = p_root_obj->get_object_by_path(s_path);
            if (p_obj == nullptr) {
                cout << "Object " << s_path << " not_found" << endl;
            } else {
                p_obj->emit_signal(p_signals[p_obj->get_class_number() - 1], s_message);
            }
        } else if (s_command == "SET_CONNECT") { // Если команда - SET_CONNECT, то установка новой связи
            cin >> s_handler;
            p_signal = p_root_obj->get_object_by_path(s_path); // получаем адрес объекта, выдающего сигнал
            if (p_signal == nullptr) {
                cout << "Object " << s_path << " not found" << endl;
            } else {
                p_handler = p_root_obj->get_object_by_path(s_handler); // получаем адрес целевого объекта
                if (p_handler == nullptr) {
                    cout << "Handler object " << s_handler << " not_found" << endl;
                } else {
                    p_signal->set_connection(p_signals[p_signal->get_class_number() - 1], p_handler, p_handlers[p_handler->get_class_number() - 1]);
                }
            }
        } else if (s_command == "DELETE_CONNECT") { // если команда DELETE_CONNECT, то удаление связи
            cin >> s_handler;
            p_signal = p_root_obj->get_object_by_path(s_path); // получаем адрес объекта, выдающего сигнал
            if (p_signal == nullptr) {
                cout << "Object " << s_path << " not found" << endl;
            } else {
                p_handler = p_root_obj->get_object_by_path(s_handler); // получаем адрес целевого объекта
                if (p_handler == nullptr) {
                    cout << "Handler object " << s_handler << " not_found" << endl;
                } else {
                    p_signal->delete_connection(p_signals[p_signal->get_class_number() - 1], p_handler, p_handlers[p_handler->get_class_number() - 1]);
                }
            }
        } else if (s_command == "SET_CONDITION") { // если команда SET_CONDITION, то установка нового состояния
            cin >> i_state;
            p_obj = p_root_obj->get_object_by_path(s_path); // получаем адрес объекта, выдающего сигнал
            if (p_obj == nullptr) {
                cout << "Object " << s_path << " not found" << endl;
            } else {
                p_obj->setState(i_state);
            }
        }

        cin >> s_command;
    }

    return 0;
}
