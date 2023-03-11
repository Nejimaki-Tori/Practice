#include <iostream>
#include <string>

using namespace std;

class Game{
protected:
    string m_name; // название игры
    int m_price; // цена игры
    string m_genre; // жанр игры
    bool inLibrary = false;
    int lib;
    virtual void isInLibrary (bool tmp) = 0;
};

class Steam: public Game {
public:
    Steam() {
        m_name = "";
        m_price = 0;
        m_genre = "";
    }

    void isInLibrary(bool tmp) {
        inLibrary = tmp;
    }

    Steam(string name, const int price, string genre) {
        m_name = name;
        m_price = price;
        m_genre = genre;
        Steam::count++;
    }

    virtual ~Steam() = default;

    string getName() const {
        return m_name;
    }

    void setName(string name) {
        m_name = name;
    }

    void setLib(int n){
        lib = n;
    }

    int getLib(){
        return lib;
    }

    int getPrice() const { return m_price; }

    void setPrice(int price) { m_price = price; }

    string getGenre() const { return m_genre; }

    void setGenre(string genre) { m_genre = genre; }

    static int count; // количество игр в магазине

    Steam &operator=(const Steam &other) { // перегрузка операции =
        if (this != &other) {
            m_name = other.m_name;
            m_price = other.m_price;
            m_genre = other.m_genre;
        }
        return *this;
    }

    bool getInLibrary(){
        return inLibrary;
    }

    friend ostream &operator<<(ostream &os, const Steam &store) { // вывод на экран
        os << "Game name: " << store.m_name << ", Genre: " << store.m_genre << ", Price: " << store.m_price;
        return os;
    }
};

// Инициализация статических членов класса
int Steam::count = 0;

class UserGameLibrary : public Steam {
    Steam *m_library; // динамический массив игр
    string l_name; //имя библиотеки
    int m_librarySize; // размер библиотеки
    int m_numGames = 0; // количество игр в библиотеке
public:

    UserGameLibrary() {
        m_library = nullptr;
        m_librarySize = 0;
        m_numGames = 0;
        inLibrary = true;
        Steam::count--;
    }

    virtual ~UserGameLibrary() {
        delete[] m_library;
    }

    int getSize() const {
        return m_librarySize;
    }

    explicit UserGameLibrary(string name = "", int librarySize = 0, int price = 0, string genre = "") : Steam(name, price, genre) { //создаем объект с помощью базового конструктора
        inLibrary = true;
        m_librarySize = librarySize;
        m_library = new Steam[m_librarySize];
        Steam::count--;
    }

    UserGameLibrary(const UserGameLibrary &other) : Steam(other) { //копирование
        m_librarySize = other.m_librarySize;
        m_library = new Steam[m_librarySize];
        inLibrary = true;
        for (int i = 0; i < m_librarySize; i++) {
            m_library[i] = Steam(other.m_library[i]);
        }
    }

    UserGameLibrary &operator=(const UserGameLibrary &other) { //перегрузка операции =
        if (this != &other) {
            inLibrary = true;
            delete[] m_library;
            m_library = new Steam[other.m_librarySize];
            m_librarySize = other.m_librarySize;
            m_numGames = other.m_numGames;
            for (int i = 0; i < m_librarySize; i++) {
                m_library[i] = Steam(other.m_library[i]);
            }
        }
        return *this;
    }

    // Методы для добавления и удаления игры из библиотеки
    void addGame(Steam &game) {
        if (m_numGames < m_librarySize) {
            m_library[m_numGames] = game;
            game.isInLibrary(true);
            game.setLib(m_numGames);
            m_numGames++;
            cout << game.getName() << " has been added to the library." << endl;
        } else {
            cout << "The library is full. Cannot add game " << game.getName() << endl;
        }
    }

    void removeGame(Steam &game) {
        if(game.getInLibrary()) {
            int index = game.getLib();
            game.isInLibrary(false);
            if (index >= 0 && index <= m_numGames) {
                m_library[index].isInLibrary(false);
                cout << "Game " << m_library[index].getName() << " has been removed from the library." << endl;
                for (int i = index; i < m_numGames - 1; i++) {
                    m_library[i] = m_library[i + 1];
                }
                m_numGames--;
            } else {
                throw out_of_range("Index out of range!"); //кидаемся исключением "вне массива"
            }
        } else {
            throw out_of_range("Index out of range!"); //кидаемся исключением "вне массива"
        }
    }

    int getNumGames() const {
        return m_numGames;
    }

    // Перегрузка операции вывода на экран
    friend ostream &operator<<(ostream &os, const UserGameLibrary &library) {
        os << "Library of user " << library.getName() << ":" << endl;
        for (int i = 0; i < library.m_numGames; i++) {
            os << library.m_library[i] << endl; //вызываем операцию вывода для базового класса
        }
        return os;
    }

    Steam &operator[](int index) const {
        if (index < 0 || index >= m_numGames) {
           throw out_of_range("Index out of range!"); //кидаемся исключением "вне массива"
        }
        return m_library[index];
    }

};

int main() {
    try {
        // Создаем "игры" класса Steam для добавления в библиотеку
        Steam game1 = Steam("Game 1", 0, "Default");
        Steam game2 = Steam("Minecraft", 5, "Sandbox");
        Steam game3 = Steam("RDR 2", 15, "Action-adventure");
        Steam game4 = Steam("Hollow Knight", 3, "Platformer");
        Steam game5 = Steam("Wallpaper Engine", 2, "Utility");

        cout << "Количество игр в Steam: " << Steam::count << endl;

        //Инициализируем библиотек
        UserGameLibrary myLibrary("Denis", 4);
        UserGameLibrary yourLibrary("default_username", 15);

        //Добавляем игры
        myLibrary.addGame(game1);
        myLibrary.addGame(game2);
        myLibrary.addGame(game3);
        myLibrary.addGame(game4);
        myLibrary.addGame(game5);
        cout << endl;

        yourLibrary = myLibrary;

        //Задаем параметры у "дефолтной" игры
        yourLibrary[0].setName("Tetris");
        yourLibrary[0].setPrice(1);
        yourLibrary[0].setGenre("Arcade");
        cout << "Название: " << yourLibrary[0].getName() << endl;
        cout << "Цена: " << yourLibrary[0].getPrice() << endl;
        cout << "Жанр: " << yourLibrary[0].getGenre() << endl;

        cout << endl;

        cout << "Максимальный размер библиотеки у пользователя " << myLibrary.getName() << " - " << myLibrary.getSize() << endl;

        cout << endl;

        for (int i = 1; i < myLibrary.getNumGames(); i++) { //выводим содержимое библиотеки с какого-то индекса по какой-то
            cout << "Название игры: " << myLibrary[i].getName() << endl;
            cout << "Цена игры: " << myLibrary[i].getPrice() << endl;
        }

        cout << endl;

        myLibrary.removeGame(game1); // Удаляем игру RDR2

        cout << endl;

        if(game1.getInLibrary()){
            cout << "Игра " << game1.getName() << " в библиотеке" << endl;
        } else {
            cout << "Игра " << game1.getName() << " не в библиотеке" << endl;
        }

        cout << endl;
        cout << myLibrary << endl;// Выводим полностью обновленное содержимое библиотеки на экран
        cout << endl;
        cout << yourLibrary << endl;
    } catch (const std::out_of_range &e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
