#include <stdio.h>

typedef struct {
    int month;
    int day;
    int year;
} date_t;

typedef struct {
    double full_gas;
    double current_gas;
} tank_t;

typedef struct {
    char make[32];
    char model[32];
    int odometer_reading;
    date_t manufacture;
    date_t purchase;
    tank_t tank;
} auto_t;

int scan_date(FILE *fp, date_t *date)
{
    return fscanf(fp, "%d%d%d", &date->month, &date->day, &date->year);
}

int scan_tank(FILE *fp, tank_t *tank)
{
    return fscanf(fp, "%lf%lf", &tank->full_gas, &tank->current_gas);
}

int scan_auto(FILE *fp, auto_t *auto_mobile)
{
    int r;
    r = fscanf(fp, "%s%s%d", auto_mobile->make, auto_mobile->model,
               &auto_mobile->odometer_reading);
    if (r == EOF) return EOF;
    r = scan_date(fp, &auto_mobile->manufacture);
    if (r == EOF) return EOF;
    r = scan_date(fp, &auto_mobile->purchase);
    if (r == EOF) return EOF;
    return scan_tank(fp, &auto_mobile->tank);
}

void print_date(const char *prefix, date_t *date)
{
    printf("%s%d/%d/%d\n", prefix ? prefix : "", date->day, date->month, date->year);
}

void print_tank(const char *prefix, tank_t *tank)
{
    printf("%s%g/%g\n", prefix ? prefix : "", tank->current_gas, tank->full_gas);
}

void print_auto(auto_t *auto_mobile)
{
    printf("Make: %s\n", auto_mobile->make);
    printf("Model: %s\n", auto_mobile->model);
    printf("Odometer reading: %d\n", auto_mobile->odometer_reading);
    print_date("Manufacture: ", &auto_mobile->manufacture);
    print_date("Purchase: ", &auto_mobile->purchase);
    print_tank("Tank: ", &auto_mobile->tank);
    puts("");
}

int main (int argc, const char *argv[])
{
    int i, n = 0;
    auto_t auto_mobile;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input-file.txt\n", argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Can not open file \"%s\"\n", argv[1]);
        return 2;
    }
    while (scan_auto(fp, &auto_mobile) != EOF) {
        print_auto(&auto_mobile);
    }
    fclose(fp);
    return 0;
}
