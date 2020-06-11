#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define tol 0.00001
#define pi 3.14159265359
struct list;
struct lmin;
//FUNCIONS MANIPULACIO LLISTES
int linies(char *);                                       //Introduïm el nom del fitxer i retorna les linies de vectors no nuls
bool llegirVectors(char *, int, struct list *llista);     //Llegeix els vectors del fitxer, si algun és nul retorna error
void imprimir(int, struct list *llista);                  //imprimeix per pantalla una llista de vectors de R^3
void imprimirMin(struct list *llista, struct lmin *vmin); //imprimeix els vectors mínims
void sortida(char *, struct list *llista, struct lmin *vmin);
//FUNCIONS BASIQUES
double norma(double[]);                                            //Retorna el mòdul d'un vector
double angle(double[], double[]);                                  //ANGLE sense signe
double sangle(double[], double[], double[]);                       //ANGLE SIGNE
void prodVectorial(double[], double[], double[]);                  //producte vectorial
double dotProduct(double[], double[]);                             //Producte escalar
double det(double[], double[], double[]);                          //determinant
bool base(double[], double[], double[]);                           //Retorna true si tres vectors son base de R3
void projOrtogonal(double[], double[], double[]);                  //projecció ortogonal
int projectacon(int, double[], struct list *v, struct list *proj); //Projectacon
//IMPORTANTS
int tipus2d(struct list *generadors, int, double[], struct lmin *vmin);
int tipus3d(struct list *generadors, int, struct lmin *vmin);
//STRUCTS
struct list
{
    double (*vect)[3];
    int len;
} list;
struct lmin
{
    int *l;
    int len;
} lmin;
//M
//A
//I
//N
int main(int argc, char *argv[])
{
    int n, resultat;
    struct list *generadors;
    struct lmin *vmin;
    int zero;
    if (argc != 3)
    {
        printf("No has introduit els arguments correctament\n");
        return 50;
    }
    n = linies(argv[1]);
    generadors = malloc(sizeof(list));
    if (generadors == NULL)
    {
        printf("No hi ha prou memoria\n");
        return 51;
    }

    vmin = malloc(sizeof(lmin));
    if (vmin == NULL)
    {
        printf("No hi ha prou memoria\n");
        return 51;
    }
    generadors->vect = malloc(n * sizeof(double[3]));
    if (generadors->vect == NULL)
    {
        printf("No hi ha prou memoria\n");
        return 51;
    }
    vmin->l = malloc(n * sizeof(double));
    if (vmin->l == NULL)
    {
        printf("No hi ha prou memoria\n");
        return 51;
    }
    generadors->len = 0;
    vmin->len = 0;
    zero = llegirVectors(argv[1], n, generadors);
    if (zero)
    {
        printf("Hi ha un vector zero, ERROR\n");
        return 50;
    }
    printf("\n\tClassificacio de cons\n");
    printf("\tHas introduit els vectors: \n");
    imprimir(n, generadors);
    resultat = tipus3d(generadors, n, vmin);
    if (resultat != 6)
    {
        printf("\n\tES UN CON DE TIPUS %i i  te %i minims\n", resultat, vmin->len);
    }
    else
    {
        printf("\n\t ES UN CON POLIEDRIC AMB %i ARESTES\n", vmin->len);
    }
    imprimirMin(generadors, vmin);
    sortida(argv[2], generadors, vmin);
    free(generadors->vect); //Alliberam mem�ria.
    free(generadors);       //Alliberam mem�ria.
    free(vmin->l);
    free(vmin);
    return 10;
}
//
//
//
//TIPUS3D
int tipus3d(struct list *generadors, int n, struct lmin *vmin)
{
    int j, i, m, tip, repetit, tmp;
    int *num, *num1, *num2, *num3;
    double perp[3];
    struct list *w = malloc(sizeof(struct list));
    if(w==NULL){
        printf("Error\n");
    }
    struct lmin *wmin = malloc(sizeof(struct lmin));
    if(wmin==NULL){
        printf("Error\n");
    }
    struct lmin *vcares = malloc(sizeof(struct lmin));
    if(vcares==NULL){
        printf("Error\n");
    }
    wmin->l = malloc(4*(sizeof(int)));
    if(wmin->l==NULL){
        printf("Error\n");
    }    
    vcares->l=malloc(n*(sizeof(int)));    
    w->vect = malloc(3*n*(sizeof(double)));
    if(w->vect==NULL){
        printf("Error\n");
    }
    vcares->len=0;
    w->len = 0;
    wmin->len = 0;
    num = &(vmin->len);
    num1 = &(vcares->len);
    num2 = &(wmin->len);
    num3 = &(w->len);
    if (n == 0)
    {
        free(w->vect);
        free(w);
        free(wmin->l);
        free(wmin);
        free(vcares->l);
        free(vcares);
        return 0; //0
    }
    if (n == 1)
    {
        free(w->vect);
        free(w);
        free(wmin->l);
        free(wmin);
        free(vcares->l);
        free(vcares);
        vmin->l[*num]=0;
        (*num)++;
        return 1; //SEMIRECTA
    }

    m = projectacon(n, generadors->vect[0], generadors, w);
    if (m == 0)
    {
        for (int i = 0; i < n; i++)
        {
            if (dotProduct(generadors->vect[0], generadors->vect[i]) < 0)
            {
                vmin->l[(*num)] = 0;
                (*num)++;
                vmin->l[(*num)] = i;
                (*num)++;
                free(w->vect);
                free(w);
                free(wmin->l);
                free(wmin);
                free(vcares->l);
                free(vcares);
                return 2; //SEMIRECTA
            }
        }
        vmin->l[(*num)] = 0;
        (*num)++;
        free(w->vect);
        free(w);
        free(wmin->l);
        free(wmin);
        free(vcares->l);
        free(vcares);
        return 1; //SEMIRECTA
    }

    tip = tipus2d(w, m, generadors->vect[0], wmin);
    if (tip < 3)
    {
        prodVectorial(generadors->vect[0], w->vect[0], perp);
        free(w->vect);
        free(w);
        free(wmin->l);
        free(wmin);
        free(vcares->l);
        free(vcares);
        return tipus2d(generadors, n, perp, vmin);
    }

    for (int i = 0; i < n; i++)
    {
        w->len = 0;
        m = projectacon(n, generadors->vect[i], generadors, w);
        tip = tipus2d(w, m, generadors->vect[i], wmin);
        wmin->len=0;
        if (tip == 3)
        {
            repetit = 0;
            for (int j = 0; j < (*num); j++)
            {
                int tmp = vmin->l[j];
                if (angle(generadors->vect[i], generadors->vect[tmp]) == 0)
                {
                    repetit = 1;
                }
            }
            if (repetit == 0)
            {
                vmin->l[(*num)] = i;
                (*num)++;
            }
        }
        if (tip == 4)
        {
            vcares->l[(*num1)] = i;
            (*num1)++;
            

        }
    }
    if ((*num) > 2)
    {
        free(w->vect);
        free(w);
        free(wmin->l);
        free(wmin);
        free(vcares->l);
        free(vcares);         
        return 6; //POLIEDRIC
    }
    if ((*num) == 2)
    {
        tmp = vmin->l[0];
        for (int i = 0; i < 3; i++)
        {
            perp[i] = generadors->vect[tmp][i];
        }
        tmp = vcares->l[0];
        vmin->l[(*num)] = tmp;
        (*num)++;
        projOrtogonal(perp, generadors->vect[tmp], w->vect[0]);
        for (int i = 0; i < (*num1); i++)
        {
            tmp = vcares->l[i];
            projOrtogonal(perp, generadors->vect[tmp], w->vect[1]);
            if (angle(w->vect[0], w->vect[1]) > 0)
            {
                
                vmin->l[(*num)] = vcares->l[i];
                (*num)++;
                free(w->vect);
                w->vect=NULL;
                free(w);
                free(wmin->l);
                free(wmin);
                free(vcares->l);
                free(vcares);               
                return 7;
            }
        }
    }
    if ((*num1) > 0)
    {
        w->len = 0;
        wmin->len = 0;
        j = n - 1;
        for (int i = 0; i < (*num1); i++)
        {
            tmp = vcares->l[i];
            w->vect[(*num3)][0] = generadors->vect[tmp][0];
            w->vect[(*num3)][1] = generadors->vect[tmp][1];
            w->vect[(*num3)][2] = generadors->vect[tmp][02];
            w->len++;
            if (angle(w->vect[0], w->vect[i]) > tol && angle(w->vect[0], w->vect[i]) < pi)
            {
                prodVectorial(w->vect[0], w->vect[i], perp);
            }
            if ((j == n - 1) && (vcares->l[i] > i))
            {
                j = i;
            }
        }
        tip = tipus2d(w, w->len, perp, wmin);
        for (int i = 0; i < (*num2); i++)
        {
            int tmp2 = wmin->l[i];
            int tmp1 = vcares->l[tmp2];
            vmin->l[i] = tmp1;
            (*num)++;
        }
        vmin->l[(*num)] = j;
        (*num)++;
        free(w->vect);
        free(w);
        free(wmin->l);
        free(wmin);
        free(vcares->l);
        free(vcares);  
        return 8;
    }
    w->len = 0;
    for (int i = 1; i < n; i++)
    {
        w->vect[*num3][0] = generadors->vect[i][0];
        w->vect[*num3][1] = generadors->vect[i][1];
        w->vect[*num3][2] = generadors->vect[i][2];
        (*num3)++;
    }
    i = 0;
    while (tipus3d(w, *num3, vmin) < 9)
    {
            imprimir(generadors->len,generadors);

        w->vect[i][0] = generadors->vect[i][0];
        w->vect[i][1] = generadors->vect[i][1];
        w->vect[i][2] = generadors->vect[i][2];
        i++;

        if (i == n)
        {
            vmin->l[(*num)] = n - 1;
            (*num)++;
            free(w->vect);
            free(w);
            free(wmin->l);
            free(wmin);
            free(vcares->l);
            free(vcares);  
            return 9;
        }

        vmin->len = 0;
    }
    for (int j = i; j < (*num); j++)
    {
        vmin->l[j] = vmin->l[j] + 1;
            (*num)++;
    }
    free(w->vect);
    free(w);
    free(wmin->l);
    free(wmin);
    free(vcares->l);
    free(vcares);  
    return 9;
}
//
//
//
//TIPUS 2D
int tipus2d(struct list *generadors, int n, double perp[], struct lmin *vmin)
{

    if (perp[0] == 0 && perp[1] == 0 && perp[2] == 0)
    {
        printf("\n\n\nEL PERP ES ZERO NO FUNCIONARA BE TIPUS 2D\n");
    }
    int dr, esq;
    bool zero = true;
    double anglemax;
    double(*A)[n];
    int *num;
    num = &(vmin->len);
    A = malloc((n * n) * sizeof(double));
    if (A == NULL)
    {
        printf("No hi ha prou memoria\n");
        return 51;
    }
    if (n == 0)
    {
        free(A);
        return 0; //CON 0
    }
    if (n == 1)
    {
        vmin->l[(*num)] = 0;
        (*num)++;
        free(A);
        return 1; //SEMIRECTA
    }
    dr = 0;
    esq = 1;
    anglemax = sangle(generadors->vect[0], generadors->vect[1], perp);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = sangle(generadors->vect[i], generadors->vect[j], perp);

            if (A[i][j] > anglemax)
            {
                anglemax = A[i][j];
                dr = i;
                esq = j;
            }
            if (!(A[i][j] > -tol && A[i][j] < tol) && !(A[i][j] > pi - tol && A[i][j] < pi + tol))
            {
                zero = false;
            }
        }
    }
    if (anglemax == 0)
    {
        vmin->l[(*num)] = 0;
        (*num)++;
        free(A);
        return 1; //SEMIRECTA
    }
    if (anglemax < pi)
    {
        for (int i = 0; i < n; i++)
        {
            if (sangle(generadors->vect[dr], generadors->vect[i], perp) < 0)
            {
                vmin->l[(*num)] = dr;
                (*num)++;
                vmin->l[(*num)] = esq;
                (*num)++;
                vmin->l[(*num)] = i;
                (*num)++;
                return 5; //ES UN PLA
            }
        }

        vmin->l[(*num)] = dr;
        (*num)++;
        vmin->l[(*num)] = esq;
        (*num)++;
        free(A);
        return 3; //ANGLE PLA
    }
    if (zero)
    {
        vmin->l[(*num)] = dr;
        (*num)++;
        vmin->l[(*num)] = esq;
        (*num)++;
        free(A);
        return 2; //RECTA
    }

    bool bol1 = false;
    int up[n], down[n];
    int lup = 0, ldown = 0;
    bol1 = false;
    bool bol2 = false;
    int t1, t2;
    for (int i = 0; i < n; i++)
    {

        if (!(A[dr][i]>-tol&&A[dr][i]<tol) && !(A[esq][i]>-tol&&A[esq][i]<tol))
        {
            if (A[dr][i] > 0 && A[esq][i] < 0)
            {
                bol1 = true;
                up[lup] = i;
                lup++;
            }
            if (A[esq][i] > 0 && A[dr][i] < 0)
            {
                bol2 = true;
                down[ldown] = i;
                ldown++;
            }
        }
    }
    if (bol1 && (!bol2))
    {
        vmin->l[(*num)] = dr;
        (*num)++;
        vmin->l[(*num)] = esq;
        (*num)++;
        vmin->l[*num] = up[0];
        (*num)++;
        free(A);
        return 4;
    }
    if (bol2 && (!bol1))
    {
        vmin->l[(*num)] = dr;
        (*num)++;
        vmin->l[(*num)] = esq;
        (*num)++;
        vmin->l[*num] = down[0];
        (*num)++;
        free(A);
        return 4;
    }

    for (int i = 0; i < lup; i++)
    {
        t1 = up[i];

        if (!(A[t1][dr]>-tol&&A[t1][dr]<tol) && !(A[t1][esq]>-tol&&A[t1][esq]<tol))
        {

            for (int j = 0; j < ldown; j++)
            {
                t2 = down[j];

                if (!(A[t2][dr]>-tol&&A[t2][dr]<tol) && !(A[t2][esq]>-tol&&A[t2][esq]<tol))
                {
                    if (0 < A[t1][t2] && A[t1][t2] < pi)
                    {
                        vmin->l[(*num)] = dr;
                        (*num)++;
                        vmin->l[(*num)] = t1;
                        (*num)++;
                        vmin->l[(*num)] = t2;
                        (*num)++;
                        free(A);
                        return 5;
                    }
                    if (0 < A[t2][t1] && A[t2][t1] < pi)
                    {
                        vmin->l[(*num)] = esq;
                        (*num)++;
                        vmin->l[(*num)] = t1;
                        (*num)++;
                        vmin->l[(*num)] = t2;
                        (*num)++;
                        free(A);
                        return 5;
                    }
                    if (fabs(A[t1][t2]) > pi - tol && fabs(A[t1][t2]) < pi + tol)
                    {
                        vmin->l[(*num)] = esq;
                        (*num)++;
                        vmin->l[(*num)] = dr;
                        (*num)++;
                        vmin->l[(*num)] = t1;
                        (*num)++;
                        vmin->l[(*num)] = t2;
                        (*num)++;
                        free(A);
                        return 5;
                    }
                }
            }
        }
    }

    printf("Tipus2D no ha funcionat correctament\n");
    free(A);
    return 66;
}
//
//
//
//FUNCIONS INTERMIG
int projectacon(int n, double u[], struct list *v, struct list *w)
{
    double t[3]; //temporal
    int nnuls = 0;
    int *lw;
    lw = &(w->len);
    for (int i = 0; i < n; i++)
    {
        projOrtogonal(u, v->vect[i], t);
        for (int j = 0; j < 3; j++)
        {

            if (t[j] < tol && t[j] > -tol)
            {
                t[j] = 0;
            }
        }
        if (t[0] != 0 || t[1] != 0 || t[2] != 0)
        {
            nnuls++;
            w->vect[(*lw)][0] = t[0];
            w->vect[(*lw)][1] = t[1];
            w->vect[(*lw)][2] = t[2];
            (*lw)++;
        }
    }
    return nnuls;
}
//
//
//
//FUNCIONS DE MANIPULACIO DE VECTORS
//LINIES
int linies(char *name)
{
    double a, b, c;
    int n = 0;
    FILE *file;
    file = fopen(name, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error");
        return 1;
    }
    while (EOF != fscanf(file, "%lf, %lf, %lf,", &a, &b, &c))
    {
        if (a != 0 || b != 0 || c != 0)
        {
            n++;
        }
    }
    fclose(file);
    return n;
}
//
//
//
//LLEGIR VECTOR
bool llegirVectors(char *name, int n, struct list *llista)
{
    bool zero = false;
    double a, b, c;
    int fila = 0;
    llista->len = n;
    FILE *file;
    file = fopen(name, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error al obrir el fitxer\n");
        return 52;
    }
    while (EOF != fscanf(file, "%lf, %lf, %lf,", &a, &b, &c))
    {
        if (a != 0 || b != 0 || c != 0)
        {
            llista->vect[fila][0] = a;
            llista->vect[fila][1] = b;
            llista->vect[fila][2] = c;
            fila++;
        }
        if (a == 0 && b == 0 && c == 0)
        {
            zero = true;
        }
    }
    fclose(file);
    return zero;
}
//IMPRIMIR VECTORS (NO MINIMS)
void imprimir(int n, struct list *llista)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("\t V[%i]->\t{", i);
        for (int j = 0; j < 3; j++)
        {
            printf("%g ", llista->vect[i][j]);
        }
        printf("}\n");
    }
}
//IMPRIMIR MIN
void imprimirMin(struct list *llista, struct lmin *vmin)
{
    int *num;
    num = &(vmin->len);
    for (int i = 0; i < (*num); i++)
    {
        int ap = vmin->l[i];
        printf("\tV[%i]={%g, %g, %g}\n", ap, llista->vect[ap][0], llista->vect[ap][1], llista->vect[ap][2]);
    }
}
//
//
void sortida(char *name, struct list *llista, struct lmin *vmin)
{
    FILE *file;
    file = fopen(name, "w+");
    if (file == NULL)
    {
        fprintf(stderr, "Error al obrir el fitxer\n");
    }
    int *num;
    num = &(vmin->len);
    for (int i = 0; i < (*num); i++)
    {
        int ap = vmin->l[i];
        fprintf(file, "%g, %g, %g", llista->vect[ap][0], llista->vect[ap][1], llista->vect[ap][2]);
    }
    fclose(file);
}

//
//
//
//OPERACIONS VECTORS BASIQUES
//NORMA
double norma(double v1[])
{
    double sqrtv = 0;
    for (int i = 0; i < 3; i++)
    {
        sqrtv = sqrtv + (v1[i]) * (v1[i]);
    }
    return sqrt(sqrtv);
}
//
//
//BASE
bool base(double v1[], double v2[], double v3[])
{
    if (det(v1, v2, v3) > -tol && det(v1, v2, v3) < tol)
    {
        return false;
    }
    else
    {
        return true;
    }
}
//
//
//DOTPRODUCT
double dotProduct(double v1[], double v2[])
{
    double product = 0;
    for (int i = 0; i < 3; i++)
    {
        product = product + v1[i] * v2[i];
    }
    return product;
}
//DETERMINANT
double det(double v1[], double v2[], double v3[])
{
    double det = v1[0] * v2[1] * v3[2] + v1[1] * v2[2] * v3[0] + v1[2] * v2[0] * v3[1] - v1[2] * v2[1] * v3[0] - v1[1] * v2[0] * v3[2] - v1[0] * v2[2] * v3[1];
    return det;
}
//PRODUCTE VECTORIAL
void prodVectorial(double v1[], double v2[], double v3[])
{
    v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
    v3[1] = v1[2] * v2[0] - v1[0] * v2[2];
    v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
}
//PROJECCIO ORTOG
void projOrtogonal(double u[], double v[], double p[])
{

    for (int i = 0; i < 3; i++)
    {
        p[i] = v[i] - ((dotProduct(u, v)) / (dotProduct(u, u)) * u[i]);
    }
}
//SIGNE ANGLE
double sangle(double v1[], double v2[], double v3[])
{
    double angle;
    double tmp = dotProduct(v1, v2) / (norma(v1) * norma(v2));
    if (tmp > 1 && tmp < 1 + tol)
    {
        tmp = 1;
    }
    if (tmp < -1 && tmp > -1 - tol)
    {
        tmp = -1;
    }
    if (tmp > 1 + tol){
        tmp=1;
    } 
    if(tmp< -1 - tol)
    {
        tmp=-1;
    }
    angle = (acos(tmp));
    if (angle < -tol)
    {
        angle = -angle;
    }
    if (angle > -tol && angle < tol)
    {
        angle = 0;
    }
    if (angle < pi + tol && angle > pi - tol)
    {
        angle = pi;
    }
    if (angle < (2 * pi + tol) && angle > (2 * pi - tol))
    {
        angle = 0;
    }
    if (det(v3, v1, v2) < -tol)
    {
        angle = -angle;
    }
    return angle;
}
//ANGLE
double angle(double v1[], double v2[])
{
    double angle;
    double tmp = dotProduct(v1, v2) / (norma(v1) * norma(v2));
    if (tmp > 1 && tmp < 1 + tol)
    {
        tmp = 1;
    }
    if (tmp < -1 && tmp > -1 - tol)
    {
        tmp = -1;
    }
    if (tmp > 1 + tol){
        tmp=1;
    } 
    if(tmp< -1 - tol)
    {
        tmp=-1;
    }
    angle = (acos(tmp));
    if (angle < -tol)
    {
        angle = -angle;
    }
    if (angle > -tol && angle < tol)
    {
        angle = 0;
    }
    if (angle < pi + tol && angle > pi - tol)
    {
        angle = pi;
    }
    if (angle < (2 * pi + tol) && angle > (2 * pi - tol))
    {
        angle = 0;
    }
    return angle;
}
