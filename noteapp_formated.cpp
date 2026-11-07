#include <algorithm>
#include <format>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

class Note {
public:
    std::string n_name;
    int id;

    Note(int id, std::string name)
    {
        this->id = id;
        this->n_name = name;
    }

    int getId() const
    {
        return id;
    }

    std::string getName() const
    {
        return n_name;
    }

    virtual std::string getType() const = 0;

    virtual void display() const = 0;

    virtual ~Note() { }
};

class Simple : public Note {
public:
    std::string s_text;

    Simple(int id, std::string name, std::string text)
        : Note(id, name)
    {
        s_text = text;
    }

    std::string getType() const override
    {
        return "Простая записка";
    }

    void display() const override
    {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Тип: " << getType() << std::endl;
        std::cout << "Название: " << n_name << std::endl;
        std::cout << "Содержание: " << s_text << std::endl;
    }
};

class Task : public Note {
public:
    std::string t_text;
    int t_date = 0;

    Task(int id, std::string name, std::string text, int date)
        : Note(id, name)
    {
        t_text = text;
        t_date = date;
    }

    std::string getType() const override
    {
        return "Задание";
    }

    void display() const override
    {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Тип: " << getType() << std::endl;
        std::cout << "Название: " << n_name << std::endl;
        std::cout << "Содержание: " << t_text << std::endl;
        std::cout << "Срок: " << t_date << " дней" << std::endl;
    }

    int getDate() const
    {
        return t_date;
    }
};

class Recipe : public Note {
public:
    std::list<std::string> r_steps;

public:
    Recipe(int id, std::string name, std::list<std::string> steps)
        : Note(id, name)
    {
        r_steps = steps;
    }

    std::string getType() const override
    {
        return "Рецепт";
    }

    void display() const override
    {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Тип: " << getType() << std::endl;
        std::cout << "Название: " << n_name << std::endl;
        std::cout << "Шаги приготовления:" << std::endl;

        int i = 1;

        for (const auto& step : r_steps) {
            std::cout << i << ". " << step << std::endl;
            i++;
        }
    }
};

class NoteManager {
private:
    std::vector<std::unique_ptr<Note>> notes;

    int nextId = 0;

public:
    void addNote();
    void showAll();
    void find();
    void deleteNote();
    void showNearestTasks();
};

void NoteManager::addNote()
{

    int choice;

    std::cout << "\nВыберите тип записки:\n";
    std::cout << "1. Простая записка\n";
    std::cout << "2. Задание\n";
    std::cout << "3. Рецепт\n";

    std::cout << "Ваш выбор: ";
    std::cin >> choice;

    std::string name;
    std::string text;

    std::cout << "Введите название: ";
    std::getline(std::cin >> std::ws, name);

    switch (choice) {

    case 1: {

        std::cout << "Введите содержание: ";
        std::getline(std::cin, text);

        notes.push_back(
            std::make_unique<Simple>(
                nextId, name, text));

        std::cout << "Записка типа 'Простая записка' успешно добавлена, присвоен ID = " << nextId << std::endl;

        break;
    }

    case 2: {

        int days;

        std::cout << "Введите содержание: ";
        std::getline(std::cin, text);

        std::cout << "Введите срок (в днях): ";
        std::cin >> days;

        if (days <= 0) {
            std::cout << "Количество шагов должно быть положительным!\n";
            return;
        }

        notes.push_back(
            std::make_unique<Task>(
                nextId, name, text, days));

        std::cout << "Записка типа 'Задание' успешно добавлена, присвоен ID = " << nextId << std::endl;

        ;

        break;
    }

    case 3: {

        int count;
        std::list<std::string> steps;

        std::cout << "Введите количество шагов: ";
        std::cin >> count;

        if (count <= 0) {
            std::cout << "Количество шагов должно быть положительным!\n";
            return;
        }

        for (int i = 0; i < count; i++) {

            std::string step;

            std::cout << "Шаг " << i + 1 << ": ";
            std::getline(std::cin >> std::ws, step);

            steps.push_back(step);
        }

        notes.push_back(
            std::make_unique<Recipe>(
                nextId, name, steps));

        std::cout << "Записка типа 'Рецепт' успешно добавлена, присвоен ID = "
                  << nextId << std::endl;

        break;
    }

    default:
        std::cout << "Неверный тип записки!\n";
        return;
    }

    nextId++;
}

void NoteManager::showAll()
{

    if (notes.empty()) {
        std::cout << "Список записок пуст!\n";
        return;
    }

    std::cout << "\n========== ВСЕ ЗАПИСКИ ==========\n";

    for (const auto& note : notes) {

        std::cout << "ID: " << note->getId()
                  << " | Тип: " << note->getType()
                  << " | Название: " << note->getName()
                  << std::endl;
    }

    std::cout << "====================================\n";
}

void NoteManager::find()
{

    int choice;

    std::cout << "Для поиска по ID введите 1, для поиска по названию введите 2.";
    std::cout << "Ваш выбор: ";
    std::cin >> choice;

    switch (choice) {

    case 1: {

        int id;

        std::cout << "Введите ID записки: ";
        std::cin >> id;

        for (const auto& note : notes) {

            if (note->getId() == id) {

                std::cout << "\nЗаписка найдена!\n";
                note->display();

                return;
            }
        }
        break;
    }

    case 2: {

        std::string name;
        std::cout << "Введите название записки: ";
        std::getline(std::cin >> std::ws, name);

        for (const auto& note : notes) {

            if (note->getName() == name) {

                std::cout << "\nЗаписка найдена!\n";
                note->display();

                return;
            }
        }
        break;
    }
    }
}

void NoteManager::showNearestTasks()
{

    std::vector<Task*> tasks;

    for (const auto& note : notes) {

        Task* task = dynamic_cast<Task*>(note.get());

        if (task != nullptr) {
            tasks.push_back(task);
        }
    }

    if (tasks.empty()) {
        std::cout << "Заданий пока нет!\n";
        return;
    }

    std::sort(tasks.begin(), tasks.end(),
        [](Task* a, Task* b) {
            return a->getDate() < b->getDate();
        });

    std::cout << "\n========== БЛИЖАЙШИЕ ЗАДАНИЯ ==========\n";

    std::cout << "ID\tНазвание\tСрок (в днях)\n";

    for (const auto& task : tasks) {

        std::cout << task->getId() << "\t"
                  << task->getName() << "\t"
                  << task->getDate() << std::endl;
    }
}

void NoteManager::deleteNote()
{

    int id;

    std::cout << "Введите ID записки для удаления: ";
    std::cin >> id;

    auto it = std::remove_if(
        notes.begin(),
        notes.end(),
        [id](const std::unique_ptr<Note>& note) {
            return note->getId() == id;
        });

    if (it == notes.end()) {
        std::cout << "Записка не найдена!\n";
    } else {
        notes.erase(it, notes.end());
        std::cout << "Записка успешно удалена!\n";
    }
}

int main()
{

    NoteManager manager;

    int choice;

    do {

        std::cout << "\n========== МЕНЕДЖЕР ЗАПИСОК ==========\n";

        std::cout << "1. Добавить записку\n";
        std::cout << "2. Показать все записки\n";
        std::cout << "3. Найти записку по ID или названию\n";
        std::cout << "4. Удалить записку\n";
        std::cout << "5. Просмотр ближайших заданий\n";
        std::cout << "0. Выход\n";

        std::cout << "Выберите действие: ";
        std::cin >> choice;

        switch (choice) {

        case 1:
            manager.addNote();
            break;

        case 2:
            manager.showAll();
            break;

        case 3:
            manager.find();
            break;

        case 4:
            manager.deleteNote();
            break;

        case 5:
            manager.showNearestTasks();
            break;

        case 0:
            std::cout << "Завершение программы...\n";
            break;

        default:
            std::cout << "Неверный пункт меню!\n";
        }

    } while (choice != 0);

    return 0;
}