#include <iostream> // Підключення бібліотеки введення/виведення
#include <vector> // Підключення бібліотеки для використання векторів
#include <cstdlib> // Підключення бібліотеки для використання функції rand()
#include <ctime> // Підключення бібліотеки для використання функції time()
#include <unistd.h> // Підключення бібліотеки для використання функції usleep()
#include <ncurses.h> // Підключення бібліотеки для використання функцій ncurses
#include <limits> // Підключення бібліотеки для використання обмежень числових типів
#include <algorithm> // Підключення бібліотеки для використання алгоритмів


const int BOARD_WIDTH = 20; // Ширина ігрового поля
const int BOARD_HEIGHT = 20; // Висота ігрового поля

struct Coordinate {
    int x; // Координата x
    int y; // Координата y

    bool operator==(const Coordinate& other) const { // Перевантажений оператор порівняння
        return x == other.x && y == other.y;
    }
};

struct GameState {
    bool gameOver; // Прапорець, що позначає закінчення гри
    int score; // Рахунок гравця
    Coordinate direction; // Напрямок руху гравця
    std::vector<Coordinate> snake; // Вектор, що містить координати сегментів змійки
    Coordinate apple; // Координати яблука
};

enum class MenuOption { // Перерахування з варіантами меню
    Play, // Грати
    Quit // Вийти
};

int getNumericInput() { // Функція для отримання числового введення
    int numericInput;
    std::cin >> numericInput;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return numericInput;
}

MenuOption showMainMenu() { // Функція для відображення головного меню
    std::cout << "=== Головне меню ===" << std::endl;
    std::cout << "1. Грати" << std::endl;
    std::cout << "2. Вийти" << std::endl;
    std::cout << "Введіть ваш вибір: ";

    int choice = getNumericInput(); // Отримання числового введення
    switch (choice) { // Перевірка вибору користувача
        case 1:
            return MenuOption::Play;
        case 2:
            return MenuOption::Quit;
        default:
            return MenuOption::Quit;
    }
}

void showGameOverMenu(GameState& gameState) { // Функція для відображення меню після закінчення гри
    std::cout << "=== Гра завершена! ===" << std::endl;
    std::cout << "Ваш рахунок: " << gameState.score << std::endl;
    std::cout << "1. Почати заново" << std::endl;
    std::cout << "2. Вийти" << std::endl;
    std::cout << "Введіть ваш вибір: ";

    int choice = getNumericInput(); // Отримання числового введення
    switch (choice) { // Перевірка вибору користувача
        case 1: {
            gameState.gameOver = false;
            gameState.score = 0;
            gameState.direction = {0, 1};
            gameState.snake.clear();
            int startX = BOARD_WIDTH / 2;
            int startY = BOARD_HEIGHT / 2;
            gameState.snake.push_back({startX, startY});
            gameState.snake.push_back({startX, startY - 1});
            gameState.snake.push_back({startX, startY - 2});
            gameState.apple = {rand() % (BOARD_WIDTH - 2) + 1, rand() % (BOARD_HEIGHT - 2) + 1};
            break;
        }
        case 2:
            gameState.gameOver = true;
            break;
        default:
            gameState.gameOver = true;
            break;
    }
}

void initialize(GameState& gameState) { // Ініціалізація початкового стану гри
    gameState.gameOver = false;
    gameState.score = 0;
    gameState.direction = {0, 1};

    int startX = BOARD_WIDTH / 2;
    int startY = BOARD_HEIGHT / 2;
    gameState.snake.push_back({startX, startY});
    gameState.snake.push_back({startX, startY - 1});
    gameState.snake.push_back({startX, startY - 2});

    gameState.apple = {rand() % (BOARD_WIDTH - 2) + 1, rand() % (BOARD_HEIGHT - 2) + 1};

    initscr(); // Ініціалізація ncurses
    cbreak(); // Вимкнення буферизації введення
    noecho(); // Вимкнення виводу введеного тексту
    nodelay(stdscr, true); // Неочікуване отримання вводу
    keypad(stdscr, true); // Включення режиму отримання спеціальних клавіш
    curs_set(0); // Приховування курсора
}

void cleanup() { // Завершення роботи з ncurses
    endwin();
}

void render(const GameState& gameState) { // Відображення гри на екрані
    clear(); // Очищення екрану

    for (int x = 0; x < BOARD_WIDTH; ++x) {
        mvprintw(0, x, "#"); // Відображення верхньої межі поля
        mvprintw(BOARD_HEIGHT - 1, x, "#"); // Відображення нижньої межі поля
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        mvprintw(y, 0, "#"); // Відображення лівої межі поля
        mvprintw(y, BOARD_WIDTH - 1, "#"); // Відображення правої межі поля
    }

    for (const Coordinate& segment : gameState.snake) {
        mvprintw(segment.y, segment.x, "O"); // Відображення сегментів змійки
    }

    mvprintw(gameState.apple.y, gameState.apple.x, "@"); // Відображення яблука

    mvprintw(BOARD_HEIGHT, 0, "Рахунок: %d", gameState.score); // Відображення рахунку гравця

    refresh(); // Оновлення екрану
}

void handleInput(GameState& gameState) { // Обробка введення гравця
    int input = getch(); // Отримання введення
    switch (input) { // Перевірка введення
        case KEY_UP:
            if (gameState.direction.y != 1)
                gameState.direction = {0, -1}; // Зміна напрямку вгору
            break;
        case KEY_DOWN:
            if (gameState.direction.y != -1)
                gameState.direction = {0, 1}; // Зміна напрямку вниз
            break;
        case KEY_LEFT:
            if (gameState.direction.x != 1)
                gameState.direction = {-1, 0}; // Зміна напрямку вліво
            break;
        case KEY_RIGHT:
            if (gameState.direction.x != -1)
                gameState.direction = {1, 0}; // Зміна напрямку вправо
            break;
        case 'q':
            gameState.gameOver = true; // Закінчення гри
            break;
    }
}

void update(GameState& gameState) { // Оновлення стану гри
    Coordinate head = gameState.snake.front();
    Coordinate newHead = {head.x + gameState.direction.x, head.y + gameState.direction.y};
    gameState.snake.insert(gameState.snake.begin(), newHead); // Додавання нової голови змійки
    gameState.snake.pop_back(); // Видалення останнього сегмента змійки

    if (newHead.x == gameState.apple.x && newHead.y == gameState.apple.y) { // Перевірка зіткнення з яблуком
        gameState.score++; // Збільшення рахунку
        gameState.apple = {rand() % (BOARD_WIDTH - 2) + 1, rand() % (BOARD_HEIGHT - 2) + 1}; // Генерація нового яблука
        gameState.snake.push_back({}); // Додавання нового сегмента змійки
    }

    // Перевірка зіткнення зі стіною або самою собою
    if (newHead.x <= 0 || newHead.x >= BOARD_WIDTH - 1 || newHead.y <= 0 || newHead.y >= BOARD_HEIGHT - 1 ||
        std::find(gameState.snake.begin() + 1, gameState.snake.end(), newHead) != gameState.snake.end()) {
        gameState.gameOver = true; // Закінчення гри
    }
}

void runGameLoop() { // Головний цикл гри
    GameState gameState;
    initialize(gameState); // Ініціалізація гри

    while (!gameState.gameOver) {
        render(gameState); // Відображення гри
        handleInput(gameState); // Обробка введення
        update(gameState); // Оновлення гри
        usleep(100000); // Затримка на 100 мілісекунд
    }

    cleanup(); // Завершення роботи з ncurses
    showGameOverMenu(gameState); // Відображення меню після закінчення гри
}

int main() { // Головна функція
    srand(static_cast<unsigned>(time(nullptr))); // Ініціалізація генератора випадкових чисел
    MenuOption option = showMainMenu(); // Відображення головного меню

    if (option == MenuOption::Play) { // Перевірка вибору користувача
        runGameLoop(); // Запуск головного циклу гри
    }

    return 0;
}
