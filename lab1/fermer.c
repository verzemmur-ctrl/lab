#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 64
#define MAX_FARMER_NAME 33
#define MAX_SUPPLY_ITEMS 128
#define MAX_SUPPLY_NAME 64

int current_day = 1;
int current_hour = 8;
int inventory[INVENTORY_SIZE] = {0};

char item_names[MAX_ITEMS][MAX_NAME_LEN];
char farmer_name[MAX_FARMER_NAME] = "Фермер";

/* Названия по умолчанию, если items.txt не найден */
const char* default_item_names[MAX_ITEMS] = {
    "Пусто",
    "Дерево",
    "Камень",
    "Семена",
    "Ягода",
    "Палка",
    "Гриб",
    "Яблоко",
    "Яйцо",
    "Ткань"
};

void see_time();
void work_hours();
void view_inventory();
void put_item();
void remove_item();
void clear_trash();
void compress_inventory();
void search_item_by_name();
void write_diary();
void process_supplies();
void load_items_catalog();
void ask_farmer_name();
void print_header();

int main() {
    ask_farmer_name();
    load_items_catalog();

    int choice;
    while (1) {
        print_header();
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Очистка от мусора\n");
        printf("[7] Сжать инвентарь (сортировка рюкзака)\n");
        printf("[8] Поиск предмета в рюкзаке по названию\n");
        printf("[9] Записать состояние в дневник фермера (diary.txt)\n");
        printf("[10] Подсчёт ресурсов (input.txt -> output.txt)\n");
        printf("Выберите пункт: ");
        if (scanf("%d", &choice) != 1) {
            /* защита от нечислового ввода */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Некорректный выбор.\n");
            continue;
        }

        switch (choice) {
            case 0:
                printf("Выход из программы.\n");
                return 0;
            case 1: see_time(); break;
            case 2: work_hours(); break;
            case 3: view_inventory(); break;
            case 4: put_item(); break;
            case 5: remove_item(); break;
            case 6: clear_trash(); break;
            case 7: compress_inventory(); break;
            case 8: search_item_by_name(); break;
            case 9: write_diary(); break;
            case 10: process_supplies(); break;
            default:
                printf("Некорректный выбор.\n");
        }
    }
    return 0;
}

/* ---------- Имя фермера ---------- */
void ask_farmer_name() {
    char buf[MAX_FARMER_NAME + 2];
    printf("Введите имя фермера (до %d символов): ", MAX_FARMER_NAME);
    if (fgets(buf, sizeof(buf), stdin)) {
        /* убираем перевод строки */
        size_t len = strlen(buf);
        while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) {
            buf[--len] = '\0';
        }
        if (len == 0) {
            strcpy(farmer_name, "Фермер");
        } else {
            /* обрезаем до MAX_FARMER_NAME */
            if (len > MAX_FARMER_NAME) buf[MAX_FARMER_NAME] = '\0';
            strcpy(farmer_name, buf);
        }
    } else {
        strcpy(farmer_name, "Фермер");
    }
    printf("Здравствуйте, %s!\n", farmer_name);
}

void print_header() {
    printf("\n=== Ферма: %s | День %d, %02d:00 ===\n",
           farmer_name, current_day, current_hour);
}

/* ---------- Каталог предметов ---------- */
void load_items_catalog() {
    FILE *f = fopen("items.txt", "r");
    int loaded = 0;
    if (f) {
        char line[256];
        int id = 0;
        while (id < MAX_ITEMS && fgets(line, sizeof(line), f)) {
            /* убираем перевод строки и возможные \r */
            size_t len = strlen(line);
            while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
                line[--len] = '\0';
            }
            if (len == 0) continue; /* пропускаем пустые строки */

            /* Формат строки: "0 Пусто" или "0 Пусто ..." */
            /* Берём первую цифру как ID, если она есть, иначе порядковый номер */
            int parsed_id = id;
            char *p = line;
            while (*p == ' ' || *p == '\t') p++;
            if (isdigit((unsigned char)*p)) {
                parsed_id = *p - '0';
                p++;
                while (*p == ' ' || *p == '\t') p++;
                if (parsed_id < 0 || parsed_id >= MAX_ITEMS) {
                    /* ID вне диапазона — всё равно попробуем как порядковый */
                    parsed_id = id;
                }
            } else {
                /* нет ID в начале — используем текущий порядковый номер */
            }

            /* Ограничиваем длину названия */
            if (strlen(p) >= MAX_NAME_LEN) p[MAX_NAME_LEN - 1] = '\0';
            strncpy(item_names[parsed_id], p, MAX_NAME_LEN - 1);
            item_names[parsed_id][MAX_NAME_LEN - 1] = '\0';
            loaded++;
            id++;
        }
        fclose(f);
    }

    if (!f || loaded == 0) {
        printf("Внимание: файл items.txt не найден или пуст. "
               "Используются названия по умолчанию.\n");
        for (int i = 0; i < MAX_ITEMS; i++) {
            strncpy(item_names[i], default_item_names[i], MAX_NAME_LEN - 1);
            item_names[i][MAX_NAME_LEN - 1] = '\0';
        }
        return;
    }

    /* Дозаполняем недостающие позиции значениями по умолчанию */
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (item_names[i][0] == '\0') {
            strncpy(item_names[i], default_item_names[i], MAX_NAME_LEN - 1);
            item_names[i][MAX_NAME_LEN - 1] = '\0';
        }
    }
}

/* ---------- Базовые функции (как были) ---------- */
void see_time() {
    printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
}

void work_hours() {
    int hours;
    printf("Введите количество часов для работы: ");
    if (scanf("%d", &hours) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Некорректное значение часов.\n");
        return;
    }
    if (hours < 0) {
        printf("Некорректное значение часов.\n");
        return;
    }
    current_hour += hours;
    while (current_hour >= 24) {
        current_hour -= 24;
        current_day++;
    }
}

void view_inventory() {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = inventory[i];
        printf("Слот %d: [%d]", i, id);
        if (id >= 0 && id < MAX_ITEMS) {
            printf(" - %s", item_names[id]);
        }
        printf("\n");
    }
}

void put_item() {
    int index, item_id;
    printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &index) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Некорректный индекс.\n");
        return;
    }
    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Некорректный индекс.\n");
        return;
    }
    printf("Введите ID предмета (0-%d): ", MAX_ITEMS - 1);
    if (scanf("%d", &item_id) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Некорректный ID предмета.\n");
        return;
    }
    if (item_id < 0 || item_id >= MAX_ITEMS) {
        printf("Некорректный ID предмета.\n");
        return;
    }
    inventory[index] = item_id;
    printf("Предмет %s добавлен в слот %d.\n", item_names[item_id], index);
}

void remove_item() {
    int index;
    printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &index) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Некорректный индекс.\n");
        return;
    }
    if (index < 0 || index >= INVENTORY_SIZE) {
        printf("Некорректный индекс.\n");
        return;
    }
    inventory[index] = 0;
    printf("Предмет удалён из слота %d.\n", index);
}

void clear_trash() {
    int item_id, count = 0;
    printf("Введите ID предмета для удаления: ");
    if (scanf("%d", &item_id) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Некорректный ID.\n");
        return;
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == item_id) {
            inventory[i] = 0;
            count++;
        }
    }
    printf("Удалено %d предметов.\n", count);
}

void compress_inventory() {
    printf("Инвентарь ДО сжатия:\n");
    view_inventory();

    int write = 0;
    for (int read = 0; read < INVENTORY_SIZE; read++) {
        if (inventory[read] != 0) {
            inventory[write] = inventory[read];
            write++;
        }
    }
    while (write < INVENTORY_SIZE) {
        inventory[write] = 0;
        write++;
    }

    printf("Инвентарь ПОСЛЕ сжатия:\n");
    view_inventory();
}

/* ---------- [8] Поиск по названию ---------- */
void search_item_by_name() {
    char query[MAX_NAME_LEN];
    printf("Введите название предмета для поиска: ");

    /* очищаем остатки ввода */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    if (!fgets(query, sizeof(query), stdin)) {
        printf("Ошибка чтения названия.\n");
        return;
    }
    size_t len = strlen(query);
    while (len > 0 && (query[len-1] == '\n' || query[len-1] == '\r')) {
        query[--len] = '\0';
    }
    if (len == 0) {
        printf("Название не введено.\n");
        return;
    }

    int found_id = -1;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(item_names[i], query) == 0) {
            found_id = i;
            break;
        }
    }

    if (found_id == -1) {
        printf("Такого предмета нет в каталоге.\n");
        return;
    }

    printf("Поиск '%s' (ID %d) в рюкзаке...\n", query, found_id);
    int count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == found_id) {
            printf("  Найдено в слоте %d\n", i);
            count++;
        }
    }
    if (count == 0) {
        printf("В рюкзаке такого предмета нет.\n");
    } else {
        printf("Всего найдено: %d\n", count);
    }
}

/* ---------- [9] Дневник ---------- */
void write_diary() {
    FILE *f = fopen("diary.txt", "a");
    if (!f) {
        printf("Не удалось открыть diary.txt для записи.\n");
        return;
    }

    fprintf(f, "=== Дневник фермера ===\n");
    fprintf(f, "Имя: %s\n", farmer_name);
    fprintf(f, "Время: День %d, %02d:00\n", current_day, current_hour);
    fprintf(f, "Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = inventory[i];
        const char *name = (id >= 0 && id < MAX_ITEMS) ? item_names[id] : "?";
        fprintf(f, "  Слот %d: [%d] - %s\n", i, id, name);
    }
    fprintf(f, "\n");
    fclose(f);

    printf("Состояние записано в diary.txt.\n");
}

/* ---------- [10] Подсчёт ресурсов ---------- */
void process_supplies() {
    FILE *in = fopen("input.txt", "r");
    if (!in) {
        printf("Не удалось открыть input.txt.\n");
        return;
    }

    char names[MAX_SUPPLY_ITEMS][MAX_SUPPLY_NAME];
    int counts[MAX_SUPPLY_ITEMS];
    int unique = 0;

    char name[MAX_SUPPLY_NAME];
    int qty;

    /* Читаем пары Название Количество */
    while (fscanf(in, "%63s %d", name, &qty) == 2) {
        int found = -1;
        for (int i = 0; i < unique; i++) {
            if (strcmp(names[i], name) == 0) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            if (unique < MAX_SUPPLY_ITEMS) {
                strncpy(names[unique], name, MAX_SUPPLY_NAME - 1);
                names[unique][MAX_SUPPLY_NAME - 1] = '\0';
                counts[unique] = qty;
                unique++;
            }
        } else {
            counts[found] += qty;
        }
    }
    fclose(in);

    FILE *out = fopen("output.txt", "w");
    if (!out) {
        printf("Не удалось открыть output.txt для записи.\n");
        return;
    }

    fprintf(out, "Сводная ведомость поставок:\n");
    for (int i = 0; i < unique; i++) {
        fprintf(out, "%s: %d шт.\n", names[i], counts[i]);
    }
    fclose(out);

    printf("Ведомость записана в output.txt. Позиций: %d\n", unique);
}