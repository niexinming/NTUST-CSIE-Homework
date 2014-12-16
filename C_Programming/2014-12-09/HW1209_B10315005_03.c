#include <stdio.h>
#include <math.h>

typedef struct Point_
{
    double x;
    double y;
} Point;

double poly_area(Point *points, int count);

int main()
{
    int i, n;

    printf("How many points? ");
    scanf("%d", &n);

    Point *points = malloc(sizeof(Point) * n);

    for (i = 0; i < n; i++) {
        printf("Input point #%d: ", i + 1);
        scanf("%lf%lf", &points[i].x, &points[i].y);
    }

    printf("Area = %g\n", poly_area(points, n));

    free(points);

    return 0;
}

double poly_area(Point *points, int count)
{
    int i;
    double sum = 0;

    for (i = 0; i < count; i++)
        sum += (points[i].x + points[(i + 1) % count].x) *
               (points[(i + 1) % count].y - points[i].y);

    return fabs(sum) / 2;
}
