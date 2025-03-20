#include "application.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    application ob_application(nullptr); // Создание объекта приложения
    ob_application.build_tree_objects(); // Построение дерева объектов
    return ob_application.exec_app(); // Запуск приложения
}
