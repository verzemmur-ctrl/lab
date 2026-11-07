#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// объявление переменных
#define INVENTORY_SIZE 10
#define EMPTY_SLOT 0
#define WOOD 1
#define STONE 2
#define SEEDS 3
#define MILK 4
#define BREAD 5
#define WOOL 6
#define IRON 7
#define GOLD 8
#define FISH 9
#define MAX_ITEM_ID FISH   // максимальный айди предмета

#define HOURS_PER_DAY 24

int current_day = 1;
int current_time = 8;

// инвентарь 
int inventory[INVENTORY_SIZE] = {WOOD, WOOD, WOOD, SEEDS, SEEDS, STONE, STONE, IRON, IRON, EMPTY_SLOT};

// очистка экрана
void clearScreen(void) {
#ifdef _WIN32
    system("cls");   // windows
#else
    system("clear"); // linux / mac os
#endif
}

// проверка корректности ввода
int getValidatedInput() {
    int value;
    while (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        printf("Ошибка ввода. Введите число: ");
    }
    while (getchar() != '\n');
    return value;
}

// вывод предмета по айди
void printItemName(int id) {
    switch (id) {
        case EMPTY_SLOT: printf("Пусто");       break;
        case WOOD:       printf("Дерево");      break;
        case STONE:      printf("Камень");      break;
        case SEEDS:      printf("Семена");      break;
        case MILK:       printf("Молоко");      break;
        case BREAD:      printf("Хлеб");        break;
        case WOOL:       printf("Шерсть");      break;
        case IRON:       printf("Железо");      break;
        case GOLD:       printf("Золото");      break;
        case FISH:       printf("Рыба");        break;
        default:         printf("Неизвестный"); break;
    }
}

// вывод инвенторя
void showInventory(void) {
    printf("\nИНВЕНТАРЬ\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d]", i, inventory[i]);
        printf(" (");
        printItemName(inventory[i]);
        printf(")");
        printf("\n");
    }
}

// положить предмет
void putItemInSlot(void) {
    printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
    int slot = getValidatedInput();

    if (slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Ошибка: индекс слота должен быть от 0 до %d!\n", INVENTORY_SIZE - 1);
        return;
    
    }
    
    printf("Введите айди предмета (0-%d): ", MAX_ITEM_ID);
    int item_id = getValidatedInput();
    
    if (item_id < 0 || item_id > MAX_ITEM_ID) {
        printf("Ошибка: айди предмета должен быть от 0 до %d!\n", MAX_ITEM_ID);
        return;
    }
    
    inventory[slot] = item_id;
    
    printf("Предмет ");
    printItemName(item_id);
    printf(" (ID: %d) помещён в слот %d.\n", item_id, slot);
}

// 3 вариант. инверсия карманов
void invertInventory(void) {
    printf("\nИнверсия карманов\n");
    
    // до преобразования
    printf("\nИнвентарь до\n");
    showInventory();
    
    // переворот
    for (int i = 0, j = INVENTORY_SIZE - 1; i < j; i++, j--) {
        int temp = inventory[i];
        inventory[i] = inventory[j];
        inventory[j] = temp;
    }
    
    // итог
    printf("\nИнвентарь ПОСЛЕ\n");
    showInventory();
    
    printf("Инвентарь перевернут.\n");
}

// константы
#define INV_SIZE 10
#define MAX_ITEMS 10
#define STR_LEN 256
#define NAME_LEN 32

// глобальные переменные
char farmer_name[NAME_LEN];
char item_names[MAX_ITEMS][NAME_LEN];

// очистка буфера ввода
void clear_buf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// загрузка из файла
void load_items() {
    FILE *f = fopen("items.txt", "r");
    
    if (f == NULL) {
        printf("Внимание: items.txt не найден. Используем запасные названия.\n");
        strcpy(item_names[0], "Пусто");
        strcpy(item_names[1], "Дерево");
        strcpy(item_names[2], "Камень");
        strcpy(item_names[3], "Семена");
        strcpy(item_names[4], "Железо");
        strcpy(item_names[5], "Золото");
        for (int i = 6; i < MAX_ITEMS; i++) {
            strcpy(item_names[i], "Ресурс");
        }
        return;
    } else {

        int id;
        char name[NAME_LEN];
        while (fscanf(f, "%d %31[^\n]", &id, name) == 2) {
            name[strcspn(name, "\r\n")] = '\0';
            
            if (id >= 0 && id < MAX_ITEMS) {
                strcpy(item_names[id], name);
                id++;
            }
        }
        fclose(f);
    }
}

// показ инвентаря
void show_inv(int inv[]) {
    printf("\n--- Рюкзак ---\n");
    for (int i = 0; i < INV_SIZE; i++) {
        int id = inv[i];
        printf("Слот %d: [%d] - %s\n", i, id, item_names[id]);
    }
}

// поиск предмета по имени
void find_item(int inv[]) {
    char search[NAME_LEN];
    printf("Введите название предмета: ");
    scanf("%31[^\n]", search);
    clear_buf();

    int found_id = -1;
    
    // ищем айди по названию
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(item_names[i], search) == 0) {
            found_id = i;
            break;
        }
    }

    if (found_id == -1) {
        printf("Такого предмета нет в каталоге.\n");
        return;
    }

    // ищем этот айди в рюкзаке
    int found = 0;
    printf("Предмет '%s' найден в слотах: ", search);
    for (int i = 0; i < INV_SIZE; i++) {
        if (inv[i] == found_id) {
            printf("%d ", i);
            found = 1;
        }
    }
    
    if (found == 0) {
        printf("нет (пусто)");
    }
    printf("\n");
}

// запись в дневник
void save_diary(int day, int hour, int inv[]) {
    // открываем на дозапись a
    FILE *f = fopen("diary.txt", "a");
    if (f == NULL) {
        printf("Не удалось открыть diary.txt\n");
        return;
    }

    fprintf(f, "\nФермер: %s\n", farmer_name);
    fprintf(f, "День: %d, Время: %d:00\n", day, hour);
    fprintf(f, "Инвентарь:\n");
    
    for (int i = 0; i < INV_SIZE; i++) {
        fprintf(f, "  Слот %d: [%d] - %s\n", i, inv[i], item_names[inv[i]]);
    }
    
    fclose(f);
    printf("Запись успешно добавлена в diary.txt\n");
}

// 1 вариант: разбор конфига
void parse_config() {
    FILE *fin = fopen("input.txt", "r");
    if (fin == NULL) {
        printf("Ошибка: файл input.txt не найден!\n");
        return;
    }

    FILE *fout = fopen("output.txt", "w");
    if (fout == NULL) {
        printf("Ошибка: не удалось создать output.txt\n");
        fclose(fin);
        return;
    }

    char line[STR_LEN];
    int ok_count = 0;
    int skip_count = 0;

    printf("\n--- Разбор конфига ---\n");

    // чит. файл построчно
    while (fgets(line, STR_LEN, fin) != NULL) {
        int len = strlen(line);
        
        // убирает перенос строки вручную без strcspn
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }

        if (len == 0 || line[0] == '#') {
            skip_count++;
            continue;
        }

        int eq_pos = -1;
        for (int i = 0; i < len; i++) {
            if (line[i] == '=') {
                eq_pos = i;
                break;
            }
        }

        if (eq_pos == -1 || eq_pos == 0 || eq_pos == len - 1) {
            skip_count++;
            continue;
        }

        line[eq_pos] = '\0'; // ключ
        char *key = line;
        char *value = &line[eq_pos + 1]; // значение

        // вывод на экран и в файл
        printf("Параметр: [%s] | Значение: [%s]\n", key, value);
        fprintf(fout, "Параметр: [%s] | Значение: [%s]\n", key, value);
        
        ok_count++;
    }

    // итоги
    printf("\nСчитано настроек: %d\n", ok_count);
    printf("Пропущено строк: %d\n", skip_count);
    
    fprintf(fout, "\nСчитано настроек: %d\n", ok_count);
    fprintf(fout, "Пропущено строк: %d\n", skip_count);

    fclose(fin);
    fclose(fout);
    printf("Результат также сохранен в output.txt\n");
}

// константы 
#define WALL '#'
#define FLOOR '.'
#define PLAYER '@'
#define TRAP '^'
#define UP 'U'
#define DOWN 'D'

// переменные
char **map = NULL;
int h = 0, w = 0; // размер карты
int py = 1, px = 1; // корды игрока
int exit_y = 1, exit_x = 1; // корды лестницы вниз (выход)
int floor_num = 1;

// очистка экрана
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}



// установка элемента на случайную пустую клетку
void place_random(char symbol) {
    int placed = 0;
    while (!placed) {
        int y = rand() % (h - 2) + 1;
        int x = rand() % (w - 2) + 1;
        if (map[y][x] == FLOOR) {
            map[y][x] = symbol;
            placed = 1;
        }
    }
}

// генерация нового этажа (с очисткой старой памяти)
void generate_floor() {
    // очистка памяти от предыдущего этажа
    if (map != NULL) {
        for (int i = 0; i < h; i++) free(map[i]);
            free(map);
    }

    // случайный размер
    h = rand() % 9 + 7;
    w = rand() % 9 + 7;

    // выделение памяти
    map = (char**)malloc(h * sizeof(char*));
    for (int i = 0; i < h; i++) {
        map[i] = (char*)malloc(w * sizeof(char));
        for (int j = 0; j < w; j++) {
        // стены по краям, пол внутри
            map[i][j] = (i == 0 || i == h-1 || j == 0 || j == w-1) ? WALL : FLOOR;
        }
    }

    // лестница вниз (под игроком на старте)
    exit_y = 1; exit_x = 1;
    map[exit_y][exit_x] = DOWN;
    py = exit_y; px = exit_x;

    // ставим 1 лестницу наверх и 3 ловушки
    place_random(UP);
    place_random(TRAP);
    place_random(TRAP);
    place_random(TRAP);
}

// отрисовка карты
void draw() {
    clear_screen();
    printf("=== БАШНЯ. Этаж: %d ===\n", floor_num);
    printf("W/A/S/D - ходить, Q - выход (только на 'D')\n");
    printf("Руны '^' телепортируют\n\n");

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i == py && j == px) {
                // если на старте, показываем игрока, иначе D
                printf("%c ", (i == exit_y && j == exit_x && floor_num == 1) ? PLAYER : PLAYER);
            } else {
                printf("%c ", map[i][j]);
            }
        }
        printf("\n");
    }
}

// тп на ловушке
void teleport() {
    int empty_cells = 0;
    // подсчет свободных клеток
    for (int i = 1; i < h - 1; i++)
        for (int j = 1; j < w - 1; j++)
            if (map[i][j] == FLOOR) empty_cells++;

    if (empty_cells == 0) return; // некуда тп

    // ищем рандом клетку
    int target = rand() % empty_cells;
    int count = 0;

    for (int i = 1; i < h - 1; i++) {
        for (int j = 1; j < w - 1; j++) {
            if (map[i][j] == FLOOR) {
                if (count == target) {
                    py = i; px = j;
                    return;
                }
                count++;
            }
        }
    }
}

// основной цикл
void playTower() {
    char move;
    generate_floor();

    while (1) {
        draw();
        printf("Ход: ");
        scanf(" %c", &move); // пробел перед %c игнорирует энтер от предыдущего ввода
        clear_buf();
        move = tolower(move);

        int ny = py, nx = px; // новые корды

        if (move == 'w') ny--;
        else if (move == 's') ny++;
        else if (move == 'a') nx--;
        else if (move == 'd') nx++;
        else if (move == 'q') {
            // проверка стоим ли на выходе
            if (py == exit_y && px == exit_x) {
                printf("\nВы сбежали домой!\n");
                return;
            } else {
                printf("\nВыход (Q) работает только на лестнице вниз 'D'!\nНажмите Enter...");
                clear_buf();
                continue;
            }
        } else continue; // неверная клавиша

        // проверка, не врезались ли мы в стену
        if (map[ny][nx] != WALL) {
            // игрок ходит
            py = ny; px = nx;

            // проверка на взаимодействие с объектами
            if (map[py][px] == TRAP) {
                map[py][px] = FLOOR; // Руна исчезает
                printf("\nНаступили на руну! Телепортация...\nНажмите Enter...");
                clear_buf();
                teleport();
            }
            else if (map[py][px] == UP) {
                printf("\nПереход на следующий этаж. Нажмите Enter...\n");
                clear_buf();
                floor_num++;
                generate_floor();
            }
        }
    }
}

// глав функция
int main() {
    printf("Введите имя фермера: ");
    scanf("%31[^\n]", farmer_name);
    clear_buf();

    load_items();

    int day = 1;
    int hour = 8;
    int inv[INV_SIZE] = {1, 2, 3, 4, 5, 6, 0, 0, 0, 0}; 
    int choice = -1;

    while (choice != 0) {
        printf("\n=== Ферма (%s) ===\n", farmer_name);
        printf("0. Выход\n");
        printf("1. Время\n");
        printf("2. Поработать\n");
        printf("3. Инвентарь\n");
        printf("4. Положить предмет\n");
        printf("5. Выбросить предмет\n");
        printf("6. Инверсия\n");
        printf("7. Поиск по названию\n");
        printf("8. Записать в дневник\n");
        printf("9. Разобрать конфиг (Вариант 1)\n");
        printf("10. Отправиться в приключение (Башня)\n");
        printf("Выбор: ");

        // защита от букв
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода! Введите цифру.\n");
            clear_buf();
            choice = -1;
            continue;
        }
        clear_buf();

        switch (choice) {
            case 0:
                printf("Выход из игры.\n");
                break;
                
            case 1:
                printf("Сейчас День %d, %d:00\n", day, hour);
                break;
                
            case 2: {
                int h;
                printf("Сколько часов работать? ");
                if (scanf("%d", &h) == 1 && h >= 0) {
                    hour += h;
                    while (hour >= 24) {
                        hour -= 24;
                        day++;
                    }
                    printf("Время обновлено.\n");
                } else {
                    printf("Ошибка ввода.\n");
                }
                clear_buf();
                break;
            }
            
            case 3:
                show_inv(inv);
                break;
                
            case 4: {
                int slot, item;
                printf("Номер слота (0-9): ");
                if (scanf("%d", &slot) == 1 && slot >= 0 && slot < INV_SIZE) {
                    printf("ID предмета (0-9): ");
                    if (scanf("%d", &item) == 1 && item >= 0 && item < MAX_ITEMS) {
                        inv[slot] = item;
                        printf("Предмет добавлен.\n");
                    } else {
                        printf("Неверный ID.\n");
                    }
                } else {
                    printf("Неверный слот.\n");
                }
                clear_buf();
                break;
            }
            
            case 5: {
                int slot;
                printf("Номер слота для очистки (0-9): ");
                if (scanf("%d", &slot) == 1 && slot >= 0 && slot < INV_SIZE) {
                    inv[slot] = 0;
                    printf("Слот очищен.\n");
                } else {
                    printf("Неверный слот.\n");
                }
                clear_buf();
                break;
            }
            
            case 6: {
                // простая инверсия
                for (int i = 0; i < INV_SIZE / 2; i++) {
                    int temp = inv[i];
                    inv[i] = inv[INV_SIZE - 1 - i];
                    inv[INV_SIZE - 1 - i] = temp;
                }
                printf("Инвентарь перевернут.\n");
                show_inv(inv);
                break;
            }
            
            case 7:
                find_item(inv);
                break;
                
            case 8:
                save_diary(day, hour, inv);
                break;
                
            case 9:
                parse_config();
                break;

            case 10:
                playTower();
                break;
                
            default:
                printf("Нет такого пункта.\n");
        }
    }

    return 0;
}
